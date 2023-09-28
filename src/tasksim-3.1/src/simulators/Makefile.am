
if ENABLE_NANOX
SUBDIRS = tasksim
endif

if !ONLY_TASKSIM
bin_PROGRAMS = sttmramsim/sttmramsim fastmemsim/fastmemsim helloworldsim memsim simplememsim
#bin_PROGRAMS = sttmramsim/sttmramsim fastmemsim/fastmemsim helloworldsim memsim simpleburstsim simplememsim
endif

frontendsim_frontendsim_SOURCES = frontendsim/frontendsim.cpp \
                                  frontendsim/runtime/FrontEnd.cpp
frontendsim_frontendsim_LDADD = \
	$(top_builddir)/src/core/libnextsim-engine.la \
	$(top_builddir)/src/core/libnextsim-cpu.la \
	$(top_builddir)/src/core/libnextsim-logic-memory.la \
	$(top_builddir)/src/core/libnextsim-utils.la

sttmramsim_sttmramsim_SOURCES = sttmramsim/sttmramsim.cpp \
                                sttmramsim/runtime/STTMRAM.cpp \
                                sttmramsim/trace/TraceReader.cpp
sttmramsim_sttmramsim_LDADD = \
	$(top_builddir)/src/core/libnextsim-engine.la \
	$(top_builddir)/src/core/libnextsim-cpu.la \
	$(top_builddir)/src/core/libnextsim-logic-memory.la \
	$(top_builddir)/src/core/libnextsim-utils.la

fastmemsim_fastmemsim_SOURCES = fastmemsim/fastmemsim.cpp \
                                fastmemsim/trace/TraceReader.cpp \
                                fastmemsim/runtime/FastMem.cpp
fastmemsim_fastmemsim_LDADD = \
	$(top_builddir)/src/core/libnextsim-engine.la \
	$(top_builddir)/src/core/libnextsim-cpu.la \
	$(top_builddir)/src/core/libnextsim-logic-memory.la \
	$(top_builddir)/src/core/libnextsim-utils.la

helloworldsim_SOURCES = examples/helloworldsim/helloworldsim.cpp
helloworldsim_LDADD = \
	$(top_builddir)/src/core/libnextsim-engine.la \
	$(top_builddir)/src/core/libnextsim-utils.la

memsim_SOURCES = examples/memsim/memsim.cpp \
                 examples/memsim/runtime/SimpleMemory.cpp
memsim_LDADD = \
	$(top_builddir)/src/core/libnextsim-engine.la \
	$(top_builddir)/src/core/libnextsim-cpu.la \
	$(top_builddir)/src/core/libnextsim-logic-memory.la \
	$(top_builddir)/src/core/libnextsim-utils.la

simpleburstsim_SOURCES = examples/simpleburst/simpleburst.cpp \
                         examples/simpleburst/runtime/OmpSs_test.cpp
simpleburstsim_LDADD = \
    $(top_builddir)/src/core/libnextsim-engine.la \
    $(top_builddir)/src/core/libnextsim-cpu.la \
    $(top_builddir)/src/core/libnextsim-stats.la \
	$(top_builddir)/src/core/libnextsim-utils.la

simplememsim_SOURCES = examples/simplememsim/simplememsim.cpp \
                       examples/simplememsim/runtime/SingleInst.cpp
simplememsim_LDADD = \
	$(top_builddir)/src/core/libnextsim-engine.la \
	$(top_builddir)/src/core/libnextsim-cpu.la \
	$(top_builddir)/src/core/libnextsim-logic-memory.la \
	$(top_builddir)/src/core/libnextsim-utils.la
