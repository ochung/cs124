CC=gcc -std=c99
CFLAGS = -ggdb3 -W -Wall -Wextra -Werror -O3

all: generate.c mst.c randmst.c
	$(CC) -o randmst generate.c mst.c randmst.c -I graph.h
