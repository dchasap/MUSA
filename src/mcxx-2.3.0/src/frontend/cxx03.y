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
   Parser of ISO/IEC 14882:2003 - C++

   Accepts a superset of the language that must be typechecked later
 */

#include "cxx-ast.h"
#include "cxx-graphviz.h"
#include "cxx-parser.h"
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


// This is a specific feature of rofi-bison 2.3
// %default-merge <ambiguityHandler>

%parse-param {AST* parsed_tree}

%locations

%{
extern int yylex(void);
static AST ambiguityHandler (YYSTYPE x0, YYSTYPE x1);
void yyerror(AST* parsed_tree UNUSED_PARAMETER, const char* c);

%}

%token END 0 "end of input"

// C++ tokens
%token<token_atrib> ADD_ASSIGN "+="
%token<token_atrib> ANDAND "&&"
%token<token_atrib> AND_ASSIGN "&="
%token<token_atrib> ASM "__asm__"
%token<token_atrib> TOKEN_AUTO_STORAGE "auto storage-specifier"
%token<token_atrib> TOKEN_AUTO_TYPE "auto type-specifier"
%token<token_atrib> TOKEN_BOOL "bool"
%token<token_atrib> BOOLEAN_LITERAL "boolean-literal"
%token<token_atrib> BREAK "break"
%token<token_atrib> CASE "case"
%token<token_atrib> CATCH "catch"
%token<token_atrib> TOKEN_CHAR "char"
%token<token_atrib> CHARACTER_LITERAL "character-literal"
%token<token_atrib> CLASS "class"
%token<token_atrib> TOKEN_CONST "const"
%token<token_atrib> CONST_CAST "const_cast"
%token<token_atrib> CONTINUE "continue"
%token<token_atrib> DECIMAL_LITERAL "decimal-literal"
%token<token_atrib> DEFAULT "default"
%token<token_atrib> TOKEN_DELETE "delete"
%token<token_atrib> TOKEN_DELETE_ARRAY "delete []"
%token<token_atrib> DIV_ASSIGN "/="
%token<token_atrib> DO "do"
%token<token_atrib> TWO_COLONS "::"
%token<token_atrib> TWO_COLONS_AND_STAR ":: *"
%token<token_atrib> TOKEN_DOUBLE "double"
%token<token_atrib> DYNAMIC_CAST "dynamic_cast"
%token<token_atrib> ELSE "else"
%token<token_atrib> ENUM "enum"
%token<token_atrib> EQUAL "=="
%token<token_atrib> DECLTYPE "decltype"
%token<token_atrib> EXPLICIT "explicit"
%token<token_atrib> EXPORT "export"
%token<token_atrib> EXTERN "extern"
%token<token_atrib> TOKEN_FLOAT "float"
%token<token_atrib> FLOATING_LITERAL "floating-literal"
%token<token_atrib> HEXADECIMAL_FLOAT "hexadecimal-floating-literal"
%token<token_atrib> FOR "for"
%token<token_atrib> FRIEND "friend"
%token<token_atrib> GOTO "goto"
%token<token_atrib> BINARY_LITERAL "binary-integer-literal"
%token<token_atrib> HEXADECIMAL_LITERAL "hexadecimal-integer-literal"
%token<token_atrib> IDENTIFIER "identifier"
%token<token_atrib> TOK_FINAL "final"
%token<token_atrib> TOK_OVERRIDE "override"
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
%token<token_atrib> MUTABLE "mutable"
%token<token_atrib> NAMESPACE "namespace"
%token<token_atrib> TOKEN_NEW "new"
%token<token_atrib> TOKEN_NEW_ARRAY "new []"
%token<token_atrib> NOT_EQUAL "!="
%token<token_atrib> OCTAL_LITERAL "octal-literal"
%token<token_atrib> OPERATOR "operator"
%token<token_atrib> OPERATOR_LITERAL "literal-operator-id"
%token<token_atrib> OR_ASSIGN "|="
%token<token_atrib> OROR "||"
%token<token_atrib> PLUSPLUS "++"
%token<token_atrib> PRIVATE "private"
%token<token_atrib> PROTECTED "protected"
%token<token_atrib> PTR_OP "->"
%token<token_atrib> PTR_OP_MUL "->*"
%token<token_atrib> PUBLIC "public"
%token<token_atrib> REGISTER "register"
%token<token_atrib> REINTERPRET_CAST "reinterpret_cast"
%token<token_atrib> RETURN "return"
%token<token_atrib> TOKEN_SHORT "short"
%token<token_atrib> TOKEN_SIGNED "signed"
%token<token_atrib> SIZEOF "sizeof"
%token<token_atrib> STATIC "static"
%token<token_atrib> STATIC_CAST "static_cast"
%token<token_atrib> STRING_LITERAL "string-literal"
%token<token_atrib> STRUCT "struct"
%token<token_atrib> SUB_ASSIGN "-="
%token<token_atrib> SWITCH "switch"
%token<token_atrib> TEMPLATE "template"
%token<token_atrib> TOKEN_THIS "this"
%token<token_atrib> THROW "throw"
%token<token_atrib> ELLIPSIS "..."
%token<token_atrib> TRY "try"
%token<token_atrib> TYPEDEF "typedef"
%token<token_atrib> TYPEID "typeid"
%token<token_atrib> TYPENAME "typename"
%token<token_atrib> UNION "union"
%token<token_atrib> TOKEN_UNSIGNED "unsigned"
%token<token_atrib> USING "using"
%token<token_atrib> VIRTUAL "virtual"
%token<token_atrib> TOKEN_VOID "void"
%token<token_atrib> TOKEN_VOLATILE "volatile"
%token<token_atrib> TOKEN_WCHAR_T "wchar_t"
%token<token_atrib> TOKEN_CHAR16_T "char16_t"
%token<token_atrib> TOKEN_CHAR32_T "char32_t"
%token<token_atrib> TOKEN_ALIGNOF "alignof"
%token<token_atrib> TOKEN_ALIGNAS "alignas"
%token<token_atrib> WHILE "while"
%token<token_atrib> XOR_ASSIGN "^="

%token<token_atrib> STATIC_ASSERT "static_assert"

%token<token_atrib> PP_COMMENT "<preprocessor-comment>"
%token<token_atrib> PP_TOKEN "<preprocessor-token>"

%token<token_atrib> MCC_REBINDABLE_REFERENCE "<rebindable-reference-specifier>"

// g++ type traits
%token<token_atrib> GXX_HAS_NOTHROW_ASSIGN "__has_nothrow_assign"
%token<token_atrib> GXX_HAS_NOTHROW_CONSTRUCTOR "__has_nothrow_constructor"
%token<token_atrib> GXX_HAS_NOTHROW_COPY "__has_nothrow_copy"
%token<token_atrib> GXX_HAS_TRIVIAL_ASSIGN "__has_trivial_assign"
%token<token_atrib> GXX_HAS_TRIVIAL_CONSTRUCTOR "__has_trivial_constructor"
%token<token_atrib> GXX_HAS_TRIVIAL_COPY "__has_trivial_copy"
%token<token_atrib> GXX_HAS_TRIVIAL_DESTRUCTOR "__has_trivial_destructor"
%token<token_atrib> GXX_HAS_VIRTUAL_DESTRUCTOR "__has_virtual_destructor"
%token<token_atrib> GXX_IS_ABSTRACT "__is_abstract"
%token<token_atrib> GXX_IS_BASE_OF "__is_base_of"
%token<token_atrib> GXX_IS_CLASS "__is_class"
%token<token_atrib> GXX_IS_CONVERTIBLE_TO "__is_convertible_to"
%token<token_atrib> GXX_IS_EMPTY "__is_empty"
%token<token_atrib> GXX_IS_ENUM "__is_enum"
%token<token_atrib> GXX_IS_LITERAL_TYPE "__is_literal_type"
%token<token_atrib> GXX_IS_POD "__is_pod"
%token<token_atrib> GXX_IS_POLYMORPHIC "__is_polymorphic"
%token<token_atrib> GXX_IS_STANDARD_LAYOUT "__is_standard_layout"
%token<token_atrib> GXX_IS_TRIVIAL "__is_trivial"
%token<token_atrib> GXX_IS_UNION "__is_union"
%token<token_atrib> GXX_IS_FINAL "__is_final"
%token<token_atrib> GXX_UNDERLYING_TYPE "__underlying_type"
%token<token_atrib> GXX_IS_TRIVIALLY_ASSIGNABLE "__is_trivially_assignable"
%token<token_atrib> GXX_IS_TRIVIALLY_CONSTRUCTIBLE "__is_trivially_constructible"
%token<token_atrib> GXX_IS_TRIVIALLY_COPYABLE "__is_trivially_copyable"

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
/* Different tokens in C++2003 and C++2011 */
/* C++2003 */
%token<token_atrib> RIGHT ">>"
%token<token_atrib> RIGHT_ASSIGN ">>="
%token<token_atrib> GREATER_OR_EQUAL ">="
/* C++2011 */
// AB1 is '>' when it is just before another '>'
// KLUDGE: Note that since names can't be repeated we surround this one with spaces
// The error message may be a bit weird, as if it was bad formatted
%token<token_atrib> AB1 "'>>' "
/* End of different tokens in C++2003 and C++2011 */
%token<token_atrib> '>'
%token<token_atrib> '?'
%token<token_atrib> '['
%token<token_atrib> ']'
%token<token_atrib> '^'
%token<token_atrib> '{'
%token<token_atrib> '|'
%token<token_atrib> '}'
%token<token_atrib> '~'

%token<token_atrib> STD_ATTRIBUTE_START "[["
%token<token_atrib> STD_ATTRIBUTE_END "]]"
%token<token_atrib> STD_ATTRIBUTE_TEXT "token-inside-attribute-clause"

%token<token_atrib> TOKEN_CONSTEXPR "constexpr"
%token<token_atrib> TOKEN_THREAD_LOCAL "thread_local"
%token<token_atrib> TOKEN_NULLPTR "nullptr"
%token<token_atrib> TOKEN_NOEXCEPT "noexcept "
%token<token_atrib> TOKEN_NOEXCEPT_ALONE "noexcept"

// Tokens implemented with grammar
%type<token_atrib> right_token
%type<token_atrib> right_assign_token
%type<token_atrib> greater_equal_token
%type<token_atrib> right_bracket_token
%type<token_atrib> identifier_token

// GNU Extensions
%token<token_atrib> BUILTIN_VA_ARG "__builtin_va_arg"
%token<token_atrib> BUILTIN_OFFSETOF "__builtin_offsetof"
%token<token_atrib> BUILTIN_ADDRESSOF "__builtin_addressof"
%token<token_atrib> TOKEN_GCC_ALIGNOF "__alignof__"
%token<token_atrib> EXTENSION "__extension__"
%token<token_atrib> REAL "__real__"
%token<token_atrib> IMAG "__imag__"
%token<token_atrib> LABEL "__label__"
%token<token_atrib> COMPLEX "__complex__"
%token<token_atrib> TYPEOF "__typeof__"
%token<token_atrib> RESTRICT "__restrict__"
%token<token_atrib> TOKEN_GCC_ATTRIBUTE "__attribute__"
%token<token_atrib> THREAD "__thread__"


// Nonterminals
%type<ast> abstract_declarator
%type<ast> abstract_declarator_not_started_with_attributes
%type<ast> abstract_main_declarator
%type<ast> abstract_main_declarator_not_started_with_attributes
%type<ast> abstract_direct_declarator
%type<ast> abstract_declarator_not_ellipsis
%type<ast> abstract_main_declarator_not_ellipsis
%type<ast> abstract_direct_declarator_not_ellipsis
%type<ast> access_specifier
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
%type<ast> gcc_attributes
%type<ast> gcc_attributes_opt
%type<ast> gcc_attribute_value
%type<ast> base_clause
%type<ast> base_clause_opt
%type<ast> base_specifier
%type<ast> base_specifier_list
%type<ast> class_or_decltype
%type<ast> block_declaration
%type<ast> non_empty_block_declaration
%type<ast> common_block_declaration
%type<ast> builtin_types
%type<ast> cast_expression
%type<ast> class_head
%type<ast> class_head_name
%type<ast> class_virt_specifier_seq_opt
%type<ast> class_virt_specifier_seq
%type<ast> class_virt_specifier
%type<ast> class_key
%type<ast> class_or_namespace_name
%type<ast> class_specifier
%type<ast> compound_statement
%type<ast> condition
%type<ast> condition_opt
%type<ast> conditional_expression
%type<ast> constant_expression
%type<ast> constant_expression_opt
%type<ast> conversion_declarator
%type<ast> conversion_function_id
%type<ast> conversion_type_id
%type<ast> ctor_initializer
%type<ast> ctor_initializer_opt
%type<ast> cv_qualifier
%type<ast> cv_qualifier_seq
%type<ast> cv_qualifier_seq_opt
%type<ast> declaration
%type<ast> declaration_sequence
%type<ast> declaration_sequence_opt
%type<ast> declaration_statement
%type<ast> declarator
%type<ast> declarator_not_started_with_attributes
%type<ast> declarator_id
%type<ast> decl_specifier_alone_seq
%type<ast> decl_specifier_seq_0
%type<ast> decl_specifier_seq_1
%type<ast> decl_specifier_seq_ended_with_named_type_spec
%type<ast> delete_expression
%type<ast> designation
%type<ast> designator
%type<ast> designator_list
%type<ast> offsetof_member_designator
%type<ast> direct_declarator
%type<ast> main_declarator
%type<ast> main_declarator_not_started_with_attributes
%type<ast> direct_new_declarator
%type<ast> elaborated_type_specifier
%type<ast> enumeration_definition
%type<ast> enumeration_list
%type<ast> enumeration_list_opt
%type<ast> enum_base_opt
%type<ast> enum_base_type_specifier_seq
%type<ast> enum_head
%type<ast> enum_key
%type<ast> enum_specifier
%type<ast> equality_expression
%type<ast> exception_declaration
%type<ast> exception_specification
%type<ast> dynamic_exception_specification
%type<ast> noexcept_specification
%type<ast> friend_specifier
%type<ast> friend_type_specifier_alone
%type<ast> friend_declaration
%type<ast> friend_function_definition
%type<ast> friend_function_definition_header
%type<ast> decl_specifier_seq_friend
%type<ast> decl_specifier_seq_friend_alone
%type<ast> decl_specifier_seq_friend_ended_with_named_type_spec
%type<ast> nonglobal_friend_declarator_list
%type<ast> nonglobal_friend_declarator_first
%type<ast> friend_declarator
%type<ast> friend_declarator_list
%type<ast> friend_declarator_list_nonempty
%type<ast> exclusive_or_expression
%type<ast> explicit_instantiation
%type<ast> explicit_specialization
%type<ast> expression
%type<ast> expression_opt
%type<ast> expression_list
%type<ast> expression_statement
%type<ast> for_init_statement
%type<ast> non_functional_declarator
%type<ast> non_functional_declarator_not_started_with_attributes
%type<ast> non_functional_main_declarator
%type<ast> non_functional_main_declarator_not_started_with_attributes
%type<ast> non_functional_direct_declarator
%type<ast> nonglobal_non_functional_declarator_not_started_with_attributes
%type<ast> nonglobal_non_functional_direct_declarator
%type<ast> functional_declarator
%type<ast> functional_main_declarator
%type<ast> functional_main_declarator_not_started_with_attributes
%type<ast> functional_declarator_id
%type<ast> functional_direct_declarator
%type<ast> functional_final_declarator_id
%type<ast> function_body
%type<ast> function_definition_header
%type<ast> function_definition
%type<ast> non_extension_function_definition
%type<ast> function_specifier
%type<ast> handler
%type<ast> handler_seq
%type<ast> id_expression
%type<ast> id_expression_no_destructor
%type<ast> id_expression_of_template_id
%type<ast> id_type_expr
%type<ast> if_statement
%type<ast> inclusive_or_expression
%type<ast> init_declarator
%type<ast> init_declarator_first
%type<ast> init_declarator_list
%type<ast> init_declarator_list_nonempty
%type<ast> initializer
%type<ast> initializer_clause
%type<ast> initializer_list
%type<ast> equal_initializer
%type<ast> parent_initializer
%type<ast> parent_initializer_ctor
%type<ast> equal_or_parent_initializer
%type<ast> iteration_statement
%type<ast> for_range_declaration
%type<ast> jump_statement
%type<ast> label_declaration
%type<ast> label_declarator_seq
%type<ast> labeled_statement
%type<ast> linkage_specification
%type<ast> literal
%type<ast> integer_literal
%type<ast> character_literal
%type<ast> floating_literal
%type<ast> string_literal
%type<ast> boolean_literal
%type<ast> pointer_literal
%type<ast> user_defined_literal
%type<ast> logical_and_expression
%type<ast> logical_or_expression
%type<ast> member_declaration
%type<ast> member_declarator
%type<ast> member_declarator_first
%type<ast> member_declarator_list
%type<ast> member_declarator_list_nonempty
%type<ast> member_specification
%type<ast> member_specification_seq
%type<ast> virt_specifier_seq
// %type<ast> virt_specifier_seq_opt
%type<ast> virt_specifier
%type<ast> gcc_attr_or_virt_specifier_seq_opt
%type<ast> gcc_attr_or_virt_specifier_seq
%type<ast> gcc_attr_or_virt_specifier
%type<ast> mem_initializer
%type<ast> mem_initializer_id
%type<ast> mem_initializer_list
%type<ast> multiplicative_expression
%type<ast> named_namespace_definition
%type<ast> namespace_inline
%type<ast> namespace_inline_opt
%type<ast> namespace_alias_definition
%type<ast> namespace_definition
%type<ast> nested_name_specifier
%type<ast> nested_name_specifier_0
%type<ast> nested_name_specifier_1
%type<ast> new_declarator
%type<ast> new_expression
%type<ast> new_initializer
%type<ast> new_placement
%type<ast> new_type_id
%type<ast> nonabstract_declarator
%type<ast> nonabstract_declarator_not_started_with_attributes
%type<ast> nonabstract_main_declarator
%type<ast> nonabstract_main_declarator_not_started_with_attributes
%type<ast> nonabstract_direct_declarator
%type<ast> nontype_specifier
%type<ast> nontype_specifier_but_SUSL
%type<ast> nontype_specifier_but_SUSL_without_attribute
%type<ast> nontype_specifier_for_named
%type<ast> nontype_specifier_for_named_without_attribute
%type<ast> nontype_specifier_seq
%type<ast> nontype_specifier_seq_with_friend
%type<ast> nontype_specifier_seq_but_SUSL
%type<ast> nontype_specifier_seq_with_SUSL
%type<ast> nontype_specifier_seq_with_SUSL_and_friend
%type<ast> nontype_specifier_seq_for_named
%type<ast> nontype_specifier_seq_for_named_with_friend
%type<ast> nontype_specifier_without_attribute
%type<ast> type_specifier_SUSL
%type<ast> operator
%type<ast> operator_function_id
%type<ast> operator_function_id_name
%type<ast> operator_function_id_name_and_template_args
%type<ast> parameter_declaration
%type<ast> parameters_and_qualifiers
%type<ast> ref_qualifier_opt
%type<ast> ref_qualifier
%type<ast> parameter_declaration_clause
%type<ast> parameter_declaration_clause_nonempty
%type<ast> parameter_declaration_list
%type<ast> pm_expression
%type<ast> postfix_expression
%type<ast> primary_expression
%type<ast> ptr_operator
%type<ast> qualified_id
%type<ast> qualified_id_no_destructor
%type<ast> qualified_id_globals
%type<ast> qualified_id_globals_no_destructor
%type<ast> global_unqualified_id
%type<ast> global_qualified_id_no_template
%type<ast> global_qualified_id_no_template_no_destructor
%type<ast> global_qualified_id_operator
%type<ast> global_qualified_id_template
%type<ast> global_qualified_id_type_no_template
%type<ast> global_qualified_operator_function_id_name_and_template_args
%type<ast> nonglobal_qualified_id_no_template
%type<ast> nonglobal_qualified_id_no_template_no_destructor
%type<ast> nonglobal_qualified_id_template
%type<ast> nonglobal_qualified_id_type_no_template
%type<ast> nonglobal_qualified_operator_function_id_name_and_template_args
%type<ast> qualified_id_type_name
%type<ast> qualified_id_type_expr
%type<ast> unqualified_id_funct
%type<ast> unqualified_id_funct_no_destructor
%type<ast> unqualified_id_type_name
%type<ast> unqualified_name
%type<ast> relational_expression
%type<ast> selection_statement
%type<ast> shift_expression
%type<ast> simple_declaration
%type<ast> non_empty_simple_declaration
%type<ast> block_simple_declaration
%type<ast> explicit_simple_type_specifier
%type<ast> explicit_type_initializer
%type<ast> statement
%type<ast> statement_seq
%type<ast> nondeclarating_statement
%type<ast> storage_class_specifier
%type<ast> template_and_expression
%type<ast> template_argument
%type<ast> template_argument_list
%type<ast> template_assignment_expression
%type<ast> template_conditional_expression
%type<ast> templated_declaration
%type<ast> friend_templated_declaration
%type<ast> template_declaration
%type<ast> template_member_declaration
%type<ast> template_friend_declaration
%type<ast> template_equality_expression
%type<ast> template_exclusive_or_expression
%type<ast> template_expression
%type<ast> template_id
%type<ast> destructor_id
%type<ast> destructor_template_id
%type<ast> template_inclusive_or_expression
%type<ast> template_logical_and_expression
%type<ast> template_logical_or_expression
%type<ast> template_parameter
%type<ast> template_parameter_declaration
%type<ast> template_parameter_list
%type<ast> template_relational_expression
%type<ast> template_shift_expression
%type<ast> throw_expression
%type<ast> translation_unit
%type<ast> try_block
%type<ast> type_id
%type<ast> type_id_not_ellipsis
%type<ast> throw_type_id_list
%type<ast> type_parameter
%type<ast> type_specifier_ended_with_identifier
%type<ast> type_specifier_not_ended_with_identifier
%type<ast> simple_type_specifier_not_ended_with_identifier
%type<ast> named_simple_type_specifier
%type<ast> type_specifier_alone
%type<ast> type_specifier_seq_0
%type<ast> type_specifier_seq_ended_with_identifier
%type<ast> typeof_type_specifier
%type<ast> unary_expression
%type<ast> unnamed_namespace_definition
%type<ast> unqualified_id
%type<ast> unqualified_id_no_destructor
%type<ast> alias_declaration
%type<ast> using_declaration
%type<ast> using_directive
%type<ast> volatile_optional
%type<ast> static_assert_declaration
%type<ast> gnu_type_traits
%type<ast> braced_init_list

