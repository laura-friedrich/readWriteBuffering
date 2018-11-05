CFLAGS= -Wall -pedantic

.PHONY: all
all: myioTest

myioTest: myioTest.o myio.o
	gcc -o $@ $^

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $^

.PHONY: clean
clean:
	rm -f myioTest myioTest.o myio.o
