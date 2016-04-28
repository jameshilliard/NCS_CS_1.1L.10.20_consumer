/* io.c
 *
 * $Id: io.c,v 1.47 2006/03/13 17:01:31 bazsi Exp $ */

/* lsh, an implementation of the ssh protocol
 *
 * Copyright (C) 1998 Niels Möller
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "io.h"
#include "werror.h"
#include "xalloc.h"
#include "format.h"

#include <assert.h>
#include <string.h>

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef HAVE_POLL
# if HAVE_POLL_H
#  include <poll.h>
# elif HAVE_SYS_POLL_H
#  include <sys/poll.h>
# endif
#else
# include "jpoll.h"
#endif

#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <signal.h>

#if HAVE_ALLOCA_H
# include  <alloca.h>
#endif

static void do_mark_callouts(struct ol_queue *q, void (*mark)(struct ol_object *o));
static void do_free_callouts(struct ol_queue *q);

#define CLASS_DEFINE
#include "io.h.x"
#undef CLASS_DEFINE

#include "io.c.x"

struct callout
{
	struct ol_queue_node header;
	int drop;
	struct callback *callout;
	time_t when;
};

static void do_mark_callouts(struct ol_queue *q, void (*mark)(struct ol_object *o))
{
	FOR_QUEUE(q, struct callout *, n) {
		mark(&n->callout->super);
	}
}

static void do_free_callouts(struct ol_queue *q)
{
	FOR_QUEUE(q, struct callout *, n) {
		ol_space_free(n);
	}
}


/* If there's nothing to do for this amount of time (ms), do
 * spontaneous gc. */

#define IDLE_TIME 100

int io_iter(struct io_backend *b)
{
	unsigned long nfds; /* FIXME: Should be nfds_t if that type is defined */
	unsigned long nco; /* number of callouts */
	struct pollfd *fds = NULL;
	int res = 0;
	int timeout = -1;
	time_t now = time(NULL);
	
	
	{
		/* Prepare fd:s. This fase calls the prepare-methods, also  closes
		 * and unlinks any fd:s that should be closed, and also counts how
		 * many fd:s there are. */
		
		struct nonblocking_fd **fd_p;
		struct nonblocking_fd *fd;

		for(fd_p = &b->files; (fd = *fd_p); ) {
			if (fd->super.alive) {
				if (fd->prepare) {
					PREPARE_FD(fd);
				} else if (fd->to_be_closed) {
					kill_fd(fd);
				}
			}
			else {
				if (fd->fd < 0)
					/* Unlink the file object, but don't close any underlying file. */
					;
				else
				{
					/* Used by write fd:s to make sure that writing to its
					 * buffer fails. */
					if (fd->really_close)
						REALLY_CLOSE_FD(fd);
					
					/* FIXME: The value returned from the close callback could be used
					 * to choose an exit code. */
					if (fd->close_callback && fd->close_reason)
						(void) CLOSE_CALLBACK(fd->close_callback, fd->close_reason);

					debug("Closing fd %i.\n", fd->fd);

					close(fd->fd);
				}
				/* Unlink this fd */
				
				/* skip newly added fds */
				while (*fd_p && *fd_p != fd)
					fd_p = &((*fd_p)->next);
				assert(fd_p);
				*fd_p = fd->next;
				continue;
			}
			fd_p = &fd->next;
		}
		
	}
	
	{
		FOR_QUEUE(&b->callouts, struct callout *, n) {
			if (n->when < now) {
				timeout = 0;
				continue;
			}
			if (timeout == -1 || (n->when - now < timeout)) {
				timeout = n->when - now;
			}
		}
	}
	nfds = 0;


	if (!b->reloading) {
		int all_events = 0;
		static int afds_max = 1;
		int afds; /* allocated fds */

		afds = afds_max;
		fds = malloc(sizeof(struct pollfd) * afds);
		/* Fill out fds-array */
		{
			struct nonblocking_fd *fd;
			
			for (fd = b->files; fd; fd = fd->next)
			{
				if (nfds == afds) {
					afds *= 2;
					fds = realloc(fds, sizeof(struct pollfd) * afds);
				}
				
				fds[nfds].fd = fd->fd;
				fds[nfds].events = 0;
				fds[nfds].revents = 0;
				
				if (fd->want_read)
					fds[nfds].events |= POLLIN;
				
				if (fd->want_write)
					fds[nfds].events |= POLLOUT;
				
				all_events |= fds[nfds].events;
				nfds++;
			}
			assert(nfds <= afds);
		}
		if (afds > afds_max)
			afds_max = afds;
		if (all_events)
			res = poll(fds, nfds, IDLE_TIME);
	}

        if (!res && nfds) {
                gc_maybe(&b->super, 0);
                res = poll(fds, nfds, timeout < 0 ? 60000 : timeout * 1000);
        }
        else {
                gc_maybe(&b->super, 1);
                if (nfds == 0) {
                	struct pollfd dummy; 
                	
                	/* this hack is needed on MacOSX where the poll emulation does not handle the fdarray being NULL */
	        	res = poll(&dummy, 0, timeout < 0 ? 60000 : timeout * 1000);
	        }
        }

	if (res < 0) {
		switch(errno)
		{
		case EAGAIN:
		case EINTR:
			break;
		default:
			free(fds);
			werror("io_iter: poll failed: %z", strerror(errno));
			return 0;
		}
	}
	
	if (nfds && res > 0) {
	  /* Do io. Note that the callback functions may add new fds to the
	   * head of the list, or clear the alive flag on any fd. */

		struct nonblocking_fd *fd;
		unsigned long i;
		
		for(fd = b->files, i=0; fd; fd = fd->next, i++)
		{
#if 0
		        debug("io.c: poll for fd %i: events = 0x%xi, revents = 0x%xi.\n",
                	      fds[i].fd, fds[i].events,fds[i].revents);
#endif

			if (!fd->super.alive)
				continue;
			
			if (fds[i].revents & (POLLHUP|POLLERR|POLLNVAL)) {
				if (fd->want_read && fd->read)
					READ_FD(fd);
				else if (fd->want_write && fd->write)
					WRITE_FD(fd);
				else {
					verbose("io_iter(): POLLHUP on inactive fd!\n");
					
					/* we have no chance to flush if POLLHUP was received, therefore closekill immediately */
					closekill_fd(fd, CLOSE_EOF);
				}
				continue;
			}
			if (fds[i].revents & POLLPRI) {
				close_fd(fd, CLOSE_PROTOCOL_FAILURE);
				continue;
			}
			/*
			if (fds[i].revents & (POLLNVAL | POLLERR)) {
				close_fd(fd, CLOSE_POLL_FAILED);
				continue;
			}
			*/
			if (fds[i].revents & POLLOUT)
				if (fd->want_write && fd->write)
					WRITE_FD(fd);
			
			if (fds[i].revents & POLLIN) {
				if (fd->want_read && fd->read)
					READ_FD(fd);
			}

		}
		assert(i == nfds);
	}
	free(fds);

	nco = 0;
	{
		FOR_QUEUE(&b->callouts, struct callout *, n) {
			if (now >= n->when) {
				CALLBACK(n->callout);
				ol_queue_remove(&n->header);
				ol_space_free(n);
			}
			nco++;
		}
	}
	if (!nfds && !nco && !b->reloading) {
		verbose("No listening fds and no pending events, exiting...\n");
		return 0;
	}
	return 1;
}

