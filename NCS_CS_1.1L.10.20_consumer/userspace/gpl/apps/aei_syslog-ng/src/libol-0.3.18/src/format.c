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
 * $Id: format.c,v 1.10 1999/11/22 18:26:24 bazsi Exp $
 *
 ***************************************************************************/

#include "format.h"

#include "list.h"
#include "werror.h"
#include "xalloc.h"

#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


struct ol_string *c_format(const char *format, ...);

struct ol_string *c_format(const char *format, ...)
{
  va_list args;
  UINT32 length;
  struct ol_string *packet;

  va_start(args, format);
  length = c_vformat_length(format, args);
  va_end(args);

  packet = ol_string_alloc(length);

  va_start(args, format);
  c_vformat_write(format, length, packet->data, args);
  va_end(args);

  return packet;
}

UINT32 c_format_length(const char *format, ...)
{
  va_list args;
  UINT32 length;

  va_start(args, format);
  length = c_vformat_length(format, args);
  va_end(args);

  return length;
}

UINT32 c_format_write(const char *format, UINT32 length, UINT8 *buffer, ...)
{
  va_list args;
  UINT32 res;
  
  va_start(args, buffer);
  res = c_vformat_write(format, length, buffer, args);
  va_end(args);
  return res;
}

#if 0     
static int write_decimal_length(UINT8 *buffer, UINT32 n);
#endif

UINT32 c_vformat_length(const char *f, va_list args)
{
  UINT32 length = 0;

  while(*f)
    {
      if (*f == '%')
	{
	  int do_hex = 0;
	  int fieldlen = 0;
	  while(*++f)
	    {
	      switch (*f)
		{
		case 'f':
		  /* Do nothing */
		  break;
		case 'x':
		  do_hex = 1;
		  break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		  fieldlen = 10*fieldlen + (*f)-'0';
		  break;
		default:
		  goto end_options;
		}
	    }
end_options:

	  switch(*f)
	    {
	    default:
	      fatal("c_vformat_length: bad format string");
	      break;

	    case 'c':
	      (void) va_arg(args, int);
	      /* Fall through */
	    case '%':
	      f++;
	      length++;
	      break;

	    case 'i':
	      {
	        UINT32 n = va_arg(args, UINT32);
	        f++;
                length += fieldlen 
		  ? fieldlen 
		  : (do_hex ? format_size_in_hex(n) : format_size_in_decimal(n));
  	        break;
	      }
	    case 's':
	      {
		UINT32 l = va_arg(args, UINT32); /* String length */ 
		(void) va_arg(args, UINT8 *);    /* data */

		f++;

		length += l;

		break;
	      }
	    case 'S':
	      {
		struct ol_string *s = va_arg(args, struct ol_string *);
		length += s ? s->length : 6;
		f++;
		
		break;
	      }
            case 'I':
              {
                char *s;

                s = inet_ntoa(va_arg(args, struct in_addr));
                length += strlen(s);
		f++;

                break;
              }
	    case 'z':
	      {
	        char *p = va_arg(args, char *);
		unsigned l = p ? strlen(p) : 6;
		length += l;

		f++;

		break;
	      }
	    case 'r':
	      {
		UINT32 l = va_arg(args, UINT32); 
		length += l;
		(void) va_arg(args, UINT8 **);    /* pointer */

		f++;

		break;
	      }
	    }
	}
      else
	{
	  length++;
	  f++;
	}
    }
  return length;
}

