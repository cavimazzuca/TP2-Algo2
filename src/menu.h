#ifndef MENU_H_
#define MENU_H_
#include <string.h>
#include <stdbool.h>
typedef struct menu menu_t;

menu_t *menu_crear();

bool menu_agregar_opcion(menu_t *menu, const char *opcion, const char * comando, bool (*f)(void *, void *));

int menu_mostrar(menu_t *menu, void (*f)(const char *, const char *, int), int estilo);

void menu_destruir(menu_t *menu);

void menu_ejecutar(menu_t *menu, const char *comando, void *extra);

#endif // MENU_H_