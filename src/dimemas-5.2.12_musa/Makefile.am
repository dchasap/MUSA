AUTOMAKE_OPTIONS = foreign no-dependencies

SUBDIRS = common_utilities include Simulator prv2dim 

ACLOCAL_AMFLAGS= -I m4

if HAVE_JAVA
SUBDIRS += GUI
endif

SUBDIRS += trf2dim

EXTRA_DIST = 

distdir = $(PACKAGE)-$(VERSION)
