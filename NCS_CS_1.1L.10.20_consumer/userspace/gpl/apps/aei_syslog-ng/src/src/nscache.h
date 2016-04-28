#ifndef __NSCACHE_H
#define __NSCACHE_H

#include <config.h>

#if 0

WITH_DEBUG
#define NSCACHE_DEBUG
#endif

#include <netdb.h>
#include <netinet/in.h>

struct nscache;

struct nscache * nscache_new(int size, int expire, int expire_failed);
void nscache_free(struct nscache *);
char * nscache_lookup(struct nscache *, struct in_addr);

#ifdef NSCACHE_DEBUG
void nscache_dump(struct nscache *);
#else
#define nscache_dump(cache) {}
#endif

#endif
