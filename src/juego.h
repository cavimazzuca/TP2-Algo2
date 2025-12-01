#ifndef JUEGO_H_
#define JUEGO_H_
#include <stdbool.h>
#include "tp1.h"
#include "menu.h"
#include "interfaz.h"

typedef struct juego juego_t;

juego_t *juego_crear(tp1_t *tp1);

bool juego_meter_tp1(juego_t *juego, tp1_t *tp1);

void cambiar_semilla(juego_t *juego, int semilla);

void juego_iniciar(juego_t *juego);

void juego_destruir(juego_t *juego);

void juego_cambiar_estilo(juego_t *juego, enum estilo estilo);

tp1_t *juego_tp1(juego_t *juego);

#endif // JUEGO_H_