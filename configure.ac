dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(libkml, 0.4)

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC


AC_CHECK_LIB(zip, zip_source_free, HAVE_LIBZIP=true, HAVE_LIBZIP=false)
if test $HAVE_LIBZIP = "false"; then
  AC_MSG_FAILURE([libzip not found])
fi



AM_PROG_LIBTOOL






AC_OUTPUT([
Makefile
src/Makefile

])
