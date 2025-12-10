#ifndef JUEGO_H_
#define JUEGO_H_
#include <stdbool.h>
#include "tp1.h"
#include "interfaz.h"

typedef struct juego juego_t;

/**
 * Crea un juego con el tp1 dado.
 * Si tp1 es NULL, el juego se crea sin datos.
 * Devuelve NULL en caso de error.
 */
juego_t *juego_crear(tp1_t *tp1);

/**
 * Inserta una copia del tp1 en el juego.
 * Devuelve true si pudo insertar el tp1.
 */
bool juego_meter_tp1(juego_t *juego, tp1_t *tp1);

/**
 * Cambia la semilla del juego y devuelve la semilla anterior.
 * Si el juego es NULL, no hace nada y devuelve 0.
 */
int cambiar_semilla(juego_t *juego, int semilla);

/**
 * Muestra las cartas por pantalla y comienza el loop de juego.
 */
void juego_iniciar(juego_t *juego);

/**
 * Libera la memoria asociada al juego.
 */
void juego_destruir(juego_t *juego);

/**
 * Cambia el estilo de la interfaz del juego.
 */
void juego_cambiar_estilo(juego_t *juego, enum estilo estilo);

#endif // JUEGO_H_