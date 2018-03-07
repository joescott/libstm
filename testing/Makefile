CFLAGS=-I../ 

TEST=$(basename $(wildcard test_*.c))

.PHONY: list clean test


all: $(TEST)

list:
	@echo $(TEST)

test: $(TEST)
	@echo Testing 
	$(foreach test,$(TEST), ./$(test);)

test_stm: test_stm.o ../libstm.a
	$(CC) $(CFLAGS) -g -Wall  -o $@   $^ 

clean:
	$(foreach test,$(TEST),rm -rf $(test);)
	rm -rf *.o *.db