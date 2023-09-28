if ENABLE_NANOX

# SMP:
noinst_LTLIBRARIES = libnextsim-runtime-tasksim-smp.la

nanox_ldflags =
# Use debug version of NANOX, if debug mode of Nextsim enabled
if ENABLE_DEBUG
nanox_ldflags += -L$(nanox_lib_path)/debug -Wl,-rpath,$(nanox_lib_path)/debug -lnanox-c
else
nanox_ldflags += -L$(nanox_lib_path)/performance -Wl,-rpath,$(nanox_lib_path)/performance -lnanox-c
endif  # ENABLE_DEBUG

libnextsim_runtime_tasksim_smp_la_SOURCES = \
    smp/SimSmpDD.cpp \
    smp/SimSmpProcessor.cpp \
    smp/SimSmpThread.cpp \
    smp/SimSmpPlugin.cpp \
    $(END)
libnextsim_runtime_tasksim_smp_la_CXXFLAGS = $(AM_CXXFLAGS) -DSMP_SUPPORTS_ULT

libnextsim_runtime_tasksim_smp_la_LDFLAGS = $(nanox_ldflags)
libnextsim_runtime_tasksim_smp_la_LIBADD = -lnanox-c -lnanox-ompss -lnanox-pe-smp \
    $(top_builddir)/src/core/libnextsim-utils.la


# Mode Selector Policies:
noinst_LTLIBRARIES += libnextsim-modeselectorpolicies.la
libnextsim_modeselectorpolicies_la_SOURCES = \
    mode_selector_policies/BurstOnly.cpp  \
    mode_selector_policies/Memory.cpp \
    mode_selector_policies/SampleHistory.cpp \
    mode_selector_policies/TaskPoint.cpp \
    $(END)
libnextsim_modeselectorpolicies_la_LIBADD = \
    $(top_builddir)/src/core/libnextsim-utils.la

# Runtime
noinst_LTLIBRARIES += libnextsim-runtime-tasksim.la libnextsim-runtime-tasksim-testing.la

libnextsim_runtime_tasksim_ladir = $(includedir)/nextsim/simulators/tasksim/runtime
nobase_libnextsim_runtime_tasksim_la_HEADERS = \
    types.h

libnextsim_runtime_tasksim_la_CXXFLAGS = $(AM_CXXFLAGS) -DSMP_SUPPORTS_ULT
libnextsim_runtime_tasksim_la_SOURCES = \
    OmpSs.cpp \
    ModeSelector.cpp \
    NanoxBridge.cpp \
    RuntimeThread.cpp
libnextsim_runtime_tasksim_la_LIBADD = \
    libnextsim-runtime-tasksim-smp.la \
    libnextsim-modeselectorpolicies.la \
    $(top_builddir)/src/core/libnextsim-cpu.la \
    $(top_builddir)/src/core/libnextsim-utils.la

libnextsim_runtime_tasksim_testing_la_CXXFLAGS = $(AM_CXXFLAGS) -DTESTING_VERSION -DSMP_SUPPORTS_ULT
libnextsim_runtime_tasksim_testing_la_SOURCES = \
    OmpSs.cpp \
    ModeSelector.cpp \
    NanoxBridge.cpp \
    RuntimeThread.cpp
libnextsim_runtime_tasksim_testing_la_LIBADD = \
    libnextsim-runtime-tasksim-smp.la \
    libnextsim-modeselectorpolicies.la \
    $(top_builddir)/src/core/libnextsim-cpu.la \
    $(top_builddir)/src/core/libnextsim-utils.la
endif  # ENABLE_NANOX
