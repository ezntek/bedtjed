OBJ = main.o bedtjed.o tb2.o
CFLAGS ?= -O2 -march=native -Wall -Wpedantic
CC ?= gcc

build: $(OBJ)
	$(CC) $(CFLAGS) -o bedtjed $(OBJ)

main.o:
bedtjed.o:
tb2.o:
	$(CC) $(CFLAGS) -DTB_IMPL -c -o tb2.o tb2.c

clean:
	rm -f $(OBJ) bedtjed

.PHONY: clean
