CC = ppc-morphos-gcc-6
CFLAGS += -noixemul -nostartfiles -s -O2 -fomit-frame-pointer -mno-prototype -mcpu=604e -mmultiple -DUSE_INLINE_STDARG
CFLAGS += -W -Wall -Wpointer-arith -Wno-parentheses -I../os-include
LD = ppc-morphos-gcc
LDFLAGS = -noixemul -nostartfiles -nostdlib
STRIP = ppc-morphos-strip -g --strip-unneeded --remove-section .comment
OUTPUT = LocalSocketCreate
OBJS = startup.o main.o

#===============================================================================

.PHONY: all dist

all: $(OUTPUT) $(OUTPUT).db

dist: $(OUTPUT) $(OUTPUT).db

clean:
	-rm -rf $(OBJS) *.bak *.s *.db $(OUTPUT)

#===============================================================================

$(OUTPUT).db: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $(OUTPUT).db

$(OUTPUT): $(OBJS)
	$(LD) -s $(LDFLAGS) $(OBJS) $(LIBS) -o $(OUTPUT).tmp
	$(STRIP) $(OUTPUT).tmp -o $(OUTPUT)
	rm $(OUTPUT).tmp
	protect $(OUTPUT) +RWED

%.o:
	@echo "Compiling $@..."
	@$(CC) -c $(CFLAGS) -o $@ $<

startup.o: startup.c main.h
main.o: main.c main.h
