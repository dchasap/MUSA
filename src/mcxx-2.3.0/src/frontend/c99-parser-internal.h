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

#ifndef YY_MC99_SRC_FRONTEND_C99_PARSER_INTERNAL_H_INCLUDED
# define YY_MC99_SRC_FRONTEND_C99_PARSER_INTERNAL_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int mc99debug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     END = 0,
     ADD_ASSIGN = 258,
     ANDAND = 259,
     AND_ASSIGN = 260,
     ASM = 261,
     TOKEN_AUTO_STORAGE = 262,
     TOKEN_BOOL = 263,
     BOOLEAN_LITERAL = 264,
     BREAK = 265,
     CASE = 266,
     TOKEN_CHAR = 267,
     CHARACTER_LITERAL = 268,
     TOKEN_CONST = 269,
     CONTINUE = 270,
     DECIMAL_LITERAL = 271,
     DEFAULT = 272,
     DIV_ASSIGN = 273,
     DO = 274,
     TOKEN_DOUBLE = 275,
     ELSE = 276,
     ENUM = 277,
     EQUAL = 278,
     EXTERN = 279,
     TOKEN_FLOAT = 280,
     FLOATING_LITERAL = 281,
     HEXADECIMAL_FLOAT = 282,
     FOR = 283,
     GOTO = 284,
     GREATER_OR_EQUAL = 285,
     BINARY_LITERAL = 286,
     HEXADECIMAL_LITERAL = 287,
     IDENTIFIER = 288,
     IF = 289,
     INLINE = 290,
     TOKEN_INT = 291,
     TOKEN_INT128 = 292,
     TOKEN_FLOAT128 = 293,
     LEFT = 294,
     LEFT_ASSIGN = 295,
     LESS_OR_EQUAL = 296,
     TOKEN_LONG = 297,
     MINUSMINUS = 298,
     MOD_ASSIGN = 299,
     MUL_ASSIGN = 300,
     NOT_EQUAL = 301,
     OCTAL_LITERAL = 302,
     OR_ASSIGN = 303,
     OROR = 304,
     PLUSPLUS = 305,
     PTR_OP = 306,
     REGISTER = 307,
     RETURN = 308,
     RIGHT = 309,
     RIGHT_ASSIGN = 310,
     TOKEN_SHORT = 311,
     TOKEN_SIGNED = 312,
     SIZEOF = 313,
     STATIC = 314,
     STRING_LITERAL = 315,
     STRUCT = 316,
     SUB_ASSIGN = 317,
     SWITCH = 318,
     ELLIPSIS = 319,
     TYPEDEF = 320,
     UNION = 321,
     TOKEN_UNSIGNED = 322,
     TOKEN_VOID = 323,
     TOKEN_VOLATILE = 324,
     WHILE = 325,
     XOR_ASSIGN = 326,
     PP_COMMENT = 327,
     PP_TOKEN = 328,
     MCC_REFERENCE = 329,
     MCC_REBINDABLE_REFERENCE = 330,
     TOKEN_ATOMIC = 331,
     TOKEN_GENERIC = 332,
     TOKEN_NORETURN = 333,
     STATIC_ASSERT = 334,
     TOKEN_THREAD_LOCAL = 335,
     BUILTIN_VA_ARG = 336,
     BUILTIN_OFFSETOF = 337,
     BUILTIN_CHOOSE_EXPR = 338,
     BUILTIN_TYPES_COMPATIBLE_P = 339,
     TOKEN_GCC_ALIGNOF = 340,
     EXTENSION = 341,
     REAL = 342,
     IMAG = 343,
     LABEL = 344,
     COMPLEX = 345,
     IMAGINARY = 346,
     TYPEOF = 347,
     RESTRICT = 348,
     TOKEN_GCC_ATTRIBUTE = 349,
     THREAD = 350,
     SUBPARSE_EXPRESSION = 351,
     SUBPARSE_EXPRESSION_LIST = 352,
     SUBPARSE_STATEMENT = 353,
     SUBPARSE_DECLARATION = 354,
     SUBPARSE_MEMBER = 355,
     SUBPARSE_TYPE = 356,
     SUBPARSE_TYPE_LIST = 357,
     SUBPARSE_ID_EXPRESSION = 358,
     NODECL_LITERAL_EXPR = 359,
     NODECL_LITERAL_STMT = 360,
     SYMBOL_LITERAL_REF = 361,
     TYPE_LITERAL_REF = 362,
     MCC_BYTE = 363,
     MCC_BOOL = 364,
     MCC_MASK = 365,
     MCC_ARRAY_SUBSCRIPT_CHECK = 366,
     MCC_CONST_VALUE_CHECK = 367,
     C_FORTRAN_ALLOCATE = 368,
     STATEMENT_PLACEHOLDER = 369,
     UNKNOWN_PRAGMA = 370,
     VERBATIM_PRAGMA = 371,
     VERBATIM_CONSTRUCT = 372,
     VERBATIM_TYPE = 373,
     VERBATIM_TEXT = 374,
     PRAGMA_CUSTOM = 375,
     PRAGMA_CUSTOM_NEWLINE = 376,
     PRAGMA_CUSTOM_DIRECTIVE = 377,
     PRAGMA_CUSTOM_CONSTRUCT = 378,
     PRAGMA_CUSTOM_END_CONSTRUCT = 379,
     PRAGMA_CUSTOM_CONSTRUCT_NOEND = 380,
     PRAGMA_CUSTOM_END_CONSTRUCT_NOEND = 381,
     PRAGMA_CUSTOM_CLAUSE = 382,
     PRAGMA_CLAUSE_ARG_TEXT = 383,
     SUBPARSE_OPENMP_DECLARE_REDUCTION = 384,
     SUBPARSE_OPENMP_DEPEND_ITEM = 385,
     SUBPARSE_OMPSS_DEPENDENCY_EXPRESSION = 386,
     TWO_DOTS = 387,
     SUBPARSE_SUPERSCALAR_DECLARATOR = 388,
     SUBPARSE_SUPERSCALAR_DECLARATOR_LIST = 389,
     SUBPARSE_SUPERSCALAR_EXPRESSION = 390,
     UPC_MYTHREAD = 391,
     UPC_RELAXED = 392,
     UPC_SHARED = 393,
     UPC_STRICT = 394,
     UPC_THREADS = 395,
     UPC_BARRIER = 396,
     UPC_BLOCKSIZEOF = 397,
     UPC_ELEMSIZEOF = 398,
     UPC_FENCE = 399,
     UPC_FORALL = 400,
     UPC_LOCALSIZEOF = 401,
     UPC_MAX_BLOCKSIZE = 402,
     UPC_NOTIFY = 403,
     UPC_WAIT = 404,
     CUDA_DEVICE = 405,
     CUDA_GLOBAL = 406,
     CUDA_HOST = 407,
     CUDA_CONSTANT = 408,
     CUDA_SHARED = 409,
     CUDA_KERNEL_LEFT = 410,
     CUDA_KERNEL_RIGHT = 411,
     OPENCL_GLOBAL = 412,
     OPENCL_KERNEL = 413,
     OPENCL_CONSTANT = 414,
     OPENCL_LOCAL = 415,
     XL_BUILTIN_SPEC = 416,
     TOKEN_DECLSPEC = 417,
     MS_INT8 = 418,
     MS_INT16 = 419,
     MS_INT32 = 420,
     MS_INT64 = 421,
     INTEL_ASSUME = 422,
     INTEL_ASSUME_ALIGNED = 423
   };
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2579 of glr.c  */
#line 58 "src/frontend/c99.y"

	token_atrib_t token_atrib;
	AST ast;
	AST ast2[2];
	AST ast3[3];
	AST ast4[4];
	node_t node_type;
    const char *text;


/* Line 2579 of glr.c  */
#line 236 "src/frontend/c99-parser-internal.h"
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

extern YYSTYPE mc99lval;
extern YYLTYPE mc99lloc;
int mc99parse (AST* parsed_tree);

#endif /* !YY_MC99_SRC_FRONTEND_C99_PARSER_INTERNAL_H_INCLUDED  */
