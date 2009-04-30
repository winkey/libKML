
INCLUDES = $(DEPS_CFLAGS)
libKML_la_LIBADD = $(DEPS_LIBS)

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" 

AM_CFLAGS =\
	 -Wall\
	 -g

lib_LTLIBRARIES = \
	libKML.la

libKML_la_SOURCES = \
	KML.c      \
	buffer.c      \
	buffer.h      \
	kml.h      \
	zipbuffer.c      \
	zipbuffer.h      \
	error.h      \
	../minizip/crypt.h      \
	../minizip/ioapi.c      \
	../minizip/ioapi.h      \
	../minizip/zip.c      \
	../minizip/zip.h



includes_HEADERS = \
	libKML.h

includesdir = \
	$(pkgincludedir)

EXTRA_DIST = \
	$(includes_HEADERS)