/* CLASS:
   (class
     (name fd_read)
     (super abstract_read)
       (vars
         (fd simple int)))
*/

#if 0
void debug_packet(UINT32 length, UINT8 *data)
{
  int i;

  debug("------\n");
  for (i = 0; i < length; i++) 
    {
      debug("%c", data[i]);
    }
  debug("\n------\n");
}
#endif

static int do_read(struct abstract_read **r, UINT32 length, UINT8 *buffer)
{
  CAST(fd_read, closure, *r);

  if (!length)
    {
      werror("io.c: do_read(): Zero length read was requested.\n");
      return 0;
    }
    
  for (;;)
    {
      int res = read(closure->fd, buffer, length);
#if 0
      if (res>=0)
	{
	  debug("readfd=%i, length=%i\n", closure->fd, res);
	  debug_packet(res, buffer);
	}
#endif
      if (!res)
	{
	  debug("Read EOF on fd %i.\n", closure->fd);
	  return A_EOF;
	  return 0;
	}
      if (res > 0)
	return res;

      switch(errno)
	{
	case EINTR:
	  continue;  /* FIXME: Is it really worth looping here,
		      * instead of in the select loop? */
	case EWOULDBLOCK:  /* aka EAGAIN */
	  return 0;
	case EPIPE:
	  werror("io.c: read() returned EPIPE! Treating it as EOF.\n");
	  return A_EOF;
	default:
	  werror("io.c: do_read: read() failed (errno %i), %z\n",
		 errno, strerror(errno));

	  debug("  fd = %i, buffer = %xi, length = %i\n",
		closure->fd, buffer, length);

	  return A_FAIL;
	}
    }
}

