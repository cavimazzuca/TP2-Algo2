#ifndef MENU_H_
#define MENU_H_
#include <string.h>
#include <stdbool.h>
typedef struct menu menu_t;

/**
 * Crea un menú con el contexto dado.
 * Devuelve NULL en caso de error.
 * Se debe destruir el menú con menu_destruir().
 */
menu_t *menu_crear(void *ctx);

/**
 * Abre el menú.
 * Se puede chequear con menu_esta_abierto().
 */
void menu_abrir(menu_t *menu);

/**
 * Cierra el menú.
 * Se puede chequear con menu_esta_abierto().
 */
void menu_cerrar(menu_t *menu);

/**
 * Agrega una opción al menú y su función asociada.
 * Devuelve true si pudo agregar la opción.
 */
bool menu_agregar_opcion(menu_t *menu, const char *opcion, const char *comando,
			 void (*f)(void *), void *ctx);

/**
 * Ejecuta la opción del menú asociada al comando dado.
 * Devuelve true si pudo ejecutar la opción.
 */
bool menu_ejecutar(menu_t *menu, const char *comando);

/**
 * Muestra las opciones del menú llamando a la función f por cada opción.
 * Devuelve la cantidad de opciones que pudo mostrar.
 */
int menu_mostrar(menu_t *menu, void (*f)(const char *, const char *, void *),
		 void *ctx);

/**
 * Muestra el título del menú llamando a la función f.
 */
void menu_mostrar_titulo(menu_t *menu, void (*f)(char *, void *), void *ctx);

/**
 * Cambia el título del menú.
 */
void menu_cambiar_titulo(menu_t *menu, char *titulo);

/**
 * Libera la memoria asociada al menú.
 */
void menu_destruir(menu_t *menu);

/**
 * Devuelve true si el menú fué abierto con menu_abrir().
 */
bool menu_esta_abierto(menu_t *menu);

/**
 * Cambia el contexto asociado al menú.
 */
void menu_cambiar_ctx(menu_t *menu, void *ctx);

/**
 * Devuelve el contexto asociado al menú.
 */
void *menu_ctx(menu_t *menu);

#endif // MENU_H_