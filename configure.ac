#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([config.h.in])
AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug], [enable DEBUG mode(default=no)]),
[],                                                            
[enable_debug=no])                                              

AS_IF([test "x$enable_debug" = "xyes"], [CXXFLAGS="-g -O0 -Wall"],        
	  [test "x$enable_debug" = "xno"], [CXXFLAGS="-O3 -Wall"],                  
	  [])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])
AC_PROG_RANLIB
AC_PROG_LIBTOOL

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([strstr])

AC_CONFIG_FILES([
				 HwSql/Makefile
				 DataModel/Makefile
				 HalloSql/Makefile
				 HalloMQ/Makefile
				 RedisClient/Makefile
				 LoginService/Makefile
				 Test/Makefile
                 Makefile
				 ])
AC_OUTPUT
