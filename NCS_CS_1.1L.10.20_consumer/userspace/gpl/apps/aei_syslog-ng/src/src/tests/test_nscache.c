#include <sys/types.h>
#include "nscache.h"
#include <stdio.h>

#if ENABLE_TCP_WRAPPER
#include <tcpd.h>
int allow_severity = 0;
int deny_severity = 0;
#endif

int main()
{
  struct in_addr in;
  struct nscache *cache;

  cache = nscache_new(1000, 3600, 1);
  in.s_addr = htonl(0x7f000001);
  printf("%s\n", nscache_lookup(cache, in));
  in.s_addr = htonl(0x4c38c8);
  printf("%s\n", nscache_lookup(cache, in));
  printf("%s\n", nscache_lookup(cache, in));
  printf("%s\n", nscache_lookup(cache, in));
  return 0;
}
