
include		../Makefile.rules

SRCS		 = radlib.c
CFLAGS		+= -I.

OBJS		= $(SRCS:.c=.o)
LIBS		= libradius.a

all:		clean $(LIBS)

$(LIBS):	$(OBJS)
		$(AR) rcs $@ $?

%.o:		%.c
		$(CC) $(CFLAGS) -o $@ -c $<

clean:
		-rm -f $(OBJS) $(LIBS) core core.*
