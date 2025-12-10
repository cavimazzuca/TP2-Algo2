#ifndef INTERFAZ_H_
#define INTERFAZ_H_
#include <stdio.h>
#include "menu.h"

enum estilo { ESTILO_NORMAL, ESTILO_2, ESTILO_3 };

/**
 * Lee una línea desde el archivo dado y la devuelve.
 * Se debe liberar la línea luego de usarla.
 */
char *leer_terminal(FILE *archivo);

/**
 * Muestra un texto con el estilo dado.
 */
void print_estilo(const char *texto, enum estilo estilo);

/**
 * Muestra mensaje_error y espera a la entrada del usuario.
 * 
 * Ejecuta la función f con la entrada, el contexto y el puntero al mensaje de error.
 * La función f debe modificar el valor de mensaje_error según sea necesario.
 * 
 * Si es usada en bucle, funciona como una interfaz de línea de comandos
 * con mensajes de error.
 */
bool leer_comando(bool (*f)(char *, void *, char *), void *ctx,
		  char *mensaje_error);

/**
 * Muestra las opciones del menú en la terminal y permite ejecutarlas
 */
void interfaz_menu_entrar(menu_t *menu);

/**
 * Crea un menú de error con el mensaje dado y lo muestra.
 */
void interfaz_menu_error(char *mensaje, enum estilo estilo);

/**
 * Escribe en la terminal el prefijo correspondiente al estilo dado.
 */
void aplicar_estilo(enum estilo estilo);

/**
 * Escribe el prefijo para resetear el estilo de la terminal.
 */
void quitar_estilo();

#endif // INTERFAZ_H_
