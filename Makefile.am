AM_CPPFLAGS =

if BUNDLED_LIBRADIUS
SUBDIRS = libradius
AM_CPPFLAGS += -I$(abs_top_srcdir)/libradius
openvpn_radauth_LDADD = libradius/libradius.a -lbsd
endif

bin_PROGRAMS = openvpn_radauth
openvpn_radauth_SOURCES = openvpn_radauth.c credentials.c

test:
	./test_build

.PHONY: test