static int do_recv(struct abstract_read **r, UINT32 length, UINT8 *buffer, abstract_addr *addr, socklen_t *addrlen)
{
	CAST(fd_read, closure, *r);

	if (!length) {
		werror("io.c: do_recv(): Zero length read was requested.\n");
		return 0;
	}

	for (;;) {
		int res;
		
		/* This is another workaround in Linux kernel bugs */
		
		((struct sockaddr *) addr)->sa_family = 0;
		
		res = recvfrom(closure->fd, buffer, length, 0, (struct sockaddr *) addr, (socklen_t *) addrlen);

		if (*addrlen == 2 || (*addrlen >= 2 && ((struct sockaddr *) addr)->sa_family == 0)) {
			/* HACK: this is a workaround of a Linux 2.2 & 2.4 bug,
			 * it doesn't return anything sensible in sockaddr buf 
			 */
			((struct sockaddr *) addr)->sa_family = AF_UNIX;
		}

		if (res == 0) {
			/* on SOCK_DGRAM protocols 0 returned bytes doesn't mean anything */
			return 0;
		}
		if (res > 0)
			return res;

		switch(errno) {
		case EINTR:
			continue;  /* FIXME: Is it really worth looping here,
				    * instead of in the select loop? */
		case EWOULDBLOCK:  /* aka EAGAIN */
			return 0;
		case EPIPE:
			werror("io.c: recvfrom() returned EPIPE! Treating it as EOF.\n");
			return A_EOF;
		default:
			werror("io.c: do_recv: recvfrom() failed (errno %i), %z\n",
			       errno, strerror(errno));

			debug("  fd = %i, buffer = %xi, length = %i\n",
			      closure->fd, buffer, length);

			return A_FAIL;
		}
	}
}

static void read_callback(struct nonblocking_fd *fd)
{
	CAST(io_fd, self, fd);
	int res;

	struct fd_read r =
	{ { STACK_HEADER, do_read, do_recv }, 0 };
	
	r.fd = fd->fd;

	/* The handler function may install a new handler */
	res = READ_HANDLER(self->handler,
			   &r.super);


#if 0
	/* NOTE: These flags are not mutually exclusive. All combinations
	 * must be handled correctly. */
  
	/* NOTE: (i) If LSH_DIE is set, LSH_CLOSE is ignored. (ii) If the fd
	 * is read_only, LSH_CLOSE is the same as LSH_DIE. */

	/* This condition must be taken care of earlier. */
	assert(!(res & LSH_CHANNEL_FINISHED));

	/* Not implemented */
	assert(!(res & LSH_KILL_OTHERS));
  
#endif

	if (res & ST_HOLD)
	{
		/* This flag should not be combined with anything else */
		assert(res == ST_HOLD);
		fd->want_read = 0;
	}
	if (res & ST_DIE)
	{
		closekill_fd(fd,
			     ST_FAILUREP(res) ? CLOSE_PROTOCOL_FAILURE : 0);
	}
	else if (res & ST_CLOSE)
	{
		close_fd(fd, ST_FAILUREP(res) ? CLOSE_PROTOCOL_FAILURE : CLOSE_EOF);
	}
}

/* CLASS:
     (class
       (name fd_write)
       (super abstract_write)
       (vars
         (fd simple int)
         (fsync simple int)))
*/

static int do_write(struct abstract_write *w, UINT32 length, UINT8 *data)
{
	CAST(fd_write, self, w);
	int saved_errno;
	int res;
	UINT8 eofbuf[1];

#ifdef MSG_DONTWAIT
	res = recv(self->fd, eofbuf, 1, MSG_PEEK | MSG_DONTWAIT);
#else
	{
		int old_flags;
	
		old_flags = fcntl(self->fd, F_GETFL);
		fcntl(self->fd, F_SETFL, old_flags | O_NONBLOCK);
		res = recv(self->fd, eofbuf, 1, MSG_PEEK);
		if ((old_flags & O_NONBLOCK) == 0)
			fcntl(self->fd, F_SETFL, old_flags);
	}
#endif

	if (res == 0) {
		/* EOF detected */
		werror("io.c: do_write: write() failed, EOF detected\n");
		errno = ECONNRESET;
		return -1;
	}
	
	res = write(self->fd, data, length);
	saved_errno = errno;

#if 0
	if (res>=0)
	  {
	    debug("writefd=%i, length=%i\n", self->fd, res);
	    debug_packet(res, data);
	  }
#endif

	if (res < 0)
	{
		switch(errno)
		{
		case EINTR:
		case EWOULDBLOCK:
			return 0;
		default:
			werror("io.c: do_write: write() failed (errno %i), %z\n",
			       errno, strerror(errno));
		}
	}
	if (self->fsync)
		fsync(self->fd);
	errno = saved_errno;
	return res;
}

