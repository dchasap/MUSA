lib_LTLIBRARIES =
bin_PROGRAMS =
noinst_LTLIBRARIES = libnextsim-stats.la \
                     libnextsim-engine.la \
                     libnextsim-cpu.la \
                     libnextsim-logic-memory.la \
                     libnextsim-utils.la
# Stats:
libnextsim_stats_la_SOURCES = stats/Stat.cpp \
                              stats/TimeStatistics.cpp

# Engine:
libnextsim_engine_ladir = $(includedir)/nextsim/core/engine
nobase_libnextsim_engine_la_HEADERS = engine/types.h

libnextsim_engine_la_SOURCES = engine/Config.cpp \
                               engine/Simulator.cpp
libnextsim_engine_la_LIBADD = libnextsim-stats.la

# Modules:
libnextsim_cpu_la_SOURCES = modules/cpu/Burst.cpp \
                            modules/cpu/models/ROA.cpp

# Logic:
libnextsim_logic_memory_la_SOURCES = logic/memory/DRAM.cpp \
                                     logic/memory/policy/Policy.cpp \
                                     logic/memory/policy/types.cpp

# Utils:
libnextsim_utils_la_SOURCES = utils/GzBuffer.cpp \
                              utils/CheckTrace.cpp \
                              utils/Log.cpp \
                              $(nobase_libnextsim_utils_la_HEADERS)
libnextsim_utils_ladir = $(includedir)/nextsim/core/utils
nobase_libnextsim_utils_la_HEADERS = utils/Log.h \
                                     utils/macros.h \
                                     utils/ReusableObject.h
libnextsim_utils_la_LIBADD = -lz


# Utils:
bin_PROGRAMS += tasksim_presim
tasksim_presim_SOURCES = utils/tsmpi/tasksim_presim.cpp

if ENABLE_MPI
bin_PROGRAMS += tasksim_presim_filter tasksim_presim_sorter
tasksim_presim_filter_SOURCES = utils/tsmpi/tasksim_presim_filter.c
tasksim_presim_sorter_SOURCES = utils/tsmpi/tasksim_presim_sorter.cpp

lib_LTLIBRARIES += utils/tsmpi/libtsmpi.la
utils_tsmpi_libtsmpi_la_SOURCES = utils/tsmpi/libtsmpi.c
utils_tsmpi_libtsmpi_la_CFLAGS=-D_GNU_SOURCE -D_TS_PATH=\"$(prefix)\" -fPIC -I$(mpi_install_path)/include
#utils_tsmpi_libtsmpi_la_LDFLAGS=-lz -fPIC -L$(mpi_lib_path) -lmpi -lmpi_cxx -ldl
utils_tsmpi_libtsmpi_la_LDFLAGS=-lz -fPIC -L$(mpi_lib_path) -lmpi -ldl

endif  # ENABLE_MPI

# DynamoRIO Tool:

if HAVE_DR
if ENABLE_NANOX

lib_LTLIBRARIES += libompss-drtrace.la

libompss_drtrace_la_CPPFLAGS = $(AM_CPPFLAGS) \
							@DR_CPPFLAGS@ -I@PTLSIM_DIR@ \
							-I$(nanox_src_path)/src/core \
							-I$(nanox_src_path)/src/support
libompss_drtrace_la_CXXFLAGS = $(AM_CXXFLAGS) @DR_CFLAGS@ @DR_CPPFLAGS@ @PTLSIM_CXXFLAGS@ -fexceptions
libompss_drtrace_la_LDFLAGS = @DR_LDFLAGS@ -Wl,--no-undefined
libompss_drtrace_la_LIBADD = @DR_LIBRARIES@ -lz libnextsim-utils.la

if ENABLE_PTLSIM
libompss_drtrace_la_LIBADD += $(top_builddir)/ext/libptlsim.la
endif  # ENABLE_PTLSIM

libompss_drtrace_la_SOURCES = trace/dr/dynamoRIOTool.cpp
endif  # ENABLE_NANOX

