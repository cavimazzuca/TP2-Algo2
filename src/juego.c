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
#define JUGADAS_MOSTRADAS 5

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
	int jugador;
} jugada_t;

struct juego {
	tp1_t *tp1;
	hash_t *pokemones;
	int turno;
	int encontrados;
	enum estilo estilo;
	int puntos_j2;
	int puntos_j1;
	carta_t *ultima_volteada;
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

void juego_cambiar_estilo(juego_t *juego, enum estilo estilo)
{
	juego->estilo = estilo;
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
	if (jugada_v == NULL)
		return;
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
	juego->ultimas_jugadas = lista_crear();
	if (juego->ultimas_jugadas == NULL) {
		hash_destruir(juego->pokemones);
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
	lista_destruir_todo(juego->ultimas_jugadas, jugada_destruir);
	free(juego);
}

void juego_cerrar(void *menu_v)
{
	menu_t *menu = (menu_t *)menu_v;
	menu_cerrar(menu);
}

bool mostrar_jugada(void *jugada_v, void *estilo_v)
{
	jugada_t *jugada = (jugada_t *)jugada_v;
	if (jugada == NULL)
		return false;
	enum estilo estilo = *(enum estilo *)estilo_v;
	aplicar_estilo(estilo);
	if (jugada->coinciden)
		printf("Jugador %i: %s -> %s :)\n", jugada->jugador, jugada->num1, jugada->num2);
	else
		printf("Jugador %i: %s -> %s :(\n", jugada->jugador, jugada->num1, jugada->num2);
	quitar_estilo();
	return true;
}

void mostrar_jugadas(juego_t *juego)
{
	lista_con_cada_elemento(juego->ultimas_jugadas, mostrar_jugada, &(juego->estilo));
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

void mostrar_ultimas_jugadas(juego_t *juego)
{
	aplicar_estilo(juego->estilo);
	printf("Últimas jugadas: \n");
	size_t cantidad = lista_cantidad(juego->ultimas_jugadas);
	for (size_t i = 0; i < JUGADAS_MOSTRADAS; i++) {
		size_t exceso = 0;
		if (cantidad >= JUGADAS_MOSTRADAS)
			exceso = cantidad - JUGADAS_MOSTRADAS;
		void *jugada = lista_buscar_elemento(juego->ultimas_jugadas, exceso + i);
		if (jugada != NULL)
			mostrar_jugada(jugada, &juego->estilo);
		else
			printf("\n");
	}
	quitar_estilo();
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
	mostrar_ultimas_jugadas(juego);
	if (juego->turno % 2 == 0)
		print_estilo("Le toca al Jugador 2.", juego->estilo);
	else
		print_estilo("Le toca al Jugador 1.", juego->estilo);	
}

jugada_t *jugada_crear()
{
	jugada_t *jugada = malloc(sizeof(jugada_t));
	jugada->num1 = malloc(sizeof(char) * 4);
	jugada->num2 = malloc(sizeof(char) * 4);
	jugada->jugador = 1;
	return jugada;
}

bool procesar_jugada(char *comando, void *juego_v, char *mensaje_error)
{
	juego_t *juego = (juego_t *)juego_v;
	carta_t *carta_volteada = hash_buscar(juego->pokemones, comando);
	strcpy(mensaje_error, "");
	if (carta_volteada == NULL) {
		strcpy(mensaje_error, ANSI_COLOR_RED "Debes seleccionar una carta válida." ANSI_COLOR_RESET);
		return false;
	}
	if (carta_volteada->encontrado) {
		strcpy(mensaje_error, ANSI_COLOR_RED "La carta seleccionada ya ha sido volteada." ANSI_COLOR_RESET);
		return false;
	}
	if (juego->ultima_volteada == NULL) {
		juego->ultima_volteada = carta_volteada;
		carta_volteada->seleccionada = true;
		return true;
	}
	if (carta_volteada == juego->ultima_volteada) {
		strcpy(mensaje_error, ANSI_COLOR_RED "La carta seleccionada ya ha sido volteada." ANSI_COLOR_RESET);
		return false;
	}

	jugada_t *jugada = jugada_crear();
	strcpy(jugada->num1,juego->ultima_volteada->numero);
	strcpy(jugada->num2,carta_volteada->numero);
	jugada->coinciden = false;
	jugada->jugador = 1;
	if (juego->turno % 2 == 0) 
		jugada->jugador = 2;
	lista_agregar(juego->ultimas_jugadas, jugada);

	carta_volteada->seleccionada = true;
	if (carta_volteada->pokemon->id == juego->ultima_volteada->pokemon->id) {
		jugada->coinciden = true;
		carta_volteada->encontrado = true;
		juego->ultima_volteada->encontrado = true;
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
	juego->ultima_volteada->seleccionada = false;
	juego->ultima_volteada = NULL;
	juego->turno++;
	return true;
}

void juego_mostrar_resultados(juego_t *juego)
{
	printf(ANSI_RESET_SCREEN);
	aplicar_estilo(juego->estilo);
	printf("Jugador 1\n");
	printf(" Puntaje: %i\n", juego->puntos_j1);
	printf("Jugador 2\n");
	printf(" Puntaje: %i\n", juego->puntos_j2);
	printf("\n");
	quitar_estilo();
	print_estilo("Jugadas: ", juego->estilo);
	mostrar_jugadas(juego);
	free(leer_terminal(stdin));
}

void juego_resetear(juego_t *juego)
{
	juego->encontrados = 0;
	juego->puntos_j1 = 0;
	juego->puntos_j2 = 0;
	lista_destruir_todo(juego->ultimas_jugadas, jugada_destruir);
	juego->ultimas_jugadas = lista_crear();
	juego->turno = 1;
}

void loop_juego(juego_t *juego)
{
	char mensaje_error[500] = "";
	while (juego->encontrados < CANTIDAD_PARES) {
		printf(ANSI_RESET_SCREEN);
		mostrar_cartas(juego);
		leer_comando(procesar_jugada, juego, mensaje_error);
	}
	juego_mostrar_resultados(juego);
	juego_resetear(juego);
}

void juego_iniciar(juego_t *juego)
{
	srand(juego->semilla);
	juego->semilla = (unsigned int)time(NULL);
	if (juego->tp1 == NULL) {
		interfaz_menu_error("Se debe incluir un archivo para jugar.", juego->estilo);
		return;
	}
	

	juego_meter_tp1(juego, juego->tp1);
	loop_juego(juego);
}