%type<ast> exception_specification_opt
%type<ast> identifier_opt
%type<ast> mutable_opt

// These include gcc and C++11
%type<ast> attribute_specifier
// %type<ast> attribute_specifier_opt
%type<ast> attribute_specifier_seq
%type<ast> attribute_specifier_seq_opt
%type<ast> attribute_specifier_seq_and_virt_specifier_seq
%type<ast> attribute_specifier_seq_and_virt_specifier_seq_opt
%type<ast> attributable_statement

// These are C++11 only
%type<ast> std_attribute_specifier
%type<ast> std_attribute_specifier_seq
%type<ast> std_attribute_specifier_seq_opt

%type<ast> alignment_specifier

%type<ast> decltype_specifier

%type<ast> std_attribute_list
%type<ast> std_attribute_item
%type<ast> std_attribute_token
%type<ast> std_attribute_argument_clause
%type<ast> std_attribute_content_balanced_seq
%type<ast> std_attribute_content_balanced

%type<ast> trailing_return_type
%type<ast> trailing_return_type_opt

%type<ast> lambda_capture
%type<ast> lambda_capture_default
%type<ast> lambda_capture_list
%type<ast> lambda_capture_item
%type<ast> lambda_capture_item_simple
%type<ast> lambda_capture_item_init
%type<ast> lambda_declarator
%type<ast> lambda_declarator_opt
%type<ast> lambda_expression
%type<ast> lambda_introducer

// Nonglobal versions of some productions
// their FIRST set should never include '::' (TWO_COLONS)
%type<ast> nonglobal_conversion_declarator
%type<ast> nonglobal_declarator
%type<ast> nonglobal_declarator_not_started_with_attributes
%type<ast> nonglobal_abstract_declarator_not_started_with_attributes
%type<ast> nonglobal_abstract_main_declarator_not_started_with_attributes
%type<ast> nonglobal_main_declarator
%type<ast> nonglobal_main_declarator_not_started_with_attributes
%type<ast> nonglobal_declarator_id
%type<ast> nonglobal_direct_declarator
%type<ast> nonglobal_functional_declarator
%type<ast> nonglobal_functional_main_declarator_not_started_with_attributes
%type<ast> nonglobal_functional_declarator_id
%type<ast> nonglobal_functional_direct_declarator
%type<ast> nonglobal_functional_final_declarator_id
%type<ast> nonglobal_id_expression
%type<ast> nonglobal_id_type_expr
%type<ast> nonglobal_init_declarator_first
%type<ast> nonglobal_init_declarator_list
%type<ast> nonglobal_member_declarator_first
%type<ast> nonglobal_member_declarator_list
%type<ast> nonglobal_new_declarator
%type<ast> nonglobal_nonabstract_declarator
%type<ast> nonglobal_nonabstract_declarator_not_started_with_attributes
%type<ast> nonglobal_nonabstract_main_declarator_not_started_with_attributes
%type<ast> nonglobal_nonabstract_direct_declarator
%type<ast> nonglobal_ptr_operator
%type<ast> nonglobal_abstract_declarator_not_started_with_attributes_not_ellipsis
%type<ast> nonglobal_abstract_main_declarator_not_started_with_attributes_not_ellipsis

%type<node_type> unary_operator
%type<node_type> assignment_operator

%type<ast> gxx_trait_comma_type_id_list_opt
%type<ast> gxx_trait_type_id_list
%type<ast> gxx_trait_type_id_list_not_ellipsis





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

// I don't know the exact reason behind this, but the first rule
// determines the result of the ambiguity handler, so define
// first translation_unit to ensure that the ambiguity handler
// returns 'ast' type.

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

