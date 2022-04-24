AC_INIT([openvpn_radauth], [1.1])
AC_CONFIG_SRCDIR([openvpn_radauth.c])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC
AC_PROG_RANLIB
AM_PROG_AR

m4_cleardivert([HELP_BEGIN])dnl
m4_divert_push([HELP_BEGIN])dnl
if test "$ac_init_help" = "long"; then
  cat <<_ACEOF
\`configure' configures AC_PACKAGE_STRING

Usage: $[0] [[OPTION]]... [[VAR=VALUE]]...

Package-specific options:

  --with-radius-config=PATH    path to libradius configuration file
  --enable-debug               Emit some debug messages to syslog (not very useful)
  --enable-bundled-libradius   Always use bundled libradius

All the usual Autoconf options and conventions are accepted as well.

_ACEOF
  exit 0
fi
m4_divert_pop([HELP_BEGIN])dnl

AC_ARG_WITH(radius-config,
    [AS_HELP_STRING([--with-radius-config=PATH], [path to libradius configuration file])],
    [
        AS_IF([test "x${with_radius_config%${with_radius_config#?}}" != x/],
            [AC_MSG_ERROR([--with-radius-config must specify an absolute path])],
	    [radius_config="${with_radius_config}"]
        )
    ],
    [
        radius_config="/etc/openvpn/radius.conf"
    ]
)
AC_DEFINE_UNQUOTED([OPENVPN_RADIUS_CONF], ["${radius_config}"])
AC_MSG_NOTICE([using libradius configuration file ${radius_config}])

AC_ARG_ENABLE(debug,
    [AS_HELP_STRING([--enable-debug], [Emit some debug messages to syslog (not very useful)])],
    [
        AC_DEFINE([DEBUG])
    ],
    []
)

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
AM_CONDITIONAL([HAVE_LIBBSD], [test "x$have_LIBBSD" = x1])

AC_ARG_ENABLE(bundled-libradius,
    [AS_HELP_STRING([--enable-bundled-libradius], [Always use bundled libradius])],
    [use_BUNDLED_LIBRADIUS=1],
    [
        AC_SEARCH_LIBS([rad_auth_open], [radius], [], [use_BUNDLED_LIBRADIUS=1])
    ]
)
AM_CONDITIONAL([BUNDLED_LIBRADIUS], [test "x$use_BUNDLED_LIBRADIUS" = x1])
    
AC_CONFIG_FILES([Makefile libradius/Makefile])
AC_OUTPUT
