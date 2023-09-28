noinst_LTLIBRARIES = libgtest.la

libgtest_la_SOURCES = gtest-1.6.0/src/gtest-all.cc
libgtest_la_CPPFLAGS = -I$(top_srcdir)/ext/gtest-1.6.0/include -I$(top_srcdir)/ext/gtest-1.6.0


if ENABLE_PTLSIM

noinst_LTLIBRARIES += libptlsim.la

libptlsim_la_SOURCES = 	ptlsim-20090314-r229/config.cpp \
						ptlsim-20090314-r229/datastore.cpp \
						ptlsim-20090314-r229/decode-complex.cpp \
						ptlsim-20090314-r229/decode-core.cpp \
						ptlsim-20090314-r229/decode-fast.cpp \
						ptlsim-20090314-r229/decode-sse.cpp \
						ptlsim-20090314-r229/decode-x87.cpp \
						ptlsim-20090314-r229/kernel.cpp \
						ptlsim-20090314-r229/glibc.cpp \
						ptlsim-20090314-r229/injectcode.cpp \
						ptlsim-20090314-r229/klibc.cpp \
						ptlsim-20090314-r229/linkend.S \
						ptlsim-20090314-r229/linkstart.S \
						ptlsim-20090314-r229/mathlib.cpp \
						ptlsim-20090314-r229/mm.cpp \
						ptlsim-20090314-r229/ptlhwdef.cpp \
						ptlsim-20090314-r229/ptlsim.cpp \
						ptlsim-20090314-r229/superstl.cpp \
						ptlsim-20090314-r229/syscalls.cpp \
						ptlsim-20090314-r229/uopimpl.cpp


# these flags are used to minimize changes to ptlsim code
libptlsim_la_CPPFLAGS = -I$(top_srcdir)/ext/ptlsim-20090314-r229
#libptlsim_la_CCASFLAGS = $(AM_CXXFLAGS) -fPIC -DPIC @PTLSIM_CXXFLAGS@
libptlsim_la_CXXFLAGS = $(AM_CXXFLAGS) @PTLSIM_CXXFLAGS@

endif
