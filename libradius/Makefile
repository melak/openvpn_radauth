
include		../Makefile.rules

SRCS		 = radlib.c
OBJS		 = $(SRCS:.c=.o)

ifeq ($(WITH_SSL_IMPL),polarssl)
CFLAGS		+= -DWITH_POLARSSL
else
CFLAGS		+= -DWITH_SSL
endif

all:		clean $(OBJS)

%.o:		%.c
		$(CC) $(CFLAGS) -o $@ -c $<

clean:
		-rm -f $(OBJS) core core.*

