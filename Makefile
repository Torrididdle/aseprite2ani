CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -ljansson

aseprite2ani: aseprite2ani.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f aseprite2ani

.PHONY: clean
