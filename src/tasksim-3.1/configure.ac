AC_INIT([TaskSim Simulation Infrastructure], [12.1],
        [tasksim@bsc.es], [tasksim])

cr_header="Copyright"
cr_name1="Barcelona Supercomputing Center"
cr_name2="Universitat Politecnica de Catalunya"
srcdir=`dirname $0`
git_hash=`git --git-dir="$srcdir/.git" rev-parse --short HEAD`
cr_revision="GIT Hash: ${git_hash}"
AC_DEFINE_UNQUOTED([PACKAGE_COPYRIGHT],
                   ["${cr_header} ${cr_name1}\\n${cr_header} ${cr_name2}\\n${cr_revision}"],
                   [Define to the copyright string for this package.])

gtest_dir="ext/gtest-1.6.0"
ptlsim_dir="ext/ptlsim-20090314-r229"

AC_PREREQ([2.60])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([subdir-objects])
AM_SILENT_RULES([yes])
#LT_PREREQ([2.4.6])
LT_INIT
AM_PROG_AS

AM_PROG_CC_C_O

dnl Check for debug mode
AC_MSG_CHECKING([whether to build with debug information])
AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--enable-debug],
    	[enable debug data generation (default=no)])],
	[debugit="$enableval"],
	[debugit=no])
AC_MSG_RESULT([$debugit])

dnl Check for asserts without debug mode
AC_MSG_CHECKING([whether to build with asserts])
AC_ARG_ENABLE([asserts],
    [AS_HELP_STRING([--enable-asserts],
        [Enable Asserts even without debug mode (default = no)])],
    [asserts="$enableval"],
    [asserts=no])

dnl Check for TaskSim system log
AC_MSG_CHECKING([whether to build with verbose logging])
AC_ARG_ENABLE([simlog],
    AS_HELP_STRING([--enable-simlog],
        [Deprecated option to enable message logging]),
    [AC_MSG_ERROR([\n--enable-simlog is deprecated, now it is always on, remove the flag please.])])

dnl Check for performance profiling mode
AC_MSG_CHECKING([whether to enable performance profiling])
AC_ARG_ENABLE([profiling],
        [AS_HELP_STRING([--enable-profiling],
        [enable performance profiling (default=no)])],
        [profileit="$enableval"],
        [profileit=no])
AC_MSG_RESULT([$profileit])

AC_ARG_WITH([simlog-level],
        AS_HELP_STRING([--with-simlog-level], [Sets the logging level]),
        [AC_DEFINE_UNQUOTED(SIMLOG_LEVEL, [$withval]) simlog_level=$withval],
        [AC_DEFINE(SIMLOG_LEVEL, 1) simlog_level=1])

#if test x"$debugit" = x"yes"; then
#  global_cxxflags="$CXXFLAGS -g -Wall -Wno-uninitialized -O0 -lz"
#else
#  if test x"$asserts" = x"yes"; then
#    global_cxxflags="$CXXFLAGS -Wall -Wno-uninitialized -O3 -lz"
#  else
#    global_cxxflags="$CXXFLAGS -Wall -Wno-uninitialized -O3 -DNDEBUG -lz"
#  fi
#fi

if test x"$debugit" = x"yes"; then
  global_cxxflags="$CXXFLAGS -g -Wno-uninitialized -O0 -lz"
else
  if test x"$asserts" = x"yes"; then
    global_cxxflags="$CXXFLAGS -Wno-uninitialized -O3 -lz"
  else
    global_cxxflags="$CXXFLAGS -Wno-uninitialized -O3 -DNDEBUG -lz"
  fi
fi

if test x"$profileit" = x"yes"; then
  global_cxxflags="${global_cxxflags} -Wl,--no-as-needed,-lprofiler,--as-needed"
fi


AC_SUBST([AM_LDFLAGS], ["-lz"])
AM_CONDITIONAL([ENABLE_DEBUG], [test "x$debugit" = "xyes"])

