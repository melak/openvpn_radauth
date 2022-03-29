
include		 Makefile.rules

SRC		 = openvpn_radauth.c credentials.c
OBJS		 = $(SRC:.c=.o) libradius/libradius.a
PROG		 = $(SRC:.c=)$(SUFFIX)
ifeq ($(shell uname -s),Linux)
LDFLAGS		+= -lbsd
endif

STRIP		?= strip

CFLAGS		+= -I$(CURDIR)/libradius

PROG		 = $(firstword $(SRC:.c=))$(SUFFIX)

ifdef BUILD_DEBUG
CFLAGS		+= -DDEBUG
endif

ifdef BUILD_TEST
CFLAGS		+= -DOPENVPN_RADIUS_CONF=\"$(shell pwd)/test/radius.conf\"
endif

all:		clean $(PROG) strip

strip:		$(PROG)
ifndef DEBUG
		$(STRIP) $(PROG)
endif

%.o:		%.c
		$(CC) $(CFLAGS) -c -o $@ $<

libradius/libradius.a:
		$(MAKE) -C libradius

$(PROG):	clean $(OBJS)
		$(CC) -o $(PROG) $(OBJS) $(LDFLAGS)

test:		clean
		$(MAKE) -C test
		$(MAKE) BUILD_TEST=1
		./test_build

clean:
		$(MAKE) -C test clean
		$(MAKE) -C libradius clean
		-rm -f *.o core core.* $(PROG)