UINT32 c_vformat_write(const char *f, UINT32 size, UINT8 *buffer, va_list args)
{
  UINT8 *start = buffer;
  
  while(*f)
    {
      if (*f == '%')
	{
	  int do_free = 0;
	  int do_hex = 0;
	  int fieldlen = 0;
	  int zeropad = 0;
	  int first = 1;
	  
	  while(*++f)
	    {
	      switch (*f)
		{
		case 'f':
		  do_free = 1;
		  break;
		case 'x':
		  do_hex = 1;
		case '0':
		  if (first) zeropad = 1;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		  first = 0;
		  fieldlen = 10*fieldlen + (*f)-'0';
		  break;
		default:
		  goto end_options;
		}
	    }
end_options:
		  
	  switch(*f)
	    {
	    default:
	      fatal("c_vformat_write: bad format string");
	      break;

	    case 'c':
	      *buffer++ = va_arg(args, int);
	      f++;

	      break;
	    case '%':
	      *buffer++ = '%';
	      f++;

	      break;

	    case 'i':
	      {
	      	UINT32 length;
		UINT32 n = va_arg(args, UINT32);

		if (!do_hex) {		
		  unsigned i;

		  length = fieldlen ? fieldlen : format_size_in_decimal(n);
		  if (n) 
		    {
		      for (i = 0; i < length; i++)
			{
			  buffer[length - i - 1] = n ? '0' + n % 10 : zeropad ? '0' : ' ';
			  n /= 10;
			}
		    }
		  else
		    buffer[0] = '0';

		}
		else {
		  char hexchars[] = "0123456789abcdef";
		  unsigned i;
		  
		  length = format_size_in_hex(n);
		  for (i = 0; i < length; i++)
		    {
		      buffer[length - i - 1] = hexchars[n & 0xf];
		      n >>= 4;
		    }
		}
  		buffer += length;
		f++;
		break;
	      }
	    case 's':
	      {
		UINT32 length = va_arg(args, UINT32);
		UINT8 *data = va_arg(args, UINT8 *);

		memcpy(buffer, data, length);
		buffer += length;
		f++;

		break;
	      }
	    case 'S':
	      {
		struct ol_string *s = va_arg(args, struct ol_string *);
		if (s) 
		  {
		    memcpy(buffer, s->data, s->length);
		    buffer += s->length;

		    if (do_free)
		      ol_string_free(s);
		  }
		else
		  {
		    memcpy(buffer, "(NULL)", 6);
		    buffer += 6;
		  }
		f++;

		break;
	      }
            case 'I':
              {
                char *s;

                s = inet_ntoa(va_arg(args, struct in_addr));
                memcpy(buffer, s, strlen(s));
		buffer += strlen(s);
		f++;
                break;
              }
	    case 'z':
	      {
		char *s = va_arg(args, char *);
		UINT32 length = s ? strlen(s) : 6;

		if (s)
		  memcpy(buffer, s, length);
		else
		  memcpy(buffer, "(NULL)", 6);
		buffer += length;
		f++;

		break;
	      }
	    case 'r':
	      {
		UINT32 length = va_arg(args, UINT32);
		UINT8 **p = va_arg(args, UINT8 **);

		if (p)
		  *p = buffer;
		buffer += length;
		f++;

		break;
	      }
	    
	    }
	}
      else
	{
	  *buffer++ = *f++;
	}
    }
  
  assert(buffer <= start + size);
  return buffer - start;
}

UINT32 format_size_in_decimal(UINT32 n)
{
  int i;
  int e;
  
  /* Table of 10^(2^n) */
  static const UINT32 powers[] = { 10UL, 100UL, 10000UL, 100000000UL };

#define SIZE (sizeof(powers) / sizeof(powers[0])) 

  /* Determine the smallest e such that n < 10^e */
  for (i = SIZE - 1 , e = 0; i >= 0; i--)
    {
      if (n >= powers[i])
	{
	  e += 1UL << i;
	  n /= powers[i];
	}
    }

#undef SIZE
  
  return e+1;
}

UINT32 format_size_in_hex(UINT32 n)
{
  UINT32 mask = 0xf0000000;
  UINT32 size = 8;
  while (n & mask) {
  	size--;
  	mask >>= 4;
  }
  return size;
}

#if 0
static int write_decimal_length(UINT8 *buffer, UINT32 n)
{
  int length = format_size_in_decimal(n);
  int i;
  
  for (i = 0; i<length; i++)
    {
      buffer[length - i - 1] = '0' + n % 10;
      n /= 10;
    }

  buffer[length] = ':';

  return length + 1;
}
#endif

/* These functions add an extra NUL-character at the end of the string
 * (not included in the length), to make it possible to pass the
 * string directly to C library functions. */

struct ol_string *format_cstring(const char *s)
{
  if (s)
    {
      struct ol_string *res = c_format("%z%c", s, 0);
      res->length--;
      return res;
    }
  return NULL; 
}

struct ol_string *make_cstring(struct ol_string *s, int free)
{
  struct ol_string *res;
  
  if (memchr(s->data, '\0', s->length))
    {
      if (free)
	ol_string_free(s);
      return 0;
    }

  res = c_format("%S%c", s, 0);
  res->length--;
  
  if (free)
    ol_string_free(s);
  return res;
}

struct ol_string *c_format_cstring(const char *format, ...)
{
  va_list args;
  UINT32 length;
  struct ol_string *packet;

  va_start(args, format);
  length = c_vformat_length(format, args);
  va_end(args);

  packet = ol_string_alloc(length + 1);
  packet->length--;

  va_start(args, format);
  c_vformat_write(format, length, packet->data, args);
  va_end(args);
  packet->data[length] = 0;

  return packet;
}