dnl Check for C++ compiler
AC_LANG([C++])
AC_PROG_CXX
AX_CHECK_COMPILE_FLAG([-std=c++11],
                      [global_cxxflags="${global_cxxflags} -std=c++11"],
                      [AC_MSG_ERROR("C++ compiler does not support C++11. And we need it.",-1)])

AC_CHECK_PIN

AC_CHECK_DR

dnl Check for Nanox support
AC_MSG_CHECKING([whether NANOS++ integration is enabled])
AC_ARG_ENABLE([nanox],
    [AS_HELP_STRING([--enable-nanox],
        [Enable NANOS++ integration (default = no)])],
    [enable_nanox="$enableval"],
    [enable_nanox=no])
AC_MSG_RESULT([$enable_nanox])
if test x"$enable_nanox" = x"yes"; then
AC_ARG_WITH([nanox-src-path],
	AS_HELP_STRING([--with-nanox-src-path], [Sets the path to the Nanos++ sources]),
        nanox_src_path="$withval",
        AC_MSG_ERROR("Nanos++ activated but flag --with-nanox-src-path not supplied."))
	AC_ARG_WITH([nanox-install-path],
	AS_HELP_STRING([--with-nanox-install-path], [Sets the installation path for Nanos++/Mercurium]),
        nanox_install_path="$withval",
        AC_MSG_ERROR("Nanos++ activated but flag --with-nanox-install-path not supplied."))
fi
AM_CONDITIONAL([ENABLE_NANOX], [test "x$enable_nanox" = "xyes"])
AC_SUBST(nanox_src_path, ["$nanox_src_path"])
AC_SUBST(nanox_install_path, ["$nanox_install_path"])

dnl find right library directory
if test "$libdir" = '${exec_prefix}/lib' ; then
  AC_SUBST(nanox_lib_path, ["$nanox_install_path/lib"])
else
  AC_SUBST(nanox_lib_path, ["$nanox_install_path/lib64"])
fi

dnl Check for DRAMSIMv2 support
AC_MSG_CHECKING([whether DRAMSSIMv2 integration is enabled])
AC_ARG_ENABLE([dramsim],
    [AS_HELP_STRING([--enable-dramsim],
        [Enable DRAMSIMv2 integration (default = no)])],
    [enable_dramsim="$enableval"],
    [enable_dramsim=no])
AC_MSG_RESULT([$enable_dramsim])
if test x"$enable_dramsim" = x"yes"; then
	AC_ARG_WITH([dramsim-src-path],
	AS_HELP_STRING([--with-dramsim-src-path], [Sets the path to the DRAMSIMv2 sources]),
        dramsim_src_path="$withval",
        AC_MSG_ERROR("dramsim-src-path not specified",-1))
	AC_ARG_WITH([dramsim-install-path],
	AS_HELP_STRING([--with-dramsim-install-path], [Sets the installation path for DRAMSIMv2]),
        dramsim_install_path="$withval",
        AC_MSG_ERROR("dramsim-install-path not specified",-1))
fi
AM_CONDITIONAL([ENABLE_DRAMSIM], [test "x$enable_dramsim" = "xyes"])
AC_SUBST(dramsim_src_path, ["$dramsim_src_path"])
AC_SUBST(dramsim_install_path, ["$dramsim_install_path"])
if test x"$enable_dramsim" = x"yes"; then
	AC_DEFINE(ENABLE_DRAMSIM)
fi

dnl Check for Ramulator support
AC_MSG_CHECKING([whether RAMULATOR integration is enabled])
AC_ARG_ENABLE([ramulator],
    [AS_HELP_STRING([--enable-ramulator],
        [Enable RAMULATOR integration (default = no)])],
    [enable_ramulator="$enableval"],
    [enable_ramulator=no])
AC_MSG_RESULT([$enable_ramulator])
if test x"$enable_ramulator" = x"yes"; then
	AC_ARG_WITH([ramulator-src-path],
	AS_HELP_STRING([--with-ramulator-src-path], [Sets the path to the Ramulator sources]),
        ramulator_src_path="$withval",
        AC_MSG_ERROR("ramulator-src-path not specified",-1))
  AC_ARG_WITH([ramulator-lib-path],
	AS_HELP_STRING([--with-ramulator-lib-path], [Sets the path to libramulator.a from Ramulator]),
        ramulator_lib_path="$withval",
        AC_MSG_ERROR("ramulator-lib-path not specified",-1))
