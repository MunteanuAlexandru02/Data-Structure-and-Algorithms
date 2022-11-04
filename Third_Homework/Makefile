# Declaratiile de variabile
CC = gcc
CFLAGS = -std=c11 -g -Wall -lm 
 
# Regula de compilare
build: tema3

tema3: tema3.c graf.h Djikstra.h stack.h 
	$(CC) -o tema3 tema3.c graf.h Djikstra.h $(CFLAGS)

# Regulile de "curatenie" (se folosesc pentru stergerea fisierelor intermediare si/sau rezultate)
.PHONY : clean
clean :
	rm -f tema3
	rm -f *.out
