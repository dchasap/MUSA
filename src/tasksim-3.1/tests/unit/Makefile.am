bin_PROGRAMS = unit_tests
unit_tests_SOURCES = \
	main.cpp \
	interconnect/Direct.cpp \
	memory/Cache.cpp \
	memory/CacheSet.cpp \
	memory/DRAM.cpp \
	coherence/Single.cpp \
	memory/MSHR.cpp \
    cpu/ROB.cpp \
    cpu/ROA.cpp \
    cpu/ROA_alt.cpp \
	trace/InstrQueue.cpp \
    utils/RangeSet.cpp

if ENABLE_NANOX
unit_tests_SOURCES += \
	trace/OmpSsTrace.cpp \
	trace/TsTrace.cpp
endif

unit_tests_CPPFLAGS = $(AM_CPPFLAGS) -I@GTEST_DIR@/include -I$(abs_srcdir)
unit_tests_LDADD = \
	$(top_builddir)/src/core/libnextsim-engine.la \
	$(top_builddir)/src/core/libnextsim-cpu.la \
	$(top_builddir)/src/core/libnextsim-logic-memory.la \
	$(top_builddir)/src/core/libnextsim-stats.la \
	$(top_builddir)/src/core/libnextsim-utils.la \
	$(top_builddir)/ext/libgtest.la -lpthread
