CC=gcc -std=c99
CFLAGS = -ggdb3 -W -Wall -Wextra -Werror -O3
LDFLAGS =
LIBS =

default: randmst
	gcc -o randmst generate.c mst.c randmst.c 
