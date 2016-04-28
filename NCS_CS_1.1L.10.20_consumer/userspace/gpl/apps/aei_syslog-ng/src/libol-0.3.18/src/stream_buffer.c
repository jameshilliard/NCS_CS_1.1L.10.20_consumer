/***************************************************************************
 *
 * Copyright (c) 1998-1999 Niels Möller
 * Copyright (c) 1999 BalaBit Computing
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: stream_buffer.c,v 1.6 2004/08/05 10:34:49 bazsi Exp $
 *
 ***************************************************************************/
 

#include "stream_buffer.h"

#include "xalloc.h"
#include "werror.h"
#include "queue.h"
#include "io.h"
#include "format.h"

#include <assert.h>
#include <string.h>


/* For the packet queue */
/* NOTE: No object header */
struct buffer_node
{
	struct ol_queue_node header;
	struct ol_string *packet;
};

/* CLASS:
   (class
     (name stream_buffer)
     (super abstract_buffer)
     (vars
       (block_size simple UINT32)
       (buffer space UINT8)        ; Size is twice the blocksize 
       (empty simple int)

       ; Total amount of data currently in the buffer)
       (length . UINT32)
       
       ;; (try_write simple int)

       (q special-struct "struct ol_queue"
                     #f do_free_buffer)

       (pos simple UINT32)        ; Partial packet
       (partial string)

       (start simple UINT32)
       (end simple UINT32)))
*/


/* Prototype */
static void do_free_buffer(struct ol_queue *q);

#include "stream_buffer.c.x"

static void do_free_buffer(struct ol_queue *q)
{
	FOR_QUEUE(q, struct buffer_node *, n) {
		ol_string_free(n->packet);
		ol_space_free(n);
	}
}

/* FIXME: this could be very well be optimized */
static int do_write(struct abstract_write *w,
		    UINT32 length, UINT8 *data)
{
	return A_WRITE_STRING(w, c_format("%s", length, data));
}

static int do_write_str(struct abstract_write *c, struct ol_string *string)
{
        CAST(stream_buffer, closure, c);
        struct buffer_node *new;

        /* debug("stream_buffer: do_write length = %i\n", 
	      	packet->length); */
        if (!string->length) {
		return ST_OK | ST_GOON;
	}

        if (closure->super.closed) {
		return ST_FAIL | ST_CLOSE;
	}
        
        /* Enqueue packet */
        NEW_SPACE(new);
        new->packet = string;

        ol_queue_add_tail(&closure->q, &new->header);

        closure->empty = 0;
        closure->length += string->length;

/*        debug("stream_buffer: do_write closure->length = %i\n",
      		closure->length); */
        
        return ST_OK | ST_GOON;
}

/* Copy data as necessary, before writing.
 *
 * FIXME: Writing of large packets could probably be optimized by
 * avoiding copying it into the buffer.
 *
 * Returns 1 if the buffer is non-empty. */
int do_prepare_write(struct abstract_buffer *c)
{
	CAST(stream_buffer, buffer, c);
	UINT32 length = buffer->end - buffer->start;

	if (buffer->empty)
		return 0;
  
	if (buffer->start > buffer->block_size) {
		/* Copy contents to the start of the buffer */
		memcpy(buffer->buffer, buffer->buffer + buffer->start, length);
		buffer->start = 0;
		buffer->end = length;
	}

	while (length < buffer->block_size) {
		/* Copy more data into buffer */
		if (buffer->partial) {
			UINT32 partial_left = buffer->partial->length - buffer->pos;
			UINT32 buffer_left = 2*buffer->block_size - buffer->end;
			if (partial_left <= buffer_left) {
				/* The rest of the partial packet fits in the buffer */
				memcpy(buffer->buffer + buffer->end,
					buffer->partial->data + buffer->pos,
					partial_left);

				buffer->end += partial_left;
				length += partial_left;
	      
				ol_string_free(buffer->partial);
				buffer->partial = NULL;
			}
			else {
				memcpy(buffer->buffer + buffer->end,
					buffer->partial->data + buffer->pos,
					buffer_left);

				buffer->end += buffer_left;
				length += buffer_left;
				buffer->pos += buffer_left;

				assert(length >= buffer->block_size);
			}
		}
		else {
			/* Dequeue a packet, if possible */
			if (!ol_queue_is_empty(&buffer->q)) {	    
				struct buffer_node *n =
					(struct buffer_node *) ol_queue_remove_head(&buffer->q);
	      
				buffer->partial = n->packet;
				buffer->pos = 0;

				ol_space_free(n);
			}
			else
				break;
		}
	}
	
	buffer->empty = !length;
	return !buffer->empty;
}

static int do_flush(struct abstract_buffer *c, struct abstract_write *w)
{
	CAST(stream_buffer, self, c);
	UINT32 size;
	int res;
	
	size = LIBOL_MIN(self->end - self->start, self->block_size);
               
	res = A_WRITE(w, size, self->buffer + self->start);
	if (res >= 0) {
		self->start += res;
		assert(self->start <= self->end);
		self->length -= res;
	}
	return ST_OK | ST_GOON;
}

void do_close(struct abstract_buffer *c)
{
	c->closed = 1;
}
  
struct abstract_buffer *make_stream_buffer(UINT32 bufsize)
{
	NEW(stream_buffer, res);
  
	res->super.super.write = do_write;
	res->super.super.writestr = do_write_str;
	res->super.flush = do_flush;
	res->super.prepare = do_prepare_write;
	res->super.close = do_close;
	res->block_size = bufsize;
	res->buffer = ol_space_alloc(2 * bufsize);
	res->empty = 1;
	res->length = 0;

	ol_queue_init(&res->q);
	res->pos = 0;
	res->partial = NULL;
	res->start = res->end = 0;

	return &res->super;
}
