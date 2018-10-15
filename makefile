# Makefile for local.socket.
# Generated with LibMaker 0.12.

CC = ppc-morphos-gcc-6
CFLAGS += -s -O2 -noixemul -nostdlib -fomit-frame-pointer -DUSE_INLINE_STDARG
CFLAGS += -Wall -Wextra -Wpointer-arith -Wno-parentheses
CFLAGS += -Ios-include/ -Ilibvstring/
LD = ppc-morphos-gcc-6
LDFLAGS = -nostartfiles -nostdlib -noixemul -Llibvstring/ -lvstring
STRIP = ppc-morphos-strip --strip-unneeded --remove-section .comment
OUTPUT = local.socket
OBJS = dummy.o library.o process.o
METHOBJS = m_new.o \
 m_dispose.o \
 m_receive.o \
 m_send.o
 
.PHONY: all clean install libvstring

all: libvstring $(OUTPUT)
	@List $(OUTPUT) LFORMAT "%n %l %d %t"

clean:
	make -C libvstring/ clean
	-rm -rf $(OBJS) $(FUNCOBJS) $(METHOBJS) *.bak *.s *.db $(OUTPUT)

install: all
	cp $(OUTPUT) /SYS/Classes
	-flushlib $(OUTPUT)

$(OBJS) $(FUNCOBJS) $(METHOBJS): %.o: %.c
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OUTPUT).db: $(OBJS) $(FUNCOBJS) $(METHOBJS)
	@echo "Linking $@..."
	@$(LD) $(LDFLAGS) $(OBJS) $(FUNCOBJS) $(METHOBJS) $(LIBS) -o $(OUTPUT).db

$(OUTPUT): $(OUTPUT).db
	@echo "Stripping $<..."
	@$(STRIP) -o $(OUTPUT) $(OUTPUT).db

libvstring:
	make -C libvstring/

dummy.o: dummy.c lib_version.h
library.o: library.c lib_version.h library.h process.h
m_dispose.o: m_dispose.c library.h process.h
m_new.o: m_new.c library.h process.h
m_receive.o: m_receive.c library.h process.h
m_send.o: m_send.c library.h process.h
process.o: process.c process.h
