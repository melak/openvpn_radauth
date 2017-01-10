
include		../Makefile.rules

SRCS		 = radlib.c

ifeq ($(WITH_SSL_IMPL),polarssl)
CFLAGS		+= -DWITH_POLARSSL
else
ifeq ($(WITH_SSL_IMPL),openssl)
CFLAGS		+= -DWITH_SSL
else
ifeq ($(WITH_SSL_IMPL),mbedtls)
CFLAGS		+= -DWITH_MBEDTLS
else
SRCS		+= md5c.c
CFLAGS		+= -I.
endif
endif
endif

OBJS		 = $(SRCS:.c=.o)
LIBS		= libradius.a

all:		clean $(LIBS)

$(LIBS):	$(OBJS)
		$(AR) rcs $@ $?

%.o:		%.c
		$(CC) $(CFLAGS) -o $@ -c $<

clean:
		-rm -f $(OBJS) $(LIBS) core core.*