static void write_callback(struct nonblocking_fd *fd)
{
	CAST(io_fd, self, fd);
	int res;

	struct fd_write w =
	{ { STACK_HEADER, do_write }, 0, 0 };
	
	w.fd = fd->fd;
	w.fsync = self->fsync;

	assert(self->buffer);

	res = BUF_FLUSH(self->buffer, &w.super);
	if (res & ST_CLOSE)
	{
		close_fd(fd, ST_FAILUREP(res) ? CLOSE_PROTOCOL_FAILURE : CLOSE_EOF);
	} else if (res & ST_DIE) {
		closekill_fd(fd, ST_FAILUREP(res) ? CLOSE_PROTOCOL_FAILURE : CLOSE_EOF);
	}
}  

static void listen_callback(struct nonblocking_fd *fd)
{
	CAST(listen_fd, self, fd);
	char sockaddr_buf[256];
	struct sockaddr *peer = (struct sockaddr *) &sockaddr_buf;
	socklen_t addr_len = sizeof(sockaddr_buf);
	int res;
	int conn;

	conn = accept(fd->fd,
		      peer, &addr_len);
	if (conn < 0)
	{
		werror("io.c: accept() failed, %z", strerror(errno));
		return;
	}
	if (addr_len) {
		res = FD_LISTEN_CALLBACK(self->callback, conn,
					 sockaddr2address_info(addr_len, peer));
		if (ST_CLOSEDP(res))
		{
			closekill_fd(fd, (ST_FAILUREP(res)
					  ? CLOSE_PROTOCOL_FAILURE
					  : CLOSE_EOF));
		}
	}
	else {
		closekill_fd(fd, CLOSE_EOF);
	}
}

static void connect_callback(struct nonblocking_fd *fd)
{
	CAST(connect_fd, self, fd);
	int sock_err;
	socklen_t len = sizeof(sock_err);
  
  	if ((getsockopt(fd->fd, SOL_SOCKET, SO_ERROR, (char *) &sock_err, &len) < 0) ||
  	    sock_err) {
  	    	verbose("connect_callback(): connect() failed\n");
  		FD_CALLBACK(self->callback, -1);
  	}
  	else {
		int res = FD_CALLBACK(self->callback, fd->fd);

		if (!ST_CLOSEDP(res))
		{
			/* To avoid actually closing the fd */
			fd->fd = -1;
		}
	}
	kill_fd(fd);
}

static void really_close(struct nonblocking_fd *fd)
{
	CAST(io_fd, self, fd);

	assert(self->buffer);
  
	BUF_CLOSE(self->buffer);
}

static void prepare(struct nonblocking_fd *fd)
{
	CAST(io_fd, self, fd);

	if (self->buffer && fd->to_be_closed == 1) 
		BUF_CLOSE(self->buffer);
		
	if (fd->super.alive && self->buffer)
		fd->want_write = BUF_PREPARE(self->buffer);
	else
		fd->want_write = 0;
	/* attempt to flush data 3 times, before fd is really closed */
	if (fd->to_be_closed) {
		if (fd->want_write) {
			if (fd->to_be_closed > 3)
				kill_fd(fd);
			fd->to_be_closed++;
		}
		else {
			kill_fd(fd);
		}
	}

}

/* FIXME: Perhaps this function should return a suitable exit code? */
void io_run(struct io_backend *b)
{
	struct sigaction pipe;
	memset(&pipe, 0, sizeof(pipe));

	pipe.sa_handler = SIG_IGN;
	sigemptyset(&pipe.sa_mask);
	pipe.sa_flags = 0;
  
	if (sigaction(SIGPIPE, &pipe, NULL) < 0)
		fatal("Failed to ignore SIGPIPE.\n");
  
	while(io_iter(b))
		;
}

void init_backend(struct io_backend *b)
{
	b->files = NULL;

	ol_queue_init(&b->callouts);

}

/* This function is called if a connection this file somehow dependent
 * on disappears. For instance, the connection may have spawned a
 * child process, and this file may be the stdin of that process. */

/* To kill a file, mark it for closing and the backend will do the work. */
static void do_kill_fd(struct resource *r)
{
	CAST_SUBTYPE(nonblocking_fd, fd, r);

	/* FIXME: It could make sense to you a separate close reason for
	 * killed files. For now, using the zero reason will supress calling
	 * of any close callbacks. */
	if (r->alive)
		close_fd(fd, 0);
}

/* Initializes a file structure, and adds it to the backend's list. */
void init_file(struct io_backend *b, struct nonblocking_fd *f, int fd,
	struct ol_string *fname)
{
	/* assert(fd); */
	f->fd = fd;
	f->fname = fname;
	f->close_reason = -1; /* Invalid reason */
	f->close_callback = NULL;

	f->want_read = 0;
	f->read = NULL;

