
CC=gcc
CFLAGS=-Wall -fpic -c -pedantic -Wextra -std=c99

OBJS_DIR=objs

OBJS=objs/src/ic_list.o

TEST_LIBS=-lcheck
TEST_OBJS=$(OBJS_DIR)/tests/check_ic_list.o

test: clean $(TEST_OBJS) $(OBJS)
	@$(CC) -o $@ $(TEST_OBJS) $(OBJS) $(TEST_LIBS)
	@./$@

test_mem:
	CK_FORK=no valgrind --leak-check=full --error-exitcode=1 ./test

test_all: test test_mem

clean:
	@rm -rf test util $(OBJS_DIR)

$(OBJS_DIR):
	-@mkdir -p $(OBJS_DIR)/src $(OBJS_DIR)/tests $(OBJS_DIR)/utils

$(OBJS_DIR)/%.o: %.c | $(OBJS_DIR)
	@$(CC) -o $@ $< $(CFLAGS)


.PHONY: clean test_mem