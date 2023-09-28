/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         configfileparse
#define yylex           configfilelex
#define yyerror         configfileerror
#define yylval          configfilelval
#define yychar          configfilechar
#define yydebug         configfiledebug
#define yynerrs         configfilenerrs
#define yylloc          configfilelloc

/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 28 "./src/driver/cxx-configfile-parser.y"


#include <string.h>
#include "cxx-driver.h"
#include "cxx-profile.h"
#include "cxx-configfile-lexer.h"
#include "cxx-configfile-parser.h"
#include "cxx-utils.h"
#include "cxx-typeenviron.h"
#include "cxx-typeutils.h"
#include "fortran03-typeenviron.h"
#include "fortran03-mangling.h"
#include "mem.h"

static void new_option_list(option_list_t* list);
static void add_to_option_list(option_list_t* list, p_compilation_configuration_line);

static p_compilation_configuration_line process_option_line(
        flag_expr_t* flag_expr,
        profile_option_name_t* name, 
        const char* option_value,
        const char* filename,
        int line);

struct flag_expr_tag
{
    enum flag_op kind;
    struct flag_expr_tag* op[2];
    const char* text;
};

static flag_expr_t* flag_name(const char* name);
static flag_expr_t* flag_true(void);
static flag_expr_t* flag_false(void);
static flag_expr_t* flag_is_defined(const char* name);
static flag_expr_t* flag_not(flag_expr_t* op);
static flag_expr_t* flag_and(flag_expr_t* op1, flag_expr_t* op2);
static flag_expr_t* flag_or(flag_expr_t* op1, flag_expr_t* op2);

static void register_implicit_names(flag_expr_t* flag_expr);

#define YYMALLOC xmalloc
#define YYFREE DELETE
#define YYREALLOC xrealloc


/* Line 371 of yacc.c  */
#line 123 "src/driver/cxx-configfile-parser.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "cxx-configfile-parser-internal.h".  */
#ifndef YY_CONFIGFILE_SRC_DRIVER_CXX_CONFIGFILE_PARSER_INTERNAL_H_INCLUDED
# define YY_CONFIGFILE_SRC_DRIVER_CXX_CONFIGFILE_PARSER_INTERNAL_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int configfiledebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     CONFIGFILE_NAME = 258,
     CONFIGFILE_FLAG_TRUE = 259,
     CONFIGFILE_FLAG_FALSE = 260,
     CONFIGFILE_OPTION_VALUE = 261,
     EOL = 262
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 75 "./src/driver/cxx-configfile-parser.y"

    profile_header_t profile_header;
    p_compilation_configuration_line profile_option;
    profile_option_name_t profile_option_name;
    struct flag_expr_tag* flag;
    const char* str;
    option_list_t option_list;


