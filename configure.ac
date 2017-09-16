#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(gnome-internet-radio-locator,1.0.0)
AM_INIT_AUTOMAKE([-Wno-portability 1.11 no-dist-gzip dist-xz tar-ustar subdir-objects])
AM_MAINTAINER_MODE
AC_CONFIG_SRCDIR([src/gnome-internet-radio-locator.c])
AC_CONFIG_HEADER([config.h])

LT_PREREQ([2.2.6])
LT_INIT([dlopen win32-dll pic-only])

m4_ifdef([AM_SILENT_RULES],
  [AM_SILENT_RULES([yes])],
  [
    AM_DEFAULT_VERBOSITY=1
    AC_SUBST(AM_DEFAULT_VERBOSITY)
  ]
)

AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_CPP
AC_PROG_INSTALL
AM_PROG_CC_C_O

AC_HEADER_STDC
AC_C_CONST
AC_C_INLINE
AC_ISC_POSIX
AC_SYS_LARGEFILE

AC_C_BIGENDIAN

AC_CHECK_LIBM
AC_SUBST(LIBM)

PKG_PROG_PKG_CONFIG

# Checks for libraries.

record_msg="no (not requested)"
try_record=:
gnome_internet_radio_locator_record=false

AC_ARG_WITH([recording],AS_HELP_STRING([--with-recording],[Support recording with gstreamer]), [
     AC_MSG_CHECKING([for recording])
     if test x$withval = xno; then
           try_record=false
	   record_msg = "no (not requested)"
	   AC_DEFINE([HAVE_GNOME_INTERNET_RADIO_LOCATOR_RECORD],0,[record missing])
	   gnome_internet_radio_locator_record=false
     else
	   AC_DEFINE([HAVE_GNOME_INTERNET_RADIO_LOCATOR_RECORD],1,[have record])
	   record_msg="yes (with recording support)"
	   gnome_internet_radio_locator_record=true
     fi
     AC_MSG_RESULT([$record_msg])
])
AM_CONDITIONAL([HAVE_GNOME_INTERNET_RADIO_LOCATOR_RECORD], [$gnome_internet_radio_locator_record])
AC_SUBST([HAVE_GNOME_INTERNET_RADIO_LOCATOR_RECORD])

AC_CHECK_LIB(m, pow, [], [], [])

PKG_CHECK_MODULES(GNOME_INTERNET_RADIO_LOCATOR,
		  gtk+-3.0 >= 3.0 \
		  glib-2.0 >= 2.38.0 \
		  gobject-2.0 >= 2.38.0 \
		  pangoft2 >= 0.28 \
		  champlain-gtk-0.12 >= 0.12.10 \
		  libxml-2.0 >= 2.0 \
		  geocode-glib-1.0 >= 3.20 \
		  gstreamer-tag-1.0 >= 1.0 \
		  gstreamer-1.0 >= 1.0 \
		  gstreamer-player-1.0 >= 1.0 \
		  gstreamer-video-1.0)

GNOME_INTERNET_RADIO_LOCATOR_PKG_DEPS="geocode-glib-1.0 gtk+-3.0 glib-2.0 gobject-2.0 pangoft2 champlain-gtk-0.12 libxml-2.0, gstreamer-1.0 gstreamer-player-1.0 gstreamer-video-1.0"

GTK_DOC_CHECK([1.16], [--flavour no-tmpl])

#YELP_HELP_INIT

#GNOMEDOC=`which yelp-build`
#if test -z $GNOMEDOC; then
#   echo "*** The tools to build the documentation are not found,"
#   echo "    please intall the yelp-tool package ***"
#   exit 1
#fi

#AC_ARG_ENABLE([user_help],
#	AS_HELP_STRING([--disable-user-help],[Disable the user help to speed the build (default: enabled)]),
#	[enable_user_help=$enableval],
#	[enable_user_help=yes])

#AM_CONDITIONAL(ENABLE_USER_HELP, test x"$enable_user_help" = "xyes")

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strdup])

# ***********                                                                   
# Translation                                                                   
# ***********                                                                   

AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL([0.50.1])

GETTEXT_PACKAGE=AC_PACKAGE_NAME
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Gettext package.])

AC_SUBST(GNOME_INTERNET_RADIO_LOCATOR_LIBS)
AC_SUBST(GNOME_INTERNET_RADIO_LOCATOR_CFLAGS)
AC_SUBST(GNOME_INTERNET_RADIO_LOCATOR_PKG_DEPS)

dnl Enable yelp-tools
# YELP_HELP_INIT

AC_OUTPUT([
Makefile
data/Makefile
data/gnome-internet-radio-locator.desktop
data/icons/Makefile
data/icons/16x16/Makefile
data/icons/22x22/Makefile
data/icons/24x24/Makefile
data/icons/32x32/Makefile
data/icons/48x48/Makefile
data/icons/256x256/Makefile
data/icons/512x512/Makefile
data/icons/1024x1024/Makefile
help/Makefile
help/C/version.xml
man/Makefile
po/Makefile.in
src/Makefile
gnome-internet-radio-locator.spec
])
