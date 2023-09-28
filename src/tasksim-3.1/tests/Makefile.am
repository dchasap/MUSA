export AM_CPPFLAGS += -I$(abs_top_builddir)/include -I$(abs_top_srcdir)/src -I$(abs_top_builddir)/src

if ENABLE_NANOX
export AM_CPPFLAGS += -I@nanox_install_path@/include/nanox -I@nanox_install_path@/include -I@nanox_src_path@/src/apis/c -I@nanox_src_path@/src/support -I@nanox_src_path@/src/core -I@nanox_src_path@/src/arch/smp -include"new_decl.hpp"
endif

SUBDIRS = unit functional
