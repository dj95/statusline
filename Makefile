# Statusline
# (c) 2015 Daniel Jankowski

NAME = statusline
VERSION = 0.1

# C Compiler
CC := $(CC) -std=gnu11


# Compiler Flags
CFLAGS += -DPROGNAME=\"${NAME}\" -DVERSION=\"${VERSION}\"
CFLAGS := $(CFLAGS)

# Executable
EXE = statusline

# Sources
SRCS = statusline.c

OBJS = $(SRCS:.c=.o)

PREFIX = /usr/local

all: $(EXE)

.o: $(SRCS) $(SRCS)
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f *.o && rm -f $(EXE)

install: $(EXE)
	install -Dm755 $(EXE) $(PREFIX)/bin/$(EXE)
