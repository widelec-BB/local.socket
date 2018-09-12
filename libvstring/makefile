#
# Makefile for libvstring
#

# Commons

CC = ppc-morphos-gcc-6
AR = ppc-morphos-ar
CFLAGS = -W -Wall -O2 -Wpointer-arith -Wno-parentheses -DUSE_INLINE_STDARG
CFLAGS += -fno-strict-aliasing -fno-builtin
OBJS  = strnew.o strcopy.o strlen.o
LIB = libvstring.a

################################################################################

.PHONY: all clean install install-iso doc

all: $(LIB)

clean:
	rm -vf $(OBJS) $(LIB) $(DOC)

################################################################################

$(LIB): $(OBJS)
	@echo "Library merging..."
	@$(AR) rs $@ $(OBJS)
	@List $(LIB) LFORMAT "%n %l %d %t"

%.o: %.c libvstring.h
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $<
