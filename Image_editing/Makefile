# Declaratiile de variabile
CC = gcc
CFLAGS = -std=c11 -g -Wall -lm 
 
# Regula de compilare
build: quadtree

quadtree: main.c
	$(CC) -o quadtree main.c $(CFLAGS)

# Regulile de "curatenie" (se folosesc pentru stergerea fisierelor intermediare si/sau rezultate)
.PHONY : clean
clean :
	rm -f tema1
	rm -f *.out
