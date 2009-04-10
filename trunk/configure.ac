dnl Process this file with autoconf to produce a configure script.
dnl Created by Anjuta application wizard.

AC_INIT(libKML, 0.6)

AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC


AM_PROG_LIBTOOL


PKG_CHECK_MODULES(DEPS, libDataStruct >= 0.1 libzip >= 0.9)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)



AC_OUTPUT([
Makefile
src/Makefile
libKML.pc
])
