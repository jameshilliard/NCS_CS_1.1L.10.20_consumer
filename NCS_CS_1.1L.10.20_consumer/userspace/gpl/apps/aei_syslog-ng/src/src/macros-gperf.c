/* ANSI-C code produced by gperf version 3.0.1 */
/* Command-line: gperf -LANSI-C -t -c -l -k1,3,5 -N find_macro  */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif


#include "macros.h"
struct macro_def { char *name; int id; int len; };

#define TOTAL_KEYWORDS 55
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 13
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 87
/* maximum key range = 85, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register unsigned int len)
{
  static unsigned char asso_values[] =
    {
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 25, 88, 15, 55,  0,
       0, 15, 30, 25, 88, 88,  5,  0,  0, 30,
      45, 88,  0,  5, 15, 10, 60,  0, 88,  5,
      88, 88, 88, 88, 88,  5, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

#ifdef __GNUC__
__inline
#endif
struct macro_def *
find_macro (register const char *str, register unsigned int len)
{
  static unsigned char lengthtable[] =
    {
       0,  0,  0,  3,  0,  5,  0,  7,  0,  9,  5,  0,  7,  0,
       9, 10,  0,  2,  3,  4, 10,  0,  7,  3,  4,  5,  0,  0,
       8,  0,  5,  0, 12,  3,  4,  5,  6,  7,  8,  4,  0,  6,
       0,  8,  4, 10,  6,  0, 13,  9, 10,  6,  0,  8,  0, 10,
       0,  0,  8,  0, 10,  0,  7,  3,  9,  5,  0,  0,  8,  9,
       5,  0,  0,  3,  4,  5,  6,  0,  8,  9,  0,  6,  7,  0,
       0,  0,  0,  7
    };
  static struct macro_def wordlist[] =
    {
      {""}, {""}, {""},
      {"MIN", M_MIN},
      {""},
      {"R_MIN", M_MIN_RECVD},
      {""},
      {"R_MONTH", M_MONTH_RECVD},
      {""},
      {"R_WEEKDAY", M_WEEKDAY_RECVD},
      {"S_MIN", M_MIN_STAMP},
      {""},
      {"S_MONTH", M_MONTH_STAMP},
      {""},
      {"S_WEEKDAY", M_WEEKDAY_STAMP},
      {"R_FULLDATE", M_FULLDATE_RECVD},
      {""},
      {"TZ", M_TZ},
      {"MSG", M_MESSAGE},
      {"R_TZ", M_TZ_RECVD},
      {"S_FULLDATE", M_FULLDATE_STAMP},
      {""},
      {"MSGONLY", M_MSGONLY},
      {"SEC", M_SEC},
      {"S_TZ", M_TZ_STAMP},
      {"R_SEC", M_SEC_RECVD},
      {""}, {""},
      {"FACILITY", M_FACILITY},
      {""},
      {"S_SEC", M_SEC_STAMP},
      {""},
      {"FACILITY_NUM", M_FACILITY_NUM},
      {"TAG", M_TAG},
      {"YEAR", M_YEAR},
      {"MONTH", M_MONTH},
      {"R_YEAR", M_YEAR_RECVD},
      {"MESSAGE", M_MESSAGE},
      {"SOURCEIP", M_SOURCE_IP},
      {"HOST", M_HOST},
      {""},
      {"S_YEAR", M_YEAR_STAMP},
      {""},
      {"FULLHOST", M_FULLHOST},
      {"HOUR", M_HOUR},
      {"R_UNIXTIME", M_UNIXTIME_RECVD},
      {"R_HOUR", M_HOUR_RECVD},
      {""},
      {"FULLHOST_FROM", M_FULLHOST_FROM},
      {"HOST_FROM", M_HOST_FROM},
      {"S_UNIXTIME", M_UNIXTIME_STAMP},
      {"S_HOUR", M_HOUR_STAMP},
      {""},
      {"TZOFFSET", M_TZOFFSET},
      {""},
      {"R_TZOFFSET", M_TZOFFSET_RECVD},
      {""}, {""},
      {"UNIXTIME", M_UNIXTIME},
      {""},
      {"S_TZOFFSET", M_TZOFFSET_STAMP},
      {""},
      {"WEEKDAY", M_WEEKDAY},
      {"DAY", M_DAY},
      {"R_ISODATE", M_ISODATE_RECVD},
      {"R_DAY", M_DAY_RECVD},
      {""}, {""},
      {"FULLDATE", M_FULLDATE},
      {"S_ISODATE", M_ISODATE_STAMP},
      {"S_DAY", M_DAY_STAMP},
      {""}, {""},
      {"PRI", M_PRI},
      {"DATE", M_DATE},
      {"LEVEL", M_LEVEL},
      {"R_DATE", M_DATE_RECVD},
      {""},
      {"PRIORITY", M_LEVEL},
      {"LEVEL_NUM", M_LEVEL_NUM},
      {""},
      {"S_DATE", M_DATE_STAMP},
      {"PROGRAM", M_PROGRAM},
      {""}, {""}, {""}, {""},
      {"ISODATE", M_ISODATE}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        if (len == lengthtable[key])
          {
            register const char *s = wordlist[key].name;

            if (*str == *s && !memcmp (str + 1, s + 1, len - 1))
              return &wordlist[key];
          }
    }
  return 0;
}
