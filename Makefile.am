AM_CPPFLAGS =

if BUNDLED_LIBRADIUS
SUBDIRS = libradius
AM_CPPFLAGS += -I$(abs_top_srcdir)/libradius
openvpn_radauth_LDADD = libradius/libradius.a
if HAVE_LIBBSD
openvpn_radauth_LDADD += -lbsd
endif
endif

sbin_PROGRAMS = openvpn_radauth
openvpn_radauth_SOURCES = openvpn_radauth.c credentials.c

.PHONY: test
test:
	$(MAKE) -C test
	./test_build
