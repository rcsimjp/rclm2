#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(rclm2, 20090507)

AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([gnu check-news])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB
AC_PROG_INSTALL

# Checks for libraries.
AX_BOOST_BASE([1.32.0])
CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"

# Checks for header files.
AC_CHECK_HEADERS([netinet/in.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
#AC_TYPE_INT16_T
#AC_TYPE_INT32_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_HEADER_STDC
AC_CHECK_FUNCS([memset rint])

AC_CONFIG_FILES([Makefile
                 bin/Makefile
                 bin/rclm2
                 rcg/Makefile
                 share/rclm2/Makefile])

AC_OUTPUT
