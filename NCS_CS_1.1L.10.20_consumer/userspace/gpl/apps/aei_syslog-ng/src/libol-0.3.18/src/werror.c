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
 * $Id: werror.c,v 1.12 2005/05/03 15:49:35 bazsi Exp $
 *
 ***************************************************************************/

#include "werror.h"

#include "format.h" 
#include "gc.h"
#include "io.h"
#include "xalloc.h"
#include <netinet/in.h>
#include <arpa/inet.h>

#include <assert.h>
/* #include <stdio.h> */
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if HAVE_SYSLOG_H
#include <syslog.h>
#endif

#if HAVE_ALLOCA_H
#include <alloca.h>
#endif

int debug_flag = 0;
int quiet_flag = 0;
int verbose_flag = 0;
int syslog_flag = 0;

int error_fd = STDERR_FILENO;

#define MAX_BUF_SIZE 65536

static int write_file(int level, UINT32 length, UINT8 *data);

int (*error_write)(int level, UINT32 length, UINT8 *data) = write_file;

#if HAVE_SYSLOG
static int write_syslog(int level UNUSED, UINT32 length, UINT8 *data)
{
  UINT8 string_buffer[length + 1];
  UINT32 to_copy;
  
  /* Make sure the message is properly terminated with \0. */
  /* snprintf(string_buffer, (BUF_SIZE > length) ? BUF_SIZE : length, "%s", data); */
  to_copy = LIBOL_MIN(length, sizeof(string_buffer) - 1);
  memcpy(string_buffer, data, to_copy);
  string_buffer[to_copy] = 0;

  /* FIXME: Should we use different log levels for werror, verbose and
   * debug? */
  
  syslog(LOG_NOTICE, "%s", string_buffer);

  return 0; /* FIXME */
}

void set_error_syslog(const char *progname)
{
  error_write = write_syslog;
  openlog(progname, LOG_PID, LOG_DAEMON);
  error_fd = -1;
}

#endif /* HAVE_SYSLOG */

static int write_ignore(int level,
			UINT32 length UNUSED, UINT8 *data UNUSED)
{
  return 1;
}

static int (*fd_write)(int fd, UINT32 length, UINT8 *data) = write_raw;

static int write_file(int level, UINT32 length, UINT8 *data)
{
  return fd_write(error_fd, length, data);
}

void set_error_stream(int fd, int with_poll)
{
  error_fd = fd;

  error_write = write_file;
  fd_write = with_poll ? write_raw_with_poll : write_raw;
}

void set_error_ignore(void)
{
  error_write = write_ignore;
}

#define MSG_WRITE(v, l, d) (error_write((v), (l), (d)))

void msg_vformat(int level, const char *f, va_list args)
{
	UINT32 length;
	va_list args_copy;
	
	/* NOTE: some platforms require that you iterate over va_list
	 * once, at least according to Aaron Schrab, therefore we
	 * need to copy it first.
	 */
	
	va_copy(args_copy, args);
	
	length = c_vformat_length(f, args_copy);
	if (length <= MAX_BUF_SIZE) {
		char errorbuf[length];
		
		c_vformat_write(f, sizeof(errorbuf), (UINT8 *) errorbuf, args);
		MSG_WRITE(level, length, (UINT8 *) errorbuf);
	}
	else {
		fatal("Internal error, too long message to werror()");
	}
	va_end(args_copy);
}

void werror(const char *format, ...) 
{
  va_list args;

  if (!quiet_flag)
    {
      va_start(args, format);
      msg_vformat(MSG_ERROR, format, args);
      va_end(args);
    }
}

void notice(const char *format, ...) 
{
  va_list args;

  if (!quiet_flag)
    {
      va_start(args, format);
      msg_vformat(MSG_NOTICE, format, args);
      va_end(args);
    }
}

void debug(const char *format, ...) 
{
  va_list args;

  if (debug_flag)
    {
      va_start(args, format);
      msg_vformat(MSG_DEBUG, format, args);
      va_end(args);
    }
}

void verbose(const char *format, ...) 
{
  va_list args;

  if (verbose_flag)
    {
      va_start(args, format);
      msg_vformat(MSG_VERBOSE, format, args);
      va_end(args);
    }
}

void fatal(const char *format, ...) 
{
  va_list args;

  va_start(args, format);
  msg_vformat(MSG_FATAL, format, args);
  va_end(args);

  abort();
}

#if 0
char error_buffer[MAX_BUF_SIZE];
int error_pos = 0;

static void werror_flush(void)
{
  if (error_pos)
    {
      write(2, error_buffer, error_pos);
      error_pos = 0;
    }
}

static void werror_putc(UINT8 c)
{
  if (error_pos == MAX_BUF_SIZE)
    werror_flush();

  error_buffer[error_pos++] = c;
  if (c == '\n')
    werror_flush();
}

static void werror_write(UINT32 length, UINT8 *msg)
{
  if (error_pos + length <= MAX_BUF_SIZE)
    {
      memcpy(error_buffer + error_pos, msg, length);
      error_pos += length;
      if (length && (msg[length-1] == '\n'))
	werror_flush();
    }
  else
    {
      werror_flush();
      write(2, msg, length);
    }
}

static void werror_cstring(char *s) { werror_write(strlen(s), s); }

static void werror_decimal(UINT32 n)
{
  unsigned length = format_size_in_decimal(n);
  char *buffer = alloca(length);
  unsigned i;
  
  for (i = 0; i<length; i++)
    {
      buffer[length - i - 1] = '0' + n % 10;
      n /= 10;
    }
  werror_write(length, buffer);
}

static unsigned format_size_in_hex(UINT32 n);

static void werror_hex_digit(unsigned digit)
{
  werror_putc("0123456789abcdef"[digit]);
}

static void werror_hex_putc(UINT8 c)
{
  werror_hex_digit(c / 16);
  werror_hex_digit(c % 16);
}

static void werror_hex(UINT32 n)
{
  unsigned left = 8;
  
  while ( (left > 1)
	  && (n & 0xf0000000UL))
    {
      left --;
      n <<= 4;
    }
		    
  while (left--)
    {
      werror_hex_digit((n >> 28) & 0xf);
      n <<= 4;
    }
}

void werror_hexdump(UINT8 *msg, UINT32 length, UINT8 *data)
{
  UINT32 i;

  werror_decimal(time(NULL));
  werror_putc(' ');
  werror_cstring(msg);
  for (i=0; i<length; i++)
  {
    if (! (i%16))
      {
	unsigned j = format_size_in_hex((UINT32) data + i);

	werror_cstring("\n0x");

	for ( ; j < 8; j++)
	  werror_putc('0');
	
	werror_hex((UINT32) data + i);
	werror_cstring(": ");
      }

    werror_hex_putc(data[i]);
    werror_putc(' ');
  }
  werror_putc('\n');
}

static void werror_paranoia_putc(UINT8 c)
{
  switch (c)
    {
    case '\\':
      werror_cstring("\\\\");
      break;
    case '\r':
      /* Ignore */
      break;
    default:
      if (!isprint(c))
	{
	  werror_putc('\\');
	  werror_hex_putc(c);
	  break;
	}
      /* Fall through */
    case '\n':
      werror_putc(c);
      break;
    }
}

static unsigned format_size_in_hex(UINT32 n)
{
  int i;
  int e;
  
  /* Table of 16^(2^n) */
  static const UINT32 powers[] = { 0x10UL, 0x100UL, 0x10000UL };

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

#endif
