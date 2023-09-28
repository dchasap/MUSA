AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4
export AM_CXXFLAGS = $(global_cxxflags)
SUBDIRS = ext src scripts doc examples
#SUBDIRS = ext src tests scripts doc examples
DIST_SUBDIRS = scripts
doc_DATA = AUTHORS COPYING NEWS README
EXTRA_DIST = $(doc_DATA)
nobase_sysconf_DATA = conf/tasksim.conf \
               conf/thunderx2.conf \
               conf/sttmramsim.conf \
               conf/simplememsim.conf \
               conf/memsim.conf \
               conf/sunway_configuration.conf \
               conf/marenostrum_configuration.conf \
               conf/mn4.conf \
               conf/mn4_musa.conf \
               conf/arm_low_power.conf \
               conf/arm_high_end.conf \
               conf/musa_sve_template_A.conf \
               conf/musa_sve_template_B.conf \
               conf/musa_sve_template_C.conf \
               conf/musa_sve_template_D.conf \
               conf/frontend_direct.conf \
               conf/frontend_bus.conf \
               conf/fastmemsim.conf \
               conf/epi_wide.conf \
               conf/epi_narrow.conf \
               conf/burstsim_paraver.conf \
               conf/burstsim_heterogeneous.conf \
               conf/burstsim.conf \
               conf/paraver/Tasksim_ram_counters.cfg \
               conf/paraver/tasksim_nanos.cfg \
               conf/paraver/tasksim_function_name.cfg \
               conf/paraver/example.pcf

if ENABLE_NANOX

.PHONY: nanox nanox-install nanox-clean

nanox:
	(cd ompss; make)

nanox-install: nanox
	(cd ompss; make install)

nanox-clean:
	(cd ompss; make clean)
endif