endif  # HAVE_DR


# Tools:

bin_PROGRAMS += clean_drtrace
clean_drtrace_SOURCES = tools/clean_drtrace.cpp
clean_drtrace_LDADD  = libnextsim-utils.la

#if ENABLE_NANOX

bin_PROGRAMS += ompss ompss_dr mem_analyzer trace_to_csvtdg trace_convert paraver_intermediate_to_txt trace_compare trace_compare_info trace_compare_new instr_breakdown instr_breakdown_musa wd_info ipc_histogram task_data merge_sve_memtrace trace_sve dr_nonanos merge_ompss_dr_traces

if ENABLE_MPI

bin_PROGRAMS +=ompss_mpi_merge ompss_mpi_perf ompss_mpi_mem ompss_mpi_dr_mem ompss_mpi_merge_mw mpi_merge_sve mpi_trace_sve

ompss_mpi_merge_SOURCES = tools/ompss_mpi_merge.cpp \
                          tools/ompss_util.cpp
ompss_mpi_merge_CPPFLAGS = -D_TS_PATH=\"$(prefix)\" $(AM_CPPFLAGS)
ompss_mpi_merge_LDADD = libnextsim-utils.la

ompss_mpi_perf_SOURCES = tools/ompss_mpi_perf.cpp
ompss_mpi_perf_CPPFLAGS = -D_TS_PATH=\"$(prefix)\" $(AM_CPPFLAGS)
ompss_mpi_perf_LDADD = libnextsim-utils.la
ompss_mpi_mem_SOURCES = tools/ompss_mpi_mem.cpp
ompss_mpi_mem_CPPFLAGS = -D_TS_PATH=\"$(prefix)\" -I$(mpi_install_path)/include $(AM_CPPFLAGS)
#ompss_mpi_mem_LDFLAGS = -L$(mpi_lib_path) -lmpi -lmpi_cxx $(AM_LDFLAGS)
ompss_mpi_mem_LDFLAGS = -L$(mpi_lib_path) -lmpi $(AM_LDFLAGS)
ompss_mpi_mem_LDADD = libnextsim-utils.la

ompss_mpi_dr_mem_SOURCES = tools/ompss_mpi_dr_mem.cpp
ompss_mpi_dr_mem_CPPFLAGS = -D_TS_PATH=\"$(prefix)\" -I$(mpi_install_path)/include $(AM_CPPFLAGS)
ompss_mpi_dr_mem_LDFLAGS = $(ompss_mpi_mem_LDFLAGS)
ompss_mpi_dr_mem_LDADD = libnextsim-utils.la

ompss_mpi_merge_mw_SOURCES = tools/ompss_mpi_merge.cpp \
                             tools/ompss_util.cpp
ompss_mpi_merge_mw_CPPFLAGS = -D_TS_PATH=\"$(prefix)\" -DENABLE_MPI -I$(mpi_install_path)/include $(AM_CPPFLAGS)
ompss_mpi_merge_mw_LDFLAGS =  $(ompss_mpi_mem_LDFLAGS)
ompss_mpi_merge_mw_LDADD = libnextsim-utils.la

mpi_trace_sve_SOURCES = tools/mpi_trace_sve.cpp
mpi_trace_sve_CPPFLAGS = -D_TS_PATH=\"$(prefix)\" -I$(mpi_install_path)/include $(AM_CPPFLAGS)
mpi_trace_sve_LDFLAGS =  $(ompss_mpi_mem_LDFLAGS)
mpi_trace_sve_LDADD = libnextsim-utils.la

mpi_merge_sve_SOURCES = tools/mpi_merge_sve.cpp \
                        tools/ompss_util.cpp
mpi_merge_sve_CPPFLAGS = -D_TS_PATH=\"$(prefix)\" -I$(mpi_install_path)/include $(AM_CPPFLAGS)
mpi_merge_sve_LDFLAGS =  $(ompss_mpi_mem_LDFLAGS)
mpi_merge_sve_LDADD = libnextsim-utils.la