	f->want_write = 0;
	f->write = NULL;

	f->super.alive = 1;
	f->super.kill = do_kill_fd;
	f->really_close = NULL;

	f->next = b->files;
	b->files = f;
}

/* Blocking read from a file descriptor (i.e. don't use the backend).
 * The fd should *not* be in non-blocking mode. */

int blocking_read(int fd, struct read_handler *handler)
{
	struct fd_read r =
	{ { STACK_HEADER, do_read }, 0 };
	
	r.fd = fd;

	for (;;)
	{
		int res = READ_HANDLER(handler,
				       &r.super);

		assert(!(res & ST_HOLD));

		if (res & (ST_CLOSE | ST_DIE))
		{
			close(fd);
			return res;
		}
		if (res & ST_FAIL)
			werror("blocking_read: Ignoring error %i\n", res);
	}
}
/*
 * Fill in ADDR from HOST, SERVICE and PROTOCOL.
 * Supplying a null pointer for HOST means use INADDR_ANY.
 * Otherwise HOST is an numbers-and-dots ip-number or a dns name.
 *
 * PROTOCOL can be tcp or udp.
 *
 * Supplying a null pointer for SERVICE, means use port 0, i.e. no port.
 * 
 * Returns zero on errors, 1 if everything is ok.
 */
int
get_inaddr(struct sockaddr_in	* addr,
	   const char		* host,
	   const char		* service,
	   const char		* protocol)
{
	memset(addr, 0, sizeof *addr);
	addr->sin_family = AF_INET;

	/*
	 *  Set host part of ADDR
	 */
	if (host == NULL)
		addr->sin_addr.s_addr = INADDR_ANY;
	else
	{
		/* First check for numerical ip-number */
#if HAVE_INET_ATON
		if (!inet_aton(host, &addr->sin_addr))
#else /* !HAVE_INET_ATON */
			/* TODO: It is wrong to work with ((unsigned long int) -1)
			 * directly, as this breaks Linux/Alpha systems. But
			 * INADDR_NONE isn't portable. The clean solution is to use
			 * inet_aton rather than inet_addr; see the GNU libc
			 * documentation. */
# ifndef INADDR_NONE
# define INADDR_NONE ((in_addr_t) -1)
# endif /* !INADDR_NONE */
			addr->sin_addr.s_addr = inet_addr(host);
		if (addr->sin_addr.s_addr == INADDR_NONE)
#endif  /* !HAVE_INET_ATON */
		{
			struct hostent * hp;
	  
			hp = gethostbyname(host);
			if (hp == NULL)
				return 0;
			memcpy(&addr->sin_addr, hp->h_addr, (size_t) (hp->h_length));
			addr->sin_family = hp->h_addrtype;
		}
	}

	/*
	 *  Set port part of ADDR
	 */
	if (service == NULL)
		addr->sin_port = htons(0);
	else
	{
		char		* end;
		long		  portno;

		portno = strtol(service, &end, 10);
		if (portno > 0  &&  portno <= 65535
		    &&  end != service  &&  *end == '\0')
		{
			addr->sin_port = htons(portno);
		}
		else
		{
			struct servent	* serv;

			serv = getservbyname(service, protocol);
			if (serv == NULL)
				return 0;
			addr->sin_port = serv->s_port;
		}
	}

	return 1;
}

/* Converts a string port number or service name to a port number.
 * Returns the port number in _host_ byte order, or -1 of the port
  * or service was invalid. */
  
int get_portno(const char *service, const char *protocol)
{
  if (service == NULL)
    return 0;
  else
    {
      char *end;
      long portno;

      portno = strtol(service, &end, 10);
      if (portno > 0
          &&  portno <= 65535
          &&  end != service
          &&  *end == '\0')
          return portno;
      else
        {
          struct servent * serv;

          serv = getservbyname(service, protocol);
          if (serv == NULL)
            return -1;
          return ntohs(serv->s_port);
        }
    }
}

/* FIXME: IPv6 support */
/* FIXME: The host name lookup may block. We would need an asyncronous
 * get_inaddr function. As a work around, we could let the client do
 * all lookups, so that the server ned only deal with ip-numbers. And
 * optionally refuse requests with dns names. */

int tcp_addr(struct sockaddr_in *sin,
	     UINT32 length,
	     UINT8 *addr,
	     UINT32 port)
{
	char *c = alloca(length + 1);
	int res;
  
	memcpy(c, addr, length);
	c[length] = '\0';

	res = get_inaddr(sin, c, NULL, "tcp");
	if (!res)
		return 0;

	sin->sin_port = htons(port);
	return 1;
}

/* inet_address_info */

