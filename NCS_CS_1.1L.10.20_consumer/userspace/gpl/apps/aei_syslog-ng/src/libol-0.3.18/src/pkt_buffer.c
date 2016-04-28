/***************************************************************************
 *
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
 * Inspired by nsyslog, originally written by Darren Reed.
 *
 * $Id: pkt_buffer.c,v 1.20 2005/11/23 08:49:21 bazsi Exp $
 *
 ***************************************************************************/
 
#include "pkt_buffer.h"
#include "list.h"
#include "io.h"
#include "format.h"

#include <string.h>

struct buffer_node {
	struct ol_queue_node header;
	struct ol_string *packet;
};

int pktbuf_dropped_pkts = 0;

/* CLASS:
     (class
       (name pkt_buffer)
       (super abstract_buffer)
       (vars
         (queue special-struct "struct ol_queue" #f do_free_buffer)
	 (pkt_flush simple int)
	 (queue_min simple int)
         (queue_size simple int)
         (queue_max simple int)))
*/

static void do_free_buffer(struct ol_queue *q);

#include "pkt_buffer.c.x"

static void do_free_buffer(struct ol_queue *q)
{
        FOR_QUEUE(q, struct buffer_node *, n) {
                ol_string_free(n->packet);
                ol_space_free(n);
        }
}

static int do_write(struct abstract_write *c, UINT32 length, UINT8 *data)
{
	return A_WRITE_STRING(c, c_format("%s", length, data));
}

static int do_write_str(struct abstract_write *c, struct ol_string *string)
{
	CAST(pkt_buffer, self, c);
	
	if (self->super.closed) {
		ol_string_free(string);
		return ST_FAIL | ST_CLOSE;
	}
	if (self->queue_size == self->queue_max) {
		/* fifo full */
		pktbuf_dropped_pkts++;
		ol_string_free(string);
		return ST_FAIL | ST_OK;
	}
	else {
		struct buffer_node *item;
		
		NEW_SPACE(item);
		item->packet = string;
		ol_queue_add_tail(&self->queue, &item->header);
		if (++self->queue_size == self->queue_max && self->super.writable)
			(*self->super.writable) = 0;
	}
	return ST_OK | ST_GOON;
}

static int do_flush_stream(struct abstract_buffer *c, struct abstract_write *w)
{
	CAST(pkt_buffer, self, c); 
	char buffer[4096];
	int res;
	int end;
	
	do {
		end = 0;
		{
			FOR_QUEUE(&self->queue, struct buffer_node *, item) {
				int avail = sizeof(buffer) - end;
				int move = LIBOL_MIN(item->packet->length, avail);
				
				if (avail <= 0 || move <= 0)
					break;
				memcpy(&buffer[end], item->packet->data, move);
				end += move;
			
				if (item->packet->length != move) {
					struct ol_string *s;
					
					s = item->packet;
					item->packet = c_format("%s", s->length - move, s->data + move);
					ol_string_free(s);
				} 
				else {
					
					/* successfully moved */
					self->queue_size--;
					ol_queue_remove((struct ol_queue_node *) item);
					ol_string_free(item->packet);
					ol_space_free(item);
				}
			}
		}
		
		if (end == 0)
			break;
			
		res = A_WRITE(w, end, (UINT8 *) buffer);
		
		if (res >= 0) {
			if (end == res) {
				if (self->super.writable)
					(*self->super.writable) = 1;
			}
			else {
				/* this is slow, because of another memory move
				 * but this is run rarely anyway */
				struct buffer_node *item;
				
				NEW_SPACE(item);
				item->packet = c_format("%s", end - res, buffer + res);
				ol_queue_add_head(&self->queue, &item->header);
				if (++self->queue_size == self->queue_max && self->super.writable)
					(*self->super.writable) = 0;
				break;
			}
		}
		else {
			struct buffer_node *item;
			
			NEW_SPACE(item);
			item->packet = c_format("%s", end, buffer);
			ol_queue_add_head(&self->queue, &item->header);
			if (++self->queue_size == self->queue_max && self->super.writable)
				(*self->super.writable) = 0;
			
			werror("pkt_buffer::do_flush(): Error flushing data\n");
			return ST_DIE;
		}
	}
	while (1);
	return ST_OK | ST_GOON;
}

static int do_flush_pkt(struct abstract_buffer *c, struct abstract_write *w)
{
 	CAST(pkt_buffer, self, c);
	int res;

	FOR_QUEUE(&self->queue, struct buffer_node *, item) {

		res = A_WRITE(w, item->packet->length, item->packet->data);
		if (res >= 0) {
                        if (item->packet->length == res) {
                                self->queue_size--;
                                ol_queue_remove((struct ol_queue_node *) item);
                                ol_string_free(item->packet);
                                ol_space_free(item);
                                if (self->super.writable)
                                        (*self->super.writable) = 1;
			}
                        else if (res != 0) {
                                struct ol_string *s;
                                s = item->packet;
                                item->packet = c_format("%s", s->length - res, s->data + res);
                                ol_string_free(s);
                                break;
			}
                        else {
                              	break;
			}
		}
                else {
                      	verbose("pkt_buffer::do_flush(): Error flushing data\n");
			return ST_DIE;
		}
	}
        return ST_OK | ST_GOON;
}

static int do_flush(struct abstract_buffer *c, struct abstract_write *w)
{
	CAST(pkt_buffer, self, c);

	if (self->pkt_flush)
		return do_flush_pkt(c, w);
	else
		return do_flush_stream(c, w);
}


static int do_prepare_write(struct abstract_buffer *c)
{
	CAST(pkt_buffer, self, c);
	
	return self->queue_size > self->queue_min;
}

static void do_close(struct abstract_buffer *c)
{
	CAST(pkt_buffer, self, c);
	if (c->writable)
		*c->writable = 0;
	c->closed = 1;
	self->queue_min = 0;
}

struct abstract_buffer *make_pkt_buffer(int queue_max)
{
	NEW(pkt_buffer, self);
	
	self->super.super.write = do_write;
	self->super.super.writestr = do_write_str;
	self->super.flush = do_flush;
	self->super.prepare = do_prepare_write;
	self->super.close = do_close;
	
	ol_queue_init(&self->queue);
	self->queue_min = 0;
	self->queue_size = 0;
	self->queue_max = queue_max;
	
	return &self->super;
}

struct abstract_buffer *make_pkt_buffer_ext(int queue_min, int queue_max, int pkt_flush)
{
	struct abstract_buffer *b = make_pkt_buffer(queue_max);
	((struct pkt_buffer *) b)->queue_min = queue_min;
	((struct pkt_buffer *) b)->pkt_flush = pkt_flush;

	return b;
}
