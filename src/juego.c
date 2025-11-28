#include "juego.h"
#include "ansi.h"
#include "interfaz.h"
#include "hash.h"
#include "lista.h"
#include <time.h>
#include <stdio.h>
#include "tp1.h"
#include <unistd.h>
#define CANTIDAD_PARES 9

typedef struct carta {
	struct pokemon *pokemon;
	bool encontrado;
	bool seleccionada;
	char *numero;
} carta_t;

typedef struct jugada {
	char *num1;
	char *num2;
	bool coinciden;
} jugada_t;

struct juego {
	tp1_t *tp1;
	hash_t *pokemones;
	int turno;
	int encontrados;
	enum estilo estilo;
	int puntos_j2;
	int puntos_j1;
	lista_t *ultimas_jugadas;
	unsigned int semilla;
};

int posicion_aleatoria(hash_t *hash)
{
	int random = rand() % (CANTIDAD_PARES * 2);
	char num_carta[4];
	snprintf(num_carta, sizeof(num_carta), "%d", random);
	while (hash_contiene(hash, num_carta)) {
		random = rand() % (CANTIDAD_PARES * 2);
		snprintf(num_carta, sizeof(num_carta), "%d", random);
	}
	return random;
}

carta_t *carta_crear()
{
	carta_t *carta = calloc(1, sizeof(carta_t));
	carta->numero = malloc(sizeof(char) * 4);
	return carta;
}

void carta_destruir(void *carta_v)
{
	carta_t *carta = (carta_t *)carta_v;
	free(carta->numero);
	free(carta);
}

void jugada_destruir(void *jugada_v)
{
	jugada_t *jugada = (jugada_t *)jugada_v;
	free(jugada->num1);
	free(jugada->num2);
	free(jugada);
}

bool meter_en_hash(struct pokemon *pokemon, void *hash)
{
	if (hash_cantidad(hash) >= (CANTIDAD_PARES * 2))
		return false;
	int random = posicion_aleatoria(hash);
	char num_carta[4];
	snprintf(num_carta, sizeof(num_carta), "%d", random);
	carta_t *carta1 = carta_crear();
	carta_t *carta2 = carta_crear();
	carta1->pokemon = pokemon;
	carta2->pokemon = pokemon;
	strcpy(carta1->numero, num_carta);

	bool insertado = hash_insertar((hash_t *)hash, num_carta, carta1, NULL);
	if (!insertado)
		return false;
	random = posicion_aleatoria(hash);
	snprintf(num_carta, sizeof(num_carta), "%d", random);
	strcpy(carta2->numero, num_carta);
	insertado = hash_insertar((hash_t *)hash, num_carta, carta2, NULL);
	return insertado;
}

juego_t *juego_crear(tp1_t *tp1)
{
	juego_t *juego = calloc(1, sizeof(juego_t));
	juego->semilla = (unsigned int)time(NULL);
	if (juego == NULL)
		return NULL;
	juego->pokemones = hash_crear(CANTIDAD_PARES * 2);
	if (juego->pokemones == NULL) {
		free(juego);
		return NULL;
	}
	juego->turno = 1;
	juego_meter_tp1(juego, tp1);
	juego->estilo = ESTILO_NORMAL;
	return juego;
}

tp1_t *copiar_tp1(tp1_t *tp1)
{
	tp1_t *copia = tp1_union(tp1, tp1);
	return copia;
}

bool juego_meter_tp1(juego_t *juego, tp1_t *copia)
{
	if (copia == NULL) {
		juego->tp1 = NULL;
		return false;
	}
	if (juego == NULL)
		return false;
	if (tp1_cantidad(copia) < CANTIDAD_PARES) {
		printf("Hay muy pocos pokemones para jugar.\n");
		return false;
	}
	tp1_t *tp1 = copiar_tp1(copia);
	tp1_destruir(juego->tp1);
	hash_destruir_todo(juego->pokemones, carta_destruir);
	juego->pokemones = hash_crear(CANTIDAD_PARES * 2);
	juego->tp1 = tp1;
	if (tp1 == NULL)
		return false;
	tp1_con_cada_pokemon(tp1, meter_en_hash, juego->pokemones);
	return true;
}

void cambiar_semilla(juego_t *juego, int semilla)
{
	juego->semilla = (unsigned int)semilla;
}

void juego_destruir(juego_t *juego)
{
	tp1_destruir(juego->tp1);
	hash_destruir_todo(juego->pokemones, carta_destruir);
	free(juego);
}

void juego_cerrar(void *menu_v)
{
	menu_t *menu = (menu_t *)menu_v;
	menu_cerrar(menu);
}

void mostrar_jugadas(juego_t *juego)
{
}

