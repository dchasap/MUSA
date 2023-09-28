# Functional tests

CLEANFILES=tests.log

check-local: $(top_srcdir)/tests/functional/run_tests
	$(top_srcdir)/tests/functional/run_tests -build_path $(abs_top_builddir) -xed_binary $(XED2_PATH) -install_path $(prefix) -o tests.log $(srcdir)/tests
