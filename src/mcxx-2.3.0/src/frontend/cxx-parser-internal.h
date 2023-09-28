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

#ifndef YY_MCXX_SRC_FRONTEND_CXX_PARSER_INTERNAL_H_INCLUDED
# define YY_MCXX_SRC_FRONTEND_CXX_PARSER_INTERNAL_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int mcxxdebug;
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
     TOKEN_AUTO_TYPE = 263,
     TOKEN_BOOL = 264,
     BOOLEAN_LITERAL = 265,
     BREAK = 266,
     CASE = 267,
     CATCH = 268,
     TOKEN_CHAR = 269,
     CHARACTER_LITERAL = 270,
     CLASS = 271,
     TOKEN_CONST = 272,
     CONST_CAST = 273,
     CONTINUE = 274,
     DECIMAL_LITERAL = 275,
     DEFAULT = 276,
     TOKEN_DELETE = 277,
     TOKEN_DELETE_ARRAY = 278,
     DIV_ASSIGN = 279,
     DO = 280,
     TWO_COLONS = 281,
     TWO_COLONS_AND_STAR = 282,
     TOKEN_DOUBLE = 283,
     DYNAMIC_CAST = 284,
     ELSE = 285,
     ENUM = 286,
     EQUAL = 287,
     DECLTYPE = 288,
     EXPLICIT = 289,
     EXPORT = 290,
     EXTERN = 291,
     TOKEN_FLOAT = 292,
     FLOATING_LITERAL = 293,
     HEXADECIMAL_FLOAT = 294,
     FOR = 295,
     FRIEND = 296,
     GOTO = 297,
     BINARY_LITERAL = 298,
     HEXADECIMAL_LITERAL = 299,
     IDENTIFIER = 300,
     TOK_FINAL = 301,
     TOK_OVERRIDE = 302,
     IF = 303,
     INLINE = 304,
     TOKEN_INT = 305,
     TOKEN_INT128 = 306,
     TOKEN_FLOAT128 = 307,
     LEFT = 308,
     LEFT_ASSIGN = 309,
     LESS_OR_EQUAL = 310,
     TOKEN_LONG = 311,
     MINUSMINUS = 312,
     MOD_ASSIGN = 313,
     MUL_ASSIGN = 314,
     MUTABLE = 315,
     NAMESPACE = 316,
     TOKEN_NEW = 317,
     TOKEN_NEW_ARRAY = 318,
     NOT_EQUAL = 319,
     OCTAL_LITERAL = 320,
     OPERATOR = 321,
     OPERATOR_LITERAL = 322,
     OR_ASSIGN = 323,
     OROR = 324,
     PLUSPLUS = 325,
     PRIVATE = 326,
     PROTECTED = 327,
     PTR_OP = 328,
     PTR_OP_MUL = 329,
     PUBLIC = 330,
     REGISTER = 331,
     REINTERPRET_CAST = 332,
     RETURN = 333,
     TOKEN_SHORT = 334,
     TOKEN_SIGNED = 335,
     SIZEOF = 336,
     STATIC = 337,
     STATIC_CAST = 338,
     STRING_LITERAL = 339,
     STRUCT = 340,
     SUB_ASSIGN = 341,
     SWITCH = 342,
     TEMPLATE = 343,
     TOKEN_THIS = 344,
     THROW = 345,
     ELLIPSIS = 346,
     TRY = 347,
     TYPEDEF = 348,
     TYPEID = 349,
     TYPENAME = 350,
     UNION = 351,
     TOKEN_UNSIGNED = 352,
     USING = 353,
     VIRTUAL = 354,
     TOKEN_VOID = 355,
     TOKEN_VOLATILE = 356,
     TOKEN_WCHAR_T = 357,
     TOKEN_CHAR16_T = 358,
     TOKEN_CHAR32_T = 359,
     TOKEN_ALIGNOF = 360,
     TOKEN_ALIGNAS = 361,
     WHILE = 362,
     XOR_ASSIGN = 363,
     STATIC_ASSERT = 364,
     PP_COMMENT = 365,
     PP_TOKEN = 366,
     MCC_REBINDABLE_REFERENCE = 367,
     GXX_HAS_NOTHROW_ASSIGN = 368,
     GXX_HAS_NOTHROW_CONSTRUCTOR = 369,
     GXX_HAS_NOTHROW_COPY = 370,
     GXX_HAS_TRIVIAL_ASSIGN = 371,
     GXX_HAS_TRIVIAL_CONSTRUCTOR = 372,
     GXX_HAS_TRIVIAL_COPY = 373,
     GXX_HAS_TRIVIAL_DESTRUCTOR = 374,
     GXX_HAS_VIRTUAL_DESTRUCTOR = 375,
     GXX_IS_ABSTRACT = 376,
     GXX_IS_BASE_OF = 377,
     GXX_IS_CLASS = 378,
     GXX_IS_CONVERTIBLE_TO = 379,
     GXX_IS_EMPTY = 380,
     GXX_IS_ENUM = 381,
     GXX_IS_LITERAL_TYPE = 382,
     GXX_IS_POD = 383,
     GXX_IS_POLYMORPHIC = 384,
     GXX_IS_STANDARD_LAYOUT = 385,
     GXX_IS_TRIVIAL = 386,
     GXX_IS_UNION = 387,
     GXX_IS_FINAL = 388,
     GXX_UNDERLYING_TYPE = 389,
     GXX_IS_TRIVIALLY_ASSIGNABLE = 390,
     GXX_IS_TRIVIALLY_CONSTRUCTIBLE = 391,
     GXX_IS_TRIVIALLY_COPYABLE = 392,
     RIGHT = 393,
     RIGHT_ASSIGN = 394,
     GREATER_OR_EQUAL = 395,
     AB1 = 396,
     STD_ATTRIBUTE_START = 397,
     STD_ATTRIBUTE_END = 398,
     STD_ATTRIBUTE_TEXT = 399,
     TOKEN_CONSTEXPR = 400,
     TOKEN_THREAD_LOCAL = 401,
     TOKEN_NULLPTR = 402,
     TOKEN_NOEXCEPT = 403,
     TOKEN_NOEXCEPT_ALONE = 404,
     BUILTIN_VA_ARG = 405,
     BUILTIN_OFFSETOF = 406,
     BUILTIN_ADDRESSOF = 407,
     TOKEN_GCC_ALIGNOF = 408,
     EXTENSION = 409,
     REAL = 410,
     IMAG = 411,
     LABEL = 412,
     COMPLEX = 413,
     TYPEOF = 414,
     RESTRICT = 415,
     TOKEN_GCC_ATTRIBUTE = 416,
     THREAD = 417,
     SUBPARSE_EXPRESSION = 418,
     SUBPARSE_EXPRESSION_LIST = 419,
     SUBPARSE_STATEMENT = 420,
     SUBPARSE_DECLARATION = 421,
     SUBPARSE_MEMBER = 422,
     SUBPARSE_TYPE = 423,
     SUBPARSE_TYPE_LIST = 424,
     SUBPARSE_ID_EXPRESSION = 425,
     NODECL_LITERAL_EXPR = 426,
     NODECL_LITERAL_STMT = 427,
     SYMBOL_LITERAL_REF = 428,
     TYPE_LITERAL_REF = 429,
     MCC_BYTE = 430,
     MCC_BOOL = 431,
     MCC_MASK = 432,
     MCC_ARRAY_SUBSCRIPT_CHECK = 433,
     MCC_CONST_VALUE_CHECK = 434,
     C_FORTRAN_ALLOCATE = 435,
     STATEMENT_PLACEHOLDER = 436,
     UNKNOWN_PRAGMA = 437,
     VERBATIM_PRAGMA = 438,
     VERBATIM_CONSTRUCT = 439,
     VERBATIM_TYPE = 440,
     VERBATIM_TEXT = 441,
     PRAGMA_CUSTOM = 442,
     PRAGMA_CUSTOM_NEWLINE = 443,
     PRAGMA_CUSTOM_DIRECTIVE = 444,
     PRAGMA_CUSTOM_CONSTRUCT = 445,
     PRAGMA_CUSTOM_END_CONSTRUCT = 446,
     PRAGMA_CUSTOM_CONSTRUCT_NOEND = 447,
     PRAGMA_CUSTOM_END_CONSTRUCT_NOEND = 448,
     PRAGMA_CUSTOM_CLAUSE = 449,
     PRAGMA_CLAUSE_ARG_TEXT = 450,
     SUBPARSE_OPENMP_DECLARE_REDUCTION = 451,
     SUBPARSE_OPENMP_DEPEND_ITEM = 452,
     SUBPARSE_OMPSS_DEPENDENCY_EXPRESSION = 453,
     TWO_DOTS = 454,
     SUBPARSE_SUPERSCALAR_DECLARATOR = 455,
     SUBPARSE_SUPERSCALAR_DECLARATOR_LIST = 456,
     SUBPARSE_SUPERSCALAR_EXPRESSION = 457,
     CUDA_DEVICE = 458,
     CUDA_GLOBAL = 459,
     CUDA_HOST = 460,
     CUDA_CONSTANT = 461,
     CUDA_SHARED = 462,
     CUDA_KERNEL_LEFT = 463,
     CUDA_KERNEL_RIGHT = 464,
     OPENCL_GLOBAL = 465,
     OPENCL_KERNEL = 466,
     OPENCL_CONSTANT = 467,
     OPENCL_LOCAL = 468,
     XL_BUILTIN_SPEC = 469,
     TOKEN_DECLSPEC = 470,
     MS_INT8 = 471,
     MS_INT16 = 472,
     MS_INT32 = 473,
     MS_INT64 = 474,
     INTEL_ASSUME = 475,
     INTEL_ASSUME_ALIGNED = 476
   };
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2579 of glr.c  */
#line 56 "src/frontend/cxx03.y"

	token_atrib_t token_atrib;
	AST ast;
	AST ast2[2];
	AST ast3[3];
	AST ast4[4];
	node_t node_type;
    const char *text;


/* Line 2579 of glr.c  */
#line 289 "src/frontend/cxx-parser-internal.h"
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

extern YYSTYPE mcxxlval;
extern YYLTYPE mcxxlloc;
int mcxxparse (AST* parsed_tree);

#endif /* !YY_MCXX_SRC_FRONTEND_CXX_PARSER_INTERNAL_H_INCLUDED  */
