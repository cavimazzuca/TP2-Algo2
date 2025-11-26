#ifndef MENU_H_
#define MENU_H_
#include <string.h>
#include <stdbool.h>
typedef struct menu menu_t;

menu_t *menu_crear(void *ctx);

void menu_abrir(menu_t *menu);

void menu_cerrar(menu_t *menu);

bool menu_agregar_opcion(menu_t *menu, const char *opcion, const char *comando,
			 void (*f)(void *), void *ctx);

bool menu_ejecutar(menu_t *menu, const char *comando);

int menu_mostrar(menu_t *menu, void (*f)(const char *, const char *, void *),
		 void *ctx);

void menu_destruir(menu_t *menu);

bool menu_esta_abierto(menu_t *menu);

void menu_cambiar_ctx(menu_t *menu, void *ctx);

void *menu_ctx(menu_t *menu);

#endif // MENU_H_