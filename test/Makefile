
TEST=stm_test

INCLUDES= -I. -I../ -I../dlinkedlist
CFLAGS= -MD -g -Wall -DNDEBUG
LDFLAGS= -L. -L../ -lstm

.PHONY: all clean 

OBJS_ALL=$(patsubst %.c,%.o,$(wildcard *.c))

all: $(TEST)

$(TEST): $(OBJS_ALL)
	@$(CC) $^ $(LDFLAGS) -o $@  

%.o: %.c
	@$(CC) $(INCLUDES) $(CFLAGS) -Wall -g -c $(DEFINE) $<

clean: 
	@/bin/rm -rf *.o *~ *.d $(TEST)
