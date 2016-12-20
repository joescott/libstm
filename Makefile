
LIBNAME=libstm

LIBDLINK_DIR=dlinkedlist
LIBDLINK_OBJ=dlist.o

INCLUDES= -I. -I$(LIBDLINK_DIR)
CFLAGS= -MD -g -Wall -DNDEBUG
LDFLAGS= -L.

.PHONY: all clean dlist

OBJS_ALL=$(patsubst %.c,%.o,$(wildcard *.c))
OBJS_LIBS:=$(filter-out main.o,$(OBJS_ALL))

all: dlist $(LIBNAME).a test 

$(LIBNAME).a: $(OBJS_LIBS)
	@$(AR) rcs $@ $(LIBDLINK_DIR)/$(LIBDLINK_OBJ) $^

dlist:
	@$(MAKE) -s -C $(LIBDLINK_DIR)

%.o: %.c
	@$(CC) $(INCLUDES) $(CFLAGS) -Wall -g -c $(DEFINE) $<

test:
	@$(MAKE) -s -C test

clean: 
	@$(MAKE) -s -C $(LIBDLINK_DIR) clean
	@$(MAKE) -s -C test clean
	@/bin/rm -rf *.o *~ *.d $(LIBNAME).a

doc:
	@dot -Tpng graph.dot -o stm.png
