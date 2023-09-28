noinst_LTLIBRARIES = libMusaClient.la

simulator_dir = $(top_srcdir)/Simulator

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
  -I$(simulator_dir)/eee_model \
	-I$(simulator_dir)/musaclient

libMusaClient_la_SOURCES = \
  tasksimclient.c \
  tasksimclient.h \
  tasksim_utils.cpp
