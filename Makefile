CC = gcc
CFLAGS = -O2 -s 
LIBS = -lncursesw
SRCS = text2048.c map_ops.c
OBJS = $(SRCS:.c=.o)
MAIN = text2048

.PHONY: clean

all:    $(MAIN)

$(MAIN): $(OBJS) 
	$(CC) -o $(MAIN) $(OBJS) $(LIBS) $(CFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)
