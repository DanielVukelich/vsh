CC=gcc
CFLAGS=-Wall -Werror -std=c99
OBJS=argbuild.o vsh.o
BINARY=vsh

.PHONY: clean all debug release

all: debug

release: $(BINARY)

debug:	CFLAGS += -g
debug:	$(BINARY)

$(BINARY): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(LDLIBS)

%.o: %.c %.h
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm $(OBJS) $(BINARY)
