#ifndef CGI_VOICE_H
#define CGI_VOICE_H
//int cgiPostLANSubnetDhc();
//int cgiPostLANIPAllocatio();

void cgiGetVoiceStatus(int argc, char **argv, char *varValue);
void cgiGetVoiceCallSummary(int argc, char **argv, char *varValue);
void cgiGetVoiceCumuLast(int argc, char **argv, char *varValue);
void cgiGetVoiceStatistics(int argc, char **argv, char *varValue);
int cgiPostVoiceLine();
#endif

