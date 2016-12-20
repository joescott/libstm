
PROGNAME=test
LIBNAME=dlist
BASE_VERSION=1
SUB_VERSION=0.0

LIB_LIST=dlist.c

MANDIR=/usr/local/man/man1/
CC=gcc
#CFLAGS=-lcurses
CFLAGS=

objects:=$(patsubst %.c,%.o,$(wildcard *.c))
objects_lib:=$(patsubst %.c,%.o,$(LIB_LIST))

$(PROGNAME): $(objects) 
	@$(CC) -o $@ $^ $(CFLAGS)

lib: $(objects_lib)
	$(CC) -shared -Wl,-soname,lib$(LIBNAME).so.$(BASE_VERSION) \
		-o lib$(LIBNAME).so.$(BASE_VERSION).$(SUB_VERSION) $^ $(CFLAGS)

%.o: %.c
	@$(CC) -fPIC $(CFLAGS) -Wall -g -c $(DEFINE) $<

clean: 
	/bin/rm -rf *.o *~ $(PROGNAME) lib$(LIBNAME).so.$(BASE_VERSION).$(SUB_VERSION)


install:
	cp ./man/$(PROGNAME).1 /usr/local/man/man1/
	cp $(PROGNAME) /usr/local/bin/
	mkdir -p /etc/ssnf/
	cp ssnf.conf /etc/ssnf/
