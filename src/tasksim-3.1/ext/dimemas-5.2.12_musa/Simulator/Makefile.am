simulator_dir = $(top_srcdir)/Simulator

SUBDIRS = io input model eee_model prv_utils sched utils musaclient

EXTRA_DIST = lib_extern_model

if VENUS_ENABLED
  SUBDIRS += venusclient
endif

AM_CPPFLAGS = \
	-I$(top_srcdir)/common_utilities \
	-I$(top_srcdir)/include \
	-I$(simulator_dir)/io \
	-I$(simulator_dir)/input \
	-I$(simulator_dir)/model \
	-I$(simulator_dir)/prv_utils \
	-I$(simulator_dir)/sched \
	-I$(simulator_dir)/utils \
	-I$(simulator_dir)/venusclient \
	-I$(simulator_dir)/musaclient \
	-I$(simulator_dir)/eee_model

bin_PROGRAMS = Dimemas DimemasUpdateCFG

Dimemas_SOURCES = \
	main.cpp

Dimemas_CFLAGS=-I$(top_srcdir) -I$(top_srcdir)/include
Dimemas_CXXFLAGS=-I$(top_srcdir) -I$(top_srcdir)/include

nodist_EXTRA_Dimemas_SOURCES = dummy.cc

Dimemas_LDADD = \
	$(top_builddir)/Simulator/model/libDimemasModel.la \
	$(top_builddir)/Simulator/sched/libDimemasScheduling.la \
	$(top_builddir)/common_utilities/libCommonUtilities.la \
	$(top_builddir)/Simulator/input/libDimemasInput.la \
	$(top_builddir)/Simulator/io/libDimemasIO.la \
	$(top_builddir)/Simulator/prv_utils/libDimemas2ParaverUtils.la \
	$(top_builddir)/Simulator/utils/libDimemasUtils.la \
  $(top_builddir)/Simulator/musaclient/libMusaClient.la \
	$(top_builddir)/Simulator/eee_model/libDimemasEEEModel.la 

if VENUS_ENABLED
Dimemas_LDADD += $(simulator_dir)/venusclient/libDimemasVenusClient.la
endif

DimemasUpdateCFG_SOURCES = Dimemas_updateCFGs.cpp

LIB_EXTERN_MODEL_SRCS=$(simulator_dir)/lib_extern_model/Makefile \
	$(simulator_dir)/lib_extern_model/extern_comm_model.c \
	$(simulator_dir)/lib_extern_model/README

install-data-hook:
	$(mkdir_p) $(includedir)/
	$(mkdir_p) $(datadir)/lib_extern_model_example
	cp -f $(LIB_EXTERN_MODEL_SRCS) $(datadir)/lib_extern_model_example
	cp -f $(top_srcdir)/Simulator/lib_extern_model/extern_comm_model.h $(includedir)/

