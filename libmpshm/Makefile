.PHONY: all clean

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld

vpath %.c   ../target/:./
vpath %.h   ../target/:./
vpath %.o   ./

CFLAGS = -Wall -std=gnu99 -fPIC -g
#CFLAGS += -I ../Src
LDFLAGS = -g

all: libmpshm.so mpshm

%.d: %.c
	@set -e; $(CC) -MM $(CFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@

%.o: %.c
	@$(RM) $@
	$(CC) $(CFLAGS) -o $@ -c $< 

libmpshm.so:	libmpshm.o
	$(LD) -shared -soname libmpshm.so -o ../target/libmpshm.so libmpshm.o $(LDFLAGS)

mpshm:	mpshm.o
	@$(CC) -o ../target/mpshm mpshm.o $(LDFLAGS) -L ../target -l mpshm -l pthread -l rt

clean:
	@rm -rf *.o *.d *.err libmpshm.so mpshm
