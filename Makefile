LIBNAME=stm
BASE_VERSION=1
SUB_VERSION=0.0
DOCS_DIR=docs
CC=gcc
CFLAGS=

objects:=$(patsubst %.c,%.o,$(wildcard *.c))

all: libstm.so libstm.a

doc:
	@dot && dot -Tpng $(DOCS_DIR)/graph.dot -o $(DOCS_DIR)/stm.png

libstm.so: $(objects)
	@$(CC) -shared -Wl -o $@ $^ $(CFLAGS)
#	@$(CC) -shared -Wl,-install_name,lib$(LIBNAME).so.$(BASE_VERSION) \
    -o lib$(LIBNAME).so.$(BASE_VERSION).$(SUB_VERSION) $^ $(CFLAGS)

libstm.a: $(objects)
	@$(AR) -r -o $@ $^

%.o: %.c
	@$(CC) $(CFLAGS) -Wall -g -c $(DEFINE) $<

clean: 
	/bin/rm -rf *.o *~ lib$(LIBNAME)* 
