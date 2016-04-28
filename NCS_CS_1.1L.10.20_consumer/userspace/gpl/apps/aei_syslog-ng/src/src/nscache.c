/***************************************************************************
 *
 * Copyright (c) 2001 BalaBit IT Ltd.
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
 * $Id: nscache.c,v 1.5 2002/09/04 14:52:25 bazsi Exp $
 *
 ***************************************************************************/
 
/*
 * Copyright (c) 2001 Gaël Roualland <gael.roualland@iname.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the author nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS `AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include "nscache.h"

/* Simple NS cache, using a flat hash table (NOT thread safe).
 * This is designed to be used with a fixed set of ip adresses.
 * Inspired from similar caches in ippl, iplog...
 *
 * The size of the cache must be a primary number for good
 * performance and somewhat larger than the maximum number
 * of estimated entries. If more entries than available in the
 * cache are used at the same time it will perform very poorly.
 */

struct nsentry {
	struct in_addr addr;
	char * name;
	time_t used;
	time_t expire;
};

struct nscache {
	int size;
	int expire;
	int expire_failed;
	struct nsentry * entries;
#ifdef NSCACHE_DEBUG
	int hits, misses;
#endif	
};

struct nscache *nscache_new(int size, int expire, int expire_failed) 
{
	struct nscache * cache;
	int i;

	if ((cache = malloc(sizeof(struct nscache))) == NULL)
		return NULL;
	
	/* check args for reasonnable values
	   0 for expire_failed means not to cache failures */
	cache->size = (size < 10) ? 10 : size;
	cache->expire = (expire < 10) ? 10 : expire;
	cache->expire_failed = (expire_failed < 0) ? 0 : expire_failed;

	if ((cache->entries =
	     malloc(cache->size * sizeof(struct nsentry))) == NULL) {
		free(cache);
		return NULL;
	}

	for (i = 0; i < cache->size; i++) {
		cache->entries[i].addr.s_addr = 0;
		cache->entries[i].name = NULL;
		cache->entries[i].expire = 0;
		cache->entries[i].used = 0;
	}

#ifdef NSCACHE_DEBUG
	fprintf(stderr, "initialising nscache: %d entries, %d/%d expiration.\n",
		cache->size, cache->expire, cache->expire_failed);
	cache->hits = cache->misses = 0;
#endif

	return cache;
}

void nscache_free(struct nscache * cache) 
{
	int i;

	if (cache == NULL)
		return;

	if (cache->entries == NULL) {
		free(cache);
		return;
	}

#ifdef NSCACHE_DEBUG
	fprintf(stderr, "destroying nscache: %d entries, %d hits, %d misses.\n",
		cache->size, cache->hits, cache->misses);
#endif
	
	for (i = 0; i < cache->size; i++) {
		if (cache->entries[i].name != NULL)
			free(cache->entries[i].name);
	}

	free(cache->entries);
	free(cache);
}

static char *nsresolve(struct in_addr addr) 
{
	struct hostent *host;

	if ((host = gethostbyaddr((char *) &addr,
				  sizeof(struct in_addr),
				  AF_INET)) != NULL &&
	    host->h_name != NULL)
		return host->h_name;
	return NULL;
}

char *nscache_lookup(struct nscache * cache, struct in_addr in) 
{
	int h, k, old, new;
#ifdef NSCACHE_DEBUG
	int iter = 1;
#endif  
	time_t now = time(NULL);
	time_t oldest = now + 1;
	char *host;

	if (cache == NULL || cache->entries == NULL) /* be safe.. */
		return nsresolve(in);

	k = h = ntohl(in.s_addr) % (cache->size - 1) + 1;
	old = new = -1;
  
	do {
		if (cache->entries[k].expire > 0 &&
		    cache->entries[k].addr.s_addr == in.s_addr) { /* found it! */
			if (now > cache->entries[k].expire) {
				/* entry has expired, update it */
#ifdef NSCACHE_DEBUG
				fprintf(stderr, "%s... updating entry %d (%d > %d).\n",
				        inet_ntoa(in), k, (int) now,
					(int) cache->entries[k].expire);
#endif
				new = k;
				break;
			}
#ifdef NSCACHE_DEBUG
			fprintf(stderr,	"%s... found on %d: %s (%d iters, %d/%d)\n",
				inet_ntoa(in), k, cache->entries[k].name,
				iter, (int) now, (int) cache->entries[k].expire);
			cache->hits++;
#endif  
			cache->entries[k].used = now;
			return cache->entries[k].name;
		}
		if (new < 0) { /* no empty entries found yet */
			if (cache->entries[k].expire < now) {
				new = k; /* use this to save new entry if not found */
				if (cache->entries[k].expire < 1)
					break; /* never used */
			} else 
				if (cache->entries[k].used < oldest) {
					old = k;
					oldest = cache->entries[k].used;
				}
		}
		k = (h + k) % cache->size;
#ifdef NSCACHE_DEBUG
		iter++;
#endif  
	} while (k > 0);
	
	if (new < 0) { /* no place to save ? use oldest. */
		if (old < 0)
			old = 0; /* shouldn't happen.. */
#ifdef NSCACHE_DEBUG
		fprintf(stderr, "%s... old entry %d will be replaced.\n",
			inet_ntoa(cache->entries[old].addr), old);
#endif  
		new = old;
	}
#ifdef NSCACHE_DEBUG
	fprintf(stderr, "%s... adding as %d (%d iters).\n",
		inet_ntoa(in), new, iter);
	cache->misses++;
#endif  
	if (cache->entries[new].name != NULL)
		free (cache->entries[new].name);
	if ((host = nsresolve(in)) != NULL) {
		cache->entries[new].name = strdup(host);
		cache->entries[new].expire =
			now + cache->expire;
	} else {
		cache->entries[new].name = NULL;
		cache->entries[new].expire =
			now + cache->expire_failed;
	}
	cache->entries[new].addr.s_addr = in.s_addr;
	return cache->entries[new].name;
}

#ifdef NSCACHE_DEBUG
void nscache_dump(struct nscache * cache) 
{
	int i;
	
	fprintf(stderr,	"nscache contents:\n");
	for (i = 0; i < cache->size; i++) {
		if (cache->entries[i].used > 0)
			fprintf(stderr, "  %5d  %-16s %09d/%09d  %s\n",
				i, inet_ntoa(cache->entries[i].addr),
				(int) cache->entries[i].used,
				(int) cache->entries[i].expire,
				cache->entries[i].name);
	}
}
#endif
