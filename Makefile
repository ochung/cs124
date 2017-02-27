CC=gcc -std=c99
CFLAGS = -ggdb3 -W -Wall -Wextra -Werror -O3

all: prune.c generate.c mst.c randmst.c
	$(CC) -o randmst prune.c generate.c mst.c randmst.c -I graph.h
