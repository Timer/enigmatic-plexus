COMPILER=CC

EXECUTABLES=matrix.o list.o bnet.o k2.o

all:
	make $(EXECUTABLES)

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
	$(COMPILER) -o $@ $^
GARBAGE+=

clean:
	rm -f $(GARBAGE) $(EXECUTABLES)
