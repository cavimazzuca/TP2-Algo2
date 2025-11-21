#ifndef JUEGO_H_
#define JUEGO_H_
#include "tp1.h"

typedef struct juego juego_t;

juego_t *juego_crear(tp1_t *tp1);

bool juego_meter_tp1(juego_t *juego, tp1_t *tp1);

void cambiar_semilla(juego_t *juego, int semilla);

void juego_destruir(juego_t *juego);


#endif // JUEGO_H_