static int inet_address2sockaddr_in(struct address_info *c, int sasize, struct sockaddr *sa)
{
	CAST(inet_address_info, a, c);
	struct sockaddr_in *in = (struct sockaddr_in *) sa;

	if (sasize != sizeof(*in))
		return 0;
	if (a->sa.sin_addr.s_addr != 0) {
		memcpy(sa, &a->sa, sizeof(a->sa));
		return 1;
	}
	if (!tcp_addr(in,
		     a && a->ip ? a->ip->length : 7, 
		     a && a->ip ? (UINT8 *) a->ip->data : (UINT8 *) "0.0.0.0",
		     a ? a->port : 0))
		return 0;
	return 1;
}

static int bind_inet_socket(struct address_info *c, int fd)
{
	CAST(inet_address_info, i, c);
	struct sockaddr_in in;
	int tmp = 1;

	if (inet_address2sockaddr_in(c, sizeof(in), (struct sockaddr *) &in)) {
		verbose("binding fd %i, inetaddr: %S, port: %i\n", fd, i->ip, i->port);
		setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(tmp));
		if (bind(fd, (struct sockaddr *) &in, sizeof(struct sockaddr_in)) == -1) {
			werror("io.c: bind_inet_socket() bind failed %I:%i %z\n", in.sin_addr, ntohs(in.sin_port), strerror(errno));
			return 0;
		}
	}
	return 1;
}

static int connect_inet_socket(struct address_info *c, int fd)
{
	CAST(inet_address_info, i, c);
	struct sockaddr_in in;

	if (inet_address2sockaddr_in(c, sizeof(in), (struct sockaddr *) &in)) {
		verbose("connecting fd %i to inetaddr %S, port %i\n", fd, i->ip, i->port);
		if (connect(fd, (struct sockaddr *) &in, sizeof(in)) < 0) {
			switch (errno) {
			case EINPROGRESS:
			case EWOULDBLOCK:
				return 1;
			default:
				return 0;
			}
		}
		return 1;
	}
	return 0;
}


struct inet_address_info *make_inet_address(struct ol_string *host, UINT32 port)
{
	NEW(inet_address_info, info);

	info->super.family = AF_INET;
	info->super.convert2sockaddr = inet_address2sockaddr_in;
	info->super.bind_socket = bind_inet_socket;
	info->super.connect_socket = connect_inet_socket;
	info->port = port; /* htons(port); */
	info->ip = host;
	return info;
}

struct inet_address_info *make_inet_address_c(const char *host, const char *port)
{
	int portno = get_portno(port, "tcp");
	if (portno < 0)
		return 0;
	else
	{
		return make_inet_address(host ? c_format("%z", host) : NULL, portno);
	}
}

/* unix_address_info */

static int unix_address2sockaddr_un(struct address_info *c, int sasize, struct sockaddr *sa)
{
	CAST(unix_address_info, a, c);
	struct sockaddr_un *un = (struct sockaddr_un *) sa;
	if (sasize != sizeof(*un))
		return 0;
	strncpy(un->sun_path, (char *) a->path->data, LIBOL_MIN(a->path->length + 1, sizeof(un->sun_path)));
	un->sun_family = AF_UNIX;
	return 1;
}

static int bind_unix_socket(struct address_info *c, int fd)
{
	CAST(unix_address_info, a, c);
	struct sockaddr_un un;
	
	verbose("binding fd %i, unixaddr: %S\n", fd, a->path);
	if (unix_address2sockaddr_un(c, sizeof(un), (struct sockaddr *) &un)) {
		struct stat st;

		if (stat(un.sun_path, &st) == 0 && !S_ISSOCK(st.st_mode)) {
			werror("io.c: bind_unix_socket(): %z not a socket\n", un.sun_path);
			return -1;
		}
		unlink(un.sun_path);
		errno = 0;
		if (bind(fd, (struct sockaddr *) &un, sizeof(un) - sizeof(un.sun_path) + strlen(un.sun_path) + 1) == -1) {
			werror("io.c: bind_unix_socket(): bind failed %z (%z)\n", 
			       un.sun_path, strerror(errno));
			return 0;
		}
		return 1;
	}
	return 0;
}

