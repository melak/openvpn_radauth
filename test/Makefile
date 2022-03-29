
include		../Makefile.rules

SRCS		 = picohash.c
PROG		 = $(SRCS:.c=)

all:		clean $(PROG)

clean:
		-rm -f $(PROG) core*

$(PROG):	$(SRCS)
		$(CC) $(CFLAGS) -o $@ $<
		./$(PROG)
