/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 26 "cfg-grammar.y"


#include "cfgfile.h"
#include "filters.h"
#include "syslog-names.h"
#include "format.h"
#include "afinter.h"
#include "affile.h"
#include "afsocket.h"
#include "afuser.h"
#include "afstreams.h"
#include "afprogram.h"
#include "afremctrl.h"

#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

void yyerror(char *msg);
int yylex();

static struct log_source_driver *last_source_driver;
static struct log_dest_driver *last_dest_driver;
UINT8 *last_prefix = NULL;
UINT32 last_pad_size;

UINT8 *
get_last_log_prefix(void)
{
	UINT8 *res = last_prefix;
	last_prefix = NULL;
	return res;
}



/* Line 268 of yacc.c  */
#line 108 "cfg-grammar.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     KW_SOURCE = 258,
     KW_DESTINATION = 259,
     KW_LOG = 260,
     KW_OPTIONS = 261,
     KW_FILTER = 262,
     KW_INTERNAL = 263,
     KW_FILE = 264,
     KW_PIPE = 265,
     KW_UNIX_STREAM = 266,
     KW_UNIX_DGRAM = 267,
     KW_UDP = 268,
     KW_TCP = 269,
     KW_USER = 270,
     KW_SPOOF_SOURCE = 271,
     KW_SO_RCVBUF = 272,
     KW_DOOR = 273,
     KW_SUN_STREAMS = 274,
     KW_PROGRAM = 275,
     KW_REMCTRL = 276,
     KW_FLAGS = 277,
     KW_CATCHALL = 278,
     KW_FALLBACK = 279,
     KW_FINAL = 280,
     KW_FSYNC = 281,
     KW_MARK_FREQ = 282,
     KW_SYNC_FREQ = 283,
     KW_STATS_FREQ = 284,
     KW_CHAIN_HOSTNAMES = 285,
     KW_KEEP_HOSTNAME = 286,
     KW_CHECK_HOSTNAME = 287,
     KW_BAD_HOSTNAME = 288,
     KW_LOG_FIFO_SIZE = 289,
     KW_LOG_MSG_SIZE = 290,
     KW_TIME_REOPEN = 291,
     KW_TIME_REAP = 292,
     KW_TIME_SLEEP = 293,
     KW_USE_TIME_RECVD = 294,
     KW_USE_DNS = 295,
     KW_USE_FQDN = 296,
     KW_GC_BUSY_THRESHOLD = 297,
     KW_GC_IDLE_THRESHOLD = 298,
     KW_CREATE_DIRS = 299,
     KW_SANITIZE_FILENAMES = 300,
     KW_DIR_OWNER = 301,
     KW_DIR_GROUP = 302,
     KW_DIR_PERM = 303,
     KW_TEMPLATE = 304,
     KW_TEMPLATE_ESCAPE = 305,
     KW_OWNER = 306,
     KW_GROUP = 307,
     KW_PERM = 308,
     KW_KEEP_ALIVE = 309,
     KW_TCP_KEEP_ALIVE = 310,
     KW_MAX_CONNECTIONS = 311,
     KW_LOCALIP = 312,
     KW_IP = 313,
     KW_LOCALPORT = 314,
     KW_PORT = 315,
     KW_DESTPORT = 316,
     KW_COMPRESS = 317,
     KW_MAC = 318,
     KW_AUTH = 319,
     KW_ENCRYPT = 320,
     KW_DNS_CACHE = 321,
     KW_DNS_CACHE_SIZE = 322,
     KW_DNS_CACHE_EXPIRE = 323,
     KW_DNS_CACHE_EXPIRE_FAILED = 324,
     KW_REMOVE_IF_OLDER = 325,
     KW_LOG_PREFIX = 326,
     KW_PAD_SIZE = 327,
     KW_FILE_SIZE_LIMIT = 328,
     KW_SIZE_LIMIT = 329,
     KW_STOP_ON_FULL = 330,
     KW_FACILITY = 331,
     KW_LEVEL = 332,
     KW_NETMASK = 333,
     KW_HOST = 334,
     KW_MATCH = 335,
     KW_YES = 336,
     KW_NO = 337,
     KW_REQUIRED = 338,
     KW_ALLOW = 339,
     KW_DENY = 340,
     DOTDOT = 341,
     IDENTIFIER = 342,
     NUMBER = 343,
     STRING = 344,
     KW_OR = 345,
     KW_AND = 346,
     KW_NOT = 347
   };
#endif
/* Tokens.  */
#define KW_SOURCE 258
#define KW_DESTINATION 259
#define KW_LOG 260
#define KW_OPTIONS 261
#define KW_FILTER 262
#define KW_INTERNAL 263
#define KW_FILE 264
#define KW_PIPE 265
#define KW_UNIX_STREAM 266
#define KW_UNIX_DGRAM 267
#define KW_UDP 268
#define KW_TCP 269
#define KW_USER 270
#define KW_SPOOF_SOURCE 271
#define KW_SO_RCVBUF 272
#define KW_DOOR 273
#define KW_SUN_STREAMS 274
#define KW_PROGRAM 275
#define KW_REMCTRL 276
#define KW_FLAGS 277
#define KW_CATCHALL 278
#define KW_FALLBACK 279
#define KW_FINAL 280
#define KW_FSYNC 281
#define KW_MARK_FREQ 282
#define KW_SYNC_FREQ 283
#define KW_STATS_FREQ 284
#define KW_CHAIN_HOSTNAMES 285
#define KW_KEEP_HOSTNAME 286
#define KW_CHECK_HOSTNAME 287
#define KW_BAD_HOSTNAME 288
#define KW_LOG_FIFO_SIZE 289
#define KW_LOG_MSG_SIZE 290
#define KW_TIME_REOPEN 291
#define KW_TIME_REAP 292
#define KW_TIME_SLEEP 293
#define KW_USE_TIME_RECVD 294
#define KW_USE_DNS 295
#define KW_USE_FQDN 296
#define KW_GC_BUSY_THRESHOLD 297
#define KW_GC_IDLE_THRESHOLD 298
#define KW_CREATE_DIRS 299
#define KW_SANITIZE_FILENAMES 300
#define KW_DIR_OWNER 301
#define KW_DIR_GROUP 302
#define KW_DIR_PERM 303
#define KW_TEMPLATE 304
#define KW_TEMPLATE_ESCAPE 305
#define KW_OWNER 306
#define KW_GROUP 307
#define KW_PERM 308
#define KW_KEEP_ALIVE 309
#define KW_TCP_KEEP_ALIVE 310
#define KW_MAX_CONNECTIONS 311
#define KW_LOCALIP 312
#define KW_IP 313
#define KW_LOCALPORT 314
#define KW_PORT 315
#define KW_DESTPORT 316
#define KW_COMPRESS 317
#define KW_MAC 318
#define KW_AUTH 319
#define KW_ENCRYPT 320
#define KW_DNS_CACHE 321
#define KW_DNS_CACHE_SIZE 322
#define KW_DNS_CACHE_EXPIRE 323
#define KW_DNS_CACHE_EXPIRE_FAILED 324
#define KW_REMOVE_IF_OLDER 325
#define KW_LOG_PREFIX 326
#define KW_PAD_SIZE 327
#define KW_FILE_SIZE_LIMIT 328
#define KW_SIZE_LIMIT 329
#define KW_STOP_ON_FULL 330
#define KW_FACILITY 331
#define KW_LEVEL 332
#define KW_NETMASK 333
#define KW_HOST 334
#define KW_MATCH 335
#define KW_YES 336
#define KW_NO 337
#define KW_REQUIRED 338
#define KW_ALLOW 339
#define KW_DENY 340
#define DOTDOT 341
#define IDENTIFIER 342
#define NUMBER 343
#define STRING 344
#define KW_OR 345
#define KW_AND 346
#define KW_NOT 347




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 62 "cfg-grammar.y"

	UINT32 num;
	char *cptr;
	void *ptr;
	struct filter_expr_node *node;