bool mostrar_carta(char *clave, void *carta_v, void *contador_v)
{
	carta_t *carta = (carta_t *)carta_v;
	if (*((int *)contador_v) % 3 == 0)
		printf("\n");
	printf("[ ");
	if (atoi(clave) < 10)
		printf(" ");
	if (carta->encontrado) {
		printf(ANSI_COLOR_GREEN);
		printf("%s", carta->pokemon->nombre);
		printf(ANSI_COLOR_RESET);
		printf(" ] ");
		return true;
	}

	if (carta->seleccionada) {
		printf(ANSI_COLOR_BLUE);
		printf("%s", carta->pokemon->nombre);
		printf(ANSI_COLOR_RESET);
		printf(" ] ");
		return true;
	}

	printf("%s", clave);

	printf(ANSI_COLOR_RESET);
	printf(" ] ");
	return true;
}

void mostrar_cartas(juego_t *juego)
{
	for (int i = 0; i < CANTIDAD_PARES * 2; i++) {
		char num_carta[4];
		snprintf(num_carta, sizeof(num_carta), "%d", i);
		carta_t *carta = hash_buscar(juego->pokemones, num_carta);
		mostrar_carta(num_carta, carta, &i);
	}
	printf("\n");
}

jugada_t *jugada_crear()
{
	jugada_t *jugada = malloc(sizeof(jugada_t));
	jugada->num1 = malloc(sizeof(char) * 4);
	jugada->num2 = malloc(sizeof(char) * 4);
	return jugada;
}

void procesar_jugada(char *comando, juego_t *juego, carta_t **carta_anterior)
{
	carta_t *carta_volteada = hash_buscar(juego->pokemones, comando);
	if (carta_volteada == NULL)
		return;
	if (carta_volteada->encontrado)
		return;
	if (*carta_anterior == NULL) {
		*carta_anterior = carta_volteada;
		carta_volteada->seleccionada = true;
		return;
	}
	if (carta_volteada == *carta_anterior)
		return;

	/*
    jugada_t *jugada = jugada_crear();
    strcpy(jugada->num1,(*carta_anterior)->numero);
    strcpy(jugada->num2,carta_volteada->numero);
    jugada->coinciden = false;
    lista_agregar(juego->ultimas_jugadas, jugada);
    */

	carta_volteada->seleccionada = true;
	if (carta_volteada->pokemon->id == (*carta_anterior)->pokemon->id) {
		//jugada->coinciden = true;
		carta_volteada->encontrado = true;
		(*carta_anterior)->encontrado = true;
		juego->encontrados++;
		if (juego->turno % 2 == 0)
			juego->puntos_j2++;
		else
			juego->puntos_j1++;
	}
	printf(ANSI_RESET_SCREEN);
	mostrar_cartas(juego);
	sleep(1);
	carta_volteada->seleccionada = false;
	(*carta_anterior)->seleccionada = false;
	*carta_anterior = NULL;
	juego->turno++;
}

void juego_mostrar_resultados(juego_t *juego)
{
	printf(ANSI_RESET_SCREEN);
	printf("Jugador 1\n");
	printf(" Puntaje: %i\n", juego->puntos_j1);
	printf("Jugador 2\n");
	printf(" Puntaje: %i\n", juego->puntos_j2);
	sleep(2);
	//mostrar todas las jugadas separadas por jugador
}

void juego_resetear(juego_t *juego)
{
	juego->encontrados = 0;
	juego->puntos_j1 = 0;
	juego->puntos_j2 = 0;
	lista_destruir_todo(juego->ultimas_jugadas, jugada_destruir);
	juego->ultimas_jugadas = NULL;
	juego->turno = 1;
}

void loop_juego(juego_t *juego)
{
	carta_t *carta_volteada = NULL;
	while (juego->encontrados < CANTIDAD_PARES) {
		printf(ANSI_RESET_SCREEN);
		mostrar_cartas(juego);
		mostrar_jugadas(juego);
		if (juego->turno % 2 == 0)
			printf("Le toca al Jugador 2.\n");
		else
			printf("Le toca al Jugador 1.\n");
		char *comando = leer_terminal(stdin);
		procesar_jugada(comando, juego, &carta_volteada);
		free(comando);
	}
	juego_mostrar_resultados(juego);
	juego_resetear(juego);
}

void juego_iniciar(juego_t *juego)
{
	srand(juego->semilla);
	juego->semilla = (unsigned int)time(NULL);
	enum estilo estilo = ESTILO_NORMAL;
	menu_t *menu = menu_crear(&estilo);
	if (juego->tp1 == NULL) {
		menu_agregar_opcion(menu, "Volver", "A", juego_cerrar, menu);
		menu_abrir(menu);
		char *msj_error = "\n";
		msj_error = ANSI_COLOR_RED
			"Se debe incluir un archivo antes de jugar.\n" ANSI_COLOR_RESET;
		leer_opciones_menu(menu, &msj_error);
		menu_destruir(menu);
		return;
	}
	juego->ultimas_jugadas = lista_crear();
	juego_meter_tp1(juego, juego->tp1);
	loop_juego(juego);
	menu_destruir(menu);
}