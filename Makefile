CC=gcc -std=c99

default: randmst
	gcc -o randmst generate.c mst.c randmst.c 
