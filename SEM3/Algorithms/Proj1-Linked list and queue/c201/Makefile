PROJECT=c201
TESTS=$(PROJECT)-test $(PROJECT)-advanced-test
TEST_OUTPUTS=$(PROJECT)-test.output $(PROJECT)-advanced-test.output
PROG=$(PROJECT)-test
CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic -lm -fcommon

.PHONY: all run tests clean

all: $(PROG)

run: $(PROG)
	@./$(PROG) > current-test.output
	@echo "\nTest output differences:"
	@diff -su $(PROG).output current-test.output
	@rm -f current-test.output

$(PROJECT)-test: $(PROJECT).c $(PROJECT)-test.c $(PROJECT)-test-utils.c
	$(CC) $(CFLAGS) -o $@ $(PROJECT).c $(PROJECT)-test.c $(PROJECT)-test-utils.c

$(PROJECT)-test.output: $(PROJECT)-test
	./$(PROJECT)-test > $(PROJECT)-test.output

$(PROJECT)-advanced-test: $(PROJECT).c $(PROJECT)-advanced-test.c $(PROJECT)-test-utils.c
	$(CC) $(CFLAGS) -o $@ $(PROJECT).c $(PROJECT)-advanced-test.c $(PROJECT)-test-utils.c

$(PROJECT)-advanced-test.output: $(PROJECT)-advanced-test
	./$(PROJECT)-advanced-test > $(PROJECT)-advanced-test.output

tests: $(TEST_OUTPUTS)

zadani: $(PROJECT)-test.output
	@mkdir $@ 2>/dev/null || echo "Directory exists, continuing"
	@unifdef -DSTUDENT_BUILD=1 -o $@/$(PROJECT).c $(PROJECT).c || printf ""
	@cp $(PROJECT).h $@/
	@cp $(PROG).c $@/
	@cp $(PROJECT)-test-utils.* $@/
	@cp $(PROJECT)-test.output $@/
	@cp Makefile $@/
	@tar -czvf $(PROJECT)-`date +%Y-%m-%d-%H-%M`.tar.gz $@/*

reseni:
	@mkdir $@

clean:
	@rm -f *.o $(PROG) $(TESTS)
	@rm -rf zadani reseni
