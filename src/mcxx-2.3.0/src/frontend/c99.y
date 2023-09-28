/*--------------------------------------------------------------------
  (C) Copyright 2006-2014 Barcelona Supercomputing Center
                          Centro Nacional de Supercomputacion
  
  This file is part of Mercurium C/C++ source-to-source compiler.
  
  See AUTHORS file in the top level directory for information 
  regarding developers and contributors.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.
  
  Mercurium C/C++ source-to-source compiler is distributed in the hope
  that it will be useful, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the GNU Lesser General Public License for more
  details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with Mercurium C/C++ source-to-source compiler; if
  not, write to the Free Software Foundation, Inc., 675 Mass Ave,
  Cambridge, MA 02139, USA.
--------------------------------------------------------------------*/


%{
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

%}

%glr-parser

%union {
	token_atrib_t token_atrib;
	AST ast;
	AST ast2[2];
	AST ast3[3];
	AST ast4[4];
	node_t node_type;
    const char *text;
};


// This is a specific feature of rofi-bison 2.1
// %default-merge <ambiguityHandler>

%parse-param {AST* parsed_tree}

%locations

%{
extern int yylex(void);
static AST ambiguityHandler (YYSTYPE x0, YYSTYPE x1);
void yyerror(AST* parsed_tree UNUSED_PARAMETER, const char* c);
%}

%token END 0 "end of input"

// C tokens
%token<token_atrib> ADD_ASSIGN "+="
%token<token_atrib> ANDAND "&&"
%token<token_atrib> AND_ASSIGN "&="
%token<token_atrib> ASM "__asm__"
%token<token_atrib> TOKEN_AUTO_STORAGE "auto storage-specifier"
%token<token_atrib> TOKEN_BOOL "_Bool"
%token<token_atrib> BOOLEAN_LITERAL "boolean-literal"
%token<token_atrib> BREAK "break"
%token<token_atrib> CASE "case"
%token<token_atrib> TOKEN_CHAR "char"
%token<token_atrib> CHARACTER_LITERAL "character-literal"
%token<token_atrib> TOKEN_CONST "const"
%token<token_atrib> CONTINUE "continue"
%token<token_atrib> DECIMAL_LITERAL "decimal-literal"
%token<token_atrib> DEFAULT "default"
%token<token_atrib> DIV_ASSIGN "/="
%token<token_atrib> DO "do"
%token<token_atrib> TOKEN_DOUBLE "double"
%token<token_atrib> ELSE "else"
%token<token_atrib> ENUM "enum"
%token<token_atrib> EQUAL "=="
%token<token_atrib> EXTERN "extern"
%token<token_atrib> TOKEN_FLOAT "float"
%token<token_atrib> FLOATING_LITERAL "floating-literal"
%token<token_atrib> HEXADECIMAL_FLOAT "hexadecimal-floating-literal"
%token<token_atrib> FOR "for"
%token<token_atrib> GOTO "goto"
%token<token_atrib> GREATER_OR_EQUAL ">="
%token<token_atrib> BINARY_LITERAL "binary-integer-literal"
%token<token_atrib> HEXADECIMAL_LITERAL "hexadecimal-integer-literal"
%token<token_atrib> IDENTIFIER "identifier"
%token<token_atrib> IF "if"
%token<token_atrib> INLINE "inline"
%token<token_atrib> TOKEN_INT "int"
%token<token_atrib> TOKEN_INT128 "__int128"
%token<token_atrib> TOKEN_FLOAT128 "__float128"
%token<token_atrib> LEFT "<<"
%token<token_atrib> LEFT_ASSIGN "<<="
%token<token_atrib> LESS_OR_EQUAL "<="
%token<token_atrib> TOKEN_LONG "long"
%token<token_atrib> MINUSMINUS "--"
%token<token_atrib> MOD_ASSIGN "%="
%token<token_atrib> MUL_ASSIGN "*="
%token<token_atrib> NOT_EQUAL "!="
%token<token_atrib> OCTAL_LITERAL "octal-integer-literal"
%token<token_atrib> OR_ASSIGN "|="
%token<token_atrib> OROR "||"
%token<token_atrib> PLUSPLUS "++"
%token<token_atrib> PTR_OP "->"
%token<token_atrib> REGISTER "register"
%token<token_atrib> RETURN "return"
%token<token_atrib> RIGHT ">>"
%token<token_atrib> RIGHT_ASSIGN ">>="
%token<token_atrib> TOKEN_SHORT "short"
%token<token_atrib> TOKEN_SIGNED "signed"
%token<token_atrib> SIZEOF "sizeof"
%token<token_atrib> STATIC "static"
%token<token_atrib> STRING_LITERAL "string-literal"
%token<token_atrib> STRUCT "struct"
%token<token_atrib> SUB_ASSIGN "-="
%token<token_atrib> SWITCH "switch"
%token<token_atrib> ELLIPSIS "..."
%token<token_atrib> TYPEDEF "typedef"
%token<token_atrib> UNION "union"
%token<token_atrib> TOKEN_UNSIGNED "unsigned"
%token<token_atrib> TOKEN_VOID "void"
%token<token_atrib> TOKEN_VOLATILE "volatile"
%token<token_atrib> WHILE "while"
%token<token_atrib> XOR_ASSIGN "^="

%token<token_atrib> PP_COMMENT "<preprocessor-comment>"
%token<token_atrib> PP_TOKEN "<preprocessor-token>"

%token<token_atrib> MCC_REFERENCE "<C-reference-specifier>"
%token<token_atrib> MCC_REBINDABLE_REFERENCE "<rebindable-reference-specifier>"

// C11
%token<token_atrib> TOKEN_ATOMIC "_Atomic"
%token<token_atrib> TOKEN_GENERIC "_Generic"
%token<token_atrib> TOKEN_NORETURN "_Noreturn"
%token<token_atrib> STATIC_ASSERT "_Static_assert"
%token<token_atrib> TOKEN_THREAD_LOCAL "_Thread_local"

// Lexical symbols
%token<token_atrib> '!'
%token<token_atrib> '%'
%token<token_atrib> '&'
%token<token_atrib> '('
%token<token_atrib> ')'
%token<token_atrib> '*'
%token<token_atrib> '+'
%token<token_atrib> ','
%token<token_atrib> '-'
%token<token_atrib> '.'
%token<token_atrib> '/'
%token<token_atrib> ':'
%token<token_atrib> ';'
%token<token_atrib> '<'
%token<token_atrib> '='
%token<token_atrib> '>'
%token<token_atrib> '?'
%token<token_atrib> '['
%token<token_atrib> ']'
%token<token_atrib> '^'
%token<token_atrib> '{'
%token<token_atrib> '|'
%token<token_atrib> '}'
%token<token_atrib> '~'

// GNU Extensions
%token<token_atrib> BUILTIN_VA_ARG "__builtin_va_arg"
%token<token_atrib> BUILTIN_OFFSETOF "__builtin_offsetof"
%token<token_atrib> BUILTIN_CHOOSE_EXPR "__builtin_choose_expr"
%token<token_atrib> BUILTIN_TYPES_COMPATIBLE_P "__builtin_types_compatible_p"
%token<token_atrib> TOKEN_GCC_ALIGNOF "__alignof__"
%token<token_atrib> EXTENSION "__extension__"
%token<token_atrib> REAL "__real__"
%token<token_atrib> IMAG "__imag__"
%token<token_atrib> LABEL "__label__"
%token<token_atrib> COMPLEX "__complex__"
%token<token_atrib> IMAGINARY "__imaginary__"
%token<token_atrib> TYPEOF "__typeof__"
// This is not a GNU extension, though
%token<token_atrib> RESTRICT "restrict"
%token<token_atrib> TOKEN_GCC_ATTRIBUTE "__attribute__"
%token<token_atrib> THREAD "__thread"

%type<token_atrib> identifier_token

// Nonterminals
%type<ast> abstract_declarator
%type<ast> abstract_declarator_not_started_with_attributes
%type<ast> additive_expression
%type<ast> and_expression
%type<ast> asm_definition
%type<ast> asm_operand
%type<ast> asm_operand_list
%type<ast> asm_operand_list_nonempty
%type<ast> gcc_extra_bits_init_declarator_opt
%type<ast> gcc_extra_bits_init_declarator
%type<ast> asm_specification
%type<ast> assignment_expression
%type<ast> gcc_attribute
%type<ast> gcc_attribute_list
%type<ast> attribute_specifier
%type<ast> attribute_specifier_seq
%type<ast> attribute_specifier_seq_opt
%type<ast> gcc_attribute_value
%type<ast> block_declaration
%type<ast> non_empty_block_declaration
%type<ast> common_block_declaration
%type<ast> static_assert_declaration
%type<ast> nested_block_declaration
%type<ast> nested_function_definition
%type<ast> builtin_types
%type<ast> typeof_type_specifier
%type<ast> atomic_type_specifier
%type<ast> cast_expression
%type<ast> class_head
%type<ast> class_key
%type<ast> class_specifier
%type<ast> compound_statement
%type<ast> condition
%type<ast> condition_opt
%type<ast> conditional_expression
%type<ast> constant_expression
%type<ast> constant_initializer
%type<ast> cv_qualifier
%type<ast> cv_qualifier_seq
%type<ast> declaration
%type<ast> declaration_sequence
%type<ast> declaration_statement
%type<ast> declarator
%type<ast> declarator_not_started_with_attributes
%type<ast> functional_declarator
%type<ast> functional_declarator_not_started_with_attributes
%type<ast> declarator_id
%type<ast> functional_declarator_id
%type<ast> functional_final_declarator_id
%type<ast> decl_specifier_seq
%type<ast> decl_specifier_seq_may_end_with_declarator
%type<ast> abstract_direct_declarator
%type<ast> direct_declarator
%type<ast> functional_direct_declarator
%type<ast> optional_array_cv_qualifier_seq
%type<ast> array_cv_qualifier_seq
%type<ast> optional_array_static_qualif
%type<ast> array_static_qualif
%type<ast> optional_array_expression
%type<ast> elaborated_type_specifier
%type<ast> enumeration_definition
%type<ast> enumeration_list
%type<ast> enum_head
%type<ast> enum_key
%type<ast> enum_specifier
%type<ast> equality_expression
%type<ast> exclusive_or_expression
%type<ast> expression
%type<ast> expression_opt
%type<ast> expression_list
%type<ast> expression_statement
%type<ast> for_init_statement
%type<ast> function_body
%type<ast> function_definition
%type<ast> function_definition_header
%type<ast> function_specifier
%type<ast> id_expression
%type<ast> inclusive_or_expression
%type<ast> init_declarator
%type<ast> init_declarator_list
%type<ast> init_declarator_list_nonempty
%type<ast> init_declarator_first
%type<ast> initializer
%type<ast> initializer_clause
%type<ast> initializer_list
%type<ast> braced_init_list
%type<ast> iteration_statement
%type<ast> jump_statement
%type<ast> label_declaration
%type<ast> label_declarator_seq
%type<ast> labeled_statement
%type<ast> literal
%type<ast> logical_and_expression
%type<ast> logical_or_expression
%type<ast> member_declaration
%type<ast> member_declarator
%type<ast> member_declarator_first
%type<ast> member_declarator_list
%type<ast> member_declarator_list_nonempty
%type<ast> member_specification_seq
%type<ast> multiplicative_expression
%type<ast> if_statement
%type<ast> parameter_type_list
%type<ast> identifier_list
%type<ast> identifier_list_kr
%type<ast> parameter_declaration
%type<ast> parameter_declaration_list
%type<ast> parameters_and_qualifiers
%type<ast> parameter_or_kr_list
%type<ast> postfix_expression
%type<ast> primary_expression
%type<ast> generic_selection
%type<ast> generic_assoc_list
%type<ast> generic_association
%type<ast> ptr_operator
%type<ast> relational_expression
%type<ast> selection_statement
%type<ast> shift_expression
%type<ast> simple_declaration
%type<ast> non_empty_simple_declaration
%type<ast> simple_type_specifier
%type<ast> statement
%type<ast> statement_seq
%type<ast> nondeclarating_statement
%type<ast> storage_class_specifier
%type<ast> string_literal
%type<ast> translation_unit
%type<ast> type_id
%type<ast> type_name
%type<ast> type_specifier
%type<ast> type_specifier_seq
%type<ast> unary_expression
%type<ast> unqualified_id
%type<ast> nontype_specifier_seq
%type<ast> nontype_specifier
// %type<ast> nontype_specifier_seq2
// %type<ast> nontype_specifier2
%type<ast> volatile_optional
%type<ast> designation
%type<ast> designator_list
%type<ast> designator
%type<ast> offsetof_member_designator

%type<ast> old_style_parameter_list
%type<ast> old_style_parameter

%type<ast> nontype_specifier_without_attribute_seq
%type<ast> nontype_specifier_without_attribute

%type<ast> old_style_decl_specifier_seq

%type<ast> decl_specifier_alone_seq
%type<ast> declarating_type_specifier
%type<ast> type_specifier_alone

%type<node_type> unary_operator
%type<node_type> assignment_operator

%type<ast> identifier_opt





%type<ast> subparse_type_list

