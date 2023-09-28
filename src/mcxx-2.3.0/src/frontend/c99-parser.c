/* A Bison parser, made by GNU Bison 2.7.  */

/* Skeleton implementation for Bison GLR parsers in C
   
      Copyright (C) 2002-2012 Free Software Foundation, Inc.
   
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

/* C GLR parser skeleton written by Paul Hilfinger.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "glr.c"

/* Pure parsers.  */
#define YYPURE 0




/* Substitute the variable and function names.  */
#define yyparse mc99parse
#define yylex   mc99lex
#define yyerror mc99error
#define yylval  mc99lval
#define yychar  mc99char
#define yydebug mc99debug
#define yynerrs mc99nerrs
#define yylloc  mc99lloc

/* Copy the first part of user declarations.  */
/* Line 207 of glr.c  */
#line 28 "src/frontend/c99.y"

/*
   Parser of ISO/IEC 9899:1999 - C

   It parses a superset of the language.

   Must be compiled with rofi-bison-2.1. 
   Ask for it at <rferrer@ac.upc.edu>
 */

#include "c99-parser.h"
#include "cxx-ast.h"
#include "cxx-lexer.h"
#include "cxx-utils.h"
#include "cxx-diagnostic.h"
#include "mem.h"

#define YYDEBUG 1
#define YYERROR_VERBOSE 1
// Sometimes we need lots of memory
#define YYMAXDEPTH (10000000)

#define YYMALLOC xmalloc
#define YYFREE DELETE
#define YYREALLOC xrealloc


/* Line 207 of glr.c  */
#line 92 "src/frontend/c99-parser.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

#include "c99-parser-internal.h"

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Default (constant) value used for initialization for null
   right-hand sides.  Unlike the standard yacc.c template, here we set
   the default value of $$ to a zeroed-out value.  Since the default
   value is undefined, this behavior is technically correct.  */
static YYSTYPE yyval_default;
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;

/* Copy the second part of user declarations.  */
/* Line 230 of glr.c  */
#line 76 "src/frontend/c99.y"

extern int yylex(void);
static AST ambiguityHandler (YYSTYPE x0, YYSTYPE x1);
void yyerror(AST* parsed_tree UNUSED_PARAMETER, const char* c);

/* Line 230 of glr.c  */
#line 132 "src/frontend/c99-parser.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#ifndef YYFREE
# define YYFREE free
#endif
#ifndef YYMALLOC
# define YYMALLOC malloc
#endif
#ifndef YYREALLOC
# define YYREALLOC realloc
#endif

#define YYSIZEMAX ((size_t) -1)

#ifdef __cplusplus
   typedef bool yybool;
#else
   typedef unsigned char yybool;
#endif
#define yytrue 1
#define yyfalse 0

#ifndef YYSETJMP
# include <setjmp.h>
# define YYJMP_BUF jmp_buf
# define YYSETJMP(Env) setjmp (Env)
/* Pacify clang.  */
# define YYLONGJMP(Env, Val) (longjmp (Env, Val), YYASSERT (0))
#endif

/*-----------------.
| GCC extensions.  |
`-----------------*/

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

#ifndef YYASSERT
# define YYASSERT(Condition) ((void) ((Condition) || (abort (), 0)))
#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  350
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   11435

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  193
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  226
/* YYNRULES -- Number of rules.  */
#define YYNRULES  634
/* YYNRULES -- Number of states.  */
#define YYNSTATES  1118
/* YYMAXRHS -- Maximum number of symbols on right-hand side of rule.  */
#define YYMAXRHS 12
/* YYMAXLEFT -- Maximum number of symbols to the left of a handle
   accessed by $0, $-1, etc., in any rule.  */
#define YYMAXLEFT 0

/* YYTRANSLATE(X) -- Bison symbol number corresponding to X.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   423

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    81,     2,     2,     2,    82,    83,     2,
      84,    85,    86,    87,    88,    89,    90,    91,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    92,    93,
      94,    95,    96,    97,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    98,     2,    99,   100,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   101,   102,   103,   104,     2,     2,     2,
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
      75,    76,    77,    78,    79,    80,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     5,     6,     8,    11,    13,    15,    17,
      19,    21,    29,    31,    33,    35,    38,    40,    42,    44,
      46,    49,    51,    53,    57,    59,    63,    65,    68,    70,
      72,    73,    80,    86,    88,    92,    94,    96,   101,   108,
     117,   128,   141,   142,   144,   146,   147,   149,   153,   158,
     166,   168,   170,   172,   176,   179,   182,   186,   190,   193,
     196,   198,   200,   202,   205,   209,   212,   215,   217,   219,
     223,   226,   229,   231,   233,   237,   240,   243,   245,   247,
     250,   252,   254,   256,   258,   260,   262,   264,   266,   268,
     270,   272,   274,   276,   278,   280,   282,   284,   286,   288,
     290,   292,   294,   296,   298,   300,   302,   304,   306,   310,
     313,   316,   318,   320,   322,   324,   326,   328,   333,   338,
     343,   345,   347,   349,   351,   353,   355,   357,   359,   361,
     363,   367,   371,   373,   377,   379,   383,   386,   390,   393,
     397,   398,   400,   402,   404,   407,   412,   415,   419,   421,
     424,   426,   429,   431,   433,   436,   440,   442,   445,   447,
     449,   453,   456,   458,   461,   466,   473,   480,   484,   487,
     489,   493,   494,   496,   498,   499,   501,   503,   504,   506,
     508,   510,   513,   515,   517,   519,   521,   523,   526,   531,
     538,   545,   549,   551,   555,   557,   558,   560,   565,   571,
     575,   577,   579,   583,   585,   588,   591,   595,   598,   600,
     603,   605,   609,   611,   614,   618,   624,   630,   635,   642,
     649,   651,   653,   657,   659,   663,   665,   669,   673,   676,
     680,   683,   685,   687,   689,   693,   696,   701,   705,   711,
     714,   716,   719,   723,   726,   729,   733,   736,   739,   741,
     743,   746,   748,   753,   757,   759,   762,   765,   769,   771,
     773,   775,   778,   782,   785,   787,   790,   792,   796,   798,
     802,   805,   809,   812,   817,   820,   824,   828,   833,   836,
     838,   840,   842,   844,   846,   848,   850,   852,   854,   858,
     863,   867,   874,   877,   879,   881,   885,   888,   890,   893,
     899,   907,   913,   915,   921,   929,   938,   940,   941,   943,
     944,   946,   948,   951,   954,   957,   961,   965,   970,   972,
     976,   978,   980,   984,   991,   998,  1007,  1014,  1021,  1023,
    1027,  1031,  1035,  1038,  1040,  1042,  1044,  1046,  1051,  1055,
    1060,  1064,  1068,  1071,  1074,  1079,  1083,  1088,  1091,  1093,
    1097,  1099,  1102,  1105,  1108,  1111,  1116,  1119,  1122,  1127,
    1130,  1133,  1136,  1138,  1140,  1142,  1144,  1146,  1148,  1150,
    1155,  1157,  1161,  1165,  1169,  1171,  1175,  1179,  1181,  1185,
    1189,  1191,  1195,  1199,  1203,  1207,  1209,  1213,  1217,  1219,
    1223,  1225,  1229,  1231,  1235,  1237,  1241,  1243,  1247,  1249,
    1255,  1260,  1262,  1266,  1268,  1272,  1274,  1276,  1278,  1280,
    1282,  1284,  1286,  1288,  1290,  1292,  1294,  1296,  1298,  1300,
    1302,  1304,  1306,  1308,  1310,  1312,  1314,  1316,  1319,  1321,
    1323,  1326,  1329,  1331,  1334,  1337,  1339,  1342,  1345,  1348,
    1351,  1353,  1357,  1362,  1367,  1371,  1373,  1378,  1383,  1385,
    1387,  1389,  1391,  1393,  1400,  1405,  1412,  1421,  1428,  1437,
    1439,  1444,  1447,  1450,  1452,  1456,  1468,  1470,  1472,  1474,
    1476,  1482,  1486,  1488,  1492,  1496,  1498,  1500,  1502,  1506,
    1512,  1516,  1522,  1526,  1528,  1534,  1536,  1538,  1540,  1542,
    1544,  1546,  1548,  1550,  1552,  1554,  1556,  1559,  1563,  1567,
    1571,  1575,  1582,  1584,  1588,  1595,  1596,  1598,  1600,  1604,
    1607,  1612,  1616,  1618,  1620,  1622,  1625,  1627,  1629,  1636,
    1639,  1641,  1643,  1646,  1649,  1655,  1663,  1665,  1667,  1669,
    1671,  1675,  1677,  1679,  1681,  1683,  1685,  1687,  1689,  1691,
    1693,  1695,  1698,  1702,  1707,  1709,  1710,  1712,  1719,  1724,
    1727,  1729,  1731,  1735,  1739,  1742,  1746,  1749,  1754,  1756,
    1761,  1762,  1764,  1766,  1769,  1772,  1776,  1782,  1788,  1791,
    1796,  1799,  1804,  1807,  1812,  1814,  1816,  1818,  1821,  1823,
    1825,  1827,  1830,  1833,  1837,  1841,  1843,  1847,  1851,  1855,
    1858,  1860,  1861,  1872,  1874,  1875,  1877,  1879,  1881,  1883,
    1885,  1887,  1889,  1891,  1893,  1898,  1904,  1908,  1916,  1922,
    1926,  1928,  1930,  1932,  1934,  1936,  1938,  1940,  1942,  1945,
    1947,  1949,  1954,  1955,  1957,  1959,  1963,  1965,  1970,  1972,
    1974,  1976,  1978,  1980,  1985
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const short int yyrhs[] =
{
     194,     0,    -1,   195,    -1,    -1,   196,    -1,   195,   196,
      -1,   197,    -1,   286,    -1,   216,    -1,   202,    -1,   198,
      -1,    79,    84,   342,    88,   344,    85,    93,    -1,   217,
      -1,   202,    -1,   200,    -1,   110,   200,    -1,   201,    -1,
     286,    -1,   211,    -1,   203,    -1,   110,   197,    -1,    72,
      -1,    73,    -1,   113,   204,    93,    -1,   345,    -1,   204,
      88,   345,    -1,   208,    -1,   206,   205,    -1,   205,    -1,
     206,    -1,    -1,   118,    84,    84,   209,    85,    85,    -1,
     118,    84,    84,    85,    85,    -1,   210,    -1,   209,    88,
     210,    -1,   345,    -1,    14,    -1,   345,    84,   324,    85,
      -1,     6,   212,    84,   344,    85,    93,    -1,     6,   212,
      84,   344,    92,   213,    85,    93,    -1,     6,   212,    84,
     344,    92,   213,    92,   213,    85,    93,    -1,     6,   212,
      84,   344,    92,   213,    92,   213,    92,   213,    85,    93,
      -1,    -1,    69,    -1,   214,    -1,    -1,   215,    -1,   214,
      88,   215,    -1,   344,    84,   340,    85,    -1,    98,   345,
      99,   344,    84,   340,    85,    -1,   344,    -1,   217,    -1,
      93,    -1,   221,   239,    93,    -1,   223,    93,    -1,   239,
      93,    -1,   219,   239,    93,    -1,   220,   229,   224,    -1,
     220,   229,    -1,   229,   224,    -1,   229,    -1,   220,    -1,
     226,    -1,   220,   226,    -1,   224,   229,   224,    -1,   224,
     229,    -1,   229,   224,    -1,   229,    -1,   224,    -1,   224,
     229,   224,    -1,   224,   229,    -1,   229,   224,    -1,   229,
      -1,   224,    -1,   224,   230,   224,    -1,   224,   230,    -1,
     230,   224,    -1,   230,    -1,   225,    -1,   224,   225,    -1,
     226,    -1,   208,    -1,   227,    -1,   228,    -1,    65,    -1,
     262,    -1,    57,    -1,    67,    -1,    42,    -1,    56,    -1,
     114,    -1,   115,    -1,     7,    -1,    52,    -1,    59,    -1,
      24,    -1,   119,    -1,    35,    -1,    78,    -1,   231,    -1,
     233,    -1,   231,    -1,   237,    -1,   234,    -1,   235,    -1,
     290,    -1,   268,    -1,   238,    -1,   224,   229,   224,    -1,
     224,   229,    -1,   229,   224,    -1,   229,    -1,   224,    -1,
     236,    -1,   237,    -1,   234,    -1,   235,    -1,   116,    84,
     340,    85,    -1,   116,    84,   271,    85,    -1,    76,    84,
     271,    85,    -1,   345,    -1,    12,    -1,     8,    -1,    36,
      -1,    25,    -1,    20,    -1,    68,    -1,    37,    -1,    38,
      -1,   132,    -1,   292,   207,   345,    -1,   267,   207,   345,
      -1,   242,    -1,   242,    88,   240,    -1,   241,    -1,   240,
      88,   241,    -1,   246,   243,    -1,   246,   243,   280,    -1,
     247,   243,    -1,   247,   243,   280,    -1,    -1,   244,    -1,
     245,    -1,   206,    -1,   245,   206,    -1,     6,    84,   344,
      85,    -1,   207,   263,    -1,   207,   248,   246,    -1,   263,
      -1,   248,   246,    -1,    86,    -1,    86,   261,    -1,    74,
      -1,    75,    -1,   207,   253,    -1,   207,   248,   249,    -1,
     253,    -1,   248,   249,    -1,   277,    -1,   275,    -1,    84,
     251,    85,    -1,    84,    85,    -1,   254,    -1,   253,   252,
      -1,   253,    98,   260,    99,    -1,   253,    98,   257,   258,
     260,    99,    -1,   253,    98,   259,   256,   260,    99,    -1,
      84,   249,    85,    -1,   255,   252,    -1,   264,    -1,    84,
     255,    85,    -1,    -1,   257,    -1,   261,    -1,    -1,   259,
      -1,    59,    -1,    -1,    86,    -1,   339,    -1,   262,    -1,
     261,   262,    -1,    14,    -1,    69,    -1,   117,    -1,    76,
      -1,   264,    -1,   263,   252,    -1,   263,    98,   260,    99,
      -1,   263,    98,   257,   258,   260,    99,    -1,   263,    98,
     259,   256,   260,    99,    -1,    84,   246,    85,    -1,   320,
      -1,   267,   207,   266,    -1,   345,    -1,    -1,    22,    -1,
     265,   101,   269,   103,    -1,   265,   101,   269,    88,   103,
      -1,   269,    88,   270,    -1,   270,    -1,   345,    -1,   345,
      95,   342,    -1,   232,    -1,   232,   273,    -1,   207,   248,
      -1,   207,   248,   272,    -1,   207,   274,    -1,   248,    -1,
     248,   272,    -1,   274,    -1,    84,   272,    85,    -1,   252,
      -1,   274,   252,    -1,    98,   260,    99,    -1,    98,   257,
     258,   260,    99,    -1,    98,   259,   256,   260,    99,    -1,
     274,    98,   260,    99,    -1,   274,    98,   257,   258,   260,
      99,    -1,   274,    98,   259,   256,   260,    99,    -1,   276,
      -1,   345,    -1,   276,    88,   345,    -1,   278,    -1,   278,
      88,    64,    -1,   279,    -1,   278,    88,   279,    -1,   221,
     247,   207,    -1,   222,   207,    -1,   221,   273,   207,    -1,
      95,   281,    -1,   339,    -1,   323,    -1,   281,    -1,   282,
      88,   281,    -1,   283,   281,    -1,   282,    88,   283,   281,
      -1,   345,    92,   281,    -1,   282,    88,   345,    92,   281,
      -1,   284,    95,    -1,   285,    -1,   284,   285,    -1,    98,
     342,    99,    -1,    90,   345,    -1,   287,   289,    -1,   287,
     288,   289,    -1,   110,   286,    -1,   221,   250,    -1,   250,
      -1,   218,    -1,   288,   218,    -1,   305,    -1,   291,   101,
     293,   103,    -1,   291,   101,   103,    -1,   292,    -1,   292,
     345,    -1,   292,   206,    -1,   292,   206,   345,    -1,    61,
      -1,    66,    -1,   294,    -1,   293,   294,    -1,   221,   295,
      93,    -1,   223,    93,    -1,    93,    -1,   110,   294,    -1,
     297,    -1,   297,    88,   296,    -1,   298,    -1,   296,    88,
     298,    -1,   247,   207,    -1,   247,   207,   299,    -1,    92,
     342,    -1,   345,   207,    92,   342,    -1,   246,   207,    -1,
     246,   207,   299,    -1,   207,    92,   342,    -1,   345,   207,
      92,   342,    -1,    95,   342,    -1,   301,    -1,   304,    -1,
     302,    -1,   303,    -1,   305,    -1,   308,    -1,   310,    -1,
     314,    -1,   307,    -1,   345,    92,   300,    -1,    11,   342,
      92,   300,    -1,    17,    92,   300,    -1,    11,   342,    64,
     342,    92,   300,    -1,   340,    93,    -1,    93,    -1,   199,
      -1,   101,   306,   103,    -1,   101,   103,    -1,   300,    -1,
     306,   300,    -1,    34,    84,   309,    85,   300,    -1,    34,
      84,   309,    85,   300,    21,   300,    -1,    63,    84,   309,
      85,   300,    -1,   340,    -1,    70,    84,   309,    85,   300,
      -1,    19,   300,    70,    84,   340,    85,    93,    -1,    28,
      84,   313,   312,    93,   311,    85,   300,    -1,   340,    -1,
      -1,   309,    -1,    -1,   303,    -1,   217,    -1,    10,    93,
      -1,    15,    93,    -1,    53,    93,    -1,    53,   340,    93,
      -1,    29,   345,    93,    -1,    29,    86,   340,    93,    -1,
     343,    -1,    84,   340,    85,    -1,   320,    -1,   316,    -1,
      84,   305,    85,    -1,   105,    84,   339,    88,   271,    85,
      -1,   106,    84,   271,    88,   319,    85,    -1,   107,    84,
     339,    88,   339,    88,   339,    85,    -1,   108,    84,   271,
      88,   271,    85,    -1,    77,    84,   339,    88,   317,    85,
      -1,   318,    -1,   317,    88,   318,    -1,   271,    92,   339,
      -1,    17,    92,   339,    -1,   345,   284,    -1,   345,    -1,
     321,    -1,   345,    -1,   315,    -1,   322,    98,   340,    99,
      -1,   322,    84,    85,    -1,   322,    84,   324,    85,    -1,
     322,    90,   320,    -1,   322,    51,   320,    -1,   322,    50,
      -1,   322,    43,    -1,    84,   271,    85,   323,    -1,   101,
     282,   103,    -1,   101,   282,    88,   103,    -1,   101,   103,
      -1,   339,    -1,   324,    88,   339,    -1,   322,    -1,    50,
     325,    -1,    43,   325,    -1,   326,   327,    -1,    58,   325,
      -1,    58,    84,   271,    85,    -1,   110,   327,    -1,   109,
     325,    -1,   109,    84,   271,    85,    -1,   111,   327,    -1,
     112,   327,    -1,     4,   345,    -1,    86,    -1,    83,    -1,
      87,    -1,    89,    -1,    81,    -1,   104,    -1,   325,    -1,
      84,   271,    85,   327,    -1,   327,    -1,   328,    86,   327,
      -1,   328,    91,   327,    -1,   328,    82,   327,    -1,   328,
      -1,   329,    87,   328,    -1,   329,    89,   328,    -1,   329,
      -1,   330,    39,   329,    -1,   330,    54,   329,    -1,   330,
      -1,   331,    94,   330,    -1,   331,    96,   330,    -1,   331,
      30,   330,    -1,   331,    41,   330,    -1,   331,    -1,   332,
      23,   331,    -1,   332,    46,   331,    -1,   332,    -1,   333,
      83,   332,    -1,   333,    -1,   334,   100,   333,    -1,   334,
      -1,   335,   102,   334,    -1,   335,    -1,   336,     4,   335,
      -1,   336,    -1,   337,    49,   336,    -1,   337,    -1,   337,
      97,   340,    92,   338,    -1,   337,    97,    92,   338,    -1,
     338,    -1,   337,   341,   339,    -1,   339,    -1,   340,    88,
     339,    -1,    95,    -1,    45,    -1,    18,    -1,     3,    -1,
      62,    -1,    40,    -1,    55,    -1,     5,    -1,    48,    -1,
      71,    -1,    44,    -1,   338,    -1,    16,    -1,    47,    -1,
      31,    -1,    32,    -1,    27,    -1,    26,    -1,     9,    -1,
      13,    -1,   344,    -1,    60,    -1,   344,    60,    -1,    33,
      -1,   346,    -1,   120,   340,    -1,   122,   306,    -1,   122,
      -1,   124,   293,    -1,   123,   195,    -1,   123,    -1,   125,
     271,    -1,   126,   347,    -1,   121,   324,    -1,   127,   320,
      -1,   232,    -1,   347,    88,   232,    -1,   128,    84,   350,
      85,    -1,   129,    84,   350,    85,    -1,   350,    88,   353,
      -1,   353,    -1,   130,    84,   353,    85,    -1,   131,    84,
     353,    85,    -1,    60,    -1,   348,    -1,   351,    -1,   349,
      -1,   352,    -1,   135,    84,   339,    88,   342,    85,    -1,
     136,    84,   339,    85,    -1,   322,    98,   311,    92,   311,
      99,    -1,   322,    98,   311,    92,   311,    92,   340,    99,
      -1,   322,    98,   340,    93,   340,    99,    -1,   322,    98,
     340,    93,   340,    92,   340,    99,    -1,   325,    -1,    84,
     271,    85,   327,    -1,   355,   354,    -1,   355,   356,    -1,
     356,    -1,    98,   340,    99,    -1,    28,    98,   351,    99,
      84,   313,   312,    93,   311,    85,   300,    -1,   357,    -1,
     133,    -1,   134,    -1,   358,    -1,   101,    91,   359,    91,
     103,    -1,   339,    88,   360,    -1,   361,    -1,   360,    88,
     361,    -1,   345,    95,   362,    -1,   364,    -1,   363,    -1,
     365,    -1,   339,    92,   339,    -1,   339,    92,   339,    92,
     339,    -1,   339,    93,   339,    -1,   339,    93,   339,    92,
     339,    -1,   101,   324,   103,    -1,   366,    -1,   137,    84,
     340,    85,    93,    -1,   367,    -1,   138,    -1,   139,    -1,
     368,    -1,   368,    -1,   372,    -1,   369,    -1,   370,    -1,
     369,    -1,   371,    -1,   369,    -1,   144,   373,    -1,   144,
     374,   196,    -1,   144,   374,   294,    -1,   144,   374,   300,
      -1,   146,   375,   145,    -1,   146,    84,   378,    85,   375,
     145,    -1,   145,    -1,   147,   375,   145,    -1,   147,    84,
     378,    85,   375,   145,    -1,    -1,   376,    -1,   377,    -1,
     376,    88,   377,    -1,   376,   377,    -1,   151,    84,   378,
      85,    -1,   151,    84,    85,    -1,   151,    -1,   379,    -1,
     380,    -1,   379,   380,    -1,   381,    -1,   152,    -1,   140,
     142,    84,   345,    85,   143,    -1,   140,   143,    -1,   382,
      -1,   382,    -1,   153,   383,    -1,   154,   392,    -1,   384,
      92,   386,    92,   389,    -1,   384,    92,   386,    92,   389,
      92,   390,    -1,   387,    -1,   385,    -1,   345,    -1,   388,
      -1,   386,    88,   388,    -1,    87,    -1,    89,    -1,    86,
      -1,    83,    -1,   102,    -1,   100,    -1,     4,    -1,    49,
      -1,   232,    -1,   340,    -1,   320,   280,    -1,   322,    84,
      85,    -1,   322,    84,   324,    85,    -1,   340,    -1,    -1,
     320,    -1,   392,    98,   391,    92,   391,    99,    -1,   392,
      98,   340,    99,    -1,   155,   393,    -1,   339,    -1,   394,
      -1,   101,   359,   103,    -1,   157,   396,   397,    -1,   158,
     395,    -1,   159,   340,   397,    -1,   396,   397,    -1,   395,
      88,   396,   397,    -1,   264,    -1,   396,    98,   339,    99,
      -1,    -1,   398,    -1,   399,    -1,   398,   399,    -1,   101,
     103,    -1,   101,   340,   103,    -1,   101,   340,   156,   340,
     103,    -1,   101,   340,    92,   340,   103,    -1,   170,   325,
      -1,   170,    84,   271,    85,    -1,   166,   325,    -1,   166,
      84,   271,    85,    -1,   167,   325,    -1,   167,    84,   271,
      85,    -1,   400,    -1,   401,    -1,   162,    -1,   162,   402,
      -1,   161,    -1,   163,    -1,   403,    -1,   402,   403,    -1,
      98,    99,    -1,    98,   342,    99,    -1,    98,    86,    99,
      -1,   404,    -1,   172,   405,    93,    -1,   173,   405,    93,
      -1,   165,   405,    93,    -1,   168,    93,    -1,   340,    -1,
      -1,   169,    84,   313,   405,    93,   405,    93,   406,    85,
     300,    -1,   407,    -1,    -1,   340,    -1,    15,    -1,   408,
      -1,   409,    -1,   174,    -1,   175,    -1,   176,    -1,   177,
      -1,   178,    -1,   322,   410,    84,    85,    -1,   322,   410,
      84,   324,    85,    -1,   412,   411,   413,    -1,   339,    88,
     339,    88,   339,    88,   339,    -1,   339,    88,   339,    88,
     339,    -1,   339,    88,   339,    -1,   179,    -1,   180,    -1,
     414,    -1,   181,    -1,   182,    -1,   183,    -1,   184,    -1,
     185,    -1,   368,   206,    -1,   368,    -1,   415,    -1,   186,
      84,   416,    85,    -1,    -1,   417,    -1,   418,    -1,   417,
      88,   418,    -1,   345,    -1,   345,    84,   324,    85,    -1,
     415,    -1,   187,    -1,   188,    -1,   189,    -1,   190,    -1,
     191,    84,   340,    85,    -1,   192,    84,   339,    88,   339,
      85,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,  2077,  2077,  2082,  2091,  2095,  2101,  2105,  2111,  2115,
    2119,  2126,  2132,  2136,  2140,  2146,  2150,  2156,  2162,  2167,
    2171,  2176,  2181,  2188,  2194,  2199,  2208,  2214,  2218,  2224,
    2229,  2234,  2238,  2244,  2248,  2255,  2261,  2267,  2276,  2281,
    2287,  2293,  2302,  2305,  2311,  2316,  2322,  2326,  2332,  2336,
    2342,  2350,  2354,  2361,  2365,  2369,  2375,  2381,  2385,  2389,
    2393,  2397,  2404,  2408,  2414,  2418,  2422,  2426,  2430,  2436,
    2440,  2444,  2448,  2452,  2464,  2468,  2472,  2476,  2482,  2486,
    2492,  2496,  2502,  2506,  2510,  2515,  2520,  2524,  2528,  2532,
    2537,  2541,  2547,  2551,  2555,  2559,  2564,  2570,  2574,  2580,
    2584,  2590,  2594,  2598,  2602,  2608,  2612,  2616,  2622,  2626,
    2630,  2634,  2638,  2644,  2648,  2652,  2656,  2663,  2667,  2673,
    2679,  2685,  2689,  2693,  2697,  2701,  2705,  2709,  2713,  2717,
    2723,  2729,  2740,  2744,  2750,  2754,  2760,  2764,  2770,  2774,
    2781,  2784,  2790,  2794,  2798,  2805,  2812,  2816,  2822,  2826,
    2832,  2836,  2840,  2844,  2854,  2858,  2864,  2868,  2875,  2879,
    2885,  2890,  2899,  2903,  2907,  2911,  2915,  2919,  2925,  2931,
    2936,  2944,  2947,  2953,  2961,  2964,  2970,  2978,  2981,  2985,
    2991,  2995,  3001,  3005,  3010,  3015,  3021,  3025,  3029,  3033,
    3037,  3041,  3047,  3053,  3059,  3064,  3069,  3075,  3079,  3085,
    3089,  3095,  3101,  3109,  3113,  3119,  3123,  3127,  3133,  3137,
    3141,  3147,  3151,  3155,  3159,  3163,  3167,  3171,  3175,  3179,
    3185,  3191,  3197,  3205,  3209,  3216,  3220,  3226,  3230,  3234,
    3240,  3246,  3250,  3256,  3260,  3264,  3270,  3277,  3285,  3295,
    3301,  3305,  3311,  3315,  3323,  3327,  3333,  3339,  3343,  3349,
    3353,  3359,  3369,  3373,  3379,  3383,  3390,  3396,  3405,  3409,
    3415,  3419,  3425,  3429,  3434,  3439,  3445,  3449,  3455,  3459,
    3465,  3469,  3473,  3477,  3486,  3490,  3494,  3498,  3507,  3517,
    3521,  3527,  3531,  3535,  3539,  3543,  3547,  3551,  3557,  3563,
    3567,  3572,  3578,  3582,  3589,  3595,  3599,  3605,  3609,  3624,
    3628,  3634,  3640,  3646,  3650,  3654,  3661,  3666,  3671,  3676,
    3682,  3686,  3692,  3696,  3700,  3704,  3708,  3715,  3725,  3729,
    3733,  3738,  3752,  3756,  3760,  3764,  3768,  3774,  3780,  3784,
    3790,  3794,  3808,  3814,  3822,  3828,  3834,  3838,  3842,  3846,
    3850,  3854,  3858,  3862,  3867,  3873,  3877,  3881,  3887,  3892,
    3899,  3903,  3907,  3911,  3915,  3919,  3924,  3928,  3932,  3936,
    3940,  3944,  3952,  3956,  3960,  3964,  3968,  3972,  3978,  3982,
    3988,  3992,  3996,  4000,  4006,  4010,  4014,  4020,  4024,  4028,
    4034,  4038,  4042,  4046,  4050,  4056,  4060,  4064,  4070,  4074,
    4080,  4084,  4090,  4094,  4100,  4104,  4110,  4114,  4120,  4124,
    4129,  4135,  4139,  4145,  4149,  4157,  4161,  4165,  4169,  4173,
    4177,  4181,  4185,  4189,  4193,  4197,  4203,  4213,  4217,  4221,
    4225,  4229,  4233,  4237,  4241,  4245,  4252,  4256,  4273,  4299,
    4305,  4309,  4313,  4317,  4321,  4325,  4329,  4333,  4337,  4341,
    4368,  4372,  4400,  4407,  4417,  4421,  4427,  4433,  4443,  4452,
    4458,  4472,  4479,  4528,  4532,  4538,  4542,  4546,  4550,  4556,
    4560,  4566,  4572,  4576,  4582,  4588,  4597,  4612,  4616,  4622,
    4624,  4630,  4637,  4641,  4648,  4655,  4659,  4663,  4669,  4673,
    4679,  4683,  4689,  4706,  4712,  4733,  4741,  4833,  4839,  4845,
    4875,  4879,  4885,  4889,  4895,  4899,  4937,  4944,  4950,  4956,
    5086,  5090,  5094,  5101,  5105,  5112,  5115,  5121,  5125,  5129,
    5135,  5139,  5143,  5149,  5157,  5161,  5167,  5173,  5181,  5187,
    5193,  5199,  5228,  5232,  5238,  5242,  5248,  5252,  5258,  5264,
    5268,  5274,  5275,  5276,  5277,  5278,  5279,  5280,  5281,  5285,
    5304,  5311,  5318,  5322,  5349,  5354,  5359,  5363,  5369,  5388,
    5394,  5398,  5404,  5427,  5431,  5435,  5441,  5446,  5453,  5457,
    5465,  5468,  5471,  5475,  5481,  5485,  5489,  5493,  5528,  5532,
    5536,  5540,  5544,  5548,  5554,  5558,  5564,  5568,  5574,  5578,
    5585,  5589,  5595,  5599,  5603,  5611,  5617,  5621,  5625,  5629,
    5635,  5640,  5645,  5654,  5659,  5663,  5667,  5694,  5700,  5706,
    5710,  5714,  5718,  5722,  5728,  5732,  5738,  5744,  5748,  5752,
    5758,  5765,  5797,  5803,  5807,  5811,  5815,  5828,  5835,  5848,
    5882,  5888,  5895,  5898,  5904,  5908,  5914,  5918,  5924,  5930,
    5934,  5938,  5942,  5958,  5962
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of input\"", "error", "$undefined", "\"+=\"", "\"&&\"", "\"&=\"",
  "\"__asm__\"", "\"auto storage-specifier\"", "\"_Bool\"",
  "\"boolean-literal\"", "\"break\"", "\"case\"", "\"char\"",
  "\"character-literal\"", "\"const\"", "\"continue\"",
  "\"decimal-literal\"", "\"default\"", "\"/=\"", "\"do\"", "\"double\"",
  "\"else\"", "\"enum\"", "\"==\"", "\"extern\"", "\"float\"",
  "\"floating-literal\"", "\"hexadecimal-floating-literal\"", "\"for\"",
  "\"goto\"", "\">=\"", "\"binary-integer-literal\"",
  "\"hexadecimal-integer-literal\"", "\"identifier\"", "\"if\"",
  "\"inline\"", "\"int\"", "\"__int128\"", "\"__float128\"", "\"<<\"",
  "\"<<=\"", "\"<=\"", "\"long\"", "\"--\"", "\"%=\"", "\"*=\"", "\"!=\"",
  "\"octal-integer-literal\"", "\"|=\"", "\"||\"", "\"++\"", "\"->\"",
  "\"register\"", "\"return\"", "\">>\"", "\">>=\"", "\"short\"",
  "\"signed\"", "\"sizeof\"", "\"static\"", "\"string-literal\"",
  "\"struct\"", "\"-=\"", "\"switch\"", "\"...\"", "\"typedef\"",
  "\"union\"", "\"unsigned\"", "\"void\"", "\"volatile\"", "\"while\"",
  "\"^=\"", "\"<preprocessor-comment>\"", "\"<preprocessor-token>\"",
  "\"<C-reference-specifier>\"", "\"<rebindable-reference-specifier>\"",
  "\"_Atomic\"", "\"_Generic\"", "\"_Noreturn\"", "\"_Static_assert\"",
  "\"_Thread_local\"", "'!'", "'%'", "'&'", "'('", "')'", "'*'", "'+'",
  "','", "'-'", "'.'", "'/'", "':'", "';'", "'<'", "'='", "'>'", "'?'",
  "'['", "']'", "'^'", "'{'", "'|'", "'}'", "'~'", "\"__builtin_va_arg\"",
  "\"__builtin_offsetof\"", "\"__builtin_choose_expr\"",
  "\"__builtin_types_compatible_p\"", "\"__alignof__\"",
  "\"__extension__\"", "\"__real__\"", "\"__imag__\"", "\"__label__\"",
  "\"__complex__\"", "\"__imaginary__\"", "\"__typeof__\"", "\"restrict\"",
  "\"__attribute__\"", "\"__thread\"", "\"<subparse-expression>\"",
  "\"<subparse-expression-list>\"", "\"<subparse-statement>\"",
  "\"<subparse-declaration>\"", "\"<subparse-member>\"",
  "\"<subparse-type>\"", "\"<subparse-type-list>\"",
  "\"<subparse-id-expression>\"", "\"<nodecl-literal-expression>\"",
  "\"<nodecl-literal-statement>\"", "\"<symbol-literal-reference>\"",
  "\"<type-literal-reference>\"", "\"<byte-type-spec>\"",
  "\"<bool-type-spec>\"", "\"<mask-type-spec>\"",
  "\"@array-subscript-check@\"", "\"@const-value-check@\"",
  "C_FORTRAN_ALLOCATE", "\"<statement-placeholder>\"",
  "\"<unknown-pragma>\"", "\"<verbatim pragma>\"",
  "\"<verbatim construct>\"", "\"<verbatim type clause>\"",
  "\"<verbatim text>\"", "\"<pragma-custom>\"",
  "\"<pragma-custom-newline>\"", "\"<pragma-custom-directive>\"",
  "\"<pragma-custom-construct>\"", "\"<pragma-custom-end-construct>\"",
  "\"<pragma-custom-construct-noend>\"",
  "\"<pragma-custom-end-construct-noend>\"", "\"<pragma-custom-clause>\"",
  "\"<pragma-clause-argument-text>\"", "\"<omp-declare-reduction>\"",
  "\"<omp-depend-item>\"", "\"<ompss-dependency-expression>\"", "\"..\"",
  "\"<subparse-superscalar-declarator>\"",
  "\"<subparse-superscalar-declarator-list>\"",
  "\"<subparse-superscalar-expression>\"", "\"MYTHREAD (UPC)\"",
  "\"relaxed (UPC)\"", "\"shared (UPC)\"", "\"strict (UPC)\"",
  "\"THREADS (UPC)\"", "\"upc_barrier\"", "\"upc_blocksizeof\"",
  "\"upc_elemsizeof\"", "\"upc_fence\"", "\"upc_forall\"",
  "\"upc_localsizeof\"", "\"UPC_MAX_BLOCKSIZE\"", "\"upc_notify\"",
  "\"upc_wait\"", "\"__device__\"", "\"__global__\"", "\"__host__\"",
  "\"__constant__\"", "\"__shared__\"", "\"<<<\"", "\">>>\"",
  "\"__global\"", "\"__kernel\"", "\"__constant\"", "\"__local\"",
  "\"_Builtin\"", "\"__declspec\"", "\"__int8\"", "\"__int16\"",
  "\"__int32\"", "\"__int64\"", "\"__assume\"", "\"__assume_aligned\"",
  "$accept", "translation_unit", "declaration_sequence", "declaration",
  "block_declaration", "static_assert_declaration",
  "non_empty_block_declaration", "nested_block_declaration",
  "nested_function_definition", "common_block_declaration",
  "label_declaration", "label_declarator_seq", "attribute_specifier",
  "attribute_specifier_seq", "attribute_specifier_seq_opt",
  "gcc_attribute", "gcc_attribute_list", "gcc_attribute_value",
  "asm_definition", "volatile_optional", "asm_operand_list",
  "asm_operand_list_nonempty", "asm_operand", "simple_declaration",
  "non_empty_simple_declaration", "old_style_parameter",
  "old_style_decl_specifier_seq",
  "nontype_specifier_without_attribute_seq", "decl_specifier_seq",
  "decl_specifier_seq_may_end_with_declarator", "decl_specifier_alone_seq",
  "nontype_specifier_seq", "nontype_specifier",
  "nontype_specifier_without_attribute", "storage_class_specifier",
  "function_specifier", "type_specifier", "type_specifier_alone",
  "declarating_type_specifier", "type_specifier_seq",
  "simple_type_specifier", "typeof_type_specifier",
  "atomic_type_specifier", "type_name", "builtin_types",
  "elaborated_type_specifier", "init_declarator_list",
  "init_declarator_list_nonempty", "init_declarator",
  "init_declarator_first", "gcc_extra_bits_init_declarator_opt",
  "gcc_extra_bits_init_declarator", "asm_specification", "declarator",
  "declarator_not_started_with_attributes", "ptr_operator",
  "functional_declarator",
  "functional_declarator_not_started_with_attributes",
  "parameter_or_kr_list", "parameters_and_qualifiers",
  "functional_direct_declarator", "functional_declarator_id",
  "functional_final_declarator_id", "optional_array_cv_qualifier_seq",
  "array_cv_qualifier_seq", "optional_array_static_qualif",
  "array_static_qualif", "optional_array_expression", "cv_qualifier_seq",
  "cv_qualifier", "direct_declarator", "declarator_id", "enum_head",
  "identifier_opt", "enum_key", "enum_specifier", "enumeration_list",
  "enumeration_definition", "type_id", "abstract_declarator",
  "abstract_declarator_not_started_with_attributes",
  "abstract_direct_declarator", "identifier_list", "identifier_list_kr",
  "parameter_type_list", "parameter_declaration_list",
  "parameter_declaration", "initializer", "initializer_clause",
  "initializer_list", "designation", "designator_list", "designator",
  "function_definition", "function_definition_header",
  "old_style_parameter_list", "function_body", "class_specifier",
  "class_head", "class_key", "member_specification_seq",
  "member_declaration", "member_declarator_list",
  "member_declarator_list_nonempty", "member_declarator_first",
  "member_declarator", "constant_initializer", "statement",
  "nondeclarating_statement", "labeled_statement", "expression_statement",
  "declaration_statement", "compound_statement", "statement_seq",
  "if_statement", "selection_statement", "condition",
  "iteration_statement", "expression_opt", "condition_opt",
  "for_init_statement", "jump_statement", "primary_expression",
  "generic_selection", "generic_assoc_list", "generic_association",
  "offsetof_member_designator", "id_expression", "unqualified_id",
  "postfix_expression", "braced_init_list", "expression_list",
  "unary_expression", "unary_operator", "cast_expression",
  "multiplicative_expression", "additive_expression", "shift_expression",
  "relational_expression", "equality_expression", "and_expression",
  "exclusive_or_expression", "inclusive_or_expression",
  "logical_and_expression", "logical_or_expression",
  "conditional_expression", "assignment_expression", "expression",
  "assignment_operator", "constant_expression", "literal",
  "string_literal", "identifier_token", "subparsing", "subparse_type_list",
  "nodecl_literal_expr", "nodecl_literal_stmt",
  "nodecl_literal_attribute_seq", "symbol_literal_ref", "type_literal_ref",
  "nodecl_string_literal", "noshape_cast_expression", "shape_seq", "shape",
  "mercurium_extended_type_specifiers", "multiexpression",
  "multiexpression_body", "multiexpression_iterator_list",
  "multiexpression_iterator", "multiexpression_range",
  "multiexpression_range_section", "multiexpression_range_size",
  "multiexpression_range_discrete", "fortran_allocate_statement",
  "statement_placeholder", "unknown_pragma", "pragma_custom_directive",
  "pragma_custom_construct_declaration",
  "pragma_custom_construct_member_declaration",
  "pragma_custom_construct_statement", "pragma_custom_line_directive",
  "pragma_custom_line_construct", "pragma_custom_clause_opt_seq",
  "pragma_custom_clause_seq", "pragma_custom_clause",
  "pragma_clause_arg_list", "pragma_clause_arg", "pragma_clause_arg_item",
  "pragma_clause_arg_text", "verbatim_construct", "omp_declare_reduction",
  "omp_dr_reduction_id", "omp_dr_identifier", "omp_dr_typename_list",
  "omp_dr_operator", "omp_dr_typename", "omp_dr_combiner",
  "omp_dr_initializer", "omp_expression_opt", "omp_depend_item",
  "ompss_dependency_expr", "ompss_old_multidependences",
  "superscalar_declarator_list", "superscalar_declarator",
  "opt_superscalar_region_spec_list", "superscalar_region_spec_list",
  "superscalar_region_spec", "upc_shared_type_qualifier",
  "upc_reference_type_qualifier", "upc_layout_qualifier",
  "upc_layout_qualifier_element", "upc_synchronization_statement",
  "upc_expression_opt", "upc_affinity_opt", "upc_affinity",
  "cuda_specifiers", "cuda_kernel_call", "cuda_kernel_arguments",
  "cuda_kernel_config_list", "cuda_kernel_config_left",
  "cuda_kernel_config_right", "opencl_specifiers", "declspec_specifier",
  "extended_decl_modifier_list", "extended_decl_modifier_list0",
  "extended_decl_modifier", YY_NULL
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short int yyr1[] =
{
       0,   193,   194,   194,   195,   195,   196,   196,   197,   197,
     197,   198,   199,   199,   199,   200,   200,   201,   202,   202,
     202,   202,   202,   203,   204,   204,   205,   206,   206,   207,
     207,   208,   208,   209,   209,   210,   210,   210,   211,   211,
     211,   211,   212,   212,   213,   213,   214,   214,   215,   215,
     215,   216,   216,   217,   217,   217,   218,   219,   219,   219,
     219,   219,   220,   220,   221,   221,   221,   221,   221,   222,
     222,   222,   222,   222,   223,   223,   223,   223,   224,   224,
     225,   225,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   227,   227,   227,   227,   227,   228,   228,   229,
     229,   230,   230,   230,   230,   231,   231,   231,   232,   232,
     232,   232,   232,   233,   233,   233,   233,   234,   234,   235,
     236,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     238,   238,   239,   239,   240,   240,   241,   241,   242,   242,
     243,   243,   244,   244,   244,   245,   246,   246,   247,   247,
     248,   248,   248,   248,   249,   249,   250,   250,   251,   251,
     252,   252,   253,   253,   253,   253,   253,   253,   254,   255,
     255,   256,   256,   257,   258,   258,   259,   260,   260,   260,
     261,   261,   262,   262,   262,   262,   263,   263,   263,   263,
     263,   263,   264,   265,   266,   266,   267,   268,   268,   269,
     269,   270,   270,   271,   271,   272,   272,   272,   273,   273,
     273,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     275,   276,   276,   277,   277,   278,   278,   279,   279,   279,
     280,   281,   281,   282,   282,   282,   282,   282,   282,   283,
     284,   284,   285,   285,   286,   286,   286,   287,   287,   288,
     288,   289,   290,   290,   291,   291,   291,   291,   292,   292,
     293,   293,   294,   294,   294,   294,   295,   295,   296,   296,
     297,   297,   297,   297,   298,   298,   298,   298,   299,   300,
     300,   301,   301,   301,   301,   301,   301,   301,   302,   302,
     302,   302,   303,   303,   304,   305,   305,   306,   306,   307,
     307,   308,   309,   310,   310,   310,   311,   311,   312,   312,
     313,   313,   314,   314,   314,   314,   314,   314,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   316,   317,   317,
     318,   318,   319,   319,   320,   321,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   323,   323,   323,   324,   324,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   326,   326,   326,   326,   326,   326,   327,   327,
     328,   328,   328,   328,   329,   329,   329,   330,   330,   330,
     331,   331,   331,   331,   331,   332,   332,   332,   333,   333,
     334,   334,   335,   335,   336,   336,   337,   337,   338,   338,
     338,   339,   339,   340,   340,   341,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   341,   342,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   344,   344,   345,   194,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     347,   347,   348,   349,   350,   350,   351,   352,   353,   315,
     321,   301,   233,   315,   315,   322,   322,   322,   322,   354,
     354,   327,   355,   355,   356,   310,   226,   357,   357,   315,
     358,   359,   360,   360,   361,   362,   362,   362,   363,   363,
     364,   364,   365,   301,   366,   300,   367,   368,   202,   294,
     301,   301,   196,   196,   294,   294,   369,   370,   371,   372,
     373,   373,   373,   374,   374,   375,   375,   376,   376,   376,
     377,   377,   377,   378,   379,   379,   380,   381,   382,   382,
     202,   294,   346,   346,   383,   383,   384,   384,   385,   386,
     386,   387,   387,   387,   387,   387,   387,   387,   387,   388,
     389,   390,   390,   390,   391,   391,   392,   392,   392,   346,
     393,   393,   394,   346,   346,   346,   395,   395,   396,   396,
     397,   397,   398,   398,   399,   399,   399,   399,   325,   325,
     325,   325,   325,   325,   262,   262,   400,   400,   401,   401,
     402,   402,   403,   403,   403,   301,   404,   404,   404,   404,
     405,   405,   310,   406,   406,   407,   407,   226,   322,   408,
     408,   408,   408,   408,   409,   409,   410,   411,   411,   411,
     412,   413,   226,   414,   414,   414,   414,   226,   244,   244,
     225,   415,   416,   416,   417,   417,   418,   418,   205,   237,
     237,   237,   237,   315,   315
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     0,     1,     2,     1,     1,     1,     1,
       1,     7,     1,     1,     1,     2,     1,     1,     1,     1,
       2,     1,     1,     3,     1,     3,     1,     2,     1,     1,
       0,     6,     5,     1,     3,     1,     1,     4,     6,     8,
      10,    12,     0,     1,     1,     0,     1,     3,     4,     7,
       1,     1,     1,     3,     2,     2,     3,     3,     2,     2,
       1,     1,     1,     2,     3,     2,     2,     1,     1,     3,
       2,     2,     1,     1,     3,     2,     2,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     2,
       2,     1,     1,     1,     1,     1,     1,     4,     4,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     1,     3,     1,     3,     2,     3,     2,     3,
       0,     1,     1,     1,     2,     4,     2,     3,     1,     2,
       1,     2,     1,     1,     2,     3,     1,     2,     1,     1,
       3,     2,     1,     2,     4,     6,     6,     3,     2,     1,
       3,     0,     1,     1,     0,     1,     1,     0,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     2,     4,     6,
       6,     3,     1,     3,     1,     0,     1,     4,     5,     3,
       1,     1,     3,     1,     2,     2,     3,     2,     1,     2,
       1,     3,     1,     2,     3,     5,     5,     4,     6,     6,
       1,     1,     3,     1,     3,     1,     3,     3,     2,     3,
       2,     1,     1,     1,     3,     2,     4,     3,     5,     2,
       1,     2,     3,     2,     2,     3,     2,     2,     1,     1,
       2,     1,     4,     3,     1,     2,     2,     3,     1,     1,
       1,     2,     3,     2,     1,     2,     1,     3,     1,     3,
       2,     3,     2,     4,     2,     3,     3,     4,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     4,
       3,     6,     2,     1,     1,     3,     2,     1,     2,     5,
       7,     5,     1,     5,     7,     8,     1,     0,     1,     0,
       1,     1,     2,     2,     2,     3,     3,     4,     1,     3,
       1,     1,     3,     6,     6,     8,     6,     6,     1,     3,
       3,     3,     2,     1,     1,     1,     1,     4,     3,     4,
       3,     3,     2,     2,     4,     3,     4,     2,     1,     3,
       1,     2,     2,     2,     2,     4,     2,     2,     4,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     4,
       1,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     5,
       4,     1,     3,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     2,     2,
       1,     3,     4,     4,     3,     1,     4,     4,     1,     1,
       1,     1,     1,     6,     4,     6,     8,     6,     8,     1,
       4,     2,     2,     1,     3,    11,     1,     1,     1,     1,
       5,     3,     1,     3,     3,     1,     1,     1,     3,     5,
       3,     5,     3,     1,     5,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     3,     3,     3,
       3,     6,     1,     3,     6,     0,     1,     1,     3,     2,
       4,     3,     1,     1,     1,     2,     1,     1,     6,     2,
       1,     1,     2,     2,     5,     7,     1,     1,     1,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     3,     4,     1,     0,     1,     6,     4,     2,
       1,     1,     3,     3,     2,     3,     2,     4,     1,     4,
       0,     1,     1,     2,     2,     3,     5,     5,     2,     4,
       2,     4,     2,     4,     1,     1,     1,     2,     1,     1,
       1,     2,     2,     3,     3,     1,     3,     3,     3,     2,
       1,     0,    10,     1,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     5,     3,     7,     5,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     4,     0,     1,     1,     3,     1,     4,     1,     1,
       1,     1,     1,     4,     6
};

/* YYDPREC[RULE-NUM] -- Dynamic precedence of rule #RULE-NUM (0 if none).  */
static const unsigned char yydprec[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     2,
       1,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     2,     1,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0
};

/* YYMERGER[RULE-NUM] -- Index of merging function for rule #RULE-NUM.  */
static const unsigned char yymerger[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     1,     1,     1,     0,     0,     0,     0,
       0,     0,     0,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     1,     1,
       1,     1,     1,     0,     0,     0,     0,     1,     1,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     1,     1,
       0,     1,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     1,     1,     1,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     1,     1,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     1,     1,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       1,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       1,     1,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     1,     1,     0,     1,     1,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       1,     1,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     1,     1,     1,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0
};

/* YYDEFACT[S] -- default reduction number in state S.  Performed when
   YYTABLE doesn't specify something else to do.  Zero means the default
   is an error.  */
static const unsigned short int yydefact[] =
{
       3,    42,    92,   122,   121,   182,   125,   196,    95,   124,
     428,    97,   123,   127,   128,    88,    93,    89,    86,    94,
     258,    84,   259,    87,   126,   183,    21,    22,   152,   153,
     185,    98,     0,    30,   150,    52,     0,     0,    90,    91,
       0,   184,     0,    96,     0,     0,   432,   435,     0,     0,
       0,     0,     0,     0,   129,   467,   468,   487,     0,     0,
       0,     0,     0,     0,     0,     0,   578,   576,   579,   599,
     600,   601,   602,   603,   613,   614,   615,   616,   617,     0,
     629,   630,   631,   632,     0,     2,     4,     6,    10,     9,
      19,    81,    18,     8,    51,     0,     0,    68,    78,    80,
      82,    83,    67,    77,    99,   100,   103,   104,   113,   102,
     107,     0,   132,   140,    30,   248,   156,   162,     0,    85,
     148,   186,     0,    30,   106,     7,     0,   105,     0,   254,
     192,   334,   120,   429,   450,   452,   466,   488,   493,   492,
     520,   574,   575,   597,   612,   620,    43,     0,     0,     0,
       0,    28,    29,     0,    26,     0,     0,     0,   169,   335,
     628,   185,   151,   180,    20,   246,     0,    24,     0,     0,
       0,   423,   424,   417,   422,   421,   419,   420,     0,   418,
       0,     0,   426,     0,   366,   363,     0,   362,   364,   365,
       0,     0,   367,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   336,
     321,   320,   350,   368,     0,   370,   374,   377,   380,   385,
     388,   390,   392,   394,   396,   398,   401,   403,   430,   318,
     425,   449,     0,   463,   469,   598,   438,   348,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    30,
     362,   293,     0,     0,     0,     0,   486,     0,   591,     0,
       0,   591,   591,   294,    14,    16,    13,    12,    17,   297,
     279,   281,   282,   280,   283,   431,   287,   284,   285,   286,
     320,     0,   120,   451,   483,   485,   491,   490,   585,   434,
     264,     0,     0,     0,     0,   433,   260,   120,   489,   495,
     494,   521,   112,   111,    99,   203,   115,   116,   114,   436,
     440,   437,   439,     0,     0,     0,   519,   502,   505,   505,
     496,     0,   537,   538,   534,   533,   531,   532,   536,   535,
     528,   522,     0,   527,   526,   546,   523,     0,   550,   549,
     551,   558,   560,   554,   560,   560,     0,   577,   580,   622,
       1,     5,     0,   247,    54,    79,    65,    75,    66,    76,
      55,    30,     0,   143,   138,   141,   142,   619,   149,   157,
       0,   177,   163,   168,   177,   187,     0,   195,     0,   249,
       0,    61,    62,    60,     0,   244,   251,     0,   256,     0,
     255,     0,     0,   398,   416,     0,    27,    30,   154,   146,
     191,   167,   170,   181,     0,    23,     0,     0,     0,   361,
       0,   352,   351,     0,   354,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   357,   356,   359,   360,
       0,     0,     0,     0,   570,     0,   572,     0,   568,     0,
       0,   343,   342,     0,     0,     0,   307,   610,     0,     0,
     353,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   408,   412,
     407,   410,   415,   406,   413,     0,   411,   409,   414,   405,
       0,     0,     0,   427,     0,   459,   461,   462,     0,   312,
       0,   313,     0,     0,     0,     0,     0,     0,     0,   314,
       0,     0,     0,     0,    81,   620,   296,     0,    15,    17,
       0,     0,     0,   590,     0,   589,     0,     0,     0,   298,
     292,     0,   265,     0,    30,     0,    30,    30,   186,     0,
     266,   335,   263,   261,   109,   110,    30,   177,    30,   212,
     204,   210,     0,   448,     0,     0,     0,     0,   512,     0,
     506,   507,     0,     0,   497,     0,   545,     0,     0,     0,
       0,   553,   561,   562,     0,   556,   555,   362,   582,     0,
     581,   626,     0,   623,   624,    53,    64,    74,     0,   133,
     134,   140,     0,     0,   139,   144,   618,   161,     0,    30,
      68,    67,     0,   159,   220,   158,   223,   225,   120,   176,
     362,   174,   171,     0,   173,   179,   174,   171,     0,     0,
     200,   201,   193,   131,     0,    63,    58,    59,   250,   245,
     253,     0,   257,   130,     0,   119,     0,   147,   155,    25,
     118,   117,    36,     0,     0,    33,    35,     0,     0,     0,
       0,   322,   319,   464,     0,     0,     0,     0,     0,     0,
       0,   445,     0,     0,     0,     0,     0,     0,     0,   341,
     338,     0,   340,     0,   306,     0,     0,     0,   373,   371,
     372,   375,   376,   378,   379,   383,   384,   381,   382,   386,
     387,   389,   391,   393,   395,   397,     0,     0,   402,   404,
       0,   349,     0,     0,   290,     0,    30,   311,     0,   310,
     309,     0,     0,   316,     0,   302,   315,     0,     0,   295,
       0,     0,   499,   588,   591,   586,   587,   288,   498,   272,
     270,   262,    30,     0,   108,     0,     0,   174,   171,     0,
     209,   177,   213,   441,   446,   447,     0,   517,     0,   513,
     514,   516,     0,   500,     0,   509,     0,   503,   539,     0,
     529,   544,     0,     0,   552,     0,   564,     0,   563,   560,
     584,   583,     0,   621,     0,    30,    30,   136,     0,     0,
     230,   232,   231,    30,    30,    30,    30,   228,    65,    66,
     160,     0,     0,   177,   175,   177,   172,   164,   177,   177,
     188,     0,   197,     0,    56,    57,   252,     0,    45,     0,
      32,     0,     0,     0,     0,   355,     0,   344,   369,     0,
       0,     0,     0,     0,   358,   442,     0,     0,   454,   571,
     573,   569,   633,     0,   339,   307,     0,   337,   604,     0,
       0,   611,   606,   400,     0,     0,     0,   289,     0,   308,
       0,     0,   317,     0,     0,     0,   443,     0,     0,     0,
     271,     0,    30,   267,   268,    30,     0,    30,   207,   211,
     177,   177,   214,   174,   171,     0,     0,   505,   515,   511,
       0,   508,   505,     0,     0,   548,   545,     0,   471,   472,
     559,     0,   565,     0,   557,     0,   625,   135,   137,   145,
       0,     0,   347,   233,     0,     0,     0,   240,   335,     0,
     227,   229,    64,   222,   224,   226,     0,     0,     0,     0,
     198,   199,   202,    38,     0,     0,    44,    46,    50,     0,
      31,    34,     0,     0,     0,     0,     0,   328,   470,     0,
       0,   333,     0,     0,   444,     0,     0,     0,   306,     0,
     605,   609,   399,   460,     0,     0,   307,     0,   299,   301,
     303,   484,   591,   278,     0,   274,    30,     0,   273,   206,
       0,     0,   177,   177,   217,   518,     0,   510,     0,   530,
     540,   524,   544,     0,     0,     0,     0,     0,   627,   243,
     401,     0,     0,   345,   235,   239,     0,   241,     0,    30,
     165,   166,   189,   190,     0,     0,    45,     0,     0,    11,
      37,     0,     0,   327,     0,   323,   324,   332,     0,   326,
     453,   634,     0,   455,     0,   457,     0,   291,     0,     0,
     309,     0,     0,   276,   275,   269,     0,   215,   216,     0,
       0,   501,   504,     0,   547,     0,     0,   474,   476,   475,
     477,   473,   567,   566,   242,   346,   234,     0,   335,   237,
       0,    39,     0,    47,     0,   331,   330,   329,     0,     0,
       0,   608,   304,     0,     0,   300,   594,   277,   218,   219,
     320,     0,   525,     0,     0,     0,   236,     0,     0,     0,
      45,    48,   325,   456,   458,     0,   305,   307,   596,   595,
       0,   593,   541,     0,   482,   478,   480,   238,     0,    40,
       0,   607,     0,     0,   338,     0,     0,     0,     0,     0,
       0,   592,   339,   479,   481,    49,    41,   465
};

/* YYPDEFGOTO[NTERM-NUM].  */
static const short int yydefgoto[] =
{
      -1,    84,    85,    86,    87,    88,   263,   264,   265,   266,
      90,   166,   151,   152,   578,    91,   634,   635,    92,   147,
     915,   916,   917,    93,   267,   379,   380,   381,    95,   589,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   305,
     105,   106,   107,   108,   109,   110,   111,   579,   580,   112,
     364,   365,   366,   155,   113,   114,   156,   115,   592,   539,
     116,   117,   118,   785,   786,   783,   784,   603,   604,   119,
     120,   121,   122,   612,   123,   124,   609,   610,   416,   726,
     540,   541,   593,   594,   595,   596,   597,   584,   893,   894,
     895,   896,   897,   268,   126,   384,   385,   127,   128,   129,
     295,   296,   529,   853,   530,   854,   850,   269,   270,   271,
     272,   273,   274,   507,   276,   277,   839,   278,   663,   840,
     700,   279,   209,   210,   926,   927,   930,   211,   131,   212,
     771,   236,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   281,   481,   981,
     229,   230,   159,   133,   311,   231,   283,   650,   134,   135,
     651,   486,   232,   233,   136,   234,   558,   878,   879,  1037,
    1038,  1039,  1040,   284,   285,   137,   286,   139,   300,   287,
     320,   321,   549,   550,   551,   738,   739,   740,   741,   140,
     331,   332,   333,   749,   334,   750,   971,  1072,   752,   336,
     339,   340,   343,   342,   561,   562,   563,   141,   142,   347,
     348,   288,   514,  1090,  1091,   143,   235,   448,   667,   449,
     832,   144,   145,   572,   573,   574
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -903
static const short int yypact[] =
{
    7531,    32,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
      38,  -903,   150,    83,   451,  -903,  7901,   268,  -903,  -903,
     242,  -903,   272,  -903,  6150,  6150,  3277,  7716,  8453,  9269,
    9269,    58,   299,   327,  -903,  -903,  -903,  -903,   231,   563,
    1038,    58,  6263,    58,    58,  6150,  -903,   320,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,   350,
    -903,  -903,  -903,  -903,   440,  7716,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,   403,   383,  9453,  -903,  -903,
    -903,  -903, 11249, 11249, 11161,  -903, 10106, 10271,  -903, 10436,
    -903,   389,   400,    53,   -22,  -903,    60,  -903,   407,  -903,
     253,   413,   404,   -22,  -903,  -903,  9637,  -903,   423,    41,
    -903,  -903,  9913,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,   430,  9269,  6150,
      66,  -903,   -22,   403,  -903,   443,   454,   460,  -903,  -903,
    -903,  -903,   451,  -903,  -903,  -903,   250,  -903,  3844,   468,
     268,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  6828,  -903,
    6828,  6941,  -903,   474,  -903,  -903,  4033,  -903,  -903,  -903,
    6150,   487,  -903,   476,   498,   514,   521,  7054,  6150,  6150,
    6150,   547,   561,   578,  7167,  7280,  7393,   605,   606,  -903,
    -903,  -903,   749,  -903,  6150,  -903,   363,   366,    74,   258,
      84,   504,   535,   545,   694,  1473,  -903,  -903,   614,  -903,
     651,  -903,  6376,  -903,  -903,  -903,   618,  -903,   621,  6150,
     623,   626,  3277,   295,   122,   631,  5128,   635,   639,  4222,
    1140,  -903,  2710,  3466,   640,   642,  -903,   563,  6150,   634,
     644,  6150,  6150,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  3277,  -903,  -903,  -903,  -903,
     815,   310, 10014,  -903,  -903,  -903,  -903,  -903,  -903,  7716,
    -903,  8453,   563,   107,   645,  8453,  -903,  -903,  -903,  -903,
    -903,  -903,  9269, 11249,  -903,   271,  -903,  -903,  -903,  -903,
    -903,   652,  -903,   679,   679,   658,  -903,  -903,   -12,     9,
    -903,  7716,  -903,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,   654,  -903,  -903,  -903,   656,  5241,  -903,  -903,
    -903,  -903,   142,   655,   142,   247,  5354,   659,  -903,   268,
    -903,  -903,   657,  -903,  -903,  -903, 11249, 11249, 11249, 11249,
    -903,   -22,   665,   -22,   661,  -903,   -22,   -22,  -903,  -903,
    8725,  2320,  -903,  -903,  2320,  -903,   268,   268,  2899,  -903,
     951,  9725,  -903, 11249,  9637,  -903,  -903,  8085,    55,   268,
    -903,   692,   674,    30,  -903,   672,  -903,   -22,    60,   253,
    -903,  -903,  -903,  -903,   268,  -903,   677,   444,   133,  -903,
    4033,  -903,  -903,  4033,  -903,  6150,   682,   693,   459,   219,
    6150,  6150,  9269,  6150,  9269,  4033,  -903,  -903,  -903,  -903,
     679,  6150,  6150,  4033,  -903,  4033,  -903,  4033,  -903,  6150,
    6150,  -903,  -903,    58,  5467,    58,  6150,  -903,   695,  6150,
    -903,  6150,  6150,  6150,  6150,  6150,  6150,  6150,  6150,  6150,
    6150,  6150,  6150,  6150,  6150,  6150,  6150,  6150,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  6150,  -903,  -903,  -903,  -903,
    5580,  6150,  6150,  -903,  4033,  -903,  -903,  -903,  6150,  -903,
     112,  -903,  3277,   707,  3655,   650,  6150,   690,  6150,  -903,
     364,  6150,  6150,  4411,   577,  1014,  -903,  3088,  -903,  2521,
     679,  6150,  3277,   614,   691,  -903,  3655,   696,   700,  -903,
    -903,  3277,  -903,  8453,   -22,  6150,   -22,   -22,  -903,   702,
     698,    -3,  -903,  -903, 11249, 11249,  8725,  2320,    69,  -903,
    -903,   318,  9269,  -903,   703,   711,   268,   649,   719,   669,
      14,  -903,   649,   670,  -903,  9269,  6150,   716,   715,  6150,
    5693,  -903,   718,  -903,    58,  -903,  -903,   723,  -903,   724,
    -903,   741,   750,   752,  -903,  -903, 11249, 11249,   951,   754,
    -903,    53,   692,  6489,  -903,   -22,   -22,  -903,   646,   -22,
    8637, 10581,   761,  -903,   760,  -903,   762,  -903,   465,  -903,
     755,   790,   451,   756,   451,  -903,   790,   451,   758,   159,
    -903,   763,  -903,   759,   768,  -903, 11249, 11249,  -903,  -903,
    -903,  8269,  -903,  -903,   240,  -903,   692,  -903,  -903,  -903,
    -903,  -903,  -903,   766,   469,  -903,   781,   783,   789,   778,
    6489,  -903,  -903,  -903,   785,   792,   793,   794,   797,   801,
     482,  -903,   799,   803,   809,   816,   817,   491,   819,  -903,
    -903,   503,  -903,   786,   308,  5806,   821,   697,  -903,  -903,
    -903,   452,   483,   366,   366,    74,    74,    74,    74,   258,
     258,    84,   504,   535,   545,   694,  6150,   236,  -903,  -903,
     826,  -903,  6150,  3277,  -903,   820,  4600,  -903,   951,  -903,
    6150,   813,   397,  -903,   830,   614,  -903,   831,   832,  -903,
     507,   530,  -903,  -903,  6150,  -903,  -903,  -903,  -903,  -903,
     825,  -903,    99,   839, 11249,   271,   849,   790,   451,   837,
    -903,  2320,  -903,  -903,  -903,  -903,   853,  -903,   854,   649,
    -903,  -903,    -1,  -903,   791,  -903,   855,  -903,  -903,   410,
    -903,   287,   856,   268,  -903,   842,  -903,    33,  -903,   142,
    -903,  -903,  6150,  -903,   268,   -22,   -22,   661,   124,  4789,
    -903,  -903,  -903,  8909,   -22,    24,   -22,  -903, 10726, 10871,
    -903,   268,  8997,  6602,  -903,  6602,  -903,  -903,  6602,  6602,
    -903,    42,  -903,  6150,  -903, 11249,  -903,   852,    26,   196,
    -903,   861,   205,  6150,   857,   858,  9181,  -903,  -903,   850,
    9269,   268,  6150,  9269,   859,  -903,   679,  6150,  -903,   860,
     864,   869,  -903,  6150,  -903,  6150,  6150,  -903,  -903,   537,
    6150,  -903,  -903,  -903,  6150,  6489,   870,  -903,  6150,  -903,
     881,   894,  -903,  3277,  3277,  3277,  -903,   886,   887,  6150,
    -903,   851,   -22,   898,  -903,   -22,  6150,   110,   318,  -903,
    6602,  6602,  -903,   790,   451,   893,   862,   791,  -903,  -903,
     908,  -903,   791,  9269,  6150,  -903,  6150,   900,   909,  -903,
    -903,  6150,  -903,  6150,  -903,   541,  -903,  -903,  -903,  -903,
     268,  6150,  -903,  -903,   194,  6489,   377,  -903,   904,   646,
    -903,  -903, 11016,  -903,  -903,  -903,   899,   907,   910,   913,
    -903,  -903,  -903,  -903,   268,   120,   911,  -903,    78,   915,
    -903,  -903,   551,  4902,   921,   923,   586,  -903,  -903,   918,
     935,   113,   939,   938,  -903,   943,   946,   165,   614,   329,
    -903,   944,  -903,  -903,  3277,   592,  6150,  3655,  1012,  -903,
    -903,  -903,  6150,  -903,  6150,   825,   104,   942,  -903,  -903,
     937,   945,  6602,  6602,  -903,  -903,   896,  -903,   901,  -903,
     614,   959,   614,   954,  6715,   268,   195,   277,  -903,  -903,
     955,   956,  5015,  -903,  -903,  -903,  6150,  -903,  6489,    35,
    -903,  -903,  -903,  -903,   957,   950,    26,    26,  6150,  -903,
    -903,  6150,  6150,  -903,  9181,  -903,  -903,   113,  6150,  -903,
    -903,  -903,  6150,  -903,  6150,  -903,  6150,  -903,   964,   953,
    6150,  3277,   965,  -903,  -903,  -903,  6150,  -903,  -903,   960,
     963,  -903,  -903,  2120,  -903,  5241,   313,  -903,  -903,  -903,
    -903,  -903,  -903,  -903,  -903,  -903,  -903,  6489,   972,  -903,
     692,  -903,   276,  -903,   598,  -903,  -903,  -903,   980,   331,
     332,   978,  -903,  3277,   976,  -903,  5924,  -903,  -903,  -903,
     661,  1141,  -903,   279,  6150,  6150,  -903,  6489,   140,   977,
      26,  -903,  -903,  -903,  -903,  6150,  -903,  6150,  -903,   614,
     987,  -903,  -903,  6037,  -903,   985,   986,  -903,  6150,  -903,
     994,  -903,   995,  3277,  1084,   599,  6150,  6150,   600,   992,
    3277,  -903,  1086,  -903,  -903,  -903,  -903,  -903
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -903,  -903,  1043,   -29,    17,  -903,  -903,   841,  -903,    64,
    -903,  -903,   -85,  -101,   300,   622,  -903,   290,  -903,  -903,
    -902,  -903,   100,  -903,    21,   717,  -903,  -903,    25,  -903,
     -37,  1011,    88,   -45,  -903,  -903,  1302,  1002,  1406,   -46,
    -903,  1489,  1510,  -903,  1697,  -903,   -94,  -903,   336,  -903,
     523,  -903,  -903,   -84,  -254,   436,   -88,  1010,  -903,   -93,
     962,  -903,    -4,  -553,  -340,  -548,  -331,   392,    -2,   -27,
    -151,    70,  -903,  -903,  -903,  -903,  -903,   315,    46,  -516,
     519,  -663,  -903,  -903,  -903,  -903,   335,  -743,  -573,  -903,
     137,   197,  -833,   105,  -903,  -903,   742,  -903,  -903,  -903,
     743,  -250,  -903,  -903,  -903,   175,   178,   495,  -903,  -903,
    -478,  -903,   683,  1089,  -903,  -903,   -60,  -903,  -783,   116,
    -503,  -903,  -903,  -903,  -903,   135,  -903,    19,  -903,   108,
    -589,  -439,   462,  -903,  -122,   117,   160,   141,   210,   673,
     678,   676,   680,   681,   -59,   -62,   188,   395,  -903,   -32,
    -903,  -347,     0,  -903,  -903,  -903,  -903,   653,   671,  -903,
    -299,  -903,  -903,   932,  -903,  -903,   725,  -903,   193,  -903,
    -903,  -903,  -903,  -903,  -903,   -15,    71,  -903,  -903,  -903,
    -903,  -149,  -316,  -903,  -515,  -491,  -903,   432,  -903,   -28,
    -903,  -903,  -903,  -903,  -903,   296,  -903,  -903,   298,  -903,
    -903,  -903,  -903,   -58,  -327,  -903,   610,  -903,  -903,  -903,
     835,  -903,  -253,  -903,  -903,  -903,  -903,  -903,  -903,  -903,
    -903,  -903,   916,  -903,  -903,   414
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -621
static const short int yytable[] =
{
     132,   352,   399,   553,   310,   661,   344,   163,   517,   518,
     770,   294,   363,   714,   544,   545,   699,   565,   566,   130,
     301,    94,   730,   372,   888,   373,   369,   375,   388,   157,
     368,   601,   162,   298,   606,   745,   132,   167,   699,   526,
     602,   522,   937,   607,   624,   533,   282,   132,   297,   297,
     297,   807,   130,   164,   789,   130,   351,    94,   788,   362,
     330,   746,   858,   987,    89,   280,   130,   396,    94,  -208,
     312,   138,   547,   293,    10,    10,   427,   428,   429,   475,
     335,   382,   130,   130,   869,   132,   182,   394,    10,   -30,
     393,    10,   450,   552,  1052,   309,    42,   297,   367,    10,
      89,   146,   744,   158,   130,   125,    94,   462,   512,  -208,
    -205,    89,  -208,   456,   130,    42,    10,   395,   138,   299,
    -205,   482,   148,  -205,   914,   881,   297,   480,   457,   390,
     463,   427,    10,   341,   341,   403,   882,    10,   483,   548,
      10,   165,    42,   523,   370,   910,   157,   632,   297,    89,
     150,   737,   125,    42,  -208,    10,   138,  -208,   371,    42,
     548,  -208,   998,  1019,    79,   548,    10,   150,   132,   130,
     409,    42,   130,    42,   987,   861,   692,   394,  1100,   860,
     393,    28,    29,    79,   483,   355,   132,    42,    52,   883,
     125,   524,    57,    34,   392,  -205,    52,   727,  -205,   525,
     483,    42,  -205,   890,   693,   995,   728,   490,   496,   889,
      79,   986,   996,    52,   406,   807,   807,    42,   633,   632,
     158,    79,    42,   163,  1098,   807,   829,    79,    42,   871,
     807,   807,   807,   237,   149,   768,   858,    52,    10,    79,
     559,    79,   282,   560,   497,   157,   807,   791,   162,   132,
     338,   870,   282,   132,   294,    79,   483,  1012,   294,   730,
     351,   280,   792,   301,  1013,   585,   586,   301,   280,    79,
     164,   280,   280,   718,    94,   282,   298,   581,   396,   799,
     298,   919,   982,   482,   394,    79,   614,   393,   458,   132,
      79,   297,   554,   531,   280,   297,    79,   983,  1042,   459,
     483,    10,   297,   396,  1102,   372,   375,   482,   130,   628,
      94,   963,   130,   627,   569,   962,   293,    89,   643,   158,
     293,   132,   984,   885,   482,   797,   168,  1092,   834,   668,
     669,   670,   798,   153,   774,   482,   615,   370,   404,   382,
     130,   959,    94,   405,   163,    28,    29,   163,   560,   571,
     294,   374,   460,    89,   461,   536,   169,    34,   509,   301,
     138,  1079,   299,   528,   922,   482,   299,   488,  1080,   537,
     598,   533,   298,   315,   316,   482,   611,   613,   282,   494,
    1043,   297,  1094,   313,   297,    89,   875,   297,   622,   623,
     355,   863,   138,   495,   125,   588,   482,   280,   482,   130,
     864,   826,   370,   520,   629,  1074,  1075,   827,   636,  1046,
     132,   314,   293,   132,   153,  1049,   731,   482,   346,   482,
     482,  1014,   297,   377,   297,   132,   125,   399,  1015,   389,
    1083,  1084,   884,   132,   349,   132,    10,   132,   704,   228,
     350,   707,   708,   368,  1020,   451,   355,   355,   732,   452,
     528,   918,   482,   454,   453,   455,   637,   706,   299,   638,
     345,   848,   659,   394,   662,     5,   393,   890,   646,   699,
     648,   649,   985,   959,  1076,   986,   354,    28,    29,   654,
     363,   655,   360,   656,   132,   482,   294,    33,   361,    34,
     842,   370,   282,   719,   132,   301,   733,  -169,   873,   157,
     396,   396,   874,   132,  1097,   376,   759,   282,   298,   748,
     163,   280,   282,   280,   391,   697,   132,   934,   808,   698,
      25,   282,   280,   297,   387,   557,   280,   161,   400,   631,
     690,   280,   482,    52,   451,   280,   598,   697,   452,   401,
     280,   698,   297,   453,   642,   402,   736,   482,   293,   153,
    -120,   966,   408,  -120,   801,   297,   968,   802,   415,   605,
     421,   588,   605,   407,   528,   451,   367,   815,    41,   452,
     816,   671,   672,   158,   453,   163,   822,   403,   420,   482,
     163,   418,   422,   130,   294,   419,   528,   464,   824,   397,
     297,   488,   846,   301,   299,   816,  1073,   130,   423,   675,
     676,   677,   678,   639,   352,   424,   298,   130,   557,   645,
     -26,   647,    66,    67,    68,   847,   673,   674,   482,   652,
     653,   297,   940,   355,   833,   488,   978,   393,   658,   488,
     394,   430,   237,   393,   341,   465,  1000,   666,   852,   488,
     411,   500,   412,   414,   418,   431,   293,   466,   528,   918,
     918,   -26,   -26,   513,  1105,   154,   513,   513,   528,   426,
     836,   -26,   432,   -26,   355,   355,   434,   436,   438,   688,
     689,  1003,   679,   680,  1004,   -26,   691,  1018,   355,    10,
     482,   627,   581,  1081,  1112,  1115,   482,   488,   482,   439,
     440,   368,   299,   282,   485,   -26,   132,   153,   467,  1022,
     399,   163,   482,  1078,   163,   355,   488,   -26,   317,   318,
     319,   483,   280,   943,   489,   498,   491,   130,   492,   501,
      28,    29,   855,   502,   510,   605,   511,   515,   516,   527,
     773,   394,    34,   918,   393,   154,   154,   493,   532,   543,
     542,   538,   546,   564,   537,   154,   555,   755,   399,   582,
     575,   154,   182,   877,   556,   394,   583,   346,   393,   625,
     626,   912,   630,   -26,   571,   732,   608,   640,   528,   898,
     519,   772,   942,   598,   154,   393,    52,   695,   641,   665,
      52,   903,   297,   703,   713,   935,   722,   394,   734,   715,
     393,   611,   441,   716,   394,   721,   735,   393,   588,   442,
     443,   737,   636,   742,   753,   418,   297,   588,   418,   386,
     297,   931,   355,   297,   743,   747,   527,   953,   754,   560,
     418,  -192,   760,   761,   958,   762,   720,   748,   418,   980,
     418,   723,   418,   444,   657,   763,   725,   163,   725,   445,
     764,   664,   766,   282,   282,   282,   780,   446,   781,   599,
     782,   800,   925,   237,  -178,   787,   929,   790,   793,   933,
    -194,   794,   280,   280,   280,   803,   806,   355,   804,   417,
     130,   504,   852,   297,   805,   687,   809,   831,   825,   418,
     810,   811,   812,   355,    10,   813,   814,   817,   818,   777,
     979,   702,   394,   705,   819,   393,   705,   705,   418,  -192,
    -192,   820,   821,  -192,   838,   627,   711,   823,  -192,   830,
    -192,   835,   841,  -192,   994,   843,   844,   845,   130,   605,
     849,   528,  1023,   898,   394,    28,    29,   393,   447,   729,
     527,   856,   417,  -192,   859,   524,   862,    34,   866,   867,
     872,   880,   548,   954,   282,   913,   920,   132,   876,   160,
     237,   751,   527,   928,  -192,   757,   855,   772,   923,   923,
     923,   923,   944,   280,   394,   923,   280,   393,   697,   528,
     923,   605,   698,   605,   946,   877,   605,   605,   947,   951,
     952,    52,  1048,   154,    10,   154,   956,   694,   154,   154,
     355,   237,   964,   967,  1067,   974,   988,   975,   990,   997,
     932,  -192,   519,  1005,   297,   965,   991,   712,   999,   992,
     154,   936,   993,  1001,   765,  1002,   717,   528,   941,   154,
    1006,   282,   851,  1009,   775,    28,    29,  1008,  1010,   160,
     160,  1011,  1016,  1021,  1026,   524,  1027,    34,  1063,   160,
     280,  1031,   322,  1051,  1028,   160,  1032,  -620,   605,   605,
     925,  1033,  1070,  1034,  -401,  1044,  1050,  1062,  1066,  1068,
     302,   302,  1069,   282,  1077,  1082,  1085,   386,   160,  1087,
    1099,    10,  1103,   899,   900,   899,   901,  1106,  1107,  1109,
    1110,    52,   280,   772,  -542,  1116,  -543,   323,  -620,  -620,
     289,   418,   921,   417,   508,   705,   417,  1053,  -620,   357,
    -620,   618,   887,   282,   767,   353,   911,   776,   417,   513,
     282,   772,  -620,   358,   359,   398,   417,   905,   417,  1047,
     417,   324,   280,   865,   325,   326,   619,   327,  1007,   280,
     621,  1025,  -620,  1024,   527,   275,  1064,   681,   328,  1057,
     329,  1071,   683,   682,  -620,   644,   154,   684,   154,   154,
     605,   605,   955,   154,     5,   957,   685,   725,   504,   302,
     154,   857,  1036,   710,   487,   505,   701,   417,  1041,   969,
     772,   868,   758,  -150,   973,   906,   772,   907,   886,   302,
     908,   909,   570,     0,   441,     0,   417,     0,   837,  1055,
    1056,   442,   443,     0,     0,     0,  1058,   302,     0,     0,
    -620,     0,     0,   154,  1061,     0,     0,   154,   154,    25,
       0,   154,     0,     0,  -150,  -150,   161,     0,     0,     0,
     938,   939,     0,   237,  -150,  1093,  -150,     0,     0,     0,
       0,   445,     0,   945,     0,   772,     0,     0,     0,   446,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   960,   961,     0,     0,   851,    41,  -150,     0,
     302,     0,  1095,  1096,     0,   772,     0,     0,     0,   970,
    -150,   972,     0,  1101,     0,     0,   976,   160,   977,   160,
       0,   237,   160,   160,     0,     0,   419,   765,     0,   899,
       0,     0,     0,     0,  1113,  1114,     0,     0,     0,     0,
       0,    66,    67,    68,   160,     0,     0,     0,     0,     0,
       0,     0,     0,   160,   535,     0,     0,     0,   504,     0,
     447,     0,     0,     0,     0,     0,  -150,     0,     0,     0,
       0,     0,     0,     0,     0,   989,     0,     0,   948,   949,
     950,   938,     0,     0,   154,     0,     0,   513,     0,     0,
       0,   303,   303,     0,  1029,  1030,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   576,   577,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   417,
       0,   590,     0,   527,     0,     0,     0,   154,   154,     0,
       0,     0,     0,  1054,   617,   504,   154,   154,   154,   356,
       0,     0,     0,     0,     0,     0,     0,  1059,     0,  1060,
       0,     0,     0,     0,     0,   705,     0,     0,     0,     0,
       0,   302,     0,     0,   302,     0,     0,     0,   383,     0,
       0,     0,     0,   302,     0,   302,   302,     0,     0,  1017,
     160,     0,   160,   160,   302,     0,   302,   160,   302,     0,
     303,     0,   505,     0,   160,   304,   304,     0,     0,     0,
       0,  1089,     0,     0,     0,     0,     0,     0,     0,     0,
     303,     0,     0,     0,   154,     0,   468,   154,   469,   154,
       0,     0,   938,     0,     0,     0,     0,     0,   303,     0,
       0,   470,     0,  1108,     0,   302,     0,   160,     0,     0,
       0,   160,   160,     0,     0,   160,     0,     0,     0,     0,
       0,     0,     0,   471,   302,     0,  1065,   472,   473,     0,
       0,   474,   475,     0,     0,     0,     0,     0,   476,     0,
       0,     0,   304,     0,     0,   477,     0,     0,   306,   306,
       0,     0,     0,     0,   478,   724,     0,   590,     0,     0,
       0,   303,     0,   302,   304,     0,     0,     0,  1086,   307,
     307,     0,     0,     0,     0,     0,   302,     0,   479,     0,
     480,     0,     0,     0,   304,     0,     0,     0,   154,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   304,     0,     0,     0,     0,     0,  1111,     0,
       0,     0,   779,     0,   534,  1117,     0,     0,     0,     0,
       0,   154,   505,     0,     0,   306,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   795,     0,     0,
       0,     0,     0,     0,     0,     0,   307,   306,   160,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   304,     0,   306,   307,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   591,     0,     0,   306,     0,     0,   307,     0,
       0,   160,   160,   616,     0,     0,   383,     0,     0,   505,
     160,   160,   160,     0,     0,     0,   307,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   302,   304,     0,
       0,     0,   303,     0,     0,   303,     0,     0,     0,     0,
       0,     0,     0,     0,   303,     0,   303,   303,     0,     0,
       0,     0,     0,     0,     0,   303,     0,   303,   306,   303,
       0,     0,     0,     0,     0,     0,   308,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   307,
       0,     0,     0,     0,     0,     0,     0,     0,   160,     0,
       0,   160,     0,   160,     0,     0,   304,     0,     0,     0,
       0,     0,     0,     0,   590,     0,   303,   304,     0,   902,
     304,   306,     0,   590,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   303,     0,     0,     0,     0,
       0,     0,   307,     0,     0,     0,   304,   302,     0,   304,
       0,   302,     0,   308,   302,     0,     0,     0,   304,     0,
     304,   304,     0,     0,     0,     0,     0,     0,   591,   304,
       0,   304,     0,   304,   303,   308,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   303,     0,   306,
       0,     0,     0,     0,     0,   308,     0,     0,     0,     0,
     306,     0,   160,   306,     0,     0,     0,     0,     0,     0,
     307,     0,     0,   308,   302,     0,     0,     0,     0,     0,
     304,   307,   778,     0,   307,     0,     0,     0,     0,   306,
       0,     0,   306,     0,     0,   160,     0,     0,     0,   304,
       0,   306,     0,   306,   306,     0,     0,     0,     0,     0,
     307,     0,   306,   307,   306,     0,   306,     0,     0,     0,
       0,     0,   307,     0,   307,   307,     0,     0,     0,     0,
       0,     0,   304,   307,     0,   307,   308,   307,   304,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   304,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   306,     0,   307,     0,   304,     0,   303,   308,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   307,     0,   302,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   306,     0,     0,     0,     0,
       0,   306,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   306,     0,   307,     0,     0,     0,
       0,     0,   307,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   307,     0,   308,     0,     0,
       0,     0,     0,     0,     0,   591,     0,     0,   308,   306,
       0,   308,     0,     0,   591,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     307,     0,   304,     0,     0,     0,     0,   308,   303,     0,
     308,     0,   303,     0,     0,   303,     0,     0,     0,   308,
       0,   308,   308,     0,     0,     0,     0,     0,     0,   171,
     308,     0,   308,   172,   308,     0,   173,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   174,   175,     0,     0,
       0,   176,   177,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   179,     0,     0,
       0,     0,     0,     0,     0,   303,     0,     0,     0,   304,
     182,   308,     0,     0,     0,   306,     0,     0,   304,     0,
       0,     0,     0,     0,     0,     0,     0,   183,     0,     0,
     308,     0,     0,     0,   410,     0,   307,     0,     0,     0,
       0,     0,   304,     0,     0,     0,   304,     0,     0,   304,
       0,   191,     0,     0,     0,   193,   194,   195,   196,     0,
       0,     0,     0,   308,     0,     0,     0,     0,     0,   308,
       0,     0,     0,     0,     0,     0,     0,     0,   201,     0,
      52,     0,   308,     0,     0,   202,   203,     0,     0,     0,
       0,     0,   306,     0,     0,     0,     0,     0,     0,     0,
       0,   306,     0,     0,     0,     0,     0,     0,     0,   304,
       0,     0,     0,   307,     0,     0,     0,   308,     0,     0,
       0,     0,   307,     0,     0,   306,     0,     0,     0,   306,
       0,     0,   306,     0,     0,     0,   303,     0,     0,     0,
       0,   207,   208,     0,     0,     0,   307,     0,     0,     0,
     307,     0,     0,   307,   170,     0,     0,     0,     0,   171,
       0,     0,     0,   172,     5,     0,   173,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   174,   175,     0,     0,
       0,   176,   177,    10,     0,     0,     0,     0,     0,     0,
       0,     0,   306,   178,     0,     0,     0,   179,     0,     0,
     180,     0,     0,     0,     0,     0,     0,     0,   181,   599,
     182,     0,     0,   307,     0,     0,     0,     0,     0,    25,
       0,     0,     0,   308,     0,     0,   161,   183,     0,     0,
       0,   184,     0,   185,   186,     0,   600,   188,     0,   189,
     304,     0,     0,     0,     0,     0,     0,     0,   190,     0,
       0,   191,     0,     0,   192,   193,   194,   195,   196,   197,
     198,   199,   200,     0,     0,     0,     0,    41,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   201,     0,
      52,     0,     0,     0,     0,   202,   203,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     308,     0,     0,     0,     0,     0,     0,     0,     0,   308,
       0,    66,    67,    68,     0,     0,   204,   205,     0,     0,
     206,     0,     0,   306,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   308,     0,     0,     0,   308,     0,     0,
     308,   207,   208,     0,   307,     0,     0,     0,     0,     0,
       0,   -17,     0,     0,     0,   -17,     0,   -17,   -17,   -17,
     -17,   -17,   -17,   -17,   -17,   -17,   -17,   -17,   -17,     0,
     -17,   -17,   -17,   -17,     0,   -17,   -17,   -17,   -17,   -17,
     -17,     0,   -17,   -17,   -17,   -17,   -17,   -17,   -17,   -17,
       0,     0,     0,   -17,   -17,     0,     0,     0,   -17,     0,
     308,   -17,     0,   -17,   -17,     0,     0,   -17,   -17,   -17,
     -17,   -17,   -17,     0,   -17,     0,   -17,   -17,   -17,   -17,
     -17,   -17,     0,   -17,   -17,   -17,   -17,   -17,   -17,   -17,
       0,     0,   -17,     0,   -17,   -17,     0,   -17,   -17,     0,
     -17,     0,     0,     0,   -17,     0,     0,     0,     0,   -17,
       0,     0,   -17,     0,   -17,   -17,   -17,   -17,   -17,   -17,
     -17,   -17,   -17,   -17,   -17,   -17,   -17,   -17,   -17,   -17,
     -17,     0,     0,     0,     0,     0,     0,     0,     0,   -17,
     -17,   -17,   -17,   -17,   -17,   -17,   -17,   -17,   -17,   -17,
     -17,   -17,     0,     0,     0,   -17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   -17,   -17,   -17,     0,   -17,   -17,   -17,   -17,
     -17,   -17,     0,   -17,   -17,   -17,   -17,   -17,   -17,   -17,
       0,   308,   -17,   -17,   -17,   -17,   -17,   -17,   -17,   -17,
     -17,   -17,   -17,   -17,   170,     0,     1,     2,     3,   171,
     238,   239,     4,   172,     5,   240,   173,   241,     0,   242,
       6,     0,     7,     0,     8,     9,   174,   175,   243,   244,
       0,   176,   177,    10,   245,    11,    12,    13,    14,     0,
       0,     0,    15,   178,     0,     0,     0,   179,     0,     0,
     180,     0,    16,   246,     0,     0,    17,    18,   181,    19,
     182,    20,     0,   247,     0,    21,    22,    23,    24,    25,
     248,     0,    26,    27,    28,    29,    30,   183,    31,     0,
       0,   184,     0,   185,   249,     0,   250,   188,     0,   189,
       0,   420,     0,   251,     0,     0,     0,     0,   190,     0,
       0,   252,     0,   506,   192,   193,   194,   195,   196,   197,
     253,   199,   200,    37,    38,    39,    40,    41,    42,    43,
       0,     0,     0,     0,     0,     0,     0,     0,   201,   254,
      52,    53,    54,    55,    56,   202,   203,   255,   256,    57,
      58,     0,     0,     0,   257,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,     0,   258,   204,   205,   259,   260,
     206,     0,   261,   262,    69,    70,    71,    72,    73,     0,
       0,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,   207,   208,   170,     0,     1,     2,     3,   171,   238,
     239,     4,   172,     5,   240,   173,   241,     0,   242,     6,
       0,     7,     0,     8,     9,   174,   175,   243,   244,     0,
     176,   177,    10,   245,    11,    12,    13,    14,     0,     0,
       0,    15,   178,     0,     0,     0,   179,     0,     0,   180,
       0,    16,   246,     0,     0,    17,    18,   181,    19,   182,
      20,     0,   247,     0,    21,    22,    23,    24,    25,   248,
       0,    26,    27,    28,    29,    30,   183,    31,     0,     0,
     184,     0,   185,   249,     0,   250,   188,     0,   189,     0,
       0,     0,   251,     0,     0,     0,     0,   190,     0,     0,
     252,     0,   506,   192,   193,   194,   195,   196,   197,   253,
     199,   200,    37,    38,    39,    40,    41,    42,    43,     0,
       0,     0,     0,     0,     0,     0,     0,   201,   254,    52,
      53,    54,    55,    56,   202,   203,   255,   256,    57,    58,
       0,     0,     0,   257,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,     0,   258,   204,   205,   259,   260,   206,
       0,   261,   262,    69,    70,    71,    72,    73,     0,     0,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
     207,   208,   170,     0,     1,     2,     3,   171,   238,   239,
       4,   172,     5,   240,   173,   241,     0,   242,     6,     0,
       7,     0,     8,     9,   174,   175,   243,   244,     0,   176,
     177,    10,   245,    11,    12,    13,    14,     0,     0,     0,
      15,   178,     0,     0,     0,   179,     0,     0,   180,     0,
      16,   246,     0,     0,    17,    18,   181,    19,   182,    20,
       0,   247,     0,    21,    22,    23,    24,    25,   248,     0,
      26,    27,    28,    29,    30,   183,    31,     0,     0,   184,
       0,   185,   249,     0,   250,   188,     0,   189,     0,     0,
       0,   251,     0,     0,     0,     0,   190,     0,     0,   252,
       0,   709,   192,   193,   194,   195,   196,   197,   253,   199,
     200,    37,    38,    39,    40,    41,    42,    43,     0,     0,
       0,     0,     0,     0,     0,     0,   201,   254,    52,    53,
      54,    55,    56,   202,   203,   255,   256,    57,    58,     0,
       0,     0,   257,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    66,
      67,    68,     0,   258,   204,   205,   259,   260,   206,     0,
     261,   262,    69,    70,    71,    72,    73,     0,     0,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,   207,
     208,   170,     0,     1,     2,     3,   171,   238,   239,     4,
     172,     5,   240,   173,   241,     0,   242,     6,     0,     7,
       0,     8,     9,   174,   175,   243,   244,     0,   176,   177,
      10,   245,    11,    12,    13,    14,     0,     0,     0,    15,
     178,     0,     0,     0,   179,     0,     0,   180,     0,    16,
     246,     0,     0,    17,    18,   181,    19,   182,    20,     0,
     247,     0,    21,    22,    23,    24,    25,   248,     0,    26,
      27,    28,    29,    30,   183,    31,     0,     0,   184,     0,
     185,   249,     0,   250,   188,     0,   189,     0,     0,     0,
     251,     0,     0,     0,     0,   190,     0,     0,   252,     0,
       0,   192,   193,   194,   195,   196,   197,   253,   199,   200,
      37,    38,    39,    40,    41,    42,    43,     0,     0,     0,
       0,     0,     0,     0,     0,   201,   254,    52,    53,    54,
      55,    56,   202,   203,   255,   256,    57,    58,     0,     0,
       0,   257,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,     0,   258,   204,   205,   259,   260,   206,     0,   261,
     262,    69,    70,    71,    72,    73,     0,     0,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,   207,   208,
     170,     0,     1,     2,     3,   171,     0,     0,     4,   172,
       5,     0,   173,     0,     0,     0,     6,     0,     7,     0,
       8,     9,   174,   175,     0,     0,     0,   176,   177,    10,
       0,    11,    12,    13,    14,     0,     0,     0,    15,   178,
       0,     0,     0,   179,     0,     0,   180,     0,    16,     0,
       0,     0,    17,    18,   181,    19,   182,    20,     0,     0,
       0,    21,    22,    23,    24,    25,     0,     0,    26,    27,
      28,    29,    30,   183,    31,    32,     0,   184,     0,   185,
     249,     0,   250,   188,     0,   189,     0,     0,     0,    35,
       0,     0,     0,     0,   190,     0,     0,   191,     0,     0,
     192,   193,   194,   195,   196,   197,   253,   199,   200,    37,
      38,    39,    40,    41,    42,    43,     0,     0,     0,     0,
       0,     0,     0,     0,   201,     0,    52,    53,    54,    55,
      56,   202,   203,     0,     0,    57,    58,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
       0,     0,   204,   205,     0,     0,   206,     0,     0,     0,
      69,    70,    71,    72,    73,     0,     0,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,   207,   208,   170,
       0,     0,     2,     3,   171,     0,     0,     4,   172,     5,
       0,   173,     0,     0,     0,     6,     0,     7,     0,     8,
       9,   174,   175,     0,     0,     0,   176,   177,    10,     0,
      11,    12,    13,    14,     0,     0,     0,    15,   178,     0,
       0,     0,   179,     0,     0,   180,     0,    16,     0,     0,
       0,    17,    18,   181,    19,   182,    20,     0,     0,     0,
      21,    22,    23,    24,    25,     0,     0,     0,     0,    28,
      29,    30,   183,    31,     0,     0,   184,     0,   185,   696,
       0,   250,   188,     0,   189,     0,     0,     0,   251,     0,
       0,     0,     0,   190,     0,     0,   191,     0,     0,   192,
     193,   194,   195,   196,   197,   198,   199,   200,     0,    38,
      39,    40,    41,    42,    43,     0,     0,     0,     0,     0,
       0,     0,     0,   201,     0,    52,    53,    54,    55,    56,
     202,   203,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,     0,
       0,   204,   205,     0,     0,   206,     0,     0,     0,    69,
      70,    71,    72,    73,     0,     0,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,   207,   208,   170,     0,
       0,     2,     3,   171,     0,     0,     4,   172,     5,     0,
     173,     0,     0,     0,     6,     0,     7,     0,     8,     9,
     174,   175,     0,     0,     0,   176,   177,    10,     0,    11,
      12,    13,    14,     0,     0,     0,    15,   178,     0,     0,
       0,   179,     0,     0,   180,     0,    16,     0,     0,     0,
      17,    18,   181,    19,   182,    20,     0,     0,     0,    21,
      22,    23,    24,    25,     0,     0,     0,     0,     0,     0,
      30,   183,    31,     0,     0,   184,     0,   185,   186,     0,
     187,   188,     0,   189,     0,     0,     0,     0,     0,     0,
       0,     0,   190,     0,     0,   191,     0,     0,   192,   193,
     194,   195,   196,   197,   198,   199,   200,     0,    38,    39,
      40,    41,    42,    43,     0,     0,     0,     0,     0,     0,
       0,     0,   201,     0,    52,    53,    54,    55,    56,   202,
     203,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    66,    67,    68,     0,     0,
     204,   205,     0,     0,   206,     0,     0,     0,    69,    70,
      71,    72,    73,     0,     0,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,   207,   208,   170,     0,     0,
       2,     3,   171,     0,     0,     4,   172,     5,     0,   173,
       0,     0,     0,     6,     0,     7,     0,     8,     9,   174,
     175,     0,     0,     0,   176,   177,    10,     0,    11,    12,
      13,    14,     0,     0,     0,    15,   178,     0,     0,     0,
     179,     0,     0,   180,     0,    16,     0,     0,     0,    17,
      18,   181,    19,   182,    20,     0,     0,     0,    21,    22,
      23,    24,    25,     0,     0,     0,     0,     0,     0,    30,
     183,    31,     0,     0,   184,     0,   185,   186,     0,   187,
     188,     0,   189,     0,     0,     0,     0,     0,     0,     0,
       0,   190,     0,     0,   252,     0,     0,   192,   193,   194,
     195,   196,   197,   198,   199,   200,     0,    38,    39,    40,
      41,    42,    43,     0,     0,     0,     0,     0,     0,     0,
       0,   201,     0,    52,    53,    54,    55,    56,   202,   203,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,     0,     0,   204,
     205,     0,     0,   206,     0,     0,     0,    69,    70,    71,
      72,    73,     0,     0,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,   207,   208,   170,     0,     0,     2,
       3,   171,     0,     0,     4,   172,     5,     0,   173,     0,
       0,     0,     6,     0,     7,     0,     8,     9,   174,   175,
       0,     0,     0,   176,   177,    10,     0,    11,    12,    13,
      14,     0,     0,     0,    15,   178,     0,     0,     0,   179,
       0,     0,   180,     0,    16,     0,     0,     0,    17,    18,
     181,    19,   182,    20,     0,     0,     0,    21,    22,    23,
      24,    25,     0,     0,     0,     0,     0,     0,    30,   183,
      31,     0,     0,   184,     0,   185,   503,     0,   187,   188,
       0,   189,     0,     0,     0,     0,     0,     0,     0,     0,
     190,     0,     0,   252,     0,     0,   192,   193,   194,   195,
     196,   197,   198,   199,   200,     0,    38,    39,    40,    41,
      42,    43,     0,     0,     0,     0,     0,     0,     0,     0,
     201,     0,    52,    53,    54,    55,    56,   202,   203,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    66,    67,    68,     0,     0,   204,   205,
       0,     0,   206,     0,     0,     0,    69,    70,    71,    72,
      73,     0,     0,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    83,   207,   208,   170,     0,     0,     2,     3,
     171,     0,     0,     4,   172,     5,     0,   173,     0,     0,
       0,     6,     0,     7,     0,     8,     9,   174,   175,     0,
       0,     0,   176,   177,    10,     0,    11,    12,    13,    14,
       0,     0,     0,    15,   178,     0,     0,     0,   179,     0,
       0,   180,     0,    16,     0,     0,     0,    17,    18,   181,
      19,   182,    20,     0,     0,     0,    21,    22,    23,    24,
      25,     0,     0,     0,     0,     0,     0,    30,   183,    31,
       0,     0,   184,     0,   185,   503,     0,   187,   188,     0,
     189,     0,     0,     0,     0,     0,     0,     0,     0,   190,
       0,     0,   252,     0,     0,   192,   193,   194,   195,   196,
     197,   198,   199,   200,     0,    38,    39,    40,    41,    42,
      43,     0,     0,     0,     0,     0,     0,     0,     0,   201,
       0,    52,    53,    54,    55,    56,   202,   203,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,     0,     0,   204,   205,     0,
       0,   206,     0,     0,     0,    69,    70,    71,    72,    73,
       0,     0,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,   207,   208,   170,     0,     0,     2,     3,   171,
       0,     0,     4,   172,     5,     0,   173,     0,     0,     0,
       6,     0,     7,     0,     8,     9,   174,   175,     0,     0,
       0,   176,   177,    10,     0,    11,    12,    13,    14,     0,
       0,     0,    15,   178,     0,     0,     0,   179,     0,     0,
     180,     0,    16,     0,     0,     0,    17,    18,   181,    19,
     182,    20,     0,     0,     0,    21,    22,    23,    24,    25,
       0,     0,     0,     0,     0,     0,    30,   183,    31,     0,
       0,   184,     0,   185,   186,     0,   187,   188,     0,   189,
       0,     0,     0,     0,     0,     0,     0,     0,   190,     0,
       0,   252,     0,     0,   192,   193,   194,   195,   196,   197,
     198,   199,   200,     0,    38,    39,    40,    41,    42,    43,
       0,     0,     0,     0,     0,     0,     0,     0,   201,     0,
      52,    53,    54,    55,    56,   202,   203,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    66,    67,    68,     0,     0,   204,   205,     0,     0,
     206,     0,     0,     0,    69,    70,    71,    72,    73,     0,
       0,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,   207,   208,   170,     0,     0,     0,     0,   171,     0,
       0,     0,   172,     0,     0,   173,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   174,   175,     0,     0,     0,
     176,   177,    10,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   178,     0,     0,     0,   179,     0,     0,   180,
       0,     0,     0,     0,     0,     0,     0,   181,     0,   182,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   183,     0,     0,     0,
     184,     0,   185,   186,     0,   187,   188,     0,   189,   890,
     420,     0,     0,     0,     0,     0,     0,   891,     0,     0,
     769,     0,   892,   192,   193,   194,   195,   196,   197,   198,
     199,   200,     0,     0,     0,     0,   170,     0,     0,     0,
       0,   171,     0,     0,     0,   172,     0,   201,   173,    52,
       0,     0,     0,     0,   202,   203,     0,     0,   174,   175,
       0,     0,     0,   176,   177,    10,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   178,     0,     0,     0,   179,
       0,     0,   180,     0,     0,   204,   205,     0,     0,   206,
     181,     0,   182,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   183,
     207,   208,     0,   184,     0,   185,   186,     0,   187,   188,
       0,   189,   890,     0,     0,     0,     0,     0,     0,     0,
     891,     0,     0,   769,     0,   892,   192,   193,   194,   195,
     196,   197,   198,   199,   200,     0,     0,     0,     0,   170,
       0,     0,     0,     0,   171,     0,     0,     0,   172,     0,
     201,   173,    52,     0,     0,     0,     0,   202,   203,     0,
       0,   174,   175,     0,     0,     0,   176,   177,    10,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   178,     0,
       0,     0,   179,     0,     0,   180,     0,     0,   204,   205,
       0,     0,   206,   181,     0,   182,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   183,   207,   208,     0,   184,     0,   185,   186,
       0,   187,   188,     0,   189,   890,     0,     0,     0,     0,
       0,     0,     0,   891,     0,     0,   769,     0,  1045,   192,
     193,   194,   195,   196,   197,   198,   199,   200,     0,     0,
       0,     0,   170,     0,     0,     0,     0,   171,     0,     0,
       0,   172,     0,   201,   173,    52,     0,     0,     0,     0,
     202,   203,     0,     0,   174,   175,     0,     0,     0,   176,
     177,    10,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   178,     0,     0,     0,   179,     0,     0,   180,     0,
       0,   204,   205,     0,     0,   206,   181,     0,   182,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   183,   207,   208,     0,   184,
       0,   185,   186,     0,   187,   188,     0,   189,     0,     0,
       0,   499,     0,     0,     0,     0,   190,     0,     0,   191,
       0,     0,   192,   193,   194,   195,   196,   197,   198,   199,
     200,     0,     0,     0,     0,   170,     0,     0,     0,     0,
     171,     0,     0,     0,   172,     0,   201,   173,    52,     0,
       0,     0,     0,   202,   203,     0,     0,   174,   175,     0,
       0,     0,   176,   177,    10,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   178,     0,     0,     0,   179,     0,
       0,   180,     0,     0,   204,   205,     0,     0,   206,   181,
       0,   182,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   183,   207,
     208,     0,   184,     0,   185,   186,     0,   187,   188,     0,
     189,     0,   420,     0,     0,     0,     0,     0,     0,   190,
       0,     0,   191,     0,     0,   192,   193,   194,   195,   196,
     197,   198,   199,   200,     0,     0,     0,     0,   170,     0,
       0,     0,     0,   171,     0,     0,     0,   172,     0,   201,
     173,    52,     0,     0,     0,     0,   202,   203,     0,     0,
     174,   175,     0,     0,     0,   176,   177,    10,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   178,     0,     0,
       0,   179,     0,     0,   180,     0,     0,   204,   205,     0,
       0,   206,   181,     0,   182,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   183,   207,   208,     0,   184,     0,   185,   186,     0,
     567,   188,     0,   189,     0,     0,     0,     0,     0,     0,
       0,     0,   190,   568,     0,   191,     0,     0,   192,   193,
     194,   195,   196,   197,   198,   199,   200,     0,     0,     0,
       0,   170,     0,     0,     0,     0,   171,     0,     0,     0,
     172,     0,   201,   173,    52,     0,     0,     0,     0,   202,
     203,     0,     0,   174,   175,     0,     0,     0,   176,   177,
      10,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     178,     0,     0,     0,   179,     0,     0,   180,     0,     0,
     204,   205,     0,     0,   206,   181,     0,   182,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   183,   207,   208,     0,   184,     0,
     185,   186,   660,   187,   188,     0,   189,     0,     0,     0,
       0,     0,     0,     0,     0,   190,     0,     0,   191,     0,
       0,   192,   193,   194,   195,   196,   197,   198,   199,   200,
       0,     0,     0,     0,   170,     0,     0,     0,     0,   171,
       0,     0,     0,   172,     0,   201,   173,    52,     0,     0,
       0,     0,   202,   203,     0,     0,   174,   175,     0,     0,
       0,   176,   177,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   178,     0,     0,     0,   179,     0,     0,
     180,     0,     0,   204,   205,     0,     0,   206,   181,     0,
     182,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   183,   207,   208,
       0,   184,     0,   185,   186,     0,   187,   188,     0,   189,
       0,     0,   686,     0,     0,     0,     0,     0,   190,     0,
       0,   191,     0,     0,   192,   193,   194,   195,   196,   197,
     198,   199,   200,     0,     0,     0,     0,   170,     0,     0,
       0,     0,   171,     0,     0,     0,   172,     0,   201,   173,
      52,     0,     0,     0,     0,   202,   203,     0,     0,   174,
     175,     0,     0,     0,   176,   177,    10,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   178,     0,     0,     0,
     179,     0,     0,   180,     0,     0,   204,   205,     0,     0,
     206,   181,     0,   182,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     183,   207,   208,     0,   184,     0,   185,   186,     0,   187,
     188,     0,   189,     0,     0,     0,     0,     0,     0,     0,
       0,   190,     0,     0,   191,     0,   756,   192,   193,   194,
     195,   196,   197,   198,   199,   200,     0,     0,     0,     0,
     170,     0,     0,     0,     0,   171,     0,     0,     0,   172,
       0,   201,   173,    52,     0,     0,     0,     0,   202,   203,
       0,     0,   174,   175,     0,     0,     0,   176,   177,    10,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   178,
       0,     0,     0,   179,     0,     0,   180,     0,     0,   204,
     205,     0,     0,   206,   181,     0,   182,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   183,   207,   208,     0,   184,     0,   185,
     186,   828,   187,   188,     0,   189,     0,     0,     0,     0,
       0,     0,     0,     0,   190,     0,     0,   191,     0,     0,
     192,   193,   194,   195,   196,   197,   198,   199,   200,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   170,     0,
       0,     0,     0,   171,   201,     0,    52,   172,     0,  1088,
     173,   202,   203,     0,     0,     0,     0,     0,     0,     0,
     174,   175,     0,     0,     0,   176,   177,    10,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   178,     0,     0,
       0,   179,   204,   205,   180,     0,   206,     0,     0,     0,
       0,     0,   181,     0,   182,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   207,   208,     0,
       0,   183,     0,     0,     0,   184,     0,   185,   186,     0,
     187,   188,     0,   189,     0,     0,     0,     0,     0,     0,
       0,     0,   190,     0,     0,   191,     0,     0,   192,   193,
     194,   195,   196,   197,   198,   199,   200,     0,     0,     0,
       0,   170,     0,     0,     0,     0,   171,     0,     0,     0,
     172,     0,   201,   173,    52,     0,     0,     0,     0,   202,
     203,     0,     0,   174,   175,     0,     0,     0,   176,   177,
      10,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     178,     0,     0,     0,   179,     0,     0,   180,     0,     0,
     204,   205,     0,     0,   206,   181,     0,   182,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   183,   207,   208,     0,   184,     0,
     185,   186,  1104,   187,   188,     0,   189,     0,     0,     0,
       0,     0,     0,     0,     0,   190,     0,     0,   191,     0,
       0,   192,   193,   194,   195,   196,   197,   198,   199,   200,
       0,     0,     0,     0,   170,     0,     0,     0,     0,   171,
       0,     0,     0,   172,     0,   201,   173,    52,     0,     0,
       0,     0,   202,   203,     0,     0,   174,   175,     0,     0,
       0,   176,   177,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   178,     0,     0,     0,   179,     0,     0,
     180,     0,     0,   204,   205,     0,     0,   206,   181,     0,
     182,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   183,   207,   208,
       0,   184,     0,   185,   186,     0,   187,   188,     0,   189,
       0,     0,     0,     0,     0,     0,     0,     0,   190,     0,
       0,   191,     0,     0,   192,   193,   194,   195,   196,   197,
     198,   199,   200,     0,     0,     0,     0,   170,     0,     0,
       0,     0,   171,     0,     0,     0,   172,     0,   201,   173,
      52,     0,     0,     0,     0,   202,   203,     0,     0,   174,
     175,     0,     0,     0,   176,   177,    10,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   178,     0,     0,     0,
     179,     0,     0,   180,     0,     0,   204,   205,     0,     0,
     206,   181,     0,   182,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     183,   207,   208,     0,   184,     0,   185,   186,     0,   187,
     188,     0,   189,     0,     0,     0,     0,     0,     0,     0,
       0,   190,     0,     0,   337,     0,     0,   192,   193,   194,
     195,   196,   197,   198,   199,   200,     0,     0,     0,     0,
     170,     0,     0,     0,     0,   171,     0,     0,     0,   172,
       0,   201,   173,    52,     0,     0,     0,     0,   202,   203,
       0,     0,   174,   175,     0,     0,     0,   176,   177,    10,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   178,
       0,     0,     0,   179,     0,     0,   180,     0,     0,   204,
     205,     0,     0,   206,   181,     0,   182,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   183,   207,   208,     0,   184,     0,   185,
     484,     0,   187,   188,     0,   189,     0,     0,     0,     0,
       0,     0,     0,     0,   190,     0,     0,   191,     0,     0,
     192,   193,   194,   195,   196,   197,   198,   199,   200,     0,
       0,     0,     0,   170,     0,     0,     0,     0,   171,     0,
       0,     0,   172,     0,   201,   173,    52,     0,     0,     0,
       0,   202,   203,     0,     0,   174,   175,     0,     0,     0,
     176,   177,    10,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   178,     0,     0,     0,   179,     0,     0,   180,
       0,     0,   204,   205,     0,     0,   206,   181,     0,   182,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   183,   207,   208,     0,
     184,     0,   185,   186,     0,   187,   188,     0,   189,     0,
       0,     0,     0,     0,     0,     0,     0,   190,     0,     0,
     769,     0,     0,   192,   193,   194,   195,   196,   197,   198,
     199,   200,     0,     0,     0,     0,   170,     0,     0,     0,
       0,   171,     0,     0,     0,   172,     0,   201,   173,    52,
       0,     0,     0,     0,   202,   203,     0,     0,   174,   175,
       0,     0,     0,   176,   177,    10,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   178,     0,     0,     0,   179,
       0,     0,   180,     0,     0,   204,   205,     0,     0,   206,
     181,     0,   182,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   183,
     207,   208,     0,   184,     0,   185,   186,     0,   600,   188,
       0,   189,     0,     0,     0,     0,     0,     0,     0,     0,
     190,     0,     0,   191,     0,     0,   192,   193,   194,   195,
     196,   197,   198,   199,   200,     0,     0,     0,     0,   170,
       0,     0,     0,     0,   171,     0,     0,     0,   172,     0,
     201,   173,    52,     0,     0,     0,     0,   202,   203,     0,
       0,   174,   175,     0,     0,     0,   176,   177,    10,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   178,     0,
       0,     0,   179,     0,     0,   180,     0,     0,   204,   205,
       0,     0,   206,   181,     0,   182,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   183,   207,   208,     0,   184,     0,   185,   186,
       0,   187,   188,     0,   189,     0,     0,     0,     0,     0,
       0,     0,     0,   190,     0,     0,  1035,     0,     0,   192,
     193,   194,   195,   196,   197,   198,   199,   200,     0,     0,
       0,     0,   170,     0,     0,     0,     0,   171,     0,     0,
       0,   172,     0,   201,   173,    52,     0,     0,     0,     0,
     202,   203,     0,     0,   174,   175,     0,     0,     0,   176,
     177,    10,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   178,     0,     0,     0,   179,     0,     0,   180,     0,
       0,   204,   205,     0,     0,   206,   181,     0,   182,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   183,   207,   208,     0,   184,
       0,   185,   410,     0,   187,   188,     0,   189,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   191,
       0,     0,   192,   193,   194,   195,   196,   197,   198,   199,
     200,     0,     0,     0,     0,   170,     0,     0,     0,     0,
     171,     0,     0,     0,   172,     0,   201,   173,    52,     0,
       0,     0,     0,   202,   203,     0,     0,   174,   175,     0,
       0,     0,   176,   177,    10,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   178,     0,     0,     0,   179,     0,
       0,   180,     0,     0,   204,   205,     0,     0,   206,   181,
       0,   182,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   183,   207,
     208,     0,   184,     0,   185,   413,     0,   187,   188,     0,
     189,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   191,     0,     0,   192,   193,   194,   195,   196,
     197,   198,   199,   200,     0,     0,     0,     0,   170,     0,
       0,     0,     0,   171,     0,     0,     0,   172,     0,   201,
     173,    52,     0,     0,     0,     0,   202,   203,     0,     0,
     174,   175,     0,     0,     0,   176,   177,    10,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   178,     0,     0,
       0,   179,     0,     0,   180,     0,     0,   204,   205,     0,
       0,   206,   181,     0,   182,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   183,   207,   208,     0,   184,     0,   185,   425,     0,
     187,   188,     0,   189,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   191,     0,     0,   192,   193,
     194,   195,   196,   197,   198,   199,   200,     0,     0,     0,
       0,   170,     0,     0,     0,     0,   171,     0,     0,     0,
     172,     0,   201,   173,    52,     0,     0,     0,     0,   202,
     203,     0,     0,   174,   175,     0,     0,     0,   176,   177,
      10,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     178,     0,     0,     0,   179,     0,     0,   180,     0,     0,
     204,   205,     0,     0,   206,   181,     0,   182,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   183,   207,   208,     0,   184,     0,
     185,   433,     0,   187,   188,     0,   189,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   191,     0,
       0,   192,   193,   194,   195,   196,   197,   198,   199,   200,
       0,     0,     0,     0,   170,     0,     0,     0,     0,   171,
       0,     0,     0,   172,     0,   201,   173,    52,     0,     0,
       0,     0,   202,   203,     0,     0,   174,   175,     0,     0,
       0,   176,   177,    10,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   178,     0,     0,     0,   179,     0,     0,
     180,     0,     0,   204,   205,     0,     0,   206,   181,     0,
     182,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   183,   207,   208,
       0,   184,     0,   185,   435,     0,   187,   188,     0,   189,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   191,     0,     0,   192,   193,   194,   195,   196,   197,
     198,   199,   200,     0,     0,     0,     0,   170,     0,     0,
       0,     0,   171,     0,     0,     0,   172,     0,   201,   173,
      52,     0,     0,     0,     0,   202,   203,     0,     0,   174,
     175,     0,     0,     0,   176,   177,    10,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   178,     0,     0,     0,
     179,     0,     0,   180,     0,     0,   204,   205,     0,     0,
     206,   181,     0,   182,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     183,   207,   208,     0,   184,     0,   185,   437,     0,   187,
     188,     0,   189,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   191,     0,     0,   192,   193,   194,
     195,   196,   197,   198,   199,   200,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   201,     0,    52,     0,     0,     0,     0,   202,   203,
       0,     0,     0,     0,     0,     0,     0,     1,     2,     3,
       0,     0,     0,     4,     0,     5,     0,     0,     0,     0,
       0,     6,     0,     7,     0,     8,     9,     0,     0,   204,
     205,     0,     0,   206,    10,     0,    11,    12,    13,    14,
       0,     0,     0,    15,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    16,   207,   208,     0,    17,    18,     0,
      19,     0,    20,     0,     0,     0,    21,    22,    23,    24,
      25,     0,     0,    26,    27,    28,    29,    30,     0,    31,
      32,     0,     0,     0,     0,    33,     0,    34,     0,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,     0,     0,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,     0,
       0,    52,    53,    54,    55,    56,     0,     0,     0,     0,
      57,    58,     0,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,    60,    61,    62,     0,    63,    64,
      65,     0,    66,    67,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
       0,     0,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,     1,     2,     3,     0,     0,     0,     4,     0,
       5,     0,     0,     0,     0,     0,     6,     0,     7,     0,
       8,     9,     0,     0,     0,     0,     0,     0,     0,    10,
       0,    11,    12,    13,    14,     0,     0,     0,    15,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    16,     0,
       0,     0,    17,    18,     0,    19,     0,    20,     0,     0,
       0,    21,    22,    23,    24,    25,     0,     0,    26,    27,
      28,    29,    30,     0,    31,    32,     0,     0,     0,     0,
      33,     0,    34,     0,     0,     0,     0,     0,     0,    35,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    36,     0,     0,    37,
      38,    39,    40,    41,    42,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,    53,    54,    55,
      56,     0,     0,     0,     0,    57,    58,     0,     0,     0,
      59,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    66,    67,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,    70,    71,    72,    73,     0,     0,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,     1,     2,     3,
       0,     0,     0,     4,     0,     5,     0,     0,     0,     0,
       0,     6,     0,     7,     0,     8,     9,     0,     0,     0,
       0,     0,     0,     0,    10,     0,    11,    12,    13,    14,
       0,     0,     0,    15,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    16,     0,     0,     0,    17,    18,     0,
      19,     0,    20,     0,     0,     0,    21,    22,    23,    24,
      25,     0,     0,    26,    27,    28,    29,    30,     0,    31,
      32,     0,     0,     0,     0,    33,     0,    34,     0,     0,
       0,     0,     0,     0,    35,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,     0,     0,    37,    38,    39,    40,    41,    42,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    52,    53,    54,    55,    56,     0,     0,     0,     0,
      57,    58,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    66,    67,    68,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
       0,     0,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,     2,     3,     0,     0,     0,     4,     0,     5,
       0,     0,     0,     0,     0,     6,     0,     7,     0,     8,
       9,     0,     0,     0,     0,     0,     0,     0,    10,     0,
      11,    12,    13,    14,     0,     0,     0,    15,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    16,     0,     0,
       0,    17,    18,     0,    19,     0,    20,     0,     0,     0,
      21,    22,    23,    24,    25,     0,     0,     0,     0,     0,
       0,    30,     0,    31,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   290,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   620,     0,
       0,     0,     0,     0,     0,   291,     0,     0,     0,    38,
      39,    40,    41,    42,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
       0,     0,     0,     0,    57,    58,     0,     0,     0,   292,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,    73,     0,     0,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,     2,     3,     0,     0,
       0,     4,     0,     5,     0,     0,     0,     0,     0,     6,
       0,     7,     0,     8,     9,     0,     0,     0,     0,     0,
       0,     0,    10,     0,    11,    12,    13,    14,     0,     0,
       0,    15,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    16,     0,     0,     0,    17,    18,     0,    19,     0,
      20,     0,     0,     0,    21,    22,    23,    24,    25,     0,
       0,     0,     0,     0,     0,    30,     0,    31,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   290,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   796,     0,     0,     0,     0,     0,     0,   291,
       0,     0,     0,    38,    39,    40,    41,    42,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,    56,     0,     0,     0,     0,    57,    58,
       0,     0,     0,   292,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,    73,     0,     0,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
       2,     3,     0,     0,     0,     4,     0,     5,     0,     0,
       0,     0,     0,     6,     0,     7,     0,     8,     9,     0,
       0,     0,     0,     0,     0,     0,    10,     0,    11,    12,
      13,    14,     0,     0,     0,    15,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    16,     0,     0,     0,    17,
      18,     0,    19,     0,    20,     0,     0,     0,    21,    22,
      23,    24,    25,     0,     0,     0,     0,     0,     0,    30,
       0,    31,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   290,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   291,     0,     0,     0,    38,    39,    40,
      41,    42,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,    54,    55,    56,     0,     0,
       0,     0,    57,    58,     0,     0,     0,   292,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,     0,     0,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,     2,     3,     0,     0,     0,     4,
       0,     5,     0,     0,     0,     0,     0,     6,     0,     7,
       0,     8,     9,     0,     0,     0,     0,     0,     0,     0,
      10,     0,    11,    12,    13,    14,     0,     0,     0,    15,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    16,
       0,     0,     0,    17,    18,     0,    19,     0,    20,     0,
       0,     0,    21,    22,    23,    24,    25,     0,     0,     0,
       0,     0,     0,    30,     0,    31,     0,     0,     0,     0,
       0,     0,   -73,     0,     0,   -73,     0,     0,     0,     0,
       0,     0,     2,     3,     0,     0,     0,     4,     0,     5,
       0,     0,     0,     0,     0,     6,     0,     7,     0,     8,
       9,    38,    39,    40,    41,    42,    43,     0,    10,     0,
      11,    12,    13,    14,     0,     0,     0,    15,    53,    54,
      55,    56,     0,     0,     0,     0,     0,    16,     0,     0,
       0,    17,    18,     0,    19,     0,    20,     0,     0,     0,
      21,    22,    23,    24,    25,     0,     0,     0,    66,    67,
      68,    30,     0,    31,     0,     0,     0,     0,     0,     0,
     587,    69,    70,    71,    72,    73,     0,     0,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    38,
      39,    40,    41,    42,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,    73,     0,     0,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,     2,     3,     0,     0,
       0,     4,     0,     5,     0,     0,     0,     0,     0,     6,
       0,     7,     0,     8,     9,     0,     0,     0,     0,     0,
       0,     0,    10,     0,    11,    12,    13,    14,     0,     0,
       0,    15,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    16,     0,     0,     0,    17,    18,     0,    19,     0,
      20,     0,     0,     0,    21,    22,    23,    24,    25,     0,
       0,     0,     0,     0,     0,    30,     0,    31,     0,     0,
       0,     0,     0,     0,   587,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     2,     3,     0,     0,     0,     4,
       0,     5,     0,     0,     0,     0,     0,     6,     0,     7,
       0,     8,     9,    38,    39,    40,    41,    42,    43,     0,
      10,     0,    11,    12,    13,    14,     0,     0,     0,    15,
      53,    54,    55,    56,     0,     0,     0,     0,     0,    16,
       0,     0,     0,    17,    18,     0,    19,     0,    20,     0,
       0,   904,    21,    22,    23,    24,    25,     0,     0,     0,
      66,    67,    68,    30,     0,    31,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,    73,     0,     0,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    38,    39,    40,    41,    42,    43,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    53,    54,
      55,    56,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    66,    67,
      68,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,     0,     0,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,     2,     3,
       0,     0,     0,     4,     0,     5,     0,     0,   924,     0,
       0,     6,     0,     7,     0,     8,     9,     0,     0,     0,
       0,     0,     0,     0,    10,     0,    11,    12,    13,    14,
       0,     0,     0,    15,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    16,     0,     0,     0,    17,    18,     0,
      19,     0,    20,     0,     0,     0,    21,    22,    23,    24,
      25,     0,     0,     0,     0,     0,     0,    30,     0,    31,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     2,     3,     0,     0,
       0,     4,     0,     5,     0,     0,     0,     0,     0,     6,
       0,     7,     0,     8,     9,    38,    39,    40,    41,    42,
      43,     0,    10,     0,    11,    12,    13,    14,     0,     0,
       0,    15,    53,    54,    55,    56,     0,     0,     0,     0,
       0,    16,     0,     0,     0,    17,    18,     0,    19,     0,
      20,     0,     0,     0,    21,    22,    23,    24,    25,     0,
       0,     0,    66,    67,    68,    30,     0,    31,     0,     0,
       0,     0,     0,     0,     0,    69,    70,    71,    72,    73,
       0,     0,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    38,    39,    40,    41,    42,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      53,    54,    55,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,    73,     0,     0,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
       2,     3,     0,     0,     0,     4,     0,     5,     0,     0,
       0,     0,     0,     6,     0,     7,     0,     8,     9,     0,
       0,     0,     0,     0,     0,     0,    10,     0,    11,    12,
      13,    14,     0,     0,     0,    15,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    16,     0,     0,     0,    17,
      18,     0,    19,     0,    20,     0,     0,     0,    21,    22,
      23,    24,    25,     0,     0,     0,     0,     0,     0,    30,
       0,    31,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    38,    39,    40,
      41,    42,    43,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,    54,    55,    56,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    66,    67,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    69,    70,    71,
      72,    73,     0,     0,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,     2,     3,     0,     0,     0,     4,
       0,     5,     0,     0,     0,     0,     0,     6,     0,     7,
       0,     8,     9,     0,     0,     0,     0,     0,     0,     0,
      10,     0,    11,    12,    13,    14,     0,     0,     0,    15,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    16,
       0,     0,     0,    17,    18,     0,    19,     0,    20,     0,
       0,     0,    21,    22,    23,    24,    25,     0,     0,     0,
       0,     0,     0,    30,     0,    31,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     2,     3,     0,     0,     0,     4,   378,     5,
       0,     0,     0,     0,     0,     6,     0,     7,     0,     8,
       9,    38,    39,    40,    41,     0,    43,     0,    10,     0,
      11,    12,    13,    14,     0,     0,     0,    15,    53,    54,
      55,    56,     0,     0,     0,     0,     0,    16,     0,     0,
       0,    17,    18,     0,    19,     0,    20,     0,     0,     0,
      21,    22,    23,    24,    25,     0,     0,     0,    66,    67,
      68,    30,     0,    31,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,     0,     0,    74,    75,
      76,    77,    78,     0,    80,    81,    82,    83,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    38,
      39,    40,    41,     0,    43,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    53,    54,    55,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    66,    67,    68,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    69,
      70,    71,    72,    73,     0,     0,    74,    75,    76,    77,
      78,     0,    80,    81,    82,    83,  -335,  -335,  -335,  -335,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -335,     0,     0,     0,     0,  -335,     0,     0,     0,
       0,     0,     0,  -335,     0,     0,     0,     0,     0,     0,
       0,     0,  -335,  -335,  -335,     0,  -335,  -335,  -335,  -335,
       0,  -335,  -335,  -335,  -335,     0,     0,  -335,  -335,     0,
       0,     0,     0,     0,     0,  -335,     0,     0,     0,     0,
       0,     0,     0,     0,  -335,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -335,  -335,  -120,  -120,  -120,
    -335,  -335,  -335,  -335,  -335,     0,  -335,  -335,  -335,  -335,
    -335,  -120,     0,  -335,     0,  -335,     0,  -335,  -335,  -335,
    -335,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -120,  -335,     0,     0,     0,     0,  -335,     0,     0,
       0,     0,     0,     0,  -335,     0,     0,     0,     0,     0,
       0,     0,  -335,  -335,  -335,  -335,     0,  -335,  -335,  -335,
    -335,     0,  -335,  -335,  -335,  -335,     0,     0,  -335,  -335,
       0,     0,     0,     0,     0,     0,  -335,     0,     0,     0,
       0,     0,     0,     0,     0,  -335,     0,     0,     0,     0,
       0,     0,  -335,     0,     0,     0,  -335,  -335,  -120,  -120,
    -120,  -335,  -335,  -335,  -335,  -335,   521,  -335,  -335,  -335,
    -335,  -335,  -335,  -103,  -335,     0,  -335,     0,     0,     0,
    -103,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -103,     0,  -120,     0,     0,     0,     0,     0,     0,  -115,
       0,  -103,     0,     0,     0,     0,     0,     0,  -103,     0,
       0,     0,     0,  -335,     0,     0,     0,     0,  -103,     0,
       0,     0,  -103,  -103,     0,  -103,     0,     0,     0,     0,
       0,  -103,     0,  -103,     0,  -103,     0,     0,     0,     0,
    -115,  -115,  -103,     0,  -103,     0,     0,     0,     0,     0,
    -115,     0,  -115,  -335,     0,     0,     0,     0,  -115,     0,
    -120,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -103,  -103,     0,  -103,  -103,  -103,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -115,     0,     0,  -103,
    -103,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  -103,  -103,  -103,
       0,     0,     0,     0,     0,     0,     0,     0,  -104,     0,
    -103,  -103,  -103,  -103,  -103,  -104,     0,  -103,  -103,  -103,
    -103,  -103,  -103,     0,     0,  -104,     0,     0,     0,     0,
       0,     0,     0,     0,  -116,     0,  -104,     0,     0,     0,
       0,     0,     0,  -104,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -104,     0,     0,     0,  -104,  -104,     0,
    -104,     0,     0,     0,     0,     0,  -104,     0,  -104,     0,
    -104,     0,     0,     0,     0,  -116,  -116,  -104,     0,  -104,
       0,     0,     0,     0,     0,  -116,     0,  -116,     0,     0,
       0,     0,     0,  -116,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  -104,  -104,     0,  -104,  -104,
    -104,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -116,     0,     0,  -104,  -104,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -104,  -104,  -104,     0,     0,     0,     0,     0,
       0,     0,     0,  -102,     0,  -104,  -104,  -104,  -104,  -104,
    -102,     0,  -104,  -104,  -104,  -104,  -104,  -104,     0,     0,
    -102,     0,     0,     0,     0,     0,     0,     0,     0,  -114,
       0,  -102,     0,     0,     0,     0,     0,     0,  -102,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  -102,     0,
       0,     0,  -102,  -102,     0,  -102,     0,     0,     0,     0,
       0,  -102,     0,  -102,     0,  -102,     0,     0,     0,     0,
    -114,  -114,  -102,     0,  -102,     0,     0,     0,     0,     0,
    -114,     0,  -114,     0,     0,     0,     0,     0,  -114,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -102,  -102,     0,  -102,  -102,  -102,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -114,     0,     0,  -102,
    -102,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     2,     0,
       0,     0,     0,     0,     0,     5,     0,  -102,  -102,  -102,
       0,     0,     0,     0,     0,     8,     0,     0,     0,     0,
    -102,  -102,  -102,  -102,  -102,     0,    11,  -102,  -102,  -102,
    -102,  -102,  -102,    15,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    16,     0,     0,     0,    17,    18,     0,
      19,     0,     0,     0,     0,     0,    21,     0,    23,     0,
      25,     0,     0,     0,     0,     0,     0,   161,     0,    31,
       0,     0,     0,     0,     0,     0,   -72,     0,     0,   -72,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    38,    39,     0,    41,    42,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     2,     0,     0,     0,     0,     0,     0,
       5,     0,    66,    67,    68,     0,     0,     0,     0,     0,
       8,     0,     0,     0,     0,    69,    70,    71,    72,    73,
       0,    11,    74,    75,    76,    77,    78,    79,    15,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    16,     0,
       0,     0,    17,    18,     0,    19,     0,     0,     0,     0,
       0,    21,     0,    23,     0,    25,     0,     0,     0,     0,
       0,     0,   161,     0,    31,     0,     0,     0,     0,     0,
       0,   -70,     0,     0,   -70,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      38,    39,     0,    41,    42,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     2,     0,
       0,     0,     0,     0,     0,     5,     0,    66,    67,    68,
       0,     0,     0,     0,     0,     8,     0,     0,     0,     0,
      69,    70,    71,    72,    73,     0,    11,    74,    75,    76,
      77,    78,    79,    15,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    16,     0,     0,     0,    17,    18,     0,
      19,     0,     0,     0,     0,     0,    21,     0,    23,     0,
      25,     0,     0,     0,     0,     0,     0,   161,     0,    31,
       0,     0,     0,     0,     0,     0,   -71,     0,     0,   -71,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    38,    39,     0,    41,    42,
      43,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    55,    56,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     2,     0,     0,     0,     0,     0,     0,
       5,     0,    66,    67,    68,     0,     0,     0,     0,     0,
       8,     0,     0,     0,     0,    69,    70,    71,    72,    73,
       0,    11,    74,    75,    76,    77,    78,    79,    15,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    16,     0,
       0,     0,    17,    18,     0,    19,     0,     0,     0,     0,
       0,    21,     0,    23,     0,    25,     0,     0,     0,     0,
       0,     0,   161,     0,    31,     0,     0,     0,     0,     0,
       0,   -69,     0,     0,   -69,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      38,    39,     0,    41,    42,    43,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    55,
      56,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -99,     0,
       0,     0,     0,     0,     0,   -99,     0,    66,    67,    68,
       0,     0,     0,     0,     0,   -99,     0,     0,     0,     0,
      69,    70,    71,    72,    73,     0,   -99,    74,    75,    76,
      77,    78,    79,   -99,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   -99,     0,     0,     0,   -99,   -99,     0,
     -99,     0,     0,     0,     0,     0,   -99,     0,   -99,     0,
     -99,     0,     0,     0,     0,     0,     0,   -99,     0,   -99,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  -101,     0,     2,     0,     0,     0,
       0,     0,     0,     5,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     8,     0,   -99,   -99,     0,   -99,   -99,
     -99,     0,     0,     0,    11,     0,     0,     0,     0,     0,
       0,    15,     0,     0,   -99,   -99,     0,     0,     0,     0,
       0,    16,     0,     0,     0,    17,    18,     0,    19,     0,
       0,     0,     0,     0,    21,     0,    23,     0,    25,     0,
       0,     0,   -99,   -99,   -99,   161,     0,    31,     0,     0,
       0,     0,     0,     0,     0,   -99,   -99,   -99,   -99,   -99,
       0,     0,   -99,   -99,   -99,   -99,   -99,   -99,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    38,    39,     0,    41,    42,    43,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,    56,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      66,    67,    68,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    69,    70,    71,    72,    73,     0,     0,
      74,    75,    76,    77,    78,    79
};

/* YYCONFLP[YYPACT[STATE-NUM]] -- Pointer into YYCONFL of start of
   list of conflicting reductions corresponding to action entry for
   state STATE-NUM in yytable.  0 means no conflicts.  The list in
   yyconfl is terminated by a rule number of 0.  */
static const unsigned short int yyconflp[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   295,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   351,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   641,     3,     0,     0,     0,
       0,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   671,     0,     0,     0,     0,   703,     0,     0,
       0,     0,   675,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   707,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     5,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   643,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     677,     0,     0,     7,     0,     0,     0,     0,     0,     0,
       0,   709,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     9,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   309,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   293,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   659,     0,     0,     0,     0,     0,     0,
     655,     0,     0,   657,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   661,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     353,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   355,   357,     0,     0,     0,     0,     0,     0,     0,
       0,   359,     0,   361,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   363,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   365,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   367,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   347,     0,     0,
       0,     0,     0,   369,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   327,
     329,     0,     0,   331,     0,     0,     0,     0,   333,     0,
     335,     0,     0,   337,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   687,
     689,   691,     0,     0,     0,   693,     0,     0,     0,     0,
     695,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   701,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   371,     0,     0,
       0,     0,     0,     0,   705,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   373,   375,
       0,     0,     0,     0,     0,     0,     0,     0,   377,     0,
     379,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   381,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   383,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   385,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   319,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     387,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   321,     0,   323,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     325,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   389,     0,     0,     0,   391,     0,   393,   395,   397,
     399,   401,   403,   405,   407,   409,   411,   413,   415,     0,
     417,   419,   421,   423,     0,   425,   427,   429,   431,   433,
     435,     0,   437,   439,   441,   443,   445,   447,   449,   451,
       0,     0,     0,   453,   455,     0,     0,     0,   457,     0,
       0,   459,     0,   461,   463,     0,     0,   465,   467,   469,
     471,   473,   475,     0,   477,     0,   479,   481,   483,   485,
     487,   489,     0,   491,   493,   495,   497,   499,   501,   503,
       0,     0,   505,     0,   507,   509,     0,   511,   513,     0,
     515,     0,     0,     0,   517,     0,     0,     0,     0,   519,
       0,     0,   521,     0,   523,   525,   527,   529,   531,   533,
     535,   537,   539,   541,   543,   545,   547,   549,   551,   553,
     555,     0,     0,     0,     0,     0,     0,     0,     0,   557,
     559,   561,   563,   565,   567,   569,   571,   573,   575,   577,
     579,   581,     0,     0,     0,   583,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   585,   587,   589,     0,   591,   593,   595,   597,
     599,   601,     0,   603,   605,   607,   609,   611,   613,   615,
       0,     0,   617,   619,   621,   623,   625,   627,   629,   631,
     633,   635,   637,   639,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   311,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   313,     0,   315,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   317,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   663,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   665,     0,   667,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     669,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     645,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   647,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   649,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   673,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    11,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   349,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   297,   299,   301,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   303,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   305,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   339,   307,
     341,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    83,     0,     0,     0,     0,     0,     0,
      85,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,     0,   343,     0,     0,     0,     0,     0,     0,     0,
       0,    89,     0,     0,     0,     0,     0,     0,    91,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    93,     0,
       0,     0,    95,    97,     0,    99,     0,     0,     0,     0,
       0,   101,     0,   103,     0,   105,     0,     0,     0,     0,
       0,     0,   107,     0,   109,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     111,   113,     0,   115,   117,   119,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   121,
     123,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   125,   127,   129,
       0,     0,     0,     0,     0,     0,     0,     0,   153,     0,
     131,   133,   135,   137,   139,   155,     0,   141,   143,   145,
     147,   149,   151,     0,     0,   157,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   159,     0,     0,     0,
       0,     0,     0,   161,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   163,     0,     0,     0,   165,   167,     0,
     169,     0,     0,     0,     0,     0,   171,     0,   173,     0,
     175,     0,     0,     0,     0,     0,     0,   177,     0,   179,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   181,   183,     0,   185,   187,
     189,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   191,   193,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   195,   197,   199,     0,     0,     0,     0,     0,
       0,     0,     0,   223,     0,   201,   203,   205,   207,   209,
     225,     0,   211,   213,   215,   217,   219,   221,     0,     0,
     227,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   229,     0,     0,     0,     0,     0,     0,   231,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   233,     0,
       0,     0,   235,   237,     0,   239,     0,     0,     0,     0,
       0,   241,     0,   243,     0,   245,     0,     0,     0,     0,
       0,     0,   247,     0,   249,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     251,   253,     0,   255,   257,   259,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   261,
     263,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   265,   267,   269,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     271,   273,   275,   277,   279,     0,     0,   281,   283,   285,
     287,   289,   291,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   651,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   653,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   679,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   681,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   683,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   685,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   697,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    13,     0,
       0,     0,     0,     0,     0,    15,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,   699,    21,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    23,     0,     0,     0,    25,    27,     0,
      29,     0,     0,     0,     0,     0,    31,     0,    33,     0,
      35,     0,     0,     0,     0,     0,     0,    37,     0,    39,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    41,    43,     0,    45,    47,
      49,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    51,    53,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    55,    57,    59,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    61,    63,    65,    67,    69,
       0,     0,    71,    73,    75,    77,    79,    81,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0
};

/* YYCONFL[I] -- lists of conflicting rule numbers, each terminated by
   0, pointed into by YYCONFLP.  */
static const short int yyconfl[] =
{
       0,   185,     0,    30,     0,    30,     0,    30,     0,   576,
       0,    68,     0,   101,     0,   101,     0,   101,     0,   101,
       0,   101,     0,   101,     0,   101,     0,   101,     0,   101,
       0,   101,     0,   101,     0,   101,     0,   101,     0,   101,
       0,   101,     0,   101,     0,   101,     0,   101,     0,   101,
       0,   101,     0,   101,     0,   101,     0,   101,     0,   101,
       0,   101,     0,   101,     0,   101,     0,   101,     0,   101,
       0,   101,     0,   101,     0,   101,     0,   101,     0,   101,
       0,   101,     0,   115,     0,   115,     0,   115,     0,   115,
       0,   115,     0,   115,     0,   115,     0,   115,     0,   115,
       0,   115,     0,   115,     0,   115,     0,   115,     0,   115,
       0,   115,     0,   115,     0,   115,     0,   115,     0,   115,
       0,   115,     0,   115,     0,   115,     0,   115,     0,   115,
       0,   115,     0,   115,     0,   115,     0,   115,     0,   115,
       0,   115,     0,   115,     0,   115,     0,   115,     0,   115,
       0,   115,     0,   116,     0,   116,     0,   116,     0,   116,
       0,   116,     0,   116,     0,   116,     0,   116,     0,   116,
       0,   116,     0,   116,     0,   116,     0,   116,     0,   116,
       0,   116,     0,   116,     0,   116,     0,   116,     0,   116,
       0,   116,     0,   116,     0,   116,     0,   116,     0,   116,
       0,   116,     0,   116,     0,   116,     0,   116,     0,   116,
       0,   116,     0,   116,     0,   116,     0,   116,     0,   116,
       0,   116,     0,   114,     0,   114,     0,   114,     0,   114,
       0,   114,     0,   114,     0,   114,     0,   114,     0,   114,
       0,   114,     0,   114,     0,   114,     0,   114,     0,   114,
       0,   114,     0,   114,     0,   114,     0,   114,     0,   114,
       0,   114,     0,   114,     0,   114,     0,   114,     0,   114,
       0,   114,     0,   114,     0,   114,     0,   114,     0,   114,
       0,   114,     0,   114,     0,   114,     0,   114,     0,   114,
       0,   114,     0,   186,     0,    30,     0,   335,     0,   335,
       0,   335,     0,   335,     0,   335,     0,   335,     0,   374,
       0,    30,     0,    30,     0,    30,     0,    30,     0,   362,
       0,   362,     0,   362,     0,   362,     0,   320,     0,   320,
       0,   320,     0,   320,     0,   320,     0,   320,     0,   335,
       0,   335,     0,   335,     0,   335,     0,   577,     0,    61,
       0,    29,     0,    81,     0,    81,     0,    81,     0,    81,
       0,    81,     0,    81,     0,    81,     0,    81,     0,    81,
       0,   628,     0,   628,     0,   628,     0,   628,     0,   628,
       0,   628,     0,   628,     0,   628,     0,   628,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   246,     0,   246,     0,   246,     0,   246,     0,   246,
       0,   335,     0,   335,     0,    68,     0,    73,     0,    73,
       0,    72,     0,    72,     0,   221,     0,   221,     0,   375,
       0,   376,     0,    30,     0,    30,     0,    30,     0,    30,
       0,    30,     0,    30,     0,   208,     0,   208,     0,    70,
       0,    70,     0,    71,     0,    71,     0,   355,     0,   358,
       0,   571,     0,   573,     0,   569,     0,    69,     0,    69,
       0,   299,     0,    30,     0,   416,     0,   205,     0,   205,
       0
};

static const short int yycheck[] =
{
       0,    95,   153,   319,    50,   444,    64,    34,   261,   262,
     583,    48,   113,   516,   313,   314,   494,   344,   345,     0,
      48,     0,   538,   116,   767,   118,   114,   120,   129,    33,
     114,   371,    34,    48,   374,   550,    36,    37,   516,   293,
     371,   291,   825,   374,   391,   295,    46,    47,    48,    49,
      50,   640,    33,    36,   607,    36,    85,    36,   606,     6,
      60,   552,   725,   896,     0,    46,    47,   152,    47,     0,
      51,     0,    84,    48,    33,    33,   198,   199,   200,    49,
      61,   126,    63,    64,    85,    85,    60,   149,    33,    92,
     149,    33,   214,    84,   996,    49,   118,    97,   113,    33,
      36,    69,    88,    33,    85,     0,    85,    23,   257,    85,
       0,    47,    88,    39,    95,   118,    33,   149,    47,    48,
      85,    88,    84,    88,    98,    92,   126,    97,    54,   129,
      46,   253,    33,    63,    64,   162,   103,    33,    60,   151,
      33,    36,   118,   292,    84,   103,   150,    14,   148,    85,
      84,   152,    47,   118,    85,    33,    85,    88,    98,   118,
     151,    92,    84,   946,   186,   151,    33,    84,   168,   150,
     170,   118,   153,   118,  1007,   728,    64,   239,  1080,   727,
     239,    74,    75,   186,    60,    97,   186,   118,   130,   156,
      85,    84,   139,    86,   148,    85,   130,   537,    88,    92,
      60,   118,    92,    90,    92,    85,   537,   239,    86,    85,
     186,    98,    92,   130,   168,   804,   805,   118,    85,    14,
     150,   186,   118,   250,    84,   814,   665,   186,   118,   744,
     819,   820,   821,    45,    84,   582,   899,   130,    33,   186,
      98,   186,   242,   101,   244,   249,   835,    88,   250,   249,
      62,   742,   252,   253,   291,   186,    60,    92,   295,   775,
     289,   242,   103,   291,    99,   366,   367,   295,   249,   186,
     253,   252,   253,   523,   253,   275,   291,   361,   363,   626,
     295,    85,    88,    88,   346,   186,   380,   346,    30,   289,
     186,   291,   321,   293,   275,   295,   186,   103,   103,    41,
      60,    33,   302,   388,  1087,   398,   399,    88,   289,   397,
     289,   864,   293,   397,   346,   863,   291,   253,    99,   249,
     295,   321,   895,   762,    88,    85,    84,  1070,    92,   451,
     452,   453,    92,    33,   588,    88,   381,    84,    88,   384,
     321,   857,   321,    93,   371,    74,    75,   374,   101,   349,
     387,    98,    94,   289,    96,    84,    84,    86,   253,   387,
     289,    85,   291,   293,   803,    88,   295,    88,    92,    98,
     370,   621,   387,   142,   143,    88,   376,   377,   378,    84,
     103,   381,   103,    84,   384,   321,    99,   387,   388,   389,
     302,   731,   321,    98,   289,   370,    88,   378,    88,   380,
     731,    93,    84,    93,   404,    92,    93,    99,   408,   982,
     410,    84,   387,   413,   114,   988,    98,    88,    98,    88,
      88,    92,   422,   123,   424,   425,   321,   578,    99,   129,
      99,    99,   759,   433,    84,   435,    33,   437,   498,    44,
       0,   501,   502,   527,   947,    82,   358,   359,   541,    86,
     380,   798,    88,    87,    91,    89,   410,    93,   387,   413,
      65,   714,   443,   525,   445,    14,   525,    90,   422,   947,
     424,   425,    95,   989,  1047,    98,    93,    74,    75,   433,
     581,   435,    93,   437,   484,    88,   523,    84,    88,    86,
      93,    84,   492,   525,   494,   523,   542,    84,    88,   503,
     585,   586,    92,   503,  1077,   101,   564,   507,   523,   555,
     537,   492,   512,   494,    84,   494,   516,   816,   640,   494,
      69,   521,   503,   523,   101,   337,   507,    76,    85,    85,
     484,   512,    88,   130,    82,   516,   536,   516,    86,    85,
     521,   516,   542,    91,    85,    85,   546,    88,   523,   249,
      85,   867,    84,    88,    85,   555,   872,    88,    84,   371,
      84,   536,   374,   168,   494,    82,   581,    85,   117,    86,
      88,   454,   455,   503,    91,   602,    85,   604,    91,    88,
     607,   186,    84,   564,   621,   190,   516,    83,    85,   153,
     590,    88,    85,   621,   523,    88,  1035,   578,    84,   458,
     459,   460,   461,   415,   698,    84,   621,   588,   420,   421,
      33,   423,   161,   162,   163,    85,   456,   457,    88,   431,
     432,   621,    85,   535,   686,    88,    85,   686,   440,    88,
     692,    84,   444,   692,   564,   100,    85,   449,   722,    88,
     178,   246,   180,   181,   249,    84,   621,   102,   578,   996,
     997,    74,    75,   258,  1093,    33,   261,   262,   588,   197,
     692,    84,    84,    86,   576,   577,   204,   205,   206,   481,
     482,    85,   462,   463,    88,    98,   488,    85,   590,    33,
      88,   765,   766,    85,    85,    85,    88,    88,    88,    84,
      84,   775,   621,   693,   232,   118,   696,   397,     4,   952,
     851,   728,    88,  1050,   731,   617,    88,   130,   145,   146,
     147,    60,   693,   835,    93,    84,    93,   698,    92,    84,
      74,    75,   722,    84,    84,   537,    84,    93,    84,   293,
      84,   793,    86,  1080,   793,   113,   114,   242,    93,    60,
      88,   305,    84,    88,    98,   123,    92,   559,   899,    84,
      93,   129,    60,   753,    98,   817,    95,    98,   817,    85,
      88,   793,    85,   186,   764,   858,   374,    85,   698,   769,
     275,   583,   834,   773,   152,   834,   130,    70,    85,    84,
     130,   781,   782,    93,    93,   817,    88,   849,    85,    93,
     849,   791,    43,    93,   856,    93,    85,   856,   773,    50,
      51,   152,   802,    84,    88,   410,   806,   782,   413,   126,
     810,   811,   724,   813,   145,   145,   380,   849,   103,   101,
     425,     6,    99,    99,   856,    84,   526,   873,   433,   891,
     435,   531,   437,    84,   439,    85,   536,   864,   538,    90,
      88,   446,    88,   843,   844,   845,    85,    98,    88,    59,
      88,    85,   806,   665,    99,    99,   810,    99,    95,   813,
     101,    93,   843,   844,   845,    84,    88,   779,    85,   186,
     851,   249,   956,   873,    85,   480,    91,   180,    92,   484,
      88,    88,    88,   795,    33,    88,    85,    88,    85,   589,
     890,   496,   954,   498,    85,   954,   501,   502,   503,    84,
      85,    85,    85,    88,    84,   989,   511,    88,    93,    88,
      95,    85,    99,    98,   914,    85,    85,    85,   899,   731,
      95,   851,   954,   923,   986,    74,    75,   986,   179,   537,
     494,    92,   249,   118,    85,    84,    99,    86,    85,    85,
      85,    99,   151,    92,   944,    93,    85,   947,    92,    33,
     762,   556,   516,   103,   139,   560,   956,   769,   101,   101,
     101,   101,    92,   944,  1026,   101,   947,  1026,   947,   899,
     101,   783,   947,   785,    93,   975,   788,   789,    84,    93,
      93,   130,   982,   361,    33,   363,    88,   492,   366,   367,
     902,   803,    99,    85,  1026,    95,    92,    88,    99,    88,
     812,   186,   507,    85,  1004,   143,    99,   512,    93,    99,
     388,   823,    99,    92,   578,    92,   521,   947,   830,   397,
      85,  1021,   722,    85,   588,    74,    75,    88,    85,   113,
     114,    85,    88,    21,    92,    84,    99,    86,    85,   123,
    1021,   145,     4,    93,    99,   129,   145,    33,   860,   861,
    1004,    92,  1033,    99,    99,    99,    99,    93,    93,    99,
      49,    50,    99,  1063,    92,    85,    88,   384,   152,    93,
      93,    33,    85,   773,   774,   775,   776,    92,    92,    85,
      85,   130,  1063,   895,     0,    93,     0,    49,    74,    75,
      47,   696,   802,   410,   253,   700,   413,   997,    84,    97,
      86,   384,   766,  1103,   581,    95,   791,   588,   425,   714,
    1110,   923,    98,   102,   103,   153,   433,   782,   435,   982,
     437,    83,  1103,   731,    86,    87,   384,    89,   931,  1110,
     387,   956,   118,   955,   698,    46,  1020,   464,   100,  1004,
     102,  1033,   466,   465,   130,   420,   524,   467,   526,   527,
     962,   963,   852,   531,    14,   855,   475,   857,   536,   148,
     538,   725,   974,   510,   232,   249,   495,   484,   975,   873,
     982,   739,   562,    33,   876,   783,   988,   785,   764,   168,
     788,   789,   347,    -1,    43,    -1,   503,    -1,   693,  1001,
    1002,    50,    51,    -1,    -1,    -1,  1008,   186,    -1,    -1,
     186,    -1,    -1,   581,  1016,    -1,    -1,   585,   586,    69,
      -1,   589,    -1,    -1,    74,    75,    76,    -1,    -1,    -1,
     825,   826,    -1,  1035,    84,    84,    86,    -1,    -1,    -1,
      -1,    90,    -1,   838,    -1,  1047,    -1,    -1,    -1,    98,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   860,   861,    -1,    -1,   956,   117,   118,    -1,
     249,    -1,  1074,  1075,    -1,  1077,    -1,    -1,    -1,   874,
     130,   876,    -1,  1085,    -1,    -1,   881,   361,   883,   363,
      -1,  1093,   366,   367,    -1,    -1,   891,   851,    -1,   989,
      -1,    -1,    -1,    -1,  1106,  1107,    -1,    -1,    -1,    -1,
      -1,   161,   162,   163,   388,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   397,   303,    -1,    -1,    -1,   696,    -1,
     179,    -1,    -1,    -1,    -1,    -1,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   899,    -1,    -1,   843,   844,
     845,   946,    -1,    -1,   722,    -1,    -1,   952,    -1,    -1,
      -1,    49,    50,    -1,   962,   963,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   356,   357,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   696,
      -1,   370,    -1,   947,    -1,    -1,    -1,   765,   766,    -1,
      -1,    -1,    -1,   998,   383,   773,   774,   775,   776,    97,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1012,    -1,  1014,
      -1,    -1,    -1,    -1,    -1,  1020,    -1,    -1,    -1,    -1,
      -1,   410,    -1,    -1,   413,    -1,    -1,    -1,   126,    -1,
      -1,    -1,    -1,   422,    -1,   424,   425,    -1,    -1,   944,
     524,    -1,   526,   527,   433,    -1,   435,   531,   437,    -1,
     148,    -1,   536,    -1,   538,    49,    50,    -1,    -1,    -1,
      -1,  1066,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     168,    -1,    -1,    -1,   852,    -1,     3,   855,     5,   857,
      -1,    -1,  1087,    -1,    -1,    -1,    -1,    -1,   186,    -1,
      -1,    18,    -1,  1098,    -1,   484,    -1,   581,    -1,    -1,
      -1,   585,   586,    -1,    -1,   589,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    40,   503,    -1,  1021,    44,    45,    -1,
      -1,    48,    49,    -1,    -1,    -1,    -1,    -1,    55,    -1,
      -1,    -1,   126,    -1,    -1,    62,    -1,    -1,    49,    50,
      -1,    -1,    -1,    -1,    71,   534,    -1,   536,    -1,    -1,
      -1,   249,    -1,   542,   148,    -1,    -1,    -1,  1063,    49,
      50,    -1,    -1,    -1,    -1,    -1,   555,    -1,    95,    -1,
      97,    -1,    -1,    -1,   168,    -1,    -1,    -1,   956,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,    -1,    -1,    -1,    -1,    -1,  1103,    -1,
      -1,    -1,   591,    -1,   302,  1110,    -1,    -1,    -1,    -1,
      -1,   989,   696,    -1,    -1,   126,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   616,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   126,   148,   722,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   249,    -1,   168,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   370,    -1,    -1,   186,    -1,    -1,   168,    -1,
      -1,   765,   766,   381,    -1,    -1,   384,    -1,    -1,   773,
     774,   775,   776,    -1,    -1,    -1,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   696,   302,    -1,
      -1,    -1,   410,    -1,    -1,   413,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   422,    -1,   424,   425,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   433,    -1,   435,   249,   437,
      -1,    -1,    -1,    -1,    -1,    -1,    49,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   249,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   852,    -1,
      -1,   855,    -1,   857,    -1,    -1,   370,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   773,    -1,   484,   381,    -1,   778,
     384,   302,    -1,   782,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   503,    -1,    -1,    -1,    -1,
      -1,    -1,   302,    -1,    -1,    -1,   410,   806,    -1,   413,
      -1,   810,    -1,   126,   813,    -1,    -1,    -1,   422,    -1,
     424,   425,    -1,    -1,    -1,    -1,    -1,    -1,   536,   433,
      -1,   435,    -1,   437,   542,   148,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   555,    -1,   370,
      -1,    -1,    -1,    -1,    -1,   168,    -1,    -1,    -1,    -1,
     381,    -1,   956,   384,    -1,    -1,    -1,    -1,    -1,    -1,
     370,    -1,    -1,   186,   873,    -1,    -1,    -1,    -1,    -1,
     484,   381,   590,    -1,   384,    -1,    -1,    -1,    -1,   410,
      -1,    -1,   413,    -1,    -1,   989,    -1,    -1,    -1,   503,
      -1,   422,    -1,   424,   425,    -1,    -1,    -1,    -1,    -1,
     410,    -1,   433,   413,   435,    -1,   437,    -1,    -1,    -1,
      -1,    -1,   422,    -1,   424,   425,    -1,    -1,    -1,    -1,
      -1,    -1,   536,   433,    -1,   435,   249,   437,   542,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   555,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   484,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   503,    -1,   484,    -1,   590,    -1,   696,   302,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   503,    -1,  1004,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   536,    -1,    -1,    -1,    -1,
      -1,   542,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   555,    -1,   536,    -1,    -1,    -1,
      -1,    -1,   542,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   555,    -1,   370,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   773,    -1,    -1,   381,   590,
      -1,   384,    -1,    -1,   782,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     590,    -1,   696,    -1,    -1,    -1,    -1,   410,   806,    -1,
     413,    -1,   810,    -1,    -1,   813,    -1,    -1,    -1,   422,
      -1,   424,   425,    -1,    -1,    -1,    -1,    -1,    -1,     9,
     433,    -1,   435,    13,   437,    -1,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    -1,    -1,
      -1,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   873,    -1,    -1,    -1,   773,
      60,   484,    -1,    -1,    -1,   696,    -1,    -1,   782,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,
     503,    -1,    -1,    -1,    84,    -1,   696,    -1,    -1,    -1,
      -1,    -1,   806,    -1,    -1,    -1,   810,    -1,    -1,   813,
      -1,   101,    -1,    -1,    -1,   105,   106,   107,   108,    -1,
      -1,    -1,    -1,   536,    -1,    -1,    -1,    -1,    -1,   542,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,
     130,    -1,   555,    -1,    -1,   135,   136,    -1,    -1,    -1,
      -1,    -1,   773,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   782,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   873,
      -1,    -1,    -1,   773,    -1,    -1,    -1,   590,    -1,    -1,
      -1,    -1,   782,    -1,    -1,   806,    -1,    -1,    -1,   810,
      -1,    -1,   813,    -1,    -1,    -1,  1004,    -1,    -1,    -1,
      -1,   191,   192,    -1,    -1,    -1,   806,    -1,    -1,    -1,
     810,    -1,    -1,   813,     4,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    -1,    13,    14,    -1,    16,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    26,    27,    -1,    -1,
      -1,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   873,    43,    -1,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    59,
      60,    -1,    -1,   873,    -1,    -1,    -1,    -1,    -1,    69,
      -1,    -1,    -1,   696,    -1,    -1,    76,    77,    -1,    -1,
      -1,    81,    -1,    83,    84,    -1,    86,    87,    -1,    89,
    1004,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,   101,    -1,    -1,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    -1,    -1,    -1,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,
     130,    -1,    -1,    -1,    -1,   135,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     773,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   782,
      -1,   161,   162,   163,    -1,    -1,   166,   167,    -1,    -1,
     170,    -1,    -1,  1004,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   806,    -1,    -1,    -1,   810,    -1,    -1,
     813,   191,   192,    -1,  1004,    -1,    -1,    -1,    -1,    -1,
      -1,     0,    -1,    -1,    -1,     4,    -1,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    -1,
      19,    20,    21,    22,    -1,    24,    25,    26,    27,    28,
      29,    -1,    31,    32,    33,    34,    35,    36,    37,    38,
      -1,    -1,    -1,    42,    43,    -1,    -1,    -1,    47,    -1,
     873,    50,    -1,    52,    53,    -1,    -1,    56,    57,    58,
      59,    60,    61,    -1,    63,    -1,    65,    66,    67,    68,
      69,    70,    -1,    72,    73,    74,    75,    76,    77,    78,
      -1,    -1,    81,    -1,    83,    84,    -1,    86,    87,    -1,
      89,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    98,
      -1,    -1,   101,    -1,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,   162,   163,    -1,   165,   166,   167,   168,
     169,   170,    -1,   172,   173,   174,   175,   176,   177,   178,
      -1,  1004,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,     4,    -1,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    -1,    19,
      20,    -1,    22,    -1,    24,    25,    26,    27,    28,    29,
      -1,    31,    32,    33,    34,    35,    36,    37,    38,    -1,
      -1,    -1,    42,    43,    -1,    -1,    -1,    47,    -1,    -1,
      50,    -1,    52,    53,    -1,    -1,    56,    57,    58,    59,
      60,    61,    -1,    63,    -1,    65,    66,    67,    68,    69,
      70,    -1,    72,    73,    74,    75,    76,    77,    78,    -1,
      -1,    81,    -1,    83,    84,    -1,    86,    87,    -1,    89,
      -1,    91,    -1,    93,    -1,    -1,    -1,    -1,    98,    -1,
      -1,   101,    -1,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,   162,   163,    -1,   165,   166,   167,   168,   169,
     170,    -1,   172,   173,   174,   175,   176,   177,   178,    -1,
      -1,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     4,    -1,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    -1,    19,    20,
      -1,    22,    -1,    24,    25,    26,    27,    28,    29,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    -1,    -1,
      -1,    42,    43,    -1,    -1,    -1,    47,    -1,    -1,    50,
      -1,    52,    53,    -1,    -1,    56,    57,    58,    59,    60,
      61,    -1,    63,    -1,    65,    66,    67,    68,    69,    70,
      -1,    72,    73,    74,    75,    76,    77,    78,    -1,    -1,
      81,    -1,    83,    84,    -1,    86,    87,    -1,    89,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    98,    -1,    -1,
     101,    -1,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
      -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,   162,   163,    -1,   165,   166,   167,   168,   169,   170,
      -1,   172,   173,   174,   175,   176,   177,   178,    -1,    -1,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,     4,    -1,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    -1,    19,    20,    -1,
      22,    -1,    24,    25,    26,    27,    28,    29,    -1,    31,
      32,    33,    34,    35,    36,    37,    38,    -1,    -1,    -1,
      42,    43,    -1,    -1,    -1,    47,    -1,    -1,    50,    -1,
      52,    53,    -1,    -1,    56,    57,    58,    59,    60,    61,
      -1,    63,    -1,    65,    66,    67,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    78,    -1,    -1,    81,
      -1,    83,    84,    -1,    86,    87,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    98,    -1,    -1,   101,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,    -1,
      -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,
     162,   163,    -1,   165,   166,   167,   168,   169,   170,    -1,
     172,   173,   174,   175,   176,   177,   178,    -1,    -1,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     192,     4,    -1,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    -1,    19,    20,    -1,    22,
      -1,    24,    25,    26,    27,    28,    29,    -1,    31,    32,
      33,    34,    35,    36,    37,    38,    -1,    -1,    -1,    42,
      43,    -1,    -1,    -1,    47,    -1,    -1,    50,    -1,    52,
      53,    -1,    -1,    56,    57,    58,    59,    60,    61,    -1,
      63,    -1,    65,    66,    67,    68,    69,    70,    -1,    72,
      73,    74,    75,    76,    77,    78,    -1,    -1,    81,    -1,
      83,    84,    -1,    86,    87,    -1,    89,    -1,    -1,    -1,
      93,    -1,    -1,    -1,    -1,    98,    -1,    -1,   101,    -1,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,    -1,    -1,
      -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,   162,
     163,    -1,   165,   166,   167,   168,   169,   170,    -1,   172,
     173,   174,   175,   176,   177,   178,    -1,    -1,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
       4,    -1,     6,     7,     8,     9,    -1,    -1,    12,    13,
      14,    -1,    16,    -1,    -1,    -1,    20,    -1,    22,    -1,
      24,    25,    26,    27,    -1,    -1,    -1,    31,    32,    33,
      -1,    35,    36,    37,    38,    -1,    -1,    -1,    42,    43,
      -1,    -1,    -1,    47,    -1,    -1,    50,    -1,    52,    -1,
      -1,    -1,    56,    57,    58,    59,    60,    61,    -1,    -1,
      -1,    65,    66,    67,    68,    69,    -1,    -1,    72,    73,
      74,    75,    76,    77,    78,    79,    -1,    81,    -1,    83,
      84,    -1,    86,    87,    -1,    89,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   128,    -1,   130,   131,   132,   133,
     134,   135,   136,    -1,    -1,   139,   140,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,   162,   163,
      -1,    -1,   166,   167,    -1,    -1,   170,    -1,    -1,    -1,
     174,   175,   176,   177,   178,    -1,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,     4,
      -1,    -1,     7,     8,     9,    -1,    -1,    12,    13,    14,
      -1,    16,    -1,    -1,    -1,    20,    -1,    22,    -1,    24,
      25,    26,    27,    -1,    -1,    -1,    31,    32,    33,    -1,
      35,    36,    37,    38,    -1,    -1,    -1,    42,    43,    -1,
      -1,    -1,    47,    -1,    -1,    50,    -1,    52,    -1,    -1,
      -1,    56,    57,    58,    59,    60,    61,    -1,    -1,    -1,
      65,    66,    67,    68,    69,    -1,    -1,    -1,    -1,    74,
      75,    76,    77,    78,    -1,    -1,    81,    -1,    83,    84,
      -1,    86,    87,    -1,    89,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,   104,
     105,   106,   107,   108,   109,   110,   111,   112,    -1,   114,
     115,   116,   117,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   128,    -1,   130,   131,   132,   133,   134,
     135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,   162,   163,    -1,
      -1,   166,   167,    -1,    -1,   170,    -1,    -1,    -1,   174,
     175,   176,   177,   178,    -1,    -1,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,     4,    -1,
      -1,     7,     8,     9,    -1,    -1,    12,    13,    14,    -1,
      16,    -1,    -1,    -1,    20,    -1,    22,    -1,    24,    25,
      26,    27,    -1,    -1,    -1,    31,    32,    33,    -1,    35,
      36,    37,    38,    -1,    -1,    -1,    42,    43,    -1,    -1,
      -1,    47,    -1,    -1,    50,    -1,    52,    -1,    -1,    -1,
      56,    57,    58,    59,    60,    61,    -1,    -1,    -1,    65,
      66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,
      76,    77,    78,    -1,    -1,    81,    -1,    83,    84,    -1,
      86,    87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    -1,    -1,   101,    -1,    -1,   104,   105,
     106,   107,   108,   109,   110,   111,   112,    -1,   114,   115,
     116,   117,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   128,    -1,   130,   131,   132,   133,   134,   135,
     136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   161,   162,   163,    -1,    -1,
     166,   167,    -1,    -1,   170,    -1,    -1,    -1,   174,   175,
     176,   177,   178,    -1,    -1,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,   192,     4,    -1,    -1,
       7,     8,     9,    -1,    -1,    12,    13,    14,    -1,    16,
      -1,    -1,    -1,    20,    -1,    22,    -1,    24,    25,    26,
      27,    -1,    -1,    -1,    31,    32,    33,    -1,    35,    36,
      37,    38,    -1,    -1,    -1,    42,    43,    -1,    -1,    -1,
      47,    -1,    -1,    50,    -1,    52,    -1,    -1,    -1,    56,
      57,    58,    59,    60,    61,    -1,    -1,    -1,    65,    66,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      77,    78,    -1,    -1,    81,    -1,    83,    84,    -1,    86,
      87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,    -1,   101,    -1,    -1,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    -1,   114,   115,   116,
     117,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,    -1,   130,   131,   132,   133,   134,   135,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,   162,   163,    -1,    -1,   166,
     167,    -1,    -1,   170,    -1,    -1,    -1,   174,   175,   176,
     177,   178,    -1,    -1,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,     4,    -1,    -1,     7,
       8,     9,    -1,    -1,    12,    13,    14,    -1,    16,    -1,
      -1,    -1,    20,    -1,    22,    -1,    24,    25,    26,    27,
      -1,    -1,    -1,    31,    32,    33,    -1,    35,    36,    37,
      38,    -1,    -1,    -1,    42,    43,    -1,    -1,    -1,    47,
      -1,    -1,    50,    -1,    52,    -1,    -1,    -1,    56,    57,
      58,    59,    60,    61,    -1,    -1,    -1,    65,    66,    67,
      68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    77,
      78,    -1,    -1,    81,    -1,    83,    84,    -1,    86,    87,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,    -1,   101,    -1,    -1,   104,   105,   106,   107,
     108,   109,   110,   111,   112,    -1,   114,   115,   116,   117,
     118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     128,    -1,   130,   131,   132,   133,   134,   135,   136,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   161,   162,   163,    -1,    -1,   166,   167,
      -1,    -1,   170,    -1,    -1,    -1,   174,   175,   176,   177,
     178,    -1,    -1,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,     4,    -1,    -1,     7,     8,
       9,    -1,    -1,    12,    13,    14,    -1,    16,    -1,    -1,
      -1,    20,    -1,    22,    -1,    24,    25,    26,    27,    -1,
      -1,    -1,    31,    32,    33,    -1,    35,    36,    37,    38,
      -1,    -1,    -1,    42,    43,    -1,    -1,    -1,    47,    -1,
      -1,    50,    -1,    52,    -1,    -1,    -1,    56,    57,    58,
      59,    60,    61,    -1,    -1,    -1,    65,    66,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    77,    78,
      -1,    -1,    81,    -1,    83,    84,    -1,    86,    87,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,    -1,   101,    -1,    -1,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    -1,   114,   115,   116,   117,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,
      -1,   130,   131,   132,   133,   134,   135,   136,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,   162,   163,    -1,    -1,   166,   167,    -1,
      -1,   170,    -1,    -1,    -1,   174,   175,   176,   177,   178,
      -1,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,     4,    -1,    -1,     7,     8,     9,
      -1,    -1,    12,    13,    14,    -1,    16,    -1,    -1,    -1,
      20,    -1,    22,    -1,    24,    25,    26,    27,    -1,    -1,
      -1,    31,    32,    33,    -1,    35,    36,    37,    38,    -1,
      -1,    -1,    42,    43,    -1,    -1,    -1,    47,    -1,    -1,
      50,    -1,    52,    -1,    -1,    -1,    56,    57,    58,    59,
      60,    61,    -1,    -1,    -1,    65,    66,    67,    68,    69,
      -1,    -1,    -1,    -1,    -1,    -1,    76,    77,    78,    -1,
      -1,    81,    -1,    83,    84,    -1,    86,    87,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,   101,    -1,    -1,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    -1,   114,   115,   116,   117,   118,   119,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,
     130,   131,   132,   133,   134,   135,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   161,   162,   163,    -1,    -1,   166,   167,    -1,    -1,
     170,    -1,    -1,    -1,   174,   175,   176,   177,   178,    -1,
      -1,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,   192,     4,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    -1,    13,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,
      31,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    58,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      81,    -1,    83,    84,    -1,    86,    87,    -1,    89,    90,
      91,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,
     101,    -1,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,    -1,    -1,    -1,    -1,     4,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,   128,    16,   130,
      -1,    -1,    -1,    -1,   135,   136,    -1,    -1,    26,    27,
      -1,    -1,    -1,    31,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,   166,   167,    -1,    -1,   170,
      58,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
     191,   192,    -1,    81,    -1,    83,    84,    -1,    86,    87,
      -1,    89,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,    -1,   101,    -1,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,    -1,    -1,    -1,    -1,     4,
      -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,
     128,    16,   130,    -1,    -1,    -1,    -1,   135,   136,    -1,
      -1,    26,    27,    -1,    -1,    -1,    31,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,   166,   167,
      -1,    -1,   170,    58,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,   191,   192,    -1,    81,    -1,    83,    84,
      -1,    86,    87,    -1,    89,    90,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,   101,    -1,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,    -1,    -1,
      -1,    -1,     4,    -1,    -1,    -1,    -1,     9,    -1,    -1,
      -1,    13,    -1,   128,    16,   130,    -1,    -1,    -1,    -1,
     135,   136,    -1,    -1,    26,    27,    -1,    -1,    -1,    31,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,   166,   167,    -1,    -1,   170,    58,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,   191,   192,    -1,    81,
      -1,    83,    84,    -1,    86,    87,    -1,    89,    -1,    -1,
      -1,    93,    -1,    -1,    -1,    -1,    98,    -1,    -1,   101,
      -1,    -1,   104,   105,   106,   107,   108,   109,   110,   111,
     112,    -1,    -1,    -1,    -1,     4,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    -1,    13,    -1,   128,    16,   130,    -1,
      -1,    -1,    -1,   135,   136,    -1,    -1,    26,    27,    -1,
      -1,    -1,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,   166,   167,    -1,    -1,   170,    58,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,   191,
     192,    -1,    81,    -1,    83,    84,    -1,    86,    87,    -1,
      89,    -1,    91,    -1,    -1,    -1,    -1,    -1,    -1,    98,
      -1,    -1,   101,    -1,    -1,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    -1,    -1,    -1,    -1,     4,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,   128,
      16,   130,    -1,    -1,    -1,    -1,   135,   136,    -1,    -1,
      26,    27,    -1,    -1,    -1,    31,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,   166,   167,    -1,
      -1,   170,    58,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,   191,   192,    -1,    81,    -1,    83,    84,    -1,
      86,    87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    99,    -1,   101,    -1,    -1,   104,   105,
     106,   107,   108,   109,   110,   111,   112,    -1,    -1,    -1,
      -1,     4,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,
      13,    -1,   128,    16,   130,    -1,    -1,    -1,    -1,   135,
     136,    -1,    -1,    26,    27,    -1,    -1,    -1,    31,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
     166,   167,    -1,    -1,   170,    58,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,   191,   192,    -1,    81,    -1,
      83,    84,    85,    86,    87,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,   101,    -1,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      -1,    -1,    -1,    -1,     4,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,   128,    16,   130,    -1,    -1,
      -1,    -1,   135,   136,    -1,    -1,    26,    27,    -1,    -1,
      -1,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,   166,   167,    -1,    -1,   170,    58,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,   191,   192,
      -1,    81,    -1,    83,    84,    -1,    86,    87,    -1,    89,
      -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,   101,    -1,    -1,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    -1,    -1,    -1,    -1,     4,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    -1,    13,    -1,   128,    16,
     130,    -1,    -1,    -1,    -1,   135,   136,    -1,    -1,    26,
      27,    -1,    -1,    -1,    31,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,   166,   167,    -1,    -1,
     170,    58,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,   191,   192,    -1,    81,    -1,    83,    84,    -1,    86,
      87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,    -1,   101,    -1,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    -1,    -1,    -1,    -1,
       4,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    13,
      -1,   128,    16,   130,    -1,    -1,    -1,    -1,   135,   136,
      -1,    -1,    26,    27,    -1,    -1,    -1,    31,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,   166,
     167,    -1,    -1,   170,    58,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,   191,   192,    -1,    81,    -1,    83,
      84,    85,    86,    87,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,
     104,   105,   106,   107,   108,   109,   110,   111,   112,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     4,    -1,
      -1,    -1,    -1,     9,   128,    -1,   130,    13,    -1,    15,
      16,   135,   136,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      26,    27,    -1,    -1,    -1,    31,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    47,   166,   167,    50,    -1,   170,    -1,    -1,    -1,
      -1,    -1,    58,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   191,   192,    -1,
      -1,    77,    -1,    -1,    -1,    81,    -1,    83,    84,    -1,
      86,    87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    98,    -1,    -1,   101,    -1,    -1,   104,   105,
     106,   107,   108,   109,   110,   111,   112,    -1,    -1,    -1,
      -1,     4,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,
      13,    -1,   128,    16,   130,    -1,    -1,    -1,    -1,   135,
     136,    -1,    -1,    26,    27,    -1,    -1,    -1,    31,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
     166,   167,    -1,    -1,   170,    58,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,   191,   192,    -1,    81,    -1,
      83,    84,    85,    86,    87,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,   101,    -1,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      -1,    -1,    -1,    -1,     4,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,   128,    16,   130,    -1,    -1,
      -1,    -1,   135,   136,    -1,    -1,    26,    27,    -1,    -1,
      -1,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,   166,   167,    -1,    -1,   170,    58,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,   191,   192,
      -1,    81,    -1,    83,    84,    -1,    86,    87,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,
      -1,   101,    -1,    -1,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    -1,    -1,    -1,    -1,     4,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    -1,    13,    -1,   128,    16,
     130,    -1,    -1,    -1,    -1,   135,   136,    -1,    -1,    26,
      27,    -1,    -1,    -1,    31,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,   166,   167,    -1,    -1,
     170,    58,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,   191,   192,    -1,    81,    -1,    83,    84,    -1,    86,
      87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    98,    -1,    -1,   101,    -1,    -1,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    -1,    -1,    -1,    -1,
       4,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    13,
      -1,   128,    16,   130,    -1,    -1,    -1,    -1,   135,   136,
      -1,    -1,    26,    27,    -1,    -1,    -1,    31,    32,    33,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,   166,
     167,    -1,    -1,   170,    58,    -1,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,   191,   192,    -1,    81,    -1,    83,
      84,    -1,    86,    87,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,
     104,   105,   106,   107,   108,   109,   110,   111,   112,    -1,
      -1,    -1,    -1,     4,    -1,    -1,    -1,    -1,     9,    -1,
      -1,    -1,    13,    -1,   128,    16,   130,    -1,    -1,    -1,
      -1,   135,   136,    -1,    -1,    26,    27,    -1,    -1,    -1,
      31,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    43,    -1,    -1,    -1,    47,    -1,    -1,    50,
      -1,    -1,   166,   167,    -1,    -1,   170,    58,    -1,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    77,   191,   192,    -1,
      81,    -1,    83,    84,    -1,    86,    87,    -1,    89,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    98,    -1,    -1,
     101,    -1,    -1,   104,   105,   106,   107,   108,   109,   110,
     111,   112,    -1,    -1,    -1,    -1,     4,    -1,    -1,    -1,
      -1,     9,    -1,    -1,    -1,    13,    -1,   128,    16,   130,
      -1,    -1,    -1,    -1,   135,   136,    -1,    -1,    26,    27,
      -1,    -1,    -1,    31,    32,    33,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    47,
      -1,    -1,    50,    -1,    -1,   166,   167,    -1,    -1,   170,
      58,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,
     191,   192,    -1,    81,    -1,    83,    84,    -1,    86,    87,
      -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      98,    -1,    -1,   101,    -1,    -1,   104,   105,   106,   107,
     108,   109,   110,   111,   112,    -1,    -1,    -1,    -1,     4,
      -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,
     128,    16,   130,    -1,    -1,    -1,    -1,   135,   136,    -1,
      -1,    26,    27,    -1,    -1,    -1,    31,    32,    33,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,
      -1,    -1,    47,    -1,    -1,    50,    -1,    -1,   166,   167,
      -1,    -1,   170,    58,    -1,    60,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    77,   191,   192,    -1,    81,    -1,    83,    84,
      -1,    86,    87,    -1,    89,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    98,    -1,    -1,   101,    -1,    -1,   104,
     105,   106,   107,   108,   109,   110,   111,   112,    -1,    -1,
      -1,    -1,     4,    -1,    -1,    -1,    -1,     9,    -1,    -1,
      -1,    13,    -1,   128,    16,   130,    -1,    -1,    -1,    -1,
     135,   136,    -1,    -1,    26,    27,    -1,    -1,    -1,    31,
      32,    33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    47,    -1,    -1,    50,    -1,
      -1,   166,   167,    -1,    -1,   170,    58,    -1,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    77,   191,   192,    -1,    81,
      -1,    83,    84,    -1,    86,    87,    -1,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,
      -1,    -1,   104,   105,   106,   107,   108,   109,   110,   111,
     112,    -1,    -1,    -1,    -1,     4,    -1,    -1,    -1,    -1,
       9,    -1,    -1,    -1,    13,    -1,   128,    16,   130,    -1,
      -1,    -1,    -1,   135,   136,    -1,    -1,    26,    27,    -1,
      -1,    -1,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,    47,    -1,
      -1,    50,    -1,    -1,   166,   167,    -1,    -1,   170,    58,
      -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,   191,
     192,    -1,    81,    -1,    83,    84,    -1,    86,    87,    -1,
      89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   101,    -1,    -1,   104,   105,   106,   107,   108,
     109,   110,   111,   112,    -1,    -1,    -1,    -1,     4,    -1,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    -1,   128,
      16,   130,    -1,    -1,    -1,    -1,   135,   136,    -1,    -1,
      26,    27,    -1,    -1,    -1,    31,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    47,    -1,    -1,    50,    -1,    -1,   166,   167,    -1,
      -1,   170,    58,    -1,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    77,   191,   192,    -1,    81,    -1,    83,    84,    -1,
      86,    87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,   104,   105,
     106,   107,   108,   109,   110,   111,   112,    -1,    -1,    -1,
      -1,     4,    -1,    -1,    -1,    -1,     9,    -1,    -1,    -1,
      13,    -1,   128,    16,   130,    -1,    -1,    -1,    -1,   135,
     136,    -1,    -1,    26,    27,    -1,    -1,    -1,    31,    32,
      33,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      43,    -1,    -1,    -1,    47,    -1,    -1,    50,    -1,    -1,
     166,   167,    -1,    -1,   170,    58,    -1,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    77,   191,   192,    -1,    81,    -1,
      83,    84,    -1,    86,    87,    -1,    89,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   101,    -1,
      -1,   104,   105,   106,   107,   108,   109,   110,   111,   112,
      -1,    -1,    -1,    -1,     4,    -1,    -1,    -1,    -1,     9,
      -1,    -1,    -1,    13,    -1,   128,    16,   130,    -1,    -1,
      -1,    -1,   135,   136,    -1,    -1,    26,    27,    -1,    -1,
      -1,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    43,    -1,    -1,    -1,    47,    -1,    -1,
      50,    -1,    -1,   166,   167,    -1,    -1,   170,    58,    -1,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    77,   191,   192,
      -1,    81,    -1,    83,    84,    -1,    86,    87,    -1,    89,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   101,    -1,    -1,   104,   105,   106,   107,   108,   109,
     110,   111,   112,    -1,    -1,    -1,    -1,     4,    -1,    -1,
      -1,    -1,     9,    -1,    -1,    -1,    13,    -1,   128,    16,
     130,    -1,    -1,    -1,    -1,   135,   136,    -1,    -1,    26,
      27,    -1,    -1,    -1,    31,    32,    33,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    43,    -1,    -1,    -1,
      47,    -1,    -1,    50,    -1,    -1,   166,   167,    -1,    -1,
     170,    58,    -1,    60,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      77,   191,   192,    -1,    81,    -1,    83,    84,    -1,    86,
      87,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   101,    -1,    -1,   104,   105,   106,
     107,   108,   109,   110,   111,   112,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   128,    -1,   130,    -1,    -1,    -1,    -1,   135,   136,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    22,    -1,    24,    25,    -1,    -1,   166,
     167,    -1,    -1,   170,    33,    -1,    35,    36,    37,    38,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,   191,   192,    -1,    56,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    65,    66,    67,    68,
      69,    -1,    -1,    72,    73,    74,    75,    76,    -1,    78,
      79,    -1,    -1,    -1,    -1,    84,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,    -1,
      -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,    -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   153,   154,   155,    -1,   157,   158,
     159,    -1,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,
      -1,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,     6,     7,     8,    -1,    -1,    -1,    12,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    20,    -1,    22,    -1,
      24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    36,    37,    38,    -1,    -1,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    56,    57,    -1,    59,    -1,    61,    -1,    -1,
      -1,    65,    66,    67,    68,    69,    -1,    -1,    72,    73,
      74,    75,    76,    -1,    78,    79,    -1,    -1,    -1,    -1,
      84,    -1,    86,    -1,    -1,    -1,    -1,    -1,    -1,    93,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,   113,
     114,   115,   116,   117,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,   131,   132,   133,
     134,    -1,    -1,    -1,    -1,   139,   140,    -1,    -1,    -1,
     144,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     174,   175,   176,   177,   178,    -1,    -1,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,     6,     7,     8,
      -1,    -1,    -1,    12,    -1,    14,    -1,    -1,    -1,    -1,
      -1,    20,    -1,    22,    -1,    24,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    36,    37,    38,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    56,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    65,    66,    67,    68,
      69,    -1,    -1,    72,    73,    74,    75,    76,    -1,    78,
      79,    -1,    -1,    -1,    -1,    84,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   110,    -1,    -1,   113,   114,   115,   116,   117,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
     139,   140,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,
      -1,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,     7,     8,    -1,    -1,    -1,    12,    -1,    14,
      -1,    -1,    -1,    -1,    -1,    20,    -1,    22,    -1,    24,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,
      35,    36,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    56,    57,    -1,    59,    -1,    61,    -1,    -1,    -1,
      65,    66,    67,    68,    69,    -1,    -1,    -1,    -1,    -1,
      -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   103,    -1,
      -1,    -1,    -1,    -1,    -1,   110,    -1,    -1,    -1,   114,
     115,   116,   117,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,   139,   140,    -1,    -1,    -1,   144,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
     175,   176,   177,   178,    -1,    -1,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,     7,     8,    -1,    -1,
      -1,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    22,    -1,    24,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    36,    37,    38,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    56,    57,    -1,    59,    -1,
      61,    -1,    -1,    -1,    65,    66,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    93,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   103,    -1,    -1,    -1,    -1,    -1,    -1,   110,
      -1,    -1,    -1,   114,   115,   116,   117,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,   139,   140,
      -1,    -1,    -1,   144,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,   175,   176,   177,   178,    -1,    -1,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
       7,     8,    -1,    -1,    -1,    12,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    22,    -1,    24,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    36,
      37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    56,
      57,    -1,    59,    -1,    61,    -1,    -1,    -1,    65,    66,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    93,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   110,    -1,    -1,    -1,   114,   115,   116,
     117,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,   139,   140,    -1,    -1,    -1,   144,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,
     177,   178,    -1,    -1,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,     7,     8,    -1,    -1,    -1,    12,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    20,    -1,    22,
      -1,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    36,    37,    38,    -1,    -1,    -1,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    56,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    65,    66,    67,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      -1,    -1,     7,     8,    -1,    -1,    -1,    12,    -1,    14,
      -1,    -1,    -1,    -1,    -1,    20,    -1,    22,    -1,    24,
      25,   114,   115,   116,   117,   118,   119,    -1,    33,    -1,
      35,    36,    37,    38,    -1,    -1,    -1,    42,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    56,    57,    -1,    59,    -1,    61,    -1,    -1,    -1,
      65,    66,    67,    68,    69,    -1,    -1,    -1,   161,   162,
     163,    76,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      85,   174,   175,   176,   177,   178,    -1,    -1,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
     115,   116,   117,   118,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
     175,   176,   177,   178,    -1,    -1,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,     7,     8,    -1,    -1,
      -1,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    22,    -1,    24,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    -1,    35,    36,    37,    38,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    56,    57,    -1,    59,    -1,
      61,    -1,    -1,    -1,    65,    66,    67,    68,    69,    -1,
      -1,    -1,    -1,    -1,    -1,    76,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     7,     8,    -1,    -1,    -1,    12,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    20,    -1,    22,
      -1,    24,    25,   114,   115,   116,   117,   118,   119,    -1,
      33,    -1,    35,    36,    37,    38,    -1,    -1,    -1,    42,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    56,    57,    -1,    59,    -1,    61,    -1,
      -1,    64,    65,    66,    67,    68,    69,    -1,    -1,    -1,
     161,   162,   163,    76,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,   175,   176,   177,   178,    -1,    -1,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   114,   115,   116,   117,   118,   119,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,   162,
     163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,   175,   176,   177,   178,    -1,    -1,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,     7,     8,
      -1,    -1,    -1,    12,    -1,    14,    -1,    -1,    17,    -1,
      -1,    20,    -1,    22,    -1,    24,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    36,    37,    38,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    56,    57,    -1,
      59,    -1,    61,    -1,    -1,    -1,    65,    66,    67,    68,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     7,     8,    -1,    -1,
      -1,    12,    -1,    14,    -1,    -1,    -1,    -1,    -1,    20,
      -1,    22,    -1,    24,    25,   114,   115,   116,   117,   118,
     119,    -1,    33,    -1,    35,    36,    37,    38,    -1,    -1,
      -1,    42,   131,   132,   133,   134,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    56,    57,    -1,    59,    -1,
      61,    -1,    -1,    -1,    65,    66,    67,    68,    69,    -1,
      -1,    -1,   161,   162,   163,    76,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,
      -1,    -1,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,   116,   117,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     131,   132,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,   175,   176,   177,   178,    -1,    -1,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
       7,     8,    -1,    -1,    -1,    12,    -1,    14,    -1,    -1,
      -1,    -1,    -1,    20,    -1,    22,    -1,    24,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    -1,    35,    36,
      37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,    -1,    56,
      57,    -1,    59,    -1,    61,    -1,    -1,    -1,    65,    66,
      67,    68,    69,    -1,    -1,    -1,    -1,    -1,    -1,    76,
      -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,   115,   116,
     117,   118,   119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   131,   132,   133,   134,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   161,   162,   163,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,   175,   176,
     177,   178,    -1,    -1,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,     7,     8,    -1,    -1,    -1,    12,
      -1,    14,    -1,    -1,    -1,    -1,    -1,    20,    -1,    22,
      -1,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    -1,    35,    36,    37,    38,    -1,    -1,    -1,    42,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    56,    57,    -1,    59,    -1,    61,    -1,
      -1,    -1,    65,    66,    67,    68,    69,    -1,    -1,    -1,
      -1,    -1,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     7,     8,    -1,    -1,    -1,    12,   101,    14,
      -1,    -1,    -1,    -1,    -1,    20,    -1,    22,    -1,    24,
      25,   114,   115,   116,   117,    -1,   119,    -1,    33,    -1,
      35,    36,    37,    38,    -1,    -1,    -1,    42,   131,   132,
     133,   134,    -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,
      -1,    56,    57,    -1,    59,    -1,    61,    -1,    -1,    -1,
      65,    66,    67,    68,    69,    -1,    -1,    -1,   161,   162,
     163,    76,    -1,    78,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   174,   175,   176,   177,   178,    -1,    -1,   181,   182,
     183,   184,   185,    -1,   187,   188,   189,   190,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   114,
     115,   116,   117,    -1,   119,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   131,   132,   133,   134,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   161,   162,   163,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   174,
     175,   176,   177,   178,    -1,    -1,   181,   182,   183,   184,
     185,    -1,   187,   188,   189,   190,     3,     4,     5,     6,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    18,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    39,    40,    41,    -1,    43,    44,    45,    46,
      -1,    48,    49,    50,    51,    -1,    -1,    54,    55,    -1,
      -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    -1,    93,    94,    95,    96,
      97,    98,    -1,   100,    -1,   102,    -1,     3,     4,     5,
       6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   118,    18,    -1,    -1,    -1,    -1,    23,    -1,    -1,
      -1,    -1,    -1,    -1,    30,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   139,    39,    40,    41,    -1,    43,    44,    45,
      46,    -1,    48,    49,    50,    51,    -1,    -1,    54,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    62,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    71,    -1,    -1,    -1,    -1,
      -1,    -1,   179,    -1,    -1,    -1,    82,    83,    84,   186,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,     7,   100,    -1,   102,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      24,    -1,   118,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,
      -1,    -1,    -1,   139,    -1,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    56,    57,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,
      74,    75,    76,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    86,   179,    -1,    -1,    -1,    -1,    92,    -1,
     186,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,    -1,   117,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   161,   162,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     7,    -1,
     174,   175,   176,   177,   178,    14,    -1,   181,   182,   183,
     184,   185,   186,    -1,    -1,    24,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    -1,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    56,    57,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,
      69,    -1,    -1,    -1,    -1,    74,    75,    76,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    84,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    92,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,   115,    -1,   117,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   130,    -1,    -1,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     7,    -1,   174,   175,   176,   177,   178,
      14,    -1,   181,   182,   183,   184,   185,   186,    -1,    -1,
      24,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,
      -1,    35,    -1,    -1,    -1,    -1,    -1,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    56,    57,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,
      74,    75,    76,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      84,    -1,    86,    -1,    -1,    -1,    -1,    -1,    92,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,    -1,   117,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   130,    -1,    -1,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     7,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    -1,   161,   162,   163,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
     174,   175,   176,   177,   178,    -1,    35,   181,   182,   183,
     184,   185,   186,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    56,    57,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,   115,    -1,   117,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     7,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    -1,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,
      -1,    35,   181,   182,   183,   184,   185,   186,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    56,    57,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,    -1,   117,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     7,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    -1,   161,   162,   163,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
     174,   175,   176,   177,   178,    -1,    35,   181,   182,   183,
     184,   185,   186,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    56,    57,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    85,    -1,    -1,    88,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   114,   115,    -1,   117,   118,
     119,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   133,   134,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     7,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    -1,   161,   162,   163,    -1,    -1,    -1,    -1,    -1,
      24,    -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,
      -1,    35,   181,   182,   183,   184,   185,   186,    42,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,
      -1,    -1,    56,    57,    -1,    59,    -1,    -1,    -1,    -1,
      -1,    65,    -1,    67,    -1,    69,    -1,    -1,    -1,    -1,
      -1,    -1,    76,    -1,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     114,   115,    -1,   117,   118,   119,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,
     134,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     7,    -1,
      -1,    -1,    -1,    -1,    -1,    14,    -1,   161,   162,   163,
      -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,    -1,    -1,
     174,   175,   176,   177,   178,    -1,    35,   181,   182,   183,
     184,   185,   186,    42,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    56,    57,    -1,
      59,    -1,    -1,    -1,    -1,    -1,    65,    -1,    67,    -1,
      69,    -1,    -1,    -1,    -1,    -1,    -1,    76,    -1,    78,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    93,    -1,     7,    -1,    -1,    -1,
      -1,    -1,    -1,    14,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    24,    -1,   114,   115,    -1,   117,   118,
     119,    -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    42,    -1,    -1,   133,   134,    -1,    -1,    -1,    -1,
      -1,    52,    -1,    -1,    -1,    56,    57,    -1,    59,    -1,
      -1,    -1,    -1,    -1,    65,    -1,    67,    -1,    69,    -1,
      -1,    -1,   161,   162,   163,    76,    -1,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   174,   175,   176,   177,   178,
      -1,    -1,   181,   182,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   114,   115,    -1,   117,   118,   119,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   133,   134,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     161,   162,   163,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   174,   175,   176,   177,   178,    -1,    -1,
     181,   182,   183,   184,   185,   186
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short int yystos[] =
{
       0,     6,     7,     8,    12,    14,    20,    22,    24,    25,
      33,    35,    36,    37,    38,    42,    52,    56,    57,    59,
      61,    65,    66,    67,    68,    69,    72,    73,    74,    75,
      76,    78,    79,    84,    86,    93,   110,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   130,   131,   132,   133,   134,   139,   140,   144,
     153,   154,   155,   157,   158,   159,   161,   162,   163,   174,
     175,   176,   177,   178,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   194,   195,   196,   197,   198,   202,
     203,   208,   211,   216,   217,   221,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   233,   234,   235,   236,   237,
     238,   239,   242,   247,   248,   250,   253,   254,   255,   262,
     263,   264,   265,   267,   268,   286,   287,   290,   291,   292,
     320,   321,   345,   346,   351,   352,   357,   368,   369,   370,
     382,   400,   401,   408,   414,   415,    69,   212,    84,    84,
      84,   205,   206,   207,   208,   246,   249,   255,   264,   345,
     415,    76,   261,   262,   197,   286,   204,   345,    84,    84,
       4,     9,    13,    16,    26,    27,    31,    32,    43,    47,
      50,    58,    60,    77,    81,    83,    84,    86,    87,    89,
      98,   101,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   128,   135,   136,   166,   167,   170,   191,   192,   315,
     316,   320,   322,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,   343,
     344,   348,   355,   356,   358,   409,   324,   339,    10,    11,
      15,    17,    19,    28,    29,    34,    53,    63,    70,    84,
      86,    93,   101,   110,   129,   137,   138,   144,   165,   168,
     169,   172,   173,   199,   200,   201,   202,   217,   286,   300,
     301,   302,   303,   304,   305,   306,   307,   308,   310,   314,
     320,   340,   345,   349,   366,   367,   369,   372,   404,   195,
      93,   110,   144,   221,   223,   293,   294,   345,   368,   369,
     371,   382,   224,   229,   231,   232,   234,   235,   237,   271,
     232,   347,   320,    84,    84,   142,   143,   145,   146,   147,
     373,   374,     4,    49,    83,    86,    87,    89,   100,   102,
     345,   383,   384,   385,   387,   320,   392,   101,   339,   393,
     394,   264,   396,   395,   396,   340,    98,   402,   403,    84,
       0,   196,   239,   250,    93,   225,   229,   230,   224,   224,
      93,    88,     6,   206,   243,   244,   245,   368,   246,   249,
      84,    98,   252,   252,    98,   252,   101,   207,   101,   218,
     219,   220,   226,   229,   288,   289,   305,   101,   206,   207,
     345,    84,   271,   337,   338,   342,   205,   248,   253,   263,
      85,    85,    85,   262,    88,    93,   271,   340,    84,   345,
      84,   325,   325,    84,   325,    84,   271,   305,   340,   340,
      91,    84,    84,    84,    84,    84,   325,   327,   327,   327,
      84,    84,    84,    84,   325,    84,   325,    84,   325,    84,
      84,    43,    50,    51,    84,    90,    98,   179,   410,   412,
     327,    82,    86,    91,    87,    89,    39,    54,    30,    41,
      94,    96,    23,    46,    83,   100,   102,     4,     3,     5,
      18,    40,    44,    45,    48,    49,    55,    62,    71,    95,
      97,   341,    88,    60,    84,   325,   354,   356,    88,    93,
     342,    93,    92,   300,    84,    98,    86,   345,    84,    93,
     340,    84,    84,    84,   208,   415,   103,   306,   200,   286,
      84,    84,   374,   340,   405,    93,    84,   405,   405,   300,
      93,    92,   294,   374,    84,    92,   247,   248,   264,   295,
     297,   345,    93,   294,   229,   224,    84,    98,   248,   252,
     273,   274,    88,    60,   353,   353,    84,    84,   151,   375,
     376,   377,    84,   375,   196,    92,    98,   339,   359,    98,
     101,   397,   398,   399,    88,   397,   397,    86,    99,   342,
     403,   345,   416,   417,   418,    93,   224,   224,   207,   240,
     241,   246,    84,    95,   280,   206,   206,    85,   221,   222,
     224,   229,   251,   275,   276,   277,   278,   279,   345,    59,
      86,   257,   259,   260,   261,   339,   257,   259,   260,   269,
     270,   345,   266,   345,   239,   226,   229,   224,   218,   289,
     103,   293,   345,   345,   344,    85,    88,   246,   249,   345,
      85,    85,    14,    85,   209,   210,   345,   271,   271,   339,
      85,    85,    85,    99,   359,   339,   271,   339,   271,   271,
     350,   353,   339,   339,   271,   271,   271,   340,   339,   320,
      85,   324,   320,   311,   340,    84,   339,   411,   327,   327,
     327,   328,   328,   329,   329,   330,   330,   330,   330,   331,
     331,   332,   333,   334,   335,   336,    92,   340,   339,   339,
     271,   339,    64,    92,   300,    70,    84,   217,   221,   303,
     313,   351,   340,    93,   309,   340,    93,   309,   309,   103,
     350,   340,   300,    93,   313,    93,    93,   300,   294,   342,
     207,    93,    88,   207,   224,   207,   272,   257,   259,   260,
     272,    98,   252,   232,    85,    85,   345,   152,   378,   379,
     380,   381,    84,   145,    88,   377,   378,   145,   232,   386,
     388,   340,   391,    88,   103,   339,   103,   340,   399,   396,
      99,    99,    84,    85,    88,   248,    88,   243,   344,   101,
     281,   323,   339,    84,   247,   248,   273,   207,   229,   224,
      85,    88,    88,   258,   259,   256,   257,    99,   258,   256,
      99,    88,   103,    95,    93,   224,   103,    85,    92,   344,
      85,    85,    88,    84,    85,    85,    88,   323,   327,    91,
      88,    88,    88,    88,    85,    85,    88,    88,    85,    85,
      85,    85,    85,    88,    85,    92,    93,    99,    85,   324,
      88,   180,   413,   338,    92,    85,   342,   300,    84,   309,
     312,    99,    93,    85,    85,    85,    85,    85,   405,    95,
     299,   207,   246,   296,   298,   345,    92,   248,   274,    85,
     258,   256,    99,   257,   259,   260,    85,    85,   380,    85,
     378,   377,    85,    88,    92,    99,    92,   345,   360,   361,
      99,    92,   103,   156,   397,   324,   418,   241,   280,    85,
      90,    98,   103,   281,   282,   283,   284,   285,   345,   207,
     207,   207,   224,   345,    64,   279,   260,   260,   260,   260,
     103,   270,   342,    93,    98,   213,   214,   215,   344,    85,
      85,   210,   324,   101,    17,   271,   317,   318,   103,   271,
     319,   345,   339,   271,   353,   342,   339,   311,   340,   340,
      85,   339,   338,   327,    92,   340,    93,    84,   300,   300,
     300,    93,    93,   342,    92,   207,    88,   207,   342,   272,
     260,   260,   258,   256,    99,   143,   375,    85,   375,   388,
     340,   389,   340,   391,    95,    88,   340,   340,    85,   345,
     338,   342,    88,   103,   281,    95,    98,   285,    92,   248,
      99,    99,    99,    99,   345,    85,    92,    88,    84,    93,
      85,    92,    92,    85,    88,    85,    85,   284,    88,    85,
      85,    85,    92,    99,    92,    99,    88,   300,    85,   311,
     313,    21,   405,   342,   299,   298,    92,    99,    99,   260,
     260,   145,   145,    92,    99,   101,   339,   362,   363,   364,
     365,   361,   103,   103,    99,   103,   281,   283,   345,   281,
      99,    93,   213,   215,   340,   339,   339,   318,   339,   340,
     340,   339,    93,    85,   312,   300,    93,   342,    99,    99,
     320,   322,   390,   324,    92,    93,   281,    92,   344,    85,
      92,    85,    85,    99,    99,    88,   300,    93,    15,   340,
     406,   407,   280,    84,   103,   339,   339,   281,    84,    93,
     213,   339,   311,    85,    85,   324,    92,    92,   340,    85,
      85,   300,    85,   339,   339,    85,    93,   300
};

/* Error token number */
#define YYTERROR 1


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (YYID (N))                                                     \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (YYID (0))
#endif

# define YYRHSLOC(Rhs, K) ((Rhs)[K].yystate.yyloc)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

__attribute__((__unused__))
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
#else
static unsigned
yy_location_print_ (yyo, yylocp)
    FILE *yyo;
    YYLTYPE const * const yylocp;
#endif
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += fprintf (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += fprintf (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += fprintf (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += fprintf (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += fprintf (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#define YYLEX yylex ()

YYSTYPE yylval;
YYLTYPE yylloc;

int yynerrs;
int yychar;

static const int YYEOF = 0;
static const int YYEMPTY = -2;

typedef enum { yyok, yyaccept, yyabort, yyerr } YYRESULTTAG;

#define YYCHK(YYE)                                                           \
   do { YYRESULTTAG yyflag = YYE; if (yyflag != yyok) return yyflag; }       \
   while (YYID (0))

#if YYDEBUG

# ifndef YYFPRINTF
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, AST* parsed_tree)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (parsed_tree);
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

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, AST* parsed_tree)
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, parsed_tree);
  YYFPRINTF (yyoutput, ")");
}

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)          \
do {                                                            \
  if (yydebug)                                                  \
    {                                                           \
      YYFPRINTF (stderr, "%s ", Title);                         \
      yy_symbol_print (stderr, Type, Value, Location, parsed_tree);        \
      YYFPRINTF (stderr, "\n");                                 \
    }                                                           \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;

#else /* !YYDEBUG */

# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)

#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYMAXDEPTH * sizeof (GLRStackItem)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

/* Minimum number of free items on the stack allowed after an
   allocation.  This is to allow allocation and initialization
   to be completed by functions that call yyexpandGLRStack before the
   stack is expanded, thus insuring that all necessary pointers get
   properly redirected to new data.  */
#define YYHEADROOM 2

#ifndef YYSTACKEXPANDABLE
# if (! defined __cplusplus \
      || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
          && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL))
#  define YYSTACKEXPANDABLE 1
# else
#  define YYSTACKEXPANDABLE 0
# endif
#endif

#if YYSTACKEXPANDABLE
# define YY_RESERVE_GLRSTACK(Yystack)                   \
  do {                                                  \
    if (Yystack->yyspaceLeft < YYHEADROOM)              \
      yyexpandGLRStack (Yystack);                       \
  } while (YYID (0))
#else
# define YY_RESERVE_GLRSTACK(Yystack)                   \
  do {                                                  \
    if (Yystack->yyspaceLeft < YYHEADROOM)              \
      yyMemoryExhausted (Yystack);                      \
  } while (YYID (0))
#endif


#if YYERROR_VERBOSE

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
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
static size_t
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
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
    return strlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* !YYERROR_VERBOSE */

/** State numbers, as in LALR(1) machine */
typedef int yyStateNum;

/** Rule numbers, as in LALR(1) machine */
typedef int yyRuleNum;

/** Grammar symbol */
typedef short int yySymbol;

/** Item references, as in LALR(1) machine */
typedef short int yyItemNum;

typedef struct yyGLRState yyGLRState;
typedef struct yyGLRStateSet yyGLRStateSet;
typedef struct yySemanticOption yySemanticOption;
typedef union yyGLRStackItem yyGLRStackItem;
typedef struct yyGLRStack yyGLRStack;

struct yyGLRState {
  /** Type tag: always true.  */
  yybool yyisState;
  /** Type tag for yysemantics.  If true, yysval applies, otherwise
   *  yyfirstVal applies.  */
  yybool yyresolved;
  /** Number of corresponding LALR(1) machine state.  */
  yyStateNum yylrState;
  /** Preceding state in this stack */
  yyGLRState* yypred;
  /** Source position of the first token produced by my symbol */
  size_t yyposn;
  union {
    /** First in a chain of alternative reductions producing the
     *  non-terminal corresponding to this state, threaded through
     *  yynext.  */
    yySemanticOption* yyfirstVal;
    /** Semantic value for this state.  */
    YYSTYPE yysval;
  } yysemantics;
  /** Source location for this state.  */
  YYLTYPE yyloc;
};

struct yyGLRStateSet {
  yyGLRState** yystates;
  /** During nondeterministic operation, yylookaheadNeeds tracks which
   *  stacks have actually needed the current lookahead.  During deterministic
   *  operation, yylookaheadNeeds[0] is not maintained since it would merely
   *  duplicate yychar != YYEMPTY.  */
  yybool* yylookaheadNeeds;
  size_t yysize, yycapacity;
};

struct yySemanticOption {
  /** Type tag: always false.  */
  yybool yyisState;
  /** Rule number for this reduction */
  yyRuleNum yyrule;
  /** The last RHS state in the list of states to be reduced.  */
  yyGLRState* yystate;
  /** The lookahead for this reduction.  */
  int yyrawchar;
  YYSTYPE yyval;
  YYLTYPE yyloc;
  /** Next sibling in chain of options.  To facilitate merging,
   *  options are chained in decreasing order by address.  */
  yySemanticOption* yynext;
};

/** Type of the items in the GLR stack.  The yyisState field
 *  indicates which item of the union is valid.  */
union yyGLRStackItem {
  yyGLRState yystate;
  yySemanticOption yyoption;
};

struct yyGLRStack {
  int yyerrState;
  /* To compute the location of the error token.  */
  yyGLRStackItem yyerror_range[3];

  YYJMP_BUF yyexception_buffer;
  yyGLRStackItem* yyitems;
  yyGLRStackItem* yynextFree;
  size_t yyspaceLeft;
  yyGLRState* yysplitPoint;
  yyGLRState* yylastDeleted;
  yyGLRStateSet yytops;
};

#if YYSTACKEXPANDABLE
static void yyexpandGLRStack (yyGLRStack* yystackp);
#endif

static void yyFail (yyGLRStack* yystackp, AST* parsed_tree, const char* yymsg)
  __attribute__ ((__noreturn__));
static void
yyFail (yyGLRStack* yystackp, AST* parsed_tree, const char* yymsg)
{
  if (yymsg != YY_NULL)
    yyerror (parsed_tree, yymsg);
  YYLONGJMP (yystackp->yyexception_buffer, 1);
}

static void yyMemoryExhausted (yyGLRStack* yystackp)
  __attribute__ ((__noreturn__));
static void
yyMemoryExhausted (yyGLRStack* yystackp)
{
  YYLONGJMP (yystackp->yyexception_buffer, 2);
}

#if YYDEBUG || YYERROR_VERBOSE
/** A printable representation of TOKEN.  */
static inline const char*
yytokenName (yySymbol yytoken)
{
  if (yytoken == YYEMPTY)
    return "";

  return yytname[yytoken];
}
#endif

/** Fill in YYVSP[YYLOW1 .. YYLOW0-1] from the chain of states starting
 *  at YYVSP[YYLOW0].yystate.yypred.  Leaves YYVSP[YYLOW1].yystate.yypred
 *  containing the pointer to the next state in the chain.  */
static void yyfillin (yyGLRStackItem *, int, int) __attribute__ ((__unused__));
static void
yyfillin (yyGLRStackItem *yyvsp, int yylow0, int yylow1)
{
  int i;
  yyGLRState *s = yyvsp[yylow0].yystate.yypred;
  for (i = yylow0-1; i >= yylow1; i -= 1)
    {
      YYASSERT (s->yyresolved);
      yyvsp[i].yystate.yyresolved = yytrue;
      yyvsp[i].yystate.yysemantics.yysval = s->yysemantics.yysval;
      yyvsp[i].yystate.yyloc = s->yyloc;
      s = yyvsp[i].yystate.yypred = s->yypred;
    }
}

/* Do nothing if YYNORMAL or if *YYLOW <= YYLOW1.  Otherwise, fill in
 * YYVSP[YYLOW1 .. *YYLOW-1] as in yyfillin and set *YYLOW = YYLOW1.
 * For convenience, always return YYLOW1.  */
static inline int yyfill (yyGLRStackItem *, int *, int, yybool)
     __attribute__ ((__unused__));
static inline int
yyfill (yyGLRStackItem *yyvsp, int *yylow, int yylow1, yybool yynormal)
{
  if (!yynormal && yylow1 < *yylow)
    {
      yyfillin (yyvsp, *yylow, yylow1);
      *yylow = yylow1;
    }
  return yylow1;
}

/** Perform user action for rule number YYN, with RHS length YYRHSLEN,
 *  and top stack item YYVSP.  YYLVALP points to place to put semantic
 *  value ($$), and yylocp points to place for location information
 *  (@$).  Returns yyok for normal return, yyaccept for YYACCEPT,
 *  yyerr for YYERROR, yyabort for YYABORT.  */
/*ARGSUSED*/ static YYRESULTTAG
yyuserAction (yyRuleNum yyn, int yyrhslen, yyGLRStackItem* yyvsp,
              yyGLRStack* yystackp,
              YYSTYPE* yyvalp, YYLTYPE *yylocp, AST* parsed_tree)
{
  yybool yynormal __attribute__ ((__unused__)) =
    (yystackp->yysplitPoint == YY_NULL);
  int yylow;
  YYUSE (parsed_tree);
# undef yyerrok
# define yyerrok (yystackp->yyerrState = 0)
# undef YYACCEPT
# define YYACCEPT return yyaccept
# undef YYABORT
# define YYABORT return yyabort
# undef YYERROR
# define YYERROR return yyerrok, yyerr
# undef YYRECOVERING
# define YYRECOVERING() (yystackp->yyerrState != 0)
# undef yyclearin
# define yyclearin (yychar = YYEMPTY)
# undef YYFILL
# define YYFILL(N) yyfill (yyvsp, &yylow, N, yynormal)
# undef YYBACKUP
# define YYBACKUP(Token, Value)                                              \
  return yyerror (parsed_tree, YY_("syntax error: cannot back up")),     \
         yyerrok, yyerr

  yylow = 1;
  if (yyrhslen == 0)
    *yyvalp = yyval_default;
  else
    *yyvalp = yyvsp[YYFILL (1-yyrhslen)].yystate.yysemantics.yysval;
  YYLLOC_DEFAULT ((*yylocp), (yyvsp - yyrhslen), yyrhslen);
  yystackp->yyerror_range[1].yystate.yyloc = *yylocp;

  switch (yyn)
    {
        case 2:
/* Line 868 of glr.c  */
#line 2078 "src/frontend/c99.y"
    {
	*parsed_tree = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 3:
/* Line 868 of glr.c  */
#line 2082 "src/frontend/c99.y"
    {
	*parsed_tree = NULL;
}
    break;

  case 4:
/* Line 868 of glr.c  */
#line 2092 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 5:
/* Line 868 of glr.c  */
#line 2096 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast));
}
    break;

  case 6:
/* Line 868 of glr.c  */
#line 2102 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 7:
/* Line 868 of glr.c  */
#line 2106 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 8:
/* Line 868 of glr.c  */
#line 2112 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 9:
/* Line 868 of glr.c  */
#line 2116 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 10:
/* Line 868 of glr.c  */
#line 2120 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 11:
/* Line 868 of glr.c  */
#line 2127 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake2(AST_STATIC_ASSERT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (7))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (7))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yyloc).first_column), NULL);
}
    break;

  case 12:
/* Line 868 of glr.c  */
#line 2133 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 13:
/* Line 868 of glr.c  */
#line 2137 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 14:
/* Line 868 of glr.c  */
#line 2141 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 15:
/* Line 868 of glr.c  */
#line 2147 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_EXTENSION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 16:
/* Line 868 of glr.c  */
#line 2151 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 17:
/* Line 868 of glr.c  */
#line 2157 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 18:
/* Line 868 of glr.c  */
#line 2163 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 19:
/* Line 868 of glr.c  */
#line 2168 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 20:
/* Line 868 of glr.c  */
#line 2172 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_EXTENSION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 21:
/* Line 868 of glr.c  */
#line 2177 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_PP_COMMENT, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 22:
/* Line 868 of glr.c  */
#line 2182 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_PP_TOKEN, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 23:
/* Line 868 of glr.c  */
#line 2189 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_LABEL_DECL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 24:
/* Line 868 of glr.c  */
#line 2195 "src/frontend/c99.y"
    {
    AST symbol_holder = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
	((*yyvalp).ast) = ASTListLeaf(symbol_holder);
}
    break;

  case 25:
/* Line 868 of glr.c  */
#line 2200 "src/frontend/c99.y"
    {
	AST label = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), label);
}
    break;

  case 26:
/* Line 868 of glr.c  */
#line 2209 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 27:
/* Line 868 of glr.c  */
#line 2215 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast));
}
    break;

  case 28:
/* Line 868 of glr.c  */
#line 2219 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 29:
/* Line 868 of glr.c  */
#line 2225 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 30:
/* Line 868 of glr.c  */
#line 2229 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = NULL;
}
    break;

  case 31:
/* Line 868 of glr.c  */
#line 2235 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_ATTRIBUTE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 32:
/* Line 868 of glr.c  */
#line 2239 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_ATTRIBUTE, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 33:
/* Line 868 of glr.c  */
#line 2245 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 34:
/* Line 868 of glr.c  */
#line 2249 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast));
}
    break;

  case 35:
/* Line 868 of glr.c  */
#line 2256 "src/frontend/c99.y"
    {
	AST identif = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);

	((*yyvalp).ast) = ASTMake2(AST_GCC_ATTRIBUTE_EXPR, identif, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 36:
/* Line 868 of glr.c  */
#line 2262 "src/frontend/c99.y"
    {
	AST identif = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);

	((*yyvalp).ast) = ASTMake2(AST_GCC_ATTRIBUTE_EXPR, identif, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 37:
/* Line 868 of glr.c  */
#line 2268 "src/frontend/c99.y"
    {
	AST identif1 = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.token_atrib).token_text);
	
	((*yyvalp).ast) = ASTMake2(AST_GCC_ATTRIBUTE_EXPR, identif1, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 38:
/* Line 868 of glr.c  */
#line 2277 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_ASM_DEFINITION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 39:
/* Line 868 of glr.c  */
#line 2282 "src/frontend/c99.y"
    {
	AST asm_parms = ASTMake4(AST_GCC_ASM_DEF_PARMS, 
			(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (8))].yystate.yysemantics.yysval.ast), NULL, NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.ast)), NULL);
	((*yyvalp).ast) = ASTMake2(AST_GCC_ASM_DEFINITION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (8))].yystate.yysemantics.yysval.ast), asm_parms, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 40:
/* Line 868 of glr.c  */
#line 2288 "src/frontend/c99.y"
    {
	AST asm_parms = ASTMake4(AST_GCC_ASM_DEF_PARMS, 
			(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (10))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (10))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((8) - (10))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (10))].yystate.yysemantics.yysval.ast)), NULL);
	((*yyvalp).ast) = ASTMake2(AST_GCC_ASM_DEFINITION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (10))].yystate.yysemantics.yysval.ast), asm_parms, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (10))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (10))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (10))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (10))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 41:
/* Line 868 of glr.c  */
#line 2294 "src/frontend/c99.y"
    {
	AST asm_parms = ASTMake4(AST_GCC_ASM_DEF_PARMS, 
			(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (12))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (12))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((8) - (12))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((10) - (12))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (12))].yystate.yysemantics.yysval.ast)), NULL);
	((*yyvalp).ast) = ASTMake2(AST_GCC_ASM_DEFINITION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (12))].yystate.yysemantics.yysval.ast), asm_parms, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (12))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (12))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (12))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (12))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 42:
/* Line 868 of glr.c  */
#line 2302 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = NULL;
}
    break;

  case 43:
/* Line 868 of glr.c  */
#line 2306 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_VOLATILE_SPEC, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 44:
/* Line 868 of glr.c  */
#line 2312 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 45:
/* Line 868 of glr.c  */
#line 2316 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = NULL;
}
    break;

  case 46:
/* Line 868 of glr.c  */
#line 2323 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 47:
/* Line 868 of glr.c  */
#line 2327 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast));
}
    break;

  case 48:
/* Line 868 of glr.c  */
#line 2333 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_GCC_ASM_OPERAND, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 49:
/* Line 868 of glr.c  */
#line 2337 "src/frontend/c99.y"
    {
    AST symbol_tree = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.token_atrib).token_text);

	((*yyvalp).ast) = ASTMake3(AST_GCC_ASM_OPERAND, symbol_tree, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (7))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (7))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yyloc).first_column), NULL);
}
    break;

  case 50:
/* Line 868 of glr.c  */
#line 2343 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_GCC_ASM_OPERAND, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 51:
/* Line 868 of glr.c  */
#line 2351 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 52:
/* Line 868 of glr.c  */
#line 2355 "src/frontend/c99.y"
    {
    // This is an error but also a common extension
    ((*yyvalp).ast) = ASTLeaf(AST_EMPTY_DECL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 53:
/* Line 868 of glr.c  */
#line 2362 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_SIMPLE_DECLARATION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 54:
/* Line 868 of glr.c  */
#line 2366 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_SIMPLE_DECLARATION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 55:
/* Line 868 of glr.c  */
#line 2370 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_SIMPLE_DECLARATION, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 56:
/* Line 868 of glr.c  */
#line 2376 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_SIMPLE_DECLARATION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 57:
/* Line 868 of glr.c  */
#line 2382 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 58:
/* Line 868 of glr.c  */
#line 2386 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 59:
/* Line 868 of glr.c  */
#line 2390 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 60:
/* Line 868 of glr.c  */
#line 2394 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 61:
/* Line 868 of glr.c  */
#line 2398 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), NULL, NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 62:
/* Line 868 of glr.c  */
#line 2405 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 63:
/* Line 868 of glr.c  */
#line 2409 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast));
}
    break;

  case 64:
/* Line 868 of glr.c  */
#line 2415 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 65:
/* Line 868 of glr.c  */
#line 2419 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 66:
/* Line 868 of glr.c  */
#line 2423 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 67:
/* Line 868 of glr.c  */
#line 2427 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 68:
/* Line 868 of glr.c  */
#line 2431 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), NULL, NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 69:
/* Line 868 of glr.c  */
#line 2437 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 70:
/* Line 868 of glr.c  */
#line 2441 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 71:
/* Line 868 of glr.c  */
#line 2445 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 72:
/* Line 868 of glr.c  */
#line 2449 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 73:
/* Line 868 of glr.c  */
#line 2453 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), NULL, NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 74:
/* Line 868 of glr.c  */
#line 2465 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 75:
/* Line 868 of glr.c  */
#line 2469 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 76:
/* Line 868 of glr.c  */
#line 2473 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 77:
/* Line 868 of glr.c  */
#line 2477 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 78:
/* Line 868 of glr.c  */
#line 2483 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 79:
/* Line 868 of glr.c  */
#line 2487 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast));
}
    break;

  case 80:
/* Line 868 of glr.c  */
#line 2493 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 81:
/* Line 868 of glr.c  */
#line 2497 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 82:
/* Line 868 of glr.c  */
#line 2503 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 83:
/* Line 868 of glr.c  */
#line 2507 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 84:
/* Line 868 of glr.c  */
#line 2511 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_TYPEDEF_SPEC, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 85:
/* Line 868 of glr.c  */
#line 2516 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 86:
/* Line 868 of glr.c  */
#line 2521 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_SIGNED_TYPE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 87:
/* Line 868 of glr.c  */
#line 2525 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_UNSIGNED_TYPE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 88:
/* Line 868 of glr.c  */
#line 2529 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_LONG_TYPE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 89:
/* Line 868 of glr.c  */
#line 2533 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_SHORT_TYPE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 90:
/* Line 868 of glr.c  */
#line 2538 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_GCC_COMPLEX_TYPE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 91:
/* Line 868 of glr.c  */
#line 2542 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_GCC_IMAGINARY_TYPE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 92:
/* Line 868 of glr.c  */
#line 2548 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_AUTO_STORAGE_SPEC, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 93:
/* Line 868 of glr.c  */
#line 2552 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_REGISTER_SPEC, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 94:
/* Line 868 of glr.c  */
#line 2556 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_STATIC_SPEC, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 95:
/* Line 868 of glr.c  */
#line 2560 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_EXTERN_SPEC, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 96:
/* Line 868 of glr.c  */
#line 2565 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_THREAD_SPEC, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 97:
/* Line 868 of glr.c  */
#line 2571 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_INLINE_SPEC, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 98:
/* Line 868 of glr.c  */
#line 2575 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_NORETURN_SPEC, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 99:
/* Line 868 of glr.c  */
#line 2581 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 100:
/* Line 868 of glr.c  */
#line 2585 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 101:
/* Line 868 of glr.c  */
#line 2591 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 102:
/* Line 868 of glr.c  */
#line 2595 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 103:
/* Line 868 of glr.c  */
#line 2599 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 104:
/* Line 868 of glr.c  */
#line 2603 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 105:
/* Line 868 of glr.c  */
#line 2609 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 106:
/* Line 868 of glr.c  */
#line 2613 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 107:
/* Line 868 of glr.c  */
#line 2617 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 108:
/* Line 868 of glr.c  */
#line 2623 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 109:
/* Line 868 of glr.c  */
#line 2627 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 110:
/* Line 868 of glr.c  */
#line 2631 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 111:
/* Line 868 of glr.c  */
#line 2635 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 112:
/* Line 868 of glr.c  */
#line 2639 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_TYPE_SPECIFIER_SEQ, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), NULL, NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 113:
/* Line 868 of glr.c  */
#line 2645 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_SIMPLE_TYPE_SPEC, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 114:
/* Line 868 of glr.c  */
#line 2649 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 115:
/* Line 868 of glr.c  */
#line 2653 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 116:
/* Line 868 of glr.c  */
#line 2657 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 117:
/* Line 868 of glr.c  */
#line 2664 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_TYPEOF_EXPR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 118:
/* Line 868 of glr.c  */
#line 2668 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_TYPEOF, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 119:
/* Line 868 of glr.c  */
#line 2674 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_ATOMIC_TYPE_SPECIFIER, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 120:
/* Line 868 of glr.c  */
#line 2680 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 121:
/* Line 868 of glr.c  */
#line 2686 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_CHAR_TYPE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 122:
/* Line 868 of glr.c  */
#line 2690 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_BOOL_TYPE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 123:
/* Line 868 of glr.c  */
#line 2694 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_INT_TYPE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 124:
/* Line 868 of glr.c  */
#line 2698 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_FLOAT_TYPE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 125:
/* Line 868 of glr.c  */
#line 2702 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_DOUBLE_TYPE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 126:
/* Line 868 of glr.c  */
#line 2706 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_VOID_TYPE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 127:
/* Line 868 of glr.c  */
#line 2710 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_GCC_INT128, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 128:
/* Line 868 of glr.c  */
#line 2714 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_GCC_FLOAT128, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 129:
/* Line 868 of glr.c  */
#line 2718 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_MCC_BYTE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 130:
/* Line 868 of glr.c  */
#line 2724 "src/frontend/c99.y"
    {
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);

	((*yyvalp).ast) = ASTMake3(AST_ELABORATED_TYPE_CLASS_SPEC, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), identifier, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 131:
/* Line 868 of glr.c  */
#line 2730 "src/frontend/c99.y"
    {
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);

	((*yyvalp).ast) = ASTMake4(AST_ELABORATED_TYPE_ENUM_SPEC, identifier, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 132:
/* Line 868 of glr.c  */
#line 2741 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 133:
/* Line 868 of glr.c  */
#line 2745 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ast_list_concat(ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast));
}
    break;

  case 134:
/* Line 868 of glr.c  */
#line 2751 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 135:
/* Line 868 of glr.c  */
#line 2755 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast));
}
    break;

  case 136:
/* Line 868 of glr.c  */
#line 2761 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_INIT_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 137:
/* Line 868 of glr.c  */
#line 2765 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_INIT_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 138:
/* Line 868 of glr.c  */
#line 2771 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_INIT_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 139:
/* Line 868 of glr.c  */
#line 2775 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_INIT_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 140:
/* Line 868 of glr.c  */
#line 2781 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = NULL;
}
    break;

  case 141:
/* Line 868 of glr.c  */
#line 2785 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 142:
/* Line 868 of glr.c  */
#line 2791 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 143:
/* Line 868 of glr.c  */
#line 2795 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 144:
/* Line 868 of glr.c  */
#line 2799 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ast_list_concat(ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast));
}
    break;

  case 145:
/* Line 868 of glr.c  */
#line 2806 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_ASM_SPEC, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 146:
/* Line 868 of glr.c  */
#line 2813 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 147:
/* Line 868 of glr.c  */
#line 2817 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_POINTER_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 148:
/* Line 868 of glr.c  */
#line 2823 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 149:
/* Line 868 of glr.c  */
#line 2827 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_POINTER_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 150:
/* Line 868 of glr.c  */
#line 2833 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_POINTER_SPEC, NULL, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 151:
/* Line 868 of glr.c  */
#line 2837 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_POINTER_SPEC, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 152:
/* Line 868 of glr.c  */
#line 2841 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_REFERENCE_SPEC, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 153:
/* Line 868 of glr.c  */
#line 2845 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_REBINDABLE_REFERENCE_SPEC, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 154:
/* Line 868 of glr.c  */
#line 2855 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 155:
/* Line 868 of glr.c  */
#line 2859 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_POINTER_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 156:
/* Line 868 of glr.c  */
#line 2865 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 157:
/* Line 868 of glr.c  */
#line 2869 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_POINTER_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 158:
/* Line 868 of glr.c  */
#line 2876 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 159:
/* Line 868 of glr.c  */
#line 2880 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 160:
/* Line 868 of glr.c  */
#line 2886 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake4(AST_PARAMETERS_AND_QUALIFIERS,
        (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), NULL, NULL, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 161:
/* Line 868 of glr.c  */
#line 2891 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake4(AST_PARAMETERS_AND_QUALIFIERS,
        ASTLeaf(AST_EMPTY_PARAMETER_DECLARATION_CLAUSE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL),
        NULL, NULL, NULL,
        make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 162:
/* Line 868 of glr.c  */
#line 2900 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 163:
/* Line 868 of glr.c  */
#line 2904 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_DECLARATOR_FUNC, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 164:
/* Line 868 of glr.c  */
#line 2908 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_DECLARATOR_ARRAY, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), NULL, NULL,  ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 165:
/* Line 868 of glr.c  */
#line 2912 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_DECLARATOR_ARRAY, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.ast),  ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 166:
/* Line 868 of glr.c  */
#line 2916 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_DECLARATOR_ARRAY, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast),  ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 167:
/* Line 868 of glr.c  */
#line 2920 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_PARENTHESIZED_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 168:
/* Line 868 of glr.c  */
#line 2926 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_DECLARATOR_FUNC, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 169:
/* Line 868 of glr.c  */
#line 2932 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 170:
/* Line 868 of glr.c  */
#line 2937 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_PARENTHESIZED_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 171:
/* Line 868 of glr.c  */
#line 2944 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = NULL;
}
    break;

  case 172:
/* Line 868 of glr.c  */
#line 2948 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 173:
/* Line 868 of glr.c  */
#line 2954 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 174:
/* Line 868 of glr.c  */
#line 2961 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = NULL;
}
    break;

  case 175:
/* Line 868 of glr.c  */
#line 2965 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 176:
/* Line 868 of glr.c  */
#line 2971 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_STATIC_SPEC, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 177:
/* Line 868 of glr.c  */
#line 2978 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = NULL;
}
    break;

  case 178:
/* Line 868 of glr.c  */
#line 2982 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_VLA_EXPRESSION, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 179:
/* Line 868 of glr.c  */
#line 2986 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 180:
/* Line 868 of glr.c  */
#line 2992 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 181:
/* Line 868 of glr.c  */
#line 2996 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast));
}
    break;

  case 182:
/* Line 868 of glr.c  */
#line 3002 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_CONST_SPEC, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 183:
/* Line 868 of glr.c  */
#line 3006 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_VOLATILE_SPEC, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 184:
/* Line 868 of glr.c  */
#line 3011 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_GCC_RESTRICT_SPEC, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 185:
/* Line 868 of glr.c  */
#line 3016 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_ATOMIC_TYPE_QUALIFIER, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 186:
/* Line 868 of glr.c  */
#line 3022 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 187:
/* Line 868 of glr.c  */
#line 3026 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_DECLARATOR_FUNC, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 188:
/* Line 868 of glr.c  */
#line 3030 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_DECLARATOR_ARRAY, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), NULL, NULL,  ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 189:
/* Line 868 of glr.c  */
#line 3034 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_DECLARATOR_ARRAY, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.ast),  ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 190:
/* Line 868 of glr.c  */
#line 3038 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_DECLARATOR_ARRAY, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast),  ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 191:
/* Line 868 of glr.c  */
#line 3042 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_PARENTHESIZED_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 192:
/* Line 868 of glr.c  */
#line 3048 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_DECLARATOR_ID_EXPR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 193:
/* Line 868 of glr.c  */
#line 3054 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake4(AST_ENUM_HEAD, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 194:
/* Line 868 of glr.c  */
#line 3060 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 195:
/* Line 868 of glr.c  */
#line 3064 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = NULL;
}
    break;

  case 196:
/* Line 868 of glr.c  */
#line 3070 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_UNSCOPED_ENUM_KEY, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 197:
/* Line 868 of glr.c  */
#line 3076 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_ENUM_SPECIFIER, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 198:
/* Line 868 of glr.c  */
#line 3080 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_ENUM_SPECIFIER, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 199:
/* Line 868 of glr.c  */
#line 3086 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast));
}
    break;

  case 200:
/* Line 868 of glr.c  */
#line 3090 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 201:
/* Line 868 of glr.c  */
#line 3096 "src/frontend/c99.y"
    {
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);

	((*yyvalp).ast) = ASTMake2(AST_ENUMERATOR_DEF, identifier, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 202:
/* Line 868 of glr.c  */
#line 3102 "src/frontend/c99.y"
    {
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);

	((*yyvalp).ast) = ASTMake2(AST_ENUMERATOR_DEF, identifier, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 203:
/* Line 868 of glr.c  */
#line 3110 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_TYPE_ID, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 204:
/* Line 868 of glr.c  */
#line 3114 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_TYPE_ID, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 205:
/* Line 868 of glr.c  */
#line 3120 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_POINTER_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 206:
/* Line 868 of glr.c  */
#line 3124 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_POINTER_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 207:
/* Line 868 of glr.c  */
#line 3128 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 208:
/* Line 868 of glr.c  */
#line 3134 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_POINTER_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), NULL, NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 209:
/* Line 868 of glr.c  */
#line 3138 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_POINTER_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 210:
/* Line 868 of glr.c  */
#line 3142 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 211:
/* Line 868 of glr.c  */
#line 3148 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_PARENTHESIZED_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 212:
/* Line 868 of glr.c  */
#line 3152 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_DECLARATOR_FUNC, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 213:
/* Line 868 of glr.c  */
#line 3156 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_DECLARATOR_FUNC, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 214:
/* Line 868 of glr.c  */
#line 3160 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_DECLARATOR_ARRAY, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), NULL, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 215:
/* Line 868 of glr.c  */
#line 3164 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_DECLARATOR_ARRAY, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_column), NULL);
}
    break;

  case 216:
/* Line 868 of glr.c  */
#line 3168 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_DECLARATOR_ARRAY, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_column), NULL);
}
    break;

  case 217:
/* Line 868 of glr.c  */
#line 3172 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_DECLARATOR_ARRAY, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), NULL, NULL,  ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 218:
/* Line 868 of glr.c  */
#line 3176 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_DECLARATOR_ARRAY, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.ast),  ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 219:
/* Line 868 of glr.c  */
#line 3180 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_DECLARATOR_ARRAY, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast),  ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 220:
/* Line 868 of glr.c  */
#line 3186 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_KR_PARAMETER_LIST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 221:
/* Line 868 of glr.c  */
#line 3192 "src/frontend/c99.y"
    {
    AST symbol = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);

    ((*yyvalp).ast) = ASTListLeaf(symbol);
}
    break;

  case 222:
/* Line 868 of glr.c  */
#line 3198 "src/frontend/c99.y"
    {
    AST symbol = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);

    ((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), symbol);
}
    break;

  case 223:
/* Line 868 of glr.c  */
#line 3206 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 224:
/* Line 868 of glr.c  */
#line 3210 "src/frontend/c99.y"
    {
    AST variadic_arg = ASTLeaf(AST_VARIADIC_ARG, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), variadic_arg);
}
    break;

  case 225:
/* Line 868 of glr.c  */
#line 3217 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 226:
/* Line 868 of glr.c  */
#line 3221 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast));
}
    break;

  case 227:
/* Line 868 of glr.c  */
#line 3227 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_PARAMETER_DECL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 228:
/* Line 868 of glr.c  */
#line 3231 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_PARAMETER_DECL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 229:
/* Line 868 of glr.c  */
#line 3235 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_PARAMETER_DECL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 230:
/* Line 868 of glr.c  */
#line 3241 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_EQUAL_INITIALIZER, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 231:
/* Line 868 of glr.c  */
#line 3247 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 232:
/* Line 868 of glr.c  */
#line 3251 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 233:
/* Line 868 of glr.c  */
#line 3257 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 234:
/* Line 868 of glr.c  */
#line 3261 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast));
}
    break;

  case 235:
/* Line 868 of glr.c  */
#line 3265 "src/frontend/c99.y"
    {
    AST designated_initializer = ASTMake2(AST_DESIGNATED_INITIALIZER, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);

    ((*yyvalp).ast) = ASTListLeaf(designated_initializer);
}
    break;

  case 236:
/* Line 868 of glr.c  */
#line 3271 "src/frontend/c99.y"
    {
    AST designated_initializer = ASTMake2(AST_DESIGNATED_INITIALIZER, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast)), NULL);

    ((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast), designated_initializer);
}
    break;

  case 237:
/* Line 868 of glr.c  */
#line 3278 "src/frontend/c99.y"
    {
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);

	AST gcc_initializer_clause = ASTMake2(AST_GCC_INITIALIZER_CLAUSE, identifier, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);

	((*yyvalp).ast) = ASTListLeaf(gcc_initializer_clause);
}
    break;

  case 238:
/* Line 868 of glr.c  */
#line 3286 "src/frontend/c99.y"
    {
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.token_atrib).token_text);

	AST gcc_initializer_clause = ASTMake2(AST_GCC_INITIALIZER_CLAUSE, identifier, (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast)), NULL);

	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast), gcc_initializer_clause);
}
    break;

  case 239:
/* Line 868 of glr.c  */
#line 3296 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_DESIGNATION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 240:
/* Line 868 of glr.c  */
#line 3302 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 241:
/* Line 868 of glr.c  */
#line 3306 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast));
}
    break;

  case 242:
/* Line 868 of glr.c  */
#line 3312 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_INDEX_DESIGNATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 243:
/* Line 868 of glr.c  */
#line 3316 "src/frontend/c99.y"
    {
    AST symbol = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.token_atrib).token_text);

    ((*yyvalp).ast) = ASTMake1(AST_FIELD_DESIGNATOR, symbol, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 244:
/* Line 868 of glr.c  */
#line 3324 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_FUNCTION_DEFINITION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 245:
/* Line 868 of glr.c  */
#line 3328 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_FUNCTION_DEFINITION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 246:
/* Line 868 of glr.c  */
#line 3334 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_EXTENSION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 247:
/* Line 868 of glr.c  */
#line 3340 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake3(AST_FUNCTION_DEFINITION_HEADER, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 248:
/* Line 868 of glr.c  */
#line 3344 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake3(AST_FUNCTION_DEFINITION_HEADER, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 249:
/* Line 868 of glr.c  */
#line 3350 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 250:
/* Line 868 of glr.c  */
#line 3354 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast));
}
    break;

  case 251:
/* Line 868 of glr.c  */
#line 3360 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_FUNCTION_BODY, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 252:
/* Line 868 of glr.c  */
#line 3370 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_CLASS_SPECIFIER, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 253:
/* Line 868 of glr.c  */
#line 3374 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_CLASS_SPECIFIER, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 254:
/* Line 868 of glr.c  */
#line 3380 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_CLASS_HEAD_SPEC, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), NULL, NULL, NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 255:
/* Line 868 of glr.c  */
#line 3384 "src/frontend/c99.y"
    {
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.token_atrib).token_text);

	((*yyvalp).ast) = ASTMake4(AST_CLASS_HEAD_SPEC, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), identifier, NULL, NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 256:
/* Line 868 of glr.c  */
#line 3391 "src/frontend/c99.y"
    {
    AST class_head_extra = ASTMake3(AST_CLASS_HEAD_EXTRA, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), NULL, NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);

	((*yyvalp).ast) = ASTMake4(AST_CLASS_HEAD_SPEC, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), NULL, NULL, class_head_extra, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 257:
/* Line 868 of glr.c  */
#line 3397 "src/frontend/c99.y"
    {
    AST class_head_extra = ASTMake3(AST_CLASS_HEAD_EXTRA, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), NULL, NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);

	AST identifier = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);
	((*yyvalp).ast) = ASTMake4(AST_CLASS_HEAD_SPEC, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), identifier, NULL, class_head_extra, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 258:
/* Line 868 of glr.c  */
#line 3406 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_CLASS_KEY_STRUCT, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 259:
/* Line 868 of glr.c  */
#line 3410 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_CLASS_KEY_UNION, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 260:
/* Line 868 of glr.c  */
#line 3416 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 261:
/* Line 868 of glr.c  */
#line 3420 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast));
}
    break;

  case 262:
/* Line 868 of glr.c  */
#line 3426 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_MEMBER_DECLARATION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 263:
/* Line 868 of glr.c  */
#line 3430 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_MEMBER_DECLARATION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 264:
/* Line 868 of glr.c  */
#line 3435 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_EMPTY_DECL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 265:
/* Line 868 of glr.c  */
#line 3440 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_EXTENSION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 266:
/* Line 868 of glr.c  */
#line 3446 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 267:
/* Line 868 of glr.c  */
#line 3450 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ast_list_concat(ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast));
}
    break;

  case 268:
/* Line 868 of glr.c  */
#line 3456 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 269:
/* Line 868 of glr.c  */
#line 3460 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast));
}
    break;

  case 270:
/* Line 868 of glr.c  */
#line 3466 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake3(AST_MEMBER_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 271:
/* Line 868 of glr.c  */
#line 3470 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake3(AST_MEMBER_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 272:
/* Line 868 of glr.c  */
#line 3474 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake3(AST_BITFIELD_DECLARATOR, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 273:
/* Line 868 of glr.c  */
#line 3478 "src/frontend/c99.y"
    {
    AST identifier = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.token_atrib).token_text);
    AST declarator_id_expr = ASTMake1(AST_DECLARATOR_ID_EXPR, identifier, ast_get_locus(identifier), NULL);

    ((*yyvalp).ast) = ASTMake3(AST_BITFIELD_DECLARATOR, declarator_id_expr, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 274:
/* Line 868 of glr.c  */
#line 3487 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_MEMBER_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 275:
/* Line 868 of glr.c  */
#line 3491 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_MEMBER_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 276:
/* Line 868 of glr.c  */
#line 3495 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_BITFIELD_DECLARATOR, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 277:
/* Line 868 of glr.c  */
#line 3499 "src/frontend/c99.y"
    {
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.token_atrib).token_text);
	AST declarator_id_expr = ASTMake1(AST_DECLARATOR_ID_EXPR, identifier, ast_get_locus(identifier), NULL);

	((*yyvalp).ast) = ASTMake3(AST_BITFIELD_DECLARATOR, declarator_id_expr, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 278:
/* Line 868 of glr.c  */
#line 3508 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast);
}
    break;

  case 279:
/* Line 868 of glr.c  */
#line 3518 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 280:
/* Line 868 of glr.c  */
#line 3522 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 281:
/* Line 868 of glr.c  */
#line 3528 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 282:
/* Line 868 of glr.c  */
#line 3532 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 283:
/* Line 868 of glr.c  */
#line 3536 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 284:
/* Line 868 of glr.c  */
#line 3540 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 285:
/* Line 868 of glr.c  */
#line 3544 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 286:
/* Line 868 of glr.c  */
#line 3548 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 287:
/* Line 868 of glr.c  */
#line 3552 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 288:
/* Line 868 of glr.c  */
#line 3558 "src/frontend/c99.y"
    {
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);
	
	((*yyvalp).ast) = ASTMake2(AST_LABELED_STATEMENT, identifier, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 289:
/* Line 868 of glr.c  */
#line 3564 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_CASE_STATEMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 290:
/* Line 868 of glr.c  */
#line 3568 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_DEFAULT_STATEMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 291:
/* Line 868 of glr.c  */
#line 3573 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_GCC_CASE_STATEMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_column), NULL);
}
    break;

  case 292:
/* Line 868 of glr.c  */
#line 3579 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_EXPRESSION_STATEMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 293:
/* Line 868 of glr.c  */
#line 3583 "src/frontend/c99.y"
    {
	// Empty statement ...
	((*yyvalp).ast) = ASTLeaf(AST_EMPTY_STATEMENT, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 294:
/* Line 868 of glr.c  */
#line 3590 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_DECLARATION_STATEMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 295:
/* Line 868 of glr.c  */
#line 3596 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_COMPOUND_STATEMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 296:
/* Line 868 of glr.c  */
#line 3600 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_COMPOUND_STATEMENT, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 297:
/* Line 868 of glr.c  */
#line 3606 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 298:
/* Line 868 of glr.c  */
#line 3610 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast));
}
    break;

  case 299:
/* Line 868 of glr.c  */
#line 3625 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_IF_ELSE_STATEMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast), NULL, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_column), NULL);
}
    break;

  case 300:
/* Line 868 of glr.c  */
#line 3629 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_IF_ELSE_STATEMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (7))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (7))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (7))].yystate.yysemantics.yysval.ast), NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yyloc).first_column), NULL);
}
    break;

  case 301:
/* Line 868 of glr.c  */
#line 3635 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_SWITCH_STATEMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast), NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_column), NULL);
}
    break;

  case 302:
/* Line 868 of glr.c  */
#line 3641 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_CONDITION, NULL, NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 303:
/* Line 868 of glr.c  */
#line 3647 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_WHILE_STATEMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_column), NULL);
}
    break;

  case 304:
/* Line 868 of glr.c  */
#line 3651 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_DO_STATEMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (7))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (7))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yyloc).first_column), NULL);
}
    break;

  case 305:
/* Line 868 of glr.c  */
#line 3655 "src/frontend/c99.y"
    {
    AST loop_control = ASTMake3(AST_LOOP_CONTROL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (8))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (8))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (8))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yyloc).first_column), NULL);
	((*yyvalp).ast) = ASTMake3(AST_FOR_STATEMENT, loop_control, (((yyGLRStackItem const *)yyvsp)[YYFILL ((8) - (8))].yystate.yysemantics.yysval.ast), NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yyloc).first_column), NULL);
}
    break;

  case 306:
/* Line 868 of glr.c  */
#line 3662 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 307:
/* Line 868 of glr.c  */
#line 3666 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = NULL;
}
    break;

  case 308:
/* Line 868 of glr.c  */
#line 3672 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 309:
/* Line 868 of glr.c  */
#line 3676 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = NULL;
}
    break;

  case 310:
/* Line 868 of glr.c  */
#line 3683 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 311:
/* Line 868 of glr.c  */
#line 3687 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 312:
/* Line 868 of glr.c  */
#line 3693 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_BREAK_STATEMENT, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 313:
/* Line 868 of glr.c  */
#line 3697 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_CONTINUE_STATEMENT, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 314:
/* Line 868 of glr.c  */
#line 3701 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_RETURN_STATEMENT, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 315:
/* Line 868 of glr.c  */
#line 3705 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_RETURN_STATEMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 316:
/* Line 868 of glr.c  */
#line 3709 "src/frontend/c99.y"
    {
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);
	
	((*yyvalp).ast) = ASTMake1(AST_GOTO_STATEMENT, identifier, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 317:
/* Line 868 of glr.c  */
#line 3716 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_GOTO_STATEMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 318:
/* Line 868 of glr.c  */
#line 3726 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 319:
/* Line 868 of glr.c  */
#line 3730 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_PARENTHESIZED_EXPRESSION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 320:
/* Line 868 of glr.c  */
#line 3734 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 321:
/* Line 868 of glr.c  */
#line 3739 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 322:
/* Line 868 of glr.c  */
#line 3753 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_PARENTHESIZED_EXPRESSION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 323:
/* Line 868 of glr.c  */
#line 3757 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_GCC_BUILTIN_VA_ARG, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_column), NULL);
}
    break;

  case 324:
/* Line 868 of glr.c  */
#line 3761 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake2(AST_GCC_BUILTIN_OFFSETOF, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_column), NULL);
}
    break;

  case 325:
/* Line 868 of glr.c  */
#line 3765 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake3(AST_GCC_BUILTIN_CHOOSE_EXPR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (8))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yyloc).first_column), NULL);
}
    break;

  case 326:
/* Line 868 of glr.c  */
#line 3769 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake2(AST_GCC_BUILTIN_TYPES_COMPATIBLE_P, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_column), NULL);
}
    break;

  case 327:
/* Line 868 of glr.c  */
#line 3775 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake2(AST_GENERIC_SELECTION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_column), NULL);
}
    break;

  case 328:
/* Line 868 of glr.c  */
#line 3781 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 329:
/* Line 868 of glr.c  */
#line 3785 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast));
}
    break;

  case 330:
/* Line 868 of glr.c  */
#line 3791 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake2(AST_GENERIC_ASSOCIATION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 331:
/* Line 868 of glr.c  */
#line 3795 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_GENERIC_ASSOCIATION_DEFAULT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast),
        make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 332:
/* Line 868 of glr.c  */
#line 3809 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake2(AST_GCC_OFFSETOF_MEMBER_DESIGNATOR,
            ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.token_atrib).token_text),
            (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 333:
/* Line 868 of glr.c  */
#line 3815 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake2(AST_GCC_OFFSETOF_MEMBER_DESIGNATOR,
            ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text),
            NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 334:
/* Line 868 of glr.c  */
#line 3823 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 335:
/* Line 868 of glr.c  */
#line 3829 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 336:
/* Line 868 of glr.c  */
#line 3835 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 337:
/* Line 868 of glr.c  */
#line 3839 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_ARRAY_SUBSCRIPT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 338:
/* Line 868 of glr.c  */
#line 3843 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_FUNCTION_CALL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 339:
/* Line 868 of glr.c  */
#line 3847 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_FUNCTION_CALL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 340:
/* Line 868 of glr.c  */
#line 3851 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_CLASS_MEMBER_ACCESS, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 341:
/* Line 868 of glr.c  */
#line 3855 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_POINTER_CLASS_MEMBER_ACCESS, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 342:
/* Line 868 of glr.c  */
#line 3859 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_POSTINCREMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 343:
/* Line 868 of glr.c  */
#line 3863 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_POSTDECREMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 344:
/* Line 868 of glr.c  */
#line 3868 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_GCC_POSTFIX_EXPRESSION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 345:
/* Line 868 of glr.c  */
#line 3874 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_INITIALIZER_BRACES, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 346:
/* Line 868 of glr.c  */
#line 3878 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_INITIALIZER_BRACES, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 347:
/* Line 868 of glr.c  */
#line 3882 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_INITIALIZER_BRACES, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 348:
/* Line 868 of glr.c  */
#line 3888 "src/frontend/c99.y"
    {
    AST expression_holder = ASTMake1(AST_EXPRESSION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
	((*yyvalp).ast) = ASTListLeaf(expression_holder);
}
    break;

  case 349:
/* Line 868 of glr.c  */
#line 3893 "src/frontend/c99.y"
    {
    AST expression_holder = ASTMake1(AST_EXPRESSION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast)), NULL);
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), expression_holder);
}
    break;

  case 350:
/* Line 868 of glr.c  */
#line 3900 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 351:
/* Line 868 of glr.c  */
#line 3904 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_PREINCREMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 352:
/* Line 868 of glr.c  */
#line 3908 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_PREDECREMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 353:
/* Line 868 of glr.c  */
#line 3912 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.node_type), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 354:
/* Line 868 of glr.c  */
#line 3916 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_SIZEOF, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 355:
/* Line 868 of glr.c  */
#line 3920 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_SIZEOF_TYPEID, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 356:
/* Line 868 of glr.c  */
#line 3925 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_EXTENSION_EXPR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 357:
/* Line 868 of glr.c  */
#line 3929 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_ALIGNOF, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 358:
/* Line 868 of glr.c  */
#line 3933 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_ALIGNOF_TYPE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 359:
/* Line 868 of glr.c  */
#line 3937 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_REAL_PART, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 360:
/* Line 868 of glr.c  */
#line 3941 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_GCC_IMAG_PART, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 361:
/* Line 868 of glr.c  */
#line 3945 "src/frontend/c99.y"
    {
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.token_atrib).token_text);

	((*yyvalp).ast) = ASTMake1(AST_GCC_LABEL_ADDR, identifier, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 362:
/* Line 868 of glr.c  */
#line 3953 "src/frontend/c99.y"
    {
	((*yyvalp).node_type) = AST_DERREFERENCE;
}
    break;

  case 363:
/* Line 868 of glr.c  */
#line 3957 "src/frontend/c99.y"
    {
	((*yyvalp).node_type) = AST_REFERENCE;
}
    break;

  case 364:
/* Line 868 of glr.c  */
#line 3961 "src/frontend/c99.y"
    {
	((*yyvalp).node_type) = AST_PLUS;
}
    break;

  case 365:
/* Line 868 of glr.c  */
#line 3965 "src/frontend/c99.y"
    {
	((*yyvalp).node_type) = AST_NEG;
}
    break;

  case 366:
/* Line 868 of glr.c  */
#line 3969 "src/frontend/c99.y"
    {
	((*yyvalp).node_type) = AST_LOGICAL_NOT;
}
    break;

  case 367:
/* Line 868 of glr.c  */
#line 3973 "src/frontend/c99.y"
    {
	((*yyvalp).node_type) = AST_BITWISE_NOT;
}
    break;

  case 368:
/* Line 868 of glr.c  */
#line 3979 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 369:
/* Line 868 of glr.c  */
#line 3983 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_CAST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast), ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast)), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 370:
/* Line 868 of glr.c  */
#line 3989 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 371:
/* Line 868 of glr.c  */
#line 3993 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_MUL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 372:
/* Line 868 of glr.c  */
#line 3997 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_DIV, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 373:
/* Line 868 of glr.c  */
#line 4001 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_MOD, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 374:
/* Line 868 of glr.c  */
#line 4007 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 375:
/* Line 868 of glr.c  */
#line 4011 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_ADD, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 376:
/* Line 868 of glr.c  */
#line 4015 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_MINUS, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 377:
/* Line 868 of glr.c  */
#line 4021 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 378:
/* Line 868 of glr.c  */
#line 4025 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_BITWISE_SHL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 379:
/* Line 868 of glr.c  */
#line 4029 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_SHR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 380:
/* Line 868 of glr.c  */
#line 4035 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 381:
/* Line 868 of glr.c  */
#line 4039 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_LOWER_THAN, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 382:
/* Line 868 of glr.c  */
#line 4043 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_GREATER_THAN, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 383:
/* Line 868 of glr.c  */
#line 4047 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_GREATER_OR_EQUAL_THAN, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 384:
/* Line 868 of glr.c  */
#line 4051 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_LOWER_OR_EQUAL_THAN, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 385:
/* Line 868 of glr.c  */
#line 4057 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 386:
/* Line 868 of glr.c  */
#line 4061 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_EQUAL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 387:
/* Line 868 of glr.c  */
#line 4065 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_DIFFERENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 388:
/* Line 868 of glr.c  */
#line 4071 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 389:
/* Line 868 of glr.c  */
#line 4075 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_BITWISE_AND, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 390:
/* Line 868 of glr.c  */
#line 4081 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 391:
/* Line 868 of glr.c  */
#line 4085 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_BITWISE_XOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 392:
/* Line 868 of glr.c  */
#line 4091 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 393:
/* Line 868 of glr.c  */
#line 4095 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_BITWISE_OR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 394:
/* Line 868 of glr.c  */
#line 4101 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 395:
/* Line 868 of glr.c  */
#line 4105 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_LOGICAL_AND, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 396:
/* Line 868 of glr.c  */
#line 4111 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 397:
/* Line 868 of glr.c  */
#line 4115 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_LOGICAL_OR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 398:
/* Line 868 of glr.c  */
#line 4121 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 399:
/* Line 868 of glr.c  */
#line 4125 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_CONDITIONAL_EXPRESSION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 400:
/* Line 868 of glr.c  */
#line 4130 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_GCC_CONDITIONAL_EXPRESSION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 401:
/* Line 868 of glr.c  */
#line 4136 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 402:
/* Line 868 of glr.c  */
#line 4140 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.node_type), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 403:
/* Line 868 of glr.c  */
#line 4146 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_EXPRESSION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 404:
/* Line 868 of glr.c  */
#line 4150 "src/frontend/c99.y"
    {
	AST comma_expression = ASTMake2(AST_COMMA, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);

	((*yyvalp).ast) = ASTMake1(AST_EXPRESSION, comma_expression, ast_get_locus(comma_expression), NULL);
}
    break;

  case 405:
/* Line 868 of glr.c  */
#line 4158 "src/frontend/c99.y"
    {
	((*yyvalp).node_type) = AST_ASSIGNMENT;
}
    break;

  case 406:
/* Line 868 of glr.c  */
#line 4162 "src/frontend/c99.y"
    {
	((*yyvalp).node_type) = AST_MUL_ASSIGNMENT;
}
    break;

  case 407:
/* Line 868 of glr.c  */
#line 4166 "src/frontend/c99.y"
    {
	((*yyvalp).node_type) = AST_DIV_ASSIGNMENT;
}
    break;

  case 408:
/* Line 868 of glr.c  */
#line 4170 "src/frontend/c99.y"
    {
	((*yyvalp).node_type) = AST_ADD_ASSIGNMENT;
}
    break;

  case 409:
/* Line 868 of glr.c  */
#line 4174 "src/frontend/c99.y"
    {
	((*yyvalp).node_type) = AST_SUB_ASSIGNMENT;
}
    break;

  case 410:
/* Line 868 of glr.c  */
#line 4178 "src/frontend/c99.y"
    {
    ((*yyvalp).node_type) = AST_BITWISE_SHL_ASSIGNMENT;
}
    break;

  case 411:
/* Line 868 of glr.c  */
#line 4182 "src/frontend/c99.y"
    {
	((*yyvalp).node_type) = AST_SHR_ASSIGNMENT;
}
    break;

  case 412:
/* Line 868 of glr.c  */
#line 4186 "src/frontend/c99.y"
    {
	((*yyvalp).node_type) = AST_BITWISE_AND_ASSIGNMENT;
}
    break;

  case 413:
/* Line 868 of glr.c  */
#line 4190 "src/frontend/c99.y"
    {
	((*yyvalp).node_type) = AST_BITWISE_OR_ASSIGNMENT;
}
    break;

  case 414:
/* Line 868 of glr.c  */
#line 4194 "src/frontend/c99.y"
    {
	((*yyvalp).node_type) = AST_BITWISE_XOR_ASSIGNMENT;
}
    break;

  case 415:
/* Line 868 of glr.c  */
#line 4198 "src/frontend/c99.y"
    {
	((*yyvalp).node_type) = AST_MOD_ASSIGNMENT;
}
    break;

  case 416:
/* Line 868 of glr.c  */
#line 4204 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_CONSTANT_EXPRESSION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 417:
/* Line 868 of glr.c  */
#line 4214 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_DECIMAL_LITERAL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 418:
/* Line 868 of glr.c  */
#line 4218 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_OCTAL_LITERAL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 419:
/* Line 868 of glr.c  */
#line 4222 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_BINARY_LITERAL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 420:
/* Line 868 of glr.c  */
#line 4226 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_HEXADECIMAL_LITERAL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 421:
/* Line 868 of glr.c  */
#line 4230 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_HEXADECIMAL_FLOAT, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 422:
/* Line 868 of glr.c  */
#line 4234 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_FLOATING_LITERAL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 423:
/* Line 868 of glr.c  */
#line 4238 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_BOOLEAN_LITERAL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 424:
/* Line 868 of glr.c  */
#line 4242 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_CHARACTER_LITERAL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 425:
/* Line 868 of glr.c  */
#line 4246 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 426:
/* Line 868 of glr.c  */
#line 4253 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_STRING_LITERAL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 427:
/* Line 868 of glr.c  */
#line 4257 "src/frontend/c99.y"
    {
	// Let's concatenate here, it will ease everything

	const char* str1 = ASTText((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast));
	const char* str2 = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.token_atrib).token_text;
	char* text = NEW_VEC0(char, strlen(str1) + strlen(str2) + 1);

	strcat(text, str1);

	// Append the second string
	strcat(text, str2);

	((*yyvalp).ast) = ASTLeaf(AST_STRING_LITERAL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), text);
}
    break;

  case 428:
/* Line 868 of glr.c  */
#line 4274 "src/frontend/c99.y"
    {
    ((*yyvalp).token_atrib) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib);
}
    break;

  case 429:
/* Line 868 of glr.c  */
#line 4300 "src/frontend/c99.y"
    {
	*parsed_tree = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 430:
/* Line 868 of glr.c  */
#line 4306 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast);
}
    break;

  case 431:
/* Line 868 of glr.c  */
#line 4310 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast);
}
    break;

  case 432:
/* Line 868 of glr.c  */
#line 4314 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = NULL;
}
    break;

  case 433:
/* Line 868 of glr.c  */
#line 4318 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast);
}
    break;

  case 434:
/* Line 868 of glr.c  */
#line 4322 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast);
}
    break;

  case 435:
/* Line 868 of glr.c  */
#line 4326 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = NULL;
}
    break;

  case 436:
/* Line 868 of glr.c  */
#line 4330 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast);
}
    break;

  case 437:
/* Line 868 of glr.c  */
#line 4334 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast);
}
    break;

  case 438:
/* Line 868 of glr.c  */
#line 4338 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast);
}
    break;

  case 439:
/* Line 868 of glr.c  */
#line 4342 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast);
}
    break;

  case 440:
/* Line 868 of glr.c  */
#line 4369 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 441:
/* Line 868 of glr.c  */
#line 4373 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast));
}
    break;

  case 442:
/* Line 868 of glr.c  */
#line 4401 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_NODECL_LITERAL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 443:
/* Line 868 of glr.c  */
#line 4412 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_NODECL_LITERAL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 444:
/* Line 868 of glr.c  */
#line 4418 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast));
}
    break;

  case 445:
/* Line 868 of glr.c  */
#line 4422 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 446:
/* Line 868 of glr.c  */
#line 4428 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_SYMBOL_LITERAL_REF, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 447:
/* Line 868 of glr.c  */
#line 4434 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_TYPE_LITERAL_REF, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 448:
/* Line 868 of glr.c  */
#line 4445 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_STRING_LITERAL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 449:
/* Line 868 of glr.c  */
#line 4453 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 450:
/* Line 868 of glr.c  */
#line 4459 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 451:
/* Line 868 of glr.c  */
#line 4473 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 452:
/* Line 868 of glr.c  */
#line 4484 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 453:
/* Line 868 of glr.c  */
#line 4529 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake2(AST_MCC_ARRAY_SUBSCRIPT_CHECK, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_column), NULL);
}
    break;

  case 454:
/* Line 868 of glr.c  */
#line 4533 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_MCC_CONSTANT_VALUE_CHECK, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 455:
/* Line 868 of glr.c  */
#line 4539 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake4(AST_ARRAY_SECTION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 456:
/* Line 868 of glr.c  */
#line 4543 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake4(AST_ARRAY_SECTION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (8))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 457:
/* Line 868 of glr.c  */
#line 4547 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake4(AST_ARRAY_SECTION_SIZE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 458:
/* Line 868 of glr.c  */
#line 4551 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake4(AST_ARRAY_SECTION_SIZE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (8))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (8))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (8))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (8))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 459:
/* Line 868 of glr.c  */
#line 4557 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 460:
/* Line 868 of glr.c  */
#line 4561 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_CAST, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast), ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast)), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 461:
/* Line 868 of glr.c  */
#line 4567 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake2(AST_SHAPING_EXPRESSION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 462:
/* Line 868 of glr.c  */
#line 4573 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast));
}
    break;

  case 463:
/* Line 868 of glr.c  */
#line 4577 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 464:
/* Line 868 of glr.c  */
#line 4583 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast);
}
    break;

  case 465:
/* Line 868 of glr.c  */
#line 4589 "src/frontend/c99.y"
    {
    AST loop_control = ASTMake3(AST_LOOP_CONTROL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (11))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (11))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((9) - (11))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (11))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (11))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (11))].yystate.yyloc).first_column), NULL);
	((*yyvalp).ast) = ASTMake4(AST_FOR_STATEMENT, loop_control, (((yyGLRStackItem const *)yyvsp)[YYFILL ((11) - (11))].yystate.yysemantics.yysval.ast), NULL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (11))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (11))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (11))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (11))].yystate.yyloc).first_column), NULL);

}
    break;

  case 466:
/* Line 868 of glr.c  */
#line 4598 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 467:
/* Line 868 of glr.c  */
#line 4613 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_MCC_BOOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 468:
/* Line 868 of glr.c  */
#line 4617 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_MCC_MASK, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 470:
/* Line 868 of glr.c  */
#line 4625 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast);
}
    break;

  case 471:
/* Line 868 of glr.c  */
#line 4631 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake2(AST_MULTIEXPRESSION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 472:
/* Line 868 of glr.c  */
#line 4638 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 473:
/* Line 868 of glr.c  */
#line 4642 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast));
}
    break;

  case 474:
/* Line 868 of glr.c  */
#line 4649 "src/frontend/c99.y"
    {
    AST symbol = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);
    ((*yyvalp).ast) = ASTMake2(AST_MULTIEXPRESSION_ITERATOR, symbol, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus(symbol), NULL);
}
    break;

  case 475:
/* Line 868 of glr.c  */
#line 4656 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 476:
/* Line 868 of glr.c  */
#line 4660 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 477:
/* Line 868 of glr.c  */
#line 4664 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 478:
/* Line 868 of glr.c  */
#line 4670 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake3(AST_MULTIEXPRESSION_RANGE_SECTION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 479:
/* Line 868 of glr.c  */
#line 4674 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake3(AST_MULTIEXPRESSION_RANGE_SECTION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 480:
/* Line 868 of glr.c  */
#line 4680 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake3(AST_MULTIEXPRESSION_RANGE_SIZE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 481:
/* Line 868 of glr.c  */
#line 4684 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake3(AST_MULTIEXPRESSION_RANGE_SIZE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 482:
/* Line 868 of glr.c  */
#line 4690 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_MULTIEXPRESSION_RANGE_DISCRETE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 483:
/* Line 868 of glr.c  */
#line 4707 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 484:
/* Line 868 of glr.c  */
#line 4713 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_FORTRAN_ALLOCATE_STATEMENT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_column), NULL);
}
    break;

  case 485:
/* Line 868 of glr.c  */
#line 4734 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 486:
/* Line 868 of glr.c  */
#line 4742 "src/frontend/c99.y"
    {
    // This is an empty statement
    ((*yyvalp).ast) = ASTLeaf(AST_STATEMENT_PLACEHOLDER, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 487:
/* Line 868 of glr.c  */
#line 4834 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_UNKNOWN_PRAGMA, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 488:
/* Line 868 of glr.c  */
#line 4840 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 489:
/* Line 868 of glr.c  */
#line 4846 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 490:
/* Line 868 of glr.c  */
#line 4876 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 491:
/* Line 868 of glr.c  */
#line 4880 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 492:
/* Line 868 of glr.c  */
#line 4886 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 493:
/* Line 868 of glr.c  */
#line 4890 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 494:
/* Line 868 of glr.c  */
#line 4896 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 495:
/* Line 868 of glr.c  */
#line 4900 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 496:
/* Line 868 of glr.c  */
#line 4938 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_PRAGMA_CUSTOM_DIRECTIVE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 497:
/* Line 868 of glr.c  */
#line 4945 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_PRAGMA_CUSTOM_CONSTRUCT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 498:
/* Line 868 of glr.c  */
#line 4951 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_PRAGMA_CUSTOM_CONSTRUCT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 499:
/* Line 868 of glr.c  */
#line 4957 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake3(AST_PRAGMA_CUSTOM_CONSTRUCT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 500:
/* Line 868 of glr.c  */
#line 5087 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_PRAGMA_CUSTOM_LINE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 501:
/* Line 868 of glr.c  */
#line 5091 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_PRAGMA_CUSTOM_LINE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 502:
/* Line 868 of glr.c  */
#line 5095 "src/frontend/c99.y"
    {
    // This is a degenerated case caused by wrong designed pragmas
    ((*yyvalp).ast) = ASTMake2(AST_PRAGMA_CUSTOM_LINE, NULL, NULL, make_locus("", 0, 0), NULL);
}
    break;

  case 503:
/* Line 868 of glr.c  */
#line 5102 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_PRAGMA_CUSTOM_LINE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 504:
/* Line 868 of glr.c  */
#line 5106 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_PRAGMA_CUSTOM_LINE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 505:
/* Line 868 of glr.c  */
#line 5112 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = NULL;
}
    break;

  case 506:
/* Line 868 of glr.c  */
#line 5116 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 507:
/* Line 868 of glr.c  */
#line 5122 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 508:
/* Line 868 of glr.c  */
#line 5126 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast));
}
    break;

  case 509:
/* Line 868 of glr.c  */
#line 5130 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast));
}
    break;

  case 510:
/* Line 868 of glr.c  */
#line 5136 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_PRAGMA_CUSTOM_CLAUSE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 511:
/* Line 868 of glr.c  */
#line 5140 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_PRAGMA_CUSTOM_CLAUSE, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 512:
/* Line 868 of glr.c  */
#line 5144 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_PRAGMA_CUSTOM_CLAUSE, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 513:
/* Line 868 of glr.c  */
#line 5150 "src/frontend/c99.y"
    {
    AST node = ASTLeaf(AST_PRAGMA_CLAUSE_ARG, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);

    ((*yyvalp).ast) = ASTListLeaf(node);
}
    break;

  case 514:
/* Line 868 of glr.c  */
#line 5158 "src/frontend/c99.y"
    {
    ((*yyvalp).token_atrib) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib);
}
    break;

  case 515:
/* Line 868 of glr.c  */
#line 5162 "src/frontend/c99.y"
    {
    ((*yyvalp).token_atrib).token_text = strappend((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.token_atrib).token_text, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 516:
/* Line 868 of glr.c  */
#line 5168 "src/frontend/c99.y"
    {
    ((*yyvalp).token_atrib) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib);
}
    break;

  case 517:
/* Line 868 of glr.c  */
#line 5174 "src/frontend/c99.y"
    {
    ((*yyvalp).token_atrib) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib);
}
    break;

  case 518:
/* Line 868 of glr.c  */
#line 5182 "src/frontend/c99.y"
    {
    AST ident = ASTLeaf(AST_SYMBOL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.token_atrib).token_text);

    ((*yyvalp).ast) = ASTMake1(AST_VERBATIM, ident, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 519:
/* Line 868 of glr.c  */
#line 5188 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_VERBATIM, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 520:
/* Line 868 of glr.c  */
#line 5194 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 521:
/* Line 868 of glr.c  */
#line 5200 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 522:
/* Line 868 of glr.c  */
#line 5229 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast);
}
    break;

  case 523:
/* Line 868 of glr.c  */
#line 5233 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast);
}
    break;

  case 524:
/* Line 868 of glr.c  */
#line 5239 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake4(AST_OMP_DECLARE_REDUCTION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 525:
/* Line 868 of glr.c  */
#line 5243 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake4(AST_OMP_DECLARE_REDUCTION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (7))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (7))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (7))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 526:
/* Line 868 of glr.c  */
#line 5249 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_OMP_DR_OPERATOR, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 527:
/* Line 868 of glr.c  */
#line 5253 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 528:
/* Line 868 of glr.c  */
#line 5259 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_OMP_DR_IDENTIFIER, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 529:
/* Line 868 of glr.c  */
#line 5265 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 530:
/* Line 868 of glr.c  */
#line 5269 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast));
}
    break;

  case 539:
/* Line 868 of glr.c  */
#line 5286 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 540:
/* Line 868 of glr.c  */
#line 5305 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 541:
/* Line 868 of glr.c  */
#line 5312 "src/frontend/c99.y"
    {
    AST declarator_id = ASTMake1(AST_DECLARATOR_ID_EXPR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
    AST declarator = ASTMake1(AST_DECLARATOR, declarator_id, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);

    ((*yyvalp).ast) = ASTMake2(AST_INIT_DECLARATOR, declarator, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 542:
/* Line 868 of glr.c  */
#line 5319 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_FUNCTION_CALL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 543:
/* Line 868 of glr.c  */
#line 5323 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_FUNCTION_CALL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 544:
/* Line 868 of glr.c  */
#line 5350 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 545:
/* Line 868 of glr.c  */
#line 5354 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = NULL;
}
    break;

  case 546:
/* Line 868 of glr.c  */
#line 5360 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 547:
/* Line 868 of glr.c  */
#line 5364 "src/frontend/c99.y"
    {
    // Note that this is to be interpreted as a [lower:size] (not [lower:upper]),
    // so we create an AST_ARRAY_SECTION_SIZE here
    ((*yyvalp).ast) = ASTMake4(AST_ARRAY_SECTION_SIZE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 548:
/* Line 868 of glr.c  */
#line 5370 "src/frontend/c99.y"
    {
    // Note that this is to be interpreted as a [lower:size] (not [lower:upper]),
    // so we create an AST_ARRAY_SECTION_SIZE here
    ((*yyvalp).ast) = ASTMake2(AST_ARRAY_SUBSCRIPT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 549:
/* Line 868 of glr.c  */
#line 5389 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast);
}
    break;

  case 550:
/* Line 868 of glr.c  */
#line 5395 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 551:
/* Line 868 of glr.c  */
#line 5399 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 552:
/* Line 868 of glr.c  */
#line 5405 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast);
}
    break;

  case 553:
/* Line 868 of glr.c  */
#line 5428 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_SUPERSCALAR_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 554:
/* Line 868 of glr.c  */
#line 5432 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast);
}
    break;

  case 555:
/* Line 868 of glr.c  */
#line 5436 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_SUPERSCALAR_EXPRESSION, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 556:
/* Line 868 of glr.c  */
#line 5442 "src/frontend/c99.y"
    {
	AST ss_decl = ASTMake2(AST_SUPERSCALAR_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), NULL);
    ((*yyvalp).ast) = ASTListLeaf(ss_decl);
}
    break;

  case 557:
/* Line 868 of glr.c  */
#line 5447 "src/frontend/c99.y"
    {
	AST ss_decl = ASTMake2(AST_SUPERSCALAR_DECLARATOR, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (4))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast)), NULL);
    ((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast), ss_decl);
}
    break;

  case 558:
/* Line 868 of glr.c  */
#line 5454 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 559:
/* Line 868 of glr.c  */
#line 5458 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake4(AST_DECLARATOR_ARRAY, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), NULL, NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 560:
/* Line 868 of glr.c  */
#line 5465 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = NULL;
}
    break;

  case 562:
/* Line 868 of glr.c  */
#line 5472 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 563:
/* Line 868 of glr.c  */
#line 5476 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast));
}
    break;

  case 564:
/* Line 868 of glr.c  */
#line 5482 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTLeaf(AST_SUPERSCALAR_REGION_SPEC_FULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 565:
/* Line 868 of glr.c  */
#line 5486 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake1(AST_SUPERSCALAR_REGION_SPEC_SINGLE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 566:
/* Line 868 of glr.c  */
#line 5490 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_SUPERSCALAR_REGION_SPEC_RANGE, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_column), NULL);
}
    break;

  case 567:
/* Line 868 of glr.c  */
#line 5494 "src/frontend/c99.y"
    {
	((*yyvalp).ast) = ASTMake2(AST_SUPERSCALAR_REGION_SPEC_LENGTH, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yyloc).first_column), NULL);
}
    break;

  case 568:
/* Line 868 of glr.c  */
#line 5529 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_UPC_LOCALSIZEOF, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 569:
/* Line 868 of glr.c  */
#line 5533 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_UPC_LOCALSIZEOF_TYPEID, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 570:
/* Line 868 of glr.c  */
#line 5537 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_UPC_BLOCKSIZEOF, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 571:
/* Line 868 of glr.c  */
#line 5541 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_UPC_BLOCKSIZEOF_TYPEID, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 572:
/* Line 868 of glr.c  */
#line 5545 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_UPC_ELEMSIZEOF, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 573:
/* Line 868 of glr.c  */
#line 5549 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_UPC_ELEMSIZEOF_TYPEID, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 574:
/* Line 868 of glr.c  */
#line 5555 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 575:
/* Line 868 of glr.c  */
#line 5559 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 576:
/* Line 868 of glr.c  */
#line 5565 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_UPC_SHARED, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 577:
/* Line 868 of glr.c  */
#line 5569 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_UPC_SHARED, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 578:
/* Line 868 of glr.c  */
#line 5575 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_UPC_RELAXED, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 579:
/* Line 868 of glr.c  */
#line 5579 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_UPC_STRICT, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 580:
/* Line 868 of glr.c  */
#line 5586 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 581:
/* Line 868 of glr.c  */
#line 5590 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast));
}
    break;

  case 582:
/* Line 868 of glr.c  */
#line 5596 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_UPC_LAYOUT_QUALIFIER, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 583:
/* Line 868 of glr.c  */
#line 5600 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_UPC_LAYOUT_QUALIFIER, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 584:
/* Line 868 of glr.c  */
#line 5604 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_UPC_LAYOUT_QUALIFIER, 
            ASTLeaf(AST_UPC_LAYOUT_UNDEF, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yyloc).first_column), NULL), 
            make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 585:
/* Line 868 of glr.c  */
#line 5612 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 586:
/* Line 868 of glr.c  */
#line 5618 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_UPC_NOTIFY, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 587:
/* Line 868 of glr.c  */
#line 5622 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_UPC_WAIT, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 588:
/* Line 868 of glr.c  */
#line 5626 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_UPC_BARRIER, (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yyloc).first_column), NULL);
}
    break;

  case 589:
/* Line 868 of glr.c  */
#line 5630 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_UPC_FENCE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yyloc).first_column), NULL);
}
    break;

  case 590:
/* Line 868 of glr.c  */
#line 5636 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 591:
/* Line 868 of glr.c  */
#line 5640 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = NULL;
}
    break;

  case 592:
/* Line 868 of glr.c  */
#line 5646 "src/frontend/c99.y"
    {
    AST upc_forall_header =
        ASTMake4(AST_UPC_FORALL_HEADER, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (10))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (10))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (10))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((8) - (10))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (10))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (10))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (10))].yystate.yyloc).first_column), NULL);

    ((*yyvalp).ast) = ASTMake2(AST_UPC_FORALL, upc_forall_header, (((yyGLRStackItem const *)yyvsp)[YYFILL ((10) - (10))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (10))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (10))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (10))].yystate.yyloc).first_column), NULL);
}
    break;

  case 593:
/* Line 868 of glr.c  */
#line 5655 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 594:
/* Line 868 of glr.c  */
#line 5659 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = NULL;
}
    break;

  case 595:
/* Line 868 of glr.c  */
#line 5664 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 596:
/* Line 868 of glr.c  */
#line 5668 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_UPC_CONTINUE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 597:
/* Line 868 of glr.c  */
#line 5695 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 598:
/* Line 868 of glr.c  */
#line 5701 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 599:
/* Line 868 of glr.c  */
#line 5707 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_CUDA_DEVICE, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 600:
/* Line 868 of glr.c  */
#line 5711 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_CUDA_GLOBAL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 601:
/* Line 868 of glr.c  */
#line 5715 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_CUDA_HOST, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 602:
/* Line 868 of glr.c  */
#line 5719 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_CUDA_CONSTANT, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 603:
/* Line 868 of glr.c  */
#line 5723 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_CUDA_SHARED, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 604:
/* Line 868 of glr.c  */
#line 5729 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake3(AST_CUDA_KERNEL_CALL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (4))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 605:
/* Line 868 of glr.c  */
#line 5733 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake3(AST_CUDA_KERNEL_CALL, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (5))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 606:
/* Line 868 of glr.c  */
#line 5739 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.ast);
}
    break;

  case 607:
/* Line 868 of glr.c  */
#line 5745 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake4(AST_CUDA_KERNEL_ARGUMENTS, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (7))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (7))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((7) - (7))].yystate.yysemantics.yysval.ast), ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (7))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 608:
/* Line 868 of glr.c  */
#line 5749 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake4(AST_CUDA_KERNEL_ARGUMENTS, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.ast), NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (5))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 609:
/* Line 868 of glr.c  */
#line 5753 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake4(AST_CUDA_KERNEL_ARGUMENTS, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast), NULL, NULL, ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast)), NULL);
}
    break;

  case 610:
/* Line 868 of glr.c  */
#line 5759 "src/frontend/c99.y"
    {
    ((*yyvalp).token_atrib) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib);
}
    break;

  case 611:
/* Line 868 of glr.c  */
#line 5766 "src/frontend/c99.y"
    {
    ((*yyvalp).token_atrib) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib);
}
    break;

  case 612:
/* Line 868 of glr.c  */
#line 5798 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 613:
/* Line 868 of glr.c  */
#line 5804 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_OPENCL_GLOBAL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 614:
/* Line 868 of glr.c  */
#line 5808 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_OPENCL_KERNEL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 615:
/* Line 868 of glr.c  */
#line 5812 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_OPENCL_CONSTANT, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 616:
/* Line 868 of glr.c  */
#line 5816 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_OPENCL_LOCAL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 617:
/* Line 868 of glr.c  */
#line 5829 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_XL_BUILTIN_SPEC, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, 0), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 618:
/* Line 868 of glr.c  */
#line 5836 "src/frontend/c99.y"
    {
    if (CURRENT_CONFIGURATION->native_vendor == NATIVE_VENDOR_IBM)
    {
        ((*yyvalp).ast) = ast_list_concat(ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast));
    }
    else
    {
        warn_printf_at(ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)), "ignoring '#pragma %s' after the declarator\n",
                 ast_get_text((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.ast)));
        ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.ast);
    }
}
    break;

  case 619:
/* Line 868 of glr.c  */
#line 5849 "src/frontend/c99.y"
    {
    if (CURRENT_CONFIGURATION->native_vendor == NATIVE_VENDOR_IBM)
    {
        ((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
    }
    else
    {
        warn_printf_at(ast_get_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)), "ignoring '#pragma %s' after the declarator\n",
                 ast_get_text((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast)));
        ((*yyvalp).ast) = NULL;
    }
}
    break;

  case 620:
/* Line 868 of glr.c  */
#line 5883 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 621:
/* Line 868 of glr.c  */
#line 5889 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_MS_DECLSPEC, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 622:
/* Line 868 of glr.c  */
#line 5895 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = NULL;
}
    break;

  case 623:
/* Line 868 of glr.c  */
#line 5899 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 624:
/* Line 868 of glr.c  */
#line 5905 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTListLeaf((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast));
}
    break;

  case 625:
/* Line 868 of glr.c  */
#line 5909 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTList((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.ast));
}
    break;

  case 626:
/* Line 868 of glr.c  */
#line 5915 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_MS_DECLSPEC_ITEM, NULL, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 627:
/* Line 868 of glr.c  */
#line 5919 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_MS_DECLSPEC_ITEM, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.token_atrib).token_text);
}
    break;

  case 628:
/* Line 868 of glr.c  */
#line 5925 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.ast);
}
    break;

  case 629:
/* Line 868 of glr.c  */
#line 5931 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_MS_INT8, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 630:
/* Line 868 of glr.c  */
#line 5935 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_MS_INT16, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 631:
/* Line 868 of glr.c  */
#line 5939 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_MS_INT32, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 632:
/* Line 868 of glr.c  */
#line 5943 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTLeaf(AST_MS_INT64, make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yyloc).first_column), NULL);
}
    break;

  case 633:
/* Line 868 of glr.c  */
#line 5959 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake1(AST_INTEL_ASSUME, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yyloc).first_column), NULL);
}
    break;

  case 634:
/* Line 868 of glr.c  */
#line 5963 "src/frontend/c99.y"
    {
    ((*yyvalp).ast) = ASTMake2(AST_INTEL_ASSUME_ALIGNED, (((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (6))].yystate.yysemantics.yysval.ast), (((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (6))].yystate.yysemantics.yysval.ast), make_locus((((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_filename, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_line, (((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yyloc).first_column), NULL);
}
    break;


/* Line 868 of glr.c  */
#line 10632 "src/frontend/c99-parser.c"
      default: break;
    }

  return yyok;
# undef yyerrok
# undef YYABORT
# undef YYACCEPT
# undef YYERROR
# undef YYBACKUP
# undef yyclearin
# undef YYRECOVERING
}


/*ARGSUSED*/ static void
yyuserMerge (int yyn, YYSTYPE* yy0, YYSTYPE* yy1)
{
  YYUSE (yy0);
  YYUSE (yy1);

  switch (yyn)
    {
        case 1: yy0->ast = ambiguityHandler (*yy0, *yy1); break;

      default: break;
    }
}

                              /* Bison grammar-table manipulation.  */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, AST* parsed_tree)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (parsed_tree);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}

/** Number of symbols composing the right hand side of rule #RULE.  */
static inline int
yyrhsLength (yyRuleNum yyrule)
{
  return yyr2[yyrule];
}

static void
yydestroyGLRState (char const *yymsg, yyGLRState *yys, AST* parsed_tree)
{
  if (yys->yyresolved)
    yydestruct (yymsg, yystos[yys->yylrState],
                &yys->yysemantics.yysval, &yys->yyloc, parsed_tree);
  else
    {
#if YYDEBUG
      if (yydebug)
        {
          if (yys->yysemantics.yyfirstVal)
            YYFPRINTF (stderr, "%s unresolved ", yymsg);
          else
            YYFPRINTF (stderr, "%s incomplete ", yymsg);
          yy_symbol_print (stderr, yystos[yys->yylrState],
                           YY_NULL, &yys->yyloc, parsed_tree);
          YYFPRINTF (stderr, "\n");
        }
#endif

      if (yys->yysemantics.yyfirstVal)
        {
          yySemanticOption *yyoption = yys->yysemantics.yyfirstVal;
          yyGLRState *yyrh;
          int yyn;
          for (yyrh = yyoption->yystate, yyn = yyrhsLength (yyoption->yyrule);
               yyn > 0;
               yyrh = yyrh->yypred, yyn -= 1)
            yydestroyGLRState (yymsg, yyrh, parsed_tree);
        }
    }
}

/** Left-hand-side symbol for rule #RULE.  */
static inline yySymbol
yylhsNonterm (yyRuleNum yyrule)
{
  return yyr1[yyrule];
}

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-903)))

/** True iff LR state STATE has only a default reduction (regardless
 *  of token).  */
static inline yybool
yyisDefaultedState (yyStateNum yystate)
{
  return yypact_value_is_default (yypact[yystate]);
}

/** The default reduction for STATE, assuming it has one.  */
static inline yyRuleNum
yydefaultAction (yyStateNum yystate)
{
  return yydefact[yystate];
}

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

/** Set *YYACTION to the action to take in YYSTATE on seeing YYTOKEN.
 *  Result R means
 *    R < 0:  Reduce on rule -R.
 *    R = 0:  Error.
 *    R > 0:  Shift to state R.
 *  Set *CONFLICTS to a pointer into yyconfl to 0-terminated list of
 *  conflicting reductions.
 */
static inline void
yygetLRActions (yyStateNum yystate, int yytoken,
                int* yyaction, const short int** yyconflicts)
{
  int yyindex = yypact[yystate] + yytoken;
  if (yypact_value_is_default (yypact[yystate])
      || yyindex < 0 || YYLAST < yyindex || yycheck[yyindex] != yytoken)
    {
      *yyaction = -yydefact[yystate];
      *yyconflicts = yyconfl;
    }
  else if (! yytable_value_is_error (yytable[yyindex]))
    {
      *yyaction = yytable[yyindex];
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
  else
    {
      *yyaction = 0;
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
}

static inline yyStateNum
yyLRgotoState (yyStateNum yystate, yySymbol yylhs)
{
  int yyr;
  yyr = yypgoto[yylhs - YYNTOKENS] + yystate;
  if (0 <= yyr && yyr <= YYLAST && yycheck[yyr] == yystate)
    return yytable[yyr];
  else
    return yydefgoto[yylhs - YYNTOKENS];
}

static inline yybool
yyisShiftAction (int yyaction)
{
  return 0 < yyaction;
}

static inline yybool
yyisErrorAction (int yyaction)
{
  return yyaction == 0;
}

                                /* GLRStates */

/** Return a fresh GLRStackItem.  Callers should call
 * YY_RESERVE_GLRSTACK afterwards to make sure there is sufficient
 * headroom.  */

static inline yyGLRStackItem*
yynewGLRStackItem (yyGLRStack* yystackp, yybool yyisState)
{
  yyGLRStackItem* yynewItem = yystackp->yynextFree;
  yystackp->yyspaceLeft -= 1;
  yystackp->yynextFree += 1;
  yynewItem->yystate.yyisState = yyisState;
  return yynewItem;
}

/** Add a new semantic action that will execute the action for rule
 *  RULENUM on the semantic values in RHS to the list of
 *  alternative actions for STATE.  Assumes that RHS comes from
 *  stack #K of *STACKP. */
static void
yyaddDeferredAction (yyGLRStack* yystackp, size_t yyk, yyGLRState* yystate,
                     yyGLRState* rhs, yyRuleNum yyrule)
{
  yySemanticOption* yynewOption =
    &yynewGLRStackItem (yystackp, yyfalse)->yyoption;
  yynewOption->yystate = rhs;
  yynewOption->yyrule = yyrule;
  if (yystackp->yytops.yylookaheadNeeds[yyk])
    {
      yynewOption->yyrawchar = yychar;
      yynewOption->yyval = yylval;
      yynewOption->yyloc = yylloc;
    }
  else
    yynewOption->yyrawchar = YYEMPTY;
  yynewOption->yynext = yystate->yysemantics.yyfirstVal;
  yystate->yysemantics.yyfirstVal = yynewOption;

  YY_RESERVE_GLRSTACK (yystackp);
}

                                /* GLRStacks */

/** Initialize SET to a singleton set containing an empty stack.  */
static yybool
yyinitStateSet (yyGLRStateSet* yyset)
{
  yyset->yysize = 1;
  yyset->yycapacity = 16;
  yyset->yystates = (yyGLRState**) YYMALLOC (16 * sizeof yyset->yystates[0]);
  if (! yyset->yystates)
    return yyfalse;
  yyset->yystates[0] = YY_NULL;
  yyset->yylookaheadNeeds =
    (yybool*) YYMALLOC (16 * sizeof yyset->yylookaheadNeeds[0]);
  if (! yyset->yylookaheadNeeds)
    {
      YYFREE (yyset->yystates);
      return yyfalse;
    }
  return yytrue;
}

static void yyfreeStateSet (yyGLRStateSet* yyset)
{
  YYFREE (yyset->yystates);
  YYFREE (yyset->yylookaheadNeeds);
}

/** Initialize STACK to a single empty stack, with total maximum
 *  capacity for all stacks of SIZE.  */
static yybool
yyinitGLRStack (yyGLRStack* yystackp, size_t yysize)
{
  yystackp->yyerrState = 0;
  yynerrs = 0;
  yystackp->yyspaceLeft = yysize;
  yystackp->yyitems =
    (yyGLRStackItem*) YYMALLOC (yysize * sizeof yystackp->yynextFree[0]);
  if (!yystackp->yyitems)
    return yyfalse;
  yystackp->yynextFree = yystackp->yyitems;
  yystackp->yysplitPoint = YY_NULL;
  yystackp->yylastDeleted = YY_NULL;
  return yyinitStateSet (&yystackp->yytops);
}


#if YYSTACKEXPANDABLE
# define YYRELOC(YYFROMITEMS,YYTOITEMS,YYX,YYTYPE) \
  &((YYTOITEMS) - ((YYFROMITEMS) - (yyGLRStackItem*) (YYX)))->YYTYPE

/** If STACK is expandable, extend it.  WARNING: Pointers into the
    stack from outside should be considered invalid after this call.
    We always expand when there are 1 or fewer items left AFTER an
    allocation, so that we can avoid having external pointers exist
    across an allocation.  */
static void
yyexpandGLRStack (yyGLRStack* yystackp)
{
  yyGLRStackItem* yynewItems;
  yyGLRStackItem* yyp0, *yyp1;
  size_t yynewSize;
  size_t yyn;
  size_t yysize = yystackp->yynextFree - yystackp->yyitems;
  if (YYMAXDEPTH - YYHEADROOM < yysize)
    yyMemoryExhausted (yystackp);
  yynewSize = 2*yysize;
  if (YYMAXDEPTH < yynewSize)
    yynewSize = YYMAXDEPTH;
  yynewItems = (yyGLRStackItem*) YYMALLOC (yynewSize * sizeof yynewItems[0]);
  if (! yynewItems)
    yyMemoryExhausted (yystackp);
  for (yyp0 = yystackp->yyitems, yyp1 = yynewItems, yyn = yysize;
       0 < yyn;
       yyn -= 1, yyp0 += 1, yyp1 += 1)
    {
      *yyp1 = *yyp0;
      if (*(yybool *) yyp0)
        {
          yyGLRState* yys0 = &yyp0->yystate;
          yyGLRState* yys1 = &yyp1->yystate;
          if (yys0->yypred != YY_NULL)
            yys1->yypred =
              YYRELOC (yyp0, yyp1, yys0->yypred, yystate);
          if (! yys0->yyresolved && yys0->yysemantics.yyfirstVal != YY_NULL)
            yys1->yysemantics.yyfirstVal =
              YYRELOC (yyp0, yyp1, yys0->yysemantics.yyfirstVal, yyoption);
        }
      else
        {
          yySemanticOption* yyv0 = &yyp0->yyoption;
          yySemanticOption* yyv1 = &yyp1->yyoption;
          if (yyv0->yystate != YY_NULL)
            yyv1->yystate = YYRELOC (yyp0, yyp1, yyv0->yystate, yystate);
          if (yyv0->yynext != YY_NULL)
            yyv1->yynext = YYRELOC (yyp0, yyp1, yyv0->yynext, yyoption);
        }
    }
  if (yystackp->yysplitPoint != YY_NULL)
    yystackp->yysplitPoint = YYRELOC (yystackp->yyitems, yynewItems,
                                 yystackp->yysplitPoint, yystate);

  for (yyn = 0; yyn < yystackp->yytops.yysize; yyn += 1)
    if (yystackp->yytops.yystates[yyn] != YY_NULL)
      yystackp->yytops.yystates[yyn] =
        YYRELOC (yystackp->yyitems, yynewItems,
                 yystackp->yytops.yystates[yyn], yystate);
  YYFREE (yystackp->yyitems);
  yystackp->yyitems = yynewItems;
  yystackp->yynextFree = yynewItems + yysize;
  yystackp->yyspaceLeft = yynewSize - yysize;
}
#endif

static void
yyfreeGLRStack (yyGLRStack* yystackp)
{
  YYFREE (yystackp->yyitems);
  yyfreeStateSet (&yystackp->yytops);
}

/** Assuming that S is a GLRState somewhere on STACK, update the
 *  splitpoint of STACK, if needed, so that it is at least as deep as
 *  S.  */
static inline void
yyupdateSplit (yyGLRStack* yystackp, yyGLRState* yys)
{
  if (yystackp->yysplitPoint != YY_NULL && yystackp->yysplitPoint > yys)
    yystackp->yysplitPoint = yys;
}

/** Invalidate stack #K in STACK.  */
static inline void
yymarkStackDeleted (yyGLRStack* yystackp, size_t yyk)
{
  if (yystackp->yytops.yystates[yyk] != YY_NULL)
    yystackp->yylastDeleted = yystackp->yytops.yystates[yyk];
  yystackp->yytops.yystates[yyk] = YY_NULL;
}

/** Undelete the last stack that was marked as deleted.  Can only be
    done once after a deletion, and only when all other stacks have
    been deleted.  */
static void
yyundeleteLastStack (yyGLRStack* yystackp)
{
  if (yystackp->yylastDeleted == YY_NULL || yystackp->yytops.yysize != 0)
    return;
  yystackp->yytops.yystates[0] = yystackp->yylastDeleted;
  yystackp->yytops.yysize = 1;
  YYDPRINTF ((stderr, "Restoring last deleted stack as stack #0.\n"));
  yystackp->yylastDeleted = YY_NULL;
}

static inline void
yyremoveDeletes (yyGLRStack* yystackp)
{
  size_t yyi, yyj;
  yyi = yyj = 0;
  while (yyj < yystackp->yytops.yysize)
    {
      if (yystackp->yytops.yystates[yyi] == YY_NULL)
        {
          if (yyi == yyj)
            {
              YYDPRINTF ((stderr, "Removing dead stacks.\n"));
            }
          yystackp->yytops.yysize -= 1;
        }
      else
        {
          yystackp->yytops.yystates[yyj] = yystackp->yytops.yystates[yyi];
          /* In the current implementation, it's unnecessary to copy
             yystackp->yytops.yylookaheadNeeds[yyi] since, after
             yyremoveDeletes returns, the parser immediately either enters
             deterministic operation or shifts a token.  However, it doesn't
             hurt, and the code might evolve to need it.  */
          yystackp->yytops.yylookaheadNeeds[yyj] =
            yystackp->yytops.yylookaheadNeeds[yyi];
          if (yyj != yyi)
            {
              YYDPRINTF ((stderr, "Rename stack %lu -> %lu.\n",
                          (unsigned long int) yyi, (unsigned long int) yyj));
            }
          yyj += 1;
        }
      yyi += 1;
    }
}

/** Shift to a new state on stack #K of STACK, corresponding to LR state
 * LRSTATE, at input position POSN, with (resolved) semantic value SVAL.  */
static inline void
yyglrShift (yyGLRStack* yystackp, size_t yyk, yyStateNum yylrState,
            size_t yyposn,
            YYSTYPE* yyvalp, YYLTYPE* yylocp)
{
  yyGLRState* yynewState = &yynewGLRStackItem (yystackp, yytrue)->yystate;

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yytrue;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yysval = *yyvalp;
  yynewState->yyloc = *yylocp;
  yystackp->yytops.yystates[yyk] = yynewState;

  YY_RESERVE_GLRSTACK (yystackp);
}

/** Shift stack #K of YYSTACK, to a new state corresponding to LR
 *  state YYLRSTATE, at input position YYPOSN, with the (unresolved)
 *  semantic value of YYRHS under the action for YYRULE.  */
static inline void
yyglrShiftDefer (yyGLRStack* yystackp, size_t yyk, yyStateNum yylrState,
                 size_t yyposn, yyGLRState* rhs, yyRuleNum yyrule)
{
  yyGLRState* yynewState = &yynewGLRStackItem (yystackp, yytrue)->yystate;

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yyfalse;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yyfirstVal = YY_NULL;
  yystackp->yytops.yystates[yyk] = yynewState;

  /* Invokes YY_RESERVE_GLRSTACK.  */
  yyaddDeferredAction (yystackp, yyk, yynewState, rhs, yyrule);
}

/** Pop the symbols consumed by reduction #RULE from the top of stack
 *  #K of STACK, and perform the appropriate semantic action on their
 *  semantic values.  Assumes that all ambiguities in semantic values
 *  have been previously resolved.  Set *VALP to the resulting value,
 *  and *LOCP to the computed location (if any).  Return value is as
 *  for userAction.  */
static inline YYRESULTTAG
yydoAction (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
            YYSTYPE* yyvalp, YYLTYPE *yylocp, AST* parsed_tree)
{
  int yynrhs = yyrhsLength (yyrule);

  if (yystackp->yysplitPoint == YY_NULL)
    {
      /* Standard special case: single stack.  */
      yyGLRStackItem* rhs = (yyGLRStackItem*) yystackp->yytops.yystates[yyk];
      YYASSERT (yyk == 0);
      yystackp->yynextFree -= yynrhs;
      yystackp->yyspaceLeft += yynrhs;
      yystackp->yytops.yystates[0] = & yystackp->yynextFree[-1].yystate;
      return yyuserAction (yyrule, yynrhs, rhs, yystackp,
                           yyvalp, yylocp, parsed_tree);
    }
  else
    {
      /* At present, doAction is never called in nondeterministic
       * mode, so this branch is never taken.  It is here in
       * anticipation of a future feature that will allow immediate
       * evaluation of selected actions in nondeterministic mode.  */
      int yyi;
      yyGLRState* yys;
      yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
      yys = yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred
        = yystackp->yytops.yystates[yyk];
      if (yynrhs == 0)
        /* Set default location.  */
        yyrhsVals[YYMAXRHS + YYMAXLEFT - 1].yystate.yyloc = yys->yyloc;
      for (yyi = 0; yyi < yynrhs; yyi += 1)
        {
          yys = yys->yypred;
          YYASSERT (yys);
        }
      yyupdateSplit (yystackp, yys);
      yystackp->yytops.yystates[yyk] = yys;
      return yyuserAction (yyrule, yynrhs, yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
                           yystackp, yyvalp, yylocp, parsed_tree);
    }
}

#if !YYDEBUG
# define YY_REDUCE_PRINT(Args)
#else
# define YY_REDUCE_PRINT(Args)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print Args;               \
} while (YYID (0))

/*----------------------------------------------------------.
| Report that the RULE is going to be reduced on stack #K.  |
`----------------------------------------------------------*/

/*ARGSUSED*/ static inline void
yy_reduce_print (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
                 YYSTYPE* yyvalp, YYLTYPE *yylocp, AST* parsed_tree)
{
  int yynrhs = yyrhsLength (yyrule);
  yybool yynormal __attribute__ ((__unused__)) =
    (yystackp->yysplitPoint == YY_NULL);
  yyGLRStackItem* yyvsp = (yyGLRStackItem*) yystackp->yytops.yystates[yyk];
  int yylow = 1;
  int yyi;
  YYUSE (yyvalp);
  YYUSE (yylocp);
  YYUSE (parsed_tree);
  YYFPRINTF (stderr, "Reducing stack %lu by rule %d (line %lu):\n",
             (unsigned long int) yyk, yyrule - 1,
             (unsigned long int) yyrline[yyrule]);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
                       &(((yyGLRStackItem const *)yyvsp)[YYFILL ((yyi + 1) - (yynrhs))].yystate.yysemantics.yysval)
                       , &(((yyGLRStackItem const *)yyvsp)[YYFILL ((yyi + 1) - (yynrhs))].yystate.yyloc)                       , parsed_tree);
      YYFPRINTF (stderr, "\n");
    }
}
#endif

/** Pop items off stack #K of STACK according to grammar rule RULE,
 *  and push back on the resulting nonterminal symbol.  Perform the
 *  semantic action associated with RULE and store its value with the
 *  newly pushed state, if FORCEEVAL or if STACK is currently
 *  unambiguous.  Otherwise, store the deferred semantic action with
 *  the new state.  If the new state would have an identical input
 *  position, LR state, and predecessor to an existing state on the stack,
 *  it is identified with that existing state, eliminating stack #K from
 *  the STACK.  In this case, the (necessarily deferred) semantic value is
 *  added to the options for the existing state's semantic value.
 */
static inline YYRESULTTAG
yyglrReduce (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
             yybool yyforceEval, AST* parsed_tree)
{
  size_t yyposn = yystackp->yytops.yystates[yyk]->yyposn;

  if (yyforceEval || yystackp->yysplitPoint == YY_NULL)
    {
      YYSTYPE yysval;
      YYLTYPE yyloc;

      YY_REDUCE_PRINT ((yystackp, yyk, yyrule, &yysval, &yyloc, parsed_tree));
      YYCHK (yydoAction (yystackp, yyk, yyrule, &yysval, &yyloc, parsed_tree));
      YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyrule], &yysval, &yyloc);
      yyglrShift (yystackp, yyk,
                  yyLRgotoState (yystackp->yytops.yystates[yyk]->yylrState,
                                 yylhsNonterm (yyrule)),
                  yyposn, &yysval, &yyloc);
    }
  else
    {
      size_t yyi;
      int yyn;
      yyGLRState* yys, *yys0 = yystackp->yytops.yystates[yyk];
      yyStateNum yynewLRState;

      for (yys = yystackp->yytops.yystates[yyk], yyn = yyrhsLength (yyrule);
           0 < yyn; yyn -= 1)
        {
          yys = yys->yypred;
          YYASSERT (yys);
        }
      yyupdateSplit (yystackp, yys);
      yynewLRState = yyLRgotoState (yys->yylrState, yylhsNonterm (yyrule));
      YYDPRINTF ((stderr,
                  "Reduced stack %lu by rule #%d; action deferred.  Now in state %d.\n",
                  (unsigned long int) yyk, yyrule - 1, yynewLRState));
      for (yyi = 0; yyi < yystackp->yytops.yysize; yyi += 1)
        if (yyi != yyk && yystackp->yytops.yystates[yyi] != YY_NULL)
          {
            yyGLRState *yysplit = yystackp->yysplitPoint;
            yyGLRState *yyp = yystackp->yytops.yystates[yyi];
            while (yyp != yys && yyp != yysplit && yyp->yyposn >= yyposn)
              {
                if (yyp->yylrState == yynewLRState && yyp->yypred == yys)
                  {
                    yyaddDeferredAction (yystackp, yyk, yyp, yys0, yyrule);
                    yymarkStackDeleted (yystackp, yyk);
                    YYDPRINTF ((stderr, "Merging stack %lu into stack %lu.\n",
                                (unsigned long int) yyk,
                                (unsigned long int) yyi));
                    return yyok;
                  }
                yyp = yyp->yypred;
              }
          }
      yystackp->yytops.yystates[yyk] = yys;
      yyglrShiftDefer (yystackp, yyk, yynewLRState, yyposn, yys0, yyrule);
    }
  return yyok;
}

static size_t
yysplitStack (yyGLRStack* yystackp, size_t yyk)
{
  if (yystackp->yysplitPoint == YY_NULL)
    {
      YYASSERT (yyk == 0);
      yystackp->yysplitPoint = yystackp->yytops.yystates[yyk];
    }
  if (yystackp->yytops.yysize >= yystackp->yytops.yycapacity)
    {
      yyGLRState** yynewStates;
      yybool* yynewLookaheadNeeds;

      yynewStates = YY_NULL;

      if (yystackp->yytops.yycapacity
          > (YYSIZEMAX / (2 * sizeof yynewStates[0])))
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yycapacity *= 2;

      yynewStates =
        (yyGLRState**) YYREALLOC (yystackp->yytops.yystates,
                                  (yystackp->yytops.yycapacity
                                   * sizeof yynewStates[0]));
      if (yynewStates == YY_NULL)
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yystates = yynewStates;

      yynewLookaheadNeeds =
        (yybool*) YYREALLOC (yystackp->yytops.yylookaheadNeeds,
                             (yystackp->yytops.yycapacity
                              * sizeof yynewLookaheadNeeds[0]));
      if (yynewLookaheadNeeds == YY_NULL)
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yylookaheadNeeds = yynewLookaheadNeeds;
    }
  yystackp->yytops.yystates[yystackp->yytops.yysize]
    = yystackp->yytops.yystates[yyk];
  yystackp->yytops.yylookaheadNeeds[yystackp->yytops.yysize]
    = yystackp->yytops.yylookaheadNeeds[yyk];
  yystackp->yytops.yysize += 1;
  return yystackp->yytops.yysize-1;
}

/** True iff Y0 and Y1 represent identical options at the top level.
 *  That is, they represent the same rule applied to RHS symbols
 *  that produce the same terminal symbols.  */
static yybool
yyidenticalOptions (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  if (yyy0->yyrule == yyy1->yyrule)
    {
      yyGLRState *yys0, *yys1;
      int yyn;
      for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
           yyn = yyrhsLength (yyy0->yyrule);
           yyn > 0;
           yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
        if (yys0->yyposn != yys1->yyposn)
          return yyfalse;
      return yytrue;
    }
  else
    return yyfalse;
}

/** Assuming identicalOptions (Y0,Y1), destructively merge the
 *  alternative semantic values for the RHS-symbols of Y1 and Y0.  */
static void
yymergeOptionSets (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  yyGLRState *yys0, *yys1;
  int yyn;
  for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
       yyn = yyrhsLength (yyy0->yyrule);
       yyn > 0;
       yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
    {
      if (yys0 == yys1)
        break;
      else if (yys0->yyresolved)
        {
          yys1->yyresolved = yytrue;
          yys1->yysemantics.yysval = yys0->yysemantics.yysval;
        }
      else if (yys1->yyresolved)
        {
          yys0->yyresolved = yytrue;
          yys0->yysemantics.yysval = yys1->yysemantics.yysval;
        }
      else
        {
          yySemanticOption** yyz0p = &yys0->yysemantics.yyfirstVal;
          yySemanticOption* yyz1 = yys1->yysemantics.yyfirstVal;
          while (YYID (yytrue))
            {
              if (yyz1 == *yyz0p || yyz1 == YY_NULL)
                break;
              else if (*yyz0p == YY_NULL)
                {
                  *yyz0p = yyz1;
                  break;
                }
              else if (*yyz0p < yyz1)
                {
                  yySemanticOption* yyz = *yyz0p;
                  *yyz0p = yyz1;
                  yyz1 = yyz1->yynext;
                  (*yyz0p)->yynext = yyz;
                }
              yyz0p = &(*yyz0p)->yynext;
            }
          yys1->yysemantics.yyfirstVal = yys0->yysemantics.yyfirstVal;
        }
    }
}

/** Y0 and Y1 represent two possible actions to take in a given
 *  parsing state; return 0 if no combination is possible,
 *  1 if user-mergeable, 2 if Y0 is preferred, 3 if Y1 is preferred.  */
static int
yypreference (yySemanticOption* y0, yySemanticOption* y1)
{
  yyRuleNum r0 = y0->yyrule, r1 = y1->yyrule;
  int p0 = yydprec[r0], p1 = yydprec[r1];

  if (p0 == p1)
    {
      if (yymerger[r0] == 0 || yymerger[r0] != yymerger[r1])
        return 0;
      else
        return 1;
    }
  if (p0 == 0 || p1 == 0)
    return 0;
  if (p0 < p1)
    return 3;
  if (p1 < p0)
    return 2;
  return 0;
}

static YYRESULTTAG yyresolveValue (yyGLRState* yys,
                                   yyGLRStack* yystackp, AST* parsed_tree);


/** Resolve the previous N states starting at and including state S.  If result
 *  != yyok, some states may have been left unresolved possibly with empty
 *  semantic option chains.  Regardless of whether result = yyok, each state
 *  has been left with consistent data so that yydestroyGLRState can be invoked
 *  if necessary.  */
static YYRESULTTAG
yyresolveStates (yyGLRState* yys, int yyn,
                 yyGLRStack* yystackp, AST* parsed_tree)
{
  if (0 < yyn)
    {
      YYASSERT (yys->yypred);
      YYCHK (yyresolveStates (yys->yypred, yyn-1, yystackp, parsed_tree));
      if (! yys->yyresolved)
        YYCHK (yyresolveValue (yys, yystackp, parsed_tree));
    }
  return yyok;
}

/** Resolve the states for the RHS of OPT, perform its user action, and return
 *  the semantic value and location.  Regardless of whether result = yyok, all
 *  RHS states have been destroyed (assuming the user action destroys all RHS
 *  semantic values if invoked).  */
static YYRESULTTAG
yyresolveAction (yySemanticOption* yyopt, yyGLRStack* yystackp,
                 YYSTYPE* yyvalp, YYLTYPE *yylocp, AST* parsed_tree)
{
  yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
  int yynrhs = yyrhsLength (yyopt->yyrule);
  YYRESULTTAG yyflag =
    yyresolveStates (yyopt->yystate, yynrhs, yystackp, parsed_tree);
  if (yyflag != yyok)
    {
      yyGLRState *yys;
      for (yys = yyopt->yystate; yynrhs > 0; yys = yys->yypred, yynrhs -= 1)
        yydestroyGLRState ("Cleanup: popping", yys, parsed_tree);
      return yyflag;
    }

  yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred = yyopt->yystate;
  if (yynrhs == 0)
    /* Set default location.  */
    yyrhsVals[YYMAXRHS + YYMAXLEFT - 1].yystate.yyloc = yyopt->yystate->yyloc;
  {
    int yychar_current = yychar;
    YYSTYPE yylval_current = yylval;
    YYLTYPE yylloc_current = yylloc;
    yychar = yyopt->yyrawchar;
    yylval = yyopt->yyval;
    yylloc = yyopt->yyloc;
    yyflag = yyuserAction (yyopt->yyrule, yynrhs,
                           yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
                           yystackp, yyvalp, yylocp, parsed_tree);
    yychar = yychar_current;
    yylval = yylval_current;
    yylloc = yylloc_current;
  }
  return yyflag;
}

#if YYDEBUG
static void
yyreportTree (yySemanticOption* yyx, int yyindent)
{
  int yynrhs = yyrhsLength (yyx->yyrule);
  int yyi;
  yyGLRState* yys;
  yyGLRState* yystates[1 + YYMAXRHS];
  yyGLRState yyleftmost_state;

  for (yyi = yynrhs, yys = yyx->yystate; 0 < yyi; yyi -= 1, yys = yys->yypred)
    yystates[yyi] = yys;
  if (yys == YY_NULL)
    {
      yyleftmost_state.yyposn = 0;
      yystates[0] = &yyleftmost_state;
    }
  else
    yystates[0] = yys;

  if (yyx->yystate->yyposn < yys->yyposn + 1)
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, empty>\n",
               yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
               yyx->yyrule - 1);
  else
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, tokens %lu .. %lu>\n",
               yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
               yyx->yyrule - 1, (unsigned long int) (yys->yyposn + 1),
               (unsigned long int) yyx->yystate->yyposn);
  for (yyi = 1; yyi <= yynrhs; yyi += 1)
    {
      if (yystates[yyi]->yyresolved)
        {
          if (yystates[yyi-1]->yyposn+1 > yystates[yyi]->yyposn)
            YYFPRINTF (stderr, "%*s%s <empty>\n", yyindent+2, "",
                       yytokenName (yyrhs[yyprhs[yyx->yyrule]+yyi-1]));
          else
            YYFPRINTF (stderr, "%*s%s <tokens %lu .. %lu>\n", yyindent+2, "",
                       yytokenName (yyrhs[yyprhs[yyx->yyrule]+yyi-1]),
                       (unsigned long int) (yystates[yyi - 1]->yyposn + 1),
                       (unsigned long int) yystates[yyi]->yyposn);
        }
      else
        yyreportTree (yystates[yyi]->yysemantics.yyfirstVal, yyindent+2);
    }
}
#endif

/*ARGSUSED*/ static YYRESULTTAG
yyreportAmbiguity (yySemanticOption* yyx0,
                   yySemanticOption* yyx1, AST* parsed_tree)
{
  YYUSE (yyx0);
  YYUSE (yyx1);

#if YYDEBUG
  YYFPRINTF (stderr, "Ambiguity detected.\n");
  YYFPRINTF (stderr, "Option 1,\n");
  yyreportTree (yyx0, 2);
  YYFPRINTF (stderr, "\nOption 2,\n");
  yyreportTree (yyx1, 2);
  YYFPRINTF (stderr, "\n");
#endif

  yyerror (parsed_tree, YY_("syntax is ambiguous"));
  return yyabort;
}

/** Starting at and including state S1, resolve the location for each of the
 *  previous N1 states that is unresolved.  The first semantic option of a state
 *  is always chosen.  */
static void
yyresolveLocations (yyGLRState* yys1, int yyn1,
                    yyGLRStack *yystackp, AST* parsed_tree)
{
  if (0 < yyn1)
    {
      yyresolveLocations (yys1->yypred, yyn1 - 1, yystackp, parsed_tree);
      if (!yys1->yyresolved)
        {
          yyGLRStackItem yyrhsloc[1 + YYMAXRHS];
          int yynrhs;
          yySemanticOption *yyoption = yys1->yysemantics.yyfirstVal;
          YYASSERT (yyoption != YY_NULL);
          yynrhs = yyrhsLength (yyoption->yyrule);
          if (yynrhs > 0)
            {
              yyGLRState *yys;
              int yyn;
              yyresolveLocations (yyoption->yystate, yynrhs,
                                  yystackp, parsed_tree);
              for (yys = yyoption->yystate, yyn = yynrhs;
                   yyn > 0;
                   yys = yys->yypred, yyn -= 1)
                yyrhsloc[yyn].yystate.yyloc = yys->yyloc;
            }
          else
            {
              /* Both yyresolveAction and yyresolveLocations traverse the GSS
                 in reverse rightmost order.  It is only necessary to invoke
                 yyresolveLocations on a subforest for which yyresolveAction
                 would have been invoked next had an ambiguity not been
                 detected.  Thus the location of the previous state (but not
                 necessarily the previous state itself) is guaranteed to be
                 resolved already.  */
              yyGLRState *yyprevious = yyoption->yystate;
              yyrhsloc[0].yystate.yyloc = yyprevious->yyloc;
            }
          {
            int yychar_current = yychar;
            YYSTYPE yylval_current = yylval;
            YYLTYPE yylloc_current = yylloc;
            yychar = yyoption->yyrawchar;
            yylval = yyoption->yyval;
            yylloc = yyoption->yyloc;
            YYLLOC_DEFAULT ((yys1->yyloc), yyrhsloc, yynrhs);
            yychar = yychar_current;
            yylval = yylval_current;
            yylloc = yylloc_current;
          }
        }
    }
}

/** Resolve the ambiguity represented in state S, perform the indicated
 *  actions, and set the semantic value of S.  If result != yyok, the chain of
 *  semantic options in S has been cleared instead or it has been left
 *  unmodified except that redundant options may have been removed.  Regardless
 *  of whether result = yyok, S has been left with consistent data so that
 *  yydestroyGLRState can be invoked if necessary.  */
static YYRESULTTAG
yyresolveValue (yyGLRState* yys, yyGLRStack* yystackp, AST* parsed_tree)
{
  yySemanticOption* yyoptionList = yys->yysemantics.yyfirstVal;
  yySemanticOption* yybest = yyoptionList;
  yySemanticOption** yypp;
  yybool yymerge = yyfalse;
  YYSTYPE yysval;
  YYRESULTTAG yyflag;
  YYLTYPE *yylocp = &yys->yyloc;

  for (yypp = &yyoptionList->yynext; *yypp != YY_NULL; )
    {
      yySemanticOption* yyp = *yypp;

      if (yyidenticalOptions (yybest, yyp))
        {
          yymergeOptionSets (yybest, yyp);
          *yypp = yyp->yynext;
        }
      else
        {
          switch (yypreference (yybest, yyp))
            {
            case 0:
              yyresolveLocations (yys, 1, yystackp, parsed_tree);
              return yyreportAmbiguity (yybest, yyp, parsed_tree);
              break;
            case 1:
              yymerge = yytrue;
              break;
            case 2:
              break;
            case 3:
              yybest = yyp;
              yymerge = yyfalse;
              break;
            default:
              /* This cannot happen so it is not worth a YYASSERT (yyfalse),
                 but some compilers complain if the default case is
                 omitted.  */
              break;
            }
          yypp = &yyp->yynext;
        }
    }

  if (yymerge)
    {
      yySemanticOption* yyp;
      int yyprec = yydprec[yybest->yyrule];
      yyflag = yyresolveAction (yybest, yystackp, &yysval, yylocp, parsed_tree);
      if (yyflag == yyok)
        for (yyp = yybest->yynext; yyp != YY_NULL; yyp = yyp->yynext)
          {
            if (yyprec == yydprec[yyp->yyrule])
              {
                YYSTYPE yysval_other;
                YYLTYPE yydummy;
                yyflag = yyresolveAction (yyp, yystackp, &yysval_other, &yydummy, parsed_tree);
                if (yyflag != yyok)
                  {
                    yydestruct ("Cleanup: discarding incompletely merged value for",
                                yystos[yys->yylrState],
                                &yysval, yylocp, parsed_tree);
                    break;
                  }
                yyuserMerge (yymerger[yyp->yyrule], &yysval, &yysval_other);
              }
          }
    }
  else
    yyflag = yyresolveAction (yybest, yystackp, &yysval, yylocp, parsed_tree);

  if (yyflag == yyok)
    {
      yys->yyresolved = yytrue;
      yys->yysemantics.yysval = yysval;
    }
  else
    yys->yysemantics.yyfirstVal = YY_NULL;
  return yyflag;
}

static YYRESULTTAG
yyresolveStack (yyGLRStack* yystackp, AST* parsed_tree)
{
  if (yystackp->yysplitPoint != YY_NULL)
    {
      yyGLRState* yys;
      int yyn;

      for (yyn = 0, yys = yystackp->yytops.yystates[0];
           yys != yystackp->yysplitPoint;
           yys = yys->yypred, yyn += 1)
        continue;
      YYCHK (yyresolveStates (yystackp->yytops.yystates[0], yyn, yystackp
                             , parsed_tree));
    }
  return yyok;
}

static void
yycompressStack (yyGLRStack* yystackp)
{
  yyGLRState* yyp, *yyq, *yyr;

  if (yystackp->yytops.yysize != 1 || yystackp->yysplitPoint == YY_NULL)
    return;

  for (yyp = yystackp->yytops.yystates[0], yyq = yyp->yypred, yyr = YY_NULL;
       yyp != yystackp->yysplitPoint;
       yyr = yyp, yyp = yyq, yyq = yyp->yypred)
    yyp->yypred = yyr;

  yystackp->yyspaceLeft += yystackp->yynextFree - yystackp->yyitems;
  yystackp->yynextFree = ((yyGLRStackItem*) yystackp->yysplitPoint) + 1;
  yystackp->yyspaceLeft -= yystackp->yynextFree - yystackp->yyitems;
  yystackp->yysplitPoint = YY_NULL;
  yystackp->yylastDeleted = YY_NULL;

  while (yyr != YY_NULL)
    {
      yystackp->yynextFree->yystate = *yyr;
      yyr = yyr->yypred;
      yystackp->yynextFree->yystate.yypred = &yystackp->yynextFree[-1].yystate;
      yystackp->yytops.yystates[0] = &yystackp->yynextFree->yystate;
      yystackp->yynextFree += 1;
      yystackp->yyspaceLeft -= 1;
    }
}

static YYRESULTTAG
yyprocessOneStack (yyGLRStack* yystackp, size_t yyk,
                   size_t yyposn, AST* parsed_tree)
{
  int yyaction;
  const short int* yyconflicts;
  yyRuleNum yyrule;

  while (yystackp->yytops.yystates[yyk] != YY_NULL)
    {
      yyStateNum yystate = yystackp->yytops.yystates[yyk]->yylrState;
      YYDPRINTF ((stderr, "Stack %lu Entering state %d\n",
                  (unsigned long int) yyk, yystate));

      YYASSERT (yystate != YYFINAL);

      if (yyisDefaultedState (yystate))
        {
          yyrule = yydefaultAction (yystate);
          if (yyrule == 0)
            {
              YYDPRINTF ((stderr, "Stack %lu dies.\n",
                          (unsigned long int) yyk));
              yymarkStackDeleted (yystackp, yyk);
              return yyok;
            }
          YYCHK (yyglrReduce (yystackp, yyk, yyrule, yyfalse, parsed_tree));
        }
      else
        {
          yySymbol yytoken;
          yystackp->yytops.yylookaheadNeeds[yyk] = yytrue;
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

          yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);

          while (*yyconflicts != 0)
            {
              size_t yynewStack = yysplitStack (yystackp, yyk);
              YYDPRINTF ((stderr, "Splitting off stack %lu from %lu.\n",
                          (unsigned long int) yynewStack,
                          (unsigned long int) yyk));
              YYCHK (yyglrReduce (yystackp, yynewStack,
                                  *yyconflicts, yyfalse, parsed_tree));
              YYCHK (yyprocessOneStack (yystackp, yynewStack,
                                        yyposn, parsed_tree));
              yyconflicts += 1;
            }

          if (yyisShiftAction (yyaction))
            break;
          else if (yyisErrorAction (yyaction))
            {
              YYDPRINTF ((stderr, "Stack %lu dies.\n",
                          (unsigned long int) yyk));
              yymarkStackDeleted (yystackp, yyk);
              break;
            }
          else
            YYCHK (yyglrReduce (yystackp, yyk, -yyaction,
                                yyfalse, parsed_tree));
        }
    }
  return yyok;
}

/*ARGSUSED*/ static void
yyreportSyntaxError (yyGLRStack* yystackp, AST* parsed_tree)
{
  if (yystackp->yyerrState != 0)
    return;
#if ! YYERROR_VERBOSE
  yyerror (parsed_tree, YY_("syntax error"));
#else
  {
  yySymbol yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);
  size_t yysize0 = yytnamerr (YY_NULL, yytokenName (yytoken));
  size_t yysize = yysize0;
  yybool yysize_overflow = yyfalse;
  char* yymsg = YY_NULL;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected").  */
  int yycount = 0;

  /* There are many possibilities here to consider:
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
      int yyn = yypact[yystackp->yytops.yystates[0]->yylrState];
      yyarg[yycount++] = yytokenName (yytoken);
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for this
             state because they are default actions.  */
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
                yyarg[yycount++] = yytokenName (yyx);
                {
                  size_t yysz = yysize + yytnamerr (YY_NULL, yytokenName (yyx));
                  yysize_overflow |= yysz < yysize;
                  yysize = yysz;
                }
              }
        }
    }

  switch (yycount)
    {
#define YYCASE_(N, S)                   \
      case N:                           \
        yyformat = S;                   \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  {
    size_t yysz = yysize + strlen (yyformat);
    yysize_overflow |= yysz < yysize;
    yysize = yysz;
  }

  if (!yysize_overflow)
    yymsg = (char *) YYMALLOC (yysize);

  if (yymsg)
    {
      char *yyp = yymsg;
      int yyi = 0;
      while ((*yyp = *yyformat))
        {
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
      yyerror (parsed_tree, yymsg);
      YYFREE (yymsg);
    }
  else
    {
      yyerror (parsed_tree, YY_("syntax error"));
      yyMemoryExhausted (yystackp);
    }
  }
#endif /* YYERROR_VERBOSE */
  yynerrs += 1;
}

/* Recover from a syntax error on *YYSTACKP, assuming that *YYSTACKP->YYTOKENP,
   yylval, and yylloc are the syntactic category, semantic value, and location
   of the lookahead.  */
/*ARGSUSED*/ static void
yyrecoverSyntaxError (yyGLRStack* yystackp, AST* parsed_tree)
{
  size_t yyk;
  int yyj;

  if (yystackp->yyerrState == 3)
    /* We just shifted the error token and (perhaps) took some
       reductions.  Skip tokens until we can proceed.  */
    while (YYID (yytrue))
      {
        yySymbol yytoken;
        if (yychar == YYEOF)
          yyFail (yystackp, parsed_tree, YY_NULL);
        if (yychar != YYEMPTY)
          {
            /* We throw away the lookahead, but the error range
               of the shifted error token must take it into account.  */
            yyGLRState *yys = yystackp->yytops.yystates[0];
            yyGLRStackItem yyerror_range[3];
            yyerror_range[1].yystate.yyloc = yys->yyloc;
            yyerror_range[2].yystate.yyloc = yylloc;
            YYLLOC_DEFAULT ((yys->yyloc), yyerror_range, 2);
            yytoken = YYTRANSLATE (yychar);
            yydestruct ("Error: discarding",
                        yytoken, &yylval, &yylloc, parsed_tree);
          }
        YYDPRINTF ((stderr, "Reading a token: "));
        yychar = YYLEX;
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
        yyj = yypact[yystackp->yytops.yystates[0]->yylrState];
        if (yypact_value_is_default (yyj))
          return;
        yyj += yytoken;
        if (yyj < 0 || YYLAST < yyj || yycheck[yyj] != yytoken)
          {
            if (yydefact[yystackp->yytops.yystates[0]->yylrState] != 0)
              return;
          }
        else if (! yytable_value_is_error (yytable[yyj]))
          return;
      }

  /* Reduce to one stack.  */
  for (yyk = 0; yyk < yystackp->yytops.yysize; yyk += 1)
    if (yystackp->yytops.yystates[yyk] != YY_NULL)
      break;
  if (yyk >= yystackp->yytops.yysize)
    yyFail (yystackp, parsed_tree, YY_NULL);
  for (yyk += 1; yyk < yystackp->yytops.yysize; yyk += 1)
    yymarkStackDeleted (yystackp, yyk);
  yyremoveDeletes (yystackp);
  yycompressStack (yystackp);

  /* Now pop stack until we find a state that shifts the error token.  */
  yystackp->yyerrState = 3;
  while (yystackp->yytops.yystates[0] != YY_NULL)
    {
      yyGLRState *yys = yystackp->yytops.yystates[0];
      yyj = yypact[yys->yylrState];
      if (! yypact_value_is_default (yyj))
        {
          yyj += YYTERROR;
          if (0 <= yyj && yyj <= YYLAST && yycheck[yyj] == YYTERROR
              && yyisShiftAction (yytable[yyj]))
            {
              /* Shift the error token.  */
              /* First adjust its location.*/
              YYLTYPE yyerrloc;
              yystackp->yyerror_range[2].yystate.yyloc = yylloc;
              YYLLOC_DEFAULT (yyerrloc, (yystackp->yyerror_range), 2);
              YY_SYMBOL_PRINT ("Shifting", yystos[yytable[yyj]],
                               &yylval, &yyerrloc);
              yyglrShift (yystackp, 0, yytable[yyj],
                          yys->yyposn, &yylval, &yyerrloc);
              yys = yystackp->yytops.yystates[0];
              break;
            }
        }
      yystackp->yyerror_range[1].yystate.yyloc = yys->yyloc;
      if (yys->yypred != YY_NULL)
        yydestroyGLRState ("Error: popping", yys, parsed_tree);
      yystackp->yytops.yystates[0] = yys->yypred;
      yystackp->yynextFree -= 1;
      yystackp->yyspaceLeft += 1;
    }
  if (yystackp->yytops.yystates[0] == YY_NULL)
    yyFail (yystackp, parsed_tree, YY_NULL);
}

#define YYCHK1(YYE)                                                          \
  do {                                                                       \
    switch (YYE) {                                                           \
    case yyok:                                                               \
      break;                                                                 \
    case yyabort:                                                            \
      goto yyabortlab;                                                       \
    case yyaccept:                                                           \
      goto yyacceptlab;                                                      \
    case yyerr:                                                              \
      goto yyuser_error;                                                     \
    default:                                                                 \
      goto yybuglab;                                                         \
    }                                                                        \
  } while (YYID (0))


/*----------.
| yyparse.  |
`----------*/

int
yyparse (AST* parsed_tree)
{
  int yyresult;
  yyGLRStack yystack;
  yyGLRStack* const yystackp = &yystack;
  size_t yyposn;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY;
  yylval = yyval_default;
  yylloc = yyloc_default;

  if (! yyinitGLRStack (yystackp, YYINITDEPTH))
    goto yyexhaustedlab;
  switch (YYSETJMP (yystack.yyexception_buffer))
    {
    case 0: break;
    case 1: goto yyabortlab;
    case 2: goto yyexhaustedlab;
    default: goto yybuglab;
    }
  yyglrShift (&yystack, 0, 0, 0, &yylval, &yylloc);
  yyposn = 0;

  while (YYID (yytrue))
    {
      /* For efficiency, we have two loops, the first of which is
         specialized to deterministic operation (single stack, no
         potential ambiguity).  */
      /* Standard mode */
      while (YYID (yytrue))
        {
          yyRuleNum yyrule;
          int yyaction;
          const short int* yyconflicts;

          yyStateNum yystate = yystack.yytops.yystates[0]->yylrState;
          YYDPRINTF ((stderr, "Entering state %d\n", yystate));
          if (yystate == YYFINAL)
            goto yyacceptlab;
          if (yyisDefaultedState (yystate))
            {
              yyrule = yydefaultAction (yystate);
              if (yyrule == 0)
                {
               yystack.yyerror_range[1].yystate.yyloc = yylloc;
                  yyreportSyntaxError (&yystack, parsed_tree);
                  goto yyuser_error;
                }
              YYCHK1 (yyglrReduce (&yystack, 0, yyrule, yytrue, parsed_tree));
            }
          else
            {
              yySymbol yytoken;
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

              yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);
              if (*yyconflicts != 0)
                break;
              if (yyisShiftAction (yyaction))
                {
                  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
                  yychar = YYEMPTY;
                  yyposn += 1;
                  yyglrShift (&yystack, 0, yyaction, yyposn, &yylval, &yylloc);
                  if (0 < yystack.yyerrState)
                    yystack.yyerrState -= 1;
                }
              else if (yyisErrorAction (yyaction))
                {
               yystack.yyerror_range[1].yystate.yyloc = yylloc;
                  yyreportSyntaxError (&yystack, parsed_tree);
                  goto yyuser_error;
                }
              else
                YYCHK1 (yyglrReduce (&yystack, 0, -yyaction, yytrue, parsed_tree));
            }
        }

      while (YYID (yytrue))
        {
          yySymbol yytoken_to_shift;
          size_t yys;

          for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
            yystackp->yytops.yylookaheadNeeds[yys] = yychar != YYEMPTY;

          /* yyprocessOneStack returns one of three things:

              - An error flag.  If the caller is yyprocessOneStack, it
                immediately returns as well.  When the caller is finally
                yyparse, it jumps to an error label via YYCHK1.

              - yyok, but yyprocessOneStack has invoked yymarkStackDeleted
                (&yystack, yys), which sets the top state of yys to NULL.  Thus,
                yyparse's following invocation of yyremoveDeletes will remove
                the stack.

              - yyok, when ready to shift a token.

             Except in the first case, yyparse will invoke yyremoveDeletes and
             then shift the next token onto all remaining stacks.  This
             synchronization of the shift (that is, after all preceding
             reductions on all stacks) helps prevent double destructor calls
             on yylval in the event of memory exhaustion.  */

          for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
            YYCHK1 (yyprocessOneStack (&yystack, yys, yyposn, parsed_tree));
          yyremoveDeletes (&yystack);
          if (yystack.yytops.yysize == 0)
            {
              yyundeleteLastStack (&yystack);
              if (yystack.yytops.yysize == 0)
                yyFail (&yystack, parsed_tree, YY_("syntax error"));
              YYCHK1 (yyresolveStack (&yystack, parsed_tree));
              YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
           yystack.yyerror_range[1].yystate.yyloc = yylloc;
              yyreportSyntaxError (&yystack, parsed_tree);
              goto yyuser_error;
            }

          /* If any yyglrShift call fails, it will fail after shifting.  Thus,
             a copy of yylval will already be on stack 0 in the event of a
             failure in the following loop.  Thus, yychar is set to YYEMPTY
             before the loop to make sure the user destructor for yylval isn't
             called twice.  */
          yytoken_to_shift = YYTRANSLATE (yychar);
          yychar = YYEMPTY;
          yyposn += 1;
          for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
            {
              int yyaction;
              const short int* yyconflicts;
              yyStateNum yystate = yystack.yytops.yystates[yys]->yylrState;
              yygetLRActions (yystate, yytoken_to_shift, &yyaction,
                              &yyconflicts);
              /* Note that yyconflicts were handled by yyprocessOneStack.  */
              YYDPRINTF ((stderr, "On stack %lu, ", (unsigned long int) yys));
              YY_SYMBOL_PRINT ("shifting", yytoken_to_shift, &yylval, &yylloc);
              yyglrShift (&yystack, yys, yyaction, yyposn,
                          &yylval, &yylloc);
              YYDPRINTF ((stderr, "Stack %lu now in state #%d\n",
                          (unsigned long int) yys,
                          yystack.yytops.yystates[yys]->yylrState));
            }

          if (yystack.yytops.yysize == 1)
            {
              YYCHK1 (yyresolveStack (&yystack, parsed_tree));
              YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
              yycompressStack (&yystack);
              break;
            }
        }
      continue;
    yyuser_error:
      yyrecoverSyntaxError (&yystack, parsed_tree);
      yyposn = yystack.yytops.yystates[0]->yyposn;
    }

 yyacceptlab:
  yyresult = 0;
  goto yyreturn;

 yybuglab:
  YYASSERT (yyfalse);
  goto yyabortlab;

 yyabortlab:
  yyresult = 1;
  goto yyreturn;

 yyexhaustedlab:
  yyerror (parsed_tree, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;

 yyreturn:
  if (yychar != YYEMPTY)
    yydestruct ("Cleanup: discarding lookahead",
                YYTRANSLATE (yychar), &yylval, &yylloc, parsed_tree);

  /* If the stack is well-formed, pop the stack until it is empty,
     destroying its entries as we go.  But free the stack regardless
     of whether it is well-formed.  */
  if (yystack.yyitems)
    {
      yyGLRState** yystates = yystack.yytops.yystates;
      if (yystates)
        {
          size_t yysize = yystack.yytops.yysize;
          size_t yyk;
          for (yyk = 0; yyk < yysize; yyk += 1)
            if (yystates[yyk])
              {
                while (yystates[yyk])
                  {
                    yyGLRState *yys = yystates[yyk];
                 yystack.yyerror_range[1].yystate.yyloc = yys->yyloc;
                  if (yys->yypred != YY_NULL)
                      yydestroyGLRState ("Cleanup: popping", yys, parsed_tree);
                    yystates[yyk] = yys->yypred;
                    yystack.yynextFree -= 1;
                    yystack.yyspaceLeft += 1;
                  }
                break;
              }
        }
      yyfreeGLRStack (&yystack);
    }

  /* Make sure YYID is used.  */
  return YYID (yyresult);
}

/* DEBUGGING ONLY */
#if YYDEBUG
static void yypstack (yyGLRStack* yystackp, size_t yyk)
  __attribute__ ((__unused__));
static void yypdumpstack (yyGLRStack* yystackp) __attribute__ ((__unused__));

static void
yy_yypstack (yyGLRState* yys)
{
  if (yys->yypred)
    {
      yy_yypstack (yys->yypred);
      YYFPRINTF (stderr, " -> ");
    }
  YYFPRINTF (stderr, "%d@%lu", yys->yylrState,
             (unsigned long int) yys->yyposn);
}

static void
yypstates (yyGLRState* yyst)
{
  if (yyst == YY_NULL)
    YYFPRINTF (stderr, "<null>");
  else
    yy_yypstack (yyst);
  YYFPRINTF (stderr, "\n");
}

static void
yypstack (yyGLRStack* yystackp, size_t yyk)
{
  yypstates (yystackp->yytops.yystates[yyk]);
}

#define YYINDEX(YYX)                                                         \
    ((YYX) == YY_NULL ? -1 : (yyGLRStackItem*) (YYX) - yystackp->yyitems)


static void
yypdumpstack (yyGLRStack* yystackp)
{
  yyGLRStackItem* yyp;
  size_t yyi;
  for (yyp = yystackp->yyitems; yyp < yystackp->yynextFree; yyp += 1)
    {
      YYFPRINTF (stderr, "%3lu. ",
                 (unsigned long int) (yyp - yystackp->yyitems));
      if (*(yybool *) yyp)
        {
          YYFPRINTF (stderr, "Res: %d, LR State: %d, posn: %lu, pred: %ld",
                     yyp->yystate.yyresolved, yyp->yystate.yylrState,
                     (unsigned long int) yyp->yystate.yyposn,
                     (long int) YYINDEX (yyp->yystate.yypred));
          if (! yyp->yystate.yyresolved)
            YYFPRINTF (stderr, ", firstVal: %ld",
                       (long int) YYINDEX (yyp->yystate
                                             .yysemantics.yyfirstVal));
        }
      else
        {
          YYFPRINTF (stderr, "Option. rule: %d, state: %ld, next: %ld",
                     yyp->yyoption.yyrule - 1,
                     (long int) YYINDEX (yyp->yyoption.yystate),
                     (long int) YYINDEX (yyp->yyoption.yynext));
        }
      YYFPRINTF (stderr, "\n");
    }
  YYFPRINTF (stderr, "Tops:");
  for (yyi = 0; yyi < yystackp->yytops.yysize; yyi += 1)
    YYFPRINTF (stderr, "%lu: %ld; ", (unsigned long int) yyi,
               (long int) YYINDEX (yystackp->yytops.yystates[yyi]));
  YYFPRINTF (stderr, "\n");
}
#endif
/* Line 2575 of glr.c  */
#line 5971 "src/frontend/c99.y"











































// This is code



#include "cxx-utils.h"

static AST ambiguityHandler (YYSTYPE x0, YYSTYPE x1)
{
	AST son0 = x0.ast;
	AST son1 = x1.ast;

	if (son0 == son1) 
	{
        return son1;
	}

    return ast_make_ambiguous(son0, son1);
}



void yyerror(AST* parsed_tree UNUSED_PARAMETER, const char* c)
{
    error_printf_at(
        make_locus(
                    mc99lloc.first_filename,
                    mc99lloc.first_line,
                    mc99lloc.first_column),
        "%s\n",
        c);
}