declaration_sequence_opt : /* empty */
{
    $$ = NULL;
}
| declaration_sequence
{
	$$ = $1;
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
| template_declaration
{
	$$ = $1;
}
| explicit_instantiation
{
	$$ = $1;
}
| explicit_specialization
{
	$$ = $1;
}
| linkage_specification
{
	$$ = $1;
}
| namespace_definition
{
	$$ = $1;
}
;

linkage_specification : EXTERN string_literal '{' declaration_sequence '}'
{
	$$ = ASTMake2(AST_LINKAGE_SPEC, $2, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| EXTERN string_literal '{' '}'
{
	$$ = ASTMake2(AST_LINKAGE_SPEC, $2, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| EXTERN string_literal declaration
{
	$$ = ASTMake2(AST_LINKAGE_SPEC_DECL, $2, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

namespace_definition : named_namespace_definition
{
	$$ = $1;
}
| unnamed_namespace_definition
{
	$$ = $1;
}
;

namespace_inline : INLINE
{
    $$ = ASTLeaf(AST_INLINE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

namespace_inline_opt : /* empty */
{
    $$ = NULL;
}
| namespace_inline
{
    $$ = $1;
}
;

// extension_namespace_definition generates exactly the same
// so it is not repeated
named_namespace_definition : namespace_inline_opt NAMESPACE identifier_token gcc_attributes_opt '{' declaration_sequence_opt '}'
{
	AST identif = ASTLeaf(AST_SYMBOL, make_locus(@3.first_filename, @3.first_line, @3.first_column), $3.token_text);

	$$ = ASTMake4(AST_NAMESPACE_DEFINITION, identif, $6, $4, $1, make_locus(@2.first_filename, @2.first_line, @2.first_column), NULL);
}
;

unnamed_namespace_definition : NAMESPACE '{' declaration_sequence_opt '}'
{
	$$ = ASTMake3(AST_NAMESPACE_DEFINITION, NULL, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
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
;

non_empty_block_declaration : block_simple_declaration
{
    $$ = $1;
}
| common_block_declaration
{
    $$ = $1;
}
;

common_block_declaration : asm_definition
{
	$$ = $1;
}
| namespace_alias_definition
{
	$$ = $1;
}
| using_declaration
{
	$$ = $1;
}
| using_directive
{
	$$ = $1;
}
| label_declaration 
{
	$$ = $1;
}
| static_assert_declaration
{
    $$ = $1;
}
| alias_declaration
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

alias_declaration : USING identifier_token attribute_specifier_seq_opt '=' type_id ';'
{
    AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text);

    $$ = ASTMake3(AST_ALIAS_DECLARATION, identifier, $5, $3, ast_get_locus(identifier), NULL);
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
gcc_attributes : gcc_attributes gcc_attribute
{
	$$ = ASTList($1, $2);
}
| gcc_attribute
{
	$$ = ASTListLeaf($1);
}
;

gcc_attributes_opt : gcc_attributes
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

// Assembler definition
// 
// GNU Extensions.
// The first one includes the standard 'asm-definition' but also allows for a
// volatile optional that is exclusive from gcc. The important thing here is
// that AST_ASM_DEFINITION never have additional colons
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
// ( string_literal : asm_operand_list : asm_operand_list )
// ( string_literal : asm_operand_list : )
// ( string_literal : : asm_operand_list )
// ( string_literal :: asm_operand_list ) [C++ only] [1]
// ( string_literal : : )
// ( string_literal :: ) [C++ only] [2]
| ASM volatile_optional '(' string_literal ':' asm_operand_list ':' asm_operand_list ')' ';'
{
	AST asm_parms = ASTMake4(AST_GCC_ASM_DEF_PARMS, 
			$4, $6, $8, NULL, ast_get_locus($4), NULL);
	$$ = ASTMake2(AST_GCC_ASM_DEFINITION, $2, asm_parms, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
// [1] and [2]
| ASM volatile_optional '(' string_literal TWO_COLONS asm_operand_list ')'
{
	AST asm_parms = ASTMake4(AST_GCC_ASM_DEF_PARMS, 
			$4, NULL, NULL, $6, ast_get_locus($4), NULL);
	$$ = ASTMake2(AST_GCC_ASM_DEFINITION, $2, asm_parms, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
// ( string_literal : asm_operand_list : asm_operand_list : asm_operand_list )
| ASM volatile_optional '(' string_literal ':' asm_operand_list ':' asm_operand_list ':' asm_operand_list ')' ';'
{
	AST asm_parms = ASTMake4(AST_GCC_ASM_DEF_PARMS, 
			$4, $6, $8, $10, ast_get_locus($4), NULL);
	$$ = ASTMake2(AST_GCC_ASM_DEFINITION, $2, asm_parms, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
// ( string_literal :: asm_operand_list ) 
// ( string_literal :: ) 
| ASM volatile_optional '(' string_literal TWO_COLONS asm_operand_list ')' ';'
{
	AST asm_parms = ASTMake4(AST_GCC_ASM_DEF_PARMS, 
			$4, NULL, $6, NULL, ast_get_locus($4), NULL);
	$$ = ASTMake2(AST_GCC_ASM_DEFINITION, $2, asm_parms, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
// ( string_literal :: asm_operand_list : asm_operand_list ) 
// ( string_literal :: : asm_operand_list )
// ( string_literal :: asm_operand_list : )
// ( string_literal :: : )
// ( string_literal ::: ) <-- Note that ":::" is lexed as ":: :"
| ASM volatile_optional '(' string_literal TWO_COLONS asm_operand_list ':' asm_operand_list ')' ';'
{
	AST asm_parms = ASTMake4(AST_GCC_ASM_DEF_PARMS, 
			$4, NULL, $6, $8, ast_get_locus($4), NULL);
	$$ = ASTMake2(AST_GCC_ASM_DEFINITION, $2, asm_parms, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
// ( string_literal : asm_operand_list :: asm_operand_list ) 
// ( string_literal : :: asm_operand_list ) 
// ( string_literal : asm_operand_list :: ) 
// ( string_literal : :: )  <-- Note that this is ": ::" not ":::"
| ASM volatile_optional '(' string_literal ':' asm_operand_list TWO_COLONS asm_operand_list ')' ';'
{
	AST asm_parms = ASTMake4(AST_GCC_ASM_DEF_PARMS, 
			$4, $6, NULL, $8, ast_get_locus($4), NULL);
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

// attribute_specifier_opt : /* empty */
// {
//     $$ = NULL;
// }
// | attribute_specifier
// {
//     $$ = $1;
// }
// ;

std_attribute_specifier : STD_ATTRIBUTE_START std_attribute_list STD_ATTRIBUTE_END
{
    $$ = ASTMake1(AST_ATTRIBUTE_SPECIFIER, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| alignment_specifier
{
    $$ = $1;
}
;

alignment_specifier : TOKEN_ALIGNAS '(' type_id ')' %merge<ambiguityHandler>
{
    $$ = ASTMake1(AST_ALIGNAS_TYPE, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TOKEN_ALIGNAS '(' assignment_expression ')' %merge<ambiguityHandler>
{
    $$ = ASTMake1(AST_ALIGNAS, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TOKEN_ALIGNAS '(' assignment_expression ELLIPSIS ')' %merge<ambiguityHandler>
{
    AST pack_expansion = ASTMake1(AST_INITIALIZER_CLAUSE_PACK_EXPANSION, $3, ast_get_locus($3), NULL);
    $$ = ASTMake1(AST_ALIGNAS, pack_expansion, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

std_attribute_list : /* empty */
{
    $$ = NULL;
}
| ','
{
    $$ = NULL;
}
| std_attribute_item
{
    $$ = ASTListLeaf($1);
}
| std_attribute_list ','  std_attribute_item
{
    if ($1 == NULL)
        $$ = $3;
    else
        $$ = ASTList($1, $3);
}
;

std_attribute_item : std_attribute_token std_attribute_argument_clause
{
    $$ = ASTMake2(AST_ATTRIBUTE_ITEM, $1, $2, ast_get_locus($1), NULL);
}
| std_attribute_token
{
    $$ = ASTMake2(AST_ATTRIBUTE_ITEM, $1, NULL, ast_get_locus($1), NULL);
}
;

std_attribute_token : IDENTIFIER
{
    // The lexer will lex both unscoped and scoped names here
    $$ = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

std_attribute_argument_clause : '(' std_attribute_content_balanced_seq ')'
{
    $$ = ASTMake1(AST_ATTRIBUTE_ARG_PARENTHESES, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '(' ')'
{
    $$ = ASTMake1(AST_ATTRIBUTE_ARG_PARENTHESES, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

std_attribute_content_balanced_seq : std_attribute_content_balanced
{
    $$ = ASTListLeaf($1);
}
| std_attribute_content_balanced_seq std_attribute_content_balanced
{
    $$ = ASTList($1, $2);
}
;

std_attribute_content_balanced : '(' std_attribute_content_balanced_seq ')'
{
    $$ = ASTMake1(AST_ATTRIBUTE_ARG_PARENTHESES, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '[' std_attribute_content_balanced_seq ']'
{
    $$ = ASTMake1(AST_ATTRIBUTE_ARG_BRACKET, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '{' std_attribute_content_balanced_seq '}'
{
    $$ = ASTMake1(AST_ATTRIBUTE_ARG_BRACE, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| STD_ATTRIBUTE_TEXT
{
    $$ = ASTLeaf(AST_ATTRIBUTE_ARG_TOKEN, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

attribute_specifier : std_attribute_specifier
{
    $$ = $1;
}
// GNU attributes
| gcc_attribute
{
    $$ = $1;
}
;

attribute_specifier_seq : attribute_specifier
{
    $$ = ASTListLeaf($1);
}
| attribute_specifier_seq attribute_specifier
{
    $$ = ASTList($1, $2);
}
;

std_attribute_specifier_seq : std_attribute_specifier
{
    $$ = ASTListLeaf($1);
}
| std_attribute_specifier_seq std_attribute_specifier
{
    $$ = ASTList($1, $2);
}
;

std_attribute_specifier_seq_opt : /* empty */
{
    $$ = NULL;
}
| std_attribute_specifier_seq
{
    $$ = $1;
}
;

attribute_specifier_seq_opt : /* empty */
{
    $$ = NULL;
}
| attribute_specifier_seq
{
    $$ = $1;
}
;


static_assert_declaration : STATIC_ASSERT '(' constant_expression ',' string_literal ')' ';'
{
    $$ = ASTMake2(AST_STATIC_ASSERT, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| STATIC_ASSERT '(' constant_expression ')' ';'
{
    $$ = ASTMake2(AST_STATIC_ASSERT, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

namespace_alias_definition : NAMESPACE identifier_token '=' id_expression ';'
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text);

	$$ = ASTMake2(AST_NAMESPACE_ALIAS, identifier, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

using_declaration : USING qualified_id ';'
{
	$$ = ASTMake1(AST_USING_DECLARATION, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| USING TYPENAME qualified_id_type_expr ';'
{
	$$ = ASTMake1(AST_USING_DECLARATION_TYPENAME, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

using_directive : USING NAMESPACE id_expression gcc_attributes_opt ';'
{
	$$ = ASTMake2(AST_USING_NAMESPACE_DIRECTIVE, $3, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

simple_declaration : non_empty_simple_declaration
{
    $$ = $1;
}
| ';'
{
    // This is an error but also a common extension
    $$ = ASTLeaf(AST_EMPTY_DECL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| attribute_specifier_seq ';'
{
    $$ = ASTMake1(AST_ATTRIBUTE_DECLARATION, $1, ast_get_locus($1), NULL);
}
;

non_empty_simple_declaration : decl_specifier_seq_0 init_declarator_list ';'  %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, $1, $2, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec nonglobal_init_declarator_list ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, $1, $2, ast_get_locus($1), NULL);
}
| init_declarator_list ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, NULL, $1, ast_get_locus($1), NULL);
}
| decl_specifier_alone_seq ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, $1, NULL, ast_get_locus($1), NULL);
}
;

block_simple_declaration : decl_specifier_seq_1 init_declarator_list ';'  %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, $1, $2, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec nonglobal_init_declarator_list ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, $1, $2, ast_get_locus($1), NULL);
}
| decl_specifier_alone_seq ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, $1, NULL, ast_get_locus($1), NULL);
}
;

// Forces a type
decl_specifier_seq_1: nontype_specifier_seq type_specifier_not_ended_with_identifier nontype_specifier_seq %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
| nontype_specifier_seq_for_named type_specifier_ended_with_identifier nontype_specifier_seq_for_named %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
| nontype_specifier_seq type_specifier_not_ended_with_identifier %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, NULL, ast_get_locus($1), NULL);
}
| type_specifier_not_ended_with_identifier nontype_specifier_seq %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_ended_with_identifier nontype_specifier_seq_for_named %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_not_ended_with_identifier %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, NULL, ast_get_locus($1), NULL);
}
| nontype_specifier_seq_with_SUSL %merge<ambiguityHandler>
{
    $$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1,
        ASTLeaf(AST_INT_TYPE, ast_get_locus($1), NULL),
        NULL, ast_get_locus($1), NULL);
}
;

// Does not end with a named type specifier
decl_specifier_seq_0 : nontype_specifier_seq type_specifier_not_ended_with_identifier nontype_specifier_seq %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
| nontype_specifier_seq_for_named type_specifier_ended_with_identifier nontype_specifier_seq_for_named %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
| nontype_specifier_seq type_specifier_not_ended_with_identifier %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, NULL, ast_get_locus($1), NULL);
}
| type_specifier_not_ended_with_identifier nontype_specifier_seq %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_ended_with_identifier nontype_specifier_seq_for_named %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_not_ended_with_identifier %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, NULL, ast_get_locus($1), NULL);
}
| nontype_specifier_seq_for_named %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, NULL, NULL, ast_get_locus($1), NULL);
}
| nontype_specifier_seq_with_SUSL %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, NULL, NULL, ast_get_locus($1), NULL);
}
;

// This one ends with a named type specifier
decl_specifier_seq_ended_with_named_type_spec : nontype_specifier_seq_for_named type_specifier_ended_with_identifier
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, NULL, ast_get_locus($1), NULL);
}
| type_specifier_ended_with_identifier
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, NULL, ast_get_locus($1), NULL);
}
;

// This does not declare an object, maybe just types, so restrict it a bit
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

friend_declaration : decl_specifier_seq_friend friend_declarator_list ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_MEMBER_DECLARATION, $1, $2, ast_get_locus($1), NULL);
}
| decl_specifier_seq_friend_ended_with_named_type_spec nonglobal_friend_declarator_list ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_MEMBER_DECLARATION, $1, $2, ast_get_locus($1), NULL);
}
| decl_specifier_seq_friend_alone ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_MEMBER_DECLARATION, $1, NULL, ast_get_locus($1), NULL);
}
| friend_function_definition
{
    $$ = $1;
}
;

friend_function_definition : friend_function_definition_header function_body
{
    $$ = ASTMake3(AST_FUNCTION_DEFINITION, $1, NULL, $2, ast_get_locus($1), NULL);
}
;

friend_function_definition_header : decl_specifier_seq_friend functional_declarator %merge<ambiguityHandler>
{
    $$ = ASTMake3(AST_FUNCTION_DEFINITION_HEADER, $1, $2, NULL, ast_get_locus($1), NULL);
}
| decl_specifier_seq_friend_ended_with_named_type_spec nonglobal_functional_declarator %merge<ambiguityHandler>
{
    $$ = ASTMake3(AST_FUNCTION_DEFINITION_HEADER, $1, $2, NULL, ast_get_locus($1), NULL);
}
// Note that in GCC C++ (but not in GCC C) there can be attributes before the function-body
| decl_specifier_seq_friend functional_declarator gcc_attributes %merge<ambiguityHandler>
{
    $$ = ASTMake3(AST_FUNCTION_DEFINITION_HEADER, $1, $2, $3, ast_get_locus($1), NULL);
}
| decl_specifier_seq_friend_ended_with_named_type_spec nonglobal_functional_declarator gcc_attributes %merge<ambiguityHandler>
{
    $$ = ASTMake3(AST_FUNCTION_DEFINITION_HEADER, $1, $2, $3, ast_get_locus($1), NULL);
}
;

decl_specifier_seq_friend_ended_with_named_type_spec : nontype_specifier_seq_for_named_with_friend type_specifier_ended_with_identifier
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, NULL, ast_get_locus($1), NULL);
}
;

decl_specifier_seq_friend_alone : nontype_specifier_seq_with_friend friend_type_specifier_alone nontype_specifier_seq
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
| nontype_specifier_seq friend_type_specifier_alone nontype_specifier_seq_with_friend
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
| nontype_specifier_seq_with_friend friend_type_specifier_alone
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, NULL, ast_get_locus($1), NULL);
}
| friend_type_specifier_alone nontype_specifier_seq_with_friend
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, $2, ast_get_locus($1), NULL);
}
;

nontype_specifier_seq_with_friend : nontype_specifier_seq friend_specifier nontype_specifier_seq
{
    $$ = ast_list_concat(ASTList($1, $2), $3);
}
| nontype_specifier_seq friend_specifier
{
    $$ = ASTList($1, $2);
}
| friend_specifier nontype_specifier_seq
{
    $$ = ast_list_concat(ASTListLeaf($1), $2);
}
| friend_specifier
{
    $$ = ASTListLeaf($1);
}
;

friend_type_specifier_alone : type_specifier_ended_with_identifier
{
    $$ = $1;
}
| type_specifier_not_ended_with_identifier
{
    $$ = $1;
}
;

decl_specifier_seq_friend : nontype_specifier_seq_with_friend type_specifier_not_ended_with_identifier nontype_specifier_seq %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
| nontype_specifier_seq type_specifier_not_ended_with_identifier nontype_specifier_seq_with_friend %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
// --
| nontype_specifier_seq_for_named_with_friend type_specifier_ended_with_identifier nontype_specifier_seq_for_named %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
| nontype_specifier_seq_for_named type_specifier_ended_with_identifier nontype_specifier_seq_for_named_with_friend %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
// --
| nontype_specifier_seq_with_friend type_specifier_not_ended_with_identifier %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, NULL, ast_get_locus($1), NULL);
}
| type_specifier_not_ended_with_identifier nontype_specifier_seq_with_friend %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_ended_with_identifier nontype_specifier_seq_for_named_with_friend %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, $2, ast_get_locus($1), NULL);
}
| nontype_specifier_seq_for_named_with_friend %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, NULL, NULL, ast_get_locus($1), NULL);
}
| nontype_specifier_seq_with_SUSL_and_friend %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, NULL, NULL, ast_get_locus($1), NULL);
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

nontype_specifier_seq_but_SUSL : nontype_specifier_but_SUSL
{
	$$ = ASTListLeaf($1);
}
| nontype_specifier_seq_but_SUSL nontype_specifier_but_SUSL
{
	$$ = ASTList($1, $2);
}
;

nontype_specifier_seq_for_named : nontype_specifier_for_named
{
	$$ = ASTListLeaf($1);
}
| nontype_specifier_seq_for_named nontype_specifier_for_named
{
	$$ = ASTList($1, $2);
}
;

nontype_specifier_seq_for_named_with_friend : nontype_specifier_seq_for_named friend_specifier nontype_specifier_seq_for_named
{
	$$ = ast_list_concat(ASTList($1, $2), $3);
}
| nontype_specifier_seq_for_named friend_specifier
{
	$$ = ASTList($1, $2);
}
| friend_specifier nontype_specifier_seq_for_named
{
	$$ = ast_list_concat(ASTListLeaf($1), $2);
}
| friend_specifier
{
    $$ = ASTListLeaf($1);
}
;

nontype_specifier : nontype_specifier_without_attribute
{
    $$ = $1;
}
| std_attribute_specifier
{
    $$ = $1;
}
// GNU extension
| gcc_attribute
{
	$$ = $1;
}
;

friend_specifier : FRIEND
{
	$$ = ASTLeaf(AST_FRIEND_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

nontype_specifier_but_SUSL_without_attribute : storage_class_specifier
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
| cv_qualifier
{
	$$ = $1;
}
| TOKEN_CONSTEXPR
{
	$$ = ASTLeaf(AST_CONSTEXPR_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
// GNU Extension
| COMPLEX
{
	$$ = ASTLeaf(AST_GCC_COMPLEX_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

nontype_specifier_but_SUSL : nontype_specifier_but_SUSL_without_attribute
{
	$$ = $1;
}
| std_attribute_specifier
{
    $$ = $1;
}
// GNU extension
| gcc_attribute
{
	$$ = $1;
}
;

nontype_specifier_for_named_without_attribute : nontype_specifier_but_SUSL_without_attribute
{
    $$ = $1;
}
;

type_specifier_SUSL : TOKEN_SIGNED
{
	$$ = ASTLeaf(AST_SIGNED_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_UNSIGNED
{
	$$ = ASTLeaf(AST_UNSIGNED_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_SHORT
{
	$$ = ASTLeaf(AST_SHORT_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_LONG
{
	$$ = ASTLeaf(AST_LONG_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

nontype_specifier_for_named : nontype_specifier_for_named_without_attribute
{
    $$ = $1;
}
| std_attribute_specifier
{
    $$ = $1;
}
// GNU extension
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
| cv_qualifier
{
	$$ = $1;
}
| TOKEN_CONSTEXPR
{
	$$ = ASTLeaf(AST_CONSTEXPR_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| type_specifier_SUSL
{
    $$ = $1;
}
// GNU Extension
| COMPLEX
{
	$$ = ASTLeaf(AST_GCC_COMPLEX_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
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
| MUTABLE
{
	$$ = ASTLeaf(AST_MUTABLE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_THREAD_LOCAL
{
	$$ = ASTLeaf(AST_THREAD_LOCAL_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
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
| VIRTUAL
{
	$$ = ASTLeaf(AST_VIRTUAL_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| EXPLICIT
{
	$$ = ASTLeaf(AST_EXPLICIT_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

type_specifier_not_ended_with_identifier : class_specifier
{
    $$ = $1;
}
| enum_specifier
{
    $$ = $1;
}
| simple_type_specifier_not_ended_with_identifier
{
    $$ = $1;
}
;

type_specifier_ended_with_identifier : named_simple_type_specifier
{
    $$ = $1;
}
| elaborated_type_specifier
{
    $$ = $1;
}
;

type_specifier_alone : builtin_types
{
	$$ = $1;
}
| class_specifier
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
| typeof_type_specifier
{
    $$ = $1;
}
;

/*
type_specifier_seq has been rewritten in a similar fashion to "decl_specifier_seq"
*/

type_specifier_seq_0 : nontype_specifier_seq type_specifier_not_ended_with_identifier nontype_specifier_seq %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
| nontype_specifier_seq_for_named type_specifier_ended_with_identifier nontype_specifier_seq_for_named %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, $3, ast_get_locus($1), NULL);
}
| nontype_specifier_seq type_specifier_not_ended_with_identifier %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, NULL, ast_get_locus($1), NULL);
}
| type_specifier_not_ended_with_identifier nontype_specifier_seq %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_ended_with_identifier nontype_specifier_seq_for_named %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_not_ended_with_identifier %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, NULL, ast_get_locus($1), NULL);
}
| nontype_specifier_seq_with_SUSL %merge<ambiguityHandler>
{
    $$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, 
        ASTLeaf(AST_INT_TYPE, ast_get_locus($1), NULL), 
        NULL, ast_get_locus($1), NULL);
}
;

nontype_specifier_seq_with_SUSL : nontype_specifier_seq type_specifier_SUSL nontype_specifier_seq_but_SUSL
{
    $$ = ast_list_concat($1, ast_list_concat(ASTListLeaf($2), $3));
}
| type_specifier_SUSL nontype_specifier_seq_but_SUSL
{
    $$ = ast_list_concat(ASTListLeaf($1), $2);
}
| nontype_specifier_seq type_specifier_SUSL
{
    $$ = ast_list_concat($1, ASTListLeaf($2));
}
| type_specifier_SUSL
{
    $$ = ASTListLeaf($1);
}
;

nontype_specifier_seq_with_SUSL_and_friend : nontype_specifier_seq_with_SUSL friend_specifier nontype_specifier_seq_with_SUSL
{
    $$ = ast_list_concat(ASTList($1, $2), $3);
}
| nontype_specifier_seq_with_SUSL friend_specifier
{
    $$ = ASTList($1, $2);
}
| friend_specifier nontype_specifier_seq_with_SUSL
{
    $$ = ast_list_concat(ASTListLeaf($1), $2);
}
;

type_specifier_seq_ended_with_identifier : nontype_specifier_seq_for_named type_specifier_ended_with_identifier %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, $1, $2, NULL, ast_get_locus($1), NULL);
}
| type_specifier_ended_with_identifier %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, NULL, ast_get_locus($1), NULL);
}
;

named_simple_type_specifier : id_type_expr
{
	$$ = ASTMake1(AST_SIMPLE_TYPE_SPEC, $1, ast_get_locus($1), NULL);
}
;

explicit_simple_type_specifier : named_simple_type_specifier
{
    $$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, NULL, ast_get_locus($1), NULL);
}
|  simple_type_specifier_not_ended_with_identifier 
{
    $$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, NULL, $1, NULL, ast_get_locus($1), NULL);
}
| type_specifier_SUSL
{
    $$ = ASTMake3(AST_TYPE_SPECIFIER_SEQ, 
            NULL, $1, NULL,
            ast_get_locus($1), NULL);
}
;

decltype_specifier : DECLTYPE '(' expression ')'
{
    $$ = ASTMake1(AST_DECLTYPE, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
// c++14
| DECLTYPE '(' TOKEN_AUTO_TYPE ')'
{
    $$ = ASTLeaf(AST_DECLTYPE_AUTO, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

simple_type_specifier_not_ended_with_identifier : builtin_types
{
    $$ = $1;
}
| decltype_specifier
{
    $$ = $1;
}
| typeof_type_specifier
{
    $$ = $1;
}
| GXX_UNDERLYING_TYPE '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_UNDERLYING_TYPE, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

// GNU Extension (difficult to ensure that this does not end with an identifier..., use parentheses)
typeof_type_specifier : TYPEOF unary_expression %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_GCC_TYPEOF_EXPR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TYPEOF '(' type_id ')' %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_GCC_TYPEOF, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

builtin_types : TOKEN_CHAR
{
	$$ = ASTLeaf(AST_CHAR_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_WCHAR_T
{
	$$ = ASTLeaf(AST_WCHAR_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
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
| TOKEN_CHAR16_T
{
	$$ = ASTLeaf(AST_CHAR16_T, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_CHAR32_T
{
	$$ = ASTLeaf(AST_CHAR32_T, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_AUTO_TYPE
{
	$$ = ASTLeaf(AST_AUTO_TYPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

elaborated_type_specifier : class_key attribute_specifier_seq_opt id_type_expr
{
	$$ = ASTMake3(AST_ELABORATED_TYPE_CLASS_SPEC, $1, $3, $2, ast_get_locus($1), NULL);
}
| enum_key attribute_specifier_seq_opt id_type_expr enum_base_opt
{
	$$ = ASTMake4(AST_ELABORATED_TYPE_ENUM_SPEC, $3, $2, $4, $1, ast_get_locus($1), NULL);
}
| TYPENAME qualified_id_type_expr
{
	$$ = ASTMake1(AST_ELABORATED_TYPENAME_SPEC, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
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

nonglobal_init_declarator_list : nonglobal_init_declarator_first
{
    $$ = ASTListLeaf($1);
}
| nonglobal_init_declarator_first ',' init_declarator_list
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

init_declarator : declarator gcc_extra_bits_init_declarator_opt %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_INIT_DECLARATOR, $1, NULL, $2, ast_get_locus($1), NULL);
}
| declarator gcc_extra_bits_init_declarator_opt equal_or_parent_initializer %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_INIT_DECLARATOR, $1, $3, $2, ast_get_locus($1), NULL);
}
| non_functional_declarator gcc_extra_bits_init_declarator_opt braced_init_list %merge<ambiguityHandler>
{
    $$ = ASTMake3(AST_INIT_DECLARATOR, $1, $3, $2, ast_get_locus($1), NULL);
}
;

init_declarator_first : declarator_not_started_with_attributes gcc_extra_bits_init_declarator_opt %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_INIT_DECLARATOR, $1, NULL, $2, ast_get_locus($1), NULL);
}
| declarator_not_started_with_attributes gcc_extra_bits_init_declarator_opt equal_or_parent_initializer %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_INIT_DECLARATOR, $1, $3, $2, ast_get_locus($1), NULL);
}
| non_functional_declarator_not_started_with_attributes gcc_extra_bits_init_declarator_opt braced_init_list %merge<ambiguityHandler>
{
    $$ = ASTMake3(AST_INIT_DECLARATOR, $1, $3, $2, ast_get_locus($1), NULL);
}
;

nonglobal_init_declarator_first : nonglobal_declarator_not_started_with_attributes gcc_extra_bits_init_declarator_opt %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_INIT_DECLARATOR, $1, NULL, $2, ast_get_locus($1), NULL);
}
| nonglobal_declarator_not_started_with_attributes gcc_extra_bits_init_declarator_opt equal_or_parent_initializer %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_INIT_DECLARATOR, $1, $3, $2, ast_get_locus($1), NULL);
}
| nonglobal_non_functional_declarator_not_started_with_attributes gcc_extra_bits_init_declarator_opt braced_init_list %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_INIT_DECLARATOR, $1, $3, $2, ast_get_locus($1), NULL);
}
;

/* GNU Extension */
asm_specification : ASM '(' string_literal ')'
{
	$$ = ASTMake1(AST_GCC_ASM_SPEC, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;
/* End of GNU Extension */

nonglobal_declarator : nonglobal_main_declarator
{
    $$ = $1;
}
| nonglobal_direct_declarator parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, $1, $2, $3, ast_get_locus($1), NULL);
}
;

nonglobal_declarator_not_started_with_attributes : nonglobal_main_declarator_not_started_with_attributes
{
    $$ = $1;
}
| nonglobal_direct_declarator parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, $1, $2, $3, ast_get_locus($1), NULL);
}
;

nonglobal_main_declarator : nonglobal_direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
| nonglobal_ptr_operator main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
| attribute_specifier_seq direct_declarator
{
    $$ = ASTMake2(AST_DECLARATOR, $2, $1, ast_get_locus($1), NULL);
}
| attribute_specifier_seq ptr_operator main_declarator
{
    $$ = ASTMake3(AST_POINTER_DECLARATOR, $2, $3, $1, ast_get_locus($1), NULL);
}
;

nonglobal_main_declarator_not_started_with_attributes : nonglobal_direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
| nonglobal_ptr_operator main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
;

nonglobal_non_functional_declarator_not_started_with_attributes : nonglobal_non_functional_direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
| nonglobal_ptr_operator non_functional_main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
;

nonglobal_non_functional_direct_declarator : nonglobal_declarator_id
{
	$$ = $1;
}
| nonglobal_non_functional_direct_declarator '[' constant_expression ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, $1, $3, ast_get_locus($1), NULL);
}
| nonglobal_non_functional_direct_declarator '[' ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, $1, NULL, ast_get_locus($1), NULL);
}
| '(' non_functional_main_declarator ')'
{
	$$ = ASTMake1(AST_PARENTHESIZED_DECLARATOR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

declarator_not_started_with_attributes : main_declarator_not_started_with_attributes
{
    $$ = $1;
}
| direct_declarator parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, $1, $2, $3, ast_get_locus($1), NULL);
}
;

declarator : main_declarator
{
    $$ = $1;
}
| direct_declarator parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, $1, $2, $3, ast_get_locus($1), NULL);
}
;

main_declarator_not_started_with_attributes : direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
| ptr_operator main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
;

main_declarator : direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
| ptr_operator main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
| attribute_specifier_seq direct_declarator
{
    $$ = ASTMake2(AST_DECLARATOR, $2, $1, ast_get_locus($1), NULL);
}
| attribute_specifier_seq ptr_operator main_declarator
{
    $$ = ASTMake3(AST_POINTER_DECLARATOR, $2, $3, $1, ast_get_locus($1), NULL);
}
;

nonglobal_ptr_operator : '*'
{
	$$ = ASTMake2(AST_POINTER_SPEC, NULL, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '*' cv_qualifier_seq
{
	$$ = ASTMake2(AST_POINTER_SPEC, NULL, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| nonglobal_id_type_expr TWO_COLONS_AND_STAR
{
	$$ = ASTMake2(AST_POINTER_SPEC, $1, NULL, ast_get_locus($1), NULL);
}
| nonglobal_id_type_expr TWO_COLONS_AND_STAR cv_qualifier_seq
{
	$$ = ASTMake2(AST_POINTER_SPEC, $1, $3, ast_get_locus($1), NULL);
}
| '&'
{
	$$ = ASTLeaf(AST_REFERENCE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
// C++11
| ANDAND
{
    $$ = ASTLeaf(AST_RVALUE_REFERENCE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
// Mercurium extension
| MCC_REBINDABLE_REFERENCE
{
	$$ = ASTLeaf(AST_REBINDABLE_REFERENCE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
// GNU Extensions
| '&' cv_qualifier_seq
{
	$$ = ASTMake1(AST_GCC_REFERENCE_SPEC, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
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
| id_type_expr TWO_COLONS_AND_STAR
{
	$$ = ASTMake2(AST_POINTER_SPEC, $1, NULL, ast_get_locus($1), NULL);
}
| id_type_expr TWO_COLONS_AND_STAR cv_qualifier_seq
{
	$$ = ASTMake2(AST_POINTER_SPEC, $1, $3, ast_get_locus($1), NULL);
}
| '&'
{
	$$ = ASTLeaf(AST_REFERENCE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
// C++11
| ANDAND
{
    $$ = ASTLeaf(AST_RVALUE_REFERENCE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
// Mercurium extension
| MCC_REBINDABLE_REFERENCE
{
	$$ = ASTLeaf(AST_REBINDABLE_REFERENCE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
// GNU Extensions
| '&' cv_qualifier_seq
{
	$$ = ASTMake1(AST_GCC_REFERENCE_SPEC, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

/* 
   A nonfunctional declarator will never generate a function declarator
 */

non_functional_declarator : non_functional_main_declarator
{
    $$ = $1;
}
| non_functional_direct_declarator parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, $1, $2, $3, ast_get_locus($1), NULL);
}
;

non_functional_declarator_not_started_with_attributes : non_functional_main_declarator_not_started_with_attributes
{
    $$ = $1;
}
| non_functional_direct_declarator parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, $1, $2, $3, ast_get_locus($1), NULL);
}
;

non_functional_main_declarator_not_started_with_attributes : non_functional_direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
| ptr_operator non_functional_main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
;

non_functional_main_declarator : non_functional_direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
| ptr_operator non_functional_main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
| attribute_specifier_seq non_functional_direct_declarator
{
	$$ = ASTMake2(AST_DECLARATOR, $2, $1, ast_get_locus($1), NULL);
}
| attribute_specifier_seq ptr_operator non_functional_main_declarator
{
	$$ = ASTMake3(AST_POINTER_DECLARATOR, $2, $3, $1, ast_get_locus($1), NULL);
}
;

non_functional_direct_declarator : declarator_id
{
	$$ = $1;
}
| non_functional_direct_declarator '[' constant_expression ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, $1, $3, ast_get_locus($1), NULL);
}
| non_functional_direct_declarator '[' ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, $1, NULL, ast_get_locus($1), NULL);
}
| '(' non_functional_main_declarator ')'
{
	$$ = ASTMake1(AST_PARENTHESIZED_DECLARATOR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

/*
   A functional declarator is a syntactic enforced declarator that will have
   a functional nature
 */

functional_declarator : functional_main_declarator_not_started_with_attributes
{
    $$ = $1;
}
| functional_final_declarator_id parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, $1, $2, $3, ast_get_locus($1), NULL);
}
;

functional_main_declarator : functional_direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
| ptr_operator functional_main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
// GNU extension
| attribute_specifier_seq functional_direct_declarator
{
	$$ = ASTMake2(AST_DECLARATOR, $2, $1, ast_get_locus($1), NULL);
}
| attribute_specifier_seq ptr_operator functional_main_declarator
{
	$$ = ASTMake3(AST_POINTER_DECLARATOR, $2, $3, $1, ast_get_locus($1), NULL);
}
;

functional_main_declarator_not_started_with_attributes : functional_direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
| ptr_operator functional_main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
;

// I think they could not cram more stuff here, but let's wait for C++2014
parameters_and_qualifiers : '(' parameter_declaration_clause ')'
                                          std_attribute_specifier_seq_opt
                                          cv_qualifier_seq_opt
                                          ref_qualifier_opt
                                          exception_specification_opt
{
    $$ = ASTMake4(AST_PARAMETERS_AND_QUALIFIERS,
            $2,
            ASTMake4(AST_PARAMETERS_AND_QUALIFIERS_EXTRA, $4, $5, $6, $7,
                    make_locus(@1.first_filename, @1.first_line, @1.first_column),
                    NULL),
            NULL, NULL,
            make_locus(@1.first_filename, @1.first_line, @1.first_column),
            NULL);
}
;

ref_qualifier_opt : /* empty */
{
    $$ = NULL;
}
| ref_qualifier
{
    $$ = $1;
}
;

ref_qualifier : '&'
{
	$$ = ASTLeaf(AST_REFERENCE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
// C++11
| ANDAND
{
    $$ = ASTLeaf(AST_RVALUE_REFERENCE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
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
| functional_direct_declarator '[' constant_expression_opt ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, $1, $3, ast_get_locus($1), NULL);
}
| '(' functional_main_declarator ')'
{
	$$ = ASTMake1(AST_PARENTHESIZED_DECLARATOR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

functional_declarator_id : functional_final_declarator_id parameters_and_qualifiers
{
	$$ = ASTMake2(AST_DECLARATOR_FUNC, $1, $2, ast_get_locus($1), NULL);
}
;

// This rule is needed because of redundant parentheses
functional_final_declarator_id : declarator_id
{
    $$ = $1;
}
| '(' functional_final_declarator_id ')'
{
    $$ = ASTMake1(AST_PARENTHESIZED_DECLARATOR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

nonglobal_functional_declarator : nonglobal_functional_main_declarator_not_started_with_attributes
{
    $$ = $1;
}
| nonglobal_functional_direct_declarator parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, $1, $2, $3, ast_get_locus($1), NULL);
}
;

nonglobal_functional_main_declarator_not_started_with_attributes : nonglobal_functional_direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
| nonglobal_ptr_operator functional_main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
;

nonglobal_functional_direct_declarator : nonglobal_functional_declarator_id
{
	$$ = $1;
}
| nonglobal_functional_direct_declarator parameters_and_qualifiers
{
	$$ = ASTMake2(AST_DECLARATOR_FUNC, $1, $2, ast_get_locus($1), NULL);
}
| nonglobal_functional_direct_declarator '[' constant_expression_opt ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, $1, $3, ast_get_locus($1), NULL);
}
| '(' functional_main_declarator ')'
{
	$$ = ASTMake1(AST_PARENTHESIZED_DECLARATOR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

nonglobal_functional_declarator_id : nonglobal_functional_final_declarator_id parameters_and_qualifiers
{
	$$ = ASTMake2(AST_DECLARATOR_FUNC, $1, $2, ast_get_locus($1), NULL);
}
;

// This rule is needed because of redundant parentheses
nonglobal_functional_final_declarator_id : nonglobal_declarator_id
{
    $$ = $1;
}
| '(' functional_final_declarator_id ')'
{
    $$ = ASTMake1(AST_PARENTHESIZED_DECLARATOR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

trailing_return_type : PTR_OP type_id_not_ellipsis
{
    $$ = ASTMake1(AST_TRAILING_RETURN, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

trailing_return_type_opt : /* empty */
{
    $$ = NULL;
}
| trailing_return_type
{
    $$ = $1;
}
;

cv_qualifier_seq_opt : /* empty */
{
    $$ = NULL;
}
| cv_qualifier_seq
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
;

nonglobal_direct_declarator : nonglobal_declarator_id
{
	$$ = $1;
}
| nonglobal_direct_declarator parameters_and_qualifiers
{
	$$ = ASTMake2(AST_DECLARATOR_FUNC, $1, $2, ast_get_locus($1), NULL);
}
| nonglobal_direct_declarator '[' constant_expression ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, $1, $3, ast_get_locus($1), NULL);
}
| nonglobal_direct_declarator '[' ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, $1, NULL, ast_get_locus($1), NULL);
}
| '(' main_declarator ')'
{
	$$ = ASTMake1(AST_PARENTHESIZED_DECLARATOR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

nonglobal_declarator_id : nonglobal_id_expression std_attribute_specifier_seq_opt
{
	$$ = ASTMake2(AST_DECLARATOR_ID_EXPR, $1, $2, ast_get_locus($1), NULL);
}
| ELLIPSIS id_expression std_attribute_specifier_seq_opt
{
	AST declarator_id_expr = ASTMake2(AST_DECLARATOR_ID_EXPR, $2, $3, ast_get_locus($2), NULL);
	$$ = ASTMake1(AST_DECLARATOR_ID_PACK,
            declarator_id_expr,
            ast_get_locus(declarator_id_expr),
            NULL);
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
| direct_declarator '[' constant_expression ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, $1, $3, ast_get_locus($1), NULL);
}
| direct_declarator '[' ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, $1, NULL, ast_get_locus($1), NULL);
}
| '(' main_declarator ')'
{
	$$ = ASTMake1(AST_PARENTHESIZED_DECLARATOR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

declarator_id : id_expression std_attribute_specifier_seq_opt
{
	$$ = ASTMake2(AST_DECLARATOR_ID_EXPR, $1, $2, ast_get_locus($1), NULL);
}
| ELLIPSIS id_expression std_attribute_specifier_seq_opt
{
	AST declarator_id_expr = ASTMake2(AST_DECLARATOR_ID_EXPR, $2, $3, ast_get_locus($2), NULL);
	$$ = ASTMake1(AST_DECLARATOR_ID_PACK,
            declarator_id_expr,
            ast_get_locus(declarator_id_expr),
            NULL);
}
;

enum_head : enum_key attribute_specifier_seq_opt identifier_opt enum_base_opt
{
    $$ = ASTMake4(AST_ENUM_HEAD, $1, $2, $3, $4, ast_get_locus($1), NULL);
}
| enum_key attribute_specifier_seq_opt nested_name_specifier identifier_token enum_base_opt
{
	AST name = ASTMake3(AST_QUALIFIED_ID, NULL, $3, 
                ASTLeaf(AST_SYMBOL, make_locus(@4.first_filename, @4.first_line, @4.first_column), $4.token_text),
                ast_get_locus($3), NULL);

    $$ = ASTMake4(AST_ENUM_HEAD, $1, $2, name, $5, ast_get_locus($1), NULL);
}
;

enum_key : ENUM
{
    $$ = ASTLeaf(AST_UNSCOPED_ENUM_KEY, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| ENUM STRUCT
{
    $$ = ASTLeaf(AST_SCOPED_ENUM_KEY, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| ENUM CLASS
{
    $$ = ASTLeaf(AST_SCOPED_ENUM_KEY, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

enum_base_opt : /* empty */
{
    $$ = NULL;
}
| ':' enum_base_type_specifier_seq
{
    $$ = $2;
}
;

enum_base_type_specifier_seq : type_specifier_seq_0
{
    $$ = ASTMake2(AST_TYPE_ID, $1, NULL, ast_get_locus($1), NULL);
}
| type_specifier_seq_ended_with_identifier
{
    $$ = ASTMake2(AST_TYPE_ID, $1, NULL, ast_get_locus($1), NULL);
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

enum_specifier : enum_head '{' enumeration_list_opt '}'
{
	$$ = ASTMake2(AST_ENUM_SPECIFIER, $1, $3, ast_get_locus($1), NULL);
}
| enum_head '{' enumeration_list_opt ',' '}'
{
	$$ = ASTMake2(AST_ENUM_SPECIFIER, $1, $3, ast_get_locus($1), NULL);
}
;

enumeration_list_opt : /* empty */
{
    $$ = NULL;
}
| enumeration_list
{
    $$ = $1;
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

type_id_not_ellipsis : type_specifier_seq_0
{
	$$ = ASTMake2(AST_TYPE_ID, $1, NULL, ast_get_locus($1), NULL);
}
| type_specifier_seq_ended_with_identifier
{
	$$ = ASTMake2(AST_TYPE_ID, $1, NULL, ast_get_locus($1), NULL);
}
| type_specifier_seq_0 abstract_declarator_not_ellipsis
{
	$$ = ASTMake2(AST_TYPE_ID, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_seq_ended_with_identifier nonglobal_abstract_declarator_not_started_with_attributes_not_ellipsis
{
	$$ = ASTMake2(AST_TYPE_ID, $1, $2, ast_get_locus($1), NULL);
}
;

type_id : type_specifier_seq_0
{
	$$ = ASTMake2(AST_TYPE_ID, $1, NULL, ast_get_locus($1), NULL);
}
| type_specifier_seq_ended_with_identifier
{
	$$ = ASTMake2(AST_TYPE_ID, $1, NULL, ast_get_locus($1), NULL);
}
| type_specifier_seq_0 abstract_declarator_not_started_with_attributes
{
	$$ = ASTMake2(AST_TYPE_ID, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_seq_ended_with_identifier nonglobal_abstract_declarator_not_started_with_attributes
{
	$$ = ASTMake2(AST_TYPE_ID, $1, $2, ast_get_locus($1), NULL);
}
;

abstract_declarator : abstract_main_declarator
{
    $$ = $1;
}
| abstract_direct_declarator parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, $1, $2, $3, ast_get_locus($1), NULL);
}
| parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, NULL, $1, $2, ast_get_locus($1), NULL);
}
;

nonglobal_abstract_declarator_not_started_with_attributes : nonglobal_abstract_main_declarator_not_started_with_attributes
{
    $$ = $1;
}
| abstract_direct_declarator parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, $1, $2, $3, ast_get_locus($1), NULL);
}
| parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, NULL, $1, $2, ast_get_locus($1), NULL);
}
;

nonglobal_abstract_declarator_not_started_with_attributes_not_ellipsis : nonglobal_abstract_main_declarator_not_started_with_attributes_not_ellipsis
{
    $$ = $1;
}
| abstract_direct_declarator_not_ellipsis parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, $1, $2, $3, ast_get_locus($1), NULL);
}
| parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, NULL, $1, $2, ast_get_locus($1), NULL);
}
;

abstract_declarator_not_started_with_attributes : abstract_main_declarator_not_started_with_attributes
{
    $$ = $1;
}
| abstract_direct_declarator parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, $1, $2, $3, ast_get_locus($1), NULL);
}
| parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, NULL, $1, $2, ast_get_locus($1), NULL);
}
;

abstract_main_declarator_not_started_with_attributes : ptr_operator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, NULL, ast_get_locus($1), NULL);
}
| ptr_operator abstract_main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
| abstract_direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
;

abstract_main_declarator : ptr_operator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, NULL, ast_get_locus($1), NULL);
}
| ptr_operator abstract_main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
| abstract_direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
| attribute_specifier_seq ptr_operator
{
	$$ = ASTMake3(AST_POINTER_DECLARATOR, $1, $2, NULL, ast_get_locus($1), NULL);
}
| attribute_specifier_seq ptr_operator abstract_main_declarator
{
	$$ = ASTMake3(AST_POINTER_DECLARATOR, $1, $2, $3, ast_get_locus($1), NULL);
}
| attribute_specifier_seq abstract_direct_declarator
{
	$$ = ASTMake2(AST_DECLARATOR, $2, $1, ast_get_locus($1), NULL);
}
;

nonglobal_abstract_main_declarator_not_started_with_attributes : nonglobal_ptr_operator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, NULL, ast_get_locus($1), NULL);
}
| nonglobal_ptr_operator abstract_main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
| abstract_direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
;

nonglobal_abstract_main_declarator_not_started_with_attributes_not_ellipsis : nonglobal_ptr_operator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, NULL, ast_get_locus($1), NULL);
}
| nonglobal_ptr_operator abstract_main_declarator_not_ellipsis
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
| abstract_direct_declarator_not_ellipsis
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
;

abstract_declarator_not_ellipsis : abstract_main_declarator_not_ellipsis
{
    $$ = $1;
}
| abstract_direct_declarator_not_ellipsis parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, $1, $2, $3, ast_get_locus($1), NULL);
}
| parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, NULL, $1, $2, ast_get_locus($1), NULL);
}
;

abstract_main_declarator_not_ellipsis : ptr_operator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, NULL, ast_get_locus($1), NULL);
}
| ptr_operator abstract_main_declarator_not_ellipsis
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
| abstract_direct_declarator_not_ellipsis
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
| attribute_specifier_seq ptr_operator
{
	$$ = ASTMake3(AST_POINTER_DECLARATOR, $1, $2, NULL, ast_get_locus($1), NULL);
}
| attribute_specifier_seq ptr_operator abstract_main_declarator_not_ellipsis
{
	$$ = ASTMake3(AST_POINTER_DECLARATOR, $1, $2, $3, ast_get_locus($1), NULL);
}
| attribute_specifier_seq abstract_direct_declarator_not_ellipsis
{
	$$ = ASTMake2(AST_DECLARATOR, $2, $1, ast_get_locus($1), NULL);
}
;

abstract_direct_declarator : '(' abstract_main_declarator ')'
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
| '[' constant_expression_opt ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, NULL, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| abstract_direct_declarator '[' constant_expression_opt ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, $1, $3, ast_get_locus($1), NULL);
}
| ELLIPSIS
{
    $$ = ASTMake1(AST_DECLARATOR_ID_PACK, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

abstract_direct_declarator_not_ellipsis : '(' abstract_main_declarator_not_ellipsis ')'
{
	$$ = ASTMake1(AST_PARENTHESIZED_DECLARATOR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| parameters_and_qualifiers
{
	$$ = ASTMake2(AST_DECLARATOR_FUNC, NULL, $1, ast_get_locus($1), NULL);
}
| abstract_direct_declarator_not_ellipsis parameters_and_qualifiers
{
	$$ = ASTMake2(AST_DECLARATOR_FUNC, $1, $2, ast_get_locus($1), NULL);
}
| '[' constant_expression_opt ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, NULL, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| abstract_direct_declarator_not_ellipsis '[' constant_expression_opt ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, $1, $3, ast_get_locus($1), NULL);
}
;

parameter_declaration_clause : parameter_declaration_clause_nonempty %merge<ambiguityHandler>
{
	$$ = $1;
}
| parameter_declaration_clause_nonempty ',' ELLIPSIS
{
    AST variadic_tree = ASTLeaf(AST_VARIADIC_ARG, make_locus(@3.first_filename, @3.first_line, @3.first_column), $3.token_text);
	$$ = ASTList($1, variadic_tree);
} 
| parameter_declaration_clause_nonempty ELLIPSIS %merge<ambiguityHandler>
{
    AST variadic_tree = ASTLeaf(AST_VARIADIC_ARG, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text);
	$$ = ASTList($1, variadic_tree);
} 
// C++ "T f(...)" is the same as C "T f()" since
// C++ "T f()" is the same as C "T f(void)"
| ELLIPSIS
{
    AST variadic_tree = ASTLeaf(AST_VARIADIC_ARG, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
	$$ = ASTListLeaf(variadic_tree);
}
// I don't like these kind of rules but we will make an exception here
| /* empty */
{
	$$ = ASTLeaf(AST_EMPTY_PARAMETER_DECLARATION_CLAUSE, make_locus("", 0, 0), NULL);
}
;

parameter_declaration_clause_nonempty : parameter_declaration_list
{
	$$ = $1;
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

parameter_declaration : decl_specifier_seq_0 %merge<ambiguityHandler>
{
   $$ = ASTMake3(AST_PARAMETER_DECL, $1, NULL, NULL, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec %merge<ambiguityHandler>
{
   $$ = ASTMake3(AST_PARAMETER_DECL, $1, NULL, NULL, ast_get_locus($1), NULL);
}
// --
| decl_specifier_seq_0 '=' initializer_clause gcc_attributes_opt %merge<ambiguityHandler>
{
   $$ = ASTMake4(AST_PARAMETER_DECL, $1, NULL, $3, $4, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec '=' initializer_clause gcc_attributes_opt %merge<ambiguityHandler>
{
   $$ = ASTMake4(AST_PARAMETER_DECL, $1, NULL, $3, $4, ast_get_locus($1), NULL);
}
// --
| decl_specifier_seq_0 nonabstract_declarator_not_started_with_attributes gcc_attributes_opt %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, NULL, $3, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec nonglobal_nonabstract_declarator_not_started_with_attributes gcc_attributes_opt %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, NULL, $3, ast_get_locus($1), NULL);
}
// --
| decl_specifier_seq_0 nonabstract_declarator_not_started_with_attributes gcc_attributes_opt '=' initializer_clause  %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, $5, $3, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec nonglobal_nonabstract_declarator_not_started_with_attributes gcc_attributes_opt '=' initializer_clause  %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, $5, $3, ast_get_locus($1), NULL);
}
// --
| decl_specifier_seq_0 abstract_declarator_not_started_with_attributes gcc_attributes_opt %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, NULL, $3, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec nonglobal_abstract_declarator_not_started_with_attributes gcc_attributes_opt %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, NULL, $3, ast_get_locus($1), NULL);
}
// --
| decl_specifier_seq_0 abstract_declarator_not_started_with_attributes gcc_attributes_opt '=' initializer_clause %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, $5, $3, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec nonglobal_abstract_declarator_not_started_with_attributes gcc_attributes_opt '=' initializer_clause %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, $5, $3, ast_get_locus($1), NULL);
}
;

nonglobal_nonabstract_declarator : nonabstract_direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
| nonglobal_ptr_operator nonabstract_main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
| attribute_specifier_seq nonabstract_direct_declarator
{
    $$ = ASTMake2(AST_DECLARATOR, $2, $1, ast_get_locus($1), NULL);
}
| attribute_specifier_seq ptr_operator nonabstract_main_declarator
{
    $$ = ASTMake3(AST_POINTER_DECLARATOR, $1, $2, $3, ast_get_locus($1), NULL);
}
;

nonglobal_nonabstract_declarator_not_started_with_attributes : nonglobal_nonabstract_main_declarator_not_started_with_attributes
{
	$$ = $1;
}
| nonglobal_nonabstract_direct_declarator parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, $1, $2, $3, ast_get_locus($1), NULL);
}
;

nonabstract_declarator_not_started_with_attributes : nonabstract_main_declarator_not_started_with_attributes
{
    $$ = $1;
}
| nonabstract_direct_declarator parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, $1, $2, $3, ast_get_locus($1), NULL);
}
;

nonabstract_declarator : nonabstract_main_declarator
{
    $$ = $1;
}
| nonabstract_direct_declarator parameters_and_qualifiers trailing_return_type
{
    $$ = ASTMake3(AST_DECLARATOR_FUNC_TRAIL, $1, $2, $3, ast_get_locus($1), NULL);
}
;

nonabstract_main_declarator_not_started_with_attributes : nonabstract_direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
| ptr_operator nonabstract_main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
;

nonglobal_nonabstract_main_declarator_not_started_with_attributes : nonglobal_nonabstract_direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
| nonglobal_ptr_operator nonabstract_main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
;

// They are the same because nonabstract_direct_declarator does not have :: in its FIRST set
nonglobal_nonabstract_direct_declarator : nonabstract_direct_declarator
{
    $$ = $1;
}
;

nonabstract_main_declarator : nonabstract_direct_declarator
{
	$$ = ASTMake1(AST_DECLARATOR, $1, ast_get_locus($1), NULL);
}
| ptr_operator nonabstract_main_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
| attribute_specifier_seq nonabstract_direct_declarator
{
    $$ = ASTMake2(AST_DECLARATOR, $2, $1, ast_get_locus($1), NULL);
}
| attribute_specifier_seq ptr_operator nonabstract_main_declarator
{
    $$ = ASTMake3(AST_POINTER_DECLARATOR, $1, $2, $3, ast_get_locus($1), NULL);
}
;

nonabstract_direct_declarator : identifier_token std_attribute_specifier_seq_opt
{
	AST symbol = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
    $$ = ASTMake2(AST_DECLARATOR_ID_EXPR, symbol, $2, ast_get_locus(symbol), NULL);
}
| ELLIPSIS identifier_token std_attribute_specifier_seq_opt
{
	AST symbol = ASTLeaf(AST_SYMBOL, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text);
    AST declarator_id_expr = ASTMake2(AST_DECLARATOR_ID_EXPR, symbol, $3, ast_get_locus(symbol), NULL);

    $$ = ASTMake1(AST_DECLARATOR_ID_PACK, declarator_id_expr, ast_get_locus(declarator_id_expr), NULL);
}
| nonabstract_direct_declarator parameters_and_qualifiers
{
	$$ = ASTMake2(AST_DECLARATOR_FUNC, $1, $2, ast_get_locus($1), NULL);
}
| nonabstract_direct_declarator '[' constant_expression_opt ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, $1, $3, ast_get_locus($1), NULL);
}
| '(' nonabstract_main_declarator ')'
{
	$$ = ASTMake1(AST_PARENTHESIZED_DECLARATOR, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

equal_initializer : '=' initializer_clause
{
    $$ = ASTMake1(AST_EQUAL_INITIALIZER, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

equal_or_parent_initializer : equal_initializer
{
    $$ = $1;
}
| parent_initializer
{
    $$ = $1;
}
;

parent_initializer : '(' expression_list ')'
{
	$$ = ASTMake1(AST_PARENTHESIZED_INITIALIZER, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

parent_initializer_ctor : '(' expression_list ')'
{
	$$ = ASTMake1(AST_PARENTHESIZED_INITIALIZER, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '(' ')'
{
	$$ = ASTMake1(AST_PARENTHESIZED_INITIALIZER, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

initializer : equal_initializer
{
	$$ = $1;
}
| parent_initializer
{
    $$ = $1;
}
| braced_init_list
{
    $$ = $1;
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

initializer_list : initializer_clause %merge<ambiguityHandler>
{
	$$ = ASTListLeaf($1);
}
| initializer_list ',' initializer_clause %merge<ambiguityHandler>
{
	$$ = ASTList($1, $3);
}
// C++2011 pack expansion
| initializer_clause ELLIPSIS %merge<ambiguityHandler>
{
    AST pack_expansion = ASTMake1(AST_INITIALIZER_CLAUSE_PACK_EXPANSION, $1, ast_get_locus($1), NULL);
    $$ = ASTListLeaf(pack_expansion);
}
| initializer_list ',' initializer_clause ELLIPSIS %merge<ambiguityHandler>
{
    AST pack_expansion = ASTMake1(AST_INITIALIZER_CLAUSE_PACK_EXPANSION, $3, ast_get_locus($3), NULL);
    $$ = ASTList($1, pack_expansion);
}
// C99 extensions
| designation initializer_clause %merge<ambiguityHandler>
{
    AST designated_initializer = ASTMake2(AST_DESIGNATED_INITIALIZER, $1, $2, ast_get_locus($1), NULL);

    $$ = ASTListLeaf(designated_initializer);
}
| initializer_list ',' designation initializer_clause %merge<ambiguityHandler>
{
    AST designated_initializer = ASTMake2(AST_DESIGNATED_INITIALIZER, $3, $4, ast_get_locus($3), NULL);

    $$ = ASTList($1, designated_initializer);
}
// GNU Extensions
| identifier_token ':' initializer_clause %merge<ambiguityHandler>
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);

	AST gcc_initializer_clause = ASTMake2(AST_GCC_INITIALIZER_CLAUSE, identifier, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTListLeaf(gcc_initializer_clause);
}
| initializer_list ',' identifier_token ':' initializer_clause %merge<ambiguityHandler>
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

function_definition_header : functional_declarator %merge<ambiguityHandler>
{
    $$ = ASTMake3(AST_FUNCTION_DEFINITION_HEADER, NULL, $1, NULL, ast_get_locus($1), NULL);
}
| decl_specifier_seq_0 functional_declarator %merge<ambiguityHandler>
{
    $$ = ASTMake3(AST_FUNCTION_DEFINITION_HEADER, $1, $2, NULL, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec nonglobal_functional_declarator %merge<ambiguityHandler>
{
    $$ = ASTMake3(AST_FUNCTION_DEFINITION_HEADER, $1, $2, NULL, ast_get_locus($1), NULL);
}
| functional_declarator gcc_attr_or_virt_specifier_seq %merge<ambiguityHandler>
{
    $$ = ASTMake3(AST_FUNCTION_DEFINITION_HEADER, NULL, $1, $2, ast_get_locus($1), NULL);
}
// Note that in GCC C++ (but not in GCC C) there can be attributes before the function-body
| decl_specifier_seq_0 functional_declarator gcc_attr_or_virt_specifier_seq %merge<ambiguityHandler>
{
    $$ = ASTMake3(AST_FUNCTION_DEFINITION_HEADER, $1, $2, $3, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec nonglobal_functional_declarator gcc_attr_or_virt_specifier_seq %merge<ambiguityHandler>
{
    $$ = ASTMake3(AST_FUNCTION_DEFINITION_HEADER, $1, $2, $3, ast_get_locus($1), NULL);
}
;

ctor_initializer_opt : /* empty */
{
    $$ = NULL;
}
| ctor_initializer
{
    $$ = $1;
}
;

non_extension_function_definition : function_definition_header ctor_initializer_opt function_body 
{
    $$ = ASTMake3(AST_FUNCTION_DEFINITION, $1, $2, $3, ast_get_locus($1), NULL);
}
| function_definition_header '=' TOKEN_DELETE ';'
{
    $$ = ASTMake1(AST_DELETED_FUNCTION_DEFINITION, $1, ast_get_locus($1), $3.token_text);
}
| function_definition_header '=' DEFAULT ';'
{
    $$ = ASTMake1(AST_DEFAULTED_FUNCTION_DEFINITION, $1, ast_get_locus($1), $3.token_text);
}
;

function_definition : non_extension_function_definition
{
    $$ = $1;
}
| EXTENSION function_definition
{
	$$ = ASTMake1(AST_GCC_EXTENSION, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

function_body : compound_statement
{
	$$ = ASTMake1(AST_FUNCTION_BODY, $1, ast_get_locus($1), NULL);
}
| try_block
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

class_head : class_key attribute_specifier_seq_opt class_head_name class_virt_specifier_seq_opt base_clause_opt
{
    AST class_head_extra = ASTMake3(AST_CLASS_HEAD_EXTRA, $2, $4, NULL, ast_get_locus($1), NULL);
	$$ = ASTMake4(AST_CLASS_HEAD_SPEC, $1, $3, $5, class_head_extra, ast_get_locus($1), NULL);
}
| class_key attribute_specifier_seq_opt base_clause_opt
{
    AST class_head_extra = ASTMake3(AST_CLASS_HEAD_EXTRA, $2, NULL, NULL, ast_get_locus($1), NULL);
	$$ = ASTMake4(AST_CLASS_HEAD_SPEC, $1, NULL, $3, class_head_extra, ast_get_locus($1), NULL);
}
;

base_clause_opt : /* empty */
{
    $$ = NULL;
}
| base_clause
{
    $$ = $1;
}
;

class_head_name : id_type_expr
{
    $$ = $1;
}
;

class_virt_specifier_seq_opt : /* empty */
{
    $$ = NULL;
}
| class_virt_specifier_seq
{
    $$ = $1;
}
;

class_virt_specifier_seq : class_virt_specifier
{
    $$ = ASTListLeaf($1);
}
| class_virt_specifier_seq class_virt_specifier
{
    $$ = ASTList($1, $2);
}
;

class_virt_specifier : EXPLICIT
{
    $$ = ASTLeaf(AST_CLASS_VIRT_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOK_FINAL
{
    $$ = ASTLeaf(AST_CLASS_VIRT_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

class_key : CLASS
{
	$$ = ASTLeaf(AST_CLASS_KEY_CLASS, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| STRUCT
{
	$$ = ASTLeaf(AST_CLASS_KEY_STRUCT, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| UNION
{
	$$ = ASTLeaf(AST_CLASS_KEY_UNION, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

member_specification_seq : member_specification
{
	$$ = ASTListLeaf($1);
}
| member_specification_seq member_specification
{
	$$ = ASTList($1, $2);
}
;

member_specification : member_declaration
{
	$$ = $1;
}
| access_specifier ':'
{
	$$ = ASTMake1(AST_MEMBER_ACCESS_SPEC, $1, ast_get_locus($1), NULL);
}
;

member_declaration : decl_specifier_seq_0 member_declarator_list ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_MEMBER_DECLARATION, $1, $2, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec nonglobal_member_declarator_list ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_MEMBER_DECLARATION, $1, $2, ast_get_locus($1), NULL);
}
| decl_specifier_alone_seq ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_MEMBER_DECLARATION, $1, NULL, ast_get_locus($1), NULL);
}
| friend_declaration
{
    $$ = $1;
}
| member_declarator_list ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_MEMBER_DECLARATION, NULL, $1, ast_get_locus($1), NULL);
}
| non_extension_function_definition
{
	$$ = $1;
}
| id_expression ';' %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_MEMBER_DECLARATION_QUALIF, $1, ast_get_locus($1), NULL);
}
| using_declaration
{
	$$ = $1;
}
| alias_declaration
{
    $$ = $1;
}
| template_member_declaration %merge<ambiguityHandler>
{
	$$ = $1;
}
| template_friend_declaration
{
    $$ = $1;
}
| verbatim_construct
{
    $$ = $1;
}
| static_assert_declaration 
{
    $$ = $1;
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

nonglobal_member_declarator_list : nonglobal_member_declarator_first
{
    $$ = ASTListLeaf($1);
}
| nonglobal_member_declarator_first ',' member_declarator_list
{
    $$ = ast_list_concat(ASTListLeaf($1), $3);
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

nonglobal_friend_declarator_list : nonglobal_friend_declarator_first
{
    $$ = ASTListLeaf($1);
}
| nonglobal_friend_declarator_first ',' friend_declarator_list_nonempty
{
    $$ = ast_list_concat(ASTListLeaf($1), $3);
}
;

friend_declarator_list : friend_declarator_list_nonempty
{
	$$ = $1;
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

friend_declarator_list_nonempty : friend_declarator
{
	$$ = ASTListLeaf($1);
}
| friend_declarator_list_nonempty ',' friend_declarator
{
	$$ = ASTList($1, $3);
}
;

nonglobal_friend_declarator_first : nonglobal_functional_declarator
{
    $$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, NULL, NULL, ast_get_locus($1), NULL);
}
| nonglobal_functional_declarator gcc_attributes
{
    $$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, NULL, $2, ast_get_locus($1), NULL);
}
;

friend_declarator : functional_declarator
{
    $$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, NULL, NULL, ast_get_locus($1), NULL);
}
| functional_declarator gcc_attributes
{
    $$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, NULL, $2, ast_get_locus($1), NULL);
}
;

attribute_specifier_seq_and_virt_specifier_seq_opt : /* empty */ 
{
    $$ = NULL;
}
| attribute_specifier_seq_and_virt_specifier_seq
{
    $$ = $1;
}
;

attribute_specifier_seq_and_virt_specifier_seq : attribute_specifier_seq
{
    $$ = $1;
}
| virt_specifier_seq
{
    $$ = $1;
}
| attribute_specifier_seq virt_specifier_seq
{
    $$ = ast_list_concat($1, $2);
}
;

member_declarator_first : declarator_not_started_with_attributes gcc_attr_or_virt_specifier_seq_opt
{
	$$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, NULL, $2, ast_get_locus($1), NULL);
}
| declarator_not_started_with_attributes gcc_attr_or_virt_specifier_seq_opt equal_initializer
{
	$$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, $3, $2, ast_get_locus($1), NULL);
}
| non_functional_declarator_not_started_with_attributes gcc_attr_or_virt_specifier_seq_opt braced_init_list
{
	$$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, $3, $2, ast_get_locus($1), NULL);
}
| attribute_specifier_seq_and_virt_specifier_seq_opt ':' constant_expression %dprec 1
{
	$$ = ASTMake3(AST_BITFIELD_DECLARATOR, NULL, $3,
        $1, make_locus(@2.first_filename, @2.first_line, @2.first_column), NULL);
}
| identifier_token attribute_specifier_seq_and_virt_specifier_seq_opt ':' constant_expression %dprec 2
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
	AST declarator_id_expr = ASTMake1(AST_DECLARATOR_ID_EXPR, identifier, ast_get_locus(identifier), NULL);

	$$ = ASTMake3(AST_BITFIELD_DECLARATOR,
        declarator_id_expr, $4,
        $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

member_declarator : declarator gcc_attr_or_virt_specifier_seq_opt
{
	$$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, NULL, $2, ast_get_locus($1), NULL);
}
| declarator gcc_attr_or_virt_specifier_seq_opt equal_initializer
{
	$$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, $3, $2, ast_get_locus($1), NULL);
}
| non_functional_declarator gcc_attr_or_virt_specifier_seq_opt braced_init_list
{
	$$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, $3, $2, ast_get_locus($1), NULL);
}
| attribute_specifier_seq_and_virt_specifier_seq_opt ':' constant_expression %dprec 1
{
	$$ = ASTMake3(AST_BITFIELD_DECLARATOR, NULL, $3,
        $1, make_locus(@2.first_filename, @2.first_line, @2.first_column), NULL);
}
| identifier_token attribute_specifier_seq_and_virt_specifier_seq_opt ':' constant_expression %dprec 2
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
	AST declarator_id_expr = ASTMake1(AST_DECLARATOR_ID_EXPR, identifier, ast_get_locus(identifier), NULL);

	$$ = ASTMake3(AST_BITFIELD_DECLARATOR,
        declarator_id_expr, $4,
        $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

virt_specifier_seq : virt_specifier
{
    $$ = ASTListLeaf($1);
}
| virt_specifier_seq virt_specifier
{
    $$ = ASTList($1, $2);
}
;

virt_specifier : TOK_FINAL
{
    $$ = ASTLeaf(AST_MEMBER_VIRT_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOK_OVERRIDE
{
    $$ = ASTLeaf(AST_MEMBER_VIRT_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_NEW
{
    $$ = ASTLeaf(AST_MEMBER_VIRT_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

gcc_attr_or_virt_specifier_seq_opt : /* empty */
{
    $$ = NULL;
}
| gcc_attr_or_virt_specifier_seq
{
    $$ = $1;
}
;

gcc_attr_or_virt_specifier_seq : gcc_attr_or_virt_specifier
{
    $$ = ASTListLeaf($1);
}
| gcc_attr_or_virt_specifier_seq gcc_attr_or_virt_specifier
{
    $$ = ASTList($1, $2);
}
;

gcc_attr_or_virt_specifier : gcc_attribute
{
    $$ = $1;
}
| virt_specifier
{
    $$ = $1;
}
;

nonglobal_member_declarator_first : nonglobal_declarator_not_started_with_attributes gcc_attr_or_virt_specifier_seq_opt
{
	$$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, NULL, $2, ast_get_locus($1), NULL);
}
| nonglobal_declarator_not_started_with_attributes gcc_attr_or_virt_specifier_seq_opt equal_initializer
{
	$$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, $3, $2, ast_get_locus($1), NULL);
}
| nonglobal_non_functional_declarator_not_started_with_attributes gcc_attr_or_virt_specifier_seq_opt braced_init_list
{
	$$ = ASTMake3(AST_MEMBER_DECLARATOR, $1, $3, $2, ast_get_locus($1), NULL);
}
| attribute_specifier_seq_and_virt_specifier_seq_opt ':' constant_expression %dprec 1
{
	$$ = ASTMake3(AST_BITFIELD_DECLARATOR, NULL, $3,
        $1, make_locus(@2.first_filename, @2.first_line, @2.first_column), NULL);
}
| identifier_token attribute_specifier_seq_and_virt_specifier_seq_opt ':' constant_expression %dprec 2
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
	AST declarator_id_expr = ASTMake1(AST_DECLARATOR_ID_EXPR, identifier, ast_get_locus(identifier), NULL);

	$$ = ASTMake3(AST_BITFIELD_DECLARATOR,
        declarator_id_expr, $4,
        $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

// *********************************************************
// A.9 - Derived classes
// *********************************************************

base_clause : ':' base_specifier_list
{
	$$ = ASTMake1(AST_BASE_CLAUSE, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

base_specifier_list : base_specifier
{
	$$ = ASTListLeaf($1);
}
| base_specifier_list ',' base_specifier
{
	$$ = ASTList($1, $3);
}
// Pack
| base_specifier ELLIPSIS
{
    AST base_pack = ASTMake1(AST_BASE_SPEC_PACK_EXPANSION, $1, ast_get_locus($1), NULL);
	$$ = ASTListLeaf(base_pack);
}
| base_specifier_list ',' base_specifier ELLIPSIS
{
    AST base_pack = ASTMake1(AST_BASE_SPEC_PACK_EXPANSION, $3, ast_get_locus($3), NULL);
	$$ = ASTList($1, base_pack);
}
;

class_or_decltype : id_type_expr
{
    $$ = $1;
}
| decltype_specifier
{
    $$ = $1;
}
;

base_specifier : class_or_decltype
// without access_specifier
{
	$$ = ASTMake3(AST_BASE_SPEC, NULL, NULL, $1, ast_get_locus($1), NULL);
}
// without virtual but with access_specifier
| access_specifier class_or_decltype
{
	$$ = ASTMake3(AST_BASE_SPEC, NULL, $1, $2, ast_get_locus($1), NULL);
}
// with virtual and optionally access_specifier
| VIRTUAL class_or_decltype
{
    AST virtual_spec = ASTLeaf(AST_VIRTUAL_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
	$$ = ASTMake3(AST_BASE_SPEC, virtual_spec, NULL, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| VIRTUAL access_specifier class_or_decltype
{
    AST virtual_spec = ASTLeaf(AST_VIRTUAL_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
	$$ = ASTMake3(AST_BASE_SPEC, virtual_spec, $2, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
// with access_specifier and then virtual
| access_specifier VIRTUAL class_or_decltype
{
    AST virtual_spec = ASTLeaf(AST_VIRTUAL_SPEC, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text);
	$$ = ASTMake3(AST_BASE_SPEC, virtual_spec, $1, $3, ast_get_locus($1), NULL);
}
;

access_specifier : PRIVATE
{
	$$ = ASTLeaf(AST_PRIVATE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| PROTECTED
{
	$$ = ASTLeaf(AST_PROTECTED_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| PUBLIC
{
	$$ = ASTLeaf(AST_PUBLIC_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

// *********************************************************
// A.5. - Statements
// *********************************************************


statement : nondeclarating_statement %merge<ambiguityHandler>
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
| try_block
{
	$$ = $1;
}
| if_statement
{
    $$ = $1;
}
/* C++2011 */
| std_attribute_specifier_seq attributable_statement
{
    warn_printf_at(ast_get_locus($1),
        "attributes at the beginning of statement are ignored\n");
    $$ = $2;
}
;

attributable_statement : expression_statement
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
| labeled_statement
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

condition : expression %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_CONDITION, NULL, NULL, $1, ast_get_locus($1), NULL);
}
| type_specifier_seq_0 declarator gcc_extra_bits_init_declarator_opt equal_initializer %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_CONDITION, $1, $2, $4, $3, ast_get_locus($1), NULL);
}
| type_specifier_seq_ended_with_identifier nonglobal_declarator gcc_extra_bits_init_declarator_opt equal_initializer %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_CONDITION, $1, $2, $4, $3, ast_get_locus($1), NULL);
}
| type_specifier_seq_0 declarator gcc_extra_bits_init_declarator_opt braced_init_list %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_CONDITION, $1, $2, $4, $3, ast_get_locus($1), NULL);
}
| type_specifier_seq_ended_with_identifier gcc_extra_bits_init_declarator_opt nonglobal_declarator braced_init_list %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_CONDITION, $1, $2, $4, $3, ast_get_locus($1), NULL);
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
| gcc_attributes
{
    $$ = $1;
}
| asm_specification gcc_attributes
{
    $$ = ast_list_concat(ASTListLeaf($1), $2);
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
| FOR '(' for_range_declaration ':' expression ')' statement
{
    AST loop_control = ASTMake2(AST_RANGE_LOOP_CONTROL, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
	$$ = ASTMake3(AST_FOR_STATEMENT, loop_control, $7, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| FOR '(' for_range_declaration ':' braced_init_list ')' statement
{
    AST loop_control = ASTMake2(AST_RANGE_LOOP_CONTROL, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
	$$ = ASTMake3(AST_FOR_STATEMENT, loop_control, $7, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

for_range_declaration : type_specifier_seq_0 declarator %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_FOR_RANGE_DECLARATION, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_seq_ended_with_identifier nonglobal_declarator %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_FOR_RANGE_DECLARATION, $1, $2, ast_get_locus($1), NULL);
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
| RETURN braced_init_list ';'
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
// A.10 - Special member functions
// *********************************************************

conversion_function_id : OPERATOR conversion_type_id
{
	$$ = ASTMake1(AST_CONVERSION_FUNCTION_ID, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

conversion_type_id : type_specifier_seq_0
{
	$$ = ASTMake2(AST_CONVERSION_TYPE_ID, $1, NULL, ast_get_locus($1), NULL);
}
| type_specifier_seq_ended_with_identifier
{
	$$ = ASTMake2(AST_CONVERSION_TYPE_ID, $1, NULL, ast_get_locus($1), NULL);
}
| type_specifier_seq_0 conversion_declarator
{
	$$ = ASTMake2(AST_CONVERSION_TYPE_ID, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_seq_ended_with_identifier nonglobal_conversion_declarator
{
	$$ = ASTMake2(AST_CONVERSION_TYPE_ID, $1, $2, ast_get_locus($1), NULL);
}
;

conversion_declarator : ptr_operator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, NULL, ast_get_locus($1), NULL);
}
| ptr_operator conversion_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
;

nonglobal_conversion_declarator : nonglobal_ptr_operator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, NULL, ast_get_locus($1), NULL);
}
| nonglobal_ptr_operator conversion_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
;

ctor_initializer : ':' mem_initializer_list
{
	$$ = ASTMake1(AST_CTOR_INITIALIZER, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

mem_initializer_list : mem_initializer
{
	$$ = ASTListLeaf($1);
}
| mem_initializer_list ',' mem_initializer
{
	$$ = ASTList($1, $3);
}
| mem_initializer ELLIPSIS
{
    AST mem_initializer_pack = ASTMake1(AST_MEM_INITIALIZER_PACK_EXPANSION, $1, ast_get_locus($1), NULL);
	$$ = ASTListLeaf(mem_initializer_pack);
}
| mem_initializer_list ',' mem_initializer ELLIPSIS
{
    AST mem_initializer_pack = ASTMake1(AST_MEM_INITIALIZER_PACK_EXPANSION, $3, ast_get_locus($3), NULL);
	$$ = ASTList($1, mem_initializer_pack);
}
;

mem_initializer : mem_initializer_id parent_initializer_ctor
{
	$$ = ASTMake2(AST_MEM_INITIALIZER, $1, $2, ast_get_locus($1), NULL);
}
| mem_initializer_id braced_init_list
{
	$$ = ASTMake2(AST_MEM_INITIALIZER, $1, $2, ast_get_locus($1), NULL);
}
// GNU Extensions
| parent_initializer_ctor
{
	$$ = ASTMake1(AST_GCC_MEM_INITIALIZER, $1, ast_get_locus($1), NULL);
}
;

mem_initializer_id : id_expression
{
	$$ = ASTMake1(AST_MEM_INITIALIZER_ID, $1, ast_get_locus($1), NULL);
}
;

// *********************************************************
// A.4 - Expressions
// *********************************************************

primary_expression : literal
{
	$$ = $1;
}
| TOKEN_THIS
{
	$$ = ASTLeaf(AST_THIS_VARIABLE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '(' expression ')' 
{
	$$ = ASTMake1(AST_PARENTHESIZED_EXPRESSION, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| id_expression_no_destructor
{
	$$ = $1;
}
| lambda_expression
{
    $$ = $1;
}
// GNU Extensions
/*
     ( compound-statement )
     __builtin_va_arg ( assignment-expression , type-name )
     __builtin_offsetof ( type-name , offsetof-member-designator )
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
| BUILTIN_ADDRESSOF '(' assignment_expression ')'
{
    $$ = ASTMake1(AST_GXX_BUILTIN_ADDRESSOF, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| gnu_type_traits
{
    $$ = $1;
}
;

/* GNU extension */
/*
     __has_nothrow_assign ( type-id )   
     __has_nothrow_constructor ( type-id )
     __has_nothrow_copy ( type-id )
     __has_trivial_assign ( type-id )   
     __has_trivial_constructor ( type-id )
     __has_trivial_copy ( type-id )
     __has_trivial_destructor ( type-id )
     __has_virtual_destructor ( type-id )     
     __is_abstract ( type-id )
     __is_base_of ( type-id , type-id )
     __is_class ( type-id )
     __is_convertible_to ( type-id , type-id )     
     __is_empty ( type-id )
     __is_enum ( type-id )
     __is_literal_type ( type-id )
     __is_pod ( type-id )
     __is_polymorphic ( type-id )
     __is_standard_layout ( type-id )
     __is_trivial ( type-id )
     __is_union ( type-id )
*/
gnu_type_traits : GXX_HAS_NOTHROW_ASSIGN '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_HAS_NOTHROW_CONSTRUCTOR '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_HAS_NOTHROW_COPY '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_HAS_TRIVIAL_ASSIGN '(' type_id ')'   
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_HAS_TRIVIAL_CONSTRUCTOR '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_HAS_TRIVIAL_COPY '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_HAS_TRIVIAL_DESTRUCTOR '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_HAS_VIRTUAL_DESTRUCTOR '(' type_id ')'     
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_IS_ABSTRACT '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_IS_BASE_OF '(' type_id ',' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_IS_CLASS '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_IS_CONVERTIBLE_TO '(' type_id ',' type_id ')'     
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_IS_EMPTY '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_IS_ENUM '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_IS_LITERAL_TYPE '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_IS_POD '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_IS_POLYMORPHIC '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_IS_STANDARD_LAYOUT '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_IS_TRIVIAL '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_IS_UNION '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_IS_FINAL '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_IS_TRIVIALLY_ASSIGNABLE '(' type_id ',' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_IS_TRIVIALLY_COPYABLE '(' type_id ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| GXX_IS_TRIVIALLY_CONSTRUCTIBLE '(' type_id gxx_trait_comma_type_id_list_opt ')'
{
    $$ = ASTMake2(AST_GXX_TYPE_TRAITS, $3, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

gxx_trait_comma_type_id_list_opt : /* empty */
{
    $$ = NULL;
}
| ',' gxx_trait_type_id_list
{
    $$ = $2;
}
;

gxx_trait_type_id_list : type_id
{
    $$ = ASTListLeaf($1);
}
| gxx_trait_type_id_list_not_ellipsis ',' type_id
{
    $$ = ASTList($1, $3);
}
;

gxx_trait_type_id_list_not_ellipsis : type_id_not_ellipsis
{
    $$ = ASTListLeaf($1);
}
| gxx_trait_type_id_list_not_ellipsis ',' type_id_not_ellipsis
{
    $$ = ASTList($1, $3);
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

id_expression : qualified_id
{
	$$ = $1;
}
| unqualified_id
{
	$$ = $1;
}
;

id_expression_no_destructor : qualified_id_no_destructor
{
	$$ = $1;
}
| unqualified_id_no_destructor
{
	$$ = $1;
}
;

id_expression_of_template_id : template_id
{
    $$ = $1;
}
| global_qualified_id_template
{
    $$ = $1;
}
| nonglobal_qualified_id_template
{
    $$ = $1;
}
| operator_function_id_name_and_template_args
{
    $$ = $1;
}
| global_qualified_operator_function_id_name_and_template_args
{
    $$ = $1;
}
| nonglobal_qualified_operator_function_id_name_and_template_args
{
    $$ = $1;
}
;

nonglobal_id_expression : unqualified_id
{
    $$ = $1;
}
| nonglobal_qualified_id_no_template
{
    $$ = $1;
}
| nonglobal_qualified_id_template
{
    $$ = $1;
}
;

id_type_expr : qualified_id_type_name
{
    $$ = $1;
}
| unqualified_id_type_name
{
    $$ = $1;
}
| nonglobal_qualified_id_template
{
    $$ = $1;
}
| global_qualified_id_template
{
    $$ = $1;
}
;

nonglobal_id_type_expr: nonglobal_qualified_id_no_template
{
    $$ = $1;
}
| nonglobal_qualified_id_template
{
    $$ = $1;
}
| unqualified_id_type_name
{
    $$ = $1;
}
;

qualified_id_type_expr : qualified_id_type_name
{
    $$ = $1;
}
| nonglobal_qualified_id_template
{
    $$ = $1;
}
| global_qualified_id_template
{
    $$ = $1;
}
;

qualified_id : nonglobal_qualified_id_no_template
{
    $$ = $1;
}
| qualified_id_globals
{
    $$ = $1;
}
| nonglobal_qualified_id_template
{
    $$ = $1;
}
;

qualified_id_no_destructor : nonglobal_qualified_id_no_template_no_destructor
{
    $$ = $1;
}
| qualified_id_globals_no_destructor
{
    $$ = $1;
}
| nonglobal_qualified_id_template
{
    $$ = $1;
}
;

qualified_id_globals_no_destructor : global_qualified_id_no_template_no_destructor
{
    $$ = $1;
}
| global_qualified_id_template
{
    $$ = $1;
}
| global_qualified_id_operator
{
    $$ = $1;
}
;

qualified_id_globals : global_qualified_id_no_template
{
    $$ = $1;
}
| global_qualified_id_template
{
    $$ = $1;
}
| global_qualified_id_operator
{
    $$ = $1;
}
;

qualified_id_type_name : global_qualified_id_type_no_template
{
    $$ = $1;
}
| nonglobal_qualified_id_type_no_template
{
    $$ = $1;
}
;

global_unqualified_id : TWO_COLONS unqualified_name
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
	$$ = ASTMake3(AST_QUALIFIED_ID, global_op, NULL, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
};

global_qualified_id_no_template : TWO_COLONS nested_name_specifier unqualified_id
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTMake3(AST_QUALIFIED_ID, global_op, $2, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| global_unqualified_id
{
    $$ = $1;
}
;

global_qualified_id_type_no_template : TWO_COLONS nested_name_specifier unqualified_id_type_name
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTMake3(AST_QUALIFIED_ID, global_op, $2, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| global_unqualified_id
{
    $$ = $1;
}
;

global_qualified_id_no_template_no_destructor : TWO_COLONS nested_name_specifier unqualified_id_no_destructor
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTMake3(AST_QUALIFIED_ID, global_op, $2, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| global_unqualified_id
{
    $$ = $1;
}
;

global_qualified_id_operator : TWO_COLONS operator_function_id
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTMake3(AST_QUALIFIED_ID, global_op, NULL, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

global_qualified_id_template : TWO_COLONS nested_name_specifier TEMPLATE template_id
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

    ast_set_text($4, "template ");

	$$ = ASTMake3(AST_QUALIFIED_ID, global_op, $2, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TWO_COLONS template_id
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTMake3(AST_QUALIFIED_ID, global_op, NULL, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

nonglobal_qualified_id_no_template : nested_name_specifier unqualified_id
{
	$$ = ASTMake3(AST_QUALIFIED_ID, NULL, $1, $2, ast_get_locus($1), NULL);
}
;

nonglobal_qualified_id_type_no_template : nested_name_specifier unqualified_id_type_name
{
	$$ = ASTMake3(AST_QUALIFIED_ID, NULL, $1, $2, ast_get_locus($1), NULL);
}
;

nonglobal_qualified_id_no_template_no_destructor : nested_name_specifier unqualified_id_no_destructor
{
	$$ = ASTMake3(AST_QUALIFIED_ID, NULL, $1, $2, ast_get_locus($1), NULL);
}
;

nonglobal_qualified_id_template : nested_name_specifier TEMPLATE template_id
{
    ast_set_text($3, "template ");

	$$ = ASTMake3(AST_QUALIFIED_ID, NULL, $1, $3, ast_get_locus($1), NULL);
}
;

nested_name_specifier : nested_name_specifier_0 TWO_COLONS %merge<ambiguityHandler>
{
    $$ = ASTListLeaf($1);
}
| nested_name_specifier nested_name_specifier_1 TWO_COLONS %merge<ambiguityHandler>
{
    $$ = ASTList($1, $2);
}
;

nested_name_specifier_0 : class_or_namespace_name
{
    $$ = $1;
}
| decltype_specifier
;

nested_name_specifier_1 : nested_name_specifier_0
{
 	$$ = $1;
}
| TEMPLATE template_id
{
    ast_set_text($2, "template ");
    $$ = $2;
}
;

class_or_namespace_name : template_id
{
	$$ = $1;
}
| unqualified_name
{
	$$ = $1;
}
;

unqualified_id : unqualified_id_type_name
{
    $$ = $1;
}
| unqualified_id_funct
{
    $$ = $1;
}
;

unqualified_id_no_destructor : unqualified_id_type_name
{
    $$ = $1;
}
| unqualified_id_funct_no_destructor
{
    $$ = $1;
}
;

unqualified_id_type_name : unqualified_name
{
	$$ = $1;
}
| template_id
{
	$$ = $1;
}
;

unqualified_name : identifier_token
{
	$$ = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

unqualified_id_funct_no_destructor : operator_function_id
{
	$$ = $1;
}
| conversion_function_id
{
	$$ = $1;
}
;

unqualified_id_funct : unqualified_id_funct_no_destructor
{
	$$ = $1;
}
| destructor_id
{
    $$ = $1;
}
;

destructor_id : '~' identifier_token
{
	// Construct an artificial name for it
    const char* destructor_name = NULL;
    uniquestr_sprintf(&destructor_name, "~%s", $2.token_text);

	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@2.first_filename, @2.first_line, @2.first_column), destructor_name);

	$$ = ASTMake1(AST_DESTRUCTOR_ID, identifier, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| destructor_template_id
{
    $$ = $1;
}
;


destructor_template_id : '~' template_id
{
	AST symbol = ASTSon0($2);

	// Construct an artificial name for it
    const char* destructor_name = NULL;
    uniquestr_sprintf(&destructor_name, "~%s", ASTText(symbol));

	$$ = ASTMake1(AST_DESTRUCTOR_TEMPLATE_ID, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), destructor_name);
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
| postfix_expression '[' braced_init_list ']'
{
	$$ = ASTMake2(AST_ARRAY_SUBSCRIPT, $1, $3, ast_get_locus($1), NULL);
}
| postfix_expression '(' ')' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_FUNCTION_CALL, $1, NULL, ast_get_locus($1), NULL);
}
| postfix_expression '(' expression_list ')' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_FUNCTION_CALL, $1, $3, ast_get_locus($1), NULL);
}
| explicit_simple_type_specifier explicit_type_initializer %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_EXPLICIT_TYPE_CONVERSION, $1, $2, ast_get_locus($1), NULL);
}
| TYPENAME qualified_id_type_expr explicit_type_initializer
{
	$$ = ASTMake2(AST_TYPENAME_EXPLICIT_TYPE_CONV, $2, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| postfix_expression '.' id_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_CLASS_MEMBER_ACCESS, $1, $3, ast_get_locus($1), NULL);
}
| postfix_expression '.' TEMPLATE id_expression_of_template_id
{
	$$ = ASTMake2(AST_CLASS_TEMPLATE_MEMBER_ACCESS, $1, $4, ast_get_locus($1), NULL);
}
| postfix_expression PTR_OP id_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_POINTER_CLASS_MEMBER_ACCESS, $1, $3, ast_get_locus($1), NULL);
}
| postfix_expression PTR_OP TEMPLATE id_expression_of_template_id
{
	$$ = ASTMake2(AST_POINTER_CLASS_TEMPLATE_MEMBER_ACCESS, $1, $4, ast_get_locus($1), NULL);
}
| postfix_expression PLUSPLUS
{
	$$ = ASTMake1(AST_POSTINCREMENT, $1, ast_get_locus($1), NULL);
}
| postfix_expression MINUSMINUS
{
	$$ = ASTMake1(AST_POSTDECREMENT, $1, ast_get_locus($1), NULL);
}
| DYNAMIC_CAST '<' type_id '>' '(' expression ')'
{
	$$ = ASTMake2(AST_DYNAMIC_CAST, $3, ASTListLeaf($6), make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| STATIC_CAST '<' type_id '>' '(' expression ')' 
{
	$$ = ASTMake2(AST_STATIC_CAST, $3, ASTListLeaf($6), make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| REINTERPRET_CAST '<' type_id '>' '(' expression ')'
{
	$$ = ASTMake2(AST_REINTERPRET_CAST, $3, ASTListLeaf($6), make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| CONST_CAST '<' type_id '>' '(' expression ')'
{
	$$ = ASTMake2(AST_CONST_CAST, $3, ASTListLeaf($6), make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TYPEID '(' expression ')' %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_TYPEID_EXPR, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TYPEID '(' type_id ')'  %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_TYPEID_TYPE, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
// GNU Extensions
| '(' type_id ')' braced_init_list
{
	$$ = ASTMake2(AST_GCC_POSTFIX_EXPRESSION, $2, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

expression_list : initializer_list
{
    $$ = $1;
}
;

explicit_type_initializer : parent_initializer_ctor
{
    $$ = $1;
}
| braced_init_list
{
    $$ = $1;
}
;

unary_expression : postfix_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| PLUSPLUS cast_expression
{
	$$ = ASTMake1(AST_PREINCREMENT, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| MINUSMINUS cast_expression
{
	$$ = ASTMake1(AST_PREDECREMENT, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| unary_operator cast_expression %merge<ambiguityHandler>
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
| SIZEOF ELLIPSIS '(' identifier_token ')'
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@4.first_filename, @4.first_line, @4.first_column), $4.token_text);

    $$ = ASTMake1(AST_SIZEOF_PACK, identifier, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| new_expression
{
	$$ = $1;
}
| delete_expression
{
	$$ = $1;
}
| TOKEN_ALIGNOF '(' type_id ')'
{
	$$ = ASTMake1(AST_ALIGNOF_TYPE, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| TOKEN_NOEXCEPT '(' expression ')'
{
    $$ = ASTMake1(AST_NOEXCEPT_EXPRESSION, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
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

new_expression : TOKEN_NEW new_type_id 
{
	$$ = ASTMake4(AST_NEW_EXPRESSION, NULL, NULL, $2, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TOKEN_NEW new_type_id new_initializer
{
	$$ = ASTMake4(AST_NEW_EXPRESSION, NULL, NULL, $2, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TOKEN_NEW new_placement new_type_id 
{
	$$ = ASTMake4(AST_NEW_EXPRESSION, NULL, $2, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TOKEN_NEW new_placement new_type_id new_initializer
{
	$$ = ASTMake4(AST_NEW_EXPRESSION, NULL, $2, $3, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TWO_COLONS TOKEN_NEW new_type_id 
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTMake4(AST_NEW_EXPRESSION, global_op, NULL, NULL, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TWO_COLONS TOKEN_NEW new_type_id new_initializer
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTMake4(AST_NEW_EXPRESSION, global_op, NULL, $3, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TWO_COLONS TOKEN_NEW new_placement new_type_id 
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTMake4(AST_NEW_EXPRESSION, global_op, $3, $4, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TWO_COLONS TOKEN_NEW new_placement new_type_id new_initializer
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTMake4(AST_NEW_EXPRESSION, global_op, $3, $4, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TOKEN_NEW '(' type_id ')' 
{
	$$ = ASTMake4(AST_NEW_TYPE_ID_EXPR, NULL, NULL, $3, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TOKEN_NEW '(' type_id ')' new_initializer
{
	$$ = ASTMake4(AST_NEW_TYPE_ID_EXPR, NULL, NULL, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TOKEN_NEW new_placement '(' type_id ')' 
{
	$$ = ASTMake4(AST_NEW_TYPE_ID_EXPR, NULL, $2, $4, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TOKEN_NEW new_placement '(' type_id ')' new_initializer
{
	$$ = ASTMake4(AST_NEW_TYPE_ID_EXPR, NULL, $2, $4, $6, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TWO_COLONS TOKEN_NEW '(' type_id ')' 
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTMake4(AST_NEW_TYPE_ID_EXPR, global_op, NULL, $4, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TWO_COLONS TOKEN_NEW '(' type_id ')' new_initializer
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTMake4(AST_NEW_TYPE_ID_EXPR, global_op, NULL, $4, $6, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TWO_COLONS TOKEN_NEW new_placement '(' type_id ')' 
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTMake4(AST_NEW_TYPE_ID_EXPR, global_op, $3, $5, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TWO_COLONS TOKEN_NEW new_placement '(' type_id ')' new_initializer
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTMake4(AST_NEW_TYPE_ID_EXPR, global_op, $3, $5, $7, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

new_placement : '(' expression_list ')'
{
	$$ = ASTMake1(AST_NEW_PLACEMENT, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

new_type_id : type_specifier_seq_0
{
	$$ = ASTMake2(AST_NEW_TYPE_ID, $1, NULL, ast_get_locus($1), NULL);
}
| type_specifier_seq_ended_with_identifier
{
	$$ = ASTMake2(AST_NEW_TYPE_ID, $1, NULL, ast_get_locus($1), NULL);
}
| type_specifier_seq_0 new_declarator
{
	$$ = ASTMake2(AST_NEW_TYPE_ID, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_seq_ended_with_identifier nonglobal_new_declarator
{
	$$ = ASTMake2(AST_NEW_TYPE_ID, $1, $2, ast_get_locus($1), NULL);
}
;

new_declarator : ptr_operator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, NULL, ast_get_locus($1), NULL);
}
| ptr_operator new_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
| direct_new_declarator
{
	$$ = $1;
}
;

nonglobal_new_declarator : nonglobal_ptr_operator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, NULL, ast_get_locus($1), NULL);
}
| nonglobal_ptr_operator new_declarator
{
	$$ = ASTMake2(AST_POINTER_DECLARATOR, $1, $2, ast_get_locus($1), NULL);
}
| direct_new_declarator
{
	$$ = $1;
}
;

direct_new_declarator : '[' expression ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, NULL, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| direct_new_declarator '[' constant_expression ']'
{
	$$ = ASTMake2(AST_DECLARATOR_ARRAY, $1, $3, ast_get_locus($1), NULL);
}
;

new_initializer : explicit_type_initializer
{
	$$ = $1;
}
;

delete_expression : TOKEN_DELETE cast_expression
{
	$$ = ASTMake2(AST_DELETE_EXPR, NULL, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TOKEN_DELETE_ARRAY cast_expression
{
	$$ = ASTMake2(AST_DELETE_ARRAY_EXPR, NULL, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TWO_COLONS TOKEN_DELETE cast_expression
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTMake2(AST_DELETE_EXPR, global_op, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TWO_COLONS TOKEN_DELETE_ARRAY cast_expression
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTMake2(AST_DELETE_ARRAY_EXPR, global_op, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
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

// pm stands for "pointer to member"
pm_expression : cast_expression
{
	$$ = $1;
}
| pm_expression PTR_OP_MUL cast_expression
{
	$$ = ASTMake2(AST_POINTER_TO_POINTER_MEMBER, $1, $3, ast_get_locus($1), NULL);
}
| pm_expression '.' '*' cast_expression
{
	$$ = ASTMake2(AST_POINTER_TO_MEMBER, $1, $4, ast_get_locus($1), NULL);
}
;

multiplicative_expression : pm_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| multiplicative_expression '*' pm_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_MUL, $1, $3, ast_get_locus($1), NULL);
}
| multiplicative_expression '/' pm_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_DIV, $1, $3, ast_get_locus($1), NULL);
}
| multiplicative_expression '%' pm_expression %merge<ambiguityHandler>
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
| shift_expression right_token additive_expression %merge<ambiguityHandler>
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
| relational_expression greater_equal_token shift_expression %merge<ambiguityHandler>
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
| logical_or_expression '?' expression ':' assignment_expression %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_CONDITIONAL_EXPRESSION, $1, $3, $5, ast_get_locus($1), NULL);
}
// GNU Extension
| logical_or_expression '?' ':' assignment_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_GCC_CONDITIONAL_EXPRESSION, $1, $4, ast_get_locus($1), NULL);
}
;

assignment_expression : conditional_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| logical_or_expression assignment_operator initializer_clause %merge<ambiguityHandler>
{
	$$ = ASTMake2($2, $1, $3, ast_get_locus($1), NULL);
}
| throw_expression %merge<ambiguityHandler>
{
	$$ = $1;
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
| right_assign_token
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

constant_expression_opt : /* empty */
{
    $$ = NULL;
}
| constant_expression
{
    $$ = $1;
}
;

constant_expression : conditional_expression
{
	$$ = ASTMake1(AST_CONSTANT_EXPRESSION, $1, ast_get_locus($1), NULL);
}
;

lambda_expression : lambda_introducer lambda_declarator_opt compound_statement
{
    AST locus = ($1 == NULL ? ($2 == NULL ? $3 : $2) : $1);
    $$ = ASTMake3(AST_LAMBDA_EXPRESSION, $1, $2, $3, ast_get_locus(locus), NULL);
}
;

lambda_introducer : '[' ']'
{
    $$ = NULL;
}
| '[' lambda_capture ']'
{
    $$ = $2;
}
;

lambda_capture : lambda_capture_default
{
    $$ = ASTMake2(AST_LAMBDA_CAPTURE, $1, NULL, ast_get_locus($1), NULL);
}
| lambda_capture_list
{
    $$ = ASTMake2(AST_LAMBDA_CAPTURE, NULL, $1, ast_get_locus($1), NULL);
}
| lambda_capture_default ',' lambda_capture_list
{
    $$ = ASTMake2(AST_LAMBDA_CAPTURE, $1, $3, ast_get_locus($1), NULL);
}
;

lambda_capture_default : '&'
{
    $$ = ASTLeaf(AST_LAMBDA_CAPTURE_DEFAULT_ADDR, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| '='
{
    $$ = ASTLeaf(AST_LAMBDA_CAPTURE_DEFAULT_VALUE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

lambda_capture_list : lambda_capture_item
{
    $$ = ASTListLeaf($1);
}
| lambda_capture_list ',' lambda_capture_item
{
    $$ = ASTList($1, $3);
}
| lambda_capture_item ELLIPSIS
{
    AST pack_exp = ASTMake1(AST_LAMBDA_CAPTURE_PACK_EXPANSION, $1, ast_get_locus($1), NULL);
    $$ = ASTListLeaf(pack_exp);
}
| lambda_capture_list ',' lambda_capture_item ELLIPSIS
{
    AST pack_exp = ASTMake1(AST_LAMBDA_CAPTURE_PACK_EXPANSION, $3, ast_get_locus($3), NULL);
    $$ = ASTList($1, pack_exp);
}
;

lambda_capture_item : lambda_capture_item_simple
{
    $$ = $1;
}
| lambda_capture_item_init
{
    $$ = $1;
}
;

lambda_capture_item_simple : identifier_token
{
	$$ = ASTLeaf(AST_LAMBDA_CAPTURE_VALUE, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| '&' identifier_token
{
	$$ = ASTLeaf(AST_LAMBDA_CAPTURE_ADDRESS, make_locus(@1.first_filename, @1.first_line, @1.first_column), $2.token_text);
}
| TOKEN_THIS
{
	$$ = ASTLeaf(AST_LAMBDA_CAPTURE_THIS, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

lambda_capture_item_init : identifier_token initializer
{
	$$ = ASTMake1(AST_LAMBDA_CAPTURE_VALUE_INIT, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| '&' identifier_token initializer
{
	$$ = ASTMake1(AST_LAMBDA_CAPTURE_ADDRESS_INIT, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), $2.token_text);
}
;

lambda_declarator_opt : /* empty */
{
    $$ = NULL;
}
| lambda_declarator
{
    $$ = $1;
}
;

lambda_declarator : '(' parameter_declaration_clause ')' mutable_opt exception_specification_opt std_attribute_specifier_seq_opt trailing_return_type_opt
{
    // This tree is the same as in parameter_and_qualifiers
    AST parameters = ASTMake4(AST_PARAMETERS_AND_QUALIFIERS,
            $2,
            ASTMake4(AST_PARAMETERS_AND_QUALIFIERS_EXTRA, $6, NULL, NULL, $5,
                    make_locus(@1.first_filename, @1.first_line, @1.first_column),
                    NULL),
            NULL, NULL,
            make_locus(@1.first_filename, @1.first_line, @1.first_column),
            NULL);

    // mutable and trailing_return_type go here
    $$ = ASTMake3(AST_LAMBDA_DECLARATOR, parameters, $4, $7,
            make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

mutable_opt : MUTABLE
{
    $$ = ASTLeaf(AST_MUTABLE_SPEC, make_locus(@1.first_filename, @1.first_line, @1.first_column), 0);
}
| /* empty */
{
    $$ = NULL;
}
;

// *********************************************************
// A.2 - Lexical conventions
// *********************************************************

literal : integer_literal
{
    $$ = $1;
}
| character_literal
{
    $$ = $1;
}
| floating_literal
{
    $$ = $1;
}
| string_literal
{
    $$ = $1;
}
| boolean_literal
{
    $$ = $1;
}
| pointer_literal
{
    $$ = $1;
}
| user_defined_literal
{
    $$ = $1;
}
// Specific C language literal
| HEXADECIMAL_FLOAT
{
    $$ = ASTLeaf(AST_HEXADECIMAL_FLOAT, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;


integer_literal : DECIMAL_LITERAL
{
	$$ = ASTLeaf(AST_DECIMAL_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| OCTAL_LITERAL
{
	$$ = ASTLeaf(AST_OCTAL_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| HEXADECIMAL_LITERAL
{
	$$ = ASTLeaf(AST_HEXADECIMAL_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| BINARY_LITERAL
{
	$$ = ASTLeaf(AST_BINARY_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

character_literal : CHARACTER_LITERAL
{
	$$ = ASTLeaf(AST_CHARACTER_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

floating_literal: FLOATING_LITERAL
{
	$$ = ASTLeaf(AST_FLOATING_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;


// This eases parsing, though it should be viewed as a lexical issue
string_literal : STRING_LITERAL
{
	$$ = ASTLeaf(AST_STRING_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
| string_literal STRING_LITERAL
{
	const char* str1 = ASTText($1);
	const char* str2 = $2.token_text;
	char* text = NEW_VEC0(char, strlen(str1) + strlen(str2) + 1);

	strcat(text, str1);
	strcat(text, str2);

	$$ = ASTLeaf(AST_STRING_LITERAL, ast_get_locus($1), text);
}
;

boolean_literal : BOOLEAN_LITERAL
{
	$$ = ASTLeaf(AST_BOOLEAN_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

pointer_literal: TOKEN_NULLPTR
{
    $$ = ASTLeaf(AST_POINTER_LITERAL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);
}
;

user_defined_literal: integer_literal identifier_token
{
    $$ = ASTMake2(AST_USER_DEFINED_LITERAL,
            $1,
            ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $2.token_text),
            make_locus(@1.first_filename, @1.first_line, @1.first_column),
            NULL);
}
| floating_literal identifier_token
{
    $$ = ASTMake2(AST_USER_DEFINED_LITERAL,
            $1,
            ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $2.token_text),
            make_locus(@1.first_filename, @1.first_line, @1.first_column),
            NULL);
}
| string_literal identifier_token
{
    $$ = ASTMake2(AST_USER_DEFINED_LITERAL,
            $1,
            ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $2.token_text),
            make_locus(@1.first_filename, @1.first_line, @1.first_column),
            NULL);
}
| character_literal identifier_token
{
    $$ = ASTMake2(AST_USER_DEFINED_LITERAL,
            $1,
            ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $2.token_text),
            make_locus(@1.first_filename, @1.first_line, @1.first_column),
            NULL);
}
;

// *********************************************************
// A.11 - Overloading
// *********************************************************

operator_function_id : operator_function_id_name
{
    $$ = $1;
}
| operator_function_id_name_and_template_args
{
    $$ = $1;
}
;

operator_function_id_name : OPERATOR operator
{
	$$ = ASTMake1(AST_OPERATOR_FUNCTION_ID, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| OPERATOR_LITERAL identifier_token
{
    $$ = ASTMake1(AST_LITERAL_OPERATOR_ID,
            ASTLeaf(AST_SYMBOL, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text),
            make_locus(@2.first_filename, @2.first_line, @2.first_column), NULL);
}
;

operator_function_id_name_and_template_args : OPERATOR operator '<' template_argument_list '>'
{
	$$ = ASTMake2(AST_OPERATOR_FUNCTION_ID_TEMPLATE, $2, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| OPERATOR operator '<' '>'
{
	$$ = ASTMake2(AST_OPERATOR_FUNCTION_ID_TEMPLATE, $2, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| OPERATOR_LITERAL identifier_token '<' '>'
{
    $$ = ASTMake2(AST_LITERAL_OPERATOR_ID_TEMPLATE,
            ASTLeaf(AST_SYMBOL, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text),
            NULL,
            make_locus(@2.first_filename, @2.first_line, @2.first_column), NULL);
}
| OPERATOR_LITERAL identifier_token '<' template_argument_list '>'
{
    $$ = ASTMake2(AST_LITERAL_OPERATOR_ID_TEMPLATE,
            ASTLeaf(AST_SYMBOL, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text),
            $4,
            make_locus(@2.first_filename, @2.first_line, @2.first_column), NULL);
}
;

global_qualified_operator_function_id_name_and_template_args : TWO_COLONS nested_name_specifier operator_function_id_name_and_template_args
{
	AST global_op = ASTLeaf(AST_GLOBAL_SCOPE, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);

	$$ = ASTMake3(AST_QUALIFIED_ID, global_op, $2, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

nonglobal_qualified_operator_function_id_name_and_template_args : nested_name_specifier operator_function_id_name_and_template_args
{
	$$ = ASTMake3(AST_QUALIFIED_ID, NULL, $1, $2, ast_get_locus($1), NULL);
}
;

operator : TOKEN_NEW
{
	$$ = ASTLeaf(AST_NEW_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TOKEN_DELETE
{
	$$ = ASTLeaf(AST_DELETE_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TOKEN_NEW_ARRAY
{
	$$ = ASTLeaf(AST_NEW_ARRAY_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TOKEN_DELETE_ARRAY
{
	$$ = ASTLeaf(AST_DELETE_ARRAY_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '+'
{
	$$ = ASTLeaf(AST_ADD_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '-'
{
	$$ = ASTLeaf(AST_MINUS_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '*'
{
	$$ = ASTLeaf(AST_MUL_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '/'
{
	$$ = ASTLeaf(AST_DIV_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '%' 
{
	$$ = ASTLeaf(AST_MOD_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '^'
{
	$$ = ASTLeaf(AST_BITWISE_XOR_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '&'
{
	$$ = ASTLeaf(AST_BITWISE_AND_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '|'
{
	$$ = ASTLeaf(AST_BITWISE_OR_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '~'
{
	$$ = ASTLeaf(AST_BITWISE_NEG_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '!'
{
	$$ = ASTLeaf(AST_LOGICAL_NOT_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '='
{
	$$ = ASTLeaf(AST_ASSIGNMENT_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '<'
{
	$$ = ASTLeaf(AST_LOWER_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '>'
{
	$$ = ASTLeaf(AST_GREATER_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| ADD_ASSIGN
{
	$$ = ASTLeaf(AST_ADD_ASSIGN_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| SUB_ASSIGN
{
	$$ = ASTLeaf(AST_SUB_ASSIGN_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| MUL_ASSIGN
{
	$$ = ASTLeaf(AST_MUL_ASSIGN_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| DIV_ASSIGN
{
	$$ = ASTLeaf(AST_DIV_ASSIGN_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| MOD_ASSIGN
{
	$$ = ASTLeaf(AST_MOD_ASSIGN_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| XOR_ASSIGN
{
	$$ = ASTLeaf(AST_BITWISE_XOR_ASSIGN_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| AND_ASSIGN
{
	$$ = ASTLeaf(AST_BITWISE_AND_ASSIGN_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| OR_ASSIGN
{
	$$ = ASTLeaf(AST_BITWISE_OR_ASSIGN_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| LEFT
{
	$$ = ASTLeaf(AST_LEFT_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| right_token
{
	$$ = ASTLeaf(AST_RIGHT_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| LEFT_ASSIGN
{
	$$ = ASTLeaf(AST_LEFT_ASSIGN_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| right_assign_token
{
	$$ = ASTLeaf(AST_RIGHT_ASSIGN_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| EQUAL
{
	$$ = ASTLeaf(AST_EQUAL_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| NOT_EQUAL
{
	$$ = ASTLeaf(AST_DIFFERENT_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| LESS_OR_EQUAL
{
	$$ = ASTLeaf(AST_LESS_OR_EQUAL_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| greater_equal_token
{
	$$ = ASTLeaf(AST_GREATER_OR_EQUAL_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| ANDAND
{
	$$ = ASTLeaf(AST_LOGICAL_AND_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| OROR
{
	$$ = ASTLeaf(AST_LOGICAL_OR_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| PLUSPLUS
{
	$$ = ASTLeaf(AST_INCREMENT_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| MINUSMINUS
{
	$$ = ASTLeaf(AST_DECREMENT_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| ','
{
	$$ = ASTLeaf(AST_COMMA_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| PTR_OP
{
	$$ = ASTLeaf(AST_POINTER_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| PTR_OP_MUL
{
	$$ = ASTLeaf(AST_POINTER_DERREF_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '(' ')'
{
	$$ = ASTLeaf(AST_FUNCTION_CALL_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| '[' ']'
{
	$$ = ASTLeaf(AST_SUBSCRIPT_OPERATOR, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

// *********************************************************
// A.12 - Templates
// *********************************************************

template_id : identifier_token '<' template_argument_list right_bracket_token
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);

	$$ = ASTMake2(AST_TEMPLATE_ID, identifier, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| identifier_token '<' right_bracket_token
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@1.first_filename, @1.first_line, @1.first_column), $1.token_text);

	$$ = ASTMake2(AST_TEMPLATE_ID, identifier, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

template_argument_list : template_argument %merge<ambiguityHandler>
{
	$$ = ASTListLeaf($1);
}
| template_argument_list ',' template_argument %merge<ambiguityHandler>
{
	$$ = ASTList($1, $3);
}
;

template_argument : template_assignment_expression %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_TEMPLATE_EXPRESSION_ARGUMENT,
			ASTMake1(AST_EXPRESSION, $1, ast_get_locus($1), NULL), 
             ast_get_locus($1), NULL);
}
| type_id_not_ellipsis %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_TEMPLATE_TYPE_ARGUMENT, $1, ast_get_locus($1), NULL);
}
|template_assignment_expression ELLIPSIS %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_TEMPLATE_EXPRESSION_ARGUMENT_PACK,
			ASTMake1(AST_EXPRESSION, $1, ast_get_locus($1), NULL), 
             ast_get_locus($1), NULL);
}
| type_id_not_ellipsis ELLIPSIS %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_TEMPLATE_TYPE_ARGUMENT_PACK,
            $1, ast_get_locus($1), NULL);
}
;

template_declaration : TEMPLATE '<' template_parameter_list '>' templated_declaration
{
	$$ = ASTMake2(AST_TEMPLATE_DECLARATION, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| EXPORT TEMPLATE '<' template_parameter_list '>' templated_declaration
{
	$$ = ASTMake2(AST_EXPORT_TEMPLATE_DECLARATION, $4, $6, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

template_member_declaration : TEMPLATE '<' template_parameter_list '>' templated_declaration
{
	$$ = ASTMake2(AST_TEMPLATE_DECLARATION, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

template_friend_declaration : TEMPLATE '<' template_parameter_list '>' friend_templated_declaration
{
	$$ = ASTMake2(AST_TEMPLATE_DECLARATION, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

template_parameter_list : template_parameter
{
	$$ = ASTListLeaf($1);
}
| template_parameter_list ',' template_parameter
{
	$$ = ASTList($1, $3);
}
;

template_parameter : type_parameter %dprec 2
{
	$$ = $1;
}
| template_parameter_declaration %dprec 1
{
	$$ = $1;
}
;

template_parameter_declaration : decl_specifier_seq_0 %merge<ambiguityHandler>
{
   $$ = ASTMake3(AST_PARAMETER_DECL, $1, NULL, NULL, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec %merge<ambiguityHandler>
{
   $$ = ASTMake3(AST_PARAMETER_DECL, $1, NULL, NULL, ast_get_locus($1), NULL);
}
// --
| decl_specifier_seq_0 '=' template_assignment_expression gcc_attributes_opt %merge<ambiguityHandler>
{
   $$ = ASTMake4(AST_PARAMETER_DECL, $1, NULL, $3, $4, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec '=' template_assignment_expression gcc_attributes_opt %merge<ambiguityHandler>
{
   $$ = ASTMake4(AST_PARAMETER_DECL, $1, NULL, $3, $4, ast_get_locus($1), NULL);
}
// --
| decl_specifier_seq_0 nonabstract_declarator_not_started_with_attributes gcc_attributes_opt %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, NULL, $3, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec nonglobal_nonabstract_declarator_not_started_with_attributes gcc_attributes_opt %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, NULL, $3, ast_get_locus($1), NULL);
}
// --
| decl_specifier_seq_0 nonabstract_declarator_not_started_with_attributes gcc_attributes_opt '=' template_assignment_expression  %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, $5, $3, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec nonglobal_nonabstract_declarator_not_started_with_attributes gcc_attributes_opt '=' template_assignment_expression  %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, $5, $3, ast_get_locus($1), NULL);
}
// --
| decl_specifier_seq_0 abstract_declarator_not_started_with_attributes gcc_attributes_opt %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, NULL, $3, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec nonglobal_abstract_declarator_not_started_with_attributes gcc_attributes_opt %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, NULL, $3, ast_get_locus($1), NULL);
}
// --
| decl_specifier_seq_0 abstract_declarator_not_started_with_attributes gcc_attributes_opt '=' template_assignment_expression %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, $5, $3, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec nonglobal_abstract_declarator_not_started_with_attributes gcc_attributes_opt '=' template_assignment_expression %merge<ambiguityHandler>
{
	$$ = ASTMake4(AST_PARAMETER_DECL, $1, $2, $5, $3, ast_get_locus($1), NULL);
}
;

type_parameter : CLASS
{
	$$ = ASTMake2(AST_TYPE_PARAMETER_CLASS, NULL, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| CLASS identifier_token
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text);

	$$ = ASTMake2(AST_TYPE_PARAMETER_CLASS, identifier, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| CLASS '=' type_id
{
	$$ = ASTMake2(AST_TYPE_PARAMETER_CLASS, NULL, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| CLASS identifier_token '=' type_id
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text);

	$$ = ASTMake2(AST_TYPE_PARAMETER_CLASS, identifier, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TYPENAME
{
	$$ = ASTMake2(AST_TYPE_PARAMETER_TYPENAME, NULL, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TYPENAME identifier_token
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text);

	$$ = ASTMake2(AST_TYPE_PARAMETER_TYPENAME, identifier, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TYPENAME '=' type_id
{
	$$ = ASTMake2(AST_TYPE_PARAMETER_TYPENAME, NULL, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TYPENAME identifier_token '=' type_id
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@2.first_filename, @2.first_line, @2.first_column), $2.token_text);

	$$ = ASTMake2(AST_TYPE_PARAMETER_TYPENAME, identifier, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TEMPLATE '<' template_parameter_list '>' CLASS
{
	$$ = ASTMake3(AST_TYPE_PARAMETER_TEMPLATE, $3, NULL, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TEMPLATE '<' template_parameter_list '>' CLASS identifier_token
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@6.first_filename, @6.first_line, @6.first_column), $6.token_text);

	$$ = ASTMake3(AST_TYPE_PARAMETER_TEMPLATE, $3, identifier, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TEMPLATE '<' template_parameter_list '>' CLASS '=' id_expression
{
	$$ = ASTMake3(AST_TYPE_PARAMETER_TEMPLATE, $3, NULL, $7, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TEMPLATE '<' template_parameter_list '>' CLASS identifier_token '=' id_expression
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@6.first_filename, @6.first_line, @6.first_column), $6.token_text);

	$$ = ASTMake3(AST_TYPE_PARAMETER_TEMPLATE, $3, identifier, $8, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
// Template pack syntax
| CLASS ELLIPSIS
{
	$$ = ASTMake2(AST_TYPE_PARAMETER_CLASS_PACK, NULL, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| CLASS ELLIPSIS identifier_token
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@3.first_filename, @3.first_line, @3.first_column), $3.token_text);

	$$ = ASTMake2(AST_TYPE_PARAMETER_CLASS_PACK, identifier, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TYPENAME ELLIPSIS
{
	$$ = ASTMake2(AST_TYPE_PARAMETER_TYPENAME_PACK, NULL, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TYPENAME ELLIPSIS identifier_token
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@3.first_filename, @3.first_line, @3.first_column), $3.token_text);

	$$ = ASTMake2(AST_TYPE_PARAMETER_TYPENAME_PACK, identifier, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TEMPLATE '<' template_parameter_list '>' CLASS ELLIPSIS
{
	$$ = ASTMake3(AST_TYPE_PARAMETER_TEMPLATE_PACK, $3, NULL, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TEMPLATE '<' template_parameter_list '>' CLASS ELLIPSIS identifier_token
{
	AST identifier = ASTLeaf(AST_SYMBOL, make_locus(@7.first_filename, @7.first_line, @7.first_column), $7.token_text);

	$$ = ASTMake3(AST_TYPE_PARAMETER_TEMPLATE_PACK, $3, identifier, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

explicit_instantiation : TEMPLATE decl_specifier_seq_0 declarator ';' %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_EXPLICIT_INSTANTIATION, NULL, $2, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TEMPLATE decl_specifier_seq_ended_with_named_type_spec nonglobal_declarator ';' %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_EXPLICIT_INSTANTIATION, NULL, $2, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TEMPLATE decl_specifier_seq_0 ';' %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_EXPLICIT_INSTANTIATION, NULL, $2, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TEMPLATE decl_specifier_seq_ended_with_named_type_spec ';' %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_EXPLICIT_INSTANTIATION, NULL, $2, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TEMPLATE declarator ';' %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_EXPLICIT_INSTANTIATION, NULL, NULL, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
// GNU Extensions
| storage_class_specifier TEMPLATE decl_specifier_seq_0 declarator ';' %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_EXPLICIT_INSTANTIATION, $1, $3, $4, ast_get_locus($1), NULL);
}
| storage_class_specifier TEMPLATE decl_specifier_seq_ended_with_named_type_spec nonglobal_declarator ';' %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_EXPLICIT_INSTANTIATION, $1, $3, $4, ast_get_locus($1), NULL);
}
| storage_class_specifier TEMPLATE decl_specifier_seq_0 ';' %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_EXPLICIT_INSTANTIATION, $1, $3, NULL, ast_get_locus($1), NULL);
}
| storage_class_specifier TEMPLATE decl_specifier_seq_ended_with_named_type_spec ';' %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_EXPLICIT_INSTANTIATION, $1, $3, NULL, ast_get_locus($1), NULL);
}
| storage_class_specifier TEMPLATE declarator ';' %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_EXPLICIT_INSTANTIATION, $1, NULL, $3, ast_get_locus($1), NULL);
}
| function_specifier TEMPLATE decl_specifier_seq_0 declarator ';' %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_EXPLICIT_INSTANTIATION, $1, $3, $4, ast_get_locus($1), NULL);
}
| function_specifier TEMPLATE decl_specifier_seq_ended_with_named_type_spec nonglobal_declarator ';' %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_EXPLICIT_INSTANTIATION, $1, $3, $4, ast_get_locus($1), NULL);
}
| function_specifier TEMPLATE decl_specifier_seq_0 ';' %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_EXPLICIT_INSTANTIATION, $1, $3, NULL, ast_get_locus($1), NULL);
}
| function_specifier TEMPLATE decl_specifier_seq_ended_with_named_type_spec ';' %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_EXPLICIT_INSTANTIATION, $1, $3, NULL, ast_get_locus($1), NULL);
}
| function_specifier TEMPLATE declarator ';' %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_EXPLICIT_INSTANTIATION, $1, NULL, $3, ast_get_locus($1), NULL);
}
;

explicit_specialization : TEMPLATE '<' '>' templated_declaration
{
	$$ = ASTMake1(AST_EXPLICIT_SPECIALIZATION, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TEMPLATE '<' '>' explicit_specialization
{
	$$ = ASTMake1(AST_EXPLICIT_SPECIALIZATION, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
// This is invalid, all enclosed things must be explicitly specialized
// | TEMPLATE '<' '>' template_declaration
// {
// 	$$ = ASTMake1(AST_EXPLICIT_SPECIALIZATION, $4, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
// }
;


// These are the declarations that can follow a template preffix
// We give them known forms to ease semantic analysis
templated_declaration : decl_specifier_seq_0 init_declarator ';' %merge<ambiguityHandler>
{
	// Crafted form of simple declaration
    AST simple_decl_holder = ASTListLeaf($2);
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, $1, simple_decl_holder, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec nonglobal_init_declarator_first ';' %merge<ambiguityHandler>
{
	// Crafted form of simple declaration
    AST simple_decl_holder = ASTListLeaf($2);
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, $1, simple_decl_holder, ast_get_locus($1), NULL);
}
| init_declarator ';' %merge<ambiguityHandler>
{
    AST init_decl_holder = ASTListLeaf($1);
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, NULL, init_decl_holder, ast_get_locus($1), NULL);
}
| decl_specifier_seq_0 ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, $1, NULL, ast_get_locus($1), NULL);
}
| decl_specifier_seq_ended_with_named_type_spec ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, $1, NULL, ast_get_locus($1), NULL);
}
| function_definition %merge<ambiguityHandler>
{
	$$ = $1;
}
| template_declaration %merge<ambiguityHandler>
{
	$$ = $1;
}
| alias_declaration
{
    $$ = $1;
}
;

friend_templated_declaration : decl_specifier_seq_friend friend_declarator ';' %merge<ambiguityHandler>
{
	// Crafted form of simple declaration
    AST simple_decl_holder = ASTListLeaf($2);
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, $1, simple_decl_holder, ast_get_locus($1), NULL);
}
| decl_specifier_seq_friend_ended_with_named_type_spec nonglobal_friend_declarator_first ';' %merge<ambiguityHandler>
{
	// Crafted form of simple declaration
    AST simple_decl_holder = ASTListLeaf($2);
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, $1, simple_decl_holder, ast_get_locus($1), NULL);
}
| decl_specifier_seq_friend_alone ';' %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_SIMPLE_DECLARATION, $1, NULL, ast_get_locus($1), NULL);
}
| friend_function_definition %merge<ambiguityHandler>
{
	$$ = $1;
}
| template_friend_declaration %merge<ambiguityHandler>
{
    $$ = $1;
}
;


// *********************************************************
// A.12.1 - Template Expressions
// *********************************************************
// Template expressions have restrictions with > token at the outermost level

// Get back to the normal expressions
template_shift_expression : additive_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| template_shift_expression LEFT additive_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_BITWISE_SHL, $1, $3, ast_get_locus($1), NULL);
}
// Not here
| template_shift_expression RIGHT additive_expression
{
	$$ = ASTMake2(AST_SHR, $1, $3, ast_get_locus($1), NULL);
}
;

template_relational_expression : template_shift_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| template_relational_expression '<' template_shift_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_LOWER_THAN, $1, $3, ast_get_locus($1), NULL);
}
// Not valid here
// | template_relational_expression '>' shift_expression
// {
// }
| template_relational_expression GREATER_OR_EQUAL shift_expression
{
	$$ = ASTMake2(AST_GREATER_OR_EQUAL_THAN, $1, $3, ast_get_locus($1), NULL);
}
| template_relational_expression LESS_OR_EQUAL template_shift_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_LOWER_OR_EQUAL_THAN, $1, $3, ast_get_locus($1), NULL);
}
;

template_equality_expression : template_relational_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| template_equality_expression EQUAL template_relational_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_EQUAL, $1, $3, ast_get_locus($1), NULL);
}
| template_equality_expression NOT_EQUAL template_relational_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_DIFFERENT, $1, $3, ast_get_locus($1), NULL);
}
;

template_and_expression : template_equality_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| template_and_expression '&' template_equality_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_BITWISE_AND, $1, $3, ast_get_locus($1), NULL);
}
;

template_exclusive_or_expression : template_and_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| template_exclusive_or_expression '^' template_and_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_BITWISE_XOR, $1, $3, ast_get_locus($1), NULL);
}
;

template_inclusive_or_expression : template_exclusive_or_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| template_inclusive_or_expression '|' template_exclusive_or_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_BITWISE_OR, $1, $3, ast_get_locus($1), NULL);
}
;

template_logical_and_expression : template_inclusive_or_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| template_logical_and_expression ANDAND template_inclusive_or_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_LOGICAL_AND, $1, $3, ast_get_locus($1), NULL);
}
;

template_logical_or_expression : template_logical_and_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| template_logical_or_expression OROR template_logical_and_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_LOGICAL_OR, $1, $3, ast_get_locus($1), NULL);
}
;

template_conditional_expression : template_logical_or_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
| template_logical_or_expression '?' template_expression ':' template_assignment_expression %merge<ambiguityHandler>
{
	$$ = ASTMake3(AST_CONDITIONAL_EXPRESSION, $1, $3, $5, ast_get_locus($1), NULL);
}
// GNU Extension
| template_logical_or_expression '?' ':' template_assignment_expression %merge<ambiguityHandler>
{
	$$ = ASTMake2(AST_GCC_CONDITIONAL_EXPRESSION, $1, $4, ast_get_locus($1), NULL);
}
;

// The standard in C++2003 says that a nontype template argument can be an
// assignment-expression but it is clear that an assignment expression can not
// be a constant-expression. In C++2011 a nontype template argument must be a
// constant-expression which starts from conditional-expression (so an
// assignment expression is not constant)
template_assignment_expression : template_conditional_expression %merge<ambiguityHandler>
{
	$$ = $1;
}
// | template_logical_or_expression assignment_operator template_assignment_expression %merge<ambiguityHandler>
// {
// 	$$ = ASTMake2($2, $1, $3, ast_get_locus($1), NULL);
// }
// Extremely unlikely
// | template_throw_expression
// {
// }
;

template_expression : template_assignment_expression %merge<ambiguityHandler>
{
	$$ = ASTMake1(AST_EXPRESSION, $1, ast_get_locus($1), NULL);
}
| template_expression ',' template_assignment_expression %merge<ambiguityHandler>
{
	AST comma_expression = ASTMake2(AST_COMMA, $1, $3, ast_get_locus($1), NULL);

	$$ = ASTMake1(AST_EXPRESSION, comma_expression, ast_get_locus(comma_expression), NULL);
}
;


// *********************************************************
// A.13 - Exception handling
// *********************************************************

try_block : TRY compound_statement handler_seq
{
	$$ = ASTMake2(AST_TRY_BLOCK, $2, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

handler_seq : handler
{
	$$ = ASTListLeaf($1);
}
| handler_seq handler
{
	$$ = ASTList($1, $2);
}
;

handler : CATCH '(' exception_declaration ')' compound_statement
{
	$$ = ASTMake2(AST_CATCH_HANDLER, $3, $5, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

exception_declaration : type_specifier_seq_0 nonabstract_declarator  
{
	$$ = ASTMake2(AST_EXCEPTION_DECLARATION, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_seq_0 abstract_declarator
{
	$$ = ASTMake2(AST_EXCEPTION_DECLARATION, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_seq_0
{
	$$ = ASTMake2(AST_EXCEPTION_DECLARATION, $1, NULL, ast_get_locus($1), NULL);
}
|  type_specifier_seq_ended_with_identifier nonglobal_nonabstract_declarator  
{
	$$ = ASTMake2(AST_EXCEPTION_DECLARATION, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_seq_ended_with_identifier abstract_declarator
{
	$$ = ASTMake2(AST_EXCEPTION_DECLARATION, $1, $2, ast_get_locus($1), NULL);
}
| type_specifier_seq_ended_with_identifier
{
	$$ = ASTMake2(AST_EXCEPTION_DECLARATION, $1, NULL, ast_get_locus($1), NULL);
}
| ELLIPSIS
{
	$$ = ASTLeaf(AST_ANY_EXCEPTION, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

exception_specification : dynamic_exception_specification
{
    $$ = $1;
}
| noexcept_specification
{
    $$ = $1;
}
;


dynamic_exception_specification : THROW '(' ')' 
{
	$$ = ASTMake1(AST_EXCEPTION_SPECIFICATION, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| THROW '(' throw_type_id_list ')'
{
	$$ = ASTMake1(AST_EXCEPTION_SPECIFICATION, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

exception_specification_opt : /* empty */
{
    $$ = NULL;
}
| exception_specification
{
    $$ = $1;
}
;

throw_type_id_list : type_id
{
	$$ = ASTListLeaf($1);
}
| throw_type_id_list ',' type_id
{
	$$ = ASTList($1, $3);
}
;

noexcept_specification : TOKEN_NOEXCEPT_ALONE
{
    $$ = ASTMake1(AST_NOEXCEPT_SPECIFICATION, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| TOKEN_NOEXCEPT '(' constant_expression ')'
{
    $$ = ASTMake1(AST_NOEXCEPT_SPECIFICATION, $3, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;

throw_expression : THROW
{
	$$ = ASTMake1(AST_THROW_EXPRESSION, NULL, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
| THROW assignment_expression
{
	$$ = ASTMake1(AST_THROW_EXPRESSION, $2, make_locus(@1.first_filename, @1.first_line, @1.first_column), NULL);
}
;


identifier_token : IDENTIFIER
{
    $$ = $1;
}
// Keywords
| TOK_FINAL
{
    $$ = $1;
}
| TOK_OVERRIDE
{
    $$ = $1;
}
;


// *******************************************************
// * Crazy tokens implemented in grammar
// * We synthesize some fake tokens here
// *******************************************************

right_token : AB1 '>'
{
    // C++2011
    $$.token_text = UNIQUESTR_LITERAL(">>");
}
| RIGHT
{
    // C++2003
    $$ = $1;
}
;

right_assign_token : RIGHT_ASSIGN
{
    $$ = $1;
}
;

greater_equal_token : GREATER_OR_EQUAL
{
    $$ = $1;
}
;

right_bracket_token : '>'
{
    // C++2003 and C++2011
    $$.token_text = $1.token_text;
}
| AB1
{
    // C++2011
    $$.token_text = $1.token_text;
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


subparse_type_list : type_specifier_seq_0
{
    $$ = ASTListLeaf($1);
}
| type_specifier_seq_ended_with_identifier
{
    $$ = ASTListLeaf($1);
}
| subparse_type_list ',' type_specifier_seq_ended_with_identifier
{
    $$ = ASTList($1, $3);
}
| subparse_type_list ',' type_specifier_seq_0
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


unqualified_id_no_destructor : symbol_literal_ref
{
    $$ = $1;
}
;


nondeclarating_statement : nodecl_literal_stmt
{
    $$ = $1;
}
;





named_simple_type_specifier : type_literal_ref

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










type_specifier_SUSL : mercurium_extended_type_specifiers
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










omp_dr_typename : type_specifier_seq_0
{
    $$ = $1;
}
| type_specifier_seq_ended_with_identifier
{
    $$ = $1;
}
;


omp_dr_combiner : expression
{
    $$ = $1;
}
;





















/* Here there is the usual T(x) ambiguity. Easily solvable checking if T is omp_priv or not */
omp_dr_initializer : unqualified_name initializer %merge<ambiguityHandler>
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









// C++2011
cuda_kernel_config_right : AB1 AB1 '>'
{
    $$ = $1;
}
// C++2003
| CUDA_KERNEL_RIGHT
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
                    mcxxlloc.first_filename,
                    mcxxlloc.first_line,
                    mcxxlloc.first_column),
        "%s\n", c);
}
