export AM_CXXFLAGS = $(global_cxxflags)
export AM_CPPFLAGS = $(global_cppflags) -I$(abs_top_builddir)/include -I$(abs_srcdir) -I$(abs_builddir)
if ENABLE_NANOX
  export AM_CPPFLAGS += -I@nanox_install_path@/include/nanox -I@nanox_install_path@/include -I@nanox_src_path@/src/apis/c -I@nanox_src_path@/src/support -I@nanox_src_path@/src/core -I@nanox_src_path@/src/arch/smp -I@nanox_src_path@/src/arch/unix-os -I@nanox_src_path@/src/arch/cluster -I@nanox_src_path@/src
  export AM_CXXFLAGS += -include "new_decl.hpp"
endif
if ENABLE_DRAMSIM
  export AM_CPPFLAGS += -I@dramsim_src_path@
endif
if ENABLE_RAMULATOR
  export AM_CPPFLAGS += -I@ramulator_src_path@
endif

SUBDIRS = core simulators
