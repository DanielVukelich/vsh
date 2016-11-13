CC=gcc
CFLAGS=-pedantic -Wall -Wextra -Werror -std=c99
OBJS=err_handling.o trie/trie.o trie/intern.o builtin.o argbuild.o vsh.o envmgr.o
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