// Subparsing
%token<token_atrib> SUBPARSE_EXPRESSION "<subparse-expression>"
%token<token_atrib> SUBPARSE_EXPRESSION_LIST "<subparse-expression-list>"
%token<token_atrib> SUBPARSE_STATEMENT "<subparse-statement>"
%token<token_atrib> SUBPARSE_DECLARATION "<subparse-declaration>"
%token<token_atrib> SUBPARSE_MEMBER "<subparse-member>"
%token<token_atrib> SUBPARSE_TYPE "<subparse-type>"
%token<token_atrib> SUBPARSE_TYPE_LIST "<subparse-type-list>"
%token<token_atrib> SUBPARSE_ID_EXPRESSION "<subparse-id-expression>"
























































































%token<token_atrib> NODECL_LITERAL_EXPR "<nodecl-literal-expression>"
%token<token_atrib> NODECL_LITERAL_STMT "<nodecl-literal-statement>"

%token<token_atrib> SYMBOL_LITERAL_REF "<symbol-literal-reference>"
%token<token_atrib> TYPE_LITERAL_REF "<type-literal-reference>"

%type<ast> nodecl_literal_expr
%type<ast> nodecl_literal_stmt
%type<ast> nodecl_literal_attribute_seq
%type<ast> nodecl_string_literal

%type<ast> symbol_literal_ref
%type<ast> type_literal_ref














































































































%type<ast> shape_seq
%type<ast> shape
%type<ast> noshape_cast_expression
%type<ast> mercurium_extended_type_specifiers

%type<ast> multiexpression
%type<ast> multiexpression_body
%type<ast> multiexpression_iterator
%type<ast> multiexpression_iterator_list
%type<ast> multiexpression_range
%type<ast> multiexpression_range_size
%type<ast> multiexpression_range_section
%type<ast> multiexpression_range_discrete

%token<token_atrib> MCC_BYTE "<byte-type-spec>"
%token<token_atrib> MCC_BOOL "<bool-type-spec>"
%token<token_atrib> MCC_MASK "<mask-type-spec>"
%token<token_atrib> MCC_ARRAY_SUBSCRIPT_CHECK "@array-subscript-check@"
%token<token_atrib> MCC_CONST_VALUE_CHECK "@const-value-check@"














































































































































































%type<ast> fortran_allocate_statement

%token<token_atrib> C_FORTRAN_ALLOCATE





















%type<ast> statement_placeholder
%token<token_atrib> STATEMENT_PLACEHOLDER "<statement-placeholder>"



































// This is code




%token<token_atrib> UNKNOWN_PRAGMA "<unknown-pragma>"
%type<ast> unknown_pragma


%token<token_atrib> VERBATIM_PRAGMA "<verbatim pragma>"
%token<token_atrib> VERBATIM_CONSTRUCT "<verbatim construct>"
%token<token_atrib> VERBATIM_TYPE "<verbatim type clause>"
%token<token_atrib> VERBATIM_TEXT "<verbatim text>"

%type<ast> verbatim_construct


%token<token_atrib> PRAGMA_CUSTOM "<pragma-custom>"
%token<token_atrib> PRAGMA_CUSTOM_NEWLINE "<pragma-custom-newline>"
%token<token_atrib> PRAGMA_CUSTOM_DIRECTIVE "<pragma-custom-directive>"
%token<token_atrib> PRAGMA_CUSTOM_CONSTRUCT "<pragma-custom-construct>"
%token<token_atrib> PRAGMA_CUSTOM_END_CONSTRUCT "<pragma-custom-end-construct>"
%token<token_atrib> PRAGMA_CUSTOM_CONSTRUCT_NOEND "<pragma-custom-construct-noend>"
%token<token_atrib> PRAGMA_CUSTOM_END_CONSTRUCT_NOEND "<pragma-custom-end-construct-noend>"

%token<token_atrib> PRAGMA_CUSTOM_CLAUSE "<pragma-custom-clause>"

%token<token_atrib> PRAGMA_CLAUSE_ARG_TEXT "<pragma-clause-argument-text>"

%type<ast> pragma_custom_directive
%type<ast> pragma_custom_line_directive
%type<ast> pragma_custom_line_construct
%type<ast> pragma_custom_construct_statement

%type<ast> pragma_custom_construct_declaration
%type<ast> pragma_custom_construct_member_declaration

















%type<ast> pragma_custom_clause
%type<ast> pragma_custom_clause_seq
%type<ast> pragma_custom_clause_opt_seq

// %type<ast> pragma_expression_entity
// %type<ast> pragma_expression_entity_list

%type<ast> pragma_clause_arg_list

%type<token_atrib> pragma_clause_arg
%type<token_atrib> pragma_clause_arg_item 
%type<token_atrib> pragma_clause_arg_text

































































































































































































































































































































































































%token<token_atrib> SUBPARSE_OPENMP_DECLARE_REDUCTION "<omp-declare-reduction>"
%token<token_atrib> SUBPARSE_OPENMP_DEPEND_ITEM "<omp-depend-item>"

%type<ast> omp_declare_reduction
%type<ast> omp_dr_reduction_id
%type<ast> omp_dr_typename_list
%type<ast> omp_dr_typename
%type<ast> omp_dr_combiner
%type<ast> omp_dr_initializer

%type<ast> omp_depend_item
%type<ast> omp_expression_opt

%type<token_atrib> omp_dr_operator
%type<ast> omp_dr_identifier




























































































































































%token<token_atrib> SUBPARSE_OMPSS_DEPENDENCY_EXPRESSION "<ompss-dependency-expression>"

%type<ast> ompss_dependency_expr
%type<ast> ompss_old_multidependences




























// Lexical Symbol for superscalar regions
%token<token_atrib> TWO_DOTS ".."

// Tokens for subparsing
%token<token_atrib> SUBPARSE_SUPERSCALAR_DECLARATOR "<subparse-superscalar-declarator>"
%token<token_atrib> SUBPARSE_SUPERSCALAR_DECLARATOR_LIST "<subparse-superscalar-declarator-list>"
%token<token_atrib> SUBPARSE_SUPERSCALAR_EXPRESSION "<subparse-superscalar-expression>"

// Tokens for rules
%type<ast> superscalar_declarator superscalar_declarator_list opt_superscalar_region_spec_list superscalar_region_spec_list superscalar_region_spec

















































































%token<token_atrib> UPC_MYTHREAD "MYTHREAD (UPC)" 
%token<token_atrib> UPC_RELAXED "relaxed (UPC)"
%token<token_atrib> UPC_SHARED "shared (UPC)"
%token<token_atrib> UPC_STRICT "strict (UPC)"
%token<token_atrib> UPC_THREADS "THREADS (UPC)"
%token<token_atrib> UPC_BARRIER "upc_barrier"
%token<token_atrib> UPC_BLOCKSIZEOF "upc_blocksizeof"
%token<token_atrib> UPC_ELEMSIZEOF "upc_elemsizeof"
%token<token_atrib> UPC_FENCE "upc_fence"
%token<token_atrib> UPC_FORALL "upc_forall"
%token<token_atrib> UPC_LOCALSIZEOF "upc_localsizeof"
%token<token_atrib> UPC_MAX_BLOCKSIZE "UPC_MAX_BLOCKSIZE"
%token<token_atrib> UPC_NOTIFY "upc_notify"
%token<token_atrib> UPC_WAIT "upc_wait"

%type<ast> upc_shared_type_qualifier
%type<ast> upc_reference_type_qualifier
%type<ast> upc_layout_qualifier
%type<ast> upc_layout_qualifier_element
%type<ast> upc_synchronization_statement
%type<ast> upc_expression_opt
%type<ast> upc_affinity_opt
%type<ast> upc_affinity






















































































































































%token<token_atrib> CUDA_DEVICE "__device__" 
%token<token_atrib> CUDA_GLOBAL "__global__"
%token<token_atrib> CUDA_HOST "__host__"
%token<token_atrib> CUDA_CONSTANT "__constant__"
%token<token_atrib> CUDA_SHARED "__shared__"
%token<token_atrib> CUDA_KERNEL_LEFT "<<<"
%token<token_atrib> CUDA_KERNEL_RIGHT ">>>"

%type<ast> cuda_specifiers
%type<ast> cuda_kernel_call
%type<ast> cuda_kernel_arguments
%type<ast> cuda_kernel_config_list

%type<token_atrib> cuda_kernel_config_left
%type<token_atrib> cuda_kernel_config_right

































































































%token<token_atrib> OPENCL_GLOBAL "__global" 
%token<token_atrib> OPENCL_KERNEL "__kernel"
%token<token_atrib> OPENCL_CONSTANT "__constant"
%token<token_atrib> OPENCL_LOCAL "__local"

%type<ast> opencl_specifiers





























%token<token_atrib> XL_BUILTIN_SPEC "_Builtin"












































%token<token_atrib> TOKEN_DECLSPEC "__declspec"

%token<token_atrib> MS_INT8 "__int8"
%token<token_atrib> MS_INT16 "__int16"
%token<token_atrib> MS_INT32 "__int32"
%token<token_atrib> MS_INT64 "__int64"

%type<ast> declspec_specifier
%type<ast> extended_decl_modifier_list
%type<ast> extended_decl_modifier_list0
%type<ast> extended_decl_modifier









































































%token<token_atrib> INTEL_ASSUME "__assume"
%token<token_atrib> INTEL_ASSUME_ALIGNED "__assume_aligned"

















%type<ast> subparsing

%start translation_unit

%%

// *********************************************************
// A.3 - Basic concepts
// *********************************************************

translation_unit : declaration_sequence
{
	*parsed_tree = $1;
}
| /* empty */
{
	*parsed_tree = NULL;
}
;

// *********************************************************
// A.6. - Declarations
// *********************************************************

declaration_sequence : declaration
{
	$$ = ASTListLeaf($1);
}
| declaration_sequence declaration
{
	$$ = ASTList($1, $2);
}
;

declaration : block_declaration 
{
	$$ = $1;
}
| function_definition
{
	$$ = $1;
}
;

block_declaration : simple_declaration
{
	$$ = $1;
}
| common_block_declaration
{
    $$ = $1;
}
| static_assert_declaration
{
    $$ = $1;
}
;