else
  #global_cxxflags="$global_cxxflags -Werror"
  global_cxxflags="$global_cxxflags"
fi
AM_CONDITIONAL([ENABLE_RAMULATOR], [test "x$enable_ramulator" = "xyes"])
AC_SUBST(ramulator_src_path, ["$ramulator_src_path"])
AC_SUBST(ramulator_lib_path, ["$ramulator_lib_path"])
if test x"$enable_ramulator" = x"yes"; then
	AC_DEFINE(ENABLE_RAMULATOR)
	AC_DEFINE(RAMULATOR)
fi

dnl Check whether to enable Compulsory/Non-Compulsory miss analysis.
AC_MSG_CHECKING([Whether to enable Compulsory/Non-Compulsory miss analysis.])
AC_ARG_ENABLE([compulsory-miss-analysis],
    [AS_HELP_STRING([--enable-compulsory-miss-analysis],
        [Enable Paraver trace generation (default = no)])],
    [enable_compulsory_miss_analysis="$enableval"],
    [enable_compulsory_miss_analysis=no])
AC_MSG_RESULT([$enable_compulsory_miss_analysis])
AM_CONDITIONAL([ENABLE_COMPULSORY_MISS], [test "x$enable_compulsory_miss_analysis" = "xyes"])
if test x"$enable_compulsory_miss_analysis" = x"yes"; then
	AC_DEFINE(ENABLE_COMPULSORY_MISS)
fi


dnl Check whether to create Paraver trace
AC_MSG_CHECKING([whether Paraver trace generation is enabled])
AC_ARG_ENABLE([paraver],
    [AS_HELP_STRING([--enable-paraver],
        [Enable Paraver trace generation (default = no)])],
    [enable_paraver="$enableval"],
    [enable_paraver=no])
AC_MSG_RESULT([$enable_paraver])
AM_CONDITIONAL([ENABLE_PARAVER], [test "x$enable_paraver" = "xyes"])
if test x"$enable_paraver" = x"yes"; then
	AC_DEFINE(ENABLE_PARAVER)
fi

dnl Check whether to generate time usage statistics
AC_MSG_CHECKING([whether Runtime statistics per code region are enabled])
AC_ARG_ENABLE([timestats],
    [AS_HELP_STRING([--enable-timestats],
        [Enable Runtime statistics (default = no)])],
    [enable_timestats="$enableval"],
    [enable_timestats=no])
AC_MSG_RESULT([$enable_timestats])
AM_CONDITIONAL([ENABLE_TIME_STATS], [test "x$enable_timestats" = "xyes"])
if test x"$enable_timestats" = x"yes"; then
	AC_DEFINE(ENABLE_TIME_STATS)
fi



dnl Check OpenMPI
AC_MSG_CHECKING([wheter MPI is enabled for Hybrid applications MPI+OmpSs])

AC_ARG_ENABLE([mpi],
    [AS_HELP_STRING([--enable-mpi],
        [Enable MPI support for hybrid applications (default = no)])],
    [enable_mpi="$enableval"],
    [enable_mpi=no])
AC_MSG_RESULT([$enable_mpi])
AM_CONDITIONAL([ENABLE_MPI], [test "x$enable_mpi" = "xyes"])
if test x"$enable_mpi" = x"yes"; then
        AC_ARG_WITH([mpi_install_path],
        AS_HELP_STRING([--with-mpi-install-path],[Sets the path to MPI installation]),
        mpi_install_path="$withval",
        mpi_install_path="/usr")

        #MPI_INCLUDES="${mpi_install_path}/bin"
        MPI_INCLUDES="${mpi_install_path}/include"
        MPI_CFLAGS="-I${mpi_install_path}/include"
        CPPFLAGS="${MPI_CFLAGS} ${CPPFLAGS}"

