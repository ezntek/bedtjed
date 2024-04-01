OBJ = main.o bedtjed.o tb2.o
CFLAGS ?= -O2 -march=native -Wall -Wpedantic
CC ?= gcc

build: $(OBJ)
	$(CC) $(CFLAGS) -o bedtjed $(OBJ)

main.o:
bedtjed.o:
tb2.o:

clean:
	rm -f $(OBJ) bedtjed

.PHONY: clean
