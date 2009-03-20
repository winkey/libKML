## Process this file with automake to produce Makefile.in

## Created by Anjuta

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
	error.h

libKML_la_LDFLAGS = \
	-lzip

includes_HEADERS = \
	libKML.h

includesdir = \
	$(pkgincludedir)

EXTRA_DIST = \
	$(includes_HEADERS)