fi
AC_SUBST(mpi_install_path, ["$mpi_install_path"])

dnl find right library directory
AC_CHECK_FILE("$mpi_install_path/lib64",[AC_SUBST(mpi_lib_path, ["$mpi_install_path/lib64"])],[AC_SUBST(mpi_lib_path, ["$mpi_install_path/lib"])])

AC_MSG_CHECKING([wheter MPI version 3 is enabled])

AX_FLAGS_SAVE()
AC_LANG_SAVE()

AC_MSG_CHECKING([Checking if compiler supports MPI version 3 ])
AC_LANG([C])
CFLAGS="-I${MPI_INCLUDES}"
AC_TRY_COMPILE(
  [#include <mpi.h>],
  [
       #if MPI_VERSION >= 3
         return 1;
       #else
         #error "MPI version different to 3"
       #endif
  ],
  [mpi3_version="yes"],
  [mpi3_version="no"]
)
if test "${mpi3_version}" = "yes"; then
   AC_DEFINE([MPI3], [1], [Define if we are using a MPI3 implementation])
fi

AC_LANG_RESTORE
AC_MSG_RESULT([${mpi3_version}])
AX_FLAGS_RESTORE()

dnl Check whether to generate a Text or Binary(default) trace
AC_MSG_CHECKING([whether a Text Trace has been requested])
AC_ARG_ENABLE([text-traces],
    [AS_HELP_STRING([--enable-text-traces],
        [Enable Text Trace Generation (default = no)])],
    [enable_text_traces="$enableval"],
    [enable_text_traces=no])
AC_MSG_RESULT([$enable_text_traces])
AM_CONDITIONAL([USE_TEXT_EVENT_STREAM], [test "x$enable_text_traces" = "xyes"])

if test x"$enable_text_traces" = x"yes"; then
	AC_DEFINE(USE_TEXT_EVENT_STREAM)
    textformat="txt"
else
    textformat="bin"
fi

dnl Check whether to generate a Text or Binary(default) trace
AC_MSG_CHECKING([whether trace compression is enabled])
AC_ARG_ENABLE([compressed-traces],
    [AS_HELP_STRING([--enable-compressed-traces],
        [Enable Trace Compression (default = no)])],
    [enable_compressed_traces="$enableval"],
    [enable_compressed_traces=no])
AC_MSG_RESULT([$enable_compressed_traces])
AM_CONDITIONAL([USE_COMPRESSED_TRACES], [test "x$enable_compressed_traces" = "xyes"])
if test x"$enable_compressed_traces" = x"yes"; then
	AC_DEFINE(USE_COMPRESSED_TRACES)
    compressedformat="gz"
else
    compressedformat="nogz"
fi

AC_ARG_WITH([only-tasksim],
      AS_HELP_STRING([--with-only-tasksim], [Disables all simulators but the tasksim version]),
      [only_tasksim="$enableval"],
      [only_tasksim=no])
AM_CONDITIONAL([ONLY_TASKSIM], [test "x$only_tasksim" = "xyes"])

dnl Trace format
AC_ARG_VAR([traceformat], [Selects the format to store basic blocks and memory accesses in traces])
AC_MSG_CHECKING([which format to use to store trace])
if test x"$traceformat" = x"mem"; then
  AC_DEFINE(USE_MEM_TRACES)
fi
if test x"$traceformat" = x"uop"; then
  AC_DEFINE(USE_UOP_TRACES)
fi
if test x"$traceformat" = x"rle"; then
  AC_DEFINE(USE_RLE_TRACES)
fi

if test x"$traceformat" = x"mem" -o x"$traceformat" = x"uop" -o x"$traceformat" = x"rle"; then
	AC_MSG_RESULT([$traceformat])
else
	AC_MSG_RESULT([undefined])
	AC_MSG_WARN([falling back to default trace format (mem)])
fi

dnl Check for PTLSim compilation
AC_MSG_CHECKING([whether to build PTLSim backend])
AC_ARG_ENABLE([ptlsim],
	[AS_HELP_STRING([--enable-ptlsim],
		[Enable ptlsim tracing support (default = no)])],
	[enable_ptlsim="$enableval"],
	[enable_ptlsim=no])
AC_MSG_RESULT([$enable_ptlsim])
if test x"$enable_ptlsim" = x"yes"; then
	AC_SUBST([PTLSIM_CXXFLAGS],
	         ["-Wl,--allow-multiple-definition \
	           -fno-trapping-math -fno-stack-protector \
	           -fno-rtti -funroll-loops \
	           -mpreferred-stack-boundary=4 -fno-strict-aliasing \
	           -Wreturn-type -Wno-sign-compare -Wno-parentheses \
	           -Wno-unused -Wno-address -Wno-format \
	           -Wno-sequence-point -Wno-missing-braces"])
    AC_SUBST(PTLSIM_DIR, ["\$(top_srcdir)/${ptlsim_dir}"])
fi
AM_CONDITIONAL([ENABLE_PTLSIM], [test "x$enable_ptlsim" = "xyes"])
if test x"$enable_ptlsim" = x"yes"; then
       AC_DEFINE(ENABLE_PTLSIM)
fi

AC_SUBST([global_cxxflags], [${global_cxxflags}])
AC_SUBST([AM_LDFLAGS], ["-lz"])
AC_SUBST(GTEST_DIR, ["\$(top_srcdir)/${gtest_dir}"])

AC_CONFIG_COMMANDS([ptlsim],[mkdir -p include && ln -Tfs $ac_abs_srcdir/${ptlsim_dir} include/ptlsim], [ptlsim_dir=${ptlsim_dir}])
AC_CONFIG_COMMANDS([nextsim],[mkdir -p include && ln -Tfs $ac_abs_srcdir/${src_dir} include/nextsim], [src_dir=src])

AC_CHECK_PROGS([DOXYGEN], [doxygen])
if test -z "$DOXYGEN";
   then AC_MSG_NOTICE([Doxygen not found - continuing without Doxygen support])
fi

dnl Check whether to generate a Text or Binary(default) trace
AC_MSG_CHECKING([whether to generate Doxygen documentation])
AC_ARG_ENABLE([doxygen],
    [AS_HELP_STRING([--enable-doxygen],
        [Enable Doxygen documentation generation (default = no)])],
    [enable_doxygen="$enableval"],
    [enable_doxygen=no])
AC_MSG_RESULT([$enable_doxygen])
AM_CONDITIONAL([USE_DOXYGEN], [test "x$enable_doxygen" = "xyes"])

AM_CONDITIONAL([ENABLE_DOXYGEN], [test -n "$DOXYGEN" -a x"$enable_doxygen" = x"yes"])


AC_OUTPUT([
    Makefile
    ext/Makefile
    ompss/Makefile
    src/Makefile
    src/core/Makefile
    src/simulators/tasksim/Makefile
    src/simulators/tasksim/runtime/Makefile
    src/simulators/Makefile
    scripts/Makefile
    examples/Makefile
    tests/Makefile
    tests/unit/Makefile
    tests/functional/Makefile
    doc/Doxyfile
    doc/Makefile
])

AC_MSG_NOTICE([Using $nanox_install_path for Nanos++/Mercurium])
AS_ECHO(["
Configuration summary for TaskSim:
 - PTLSim decoding enabled: $enable_ptlsim
 - PINTool enabled: $enable_pin
 - DynamoRIO Tool enabled: $enable_dr
 - Nanos++ runtime integration enabled: $enable_nanox
 - MUSA MPI code: $enable_mpi
 - Paraver output trace generation: $enable_paraver
 - Ramulator DRAM simulator: $enable_ramulator
 - DRAMSim DRAM simulator: $enable_dramsim
 - System logging verbosity_level: $simlog_level
 - Simulation time statistics: $enable_timestats
 - Debug mode: $debugit
 - Asserts: $asserts
 - Trace format: $traceformat $textformat $compressedformat
 - Doxygen documentation: $enable_doxygen
 "])