endif  # ENABLE_MPI

ompss_SOURCES = tools/ompss_util.cpp \
                tools/ompss.cpp
ompss_LDADD  = libnextsim-utils.la

ompss_dr_SOURCES = tools/ompss_util.cpp tools/ompss_dr.cpp
ompss_dr_LDADD  = libnextsim-utils.la

instr_breakdown_SOURCES = tools/instr_breakdown.cpp \
                          tools/instr_breakdown_utils.cpp
instr_breakdown_LDADD = libnextsim-utils.la

instr_breakdown_musa_SOURCES = tools/instr_breakdown_musa.cpp \
                               tools/instr_breakdown_utils.cpp
instr_breakdown_musa_LDADD = libnextsim-utils.la

wd_info_SOURCES = tools/wd_info.cpp
wd_info_LDADD = libnextsim-utils.la

task_data_SOURCES = tools/task_data.cpp
task_data_LDADD = libnextsim-utils.la

ipc_histogram_SOURCES = tools/ipc_histogram.cpp
ipc_histogram_LDADD = libnextsim-utils.la

mem_analyzer_SOURCES = tools/mem_analyzer.cpp
mem_analyzer_LDADD = libnextsim-utils.la

trace_to_csvtdg_SOURCES = tools/trace_to_csvtdg.cpp
trace_to_csvtdg_LDADD = libnextsim-utils.la

paraver_intermediate_to_txt_SOURCES = tools/paraver_intermediate_to_txt.cpp
paraver_intermediate_to_txt_LDADD  = libnextsim-utils.la

trace_compare_SOURCES = tools/trace_compare.cpp
trace_compare_LDADD = libnextsim-utils.la

trace_compare_new_SOURCES = tools/trace_compare_new.cpp
trace_compare_new_LDADD = libnextsim-utils.la

trace_compare_info_SOURCES = tools/trace_compare_info.cpp
trace_compare_info_LDADD = libnextsim-utils.la

trace_convert_SOURCES = tools/trace_conversion_config.cpp \
                        tools/trace_convert.cpp
trace_convert_LDADD = libnextsim-utils.la

merge_sve_memtrace_SOURCES =  tools/merge_sve_memtrace.cpp
merge_sve_memtrace_LDADD = libnextsim-utils.la

trace_sve_SOURCES =  tools/trace_sve.cpp
trace_sve_LDADD = libnextsim-utils.la

dr_nonanos_SOURCES = tools/dr_nonanos.cpp \
                     tools/ompss_util.cpp
dr_nonanos_LDADD = libnextsim-utils.la

merge_ompss_dr_traces_SOURCES = tools/merge_ompss_dr_traces.cpp \
                     tools/ompss_util.cpp
merge_ompss_dr_traces_LDADD = libnextsim-utils.la

nobase_data_DATA = tools/power_consumption/README \
                 tools/power_consumption/tasksim-mcpat.py \
                 tools/power_consumption/Paraver/mcpat-paraver.sh \
                 tools/power_consumption/Paraver/cfgs/L1Dhits.cfg \
                 tools/power_consumption/Paraver/cfgs/L1Dmisses.cfg \
                 tools/power_consumption/Paraver/cfgs/L1DTotal.cfg \
                 tools/power_consumption/Paraver/cfgs/L2hits.cfg \
                 tools/power_consumption/Paraver/cfgs/L2misses.cfg \
                 tools/power_consumption/Paraver/cfgs/L2Total.cfg \
                 tools/power_consumption/Paraver/cfgs/tasksim_function_name.cfg \
                 tools/power_consumption/Paraver/cfgs/tasksim_nanos.cfg \
                 tools/power_consumption/Paraver/cfgs/Total.cfg \
                 tools/power_consumption/templates/mcpat-input.xml
#endif  # ENABLE_NANOX
