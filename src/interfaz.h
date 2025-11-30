#ifndef INTERFAZ_H_
#define INTERFAZ_H_
#include <stdio.h>
#include "menu.h"

enum estilo { ESTILO_NORMAL, ESTILO_2, ESTILO_3 };

typedef struct interfaz interfaz_t;

char *leer_terminal(FILE *archivo);

void print_estilo(const char *texto, enum estilo estilo);

void mostrar_opcion(const char *texto, const char *comando, void *estilo_v);

void leer_comando(bool (*f)(char *, void *, char *), void *ctx, char *mensaje_error);

void interfaz_menu_entrar(menu_t *menu);

void interfaz_menu_error(char* mensaje, enum estilo estilo);

#endif // INTERFAZ_H_