// C11
static_assert_declaration : STATIC_ASSERT '(' constant_expression ',' string_literal ')' ';'
{
    $$ = ASTMake2(AST_STATIC_ASSERT, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

non_empty_block_declaration : non_empty_simple_declaration
{
    $$ = $1;
}
| common_block_declaration
{
    $$ = $1;
}
| nested_block_declaration
{
    $$ = $1;
}
;

nested_block_declaration : EXTENSION nested_block_declaration
{
	$$ = ASTMake1(AST_GCC_EXTENSION, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| nested_function_definition
{
    $$ = $1;
}
;

nested_function_definition : function_definition
{
    $$ = $1;
}
;

common_block_declaration : asm_definition
{
	$$ = $1;
}
/* GNU extensions */
| label_declaration 
{
	$$ = $1;
}
| EXTENSION block_declaration
{
	$$ = ASTMake1(AST_GCC_EXTENSION, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
// Prettyprinted comments
| PP_COMMENT
{
	$$ = ASTLeaf(AST_PP_COMMENT, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
// Prettyprinted preprocessor elements
| PP_TOKEN
{
	$$ = ASTLeaf(AST_PP_TOKEN, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

/* GNU Extension */
label_declaration : LABEL label_declarator_seq ';'
{
	$$ = ASTMake1(AST_GCC_LABEL_DECL, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

label_declarator_seq : identifier_token 
{
    AST symbol_holder = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
	$$ = ASTListLeaf(symbol_holder);
}
| label_declarator_seq ',' identifier_token
{
	AST label = ASTLeaf(AST_SYMBOL, make_locus(@3.first_filename, @3.first_line, @3.first_column), $3.token_text);
	$$ = ASTList($1, label);
}
;
/* End of GNU extension */

/* GNU Extension */
attribute_specifier : gcc_attribute
{
    $$ = $1;
}
;

attribute_specifier_seq : attribute_specifier_seq attribute_specifier
{
	$$ = ASTList($1, $2);
}
| attribute_specifier
{
    $$ = ASTListLeaf($1);
}
;

attribute_specifier_seq_opt : attribute_specifier_seq
{
    $$ = $1;
}
| /* empty */
{
    $$ = NULL;
}
;

gcc_attribute : TOKEN_GCC_ATTRIBUTE '(' '(' gcc_attribute_list ')' ')'
{
	$$ = ASTMake1(AST_GCC_ATTRIBUTE, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_GCC_ATTRIBUTE '(''(' ')'')'
{
	$$ = ASTMake1(AST_GCC_ATTRIBUTE, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

gcc_attribute_list : gcc_attribute_value
{
	$$ = ASTListLeaf($1);
}
| gcc_attribute_list ',' gcc_attribute_value
{
	$$ = ASTList($1, $3);
}
;

// Why on earth ASTSon1 is always null ?
gcc_attribute_value : identifier_token
{
	AST identif = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);

	$$ = ASTMake2(AST_GCC_ATTRIBUTE_EXPR, identif, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TOKEN_CONST
{
	AST identif = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);

	$$ = ASTMake2(AST_GCC_ATTRIBUTE_EXPR, identif, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| identifier_token '(' expression_list ')'
{
	AST identif1 = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
	
	$$ = ASTMake2(AST_GCC_ATTRIBUTE_EXPR, identif1, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;
/* End of GNU extension */

asm_definition : ASM volatile_optional '(' string_literal ')' ';'
{
	$$ = ASTMake2(AST_ASM_DEFINITION, $4, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
// From here, none of these asm-definitions are standard but gcc only
| ASM volatile_optional '(' string_literal ':' asm_operand_list ')' ';'
{
	AST asm_parms = ASTMake4(AST_GCC_ASM_DEF_PARMS, 
			$4, $6, NULL, NULL, ast_get_locus($4), NULL);
	$$ = ASTMake2(AST_GCC_ASM_DEFINITION, $2, asm_parms, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| ASM volatile_optional '(' string_literal ':' asm_operand_list ':' asm_operand_list ')' ';'
{
	AST asm_parms = ASTMake4(AST_GCC_ASM_DEF_PARMS, 
			$4, $6, $8, NULL, ast_get_locus($4), NULL);
	$$ = ASTMake2(AST_GCC_ASM_DEFINITION, $2, asm_parms, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| ASM volatile_optional '(' string_literal ':' asm_operand_list ':' asm_operand_list ':' asm_operand_list ')' ';'
{
	AST asm_parms = ASTMake4(AST_GCC_ASM_DEF_PARMS, 
			$4, $6, $8, $10, ast_get_locus($4), NULL);
	$$ = ASTMake2(AST_GCC_ASM_DEFINITION, $2, asm_parms, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

volatile_optional : /* empty */
{
	$$ = NULL;
}
| TOKEN_VOLATILE
{
	$$ = ASTLeaf(AST_VOLATILE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

asm_operand_list : asm_operand_list_nonempty
{
	$$ = $1;
}
| /* empty */
{
	$$ = NULL;
}
;

/* GNU Extensions */
asm_operand_list_nonempty : asm_operand
{
	$$ = ASTListLeaf($1);
}
| asm_operand_list_nonempty ',' asm_operand
{
	$$ = ASTList($1, $3);
}
;

asm_operand : string_literal '(' expression ')' 
{
	$$ = ASTMake3(AST_GCC_ASM_OPERAND, NULL, $1, $3, ast_get_locus($1), NULL);
}
| '[' identifier_token ']' string_literal '(' expression ')'
{
    AST symbol_tree = ASTLeaf(AST_SYMBOL, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text);

	$$ = ASTMake3(AST_GCC_ASM_OPERAND, symbol_tree, $4, $6, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| string_literal
{
	$$ = ASTMake3(AST_GCC_ASM_OPERAND, NULL, $1, NULL, ast_get_locus($1), NULL);
}
;
/* End of GNU extensions */


simple_declaration : non_empty_simple_declaration
{
    $$ = $1;
}
| ';'
{
    // This is an error but also a common extension
    $$ = ASTLeaf(AST_EMPTY_DECL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

non_empty_simple_declaration : decl_specifier_seq init_declarator_list ';'  %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, $1, $2, ast_get_locus($1), NULL);
}
| decl_specifier_alone_seq ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, $1, NULL, ast_get_locus($1), NULL);
}
| init_declarator_list ';'  %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, NULL, $1, ast_get_locus($1), NULL);
}
;

old_style_parameter : old_style_decl_specifier_seq init_declarator_list ';' 
{
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, $1, $2, ast_get_locus($1), NULL);
}
;

old_style_decl_specifier_seq : nontype_specifier_without_attribute_seq type_specifier nontype_specifier_seq
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
| nontype_specifier_without_attribute_seq type_specifier
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, NULL, ast_get_locus($1), NULL);
}
| type_specifier nontype_specifier_seq
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, NULL, ast_get_locus($1), NULL);
}
| nontype_specifier_without_attribute_seq
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, NULL, NULL, ast_get_locus($1), NULL);
}
;

// This avoids a conflict existing in GCC
nontype_specifier_without_attribute_seq : nontype_specifier_without_attribute 
{
    $$ = ASTListLeaf($1);
}
| nontype_specifier_without_attribute_seq nontype_specifier_without_attribute
{
    $$ = ASTList($1, $2);
}
;

decl_specifier_seq : nontype_specifier_seq type_specifier nontype_specifier_seq %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
| nontype_specifier_seq type_specifier %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, NULL, ast_get_locus($1), NULL);
}
| type_specifier nontype_specifier_seq %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, NULL, ast_get_locus($1), NULL);
}
| nontype_specifier_seq %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, NULL, NULL, ast_get_locus($1), NULL);
}
;

decl_specifier_seq_may_end_with_declarator : nontype_specifier_seq type_specifier nontype_specifier_seq %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
| nontype_specifier_seq type_specifier %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, NULL, ast_get_locus($1), NULL);
}
| type_specifier nontype_specifier_seq %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, NULL, ast_get_locus($1), NULL);
}
| nontype_specifier_seq %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, NULL, NULL, ast_get_locus($1), NULL);
}
;

// This does not declare an object, maybe just types, so restrict it a bit
// 
// Note that, in contrast to decl_specifier_seq, we allow here to end with an gcc_attribute
// ('nontype_specifier_seq' instead of 'nontype_specifier_no_end_attrib_seq')
// In these cases there is not any declarator, so a trailing __attribute__ would not be
// valid
decl_specifier_alone_seq : nontype_specifier_seq type_specifier_alone nontype_specifier_seq
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
| nontype_specifier_seq type_specifier_alone
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, NULL, ast_get_locus($1), NULL);
}
| type_specifier_alone nontype_specifier_seq
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_alone
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, NULL, ast_get_locus($1), NULL);
}
;

nontype_specifier_seq : nontype_specifier
{
	$$ = ASTListLeaf($1);
}
| nontype_specifier_seq nontype_specifier
{
	$$ = ASTList($1, $2);
}
;

nontype_specifier : nontype_specifier_without_attribute
{
    $$ = $1;
}
| gcc_attribute
{
	$$ = $1;
}
;

nontype_specifier_without_attribute : storage_class_specifier
{
	$$ = $1;
}
| function_specifier
{
	$$ = $1;
}
| TYPEDEF
{
	$$ = ASTLeaf(AST_TYPEDEF_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
// To ease things
| cv_qualifier
{
	$$ = $1;
}
// Repeat them
| TOKEN_SIGNED
{
	$$ = ASTLeaf(AST_SIGNED_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_UNSIGNED
{
	$$ = ASTLeaf(AST_UNSIGNED_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_LONG
{
	$$ = ASTLeaf(AST_LONG_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_SHORT
{
	$$ = ASTLeaf(AST_SHORT_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
// GNU Extension for C++ but not for C99
| COMPLEX
{
	$$ = ASTLeaf(AST_GCC_COMPLEX_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| IMAGINARY
{
	$$ = ASTLeaf(AST_GCC_IMAGINARY_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

storage_class_specifier : TOKEN_AUTO_STORAGE
{
	$$ = ASTLeaf(AST_AUTO_STORAGE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| REGISTER
{
	$$ = ASTLeaf(AST_REGISTER_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| STATIC
{
	$$ = ASTLeaf(AST_STATIC_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| EXTERN
{
	$$ = ASTLeaf(AST_EXTERN_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
// GNU Extension
| THREAD
{
	$$ = ASTLeaf(AST_THREAD_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

function_specifier : INLINE
{
	$$ = ASTLeaf(AST_INLINE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_NORETURN
{
	$$ = ASTLeaf(AST_NORETURN_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

type_specifier : declarating_type_specifier
{
    $$ = $1;
}
| simple_type_specifier
{
    $$ = $1;
}
;

type_specifier_alone : declarating_type_specifier
{
    $$ = $1;
}
| builtin_types
{
    $$ = $1;
}
| typeof_type_specifier
{
    $$ = $1;
}
| atomic_type_specifier
{
    $$ = $1;
}
;

declarating_type_specifier : class_specifier
{
	$$ = $1;
}
| enum_specifier
{
	$$ = $1;
}
| elaborated_type_specifier
{
	$$ = $1;
}
;

type_specifier_seq : nontype_specifier_seq type_specifier nontype_specifier_seq %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
| nontype_specifier_seq type_specifier %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, NULL, ast_get_locus($1), NULL);
}
| type_specifier nontype_specifier_seq %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, NULL, ast_get_locus($1), NULL);
}
| nontype_specifier_seq %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, NULL, NULL, ast_get_locus($1), NULL);
}
;

simple_type_specifier : type_name
{
	$$ = ASTMake1(AST_SIMPLE_TYPE_SPEC, $1, ast_get_locus($1), NULL);
}
| builtin_types
{
	$$ = $1;
}
| typeof_type_specifier
{
    $$ = $1;
}
| atomic_type_specifier
{
    $$ = $1;
}
;

// GNU Extension. Somebody decided that this had to be different in gcc and g++
typeof_type_specifier : TYPEOF '(' expression ')' %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_GCC_TYPEOF_EXPR, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TYPEOF '(' type_id ')' %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_GCC_TYPEOF, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

atomic_type_specifier : TOKEN_ATOMIC '(' type_id ')'
{
    $$ = ASTMake1(AST_ATOMIC_TYPE_SPECIFIER, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
};

// Simplified rule
type_name : identifier_token
{
	$$ = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

builtin_types : TOKEN_CHAR
{
	$$ = ASTLeaf(AST_CHAR_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_BOOL
{
	$$ = ASTLeaf(AST_BOOL_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_INT
{
	$$ = ASTLeaf(AST_INT_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_FLOAT
{
	$$ = ASTLeaf(AST_FLOAT_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_DOUBLE
{
	$$ = ASTLeaf(AST_DOUBLE_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_VOID
{
	$$ = ASTLeaf(AST_VOID_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_INT128
{
	$$ = ASTLeaf(AST_GCC_INT128, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_FLOAT128
{
	$$ = ASTLeaf(AST_GCC_FLOAT128, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| MCC_BYTE
{
	$$ = ASTLeaf(AST_MCC_BYTE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

elaborated_type_specifier : class_key attribute_specifier_seq_opt identifier_token
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@3.first_filename, @3.first_line, @3.first_column), $3.token_text);

	$$ = ASTMake3(AST_ELABORATED_TYPE_CLASS_SPEC, $1, identifier, $2, ast_get_locus($1), NULL);
}
| enum_key attribute_specifier_seq_opt identifier_token
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@3.first_filename, @3.first_line, @3.first_column), $3.token_text);

	$$ = ASTMake4(AST_ELABORATED_TYPE_ENUM_SPEC, identifier, $2, NULL, $1, ast_get_locus($1), NULL);
}
;

// *********************************************************
// A.7 - Declarators
// *********************************************************
init_declarator_list : init_declarator_first
{
    $$ = ASTListLeaf($1);
}
| init_declarator_first ',' init_declarator_list_nonempty
{
    $$ = ast_list_concat(ASTListLeaf($1), $3);
}
;

init_declarator_list_nonempty : init_declarator
{
	$$ = ASTListLeaf($1);
}
| init_declarator_list_nonempty ',' init_declarator
{
	$$ = ASTList($1, $3);
}
;

init_declarator : declarator gcc_extra_bits_init_declarator_opt
{
	$$ = ASTMake3(AST_INIT_DECLARATOR, $1, NULL, $2, ast_get_locus($1), NULL);
}
| declarator gcc_extra_bits_init_declarator_opt initializer
{
	$$ = ASTMake3(AST_INIT_DECLARATOR, $1, $3, $2, ast_get_locus($1), NULL);
}
;

init_declarator_first : declarator_not_started_with_attributes gcc_extra_bits_init_declarator_opt
{
	$$ = ASTMake3(AST_INIT_DECLARATOR, $1, NULL, $2, ast_get_locus($1), NULL);
}
| declarator_not_started_with_attributes gcc_extra_bits_init_declarator_opt initializer
{
	$$ = ASTMake3(AST_INIT_DECLARATOR, $1, $3, $2, ast_get_locus($1), NULL);
}
;

gcc_extra_bits_init_declarator_opt : /* empty */
{
    $$ = NULL;
}
| gcc_extra_bits_init_declarator
{
    $$ = $1;
}
;

gcc_extra_bits_init_declarator : asm_specification
{
    $$ = ASTListLeaf($1);
}
| attribute_specifier_seq
{
    $$ = $1;
}
| asm_specification attribute_specifier_seq
{
    $$ = ast_list_concat(ASTListLeaf($1), $2);
}
;

/* GNU Extension */
asm_specification : ASM '(' string_literal ')'
{
	$$ = ASTMake1(AST_GCC_ASM_SPEC, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;
/* End of GNU Extension */

declarator : attribute_specifier_seq_opt direct_declarator
{
	$$ = ASTMake2(AST_DECLARATOR, $2, $1, ast_get_locus($2), NULL);
}
| attribute_specifier_seq_opt ptr_operator declarator
{
	$$ = ASTMake3(AST_POINTER_DECLARATOR, $2, $3, $1, ast_get_locus($2), NULL);
}
;

declarator_not_started_with_attributes : direct_declarator
{
	$$ = ASTMake2(AST_DECLARATOR, $1, NULL, ast_get_locus($1), NULL);
}
| ptr_operator declarator
{
	$$ = ASTMake3(AST_POINTER_DECLARATOR, $1, $2, NULL, ast_get_locus($1), NULL);
}
;

ptr_operator : '*'
{
	$$ = ASTMake2(AST_POINTER_SPEC, NULL, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '*' cv_qualifier_seq
{
	$$ = ASTMake2(AST_POINTER_SPEC, NULL, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| MCC_REFERENCE
{
	$$ = ASTLeaf(AST_REFERENCE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| MCC_REBINDABLE_REFERENCE
{
	$$ = ASTLeaf(AST_REBINDABLE_REFERENCE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

/*
   A functional declarator is a syntactic enforced declarator that will have
   a functional nature
 */
functional_declarator : attribute_specifier_seq_opt functional_direct_declarator
{
	$$ = ASTMake2(AST_DECLARATOR, $2, $1, ast_get_locus($2), NULL);
}
| attribute_specifier_seq_opt ptr_operator functional_declarator
{
	$$ = ASTMake3(AST_POINTER_DECLARATOR, $2, $3, $1, ast_get_locus($2), NULL);
}
;

functional_declarator_not_started_with_attributes : functional_direct_declarator
{
    $$ = $1;
}
| ptr_operator functional_declarator
{
	$$ = ASTMake3(AST_POINTER_DECLARATOR, $1, $2, NULL, ast_get_locus($2), NULL);
}
;

// Isolate the ambiguity here
parameter_or_kr_list : parameter_type_list %merge<ambiguityHandler>
{
    $$ = $1;
}
| identifier_list %merge<ambiguityHandler>
{
    $$ = $1;
}
;

parameters_and_qualifiers : '(' parameter_or_kr_list ')'
{
    $$ = ASTMake4(AST_PARAMETERS_AND_QUALIFIERS,
        $2, NULL, NULL, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '(' ')' %merge <ambiguityHandler>
{
    $$ = ASTMake4(AST_PARAMETERS_AND_QUALIFIERS,
        ASTLeaf(AST_EMPTY_PARAMETER_DECLARATION_CLAUSE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL),
        NULL, NULL, NULL,
        make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

functional_direct_declarator : functional_declarator_id
{
	$$ = $1;
}
| functional_direct_declarator parameters_and_qualifiers
{
	$$ = ASTMake2(AST_DECLARATOR_FUNC, $1, $2, ast_get_locus($1), NULL);
}
| functional_direct_declarator '[' optional_array_expression ']'
{
	$$ = ASTMake4(AST_DECLARATOR_ARRAY, $1, $3, NULL, NULL,  ast_get_locus($1), NULL);
}
| functional_direct_declarator '[' array_cv_qualifier_seq optional_array_static_qualif optional_array_expression ']'
{
	$$ = ASTMake4(AST_DECLARATOR_ARRAY, $1, $5, $3, $4,  ast_get_locus($1), NULL);
}
| functional_direct_declarator '[' array_static_qualif optional_array_cv_qualifier_seq optional_array_expression ']'
{
	$$ = ASTMake4(AST_DECLARATOR_ARRAY, $1, $5, $4, $3,  ast_get_locus($1), NULL);
}
| '(' functional_declarator ')'
{
	$$ = ASTMake1(AST_PARENTHESIZED_DECLARATOR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

functional_declarator_id : functional_final_declarator_id parameters_and_qualifiers
{
	$$ = ASTMake2(AST_DECLARATOR_FUNC, $1, $2, ast_get_locus($1), NULL);
}
;

functional_final_declarator_id : declarator_id
{
    $$ = $1;
}
// This rule is needed because of redundant parentheses
| '(' functional_final_declarator_id ')'
{
    $$ = ASTMake1(AST_PARENTHESIZED_DECLARATOR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

// This eases the writing of array declarators
optional_array_cv_qualifier_seq : /* empty */
{
    $$ = NULL;
}
| array_cv_qualifier_seq
{
    $$ = $1;
}
;

array_cv_qualifier_seq : cv_qualifier_seq
{
    $$ = $1;
}
;

// This eases the writing of array declarators
optional_array_static_qualif : /* empty */
{
    $$ = NULL;
}
| array_static_qualif
{
    $$ = $1;
}
;

array_static_qualif : STATIC
{
    $$ = ASTLeaf(AST_STATIC_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

// This eases the writing of array declarators
optional_array_expression : /* empty */
{
    $$ = NULL;
}
| '*'
{
    $$ = ASTLeaf(AST_VLA_EXPRESSION, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| assignment_expression
{
    $$ = $1;
}
;

cv_qualifier_seq : cv_qualifier
{
	$$ = ASTListLeaf($1);
}
| cv_qualifier_seq cv_qualifier
{
	$$ = ASTList($1, $2);
}
;

cv_qualifier : TOKEN_CONST
{
	$$ = ASTLeaf(AST_CONST_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_VOLATILE
{
	$$ = ASTLeaf(AST_VOLATILE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
// GNU Extension
| RESTRICT
{
	$$ = ASTLeaf(AST_GCC_RESTRICT_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
// C11
| TOKEN_ATOMIC
{
    $$ = ASTLeaf(AST_ATOMIC_TYPE_QUALIFIER, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

direct_declarator : declarator_id
{
	$$ = $1;
}
| direct_declarator parameters_and_qualifiers
{
	$$ = ASTMake2(AST_DECLARATOR_FUNC, $1, $2, ast_get_locus($1), NULL);
}
| direct_declarator '[' optional_array_expression ']'
{
	$$ = ASTMake4(AST_DECLARATOR_ARRAY, $1, $3, NULL, NULL,  ast_get_locus($1), NULL);
}
| direct_declarator '[' array_cv_qualifier_seq optional_array_static_qualif optional_array_expression ']'
{
	$$ = ASTMake4(AST_DECLARATOR_ARRAY, $1, $5, $3, $4,  ast_get_locus($1), NULL);
}
| direct_declarator '[' array_static_qualif optional_array_cv_qualifier_seq optional_array_expression ']'
{
	$$ = ASTMake4(AST_DECLARATOR_ARRAY, $1, $5, $4, $3,  ast_get_locus($1), NULL);
}
| '(' declarator ')'
{
	$$ = ASTMake1(AST_PARENTHESIZED_DECLARATOR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

declarator_id : id_expression
{
	$$ = ASTMake1(AST_DECLARATOR_ID_EXPR, $1, ast_get_locus($1), NULL);
}
;

enum_head : enum_key attribute_specifier_seq_opt identifier_opt 
{
    $$ = ASTMake4(AST_ENUM_HEAD, $1, $2, $3, NULL, ast_get_locus($1), NULL);
}
;

identifier_opt : identifier_token
{
	$$ = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| /* empty */
{
    $$ = NULL;
}
;

enum_key : ENUM
{
    $$ = ASTLeaf(AST_UNSCOPED_ENUM_KEY, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

enum_specifier : enum_head '{' enumeration_list '}'
{
	$$ = ASTMake2(AST_ENUM_SPECIFIER, $1, $3, ast_get_locus($1), NULL);
}
| enum_head '{' enumeration_list ',' '}'
{
	$$ = ASTMake2(AST_ENUM_SPECIFIER, $1, $3, ast_get_locus($1), NULL);
}
;

enumeration_list: enumeration_list ',' enumeration_definition
{
	$$ = ASTList($1, $3);
}
| enumeration_definition
{
	$$ = ASTListLeaf($1);
}
;

enumeration_definition : identifier_token
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);

	$$ = ASTMake2(AST_ENUMERATOR_DEF, identifier, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| identifier_token '=' constant_expression
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);

	$$ = ASTMake2(AST_ENUMERATOR_DEF, identifier, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

type_id : type_specifier_seq
{
	$$ = ASTMake2(AST_TYPE_ID, $1, NULL, ast_get_locus($1), NULL);
}
| type_specifier_seq abstract_declarator_not_started_with_attributes
{
	$$ = ASTMake2(AST_TYPE_ID, $1, $2, ast_get_locus($1), NULL);
}
;

abstract_declarator : attribute_specifier_seq_opt ptr_operator
{
	$$ = ASTMake3(AST_POINTER_DECLARATOR, $2, NULL, $1, ast_get_locus($2), NULL);
}
| attribute_specifier_seq_opt ptr_operator abstract_declarator
{
	$$ = ASTMake3(AST_POINTER_DECLARATOR, $2, $3, $1, ast_get_locus($2), NULL);
}
| attribute_specifier_seq_opt abstract_direct_declarator
{
	$$ = ASTMake2(AST_DECLARATOR, $2, $1, ast_get_locus($2), NULL);
}
;

abstract_declarator_not_started_with_attributes : ptr_operator
{
	$$ = ASTMake3(AST_POINTER_DECLARATOR, $1, NULL, NULL, ast_get_locus($1), NULL);
}
| ptr_operator abstract_declarator
{
	$$ = ASTMake3(AST_POINTER_DECLARATOR, $1, $2, NULL, ast_get_locus($1), NULL);
}
| abstract_direct_declarator
{
	$$ = ASTMake2(AST_DECLARATOR, $1, NULL, ast_get_locus($1), NULL);
}
;

abstract_direct_declarator : '(' abstract_declarator ')'
{
	$$ = ASTMake1(AST_PARENTHESIZED_DECLARATOR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| parameters_and_qualifiers
{
	$$ = ASTMake2(AST_DECLARATOR_FUNC, NULL, $1, ast_get_locus($1), NULL);
}
| abstract_direct_declarator parameters_and_qualifiers
{
	$$ = ASTMake2(AST_DECLARATOR_FUNC, $1, $2, ast_get_locus($1), NULL);
}
| '[' optional_array_expression ']'
{
	$$ = ASTMake4(AST_DECLARATOR_ARRAY, NULL, $2, NULL, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '[' array_cv_qualifier_seq optional_array_static_qualif optional_array_expression ']'
{
	$$ = ASTMake4(AST_DECLARATOR_ARRAY, NULL, $4, $2, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '[' array_static_qualif optional_array_cv_qualifier_seq optional_array_expression ']'
{
	$$ = ASTMake4(AST_DECLARATOR_ARRAY, NULL, $4, $3, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| abstract_direct_declarator '[' optional_array_expression ']'
{
	$$ = ASTMake4(AST_DECLARATOR_ARRAY, $1, $3, NULL, NULL,  ast_get_locus($1), NULL);
}
| abstract_direct_declarator '[' array_cv_qualifier_seq optional_array_static_qualif optional_array_expression ']'
{
	$$ = ASTMake4(AST_DECLARATOR_ARRAY, $1, $5, $3, $4,  ast_get_locus($1), NULL);
}
| abstract_direct_declarator '[' array_static_qualif optional_array_cv_qualifier_seq optional_array_expression ']'
{
	$$ = ASTMake4(AST_DECLARATOR_ARRAY, $1, $5, $4, $3,  ast_get_locus($1), NULL);
}
;

identifier_list : identifier_list_kr
{
    $$ = ASTMake1(AST_KR_PARAMETER_LIST, $1, ast_get_locus($1), NULL);
}
;

identifier_list_kr : identifier_token
{
    AST symbol = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);

    $$ = ASTListLeaf(symbol);
}
| identifier_list_kr ',' identifier_token 
{
    AST symbol = ASTLeaf(AST_SYMBOL, make_locus(@3.first_filename, @3.first_line, @3.first_column), $3.token_text);

    $$ = ASTList($1, symbol);
}
;

parameter_type_list : parameter_declaration_list
{
    $$ = $1;
}
| parameter_declaration_list ',' ELLIPSIS
{
    AST variadic_arg = ASTLeaf(AST_VARIADIC_ARG, make_locus(@3.first_filename, @3.first_line, @3.first_column), $3.token_text);
	$$ = ASTList($1, variadic_arg);
}
;

parameter_declaration_list : parameter_declaration
{
	$$ = ASTListLeaf($1);
}
| parameter_declaration_list ',' parameter_declaration
{
	$$ = ASTList($1, $3);
}
;

parameter_declaration : decl_specifier_seq declarator_not_started_with_attributes attribute_specifier_seq_opt %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, NULL, $3, ast_get_locus($1), NULL);
}
| decl_specifier_seq_may_end_with_declarator attribute_specifier_seq_opt %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, NULL, NULL, $2, ast_get_locus($1), NULL);
}
| decl_specifier_seq abstract_declarator_not_started_with_attributes attribute_specifier_seq_opt %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, NULL, $3, ast_get_locus($1), NULL);
}
;

initializer : '=' initializer_clause
{
	$$ = ASTMake1(AST_EQUAL_INITIALIZER, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

initializer_clause : assignment_expression
{
	$$ = $1;
}
| braced_init_list
{
    $$ = $1;
}
;

initializer_list : initializer_clause
{
	$$ = ASTListLeaf($1);
}
| initializer_list ',' initializer_clause
{
	$$ = ASTList($1, $3);
}
| designation initializer_clause
{
    AST designated_initializer = ASTMake2(AST_DESIGNATED_INITIALIZER, $1, $2, ast_get_locus($1), NULL);

    $$ = ASTListLeaf(designated_initializer);
}
| initializer_list ',' designation initializer_clause
{
    AST designated_initializer = ASTMake2(AST_DESIGNATED_INITIALIZER, $3, $4, ast_get_locus($3), NULL);

    $$ = ASTList($1, designated_initializer);
}
// GNU Extensions
| identifier_token ':' initializer_clause
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);

	AST gcc_initializer_clause = ASTMake2(AST_GCC_INITIALIZER_CLAUSE, identifier, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTListLeaf(gcc_initializer_clause);
}
| initializer_list ',' identifier_token ':' initializer_clause
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@3.first_filename, @3.first_line, @3.first_column), $3.token_text);

	AST gcc_initializer_clause = ASTMake2(AST_GCC_INITIALIZER_CLAUSE, identifier, $5, ast_get_locus($1), NULL);

	$$ = ASTList($1, gcc_initializer_clause);
}
;

designation : designator_list '='
{
    $$ = ASTMake1(AST_DESIGNATION, $1, ast_get_locus($1), NULL);
}
;

designator_list : designator
{
    $$ = ASTListLeaf($1);
}
| designator_list designator
{
    $$ = ASTList($1, $2);
}
;

designator : '[' constant_expression ']'
{
    $$ = ASTMake1(AST_INDEX_DESIGNATOR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '.' identifier_token
{
    AST symbol = ASTLeaf(AST_SYMBOL, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text);

    $$ = ASTMake1(AST_FIELD_DESIGNATOR, symbol, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

function_definition : function_definition_header function_body
{
	$$ = ASTMake3(AST_FUNCTION_DEFINITION, $1, NULL, $2, ast_get_locus($1), NULL);
}
| function_definition_header old_style_parameter_list function_body
{
	$$ = ASTMake3(AST_FUNCTION_DEFINITION, $1, $2, $3, ast_get_locus($1), NULL);
}

// GCC extension
| EXTENSION function_definition
{
	$$ = ASTMake1(AST_GCC_EXTENSION, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

function_definition_header : decl_specifier_seq functional_declarator_not_started_with_attributes %merge<ambiguityHandler>
{
    $$ = ASTMake3(AST_FUNCTION_DEFINITION_HEADER, $1, $2, NULL, ast_get_locus($1), NULL);
}
| functional_declarator_not_started_with_attributes %merge<ambiguityHandler>
{
    $$ = ASTMake3(AST_FUNCTION_DEFINITION_HEADER, NULL, $1, NULL, ast_get_locus($1), NULL);
}
;

old_style_parameter_list : old_style_parameter
{
    $$ = ASTListLeaf($1);
}
| old_style_parameter_list old_style_parameter
{
    $$ = ASTList($1, $2);
}
;

function_body : compound_statement
{
	$$ = ASTMake1(AST_FUNCTION_BODY, $1, ast_get_locus($1), NULL);
}
;

// *********************************************************
// A.8 - Classes
// *********************************************************

class_specifier : class_head '{' member_specification_seq '}'
{
	$$ = ASTMake2(AST_CLASS_SPECIFIER, $1, $3, ast_get_locus($1), NULL);
}
| class_head '{' '}'
{
	$$ = ASTMake2(AST_CLASS_SPECIFIER, $1, NULL, ast_get_locus($1), NULL);
}
;

class_head : class_key 
{
	$$ = ASTMake4(AST_CLASS_HEAD_SPEC, $1, NULL, NULL, NULL, ast_get_locus($1), NULL);
}
| class_key identifier_token
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text);

	$$ = ASTMake4(AST_CLASS_HEAD_SPEC, $1, identifier, NULL, NULL, ast_get_locus($1), NULL);
}
// GNU Extensions
| class_key attribute_specifier_seq
{
    AST class_head_extra = ASTMake3(AST_CLASS_HEAD_EXTRA, $2, NULL, NULL, ast_get_locus($1), NULL);

	$$ = ASTMake4(AST_CLASS_HEAD_SPEC, $1, NULL, NULL, class_head_extra, ast_get_locus($1), NULL);
}
| class_key attribute_specifier_seq identifier_token
{
    AST class_head_extra = ASTMake3(AST_CLASS_HEAD_EXTRA, $2, NULL, NULL, ast_get_locus($1), NULL);

	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@3.first_filename, @3.first_line, @3.first_column), $3.token_text);
	$$ = ASTMake4(AST_CLASS_HEAD_SPEC, $1, identifier, NULL, class_head_extra, ast_get_locus($1), NULL);
}
;

class_key : STRUCT
{
	$$ = ASTLeaf(AST_CLASS_KEY_STRUCT, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| UNION
{
	$$ = ASTLeaf(AST_CLASS_KEY_UNION, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

member_specification_seq : member_declaration
{
	$$ = ASTListLeaf($1);
}
| member_specification_seq member_declaration
{
	$$ = ASTList($1, $2);
}
;

member_declaration : decl_specifier_seq member_declarator_list ';'  %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_MEMBER_DECLARATION, $1, $2, ast_get_locus($1), NULL);
}
| decl_specifier_alone_seq ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_MEMBER_DECLARATION, $1, NULL, ast_get_locus($1), NULL);
}
// This is a common tolerated error
| ';' 
{
	$$ = ASTLeaf(AST_EMPTY_DECL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
// GNU Extension
| EXTENSION member_declaration
{
	$$ = ASTMake1(AST_GCC_EXTENSION, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

member_declarator_list : member_declarator_first
{
	$$ = ASTListLeaf($1);
}
| member_declarator_first ',' member_declarator_list_nonempty
{
    $$ = ast_list_concat(ASTListLeaf($1), $3);
}
;

member_declarator_list_nonempty : member_declarator
{
    $$ = ASTListLeaf($1);
}
| member_declarator_list_nonempty ',' member_declarator
{
    $$ = ASTList($1, $3);
}
;

member_declarator_first : declarator_not_started_with_attributes attribute_specifier_seq_opt
{
    $$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, NULL, $2, ast_get_locus($1), NULL);
}
| declarator_not_started_with_attributes attribute_specifier_seq_opt constant_initializer
{
    $$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, $3, $2, ast_get_locus($1), NULL);
}
| ':' constant_expression
{
    $$ = ASTMake3(AST_BITFIELD_DECLARATOR, NULL, $2, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| identifier_token attribute_specifier_seq_opt ':' constant_expression
{
    AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
    AST declarator_id_expr = ASTMake1(AST_DECLARATOR_ID_EXPR, identifier, ast_get_locus(identifier), NULL);

    $$ = ASTMake3(AST_BITFIELD_DECLARATOR, declarator_id_expr, $4, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

member_declarator : declarator attribute_specifier_seq_opt
{
	$$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, NULL, $2, ast_get_locus($1), NULL);
}
| declarator attribute_specifier_seq_opt constant_initializer
{
	$$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, $3, $2, ast_get_locus($1), NULL);
}
| attribute_specifier_seq_opt ':' constant_expression
{
	$$ = ASTMake3(AST_BITFIELD_DECLARATOR, NULL, $3, $1, make_locus(@2.first_filename, @2.first_line, @2.first_column), NULL);
}
| identifier_token attribute_specifier_seq_opt ':' constant_expression
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
	AST declarator_id_expr = ASTMake1(AST_DECLARATOR_ID_EXPR, identifier, ast_get_locus(identifier), NULL);

	$$ = ASTMake3(AST_BITFIELD_DECLARATOR, declarator_id_expr, $4, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

constant_initializer : '=' constant_expression
{
	$$ = $2;
}
;

// *********************************************************
// A.5. - Statements
// *********************************************************

statement: nondeclarating_statement %merge<ambiguityHandler>
{
    $$ = $1;
}
| declaration_statement %merge<ambiguityHandler>
{
	$$ = $1;
}
;

nondeclarating_statement : labeled_statement
{
	$$ = $1;
}
| expression_statement
{
	$$ = $1;
}
| compound_statement
{
	$$ = $1;
}
| selection_statement
{
	$$ = $1;
}
| iteration_statement
{
	$$ = $1;
}
| jump_statement
{
	$$ = $1;
}
| if_statement
{
    $$ = $1;
}
;

labeled_statement : identifier_token ':' statement
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
	
	$$ = ASTMake2(AST_LABELED_STATEMENT, identifier, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| CASE constant_expression ':' statement
{
	$$ = ASTMake2(AST_CASE_STATEMENT, $2, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| DEFAULT ':' statement
{
	$$ = ASTMake1(AST_DEFAULT_STATEMENT, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
// GNU Extension
| CASE constant_expression ELLIPSIS constant_expression ':' statement
{
	$$ = ASTMake3(AST_GCC_CASE_STATEMENT, $2, $4, $6, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

expression_statement : expression ';'
{
	$$ = ASTMake1(AST_EXPRESSION_STATEMENT, $1, ast_get_locus($1), NULL);
}
| ';'
{
	// Empty statement ...
	$$ = ASTLeaf(AST_EMPTY_STATEMENT, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

declaration_statement : non_empty_block_declaration
{
	$$ = ASTMake1(AST_DECLARATION_STATEMENT, $1, ast_get_locus($1), NULL);
}
;

compound_statement : '{' statement_seq '}'
{
	$$ = ASTMake1(AST_COMPOUND_STATEMENT, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '{' '}'
{
	$$ = ASTMake1(AST_COMPOUND_STATEMENT, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

statement_seq : statement
{
	$$ = ASTListLeaf($1);
}
| statement_seq statement
{
	$$ = ASTList($1, $2);
}
;

// If ambiguity
// Note the precedence seems weird but makes sense.
// When faced with the ambiguity
//
//            IF ( e1 ) IF ( e2 )  s1 ELSE s2
//
// we want the case where "IF ( condition ) statement" where statement will be
// "IF ( e2 ) s1 ELSE s2". This is why the first rule has higher priority
// (although it seems it should be the other way round)
if_statement : IF '(' condition ')' statement %dprec 2
{
	$$ = ASTMake4(AST_IF_ELSE_STATEMENT, $3, $5, NULL, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| IF '(' condition ')' statement ELSE statement %dprec 1
{
	$$ = ASTMake4(AST_IF_ELSE_STATEMENT, $3, $5, $7, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

selection_statement : SWITCH '(' condition ')' statement
{
	$$ = ASTMake3(AST_SWITCH_STATEMENT, $3, $5, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

condition : expression
{
	$$ = ASTMake3(AST_CONDITION, NULL, NULL, $1, ast_get_locus($1), NULL);
}
;

iteration_statement : WHILE '(' condition ')' statement
{
	$$ = ASTMake2(AST_WHILE_STATEMENT, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| DO statement WHILE '(' expression ')' ';'
{
	$$ = ASTMake2(AST_DO_STATEMENT, $2, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| FOR '(' for_init_statement condition_opt ';' expression_opt ')' statement
{
    AST loop_control = ASTMake3(AST_LOOP_CONTROL, $3, $4, $6, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
	$$ = ASTMake3(AST_FOR_STATEMENT, loop_control, $8, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

expression_opt : expression
{
    $$ = $1;
}
| /* empty */
{
    $$ = NULL;
}
;

condition_opt : condition
{
    $$ = $1;
}
| /* empty */
{
    $$ = NULL;
}
;


for_init_statement : expression_statement %merge<ambiguityHandler>
{
	$$ = $1;
}
| non_empty_simple_declaration %merge<ambiguityHandler>
{
	$$ = $1;
}
;

jump_statement : BREAK ';'
{
	$$ = ASTLeaf(AST_BREAK_STATEMENT, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| CONTINUE ';'
{
	$$ = ASTLeaf(AST_CONTINUE_STATEMENT, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| RETURN ';'
{
	$$ = ASTMake1(AST_RETURN_STATEMENT, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| RETURN expression ';'
{
	$$ = ASTMake1(AST_RETURN_STATEMENT, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| GOTO identifier_token ';'
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text);
	
	$$ = ASTMake1(AST_GOTO_STATEMENT, identifier, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
// GNU Extension
| GOTO '*' expression ';'
{
	$$ = ASTMake1(AST_GCC_GOTO_STATEMENT, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

// *********************************************************
// A.4 - Expressions
// *********************************************************

primary_expression : literal
{
	$$ = $1;
}
| '(' expression ')' 
{
	$$ = ASTMake1(AST_PARENTHESIZED_EXPRESSION, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| id_expression
{
	$$ = $1;
}
// C11
| generic_selection
{
    $$ = $1;
}
// GNU Extensions
/*
     ( compound-statement )
     __builtin_va_arg ( assignment-expression , type-name )
     __builtin_offsetof ( type-name , offsetof-member-designator )
     __builtin_choose_expr ( assignment-expression ,
			     assignment-expression ,
			     assignment-expression )
     __builtin_types_compatible_p ( type-name , type-name )
*/
| '(' compound_statement ')'
{
	$$ = ASTMake1(AST_GCC_PARENTHESIZED_EXPRESSION, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| BUILTIN_VA_ARG '(' assignment_expression ',' type_id ')'
{
	$$ = ASTMake2(AST_GCC_BUILTIN_VA_ARG, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| BUILTIN_OFFSETOF '(' type_id ',' offsetof_member_designator ')'
{
    $$ = ASTMake2(AST_GCC_BUILTIN_OFFSETOF, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| BUILTIN_CHOOSE_EXPR '(' assignment_expression ',' assignment_expression ',' assignment_expression ')'
{
    $$ = ASTMake3(AST_GCC_BUILTIN_CHOOSE_EXPR, $3, $5, $7, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| BUILTIN_TYPES_COMPATIBLE_P '(' type_id ',' type_id ')'
{
    $$ = ASTMake2(AST_GCC_BUILTIN_TYPES_COMPATIBLE_P, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

generic_selection : TOKEN_GENERIC '(' assignment_expression ',' generic_assoc_list ')'
{
    $$ = ASTMake2(AST_GENERIC_SELECTION, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

generic_assoc_list : generic_association
{
    $$ = ASTListLeaf($1);
}
| generic_assoc_list ',' generic_association
{
    $$ = ASTList($1, $3);
}
;

generic_association : type_id ':' assignment_expression
{
    $$ = ASTMake2(AST_GENERIC_ASSOCIATION, $1, $3, ast_get_locus($1), NULL);
}
| DEFAULT ':' assignment_expression
{
    $$ = ASTMake1(AST_GENERIC_ASSOCIATION_DEFAULT, $3,
        make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

/* GNU extension */
/*
   offsetof-member-designator:
     identifier
     offsetof-member-designator . identifier
     offsetof-member-designator [ expression ]
*/
offsetof_member_designator :  identifier_token designator_list
{
    $$ = ASTMake2(AST_GCC_OFFSETOF_MEMBER_DESIGNATOR,
            ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text),
            $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| identifier_token
{
    $$ = ASTMake2(AST_GCC_OFFSETOF_MEMBER_DESIGNATOR,
            ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text),
            NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

id_expression : unqualified_id
{
	$$ = $1;
}
;

unqualified_id : identifier_token
{
	$$ = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

postfix_expression : primary_expression
{
	$$ = $1;
}
| postfix_expression '[' expression ']'
{
	$$ = ASTMake2(AST_ARRAY_SUBSCRIPT, $1, $3, ast_get_locus($1), NULL);
}
| postfix_expression '(' ')' 
{
	$$ = ASTMake2(AST_FUNCTION_CALL, $1, NULL, ast_get_locus($1), NULL);
}
| postfix_expression '(' expression_list ')' 
{
	$$ = ASTMake2(AST_FUNCTION_CALL, $1, $3, ast_get_locus($1), NULL);
}
| postfix_expression '.' id_expression
{
	$$ = ASTMake2(AST_CLASS_MEMBER_ACCESS, $1, $3, ast_get_locus($1), NULL);
}
| postfix_expression PTR_OP id_expression
{
	$$ = ASTMake2(AST_POINTER_CLASS_MEMBER_ACCESS, $1, $3, ast_get_locus($1), NULL);
}
| postfix_expression PLUSPLUS
{
	$$ = ASTMake1(AST_POSTINCREMENT, $1, ast_get_locus($1), NULL);
}
| postfix_expression MINUSMINUS
{
	$$ = ASTMake1(AST_POSTDECREMENT, $1, ast_get_locus($1), NULL);
}
// GNU Extensions
| '(' type_id ')' braced_init_list
{
	$$ = ASTMake2(AST_GCC_POSTFIX_EXPRESSION, $2, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

braced_init_list : '{' initializer_list '}'
{
	$$ = ASTMake1(AST_INITIALIZER_BRACES, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '{' initializer_list ',' '}'
{
	$$ = ASTMake1(AST_INITIALIZER_BRACES, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '{' '}'
{
	$$ = ASTMake1(AST_INITIALIZER_BRACES, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

expression_list : assignment_expression 
{
    AST expression_holder = ASTMake1(AST_EXPRESSION, $1, ast_get_locus($1), NULL);
	$$ = ASTListLeaf(expression_holder);
}
| expression_list ',' assignment_expression
{
    AST expression_holder = ASTMake1(AST_EXPRESSION, $3, ast_get_locus($3), NULL);
	$$ = ASTList($1, expression_holder);
}
;

unary_expression : postfix_expression
{
	$$ = $1;
}
| PLUSPLUS unary_expression
{
	$$ = ASTMake1(AST_PREINCREMENT, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| MINUSMINUS unary_expression
{
	$$ = ASTMake1(AST_PREDECREMENT, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| unary_operator cast_expression
{
	$$ = ASTMake1($1, $2, ast_get_locus($2), NULL);
}
| SIZEOF unary_expression %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_SIZEOF, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| SIZEOF '(' type_id ')' %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_SIZEOF_TYPEID, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
// GNU Extensions
| EXTENSION cast_expression
{
	$$ = ASTMake1(AST_GCC_EXTENSION_EXPR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_GCC_ALIGNOF unary_expression %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_GCC_ALIGNOF, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_GCC_ALIGNOF '(' type_id ')' %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_GCC_ALIGNOF_TYPE, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| REAL cast_expression
{
	$$ = ASTMake1(AST_GCC_REAL_PART, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| IMAG cast_expression
{
	$$ = ASTMake1(AST_GCC_IMAG_PART, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| ANDAND identifier_token
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text);

	$$ = ASTMake1(AST_GCC_LABEL_ADDR, identifier, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

unary_operator : '*'
{
	$$ = AST_DERREFERENCE;
}
| '&' 
{
	$$ = AST_REFERENCE;
}
| '+'
{
	$$ = AST_PLUS;
}
| '-'
{
	$$ = AST_NEG;
}
| '!'
{
	$$ = AST_LOGICAL_NOT;
}
| '~'
{
	$$ = AST_BITWISE_NOT;
}
;

cast_expression : unary_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| '(' type_id ')' cast_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_CAST, $2, ASTListLeaf($4), make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

multiplicative_expression : cast_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| multiplicative_expression '*' cast_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_MUL, $1, $3, ast_get_locus($1), NULL);
}
| multiplicative_expression '/' cast_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_DIV, $1, $3, ast_get_locus($1), NULL);
}
| multiplicative_expression '%' cast_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_MOD, $1, $3, ast_get_locus($1), NULL);
}
;

additive_expression : multiplicative_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| additive_expression '+' multiplicative_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_ADD, $1, $3, ast_get_locus($1), NULL);
}
| additive_expression '-' multiplicative_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_MINUS, $1, $3, ast_get_locus($1), NULL);
}
;

shift_expression : additive_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| shift_expression LEFT additive_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_BITWISE_SHL, $1, $3, ast_get_locus($1), NULL);
}
| shift_expression RIGHT additive_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_SHR, $1, $3, ast_get_locus($1), NULL);
}
;

relational_expression : shift_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| relational_expression '<' shift_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_LOWER_THAN, $1, $3, ast_get_locus($1), NULL);
}
| relational_expression '>' shift_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_GREATER_THAN, $1, $3, ast_get_locus($1), NULL);
}
| relational_expression GREATER_OR_EQUAL shift_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_GREATER_OR_EQUAL_THAN, $1, $3, ast_get_locus($1), NULL);
}
| relational_expression LESS_OR_EQUAL shift_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_LOWER_OR_EQUAL_THAN, $1, $3, ast_get_locus($1), NULL);
}
;

equality_expression : relational_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| equality_expression EQUAL relational_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_EQUAL, $1, $3, ast_get_locus($1), NULL);
}
| equality_expression NOT_EQUAL relational_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_DIFFERENT, $1, $3, ast_get_locus($1), NULL);
}
;

and_expression : equality_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| and_expression '&' equality_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_BITWISE_AND, $1, $3, ast_get_locus($1), NULL);
}
;

exclusive_or_expression : and_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| exclusive_or_expression '^' and_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_BITWISE_XOR, $1, $3, ast_get_locus($1), NULL);
}
;

inclusive_or_expression : exclusive_or_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| inclusive_or_expression '|' exclusive_or_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_BITWISE_OR, $1, $3, ast_get_locus($1), NULL);
}
;

logical_and_expression : inclusive_or_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| logical_and_expression ANDAND inclusive_or_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_LOGICAL_AND, $1, $3, ast_get_locus($1), NULL);
}
;

logical_or_expression : logical_and_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| logical_or_expression OROR logical_and_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_LOGICAL_OR, $1, $3, ast_get_locus($1), NULL);
}
;

conditional_expression : logical_or_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| logical_or_expression '?' expression ':' conditional_expression %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_CONDITIONAL_EXPRESSION, $1, $3, $5, ast_get_locus($1), NULL);
}
// GNU Extension
| logical_or_expression '?' ':' conditional_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_GCC_CONDITIONAL_EXPRESSION, $1, $4, ast_get_locus($1), NULL);
}
;

assignment_expression : conditional_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| logical_or_expression assignment_operator assignment_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2($2, $1, $3, ast_get_locus($1), NULL);
}
;

expression : assignment_expression %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_EXPRESSION, $1, ast_get_locus($1), NULL);
}
| expression ',' assignment_expression %merge<ambiguityHandler>
{
	AST comma_expression = ASTMake2(AST_COMMA, $1, $3, ast_get_locus($1), NULL);

	$$ = ASTMake1(AST_EXPRESSION, comma_expression, ast_get_locus(comma_expression), NULL);
}
;

assignment_operator : '='
{
	$$ = AST_ASSIGNMENT;
}
| MUL_ASSIGN
{
	$$ = AST_MUL_ASSIGNMENT;
}
| DIV_ASSIGN
{
	$$ = AST_DIV_ASSIGNMENT;
}
| ADD_ASSIGN
{
	$$ = AST_ADD_ASSIGNMENT;
}
| SUB_ASSIGN
{
	$$ = AST_SUB_ASSIGNMENT;
}
| LEFT_ASSIGN
{
    $$ = AST_BITWISE_SHL_ASSIGNMENT;
}
| RIGHT_ASSIGN
{
	$$ = AST_SHR_ASSIGNMENT;
}
| AND_ASSIGN
{
	$$ = AST_BITWISE_AND_ASSIGNMENT;
}
| OR_ASSIGN
{
	$$ = AST_BITWISE_OR_ASSIGNMENT;
}
| XOR_ASSIGN
{
	$$ = AST_BITWISE_XOR_ASSIGNMENT;
}
| MOD_ASSIGN
{
	$$ = AST_MOD_ASSIGNMENT;
}
;

constant_expression : conditional_expression
{
	$$ = ASTMake1(AST_CONSTANT_EXPRESSION, $1, ast_get_locus($1), NULL);
}
;

// *********************************************************
// A.2 - Lexical conventions
// *********************************************************

literal : DECIMAL_LITERAL
{
	$$ = ASTLeaf(AST_DECIMAL_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| OCTAL_LITERAL
{
	$$ = ASTLeaf(AST_OCTAL_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| BINARY_LITERAL
{
	$$ = ASTLeaf(AST_BINARY_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| HEXADECIMAL_LITERAL
{
	$$ = ASTLeaf(AST_HEXADECIMAL_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| HEXADECIMAL_FLOAT
{
    $$ = ASTLeaf(AST_HEXADECIMAL_FLOAT, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| FLOATING_LITERAL
{
	$$ = ASTLeaf(AST_FLOATING_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| BOOLEAN_LITERAL
{
	$$ = ASTLeaf(AST_BOOLEAN_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| CHARACTER_LITERAL
{
	$$ = ASTLeaf(AST_CHARACTER_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| string_literal
{
	$$ = $1;
}
;

// This eases parsing, though it should be viewed as a lexical issue
string_literal : STRING_LITERAL
{
	$$ = ASTLeaf(AST_STRING_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| string_literal STRING_LITERAL
{
	// Let's concatenate here, it will ease everything

	const char* str1 = ASTText($1);
	const char* str2 = $2.token_text;
	char* text = NEW_VEC0(char, strlen(str1) + strlen(str2) + 1);

	strcat(text, str1);

	// Append the second string
	strcat(text, str2);

	$$ = ASTLeaf(AST_STRING_LITERAL, ast_get_locus($1), text);
}
;

identifier_token : IDENTIFIER
{
    $$ = $1;
}
;





















translation_unit : subparsing
{
	*parsed_tree = $1;
}
;

subparsing : SUBPARSE_EXPRESSION expression
{
	$$ = $2;
}
| SUBPARSE_STATEMENT statement_seq
{
	$$ = $2;
}
| SUBPARSE_STATEMENT
{
	$$ = NULL;
}
| SUBPARSE_MEMBER member_specification_seq
{
	$$ = $2;
}
| SUBPARSE_DECLARATION declaration_sequence
{
	$$ = $2;
}
| SUBPARSE_DECLARATION
{
    $$ = NULL;
}
| SUBPARSE_TYPE type_id
{
    $$ = $2;
}
| SUBPARSE_TYPE_LIST subparse_type_list
{
    $$ = $2;
}
| SUBPARSE_EXPRESSION_LIST expression_list
{
    $$ = $2;
}
| SUBPARSE_ID_EXPRESSION id_expression
{
    $$ = $2;
}
;






















subparse_type_list : type_specifier_seq
{
    $$ = ASTListLeaf($1);
}
| subparse_type_list ',' type_specifier_seq
{
    $$ = ASTList($1, $3);
}
;























nodecl_literal_expr : NODECL_LITERAL_EXPR '(' nodecl_literal_attribute_seq  ')'
{
    $$ = ASTMake1(AST_NODECL_LITERAL, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;


nodecl_literal_stmt : NODECL_LITERAL_STMT '(' nodecl_literal_attribute_seq ')'




{
    $$ = ASTMake1(AST_NODECL_LITERAL, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

nodecl_literal_attribute_seq : nodecl_literal_attribute_seq ',' nodecl_string_literal
{
    $$ = ASTList($1, $3);
}
| nodecl_string_literal
{
    $$ = ASTListLeaf($1);
}
;

symbol_literal_ref : SYMBOL_LITERAL_REF '(' nodecl_string_literal ')'
{
    $$ = ASTMake1(AST_SYMBOL_LITERAL_REF, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

type_literal_ref : TYPE_LITERAL_REF '(' nodecl_string_literal ')'
{
    $$ = ASTMake1(AST_TYPE_LITERAL_REF, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;





nodecl_string_literal : STRING_LITERAL

{
    $$ = ASTLeaf(AST_STRING_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

/* Entry points in the grammars */

primary_expression : nodecl_literal_expr
{
    $$ = $1;
}
;

unqualified_id : symbol_literal_ref
{
    $$ = $1;
}
;









nondeclarating_statement : nodecl_literal_stmt
{
    $$ = $1;
}
;


simple_type_specifier : type_literal_ref




{
    $$ = $1;
}
;








































primary_expression : MCC_ARRAY_SUBSCRIPT_CHECK '(' assignment_expression ',' constant_expression ')'
{
    $$ = ASTMake2(AST_MCC_ARRAY_SUBSCRIPT_CHECK, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| MCC_CONST_VALUE_CHECK '(' assignment_expression ')'
{
    $$ = ASTMake1(AST_MCC_CONSTANT_VALUE_CHECK, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

postfix_expression : postfix_expression '[' expression_opt ':' expression_opt ']'
{
    $$ = ASTMake4(AST_ARRAY_SECTION, $1, $3, $5, NULL, ast_get_locus($1), NULL);
}
| postfix_expression '[' expression_opt ':' expression_opt ':' expression ']'
{
    $$ = ASTMake4(AST_ARRAY_SECTION, $1, $3, $5, $7, ast_get_locus($1), NULL);
}
| postfix_expression '[' expression ';' expression ']'
{
    $$ = ASTMake4(AST_ARRAY_SECTION_SIZE, $1, $3, $5, NULL, ast_get_locus($1), NULL);
}
| postfix_expression '[' expression ';' expression ':' expression ']'
{
    $$ = ASTMake4(AST_ARRAY_SECTION_SIZE, $1, $3, $5, $7, ast_get_locus($1), NULL);
}
;

noshape_cast_expression : unary_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| '(' type_id ')' cast_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_CAST, $2, ASTListLeaf($4), make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

cast_expression : shape_seq noshape_cast_expression %merge<ambiguityHandler>
{
    $$ = ASTMake2(AST_SHAPING_EXPRESSION, $1, $2, ast_get_locus($1), NULL);
}
;

shape_seq : shape_seq shape %dprec 2
{
    $$ = ASTList($1, $2);
}
| shape %dprec 1
{
    $$ = ASTListLeaf($1);
}
;

shape: '[' expression ']'
{
    $$ = $2;
}
;

iteration_statement : FOR '[' symbol_literal_ref ']' '(' for_init_statement condition_opt ';' expression_opt ')' statement
{
    AST loop_control = ASTMake3(AST_LOOP_CONTROL, $6, $7, $9, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
	$$ = ASTMake4(AST_FOR_STATEMENT, loop_control, $11, NULL, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

}
;


nontype_specifier_without_attribute : mercurium_extended_type_specifiers
{
    $$ = $1;
}
;










mercurium_extended_type_specifiers : MCC_BOOL
{
	$$ = ASTLeaf(AST_MCC_BOOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| MCC_MASK
{
	$$ = ASTLeaf(AST_MCC_MASK, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

primary_expression : multiexpression;

multiexpression : '{' '/' multiexpression_body '/' '}'
{
    $$ = $3;
}
;

multiexpression_body : assignment_expression ',' multiexpression_iterator_list
{
    $$ = ASTMake2(AST_MULTIEXPRESSION, $1, $3, ast_get_locus($1), NULL);
}
;


multiexpression_iterator_list : multiexpression_iterator
{
	$$ = ASTListLeaf($1);
}
| multiexpression_iterator_list ',' multiexpression_iterator
{
	$$ = ASTList($1, $3);
}
;


multiexpression_iterator : identifier_token '=' multiexpression_range
{
    AST symbol = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
    $$ = ASTMake2(AST_MULTIEXPRESSION_ITERATOR, symbol, $3, ast_get_locus(symbol), NULL);
}
;

multiexpression_range : multiexpression_range_size
{
    $$ = $1;
}
| multiexpression_range_section
{
    $$ = $1;
}
| multiexpression_range_discrete
{
    $$ = $1;
}
;

multiexpression_range_section : assignment_expression ':' assignment_expression
{
    $$ = ASTMake3(AST_MULTIEXPRESSION_RANGE_SECTION, $1, $3, NULL, ast_get_locus($1), NULL);
}
| assignment_expression ':' assignment_expression ':' assignment_expression
{
    $$ = ASTMake3(AST_MULTIEXPRESSION_RANGE_SECTION, $1, $3, $5, ast_get_locus($1), NULL);
}
;

multiexpression_range_size : assignment_expression ';' assignment_expression
{
    $$ = ASTMake3(AST_MULTIEXPRESSION_RANGE_SIZE, $1, $3, NULL, ast_get_locus($1), NULL);
}
| assignment_expression ';' assignment_expression ':' assignment_expression
{
    $$ = ASTMake3(AST_MULTIEXPRESSION_RANGE_SIZE, $1, $3, $5, ast_get_locus($1), NULL);
}
;

multiexpression_range_discrete : '{' expression_list '}'
{
    $$ = ASTMake1(AST_MULTIEXPRESSION_RANGE_DISCRETE, $2, ast_get_locus($2), NULL);
}













nondeclarating_statement : fortran_allocate_statement
{
    $$ = $1;
}
;

fortran_allocate_statement : C_FORTRAN_ALLOCATE '(' expression ')' ';'
{
    $$ = ASTMake1(AST_FORTRAN_ALLOCATE_STATEMENT, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;
















statement : statement_placeholder
{
    $$ = $1;
}
;



statement_placeholder : STATEMENT_PLACEHOLDER 
{
    // This is an empty statement
    $$ = ASTLeaf(AST_STATEMENT_PLACEHOLDER, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;












// This is code




































































// ****************************
//   Unknown pragma
// ****************************


unknown_pragma : UNKNOWN_PRAGMA
{
	$$ = ASTLeaf(AST_UNKNOWN_PRAGMA, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

common_block_declaration : unknown_pragma
{
    $$ = $1;
}
;

member_declaration : unknown_pragma
{
    $$ = $1;
}
;




















// ****************************
//   Pragma rules
// ****************************


nondeclarating_statement : pragma_custom_construct_statement
{
    $$ = $1;
}
| pragma_custom_directive
{
    $$ = $1;
}
;

declaration : pragma_custom_construct_declaration
{
    $$ = $1;
}
| pragma_custom_directive
{
	$$ = $1;
}
;

member_declaration : pragma_custom_construct_member_declaration
{
    $$ = $1;
}
| pragma_custom_directive
{
    $$ = $1;
}
;































// Pragma custom

pragma_custom_directive : PRAGMA_CUSTOM pragma_custom_line_directive
{
	$$ = ASTMake2(AST_PRAGMA_CUSTOM_DIRECTIVE, $2, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;


pragma_custom_construct_declaration : PRAGMA_CUSTOM pragma_custom_line_construct declaration
{
	$$ = ASTMake3(AST_PRAGMA_CUSTOM_CONSTRUCT, $2, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

pragma_custom_construct_member_declaration : PRAGMA_CUSTOM pragma_custom_line_construct member_declaration
{
	$$ = ASTMake3(AST_PRAGMA_CUSTOM_CONSTRUCT, $2, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

pragma_custom_construct_statement : PRAGMA_CUSTOM pragma_custom_line_construct statement
{
	$$ = ASTMake3(AST_PRAGMA_CUSTOM_CONSTRUCT, $2, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;





























































































































pragma_custom_line_directive : PRAGMA_CUSTOM_DIRECTIVE pragma_custom_clause_opt_seq PRAGMA_CUSTOM_NEWLINE
{
	$$ = ASTMake2(AST_PRAGMA_CUSTOM_LINE, $2, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| PRAGMA_CUSTOM_DIRECTIVE '(' pragma_clause_arg_list ')' pragma_custom_clause_opt_seq PRAGMA_CUSTOM_NEWLINE
{
	$$ = ASTMake2(AST_PRAGMA_CUSTOM_LINE, $5, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| PRAGMA_CUSTOM_NEWLINE
{
    // This is a degenerated case caused by wrong designed pragmas
    $$ = ASTMake2(AST_PRAGMA_CUSTOM_LINE, NULL, NULL, make_locus("", 0, 0), NULL);
}
;

pragma_custom_line_construct : PRAGMA_CUSTOM_CONSTRUCT pragma_custom_clause_opt_seq PRAGMA_CUSTOM_NEWLINE
{
	$$ = ASTMake2(AST_PRAGMA_CUSTOM_LINE, $2, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| PRAGMA_CUSTOM_CONSTRUCT '(' pragma_clause_arg_list ')' pragma_custom_clause_opt_seq PRAGMA_CUSTOM_NEWLINE
{
	$$ = ASTMake2(AST_PRAGMA_CUSTOM_LINE, $5, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

pragma_custom_clause_opt_seq : /* empty */
{
	$$ = NULL;
}
| pragma_custom_clause_seq
{
	$$ = $1;
}
;

pragma_custom_clause_seq : pragma_custom_clause
{
	$$ = ASTListLeaf($1);
}
| pragma_custom_clause_seq ',' pragma_custom_clause
{
	$$ = ASTList($1, $3);
}
| pragma_custom_clause_seq pragma_custom_clause
{
	$$ = ASTList($1, $2);
}
;

pragma_custom_clause : PRAGMA_CUSTOM_CLAUSE '(' pragma_clause_arg_list ')'
{
	$$ = ASTMake1(AST_PRAGMA_CUSTOM_CLAUSE, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| PRAGMA_CUSTOM_CLAUSE '(' ')'
{
	$$ = ASTMake1(AST_PRAGMA_CUSTOM_CLAUSE, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| PRAGMA_CUSTOM_CLAUSE 
{
	$$ = ASTMake1(AST_PRAGMA_CUSTOM_CLAUSE, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

pragma_clause_arg_list : pragma_clause_arg
{
    AST node = ASTLeaf(AST_PRAGMA_CLAUSE_ARG, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);

    $$ = ASTListLeaf(node);
}
;

pragma_clause_arg : pragma_clause_arg_item
{
    $$ = $1;
}
| pragma_clause_arg pragma_clause_arg_item
{
    $$.token_text = strappend($1.token_text, $2.token_text);
}
;

pragma_clause_arg_item : pragma_clause_arg_text
{
    $$ = $1;
}
;

pragma_clause_arg_text : PRAGMA_CLAUSE_ARG_TEXT
{
    $$ = $1;
}
;


// Verbatim construct
verbatim_construct : VERBATIM_PRAGMA VERBATIM_TYPE '(' identifier_token ')' VERBATIM_TEXT
{
    AST ident = ASTLeaf(AST_SYMBOL, make_locus(@4.first_filename, @4.first_line, @4.first_column), $4.token_text);

    $$ = ASTMake1(AST_VERBATIM, ident, make_locus(@1.first_filename, @1.first_line, @1.first_column), $6.token_text);
}
| VERBATIM_PRAGMA VERBATIM_TEXT
{
    $$ = ASTMake1(AST_VERBATIM, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $2.token_text);
}
;

common_block_declaration : verbatim_construct
{
    $$ = $1;
}
;

member_declaration : verbatim_construct
{
    $$ = $1;
}
;
























subparsing : SUBPARSE_OPENMP_DECLARE_REDUCTION omp_declare_reduction
{
    $$ = $2;
}
| SUBPARSE_OPENMP_DEPEND_ITEM omp_depend_item
{
    $$ = $2;
}
;

omp_declare_reduction : omp_dr_reduction_id ':' omp_dr_typename_list ':' omp_dr_combiner
{
    $$ = ASTMake4(AST_OMP_DECLARE_REDUCTION, $1, $3, $5, NULL, ast_get_locus($1), NULL);
}
| omp_dr_reduction_id ':' omp_dr_typename_list ':' omp_dr_combiner ':' omp_dr_initializer
{
    $$ = ASTMake4(AST_OMP_DECLARE_REDUCTION, $1, $3, $5, $7, ast_get_locus($1), NULL);
}
;

omp_dr_reduction_id : omp_dr_operator
{
    $$ = ASTLeaf(AST_OMP_DR_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| omp_dr_identifier
{
    $$ = $1;
}
;

omp_dr_identifier : identifier_token
{
    $$ = ASTLeaf(AST_OMP_DR_IDENTIFIER, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

omp_dr_typename_list : omp_dr_typename
{
    $$ = ASTListLeaf($1);
}
| omp_dr_typename_list ',' omp_dr_typename
{
    $$ = ASTList($1, $3);
}
;

omp_dr_operator : '+'
| '-'
| '*'
| '&'
| '|'
| '^'
| ANDAND
| OROR
;


omp_dr_typename : type_specifier_seq
{
    $$ = $1;
}
;














omp_dr_combiner : expression
{
    $$ = $1;
}
;


omp_dr_initializer : id_expression initializer %merge<ambiguityHandler>
{
    AST declarator_id = ASTMake1(AST_DECLARATOR_ID_EXPR, $1, ast_get_locus($1), NULL);
    AST declarator = ASTMake1(AST_DECLARATOR, declarator_id, ast_get_locus($1), NULL);

    $$ = ASTMake2(AST_INIT_DECLARATOR, declarator, $2, ast_get_locus($1), NULL);
}
| postfix_expression '(' ')' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_FUNCTION_CALL, $1, NULL, ast_get_locus($1), NULL);
}
| postfix_expression '(' expression_list ')' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_FUNCTION_CALL, $1, $3, ast_get_locus($1), NULL);
}
;






















omp_expression_opt : expression
{
    $$ = $1;
}
| /* empty */
{
    $$ = NULL;
}
;

omp_depend_item : id_expression
{
    $$ = $1;
}
| omp_depend_item '[' omp_expression_opt ':' omp_expression_opt ']'
{
    // Note that this is to be interpreted as a [lower:size] (not [lower:upper]),
    // so we create an AST_ARRAY_SECTION_SIZE here
    $$ = ASTMake4(AST_ARRAY_SECTION_SIZE, $1, $3, $5, NULL, ast_get_locus($1), NULL);
}
| omp_depend_item '[' expression ']'
{
    // Note that this is to be interpreted as a [lower:size] (not [lower:upper]),
    // so we create an AST_ARRAY_SECTION_SIZE here
    $$ = ASTMake2(AST_ARRAY_SUBSCRIPT, $1, $3, ast_get_locus($1), NULL);
}
;












subparsing : SUBPARSE_OMPSS_DEPENDENCY_EXPRESSION ompss_dependency_expr
{
    $$ = $2;
}
;

ompss_dependency_expr : assignment_expression
{
    $$ = $1;
}
| ompss_old_multidependences
{
    $$ = $1;
}
;

ompss_old_multidependences : '{' multiexpression_body '}'
{
    $$ = $2;
}


















// Grammar entry point
subparsing : SUBPARSE_SUPERSCALAR_DECLARATOR superscalar_declarator opt_superscalar_region_spec_list
{
	$$ = ASTMake2(AST_SUPERSCALAR_DECLARATOR, $2, $3, ast_get_locus($2), NULL);
}
| SUBPARSE_SUPERSCALAR_DECLARATOR_LIST superscalar_declarator_list
{
    $$ = $2;
}
| SUBPARSE_SUPERSCALAR_EXPRESSION expression opt_superscalar_region_spec_list
{
	$$ = ASTMake2(AST_SUPERSCALAR_EXPRESSION, $2, $3, ast_get_locus($2), NULL);
}
;

superscalar_declarator_list : superscalar_declarator opt_superscalar_region_spec_list
{
	AST ss_decl = ASTMake2(AST_SUPERSCALAR_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
    $$ = ASTListLeaf(ss_decl);
}
| superscalar_declarator_list ',' superscalar_declarator opt_superscalar_region_spec_list
{
	AST ss_decl = ASTMake2(AST_SUPERSCALAR_DECLARATOR, $3, $4, ast_get_locus($3), NULL);
    $$ = ASTList($1, ss_decl);
}
;

superscalar_declarator : declarator_id
{
	$$ = $1;
}
| superscalar_declarator '[' assignment_expression ']'
{
	$$ = ASTMake4(AST_DECLARATOR_ARRAY, $1, $3, NULL, NULL, ast_get_locus($1), NULL);
}
;

opt_superscalar_region_spec_list :
/* NULL */
{
	$$ = NULL;
}
| superscalar_region_spec_list
;

superscalar_region_spec_list : superscalar_region_spec
{
	$$ = ASTListLeaf($1);
}
| superscalar_region_spec_list superscalar_region_spec
{
	$$ = ASTList($1, $2);
}
;

superscalar_region_spec : '{' '}'
{
	$$ = ASTLeaf(AST_SUPERSCALAR_REGION_SPEC_FULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '{' expression '}'
{
	$$ = ASTMake1(AST_SUPERSCALAR_REGION_SPEC_SINGLE, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '{' expression TWO_DOTS expression '}'
{
	$$ = ASTMake2(AST_SUPERSCALAR_REGION_SPEC_RANGE, $2, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '{' expression ':' expression '}'
{
	$$ = ASTMake2(AST_SUPERSCALAR_REGION_SPEC_LENGTH, $2, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;






























unary_expression : UPC_LOCALSIZEOF unary_expression
{
    $$ = ASTMake1(AST_UPC_LOCALSIZEOF, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| UPC_LOCALSIZEOF '(' type_id ')'
{
    $$ = ASTMake1(AST_UPC_LOCALSIZEOF_TYPEID, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| UPC_BLOCKSIZEOF unary_expression
{
    $$ = ASTMake1(AST_UPC_BLOCKSIZEOF, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| UPC_BLOCKSIZEOF '(' type_id ')'
{
    $$ = ASTMake1(AST_UPC_BLOCKSIZEOF_TYPEID, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| UPC_ELEMSIZEOF unary_expression
{
    $$ = ASTMake1(AST_UPC_ELEMSIZEOF, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| UPC_ELEMSIZEOF '(' type_id ')'
{
    $$ = ASTMake1(AST_UPC_ELEMSIZEOF_TYPEID, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

cv_qualifier : upc_shared_type_qualifier
{
    $$ = $1;
}
| upc_reference_type_qualifier
{
    $$ = $1;
}
;

upc_shared_type_qualifier : UPC_SHARED
{
    $$ = ASTMake1(AST_UPC_SHARED, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| UPC_SHARED upc_layout_qualifier
{
    $$ = ASTMake1(AST_UPC_SHARED, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

upc_reference_type_qualifier : UPC_RELAXED
{
    $$ = ASTLeaf(AST_UPC_RELAXED, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| UPC_STRICT
{
    $$ = ASTLeaf(AST_UPC_STRICT, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

// UPC only allows one of these qualifiers but as an extension we allow a list
upc_layout_qualifier: upc_layout_qualifier_element
{
    $$ = ASTListLeaf($1);
}
| upc_layout_qualifier upc_layout_qualifier_element
{
    $$ = ASTList($1, $2);
}
;

upc_layout_qualifier_element : '[' ']'
{
    $$ = ASTMake1(AST_UPC_LAYOUT_QUALIFIER, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '[' constant_expression ']'
{
    $$ = ASTMake1(AST_UPC_LAYOUT_QUALIFIER, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '[' '*' ']'
{
    $$ = ASTMake1(AST_UPC_LAYOUT_QUALIFIER, 
            ASTLeaf(AST_UPC_LAYOUT_UNDEF, make_locus(@2.first_filename, @2.first_line, @2.first_column), NULL), 
            make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

nondeclarating_statement : upc_synchronization_statement
{
    $$ = $1;
}
;

upc_synchronization_statement : UPC_NOTIFY upc_expression_opt ';'
{
    $$ = ASTMake1(AST_UPC_NOTIFY, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| UPC_WAIT upc_expression_opt ';'
{
    $$ = ASTMake1(AST_UPC_WAIT, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| UPC_BARRIER upc_expression_opt ';'
{
    $$ = ASTMake1(AST_UPC_BARRIER, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| UPC_FENCE ';'
{
    $$ = ASTLeaf(AST_UPC_FENCE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

upc_expression_opt : expression
{
    $$ = $1;
}
|
{
    $$ = NULL;
}
;

iteration_statement : UPC_FORALL '(' for_init_statement upc_expression_opt ';' upc_expression_opt ';' upc_affinity_opt ')' statement
{
    AST upc_forall_header =
        ASTMake4(AST_UPC_FORALL_HEADER, $3, $4, $6, $8, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

    $$ = ASTMake2(AST_UPC_FORALL, upc_forall_header, $10, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

upc_affinity_opt : upc_affinity
{
    $$ = $1;
}
| 
{
    $$ = NULL;
};

upc_affinity : expression
{
    $$ = $1;
}
| CONTINUE
{
    $$ = ASTLeaf(AST_UPC_CONTINUE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;






















nontype_specifier_without_attribute : cuda_specifiers
{
    $$ = $1;
}
;

postfix_expression : cuda_kernel_call
{
    $$ = $1;
}
;

cuda_specifiers : CUDA_DEVICE
{
    $$ = ASTLeaf(AST_CUDA_DEVICE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| CUDA_GLOBAL
{
    $$ = ASTLeaf(AST_CUDA_GLOBAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| CUDA_HOST
{
    $$ = ASTLeaf(AST_CUDA_HOST, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| CUDA_CONSTANT
{
    $$ = ASTLeaf(AST_CUDA_CONSTANT, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| CUDA_SHARED
{
    $$ = ASTLeaf(AST_CUDA_SHARED, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

cuda_kernel_call : postfix_expression cuda_kernel_arguments '(' ')'
{
    $$ = ASTMake3(AST_CUDA_KERNEL_CALL, $1, $2, NULL, ast_get_locus($1), NULL);
}
| postfix_expression cuda_kernel_arguments '(' expression_list ')'
{
    $$ = ASTMake3(AST_CUDA_KERNEL_CALL, $1, $2, $4, ast_get_locus($1), NULL);
}
;

cuda_kernel_arguments : cuda_kernel_config_left cuda_kernel_config_list cuda_kernel_config_right
{
    $$ = $2;
}
;

cuda_kernel_config_list : assignment_expression ',' assignment_expression ',' assignment_expression ',' assignment_expression
{
    $$ = ASTMake4(AST_CUDA_KERNEL_ARGUMENTS, $1, $3, $5, $7, ast_get_locus($1), NULL);
}
| assignment_expression ',' assignment_expression ',' assignment_expression
{
    $$ = ASTMake4(AST_CUDA_KERNEL_ARGUMENTS, $1, $3, $5, NULL, ast_get_locus($1), NULL);
}
| assignment_expression ',' assignment_expression
{
    $$ = ASTMake4(AST_CUDA_KERNEL_ARGUMENTS, $1, $3, NULL, NULL, ast_get_locus($1), NULL);
}
;

cuda_kernel_config_left : CUDA_KERNEL_LEFT
{
    $$ = $1;
}
;


cuda_kernel_config_right : CUDA_KERNEL_RIGHT
{
    $$ = $1;
}
;



























nontype_specifier_without_attribute : opencl_specifiers
{
    $$ = $1;
}
;

opencl_specifiers : OPENCL_GLOBAL
{
    $$ = ASTLeaf(AST_OPENCL_GLOBAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| OPENCL_KERNEL
{
    $$ = ASTLeaf(AST_OPENCL_KERNEL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| OPENCL_CONSTANT
{
    $$ = ASTLeaf(AST_OPENCL_CONSTANT, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| OPENCL_LOCAL
{
    $$ = ASTLeaf(AST_OPENCL_LOCAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;








nontype_specifier_without_attribute : XL_BUILTIN_SPEC
{
    $$ = ASTLeaf(AST_XL_BUILTIN_SPEC, make_locus(@1.first_filename, @1.first_line, 0), $1.token_text);
}
;


gcc_extra_bits_init_declarator : unknown_pragma attribute_specifier_seq
{
    if (CURRENT_CONFIGURATION->native_vendor == NATIVE_VENDOR_IBM)
    {
        $$ = ast_list_concat(ASTListLeaf($1), $2);
    }
    else
    {
        warn_printf_at(ast_get_locus($1), "ignoring '#pragma %s' after the declarator\n",
                 ast_get_text($1));
        $$ = $2;
    }
}
| unknown_pragma
{
    if (CURRENT_CONFIGURATION->native_vendor == NATIVE_VENDOR_IBM)
    {
        $$ = ASTListLeaf($1);
    }
    else
    {
        warn_printf_at(ast_get_locus($1), "ignoring '#pragma %s' after the declarator\n",
                 ast_get_text($1));
        $$ = NULL;
    }
}
;




















nontype_specifier : declspec_specifier
{
    $$ = $1;
}
;

declspec_specifier : TOKEN_DECLSPEC '(' extended_decl_modifier_list ')'
{
    $$ = ASTMake1(AST_MS_DECLSPEC, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

extended_decl_modifier_list : /* empty */
{
    $$ = NULL;
}
| extended_decl_modifier_list0
{
    $$ = $1;
}
;

extended_decl_modifier_list0 : extended_decl_modifier
{
    $$ = ASTListLeaf($1);
}
| extended_decl_modifier_list0 ',' extended_decl_modifier
{
    $$ = ASTList($1, $3);
}
;

extended_decl_modifier : identifier_token
{
    $$ = ASTMake1(AST_MS_DECLSPEC_ITEM, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| identifier_token '(' expression_list ')'
{
    $$ = ASTMake1(AST_MS_DECLSPEC_ITEM, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

attribute_specifier : declspec_specifier
{
    $$ = $1;
}
;

builtin_types : MS_INT8
{
    $$ = ASTLeaf(AST_MS_INT8, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| MS_INT16
{
    $$ = ASTLeaf(AST_MS_INT16, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| MS_INT32
{
    $$ = ASTLeaf(AST_MS_INT32, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| MS_INT64
{
    $$ = ASTLeaf(AST_MS_INT64, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;











primary_expression : INTEL_ASSUME '(' expression ')'
{
    $$ = ASTMake1(AST_INTEL_ASSUME, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| INTEL_ASSUME_ALIGNED '(' assignment_expression ',' assignment_expression ')'
{
    $$ = ASTMake2(AST_INTEL_ASSUME_ALIGNED, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;




%%










































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