static int connect_unix_socket(struct address_info *c, int fd)
{
	CAST(unix_address_info, a, c);
	struct sockaddr_un un;

	verbose("connecting fd %i, unixaddr: %S\n", fd, a->path);
	if (unix_address2sockaddr_un(c, sizeof(un), (struct sockaddr *) &un)) {
		if (connect(fd, (struct sockaddr *) &un, sizeof(un)) < 0) {
			switch (errno) {
			case EWOULDBLOCK:
				return 1;
			default:
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

struct unix_address_info *make_unix_address(struct ol_string *path)
{
	NEW(unix_address_info, info);

	info->super.family = AF_UNIX;
	info->super.convert2sockaddr = unix_address2sockaddr_un;
	info->super.bind_socket = bind_unix_socket;
	info->super.connect_socket = connect_unix_socket;
	info->path = path;
	return info;
}

struct unix_address_info *make_unix_address_c(const char *path)
{
	return make_unix_address(c_format_cstring("%z", path));
}

struct address_info *sockaddr2address_info(size_t addr_len,
                                           struct sockaddr *addr)
{
	if (addr_len == 0)
		return NULL;
	switch(addr->sa_family)
	{
	case AF_INET:
	{
		struct inet_address_info *a;
		struct sockaddr_in *in = (struct sockaddr_in *) addr;
		UINT32 ip, port;

		assert(addr_len == sizeof(struct sockaddr_in));
		ip = ntohl(in->sin_addr.s_addr);
		port = ntohs(in->sin_port);
		a = make_inet_address(c_format_cstring("%i.%i.%i.%i",
					       (ip >> 24) & 0xff,
					       (ip >> 16) & 0xff,
					       (ip >> 8) & 0xff,
					       ip & 0xff), port);
		memcpy(&a->sa, addr, addr_len);
		return &a->super;
	}
	case AF_UNIX: {
		struct sockaddr_un *un = (struct sockaddr_un *) addr;
		return &make_unix_address(c_format("%z", un->sun_path))->super;
	}
	case AF_UNSPEC:
		break;
#if 0
	case AF_INETv6:
		break;
#endif
	default:
		verbose("io.c: sockaddr2info(): Unsupported address family %i (addrlen=%i).\n", addr->sa_family, addr_len);
		break;
	}
	return NULL;
}

#if 0
int open_inet_socket(int socktype, int proto, struct sockaddr_in *sa)
{
        int fd;

        fd = socket(AF_INET, socktype, proto);
        if (fd != -1 && sa) {
        	int tmp = 1;
                setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(tmp));
                if (bind(fd, sa, sizeof(struct sockaddr_in)) == -1) {
			werror("io.c: open_inet_socket() bind failed %z\n", strerror(errno));
                        close(fd);
                        fd = -1;
                }
        }
        return fd;
}
#endif

int open_socket(int family, int socktype, int proto, struct address_info *a)
{

	int fd;
	fd = socket(family, socktype, proto);
	if (fd != -1 && a) {
		if (a && !ADDRESS_BIND(a, fd)) {
			werror("open_socket(): error binding socket\n");
			close(fd);
			return -1;
		}

	}
	return fd;
}

/* For fd:s in blocking mode. */
int write_raw(int fd, UINT32 length, UINT8 *data)
{
	while(length)
	{
		int written = write(fd, data, length);

		if (written < 0)
			switch(errno)
			{
			case EINTR:
			case EAGAIN:
				continue;
			default:
				return 0;
			}
      
		length -= written;
		data += written;
	}
	return 1;
}

int write_raw_with_poll(int fd, UINT32 length, UINT8 *data)
{
	while(length)
	{
		struct pollfd pfd;
		int res;
		int written;
      
		pfd.fd = fd;
		pfd.events = POLLOUT;

		res = poll(&pfd, 1, -1);

		if (res < 0)
			switch(errno)
			{
			case EINTR:
			case EAGAIN:
				continue;
			default:
				return 0;
			}
      
		written = write(fd, data, length);

		if (written < 0)
			switch(errno)
			{
			case EINTR:
			case EAGAIN:
				continue;
			default:
				return 0;
			}
      
		length -= written;
		data += written;
	}
	return 1;
}

void io_set_nonblocking(int fd)
{
	if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK) < 0)
		fatal("io_set_nonblocking: fcntl() failed, %z", strerror(errno));
}

void io_set_close_on_exec(int fd)
{
	if (fcntl(fd, F_SETFD, 1) < 0)
		fatal("Can't set close-on-exec flag for fd %i: %z\n",
		      fd, strerror(errno));
}

/* ALL file descripters handled by the backend should use non-blocking mode,
 * and have the close-on-exec flag set. */

void io_init_fd(int fd)
{
  io_set_nonblocking(fd);
  io_set_close_on_exec(fd);
}

int io_open_socket(int family, int socktype, int proto, struct address_info *local)
{
	int s;

	s = socket(family, socktype, proto);
	if (s < 0)
		return -1;
  	io_init_fd(s);
	if (local && ADDRESS_BIND(local, s))
		return s;
	else
		if (!local)
			return s;
	close(s);
	return -1;
}

/* Some code is taken from bellman's tcputils. */
struct connect_fd *io_connect(struct io_backend *b,
			      int s,
			      struct address_info *remote,
			      struct fd_callback *f)
{

	debug("io.c: connecting using fd %i\n", s);

	if (!ADDRESS_CONNECT(remote, s)) {
		int saved_errno = errno;
		close(s);
		errno = saved_errno;
		return NULL;
	}

	{
		NEW(connect_fd, fd);

		init_file(b, &fd->super, s, NULL);

		fd->super.want_write = 1;
		fd->super.write = connect_callback;
		fd->callback = f;
    
		return fd;
	}
}

struct listen_fd *io_listen(struct io_backend *b,
			    int s,
			    struct fd_listen_callback *callback)
{
	if (s < 0) return NULL;

	debug("io.c: listening on fd %i\n", s);


	if (listen(s, 256) < 0) {
		close(s);
		return NULL;
	}

	{
		NEW(listen_fd, fd);

		init_file(b, &fd->super, s, NULL);
    
		fd->super.want_read = 1;
		fd->super.read = listen_callback;
		fd->callback = callback;
    
		return fd;
	}
}

struct io_fd *make_io_fd(struct io_backend *b,
                         int fd,
			 struct ol_string *fname)
{
	NEW(io_fd, f);

	io_init_fd(fd);
	init_file(b, &f->super, fd, fname);

	return f;
}

struct io_fd *io_read_write(struct io_fd *f,
			    struct read_handler *handler,
			    struct abstract_buffer *buffer,
			    struct close_callback *close_callback)
{

	debug("io.c: Preparing fd %i for reading and writing\n", f->super.fd);


	f->super.prepare = prepare;

	/* Reading */
	f->super.read = read_callback;
	f->super.want_read = !!handler;
	f->handler = handler;

	/* Writing */
	f->super.write = write_callback;
	f->buffer = buffer;

	/* Closing */
	f->super.really_close = really_close;
	f->super.close_callback = close_callback;

	return f;
}

struct io_fd *io_read(struct io_fd *f,
		      struct read_handler *handler,
		      struct close_callback *close_callback)
{

	debug("io.c: Preparing fd %i for reading\n", f->super.fd);

	f->super.prepare = prepare;

	/* Reading */
	f->super.want_read = !!handler;
	f->super.read = read_callback;
	f->handler = handler;

	f->super.close_callback = close_callback;

	return f;
}

struct io_fd *io_write(struct io_fd *f,
                       struct abstract_buffer *buffer,
                       struct close_callback *close_callback)
{


	debug("io.c: Preparing fd %i for writing\n", f->super.fd);

	f->super.prepare = prepare;

	/* Writing */
	f->super.write = write_callback;
	f->buffer = buffer;

	f->super.close_callback = close_callback;

	return f;
}

struct callout *io_callout(struct io_backend *backend, time_t timeout, struct callback *callout)
{
	struct callout *co;

	NEW_SPACE(co);
	co->when = time(NULL) + timeout;
	co->callout = callout;
	co->drop = 1;
	ol_queue_add_tail(&backend->callouts, &co->header);
	return co;
}

void io_callout_set_timeout(struct callout *co, time_t timeout)
{
	co->when = time(NULL) + timeout;
}

void io_callout_set_drop(struct callout *co, int drop)
{
	co->drop = drop;
}

void io_callout_flush(struct io_backend *backend)
{
	FOR_QUEUE(&backend->callouts, struct callout *, n) {
		if (!n->drop)
			CALLBACK(n->callout);
		ol_queue_remove(&n->header);
		ol_space_free(n);
	}

}

int reopen_fd(struct nonblocking_fd *fd)
{
	struct stat statbuf;
	int oldfd, newfd, oldmode;

	oldfd = fd->fd;
	if (!fd->fname || oldfd < 0)
		return -1;

	fstat(oldfd, &statbuf);
	oldmode = fcntl(oldfd, F_GETFL);
	if ((oldmode & O_WRONLY) || (oldmode & O_RDWR))
		oldmode |= O_CREAT | O_APPEND;

	if ((newfd = open((char *) fd->fname->data, oldmode)) == -1)
	{
		werror("reopen_fd: open failedfor %S: %z\n",
			fd->fname, strerror(errno));
		return -1;
	}

	fchmod(newfd, statbuf.st_mode);
	fchown(newfd, statbuf.st_uid, statbuf.st_gid);
	dup2(newfd, oldfd);
	close(newfd);

	return 0;
}

void kill_fd(struct nonblocking_fd *fd)
{
	fd->super.alive = 0;
}

void close_fd(struct nonblocking_fd *fd, int reason)
{

	debug("Marking fd %i for closing.\n", fd->fd);

	fd->close_reason = reason;
	if (!fd->to_be_closed)
		fd->to_be_closed = 1;
}

