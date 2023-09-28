/* Autogenerated file. DO NOT MODIFY. */
/* Changes in nodecl-generator.py or cxx-nodecl.def will overwrite this file */

#ifndef CXX_NODECL_VISITOR_H
#define CXX_NODECL_VISITOR_H

#include "cxx-macros.h"
#include "cxx-nodecl-decls.h"

MCXX_BEGIN_DECLS

struct nodecl_external_visitor_tag;
typedef struct nodecl_external_visitor_tag nodecl_external_visitor_t;

struct nodecl_external_visitor_tag
{

void (*visit_omp_ss_cost)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_dep_new)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_copy_in)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_first_lastprivate)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_checkpoint_store)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_gcc_asm_definition)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_capture_copy)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_member_init)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_master)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_mask_conversion)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_div)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_copy_out)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_dep_in_private)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_deallocate_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_upc_sync_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_alignof)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_live_in)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_function_form_unary_postfix)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_postincrement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_dep_template_id)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_private_init)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_fmminus)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_nontemporal_flag)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_flush_at_entry)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_intel_assume_aligned)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_dep_name_simple)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_parenthesized_initializer)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_computed_goto_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_range_union)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_object_init)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_align_right)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_dep_inout)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_lower_or_equal_than)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_div)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_reduction_item)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_pause_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_function_task_parsing_context)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_dep_weak_inout)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_bitwise_shr_assignment)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_gcc_builtin_va_arg)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_num_teams)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_correctness_incoherent_out_pointed)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_compound_expression)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_if)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_checkpoint_data)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_compound_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_neg)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_relaxed_flag)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_reduction_mul)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_new)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_simd_reduction)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_sizeof)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_range_discrete)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_add_assignment)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_fmadd)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_motion_from)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_explicit_instantiation_def)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_lane_id)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_virtual_function_call)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_arithmetic_if_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_auto_scope_private)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_overlap)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_unbounded_loop_control)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_nontemporal)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_div_assignment)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_dep_weak_reduction)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_bind_c)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_bitwise_and)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_iterator_loop_control)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_loop)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_if_else_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_greater_or_equal_than)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_checkpoint_kind)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_lower_than)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_catch_handler)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_c99_index_designator)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_default_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_reduction_add)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_type)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_dep_commutative)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_unroll)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_logical_and)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_has_been_defined_flag)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_reduction)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_mask_and)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_task_is_loop)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_range_intersection)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_task_label)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_task_is_taskwait)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cuda_kernel_call)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_store)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_dep_name_conversion)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_linear)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_chunksize)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_implements)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_function_form_default_init_braced)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_logical_or)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_dep_weak_in)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_gxx_builtin_addressof)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_task_expression)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_no_prefetch)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_hollerith)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_predecrement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_return_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_task_call)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_err_expr)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_actual_argument)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_in_reduction)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_structured_value_braced_typecast)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_checkpoint_comm)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_mask_and_1_not)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_integer_literal)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_pragma_custom_line)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_simd_for)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_dep_function_call)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_alloca)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_name)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_conditional_expression)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_value_pack)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_checkpoint_shutdown)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_err_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_boz_literal)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_parse_later)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_function_form_unary_prefix)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_atomic)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_boolean_literal)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_correctness_incoherent_fp)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_comma)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_register)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_stop_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_sizeof_pack)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_taskwait)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_barrier_wait)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_field_designator)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_mul_assignment)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_bitwise_shl_assignment)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_gcc_asm_operand)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_nullify_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_sections)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_for_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_grainsize)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_untied)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_floating_literal)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_critical)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_target_data)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_minus_assignment)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_cast)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_fabs)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_priority)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_task)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_add)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_prefetch)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_bitwise_shl)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_barrier_signal)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_defined)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_range_loop_control)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_goto_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_pragma_clause_arg)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_bitwise_shr)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_alignas)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_evict_flag)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_pragma_custom_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_correctness_dead)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_using_decl)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_scatter)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_weak_reduction)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_read_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_arrow)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_capture_reference)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_allocate_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_section)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_case_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_for_ranged)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_class_member_access)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_initializer)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_top_level)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_close_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_declare_target)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_braced_initializer)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_loop)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_auto_scope_firstprivate)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_checkpoint_init)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_n_d_range)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_for_appendix)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_structured_value_compound_literal)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_simd_function)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_gxx_trait)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_do_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_upper_exposed)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_gather)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_unknown)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_correctness_auto_storage)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_lower_or_equal_than)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_delete_array)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_taskloop)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_assert)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_bitwise_not)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_empty_range)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_device)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_barrier_full)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_range_sub)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_c99_designated_initializer)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_logical_or)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_simd)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_noexcept)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_vector_length)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_alternate_return_argument)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_bitwise_and_assignment)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_def)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_checkpoint_level)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_minimum)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_array_subscript)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_postdecrement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_io_spec)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_mask_and_2_not)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_minus)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_parallel_simd_for)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_schedule)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_threadprivate)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_loop_control)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_pragma_context)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vla_wildcard)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_preprocessor_line)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_task_is_task_call)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_conversion)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_arithmetic_shr_assignment)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_taskgroup)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_target)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_preincrement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_arrow_ptr_member)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_wait)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_aligned_flag)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_task_reduction)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_function_code)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_imag_part)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_function_form_default_init)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_alignof)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_uniform)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_map_to)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_parenthesized_expression)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_dummy)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_greater_or_equal_than)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_logical_not)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_auto_scope_shared)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_structured_value_fortran_typespec_array_constructor)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_private)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_mul)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_mask_or)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_lower_than)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_literal)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_shared)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_text)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_dep_global_name_nested)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_data)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_on_top_flag)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_dep_reduction)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_dep_weak_out)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_bitwise_xor_assignment)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_induction_var_expr)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_while_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_try_block)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_print_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_dep_in)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_shaping)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_pragma_custom_clause)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_shared_and_alloca)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_using_namespace)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_sh_mem)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_real_part)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_complex_literal)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_greater_than)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_different)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_live_out)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_reference)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_dot_ptr_member)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_single)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_use_only)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_break_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_function_code)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_mod)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_mask_xor)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_unregister)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_pseudo_destructor_name)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_prefetch)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_flush_memory)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_mul)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_mod_assignment)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_pragma_custom_directive)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_mask_not)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_phi)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_dep_concurrent)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_string_literal)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_not_present)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_distribute)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_load)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_dist_schedule)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_equal)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_checkpoint_load)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_dep_decltype)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_context)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_onto)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_postfix_initializer)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_lastprivate)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_auto)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_bitwise_or)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_num_tasks)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_in_place_flag)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_firstprivate)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_cast)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_asm_definition)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_implicit_instantiation)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_copy_inout)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_barrier_at_end)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_correctness_incoherent_in_pointed)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_alignment_info)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_undefined)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_minus_infinity)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_function_call)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_neg)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_maximum)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_mask_assignment)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_dep_out)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_io_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_rsqrt)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_correctness_incoherent_p)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_structured_value_braced_implicit)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_induction_variable)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_bitwise_xor)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_different)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_combined_with_parallel)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_minus)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_function_form_implicit)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_equivalence)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_logical_not)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_vector_length_for)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_taskyield)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_bitwise_shr)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_assignment)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_dereference)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_equal_initializer)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_mask)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_plus_infinity)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_bitwise_shl)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_target)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_sqrt)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_subscript)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_motion_to)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_rcp)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_entry_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_for)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_write_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_explicit_instantiation_decl)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_decl)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_expression_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_noexcept_implicit_true)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_checkpoint_if)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_checkpoint_id)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_correctness_incoherent_out)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_open_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_where_pair)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_equal)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_final)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_pointer_to_member)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_map_to_from)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_add)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_arithmetic_shr)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_range)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_source_comment)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_map_from)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_dep_name_nested)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_flush_at_exit)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_c99_field_designator)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_assigned_goto_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_function_form_template_id)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_mod)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_bitwise_not)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_gcc_asm_spec)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_default_argument)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_labeled_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_bitwise_xor)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_mergeable)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_file)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_reach_def_expr)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_release)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_where)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_function_call)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_structured_value)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_template_function_code)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_unknown_pragma)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_logical_and)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_multi_expression_iterator)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_index_designator)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_sizeof)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_switch_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_intel_assume)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_map_alloc)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_promotion)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_multi_expression)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_member_init)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_target_update)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_continue_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_symbol)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_power)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_bitwise_and)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_sincos)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_offset)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_throw)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_implicit_member_init)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_static_assert)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_range)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_plus)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_conversion)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_reaching_definition_in)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_alternate_return_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_explicit_type_cast)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_thread_limit)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_assignment)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_reaching_definition_out)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_use)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_correctness_incoherent_in)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_parallel)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_no_mask)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_label_assign_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_no_flush)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_verbatim)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_class_member_access)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_workshare)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_critical_name)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_mask_literal)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_lambda)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_dead)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_conditional_expression)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_empty_statement)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_suitable)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_dep_weak_commutative)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_teams)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_structured_value_parenthesized)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_delete)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_greater_than)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_value_pack_expanded)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_target_task_undeferred)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_bitwise_or)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_cxx_function_form_binary_infix)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_implied_do)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_bitwise_or_assignment)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_assert_decl)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_omp_ss_target_declaration)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_aligned)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_fortran_forall)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_concat)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_typeid)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_analysis_correctness_race)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_arithmetic_shr)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_vector_reduction_minus)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_pragma_custom_declaration)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_open_m_p_unroll_and_jam)(nodecl_external_visitor_t*, nodecl_t a);
void (*visit_offsetof)(nodecl_external_visitor_t*, nodecl_t a);

};

void nodecl_walk(nodecl_external_visitor_t* external_visitor, nodecl_t node);
void nodecl_init_walker(nodecl_external_visitor_t* external_visitor, void (*default_visit)(nodecl_external_visitor_t*, nodecl_t));

#define NODECL_VISITOR_FUN(_x) (void (*)(nodecl_external_visitor_t* visitor, nodecl_t a))(_x)
#define NODECL_VISITOR(_x) ((nodecl_external_visitor_t*)(_x))
#define NODECL_WALK(v, tree) nodecl_walk(NODECL_VISITOR(v), tree)

MCXX_END_DECLS

#endif // CXX_NODECL_VISITOR_H
