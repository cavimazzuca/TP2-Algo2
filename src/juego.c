#include "juego.h"
#include "lista.h"
#include "tp1.h"
#define CANTIDAD_PARES 9

typedef struct jugador {
    lista_t *jugadas;
    int puntos;
} jugador_t;

struct juego {
    tp1_t *tp1;
    lista_t *pokemones;
    jugador_t jugador1;
    jugador_t jugador2;
};

bool meter_en_lista(struct pokemon *pokemon, void *lista)
{
    //random
    size_t random = 1;
    void *existente = lista_buscar_elemento((lista_t *)lista, random);
    if (existente == NULL) {
        //return meter_en_lista...
    }
    return lista_insertar((lista_t *)lista, pokemon, random);
}

juego_t *juego_crear(tp1_t *tp1)
{
    juego_t *juego = malloc(sizeof(juego_t));
    if (juego == NULL)
        return NULL;
    juego->pokemones = lista_crear();
    if (!juego_meter_tp1(juego, tp1)) {
        juego_destruir(juego);
        return false;
    }
    return juego;
}

bool juego_meter_tp1(juego_t *juego, tp1_t *tp1)
{
    if (juego == NULL)
        return false;
    juego->tp1 = tp1;
    if (tp1 == NULL)
        return true;
    size_t i = tp1_con_cada_pokemon(tp1, meter_en_lista, juego->pokemones);
    if (i < tp1_cantidad(juego->tp1))
        return false;
    return true;
}

void cambiar_semilla(juego_t *juego, int semilla)
{
}

void juego_destruir(juego_t *juego)
{
    lista_destruir(juego->pokemones);
    free(juego);
}
