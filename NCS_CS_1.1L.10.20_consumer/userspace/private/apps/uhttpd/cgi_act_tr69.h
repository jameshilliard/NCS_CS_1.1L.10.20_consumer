#ifndef ACT_TR69_H
#define ACT_TR69_H

int tr69SetUnfreshLeafDataInt(char *mdmoid, int num, char *parameter, int value);
int tr69SetLeafDataInt(char *mdmoid, int num, char *parameter, int value);

int tr69SetUnfreshLeafDataString(char *mdmoid, int num, char *parameter, char *value);
int tr69SetLeafDataString(char *mdmoid, int num, char *parameter, char *value);

int tr69CommitSetLeafDataInt(char *mdmoid, int num, char *parameter, int value);
int tr69CommitSetLeafDataString(char *mdmoid, int num, char *parameter, char *value);
int tr69CommitSetLeafDataEnd(char *mdmoid, int num);

int tr69GetUnfreshLeafData(char *buf, char *full_name, char *parameter);
int tr69GetFreshLeafData(char *buf, char *full_name, char *parameter);

int tr69SaveNow(void);

void turnGetTR69(char *buf);
void cutGetTR69(char *buf);

#define CMSMEM_FREE_BUF_AND_NULL_PTR(p) \
   do { \
      if ((p) != NULL) {free((p)); (p) = NULL;}   \
   } while (0)

char *safe_strstr(const char *s1, const char *s2) ;

#endif

