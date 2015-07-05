COMPILER=gcc -std=c99

EXECUTABLES=matrix.o list.o bnet.o k2.o
TEST_EXECUTABLES=deeter.o test.o

all:
	make $(EXECUTABLES) $(TEST_EXECUTABLES)

matrix.o: matrix.c
	$(COMPILER) -c -o $@ $^
GARBAGE+=

list.o: list.c
	$(COMPILER) -c -o $@ $^
GARBAGE+=

bnet.o: bnet.c
	$(COMPILER) -c -o $@ $^
GARBAGE+=

k2.o: matrix.o list.o bnet.o k2.c
	$(COMPILER) -o $@ $^ -lm
GARBAGE+=

deeter.o: deeter.c
	$(COMPILER) -c -o $@ $^
GARBAGE+=

test.o: deeter.o matrix.o list.o test.c
	$(COMPILER) -o $@ $^ -lm
GARBAGE+=

clean:
	rm -f $(GARBAGE) $(EXECUTABLES) $(TEST_EXECUTABLES)
