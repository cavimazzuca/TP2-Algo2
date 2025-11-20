#ifndef MENU_H_
#define MENU_H_
#include <string.h>
#include <stdbool.h>
typedef struct menu menu_t;

menu_t *menu_crear();

bool menu_agregar_opcion(menu_t *menu, const char *opcion, const char * comando, bool (*f)(void *, void *));

int menu_iterar(menu_t *menu, bool (*f)(const char *, const char *, void *), void *extra);

void menu_destruir(menu_t *menu);

bool menu_contiene(menu_t *menu, const char *comando);

int menu_buscar(menu_t *menu, const char *comando);

void menu_ejecutar(menu_t *menu, const char *comando, void *extra);

#endif // MENU_H_