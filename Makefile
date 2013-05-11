
include		 Makefile.rules

SRC		 = openvpn_radauth.c credentials.c
OBJS		 = $(SRC:.c=.o) libradius/radlib.o
PROG		 = $(SRC:.c=)$(SUFFIX)

CFLAGS		+= -I$(CURDIR)/libradius

ifeq ($(WITH_SSL_IMPL),polarssl)
LDFLAGS		+= -lpolarssl
else
LDFLAGS		+= -lcrypto
endif

export		WITH_SSL_IMPL
$(info *** CFLAGS=[$(CFLAGS)] ***)

ifeq ($(filter -DHAVE_STRL,$(CFLAGS)),)
SRC		+= compat/strl.c
endif

OBJS		+= $(SRC:.c=.o)
PROG		 = $(firstword $(SRC:.c=))$(SUFFIX)

all:		clean $(PROG) strip

strip:		$(PROG)
ifndef DEBUG
		$(STRIP) $(PROG)
endif

%.o:		%.c
		$(CC) $(CFLAGS) -c -o $@ $<

compat/%.o:
		$(MAKE) -C compat

libradius/radlib.o:
		$(MAKE) -C libradius

$(PROG):	clean $(OBJS)
		$(CC) -o $(PROG) $(shell lorder $(OBJS) | tsort) $(LDFLAGS)

clean:
		$(MAKE) -C compat clean
		$(MAKE) -C libradius clean
		-rm -f *.o core core.* $(PROG)

