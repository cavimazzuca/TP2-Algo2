VALGRIND_FLAGS=-s --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes
CFLAGS =-std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g
CC = gcc
CLANGFLAGS =-i -style=file

all: clean valgrind-alumno

valgrind-alumno: pruebas_alumno
	valgrind $(VALGRIND_FLAGS) ./pruebas_alumno

pruebas_alumno:
	$(CC) $(CFLAGS) src/*.c pruebas_alumno.c -o pruebas_alumno

clean:
	rm -f pruebas_alumno

clang:
	clang-format $(CLANGFLAGS) *.c src/*.c src/menu.h

compilar_tp2:
	$(CC) $(CFLAGS) src/*.c tp2.c -o tp2

tp2: compilar_tp2
	valgrind $(VALGRIND_FLAGS) ./tp2