/* Line 387 of yacc.c  */
#line 183 "src/driver/cxx-configfile-parser.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE configfilelval;
extern YYLTYPE configfilelloc;
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int configfileparse (void *YYPARSE_PARAM);
#else
int configfileparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int configfileparse (void);
#else
int configfileparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_CONFIGFILE_SRC_DRIVER_CXX_CONFIGFILE_PARSER_INTERNAL_H_INCLUDED  */

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 224 "src/driver/cxx-configfile-parser.c"

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
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
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
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   57

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  22
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  16
/* YYNRULES -- Number of rules.  */
#define YYNRULES  35
/* YYNRULES -- Number of states.  */
#define YYNSTATES  58

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   262

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     2,     2,     2,     2,    11,     2,
       9,    10,     2,     2,     8,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     6,     2,
       2,    18,     7,    21,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     4,     2,     5,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    19,    12,    20,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,    13,    14,
      15,    16,    17
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    14,    19,    26,
      33,    35,    36,    39,    41,    47,    50,    52,    53,    55,
      60,    64,    67,    68,    70,    74,    76,    80,    84,    86,
      89,    91,    93,    96,   100,   102
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      23,     0,    -1,    24,    -1,    -1,    24,    25,    -1,    25,
      -1,    26,    27,    -1,     4,    13,     5,    17,    -1,     4,
      13,     6,    13,     5,    17,    -1,     4,    13,     7,    13,
       5,    17,    -1,    28,    -1,    -1,    28,    29,    -1,    29,
      -1,    32,    31,    18,    30,    17,    -1,    32,    17,    -1,
      16,    -1,    -1,    13,    -1,    13,     4,    13,     5,    -1,
      19,    33,    20,    -1,    19,    20,    -1,    -1,    34,    -1,
      34,    12,    35,    -1,    35,    -1,    35,     8,    36,    -1,
      35,    11,    36,    -1,    36,    -1,     3,    37,    -1,    37,
      -1,    13,    -1,    21,    13,    -1,     9,    33,    10,    -1,
      14,    -1,    15,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   124,   124,   128,   132,   135,   140,   181,   186,   191,
     198,   204,   209,   217,   228,   233,   239,   244,   249,   254,
     261,   266,   272,   277,   283,   287,   293,   297,   301,   307,
     311,   317,   321,   325,   329,   333
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'!'", "'['", "']'", "':'", "'>'", "','",
  "'('", "')'", "'&'", "'|'", "\"identifier\"", "\":true:\"",
  "\":false:\"", "\"option-value\"", "\"end-of-line\"", "'='", "'{'",
  "'}'", "'?'", "$accept", "config_file", "profile_seq", "profile",
  "profile_header", "profile_body", "option_line_seq", "option_line",
  "option_value", "option_name", "flag_spec", "flag_expr", "flag_or",
  "flag_and", "flag_not", "flag_atom", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,    33,    91,    93,    58,    62,    44,    40,
      41,    38,   124,   258,   259,   260,   261,   262,    61,   123,
     125,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    22,    23,    23,    24,    24,    25,    26,    26,    26,
      27,    27,    28,    28,    29,    29,    30,    30,    31,    31,
      32,    32,    32,    33,    34,    34,    35,    35,    35,    36,
      36,    37,    37,    37,    37,    37
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     2,     4,     6,     6,
       1,     0,     2,     1,     5,     2,     1,     0,     1,     4,
       3,     2,     0,     1,     3,     1,     3,     3,     1,     2,
       1,     1,     2,     3,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     2,     5,    11,     0,     1,     4,     0,
       6,    10,    13,     0,     0,     0,     0,     0,     0,    31,
      34,    35,    21,     0,     0,    23,    25,    28,    30,    12,
      18,    15,     0,     7,     0,     0,    29,     0,    32,    20,
       0,     0,     0,     0,    17,     0,     0,    33,    24,    26,
      27,     0,    16,     0,     8,     9,    19,    14
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     3,     4,     5,    10,    11,    12,    53,    32,
      13,    24,    25,    26,    27,    28
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -15
static const yytype_int8 yypact[] =
{
      -2,     3,    20,    -2,   -15,   -12,    27,   -15,   -15,    -3,
     -15,   -12,   -15,    -9,     5,    17,    22,    10,     0,   -15,
     -15,   -15,   -15,    23,    19,    25,    18,   -15,   -15,   -15,
      34,   -15,    24,   -15,    35,    36,   -15,    33,   -15,   -15,
       0,     0,     0,    31,    29,    30,    32,   -15,    18,   -15,
     -15,    41,   -15,    37,   -15,   -15,   -15,   -15
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -15,   -15,   -15,    45,   -15,   -15,   -15,    39,   -15,   -15,
     -15,    38,   -15,    11,   -14,    40
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -23
static const yytype_int8 yytable[] =
{
      17,   -22,     1,    17,    30,   -22,    18,     9,    31,    18,
      19,    20,    21,    19,    20,    21,     6,    22,    23,    18,
       7,    23,    33,    19,    20,    21,    41,    49,    50,    42,
      34,    23,    14,    15,    16,    35,    38,    40,    43,    39,
      45,    46,    44,    47,    51,    52,    56,    54,     8,    55,
      29,    48,     0,     0,    57,     0,    37,    36
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-15)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
       3,    13,     4,     3,    13,    17,     9,    19,    17,     9,
      13,    14,    15,    13,    14,    15,    13,    20,    21,     9,
       0,    21,    17,    13,    14,    15,     8,    41,    42,    11,
      13,    21,     5,     6,     7,    13,    13,    12,     4,    20,
       5,     5,    18,    10,    13,    16,     5,    17,     3,    17,
      11,    40,    -1,    -1,    17,    -1,    18,    17
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     4,    23,    24,    25,    26,    13,     0,    25,    19,
      27,    28,    29,    32,     5,     6,     7,     3,     9,    13,
      14,    15,    20,    21,    33,    34,    35,    36,    37,    29,
      13,    17,    31,    17,    13,    13,    37,    33,    13,    20,
      12,     8,    11,     4,    18,     5,     5,    10,    35,    36,
      36,    13,    16,    30,    17,    17,     5,    17
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

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


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

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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
		  Type, Value, Location); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
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
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
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
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule); \
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
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
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
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

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;


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
       `yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1792 of yacc.c  */
#line 125 "./src/driver/cxx-configfile-parser.y"
    {
}
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 128 "./src/driver/cxx-configfile-parser.y"
    {
}
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 133 "./src/driver/cxx-configfile-parser.y"
    {
}
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 136 "./src/driver/cxx-configfile-parser.y"
    {
}
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 141 "./src/driver/cxx-configfile-parser.y"
    {
    compilation_configuration_t* base_config = NULL;
    if ((yyvsp[(1) - (2)].profile_header).base_profile_name != NULL)
    {
        base_config = get_compilation_configuration((yyvsp[(1) - (2)].profile_header).base_profile_name);

        if (base_config == NULL)
        {
            fprintf(stderr, "%s:%d: warning: base configuration '%s' does not exist. Ignoring\n",
                    (yylsp[(1) - (2)]).filename, (yylsp[(1) - (2)]).first_line, (yyvsp[(1) - (2)].profile_header).base_profile_name);
        }
    }

    // fprintf(stderr, "--> PROCESSING SECTION ['%s' : '%s']\n", 
    //         $1.profile_name, 
    //         $1.base_profile_name == NULL ? "" : $1.base_profile_name);

    compilation_configuration_t *new_configuration = new_compilation_configuration((yyvsp[(1) - (2)].profile_header).profile_name, base_config);

    new_configuration->num_configuration_lines = (yyvsp[(2) - (2)].option_list).num_options;
    new_configuration->configuration_lines = (yyvsp[(2) - (2)].option_list).options;

    new_configuration->type_environment = default_environment;
    new_configuration->fortran_array_descriptor = default_fortran_array_descriptor;
    new_configuration->fortran_name_mangling = default_fortran_name_mangling;
    new_configuration->print_vector_type = print_gnu_vector_type;

    if (get_compilation_configuration((yyvsp[(1) - (2)].profile_header).profile_name) != NULL)
    {
        fprintf(stderr, "%s:%d: warning: configuration profile '%s' already exists. First one defined will be used!\n",
                (yylsp[(1) - (2)]).filename, (yylsp[(1) - (2)]).first_line,
                (yyvsp[(1) - (2)].profile_header).profile_name);
    }

    P_LIST_ADD(compilation_process.configuration_set, 
            compilation_process.num_configurations, 
            new_configuration);
}
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 182 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.profile_header).profile_name = (yyvsp[(2) - (4)].str);
    (yyval.profile_header).base_profile_name = NULL;
}
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 187 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.profile_header).profile_name = (yyvsp[(2) - (6)].str);
    (yyval.profile_header).base_profile_name = (yyvsp[(4) - (6)].str);
}
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 192 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.profile_header).profile_name = (yyvsp[(2) - (6)].str);
    (yyval.profile_header).base_profile_name = (yyvsp[(4) - (6)].str);
}
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 199 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.option_list) = (yyvsp[(1) - (1)].option_list);
}
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 204 "./src/driver/cxx-configfile-parser.y"
    {
    new_option_list(&(yyval.option_list));
}
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 210 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.option_list) = (yyvsp[(1) - (2)].option_list);
    if ((yyvsp[(2) - (2)].profile_option) != NULL)
    {
        add_to_option_list(&(yyval.option_list), (yyvsp[(2) - (2)].profile_option));
    }
}
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 218 "./src/driver/cxx-configfile-parser.y"
    {
    new_option_list(&(yyval.option_list));
    if ((yyvsp[(1) - (1)].profile_option) != NULL)
    {
        add_to_option_list(&(yyval.option_list), (yyvsp[(1) - (1)].profile_option));
    }
}
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 229 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.profile_option) = process_option_line((yyvsp[(1) - (5)].flag), &(yyvsp[(2) - (5)].profile_option_name), (yyvsp[(4) - (5)].str), (yylsp[(2) - (5)]).filename, (yylsp[(2) - (5)]).first_line);
}
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 234 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.profile_option) = NULL;
}
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 240 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.str) = (yyvsp[(1) - (1)].str);
}
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 244 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.str) = "";
}
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 250 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.profile_option_name).option_name = (yyvsp[(1) - (1)].str);
    (yyval.profile_option_name).option_index = NULL;
}
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 255 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.profile_option_name).option_name = (yyvsp[(1) - (4)].str);
    (yyval.profile_option_name).option_index = (yyvsp[(3) - (4)].str);
}
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 262 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.flag) = (yyvsp[(2) - (3)].flag);
}
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 267 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.flag) = NULL;
}
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 272 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.flag) = NULL;
}
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 278 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.flag) = (yyvsp[(1) - (1)].flag);
}
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 284 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.flag) = flag_or((yyvsp[(1) - (3)].flag), (yyvsp[(3) - (3)].flag));
}
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 288 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.flag) = (yyvsp[(1) - (1)].flag);
}
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 294 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.flag) = flag_and((yyvsp[(1) - (3)].flag), (yyvsp[(3) - (3)].flag));
}
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 298 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.flag) = flag_and((yyvsp[(1) - (3)].flag), (yyvsp[(3) - (3)].flag));
}
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 302 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.flag) = (yyvsp[(1) - (1)].flag);
}
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 308 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.flag) = flag_not((yyvsp[(2) - (2)].flag));
}
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 312 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.flag) = (yyvsp[(1) - (1)].flag);
}
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 318 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.flag) = flag_name((yyvsp[(1) - (1)].str));
}
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 322 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.flag) = flag_is_defined((yyvsp[(2) - (2)].str));
}
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 326 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.flag) = (yyvsp[(2) - (3)].flag);
}
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 330 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.flag) = flag_true();
}
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 334 "./src/driver/cxx-configfile-parser.y"
    {
    (yyval.flag) = flag_false();
}
    break;


/* Line 1792 of yacc.c  */
#line 1881 "src/driver/cxx-configfile-parser.c"
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
  *++yylsp = yyloc;

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

  yyerror_range[1] = yylloc;

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
		      yytoken, &yylval, &yylloc);
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

  yyerror_range[1] = yylsp[1-yylen];
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

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

#if !defined yyoverflow || YYERROR_VERBOSE
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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp);
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


/* Line 2055 of yacc.c  */
#line 339 "./src/driver/cxx-configfile-parser.y"


static void new_option_list(option_list_t* list)
{
    memset(list, 0, sizeof(*list));
}

static void add_to_option_list(option_list_t* list, p_compilation_configuration_line p_line)
{
    P_LIST_ADD(list->options, list->num_options, p_line);
}

void yyerror(const char *c)
{
  fprintf(
      stderr,
      "%s:%d:%d: warning: error encountered when parsing configuration file: %s\n",
      yylloc.filename, yylloc.first_line, yylloc.first_column, c);
}

static void register_implicit_names(flag_expr_t* flag_expr)
{
    if (flag_expr != NULL)
    {
        if (flag_expr->kind == FLAG_OP_NAME
            || flag_expr->kind == FLAG_OP_IS_DEFINED)
        {
            parameter_flags_t *new_parameter_flag = NEW0(parameter_flags_t);

            new_parameter_flag->name = flag_expr->text;
            new_parameter_flag->value = PFV_UNDEFINED;

            P_LIST_ADD(compilation_process.parameter_flags, 
                    compilation_process.num_parameter_flags,
                    new_parameter_flag);
        }
        else 
        {
            register_implicit_names(flag_expr->op[0]);
            register_implicit_names(flag_expr->op[1]);
        }
    }
}

static flag_expr_t* new_flag(void)
{
    flag_expr_t* result = NEW0(flag_expr_t);
    return result;
}

static flag_expr_t* flag_true(void)
{
    flag_expr_t* result = new_flag();

    result->kind = FLAG_OP_TRUE;

    return result;
}

static flag_expr_t* flag_false(void)
{
    flag_expr_t* result = new_flag();

    result->kind = FLAG_OP_FALSE;

    return result;
}

static flag_expr_t* flag_name(const char* name)
{
    flag_expr_t* result = new_flag();

    result->kind = FLAG_OP_NAME;
    result->text = name;

    return result;
}

static flag_expr_t* flag_is_defined(const char* name)
{
    flag_expr_t* result = new_flag();

    result->kind = FLAG_OP_IS_DEFINED;
    result->text = name;

    return result;
}

static flag_expr_t* flag_not(flag_expr_t* op)
{
    flag_expr_t* result = new_flag();

    result->kind = FLAG_OP_NOT;
    result->op[0] = op;

    return result;
}

static flag_expr_t* flag_and(flag_expr_t* op1, flag_expr_t* op2)
{
    flag_expr_t* result = new_flag();

    result->kind = FLAG_OP_AND;
    result->op[0] = op1;
    result->op[1] = op2;

    return result;
}

static flag_expr_t* flag_or(flag_expr_t* op1, flag_expr_t* op2)
{
    flag_expr_t* result = new_flag();

    result->kind = FLAG_OP_OR;
    result->op[0] = op1;
    result->op[1] = op2;

    return result;
}

static p_compilation_configuration_line process_option_line(
        flag_expr_t* flag_expr,
        profile_option_name_t* name,
        const char* option_value,
        const char* filename,
        int line)
{
    p_compilation_configuration_line result;
    char *duplicated_option_value = xstrdup(option_value);
    char *fixed_option_value = duplicated_option_value;
    {
        if (*fixed_option_value == '"')
        {
            fixed_option_value++;
            char *p = &fixed_option_value[strlen(fixed_option_value) - 1];
            while (p >= fixed_option_value
                    && (*p == ' ' || *p == '\t'))
            {
                p--;
            }

            if(p >= fixed_option_value && *p == '"')
            {
                *p = '\0';
            }
            else
            {
                fprintf(stderr,"%s:%d: warning: missing '\"' in '%s' string value\n", filename, line, option_value);
            }

            // We do not trim the content of the string
        }
        else
        {
            // Trim the option value
            char *p = &fixed_option_value[strlen(fixed_option_value) - 1];
            while (p >= fixed_option_value
                    && (*p == ' ' || *p == '\t'))
            {
                *p = '\0';
                p--;
            }
        }
    }

    result = NEW0(compilation_configuration_line_t);

    result->name = uniquestr(name->option_name);
    result->index = uniquestr(name->option_index);
    result->value = uniquestr(fixed_option_value);
    DELETE(duplicated_option_value);

    result->flag_expr = flag_expr;
    result->filename = filename;
    result->line = line;

#if 0
    fprintf(stderr, "LINE: |%s = %s| at %s:%d\n", result->name, result->value, result->filename, result->line);
#endif

    register_implicit_names(flag_expr);

    return result;
}

#if 0
const char* flag_expr_print(flag_expr_t* flag_expr)
{
    switch (flag_expr->kind)
    {
        case FLAG_OP_NAME:
            {
                return flag_expr->text;
            }
        case FLAG_OP_IS_DEFINED:
            {
                return strappend(strappend("(?", flag_expr->text), ")");
            }
        case FLAG_OP_NOT:
            {
                return strappend(strappend("(!", flag_expr_print(flag_expr->op[0])), ")");
            }
        case FLAG_OP_OR:
            {
                return strappend(strappend("(", strappend(strappend(flag_expr_print(flag_expr->op[0]), " || "), flag_expr_print(flag_expr->op[1]))), ")");
            }
        case FLAG_OP_AND:
            {
                return strappend(strappend("(", strappend(strappend(flag_expr_print(flag_expr->op[0]), " && "), flag_expr_print(flag_expr->op[1]))), ")");
            }
        case FLAG_OP_TRUE:
            {
                return "true";
            }
        case FLAG_OP_FALSE:
            {
                return "false";
            }
        default:
            {
                internal_error("Invalid flag expr", 0);
            }
    }
    return "invalid";
}
#endif

char flag_expr_eval(flag_expr_t* flag_expr)
{
    char result = 0;
    switch (flag_expr->kind)
    {
        case FLAG_OP_NAME:
        case FLAG_OP_IS_DEFINED:
            {
                // Ugly and inefficient lookup
                char found = 0;
                parameter_flag_value_t value_of_flag = PFV_UNDEFINED;
                int q;
                for (q = 0; !found && q < compilation_process.num_parameter_flags; q++)
                {
                    parameter_flags_t *parameter_flag = compilation_process.parameter_flags[q];
                    found = (strcmp(parameter_flag->name, flag_expr->text) == 0);
                    value_of_flag = parameter_flag->value;
                }

                if (flag_expr->kind == FLAG_OP_NAME)
                {
                    result = (value_of_flag == PFV_TRUE);
                }
                else if (flag_expr->kind == FLAG_OP_IS_DEFINED)
                {
                    result = (value_of_flag != PFV_UNDEFINED);
                }
                else
                {
                    internal_error("Code unreachable", 0);
                }
                break;
            }
        case FLAG_OP_NOT:
            {
                result = !flag_expr_eval(flag_expr->op[0]);
                break;
            }
        case FLAG_OP_OR:
            {
                result = flag_expr_eval(flag_expr->op[0]) || flag_expr_eval(flag_expr->op[1]);
                break;
            }
        case FLAG_OP_AND:
            {
                result = flag_expr_eval(flag_expr->op[0]) && flag_expr_eval(flag_expr->op[1]);
                break;
            }
        case FLAG_OP_TRUE:
            {
                result = 1;
                break;
            }
        case FLAG_OP_FALSE:
            {
                result = 0;
                break;
            }
        default:
            {
                internal_error("Invalid flag expr", 0);
            }
    }
    //printf("FLAG EVAL: '%s' -> %d\n", flag_expr_print(flag_expr), result);
    return result;
}
