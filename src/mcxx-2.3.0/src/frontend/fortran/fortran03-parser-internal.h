/* A Bison parser, made by GNU Bison 2.7.  */

/* Skeleton interface for Bison GLR parsers in C
   
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

#ifndef YY_MF03_SRC_FRONTEND_FORTRAN_FORTRAN03_PARSER_INTERNAL_H_INCLUDED
# define YY_MF03_SRC_FRONTEND_FORTRAN_FORTRAN03_PARSER_INTERNAL_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int mf03debug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     END = 0,
     TOKEN_FORMAT = 258,
     TOKEN_GENERIC = 259,
     TOKEN_VOLATILE = 260,
     TOKEN_EQUIVALENCE = 261,
     TOKEN_EOR = 262,
     TOKEN_PROTECTED = 263,
     TOKEN_PAD = 264,
     TOKEN_FILE = 265,
     TOKEN_UNFORMATTED = 266,
     TOKEN_NAMED = 267,
     TOKEN_SUBROUTINE = 268,
     TOKEN_OPENED = 269,
     TOKEN_DELIM = 270,
     TOKEN_SOURCE = 271,
     TOKEN_CRITICAL = 272,
     TOKEN_ACTION = 273,
     TOKEN_IMPORT = 274,
     TOKEN_BLOCK = 275,
     TOKEN_ENUM = 276,
     TOKEN_OVERRIDABLE = 277,
     TOKEN_MODULE = 278,
     TOKEN_SIGN = 279,
     TOKEN_EXTENDS = 280,
     TOKEN_PASS = 281,
     TOKEN_GO = 282,
     TOKEN_ENTRY = 283,
     TOKEN_WHERE = 284,
     TOKEN_ENDWHERE = 285,
     TOKEN_PROCEDURE = 286,
     TOKEN_NAME = 287,
     TOKEN_DOUBLE = 288,
     TOKEN_CONTAINS = 289,
     TOKEN_LOGICAL = 290,
     TOKEN_NEWUNIT = 291,
     TOKEN_NULLIFY = 292,
     TOKEN_DEFERRED = 293,
     TOKEN_ONLY = 294,
     TOKEN_EXIST = 295,
     TOKEN_INTERFACE = 296,
     TOKEN_SAVE = 297,
     TOKEN_ADVANCE = 298,
     TOKEN_STAT = 299,
     TOKEN_RETURN = 300,
     TOKEN_READWRITE = 301,
     TOKEN_ASSIGNMENT = 302,
     TOKEN_ASSIGN = 303,
     TOKEN_PRINT = 304,
     TOKEN_IOSTAT = 305,
     TOKEN_SYNC = 306,
     TOKEN_COMPLEX = 307,
     TOKEN_ASYNCHRONOUS = 308,
     TOKEN_IMAGES = 309,
     TOKEN_END = 310,
     TOKEN_FINAL = 311,
     TOKEN_INOUT = 312,
     TOKEN_NEXTREC = 313,
     TOKEN_KIND = 314,
     TOKEN_COMMON = 315,
     TOKEN_BLANK = 316,
     TOKEN_IOLENGTH = 317,
     TOKEN_INTEGER = 318,
     TOKEN_DEALLOCATE = 319,
     TOKEN_FUNCTION = 320,
     TOKEN_C = 321,
     TOKEN_OPENCL = 322,
     TOKEN_ENUMERATOR = 323,
     TOKEN_RECURSIVE = 324,
     TOKEN_SEQUENCE = 325,
     TOKEN_INQUIRE = 326,
     TOKEN_FLUSH = 327,
     TOKEN_ELSE = 328,
     TOKEN_INTENT = 329,
     TOKEN_EXTERNAL = 330,
     TOKEN_OPERATOR = 331,
     TOKEN_OPTIONAL = 332,
     TOKEN_UNIT = 333,
     TOKEN_SIZE = 334,
     TOKEN_NOPASS = 335,
     TOKEN_TYPE = 336,
     TOKEN_MOLD = 337,
     TOKEN_PRECISION = 338,
     TOKEN_PENDING = 339,
     TOKEN_CONTINUE = 340,
     TOKEN_RESULT = 341,
     TOKEN_REAL = 342,
     TOKEN_THEN = 343,
     TOKEN_STREAM = 344,
     TOKEN_DO = 345,
     TOKEN_DEFAULT = 346,
     TOKEN_CONTIGUOUS = 347,
     TOKEN_STOP = 348,
     TOKEN_PAUSE = 349,
     TOKEN_WHILE = 350,
     TOKEN_PROGRAM = 351,
     TOKEN_REC = 352,
     TOKEN_OPEN = 353,
     TOKEN_CASE = 354,
     TOKEN_RECL = 355,
     TOKEN_DIMENSION = 356,
     TOKEN_ELEMENTAL = 357,
     TOKEN_FORALL = 358,
     TOKEN_POSITION = 359,
     TOKEN_FMT = 360,
     TOKEN_READ = 361,
     TOKEN_OUT = 362,
     TOKEN_LOCK = 363,
     TOKEN_DECIMAL = 364,
     TOKEN_SELECT = 365,
     TOKEN_SELECTCASE = 366,
     TOKEN_DIRECT = 367,
     TOKEN_WRITE = 368,
     TOKEN_CODIMENSION = 369,
     TOKEN_CLOSE = 370,
     TOKEN_ID = 371,
     TOKEN_WAIT = 372,
     TOKEN_ALL = 373,
     TOKEN_ELSEWHERE = 374,
     TOKEN_LEN = 375,
     TOKEN_IMPURE = 376,
     TOKEN_EXIT = 377,
     TOKEN_REWIND = 378,
     TOKEN_BACKSPACE = 379,
     TOKEN_NML = 380,
     TOKEN_INTRINSIC = 381,
     TOKEN_PUBLIC = 382,
     TOKEN_ERRMSG = 383,
     TOKEN_NAMELIST = 384,
     TOKEN_USE = 385,
     TOKEN_SUBMODULE = 386,
     TOKEN_ERR = 387,
     TOKEN_ENCODING = 388,
     TOKEN_ACQUIRED = 389,
     TOKEN_ABSTRACT = 390,
     TOKEN_CHARACTER = 391,
     TOKEN_NUMBER = 392,
     TOKEN_CONCURRENT = 393,
     TOKEN_ENDFILE = 394,
     TOKEN_PARAMETER = 395,
     TOKEN_DATA = 396,
     TOKEN_POS = 397,
     TOKEN_PRIVATE = 398,
     TOKEN_ROUND = 399,
     TOKEN_TO = 400,
     TOKEN_SEQUENTIAL = 401,
     TOKEN_ALLOCATE = 402,
     TOKEN_TARGET = 403,
     TOKEN_CLASS = 404,
     TOKEN_CYCLE = 405,
     TOKEN_STATUS = 406,
     TOKEN_IOMSG = 407,
     TOKEN_FORM = 408,
     TOKEN_IS = 409,
     TOKEN_NONE = 410,
     TOKEN_VALUE = 411,
     TOKEN_UNLOCK = 412,
     TOKEN_FORMATTED = 413,
     TOKEN_IN = 414,
     TOKEN_IMPLICIT = 415,
     TOKEN_IF = 416,
     TOKEN_ASSOCIATE = 417,
     TOKEN_BIND = 418,
     TOKEN_ALLOCATABLE = 419,
     TOKEN_ACCESS = 420,
     TOKEN_CALL = 421,
     TOKEN_CONVERT = 422,
     TOKEN_BUFFERED = 423,
     TOKEN_PURE = 424,
     TOKEN_MEMORY = 425,
     TOKEN_POINTER = 426,
     TOKEN_VECTOR = 427,
     TOKEN_PIXEL = 428,
     TOKEN_NON_OVERRIDABLE = 429,
     TOKEN_NON_INTRINSIC = 430,
     TOKEN_HOLLERITH_CONSTANT = 431,
     TOKEN_ALLSTOP = 432,
     TOKEN_ENDDO = 433,
     TOKEN_ENDIF = 434,
     TOKEN_ENDFUNCTION = 435,
     TOKEN_BLOCKDATA = 436,
     TOKEN_ENDMODULE = 437,
     TOKEN_DOUBLEPRECISION = 438,
     TOKEN_ENDINTERFACE = 439,
     TOKEN_ELSEIF = 440,
     TOKEN_ENDPROCEDURE = 441,
     TOKEN_ENDPROGRAM = 442,
     TOKEN_ENDASSOCIATE = 443,
     TOKEN_ENDSELECT = 444,
     TOKEN_ENDBLOCK = 445,
     TOKEN_ENDSUBMODULE = 446,
     TOKEN_ENDBLOCKDATA = 447,
     TOKEN_ENDSUBROUTINE = 448,
     TOKEN_ENDCRITICAL = 449,
     TOKEN_ENDTYPE = 450,
     TOKEN_DOUBLECOMPLEX = 451,
     TOKEN_GOTO = 452,
     TOKEN_LOGICAL_NOT_EQUIVALENT = 453,
     TOKEN_POINTER_ACCESS = 454,
     TOKEN_GREATER_THAN = 455,
     TOKEN_EQUAL = 456,
     TOKEN_LOGICAL_AND = 457,
     TOKEN_LOWER_THAN = 458,
     TOKEN_LOGICAL_NOT = 459,
     TOKEN_LOGICAL_EQUIVALENT = 460,
     TOKEN_GREATER_OR_EQUAL_THAN = 461,
     TOKEN_LOGICAL_OR = 462,
     TOKEN_LOWER_OR_EQUAL_THAN = 464,
     TOKEN_NOT_EQUAL = 465,
     TOKEN_RAISE = 466,
     TOKEN_DOUBLE_SLASH = 467,
     TOKEN_LPARENT_SLASH = 468,
     TOKEN_SLASH_RPARENT = 469,
     TOKEN_TRUE = 470,
     TOKEN_FALSE = 471,
     DECIMAL_LITERAL = 472,
     FORMAT_SPEC = 473,
     IDENTIFIER = 474,
     EOS = 475,
     BINARY_LITERAL = 476,
     OCTAL_LITERAL = 477,
     REAL_LITERAL = 478,
     CHAR_LITERAL = 479,
     HEX_LITERAL = 480,
     USER_DEFINED_OPERATOR = 481,
     TOKEN_END_NONBLOCK_DO = 482,
     TOKEN_SHARED_LABEL = 483,
     GLOBAL = 484,
     END_GLOBAL = 485,
     TYPEDEF = 486,
     TOKEN_IS_VARIABLE = 487,
     SUBPARSE_PROGRAM_UNIT = 488,
     SUBPARSE_EXPRESSION = 489,
     SUBPARSE_STATEMENT = 490,
     NODECL_LITERAL_EXPR = 491,
     NODECL_LITERAL_STMT = 492,
     SYMBOL_LITERAL_REF = 493,
     TYPE_LITERAL_REF = 494,
     UNKNOWN_PRAGMA = 495,
     PRAGMA_CUSTOM = 496,
     PRAGMA_CUSTOM_NEWLINE = 497,
     PRAGMA_CUSTOM_DIRECTIVE = 498,
     PRAGMA_CUSTOM_CONSTRUCT = 499,
     PRAGMA_CUSTOM_END_CONSTRUCT = 500,
     PRAGMA_CUSTOM_CONSTRUCT_NOEND = 501,
     PRAGMA_CUSTOM_END_CONSTRUCT_NOEND = 502,
     PRAGMA_CUSTOM_CLAUSE = 503,
     PRAGMA_CLAUSE_ARG_TEXT = 504,
     STATEMENT_PLACEHOLDER = 505,
     SUBPARSE_OPENMP_DECLARE_REDUCTION = 506,
     SUBPARSE_OPENMP_DEPEND_ITEM = 507,
     SUBPARSE_OMPSS_DEPENDENCY_EXPRESSION = 508
   };
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2579 of glr.c  */
#line 93 "src/frontend/fortran/fortran03.y"

	token_atrib_t token_atrib;
	AST ast;
	AST ast2[2];
	AST ast3[3];
	AST ast4[4];
	node_t node_type;
    const char *text;


/* Line 2579 of glr.c  */
#line 320 "src/frontend/fortran/fortran03-parser-internal.h"
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

extern YYSTYPE mf03lval;
extern YYLTYPE mf03lloc;
int mf03parse (AST* parsed_tree);

#endif /* !YY_MF03_SRC_FRONTEND_FORTRAN_FORTRAN03_PARSER_INTERNAL_H_INCLUDED  */
