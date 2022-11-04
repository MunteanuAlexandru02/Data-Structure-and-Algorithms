# Declaratiile de variabile
CC = gcc
CFLAGS = -std=c11 -g -Wall -lm 
 
# Regula de compilare
build: tema1

tema1: tema.c
	$(CC) -o tema1 tema.c $(CFLAGS)

# Regulile de "curatenie" (se folosesc pentru stergerea fisierelor intermediare si/sau rezultate)
.PHONY : clean
clean :
	rm -f tema1
	rm -f *.out
