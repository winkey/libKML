pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libKML.pc

INCLUDES = $(DEPS_CFLAGS)
LIBS = $(DEPS_LIBS)

SUBDIRS = src 

libkmldocdir = ${prefix}/doc/libkml
libkmldoc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS

EXTRA_DIST = $(libkmldoc_DATA)

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

