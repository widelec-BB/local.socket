# Makefile for local.socket.
# Generated with LibMaker 0.12.

CC = ppc-morphos-gcc-6
CFLAGS += -s -O2 -noixemul -nostdlib -fomit-frame-pointer -DUSE_INLINE_STDARG
CFLAGS += -Wall -Wextra -Wpointer-arith -Wno-parentheses
CFLAGS += -Ios-include/
LD = ppc-morphos-gcc-6
LDFLAGS = -nostartfiles -nostdlib -noixemul
STRIP = ppc-morphos-strip --strip-unneeded --remove-section .comment
OUTPUT = local.socket
OBJS = dummy.o library.o process.o
METHOBJS = m_new.o \
 m_dispose.o \
 m_receive.o \
 m_send.o \
 m_ondatareceived.o \
 m_ondatasent.o

.PHONY: all clean install

all: $(OUTPUT)
	@ls -l $<

clean:
	-rm -rf $(OBJS) $(FUNCOBJS) $(METHOBJS) *.bak *.s *.db $(OUTPUT)

install: all
	cp $(OUTPUT) /SYS/Classes
	-flushlib $(OUTPUT)

$(OBJS): %.o: %.c
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $<

$(FUNCOBJS): %.o: %.c library.h
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $<

$(METHOBJS): %.o: %.c library.h
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OUTPUT).db: $(OBJS) $(FUNCOBJS) $(METHOBJS)
	@echo "Linking $@..."
	@$(LD) $(LDFLAGS) $(OBJS) $(FUNCOBJS) $(METHOBJS) $(LIBS) -o $(OUTPUT).db

$(OUTPUT): $(OUTPUT).db
	@echo "Stripping $<..."
	@$(STRIP) -o $(OUTPUT) $(OUTPUT).db

library.o: library.c library.h lib_version.h
#	@echo "Compiling $@..."
#	@$(CC) $(CFLAGS) -c -o $@ $<

dummy.o: dummy.c lib_version.h
#	@echo "Compiling $@..."
#	@$(CC) $(CFLAGS) -c -o $@ $<

process.o: library.h
