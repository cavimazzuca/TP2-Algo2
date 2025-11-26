#ifndef INTERFAZ_H_
#define INTERFAZ_H_
#include <stdio.h>
#include "menu.h"
enum estilo { ESTILO_NORMAL, ESTILO_2, ESTILO_3 };

typedef struct interfaz interfaz_t;

char *leer_respuesta(FILE *archivo);

void esperar_respuesta(menu_t *menu, char **msj_error);

void mostrar_opcion(const char *texto, const char *comando, void *estilo_v);

#endif // INTERFAZ_H_