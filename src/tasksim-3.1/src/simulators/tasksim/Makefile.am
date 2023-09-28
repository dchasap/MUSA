if ONLY_TASKSIM
bin_PROGRAMS = tasksim
else
bin_PROGRAMS = tasksim burstsim testing tasksim_arm tasksim_sumway
endif

tasksim_SOURCES = tasksim.cpp
tasksim_CPPFLAGS = $(AM_CPPFLAGS)
tasksim_LDADD = \
	$(top_builddir)/src/core/libnextsim-engine.la \
	$(top_builddir)/src/core/libnextsim-cpu.la \
	$(top_builddir)/src/core/libnextsim-stats.la \
	$(top_builddir)/src/core/libnextsim-utils.la \
	$(top_builddir)/src/core/libnextsim-logic-memory.la \
	$(top_builddir)/src/simulators/tasksim/runtime/libnextsim-runtime-tasksim.la

if ENABLE_RAMULATOR
tasksim_LDADD += $(ramulator_lib_path)/libramulator.a
endif

tasksim_arm_SOURCES = tasksim_arm.cpp
tasksim_arm_CPPFLAGS = $(AM_CPPFLAGS)
tasksim_arm_LDADD = $(tasksim_LDADD)

tasksim_sumway_SOURCES = tasksim_sumway.cpp
tasksim_sumway_CPPFLAGS = $(AM_CPPFLAGS)
tasksim_sumway_LDADD = $(tasksim_LDADD)

burstsim_SOURCES = burstsim.cpp
burstsim_CPPFLAGS = $(AM_CPPFLAGS)
burstsim_LDADD = $(tasksim_LDADD)

testing_SOURCES = tasksim.cpp
testing_CPPFLAGS = $(AM_CPPFLAGS)
testing_CXXFLAGS = -DTESTING_VERSION $(AM_CXXFLAGS)
testing_LDADD = $(tasksim_LDADD) 

SUBDIRS = runtime
