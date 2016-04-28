/*
 *      TR69 Function Define
 *
 *      Authors: Erishen  <lsun@actiontec.com>
 *
 */

#ifndef ACT_TR69_H
#define ACT_TR69_H


int tr69SetUnfreshLeafDataInt(char *mdmoid, int num, char *parameter, int value);
int tr69SetLeafDataInt(char *mdmoid, int num, char *parameter, int value);

int tr69SetUnfreshLeafDataString(char *mdmoid, int num, char *parameter, char *value);
int tr69SetLeafDataString(char *mdmoid, int num, char *parameter, char *value);

int tr69CommitSetLeafDataInt(char *mdmoid, int num, char *parameter, int value);
int tr69CommitSetLeafDataString(char *mdmoid, int num, char *parameter, char *value);
int tr69CommitSetLeafDataEnd(char *mdmoid, int num);
int tr69Save();

int tr69GetUnfreshLeafData(char *buf, char *full_name, char *parameter);
int tr69GetFreshLeafData(char *buf, char *full_name, char *parameter);

void turnGetTR69(char *buf);
void cutGetTR69(char *buf);


#endif

