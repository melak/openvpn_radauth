AM_CPPFLAGS =

if BUNDLED_LIBRADIUS
SUBDIRS = libradius
AM_CPPFLAGS += -Ilibradius
openvpn_radauth_LDADD = libradius/libradius.a -lbsd
endif

bin_PROGRAMS = openvpn_radauth
openvpn_radauth_SOURCES = openvpn_radauth.c credentials.c