/* Line 293 of yacc.c  */
#line 337 "cfg-grammar.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 349 "cfg-grammar.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  22
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   578

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  93
/* YYNRULES -- Number of rules.  */
#define YYNRULES  242
/* YYNRULES -- Number of states.  */
#define YYNSTATES  644

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   347

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      96,    97,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    93,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    94,     2,    95,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     9,    10,    13,    16,    19,    22,
      25,    30,    35,    40,    44,    48,    49,    51,    53,    55,
      57,    61,    67,    73,    76,    77,    82,    87,    92,    97,
     102,   107,   108,   112,   113,   117,   120,   121,   126,   131,
     136,   138,   139,   142,   145,   146,   148,   153,   158,   163,
     168,   173,   178,   183,   184,   187,   190,   191,   193,   198,
     203,   208,   213,   218,   223,   225,   230,   235,   240,   241,
     245,   248,   249,   254,   258,   259,   261,   263,   265,   267,
     269,   271,   276,   281,   282,   286,   289,   290,   292,   297,
     302,   307,   312,   317,   322,   327,   332,   337,   342,   347,
     352,   357,   362,   367,   372,   377,   378,   382,   385,   386,
     391,   396,   401,   406,   411,   416,   421,   426,   431,   436,
     441,   444,   445,   446,   450,   451,   455,   456,   460,   463,
     464,   466,   471,   476,   481,   486,   491,   496,   498,   503,
     508,   513,   518,   519,   523,   526,   527,   529,   534,   539,
     544,   549,   554,   559,   564,   569,   574,   575,   579,   584,
     589,   592,   593,   598,   599,   603,   608,   613,   616,   617,
     621,   625,   626,   631,   636,   641,   647,   648,   651,   652,
     654,   656,   658,   662,   663,   668,   673,   678,   683,   688,
     693,   698,   703,   708,   713,   718,   723,   728,   733,   738,
     743,   748,   753,   758,   763,   768,   773,   778,   783,   788,
     793,   798,   803,   808,   814,   816,   819,   823,   827,   831,
     836,   841,   846,   851,   856,   861,   866,   871,   874,   876,
     878,   881,   883,   887,   889,   891,   893,   895,   897,   899,
     901,   903,   905
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      99,     0,    -1,   100,    -1,   101,    93,   100,    -1,    -1,
       3,   102,    -1,     4,   103,    -1,     5,   104,    -1,     7,
     181,    -1,     6,   105,    -1,   190,    94,   106,    95,    -1,
     190,    94,   134,    95,    -1,    94,   174,   176,    95,    -1,
      94,   179,    95,    -1,   107,    93,   106,    -1,    -1,   108,
      -1,   109,    -1,   112,    -1,   129,    -1,     8,    96,    97,
      -1,     9,    96,   190,   110,    97,    -1,    10,    96,   190,
     110,    97,    -1,   111,   110,    -1,    -1,    71,    96,   190,
      97,    -1,    72,    96,    88,    97,    -1,    12,    96,   113,
      97,    -1,    11,    96,   115,    97,    -1,    13,    96,   119,
      97,    -1,    14,    96,   124,    97,    -1,    -1,   190,   114,
     117,    -1,    -1,   190,   116,   117,    -1,   118,   117,    -1,
      -1,    51,    96,   190,    97,    -1,    52,    96,   190,    97,
      -1,    53,    96,    88,    97,    -1,   128,    -1,    -1,   120,
     121,    -1,   122,   121,    -1,    -1,   123,    -1,    59,    96,
     190,    97,    -1,    60,    96,   190,    97,    -1,    17,    96,
      88,    97,    -1,    57,    96,   190,    97,    -1,    59,    96,
      88,    97,    -1,    60,    96,    88,    97,    -1,    58,    96,
     190,    97,    -1,    -1,   125,   126,    -1,   127,   126,    -1,
      -1,   123,    -1,    55,    96,   188,    97,    -1,    59,    96,
     190,    97,    -1,    60,    96,   190,    97,    -1,    64,    96,
     189,    97,    -1,    63,    96,   189,    97,    -1,    65,    96,
     189,    97,    -1,   128,    -1,    54,    96,   188,    97,    -1,
      56,    96,    88,    97,    -1,    19,    96,   130,    97,    -1,
      -1,   190,   131,   132,    -1,   133,   132,    -1,    -1,    18,
      96,   190,    97,    -1,   135,    93,   134,    -1,    -1,   137,
      -1,   142,    -1,   147,    -1,   163,    -1,   168,    -1,   173,
      -1,    34,    96,    88,    97,    -1,     9,    96,   138,    97,
      -1,    -1,   190,   139,   140,    -1,   141,   140,    -1,    -1,
     136,    -1,    28,    96,    88,    97,    -1,    62,    96,   188,
      97,    -1,    65,    96,   188,    97,    -1,    51,    96,   190,
      97,    -1,    52,    96,   190,    97,    -1,    53,    96,    88,
      97,    -1,    46,    96,   190,    97,    -1,    47,    96,   190,
      97,    -1,    48,    96,    88,    97,    -1,    44,    96,   188,
      97,    -1,    70,    96,    88,    97,    -1,    49,    96,   190,
      97,    -1,    50,    96,   188,    97,    -1,    26,    96,   188,
      97,    -1,    74,    96,    88,    97,    -1,    75,    96,   188,
      97,    -1,    10,    96,   143,    97,    -1,    -1,   190,   144,
     145,    -1,   146,   145,    -1,    -1,    51,    96,   190,    97,
      -1,    52,    96,   190,    97,    -1,    53,    96,    88,    97,
      -1,    49,    96,   190,    97,    -1,    50,    96,   188,    97,
      -1,    12,    96,   150,    97,    -1,    11,    96,   152,    97,
      -1,    13,    96,   154,    97,    -1,    14,    96,   159,    97,
      -1,    49,    96,   190,    97,    -1,    50,    96,   188,    97,
      -1,   149,   148,    -1,    -1,    -1,   190,   151,   149,    -1,
      -1,   190,   153,   149,    -1,    -1,   190,   155,   156,    -1,
     156,   158,    -1,    -1,   136,    -1,    57,    96,   190,    97,
      -1,    59,    96,    88,    97,    -1,    60,    96,    88,    97,
      -1,    61,    96,    88,    97,    -1,    49,    96,   190,    97,
      -1,    50,    96,   188,    97,    -1,   157,    -1,    59,    96,
     190,    97,    -1,    60,    96,   190,    97,    -1,    61,    96,
     190,    97,    -1,    16,    96,   188,    97,    -1,    -1,   190,
     160,   161,    -1,   161,   162,    -1,    -1,   157,    -1,    55,
      96,   188,    97,    -1,    59,    96,   190,    97,    -1,    60,
      96,   190,    97,    -1,    61,    96,   190,    97,    -1,    63,
      96,   188,    97,    -1,    64,    96,   188,    97,    -1,    65,
      96,   188,    97,    -1,    28,    96,    88,    97,    -1,    15,
      96,   164,    97,    -1,    -1,   190,   165,   167,    -1,    49,
      96,   190,    97,    -1,    50,    96,   188,    97,    -1,   167,
     166,    -1,    -1,    20,    96,   169,    97,    -1,    -1,   190,
     170,   172,    -1,    49,    96,   190,    97,    -1,    50,    96,
     188,    97,    -1,   172,   171,    -1,    -1,    21,    96,    97,
      -1,   175,    93,   174,    -1,    -1,     3,    96,   190,    97,
      -1,     7,    96,   190,    97,    -1,     4,    96,   190,    97,
      -1,    22,    96,   177,    97,    93,    -1,    -1,   178,   177,
      -1,    -1,    23,    -1,    24,    -1,    25,    -1,   180,    93,
     179,    -1,    -1,    27,    96,    88,    97,    -1,    28,    96,
      88,    97,    -1,    29,    96,    88,    97,    -1,    30,    96,
     188,    97,    -1,    31,    96,   188,    97,    -1,    32,    96,
     188,    97,    -1,    33,    96,    89,    97,    -1,    39,    96,
     188,    97,    -1,    41,    96,   188,    97,    -1,    40,    96,
     188,    97,    -1,    36,    96,    88,    97,    -1,    38,    96,
      88,    97,    -1,    37,    96,    88,    97,    -1,    34,    96,
      88,    97,    -1,    42,    96,    88,    97,    -1,    43,    96,
      88,    97,    -1,    44,    96,   188,    97,    -1,    45,    96,
     188,    97,    -1,    51,    96,   190,    97,    -1,    52,    96,
     190,    97,    -1,    53,    96,    88,    97,    -1,    46,    96,
     190,    97,    -1,    47,    96,   190,    97,    -1,    48,    96,
      88,    97,    -1,    66,    96,   188,    97,    -1,    67,    96,
      88,    97,    -1,    68,    96,    88,    97,    -1,    69,    96,
      88,    97,    -1,    35,    96,    88,    97,    -1,   190,    94,
     182,    93,    95,    -1,   183,    -1,    92,   182,    -1,   182,
      90,   182,    -1,   182,    91,   182,    -1,    96,   182,    97,
      -1,    76,    96,   184,    97,    -1,    76,    96,    88,    97,
      -1,    77,    96,   186,    97,    -1,    20,    96,   190,    97,
      -1,    78,    96,   190,    97,    -1,    79,    96,   190,    97,
      -1,    80,    96,   190,    97,    -1,     7,    96,   190,    97,
      -1,   185,   184,    -1,   185,    -1,    87,    -1,   187,   186,
      -1,   187,    -1,    87,    86,    87,    -1,    87,    -1,    81,
      -1,    82,    -1,    88,    -1,    83,    -1,    84,    -1,    85,
      -1,    87,    -1,    89,    -1,    88,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   174,   174,   178,   179,   183,   184,   185,   186,   187,
     191,   195,   199,   203,   207,   208,   212,   213,   214,   215,
     219,   223,   228,   236,   237,   241,   242,   246,   247,   248,
     249,   254,   253,   265,   264,   276,   277,   281,   282,   283,
     284,   289,   289,   298,   299,   303,   304,   305,   306,   310,
     311,   312,   313,   318,   318,   327,   328,   332,   333,   334,
     335,   336,   337,   338,   339,   343,   344,   348,   353,   352,
     361,   362,   366,   370,   371,   375,   376,   377,   378,   379,
     380,   384,   388,   393,   392,   402,   403,   407,   408,   409,
     410,   411,   412,   413,   414,   415,   416,   417,   418,   419,
     420,   421,   422,   423,   427,   432,   431,   441,   442,   446,
     447,   448,   449,   450,   455,   456,   457,   458,   463,   464,
     468,   469,   473,   473,   482,   482,   492,   491,   502,   503,
     507,   508,   509,   510,   511,   512,   513,   517,   518,   519,
     520,   521,   526,   525,   536,   537,   541,   542,   543,   544,
     545,   546,   547,   548,   549,   553,   557,   557,   562,   563,
     567,   568,   572,   576,   576,   581,   582,   586,   587,   591,
     595,   596,   600,   601,   602,   606,   607,   612,   613,   617,
     618,   619,   623,   624,   628,   629,   630,   631,   632,   633,
     634,   635,   636,   637,   638,   639,   640,   641,   642,   643,
     644,   645,   646,   647,   648,   649,   650,   651,   652,   653,
     654,   655,   657,   661,   665,   666,   667,   668,   669,   673,
     674,   675,   676,   677,   678,   679,   680,   684,   685,   689,
     704,   705,   709,   729,   744,   745,   746,   750,   751,   752,
     756,   757,   758
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "KW_SOURCE", "KW_DESTINATION", "KW_LOG",
  "KW_OPTIONS", "KW_FILTER", "KW_INTERNAL", "KW_FILE", "KW_PIPE",
  "KW_UNIX_STREAM", "KW_UNIX_DGRAM", "KW_UDP", "KW_TCP", "KW_USER",
  "KW_SPOOF_SOURCE", "KW_SO_RCVBUF", "KW_DOOR", "KW_SUN_STREAMS",
  "KW_PROGRAM", "KW_REMCTRL", "KW_FLAGS", "KW_CATCHALL", "KW_FALLBACK",
  "KW_FINAL", "KW_FSYNC", "KW_MARK_FREQ", "KW_SYNC_FREQ", "KW_STATS_FREQ",
  "KW_CHAIN_HOSTNAMES", "KW_KEEP_HOSTNAME", "KW_CHECK_HOSTNAME",
  "KW_BAD_HOSTNAME", "KW_LOG_FIFO_SIZE", "KW_LOG_MSG_SIZE",
  "KW_TIME_REOPEN", "KW_TIME_REAP", "KW_TIME_SLEEP", "KW_USE_TIME_RECVD",
  "KW_USE_DNS", "KW_USE_FQDN", "KW_GC_BUSY_THRESHOLD",
  "KW_GC_IDLE_THRESHOLD", "KW_CREATE_DIRS", "KW_SANITIZE_FILENAMES",
  "KW_DIR_OWNER", "KW_DIR_GROUP", "KW_DIR_PERM", "KW_TEMPLATE",
  "KW_TEMPLATE_ESCAPE", "KW_OWNER", "KW_GROUP", "KW_PERM", "KW_KEEP_ALIVE",
  "KW_TCP_KEEP_ALIVE", "KW_MAX_CONNECTIONS", "KW_LOCALIP", "KW_IP",
  "KW_LOCALPORT", "KW_PORT", "KW_DESTPORT", "KW_COMPRESS", "KW_MAC",
  "KW_AUTH", "KW_ENCRYPT", "KW_DNS_CACHE", "KW_DNS_CACHE_SIZE",
  "KW_DNS_CACHE_EXPIRE", "KW_DNS_CACHE_EXPIRE_FAILED",
  "KW_REMOVE_IF_OLDER", "KW_LOG_PREFIX", "KW_PAD_SIZE",
  "KW_FILE_SIZE_LIMIT", "KW_SIZE_LIMIT", "KW_STOP_ON_FULL", "KW_FACILITY",
  "KW_LEVEL", "KW_NETMASK", "KW_HOST", "KW_MATCH", "KW_YES", "KW_NO",
  "KW_REQUIRED", "KW_ALLOW", "KW_DENY", "DOTDOT", "IDENTIFIER", "NUMBER",
  "STRING", "KW_OR", "KW_AND", "KW_NOT", "';'", "'{'", "'}'", "'('", "')'",
  "$accept", "start", "stmts", "stmt", "source_stmt", "dest_stmt",
  "log_stmt", "options_stmt", "source_items", "source_item",
  "source_afinter", "source_affile", "source_affile_options",
  "source_affile_option", "source_afsocket", "source_afunix_dgram_params",
  "$@1", "source_afunix_stream_params", "$@2", "source_afunix_options",
  "source_afunix_option", "source_afinet_udp_params", "$@3",
  "source_afinet_udp_options", "source_afinet_udp_option",
  "source_afinet_option", "source_afinet_tcp_params", "$@4",
  "source_afinet_tcp_options", "source_afinet_tcp_option",
  "source_afsocket_stream_params", "source_afstreams",
  "source_afstreams_params", "$@5", "source_afstreams_options",
  "source_afstreams_option", "dest_items", "dest_item", "dest_item_option",
  "dest_affile", "dest_affile_params", "$@6", "dest_affile_options",
  "dest_affile_option", "dest_afpipe", "dest_afpipe_params", "$@7",
  "dest_afpipe_options", "dest_afpipe_option", "dest_afsocket",
  "dest_afunix_option", "dest_afunix_options", "dest_afunix_dgram_params",
  "$@8", "dest_afunix_stream_params", "$@9", "dest_afinet_udp_params",
  "$@10", "dest_afinet_udp_options", "dest_afinet_option",
  "dest_afinet_udp_option", "dest_afinet_tcp_params", "$@11",
  "dest_afinet_tcp_options", "dest_afinet_tcp_option", "dest_afuser",
  "dest_afuser_params", "$@12", "dest_afuser_option",
  "dest_afuser_options", "dest_afprogram", "dest_afprogram_params", "$@13",
  "dest_afprogram_option", "dest_afprogram_options", "dest_afremctrl",
  "log_items", "log_item", "log_flags", "log_flags_items",
  "log_flags_item", "options_items", "options_item", "filter_stmt",
  "filter_expr", "filter_simple_expr", "filter_fac_list", "filter_fac",
  "filter_level_list", "filter_level", "yesno", "tripleoption", "string", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,    59,   123,   125,    40,    41
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    98,    99,   100,   100,   101,   101,   101,   101,   101,
     102,   103,   104,   105,   106,   106,   107,   107,   107,   107,
     108,   109,   109,   110,   110,   111,   111,   112,   112,   112,
     112,   114,   113,   116,   115,   117,   117,   118,   118,   118,
     118,   120,   119,   121,   121,   122,   122,   122,   122,   123,
     123,   123,   123,   125,   124,   126,   126,   127,   127,   127,
     127,   127,   127,   127,   127,   128,   128,   129,   131,   130,
     132,   132,   133,   134,   134,   135,   135,   135,   135,   135,
     135,   136,   137,   139,   138,   140,   140,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   142,   144,   143,   145,   145,   146,
     146,   146,   146,   146,   147,   147,   147,   147,   148,   148,
     149,   149,   151,   150,   153,   152,   155,   154,   156,   156,
     157,   157,   157,   157,   157,   157,   157,   158,   158,   158,
     158,   158,   160,   159,   161,   161,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   163,   165,   164,   166,   166,
     167,   167,   168,   170,   169,   171,   171,   172,   172,   173,
     174,   174,   175,   175,   175,   176,   176,   177,   177,   178,
     178,   178,   179,   179,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   180,   180,   180,   180,   180,   180,   180,
     180,   180,   180,   181,   182,   182,   182,   182,   182,   183,
     183,   183,   183,   183,   183,   183,   183,   184,   184,   185,
     186,   186,   187,   187,   188,   188,   188,   189,   189,   189,
     190,   190,   190
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     0,     2,     2,     2,     2,     2,
       4,     4,     4,     3,     3,     0,     1,     1,     1,     1,
       3,     5,     5,     2,     0,     4,     4,     4,     4,     4,
       4,     0,     3,     0,     3,     2,     0,     4,     4,     4,
       1,     0,     2,     2,     0,     1,     4,     4,     4,     4,
       4,     4,     4,     0,     2,     2,     0,     1,     4,     4,
       4,     4,     4,     4,     1,     4,     4,     4,     0,     3,
       2,     0,     4,     3,     0,     1,     1,     1,     1,     1,
       1,     4,     4,     0,     3,     2,     0,     1,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     0,     3,     2,     0,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       2,     0,     0,     3,     0,     3,     0,     3,     2,     0,
       1,     4,     4,     4,     4,     4,     4,     1,     4,     4,
       4,     4,     0,     3,     2,     0,     1,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     0,     3,     4,     4,
       2,     0,     4,     0,     3,     4,     4,     2,     0,     3,
       3,     0,     4,     4,     4,     5,     0,     2,     0,     1,
       1,     1,     3,     0,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     5,     1,     2,     3,     3,     3,     4,
       4,     4,     4,     4,     4,     4,     4,     2,     1,     1,
       2,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     0,     0,     0,     0,     0,     2,     0,   240,
     242,   241,     5,     0,     6,     0,   171,     7,   183,     9,
       8,     0,     1,     4,    15,    74,     0,     0,     0,   176,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     3,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    16,    17,    18,    19,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    75,
      76,    77,    78,    79,    80,     0,     0,     0,     0,     0,
     171,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      13,   183,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   214,     0,     0,     0,     0,     0,    41,    53,
       0,    10,    15,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    11,    74,     0,     0,     0,   178,    12,   170,
       0,     0,     0,   234,   235,   236,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   182,     0,     0,     0,     0,     0,     0,     0,
     215,     0,     0,     0,     0,    20,    24,    24,     0,    33,
       0,    31,     0,    44,     0,    56,     0,    68,    14,     0,
      83,     0,   105,     0,   124,     0,   122,     0,   126,     0,
     142,     0,   156,     0,   163,   169,    73,   172,   174,   173,
     179,   180,   181,     0,   178,   184,   185,   186,   187,   188,
     189,   190,   197,   212,   194,   196,   195,   191,   193,   192,
     198,   199,   200,   201,   205,   206,   207,   202,   203,   204,
     208,   209,   210,   211,     0,     0,   229,     0,     0,   228,
     233,     0,   231,     0,     0,     0,   218,   216,   217,   213,
       0,     0,     0,    24,     0,    28,    36,    27,    36,    29,
       0,     0,     0,     0,     0,    42,    44,    45,    30,     0,
       0,     0,     0,     0,     0,     0,     0,    57,    54,    56,
      64,    67,    71,    82,    86,   104,   108,   115,   121,   114,
     121,   116,   129,   117,   145,   155,   161,   162,   168,     0,
     177,   226,   222,   220,   219,   227,     0,   221,   230,   223,
     224,   225,     0,     0,    21,    23,    22,     0,     0,     0,
      34,    36,    40,    32,     0,     0,     0,     0,     0,    43,
       0,     0,     0,     0,     0,     0,     0,     0,    55,     0,
      69,    71,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      84,    86,     0,     0,     0,     0,     0,   106,   108,   125,
     123,   127,   143,   157,   164,   175,   232,     0,     0,     0,
       0,     0,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   237,   238,   239,     0,     0,
       0,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,   107,     0,     0,   120,
       0,     0,     0,     0,     0,     0,     0,   130,   137,   128,
       0,     0,     0,     0,     0,     0,     0,     0,   146,   144,
       0,     0,   160,     0,     0,   167,    25,    26,     0,     0,
       0,    48,    49,    52,    50,    46,    51,    47,    65,    58,
      66,    59,    60,    62,    61,    63,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,    38,    39,    72,   101,    88,    81,    97,    94,    95,
      96,    99,   100,    91,    92,    93,    89,    90,    98,   102,
     103,   112,   113,   109,   110,   111,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     118,   119,   141,   135,   136,   131,   132,   138,   133,   139,
     134,   140,   154,   147,   148,   149,   150,   151,   152,   153,
     158,   159,   165,   166
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     6,     7,     8,    12,    14,    17,    19,    72,    73,
      74,    75,   302,   303,    76,   220,   308,   218,   306,   370,
     371,   222,   223,   315,   316,   317,   224,   225,   328,   329,
     372,    77,   226,   332,   390,   391,    87,    88,   409,    89,
     229,   334,   410,   411,    90,   231,   336,   417,   418,    91,
     479,   419,   235,   340,   233,   338,   237,   342,   421,   488,
     489,   239,   344,   422,   499,    92,   241,   346,   502,   423,
      93,   243,   348,   505,   424,    94,    29,    30,    99,   253,
     254,    60,    61,    20,   141,   142,   288,   289,   291,   292,
     176,   448,    13
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -304
static const yytype_int16 yypact[] =
{
      67,    14,    14,   -88,   -82,    14,    16,  -304,   -65,  -304,
    -304,  -304,  -304,   -58,  -304,   -54,    11,  -304,   142,  -304,
    -304,   -39,  -304,    67,   233,    12,   -33,   -21,    -8,    82,
      15,    21,    24,    36,    66,    96,   100,   104,   107,   108,
     118,   119,   120,   121,   123,   125,   129,   140,   141,   152,
     153,   154,   155,   157,   158,   159,   160,   161,   162,   164,
     166,   169,     0,  -304,   167,   168,   170,   171,   172,   173,
     174,   176,   178,   181,  -304,  -304,  -304,  -304,   179,   180,
     182,   183,   184,   185,   186,   187,   188,   190,   193,  -304,
    -304,  -304,  -304,  -304,  -304,    14,    14,    14,   191,   194,
      11,   177,   189,   200,   -71,   -71,   -71,   201,   203,   204,
     205,   206,   207,   -71,   -71,   -71,   208,   209,   -71,   -71,
      14,    14,   210,    14,    14,   211,   -71,   212,   213,   214,
    -304,   142,   215,   216,   217,   218,   219,   220,   221,     0,
       0,   -42,  -304,   222,    14,    14,    14,    14,  -304,  -304,
      14,  -304,   233,    14,    14,    14,    14,    14,    14,    14,
      14,   223,  -304,    12,   224,   225,   226,    75,  -304,  -304,
     227,   228,   229,  -304,  -304,  -304,   230,   231,   232,   234,
     235,   236,   237,   238,   239,   240,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,  -304,    14,    14,   -43,   267,    14,    14,    14,
    -304,   -56,     0,     0,   260,  -304,   -19,   -19,   259,  -304,
     261,  -304,   262,     2,   264,   175,   265,  -304,  -304,   266,
    -304,   268,  -304,   269,  -304,   270,  -304,   271,  -304,   272,
    -304,   273,  -304,   274,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,   275,    75,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,   281,   282,  -304,   286,   287,   277,
     300,   290,   267,   291,   294,   295,  -304,   303,  -304,  -304,
     299,   301,   302,   -19,   304,  -304,    13,  -304,    13,  -304,
     306,   307,   308,   309,   310,  -304,     2,  -304,  -304,   311,
     312,   313,   314,   315,   316,   317,   318,  -304,  -304,   175,
    -304,  -304,   285,  -304,    63,  -304,    34,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,   305,
    -304,  -304,  -304,  -304,  -304,  -304,   328,  -304,  -304,  -304,
    -304,  -304,    14,   329,  -304,  -304,  -304,   320,   322,   323,
    -304,    13,  -304,  -304,   332,    14,    14,    42,    47,  -304,
     -71,   -71,   333,    42,    47,    56,    56,    56,  -304,   326,
    -304,   285,   327,   330,   334,   335,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   345,   346,   347,   348,  -304,
    -304,    63,   349,   350,   351,   352,   354,  -304,    34,    74,
      74,    -3,   163,    77,    97,  -304,  -304,   355,   356,    14,
      14,   363,  -304,   357,   360,   362,   365,   366,   367,   368,
     369,   370,   371,   373,   376,  -304,  -304,  -304,   379,   381,
     382,    14,  -304,   -71,   392,   393,   -71,    14,    14,   394,
      14,   -71,    14,    14,   396,   -71,   -71,   397,   398,   -71,
    -304,    14,   -71,    14,    14,   402,  -304,   399,   400,  -304,
     401,   403,   404,   405,   406,   407,   408,  -304,  -304,  -304,
     409,   410,   411,   412,   413,   414,   415,   416,  -304,  -304,
     417,   418,  -304,   419,   420,  -304,  -304,  -304,   395,   421,
     422,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,   423,   424,   425,   426,
     427,   428,   429,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,   443,   444,   445,    14,
     -71,   -71,    14,   -71,    14,    72,    76,    79,   455,   -71,
      72,    76,    79,   -71,   -71,   -71,    14,   -71,    14,   -71,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,   447,   448,   449,   451,
     452,   454,   459,   460,   464,   465,   466,   468,   470,   471,
     472,   473,   474,   475,   476,   477,   478,   479,   480,   481,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -304,  -304,   283,  -304,  -304,  -304,  -304,  -304,   156,  -304,
    -304,  -304,  -213,  -304,  -304,  -304,  -304,  -304,  -304,  -303,
    -304,  -304,  -304,   -57,  -304,  -224,  -304,  -304,   -22,  -304,
    -223,  -304,  -304,  -304,  -120,  -304,   146,  -304,  -272,  -304,
    -304,  -304,  -101,  -304,  -304,  -304,  -304,  -100,  -304,  -304,
    -304,   -10,  -304,  -304,  -304,  -304,  -304,  -304,  -304,     3,
    -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,  -304,
    -304,  -304,  -304,  -304,  -304,  -304,   257,  -304,  -304,   263,
    -304,   293,  -304,  -304,  -131,  -304,   111,  -304,   137,  -304,
     -76,  -188,    -2
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
      15,   327,   330,    21,   304,   373,    16,   132,   210,   211,
     173,   174,    18,   480,    26,    27,    22,   175,    28,   310,
     133,    78,    79,    80,    81,    82,    83,    84,    23,   177,
     178,   394,    85,    86,   212,   213,    24,   185,   186,   187,
      25,   296,   190,   191,   286,   287,   481,   482,   212,   213,
     198,   214,   300,   301,   483,    62,   484,   485,   486,   311,
     312,   313,   314,    95,   367,   368,   369,   319,   432,   321,
       1,     2,     3,     4,     5,    96,   134,   135,   136,   137,
     138,   297,   298,   412,   413,   414,   415,   416,    97,   392,
     365,   393,   139,   164,   165,   166,   140,   394,   250,   251,
     252,     9,    10,    11,    98,   327,   330,   395,   100,   396,
     397,   398,   399,   400,   401,   402,   403,   101,   192,   193,
     102,   195,   196,   477,   478,   404,   500,   501,   405,     9,
     436,    11,   103,   406,     9,   438,    11,   407,   408,   445,
     446,   447,   216,   217,   219,   221,   503,   504,   227,   487,
     487,   230,   232,   234,   236,   238,   240,   242,   244,     9,
     602,    11,   104,     9,   604,    11,     9,   606,    11,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,   490,   105,    53,    54,    55,   106,   394,   449,   450,
     107,   284,   285,   108,   109,   293,   294,   295,    56,    57,
      58,    59,   481,   482,   110,   111,   112,   113,   491,   114,
     483,   115,   492,   493,   494,   116,   495,   496,   497,   319,
     320,   321,   311,   312,   322,   323,   117,   118,   324,   325,
     326,    64,    65,    66,    67,    68,    69,    70,   119,   120,
     121,   122,    71,   123,   124,   125,   126,   127,   128,   379,
     129,   130,   131,   143,   144,   170,   145,   146,   147,   148,
     149,   452,   150,   151,   152,   153,   154,   171,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   167,   172,   168,
     179,   180,   181,   182,   183,   184,   188,   189,   194,   197,
     199,   200,   201,   389,   440,   441,    63,   388,   228,   246,
     470,   203,   204,   205,   206,   207,   208,   209,   476,   215,
     245,   247,   248,   249,   255,   256,   257,   258,   259,   260,
     420,   261,   262,   263,   264,   265,   266,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   277,   278,   279,
     280,   281,   282,   283,   290,   299,   305,   169,   307,   309,
     427,   318,   331,   333,   286,   335,   337,   339,   341,   343,
     345,   347,   349,   434,   435,   437,   439,   527,   351,   352,
     530,   443,   444,   353,   354,   535,   356,   357,   359,   539,
     540,   360,   361,   543,   213,   362,   545,   363,   425,   364,
     355,   366,   374,   375,   376,   377,   378,   380,   381,   382,
     383,   384,   385,   386,   387,   426,   429,   428,   430,   431,
     433,   442,   451,   453,   202,   498,   454,   508,   509,   358,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   471,   472,   473,   474,   526,
     475,   510,   506,   507,   511,   531,   532,   512,   534,   513,
     536,   537,   514,   515,   516,   517,   518,   519,   520,   544,
     521,   546,   547,   522,   597,   598,   523,   600,   524,   525,
     528,   529,   533,   609,   538,   541,   542,   613,   614,   615,
     548,   617,   570,   619,     0,   549,   550,   551,     0,   552,
     553,   554,   555,   556,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,   350,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   590,   591,   592,
     593,   594,   595,   608,   620,   621,   622,   596,   623,   624,
     599,   625,   601,   603,   605,   607,   626,   627,   610,   611,
     612,   628,   629,   630,   616,   631,   618,   632,   633,   634,
     635,   636,   637,   638,   639,   640,   641,   642,   643
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-304))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       2,   225,   225,     5,   217,   308,    94,     7,   139,   140,
      81,    82,    94,    16,     3,     4,     0,    88,     7,    17,
      20,     9,    10,    11,    12,    13,    14,    15,    93,   105,
     106,    34,    20,    21,    90,    91,    94,   113,   114,   115,
      94,    97,   118,   119,    87,    88,    49,    50,    90,    91,
     126,    93,    71,    72,    57,    94,    59,    60,    61,    57,
      58,    59,    60,    96,    51,    52,    53,    54,   371,    56,
       3,     4,     5,     6,     7,    96,    76,    77,    78,    79,
      80,   212,   213,    49,    50,    51,    52,    53,    96,    26,
     303,    28,    92,    95,    96,    97,    96,    34,    23,    24,
      25,    87,    88,    89,    22,   329,   329,    44,    93,    46,
      47,    48,    49,    50,    51,    52,    53,    96,   120,   121,
      96,   123,   124,    49,    50,    62,    49,    50,    65,    87,
      88,    89,    96,    70,    87,    88,    89,    74,    75,    83,
      84,    85,   144,   145,   146,   147,    49,    50,   150,   421,
     422,   153,   154,   155,   156,   157,   158,   159,   160,    87,
      88,    89,    96,    87,    88,    89,    87,    88,    89,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    28,    96,    51,    52,    53,    96,    34,   386,   387,
      96,   203,   204,    96,    96,   207,   208,   209,    66,    67,
      68,    69,    49,    50,    96,    96,    96,    96,    55,    96,
      57,    96,    59,    60,    61,    96,    63,    64,    65,    54,
      55,    56,    57,    58,    59,    60,    96,    96,    63,    64,
      65,     8,     9,    10,    11,    12,    13,    14,    96,    96,
      96,    96,    19,    96,    96,    96,    96,    96,    96,   316,
      96,    95,    93,    96,    96,    88,    96,    96,    96,    96,
      96,   391,    96,    95,    93,    96,    96,    88,    96,    96,
      96,    96,    96,    96,    96,    95,    93,    96,    88,    95,
      89,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    18,   380,   381,    23,   329,   152,   163,
     411,    96,    96,    96,    96,    96,    96,    96,   418,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
     340,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    87,    95,    97,   100,    97,    97,
     362,    97,    97,    97,    87,    97,    97,    97,    97,    97,
      97,    97,    97,   375,   376,   377,   378,   453,    97,    97,
     456,   383,   384,    97,    97,   461,    86,    97,    97,   465,
     466,    97,    97,   469,    91,    96,   472,    96,    93,    97,
     289,    97,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    87,    96,    88,    96,    96,
      88,    88,    96,    96,   131,   422,    96,   429,   430,   292,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,   451,
      96,    88,    97,    97,    97,   457,   458,    97,   460,    97,
     462,   463,    97,    97,    97,    97,    97,    97,    97,   471,
      97,   473,   474,    97,   550,   551,    97,   553,    97,    97,
      88,    88,    88,   559,    88,    88,    88,   563,   564,   565,
      88,   567,    97,   569,    -1,    96,    96,    96,    -1,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,   254,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    88,    97,    97,    97,   549,    97,    97,
     552,    97,   554,   555,   556,   557,    97,    97,   560,   561,
     562,    97,    97,    97,   566,    97,   568,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,    99,   100,   101,    87,
      88,    89,   102,   190,   103,   190,    94,   104,    94,   105,
     181,   190,     0,    93,    94,    94,     3,     4,     7,   174,
     175,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    51,    52,    53,    66,    67,    68,    69,
     179,   180,    94,   100,     8,     9,    10,    11,    12,    13,
      14,    19,   106,   107,   108,   109,   112,   129,     9,    10,
      11,    12,    13,    14,    15,    20,    21,   134,   135,   137,
     142,   147,   163,   168,   173,    96,    96,    96,    22,   176,
      93,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      95,    93,     7,    20,    76,    77,    78,    79,    80,    92,
      96,   182,   183,    96,    96,    96,    96,    96,    96,    96,
      96,    95,    93,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    95,    93,   190,   190,   190,    96,    95,   174,
      88,    88,    88,    81,    82,    88,   188,   188,   188,    89,
      88,    88,    88,    88,    88,   188,   188,   188,    88,    88,
     188,   188,   190,   190,    88,   190,   190,    88,   188,    88,
      88,    88,   179,    96,    96,    96,    96,    96,    96,    96,
     182,   182,    90,    91,    93,    97,   190,   190,   115,   190,
     113,   190,   119,   120,   124,   125,   130,   190,   106,   138,
     190,   143,   190,   152,   190,   150,   190,   154,   190,   159,
     190,   164,   190,   169,   190,    97,   134,    97,    97,    97,
      23,    24,    25,   177,   178,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,   190,   190,    87,    88,   184,   185,
      87,   186,   187,   190,   190,   190,    97,   182,   182,    95,
      71,    72,   110,   111,   110,    97,   116,    97,   114,    97,
      17,    57,    58,    59,    60,   121,   122,   123,    97,    54,
      55,    56,    59,    60,    63,    64,    65,   123,   126,   127,
     128,    97,   131,    97,   139,    97,   144,    97,   153,    97,
     151,    97,   155,    97,   160,    97,   165,    97,   170,    97,
     177,    97,    97,    97,    97,   184,    86,    97,   186,    97,
      97,    97,    96,    96,    97,   110,    97,    51,    52,    53,
     117,   118,   128,   117,    96,    96,    96,    96,    96,   121,
      96,    96,    96,    96,    96,    96,    96,    96,   126,    18,
     132,   133,    26,    28,    34,    44,    46,    47,    48,    49,
      50,    51,    52,    53,    62,    65,    70,    74,    75,   136,
     140,   141,    49,    50,    51,    52,    53,   145,   146,   149,
     149,   156,   161,   167,   172,    93,    87,   190,    88,    96,
      96,    96,   117,    88,   190,   190,    88,   190,    88,   190,
     188,   188,    88,   190,   190,    83,    84,    85,   189,   189,
     189,    96,   132,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
     140,    96,    96,    96,    96,    96,   145,    49,    50,   148,
      16,    49,    50,    57,    59,    60,    61,   136,   157,   158,
      28,    55,    59,    60,    61,    63,    64,    65,   157,   162,
      49,    50,   166,    49,    50,   171,    97,    97,   190,   190,
      88,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,   190,   188,    88,    88,
     188,   190,   190,    88,   190,   188,   190,   190,    88,   188,
     188,    88,    88,   188,   190,   188,   190,   190,    88,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,   190,   188,   188,   190,
     188,   190,    88,   190,    88,   190,    88,   190,    88,   188,
     190,   190,   190,   188,   188,   188,   190,   188,   190,   188,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,    97,    97
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 5:

