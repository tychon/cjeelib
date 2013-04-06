
CFLAGS := -g -std=c99 -Wall -Werror
LDFLAGS := -g
LIBS :=

all: reader.x

%.x: %.o
	gcc $^ $(LIBS) -o $*.x

%.o: %.c
	gcc $(CFLAGS) -c $*.c -o $*.o

clean:
	rm -f *.x *.o
