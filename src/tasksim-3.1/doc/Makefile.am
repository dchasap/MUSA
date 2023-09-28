
if ENABLE_DOXYGEN

doxygen:Doxyfile
	$(DOXYGEN) Doxyfile

all-local:doxygen
clean-local:
	rm -rf $(top_srcdir)/doc/html doxygen_warnings.log

endif