/* Line 1806 of yacc.c  */
#line 183 "cfg-grammar.y"
    { add_source_group((yyvsp[(2) - (2)].ptr)); }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 184 "cfg-grammar.y"
    { add_dest_group((yyvsp[(2) - (2)].ptr)); }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 185 "cfg-grammar.y"
    { add_log_connection((yyvsp[(2) - (2)].ptr)); }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 186 "cfg-grammar.y"
    { add_filter_rule((yyvsp[(2) - (2)].ptr)); }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 187 "cfg-grammar.y"
    {  }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 191 "cfg-grammar.y"
    { (yyval.ptr) = make_source_group((yyvsp[(1) - (4)].cptr), (yyvsp[(3) - (4)].ptr)); free((yyvsp[(1) - (4)].cptr)); }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 195 "cfg-grammar.y"
    { (yyval.ptr) = make_dest_group((yyvsp[(1) - (4)].cptr), (yyvsp[(3) - (4)].ptr)); free((yyvsp[(1) - (4)].cptr)); }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 199 "cfg-grammar.y"
    { (yyval.ptr) = make_log_connection((yyvsp[(2) - (4)].ptr), (yyvsp[(3) - (4)].num)); }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 203 "cfg-grammar.y"
    { (yyval.ptr) = NULL; }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 207 "cfg-grammar.y"
    { append_source_driver((yyvsp[(1) - (3)].ptr), (yyvsp[(3) - (3)].ptr)); (yyval.ptr) = (yyvsp[(1) - (3)].ptr); }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 208 "cfg-grammar.y"
    { (yyval.ptr) = NULL; }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 212 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(1) - (1)].ptr); }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 213 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(1) - (1)].ptr); }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 214 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(1) - (1)].ptr); }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 215 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(1) - (1)].ptr); }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 219 "cfg-grammar.y"
    { (yyval.ptr) = make_afinter_source(); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 224 "cfg-grammar.y"
    { 
        	(yyval.ptr) = make_affile_source((yyvsp[(3) - (5)].cptr), 0, get_last_log_prefix(), last_pad_size); 
		free((yyvsp[(3) - (5)].cptr)); 
	  }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 229 "cfg-grammar.y"
    { 
		(yyval.ptr) = make_affile_source((yyvsp[(3) - (5)].cptr), AFFILE_PIPE, get_last_log_prefix(), last_pad_size); 
		free((yyvsp[(3) - (5)].cptr)); 
	  }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 241 "cfg-grammar.y"
    { last_prefix = (UINT8 *) (yyvsp[(3) - (4)].cptr); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 242 "cfg-grammar.y"
    { last_pad_size = (yyvsp[(3) - (4)].num); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 246 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(3) - (4)].ptr); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 247 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(3) - (4)].ptr); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 248 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(3) - (4)].ptr); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 249 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(3) - (4)].ptr); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 254 "cfg-grammar.y"
    { 
	    last_source_driver = make_afunix_source(
		&make_unix_address_c((yyvsp[(1) - (1)].cptr))->super, 
		AFSOCKET_DGRAM); 
	    free((yyvsp[(1) - (1)].cptr)); 
	  }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 260 "cfg-grammar.y"
    { (yyval.ptr) = last_source_driver; }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 265 "cfg-grammar.y"
    { 
	    last_source_driver = make_afunix_source(
		&make_unix_address_c((yyvsp[(1) - (1)].cptr))->super,
		AFSOCKET_STREAM | AFSOCKET_KEEP_ALIVE);
	    free((yyvsp[(1) - (1)].cptr));
	  }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 271 "cfg-grammar.y"
    { (yyval.ptr) = last_source_driver; }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 281 "cfg-grammar.y"
    { afunix_set_owner(last_source_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 282 "cfg-grammar.y"
    { afunix_set_group(last_source_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 283 "cfg-grammar.y"
    { afunix_set_perm(last_source_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 284 "cfg-grammar.y"
    {}
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 289 "cfg-grammar.y"
    { 
	    last_source_driver = make_afinet_source(
			&make_inet_address_c("0.0.0.0", "514")->super, 
			AFSOCKET_DGRAM);
	  }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 294 "cfg-grammar.y"
    { (yyval.ptr) = last_source_driver; }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 304 "cfg-grammar.y"
    { afinet_src_set_localport(last_source_driver, 0, (yyvsp[(3) - (4)].cptr) ,"udp"); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 305 "cfg-grammar.y"
    { afinet_src_set_localport(last_source_driver, 0, (yyvsp[(3) - (4)].cptr), "udp"); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 306 "cfg-grammar.y"
    { afinet_src_set_so_rcvbuf(last_source_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 310 "cfg-grammar.y"
    { afinet_src_set_localip(last_source_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 311 "cfg-grammar.y"
    { afinet_src_set_localport(last_source_driver, (yyvsp[(3) - (4)].num), NULL, NULL); }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 312 "cfg-grammar.y"
    { afinet_src_set_localport(last_source_driver, (yyvsp[(3) - (4)].num), NULL, NULL); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 313 "cfg-grammar.y"
    { afinet_src_set_localip(last_source_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 318 "cfg-grammar.y"
    { 
	    last_source_driver = make_afinet_source(
			&make_inet_address_c("0.0.0.0", "514")->super,
			AFSOCKET_STREAM);
	  }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 323 "cfg-grammar.y"
    { (yyval.ptr) = last_source_driver; }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 333 "cfg-grammar.y"
    { afinet_src_set_tcp_keepalive(last_source_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 334 "cfg-grammar.y"
    { afinet_src_set_localport(last_source_driver, 0, (yyvsp[(3) - (4)].cptr), "tcp"); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 335 "cfg-grammar.y"
    { afinet_src_set_localport(last_source_driver, 0, (yyvsp[(3) - (4)].cptr), "tcp"); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 336 "cfg-grammar.y"
    { afinet_src_set_auth(last_source_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 337 "cfg-grammar.y"
    { afinet_src_set_mac(last_source_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 338 "cfg-grammar.y"
    { afinet_src_set_encrypt(last_source_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 339 "cfg-grammar.y"
    {}
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 343 "cfg-grammar.y"
    { afsocket_src_set_keep_alive(last_source_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 344 "cfg-grammar.y"
    { afsocket_src_set_max_connections(last_source_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 348 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(3) - (4)].ptr); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 353 "cfg-grammar.y"
    { 
	    last_source_driver = make_afstreams_source((yyvsp[(1) - (1)].cptr)); 
	    free((yyvsp[(1) - (1)].cptr)); 
	  }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 357 "cfg-grammar.y"
    { (yyval.ptr) = last_source_driver; }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 366 "cfg-grammar.y"
    { afstreams_set_sundoor(last_source_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 370 "cfg-grammar.y"
    { append_dest_driver((yyvsp[(1) - (3)].ptr), (yyvsp[(3) - (3)].ptr)); (yyval.ptr) = (yyvsp[(1) - (3)].ptr); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 371 "cfg-grammar.y"
    { (yyval.ptr) = NULL; }
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 375 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(1) - (1)].ptr); }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 376 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(1) - (1)].ptr); }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 377 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(1) - (1)].ptr); }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 378 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(1) - (1)].ptr); }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 379 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(1) - (1)].ptr); }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 380 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(1) - (1)].ptr); }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 384 "cfg-grammar.y"
    { last_dest_driver->log_fifo_size = (yyvsp[(3) - (4)].num); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 388 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(3) - (4)].ptr); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 393 "cfg-grammar.y"
    { 
	    last_dest_driver = make_affile_dest((yyvsp[(1) - (1)].cptr), 0); 
	    free((yyvsp[(1) - (1)].cptr)); 
	  }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 398 "cfg-grammar.y"
    { (yyval.ptr) = last_dest_driver; }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 408 "cfg-grammar.y"
    { affile_set_syncfreq(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 409 "cfg-grammar.y"
    { affile_set_compress(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 410 "cfg-grammar.y"
    { affile_set_encrypt(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 411 "cfg-grammar.y"
    { affile_set_owner(last_dest_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 412 "cfg-grammar.y"
    { affile_set_group(last_dest_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 413 "cfg-grammar.y"
    { affile_set_perm(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 414 "cfg-grammar.y"
    { affile_set_dir_owner(last_dest_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 415 "cfg-grammar.y"
    { affile_set_dir_group(last_dest_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 416 "cfg-grammar.y"
    { affile_set_dir_perm(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 417 "cfg-grammar.y"
    { affile_set_create_dirs(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 418 "cfg-grammar.y"
    { affile_set_remove_if_older(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 419 "cfg-grammar.y"
    { affile_set_file_template(last_dest_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 420 "cfg-grammar.y"
    { affile_set_template_escape(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 421 "cfg-grammar.y"
    { affile_set_fsync(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 422 "cfg-grammar.y"
    { affile_dd_set_file_size_limit(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 423 "cfg-grammar.y"
    { affile_set_stop_on_full(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 427 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(3) - (4)].ptr); }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 432 "cfg-grammar.y"
    { 
	    last_dest_driver = make_affile_dest((yyvsp[(1) - (1)].cptr), AFFILE_NO_EXPAND | AFFILE_PIPE);
	    affile_set_syncfreq(last_dest_driver, 0);
	    free((yyvsp[(1) - (1)].cptr)); 
	  }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 437 "cfg-grammar.y"
    { (yyval.ptr) = last_dest_driver; }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 446 "cfg-grammar.y"
    { affile_set_owner(last_dest_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 110:

/* Line 1806 of yacc.c  */
#line 447 "cfg-grammar.y"
    { affile_set_group(last_dest_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 448 "cfg-grammar.y"
    { affile_set_perm(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 449 "cfg-grammar.y"
    { affile_set_file_template(last_dest_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 450 "cfg-grammar.y"
    { affile_set_template_escape(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 455 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(3) - (4)].ptr); }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 456 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(3) - (4)].ptr); }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 457 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(3) - (4)].ptr); }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 458 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(3) - (4)].ptr); }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 463 "cfg-grammar.y"
    { afunix_dest_set_template(last_dest_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 464 "cfg-grammar.y"
    { afunix_dest_set_template_escape(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 473 "cfg-grammar.y"
    { last_dest_driver = make_afunix_dest(
							&make_unix_address_c((yyvsp[(1) - (1)].cptr))->super, 
							AFSOCKET_DGRAM);
							free((yyvsp[(1) - (1)].cptr));
						}
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 478 "cfg-grammar.y"
    { (yyval.ptr) = last_dest_driver; }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 482 "cfg-grammar.y"
    { last_dest_driver = make_afunix_dest(
							&make_unix_address_c((yyvsp[(1) - (1)].cptr))->super, 
							AFSOCKET_STREAM);
							free((yyvsp[(1) - (1)].cptr));
						}
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 487 "cfg-grammar.y"
    { (yyval.ptr) = last_dest_driver; }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 492 "cfg-grammar.y"
    { 
	    last_dest_driver = make_afinet_dest(
			&make_inet_address_c((yyvsp[(1) - (1)].cptr), "514")->super, 
			AFSOCKET_DGRAM);
	    free((yyvsp[(1) - (1)].cptr));
	  }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 498 "cfg-grammar.y"
    { (yyval.ptr) = last_dest_driver; }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 508 "cfg-grammar.y"
    { afinet_dest_set_localip(last_dest_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 509 "cfg-grammar.y"
    { afinet_dest_set_localport(last_dest_driver, (yyvsp[(3) - (4)].num), NULL, NULL); }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 510 "cfg-grammar.y"
    { afinet_dest_set_destport(last_dest_driver, (yyvsp[(3) - (4)].num), NULL, NULL); }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 511 "cfg-grammar.y"
    { afinet_dest_set_destport(last_dest_driver, (yyvsp[(3) - (4)].num), NULL, NULL); }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 512 "cfg-grammar.y"
    { afinet_dest_set_template(last_dest_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 513 "cfg-grammar.y"
    { afinet_dest_set_template_escape(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 518 "cfg-grammar.y"
    { afinet_dest_set_localport(last_dest_driver, 0, (yyvsp[(3) - (4)].cptr), "udp"); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 519 "cfg-grammar.y"
    { afinet_dest_set_destport(last_dest_driver, 0, (yyvsp[(3) - (4)].cptr), "udp"); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 520 "cfg-grammar.y"
    { afinet_dest_set_destport(last_dest_driver, 0, (yyvsp[(3) - (4)].cptr), "udp"); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 521 "cfg-grammar.y"
    { afinet_dest_set_spoof_source(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 526 "cfg-grammar.y"
    { 
	    last_dest_driver = make_afinet_dest(
			&make_inet_address_c((yyvsp[(1) - (1)].cptr), "514")->super, 
			AFSOCKET_STREAM); 
	    free((yyvsp[(1) - (1)].cptr));
	  }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 532 "cfg-grammar.y"
    { (yyval.ptr) = last_dest_driver; }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 542 "cfg-grammar.y"
    { afinet_dest_set_tcp_keepalive(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 543 "cfg-grammar.y"
    { afinet_dest_set_localport(last_dest_driver, 0, (yyvsp[(3) - (4)].cptr), "tcp"); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 544 "cfg-grammar.y"
    { afinet_dest_set_destport(last_dest_driver, 0, (yyvsp[(3) - (4)].cptr), "tcp"); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 545 "cfg-grammar.y"
    { afinet_dest_set_destport(last_dest_driver, 0, (yyvsp[(3) - (4)].cptr), "tcp"); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 549 "cfg-grammar.y"
    { afinet_dest_set_syncfreq(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 553 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(3) - (4)].ptr); }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 557 "cfg-grammar.y"
    { last_dest_driver = make_afuser_dest((yyvsp[(1) - (1)].cptr)); free((yyvsp[(1) - (1)].cptr)); }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 558 "cfg-grammar.y"
    { (yyval.ptr) = last_dest_driver; }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 562 "cfg-grammar.y"
    { afuser_dest_set_template(last_dest_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 563 "cfg-grammar.y"
    { afuser_dest_set_template_escape(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 572 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(3) - (4)].ptr); }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 576 "cfg-grammar.y"
    { last_dest_driver = make_afprogram_dest((yyvsp[(1) - (1)].cptr)); free((yyvsp[(1) - (1)].cptr)); }
    break;

  case 164:

/* Line 1806 of yacc.c  */
#line 577 "cfg-grammar.y"
    { (yyval.ptr) = last_dest_driver; }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 581 "cfg-grammar.y"
    { afprogram_dest_set_template(last_dest_driver, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 582 "cfg-grammar.y"
    { afprogram_dest_set_template_escape(last_dest_driver, (yyvsp[(3) - (4)].num)); }
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 591 "cfg-grammar.y"
    { (yyval.ptr) = make_afremctrl_dest(); }
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 595 "cfg-grammar.y"
    { append_endpoint_info((yyvsp[(1) - (3)].ptr), (yyvsp[(3) - (3)].ptr)); (yyval.ptr) = (yyvsp[(1) - (3)].ptr); }
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 596 "cfg-grammar.y"
    { (yyval.ptr) = NULL; }
    break;

  case 172:

/* Line 1806 of yacc.c  */
#line 600 "cfg-grammar.y"
    { (yyval.ptr) = make_log_endpoint_info(EP_SOURCE, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 601 "cfg-grammar.y"
    { (yyval.ptr) = make_log_endpoint_info(EP_FILTER, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 602 "cfg-grammar.y"
    { (yyval.ptr) = make_log_endpoint_info(EP_DESTINATION, (yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 606 "cfg-grammar.y"
    { (yyval.num) = (yyvsp[(3) - (5)].num); }
    break;

  case 176:

/* Line 1806 of yacc.c  */
#line 607 "cfg-grammar.y"
    { (yyval.num) = 0; }
    break;

  case 177:

/* Line 1806 of yacc.c  */
#line 612 "cfg-grammar.y"
    { (yyval.num) |= (yyvsp[(2) - (2)].num); }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 613 "cfg-grammar.y"
    { (yyval.num) = 0; }
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 617 "cfg-grammar.y"
    { (yyval.num) = LOG_CONN_CATCHALL; }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 618 "cfg-grammar.y"
    { (yyval.num) = LOG_CONN_FALLBACK; }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 619 "cfg-grammar.y"
    { (yyval.num) = LOG_CONN_FINAL; }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 623 "cfg-grammar.y"
    { (yyval.ptr) = (yyvsp[(1) - (3)].ptr); }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 624 "cfg-grammar.y"
    { (yyval.ptr) = NULL; }
    break;

  case 184:

/* Line 1806 of yacc.c  */
#line 628 "cfg-grammar.y"
    { configuration->mark_freq = (yyvsp[(3) - (4)].num); }
    break;

  case 185:

/* Line 1806 of yacc.c  */
#line 629 "cfg-grammar.y"
    { configuration->sync_freq = (yyvsp[(3) - (4)].num); }
    break;

  case 186:

/* Line 1806 of yacc.c  */
#line 630 "cfg-grammar.y"
    { configuration->stats_freq = (yyvsp[(3) - (4)].num); }
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 631 "cfg-grammar.y"
    { configuration->chain_hostnames = (yyvsp[(3) - (4)].num); }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 632 "cfg-grammar.y"
    { configuration->keep_hostname = (yyvsp[(3) - (4)].num); }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 633 "cfg-grammar.y"
    { configuration->check_hostname = (yyvsp[(3) - (4)].num); }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 634 "cfg-grammar.y"
    { cfg_set_bad_hostname((yyvsp[(3) - (4)].cptr)); }
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 635 "cfg-grammar.y"
    { configuration->use_time_recvd = (yyvsp[(3) - (4)].num); }
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 636 "cfg-grammar.y"
    { configuration->use_fqdn = (yyvsp[(3) - (4)].num); }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 637 "cfg-grammar.y"
    { configuration->use_dns = (yyvsp[(3) - (4)].num); }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 638 "cfg-grammar.y"
    { configuration->time_reopen = (yyvsp[(3) - (4)].num); }
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 639 "cfg-grammar.y"
    { configuration->time_sleep = (yyvsp[(3) - (4)].num); }
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 640 "cfg-grammar.y"
    { configuration->time_reap = (yyvsp[(3) - (4)].num); }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 641 "cfg-grammar.y"
    { configuration->log_fifo_size = (yyvsp[(3) - (4)].num); }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 642 "cfg-grammar.y"
    { configuration->gc_busy_threshold = (yyvsp[(3) - (4)].num); }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 643 "cfg-grammar.y"
    { configuration->gc_idle_threshold = (yyvsp[(3) - (4)].num); }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 644 "cfg-grammar.y"
    { configuration->create_dirs = (yyvsp[(3) - (4)].num); }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 645 "cfg-grammar.y"
    { configuration->sanitize_filenames = (yyvsp[(3) - (4)].num); }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 646 "cfg-grammar.y"
    { cfg_set_owner((yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 647 "cfg-grammar.y"
    { cfg_set_group((yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 648 "cfg-grammar.y"
    { cfg_set_perm((yyvsp[(3) - (4)].num)); }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 649 "cfg-grammar.y"
    { cfg_set_dir_owner((yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 650 "cfg-grammar.y"
    { cfg_set_dir_group((yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 651 "cfg-grammar.y"
    { cfg_set_dir_perm((yyvsp[(3) - (4)].num)); }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 652 "cfg-grammar.y"
    { configuration->dns_cache = (yyvsp[(3) - (4)].num); }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 653 "cfg-grammar.y"
    { configuration->dns_cache_size = (yyvsp[(3) - (4)].num); }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 654 "cfg-grammar.y"
    { configuration->dns_cache_expire = (yyvsp[(3) - (4)].num); }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 656 "cfg-grammar.y"
    { configuration->dns_cache_expire_failed = (yyvsp[(3) - (4)].num); }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 657 "cfg-grammar.y"
    { configuration->log_msg_size = (yyvsp[(3) - (4)].num); }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 661 "cfg-grammar.y"
    { (yyval.ptr) = make_filter_rule((yyvsp[(1) - (5)].cptr), (yyvsp[(3) - (5)].node)); free((yyvsp[(1) - (5)].cptr)); }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 665 "cfg-grammar.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); if (!(yyvsp[(1) - (1)].node)) return 1; }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 666 "cfg-grammar.y"
    { (yyvsp[(2) - (2)].node)->comp = !((yyvsp[(2) - (2)].node)->comp); (yyval.node) = (yyvsp[(2) - (2)].node); }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 667 "cfg-grammar.y"
    { (yyval.node) = make_filter_or((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 668 "cfg-grammar.y"
    { (yyval.node) = make_filter_and((yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node)); }
    break;

  case 218:

/* Line 1806 of yacc.c  */
#line 669 "cfg-grammar.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 673 "cfg-grammar.y"
    { (yyval.node) = make_filter_facility((yyvsp[(3) - (4)].num));  }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 674 "cfg-grammar.y"
    { (yyval.node) = make_filter_facility(0x80000000 | (yyvsp[(3) - (4)].num)); }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 675 "cfg-grammar.y"
    { (yyval.node) = make_filter_level((yyvsp[(3) - (4)].num)); }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 676 "cfg-grammar.y"
    { (yyval.node) = make_filter_prog((yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 677 "cfg-grammar.y"
    { (yyval.node) = make_filter_netmask((yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 678 "cfg-grammar.y"
    { (yyval.node) = make_filter_host((yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 679 "cfg-grammar.y"
    { (yyval.node) = make_filter_match((yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 680 "cfg-grammar.y"
    { (yyval.node) = make_filter_call((yyvsp[(3) - (4)].cptr)); free((yyvsp[(3) - (4)].cptr)); }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 684 "cfg-grammar.y"
    { (yyval.num) = (yyvsp[(1) - (2)].num) + (yyvsp[(2) - (2)].num); }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 685 "cfg-grammar.y"
    { (yyval.num) = (yyvsp[(1) - (1)].num); }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 690 "cfg-grammar.y"
    { 
	    int n = syslog_lookup_facility((yyvsp[(1) - (1)].cptr));
	    if (n == -1)
	      {
	        werror("Warning: Unknown facility %z\n", (yyvsp[(1) - (1)].cptr));
	        (yyval.num) = 0;
	      }
	    else
	      (yyval.num) = (1 << n); 
	    free((yyvsp[(1) - (1)].cptr)); 
	  }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 704 "cfg-grammar.y"
    { (yyval.num) = (yyvsp[(1) - (2)].num) + (yyvsp[(2) - (2)].num); }
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 705 "cfg-grammar.y"
    { (yyval.num) = (yyvsp[(1) - (1)].num); }
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 710 "cfg-grammar.y"
    { 
	    int r1, r2;
	    r1 = syslog_lookup_level((yyvsp[(1) - (3)].cptr));
	    if (r1 == -1)
	      werror("Warning: Unknown priority level %z\n", (yyvsp[(1) - (3)].cptr));
	    else
	      r1 = sl_levels[r1].value;
	    r2 = syslog_lookup_level((yyvsp[(3) - (3)].cptr));
	    if (r2 == -1)
	      werror("Warning: Unknown priority level %z\n", (yyvsp[(1) - (3)].cptr));
	    else
	      r2 = sl_levels[r2].value;
	    if (r1 != -1 && r2 != -1)
	      (yyval.num) = syslog_make_range(r1, r2); 
	    else
	      (yyval.num) = 0;
	    free((yyvsp[(1) - (3)].cptr)); 
	    free((yyvsp[(3) - (3)].cptr)); 
	  }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 730 "cfg-grammar.y"
    { 
	    int n = syslog_lookup_level((yyvsp[(1) - (1)].cptr)); 
	    if (n == -1)
	      {
	        werror("Warning: Unknown priority level %z\n", (yyvsp[(1) - (1)].cptr));
	        (yyval.num) = 0;
	      }
	    else
	      (yyval.num) = 1 << sl_levels[n].value; 
	    free((yyvsp[(1) - (1)].cptr)); 
	  }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 744 "cfg-grammar.y"
    { (yyval.num) = 1; }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 745 "cfg-grammar.y"
    { (yyval.num) = 0; }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 746 "cfg-grammar.y"
    { (yyval.num) = (yyvsp[(1) - (1)].num); }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 750 "cfg-grammar.y"
    { (yyval.num) = 2; }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 751 "cfg-grammar.y"
    { (yyval.num) = 1; }
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 752 "cfg-grammar.y"
    { (yyval.num) = 0; }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 758 "cfg-grammar.y"
    { char buf[32]; snprintf(buf, sizeof(buf), "%d", (yyvsp[(1) - (1)].num)); (yyval.cptr) = strdup(buf); }
    break;



/* Line 1806 of yacc.c  */
#line 3644 "cfg-grammar.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 761 "cfg-grammar.y"


extern int linenum;

void yyerror(char *msg)
{
	fprintf(stderr, "%s at %d\n", msg, linenum);
}

