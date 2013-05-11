
include		../Makefile.rules

SRCS		 = strl.c
OBJS		 = $(SRCS:.c=.o)

all:		clean $(OBJS)

%.o:		%.c
		$(CC) $(CFLAGS) -o $@ -c $<

clean:
		-rm -f $(OBJS) core core.*

