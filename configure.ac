AC_INIT([openvpn_radauth], [1.0])
AC_CONFIG_SRCDIR([openvpn_radauth.c])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC
AC_PROG_RANLIB
AM_PROG_AR

AC_SEARCH_LIBS([strlcpy], [], [], [
    PKG_CHECK_MODULES(LIBBSD, [libbsd >= 0.8.0], [have_LIBBSD=1])
])
AC_SEARCH_LIBS([explicit_bzero], [], [], [
    AS_IF([test "x$have_LIBBSD" != x1], [
        PKG_CHECK_MODULES(LIBBSD, [libbsd >= 0.8.0], [have_LIBBSD=1])
    ],
    [])
])
AC_SEARCH_LIBS([arc4random], [], [], [
    AS_IF([test "x$have_LIBBSD" != x1], [
        PKG_CHECK_MODULES(LIBBSD, [libbsd >= 0.8.0], [have_LIBBSD=1])
    ],
    [])
])
AS_IF([test "x$have_LIBBSD" = x1], [AC_DEFINE([HAVE_LIBBSD])])

AC_SEARCH_LIBS([rad_auth_open], [radius], [], [use_BUNDLED_LIBRADIUS=1])
AM_CONDITIONAL([BUNDLED_LIBRADIUS], [test "x$use_BUNDLED_LIBRADIUS" = x1])
    
AC_CONFIG_FILES([Makefile libradius/Makefile])
AC_OUTPUT
