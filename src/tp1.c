#include "tp1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define LETRAS_LEIDAS_POR_ITERACION 10
#define TEXTO_ELEC "ELEC"
#define TEXTO_FUEG "FUEG"
#define TEXTO_PLAN "PLAN"
#define TEXTO_AGUA "AGUA"
#define TEXTO_NORM "NORM"
#define TEXTO_FANT "FANT"
#define TEXTO_PSI "PSI"
#define TEXTO_LUCH "LUCH"
#define TEXTO_ERROR "????"
#define ERROR -1

struct tp1 {
	struct pokemon *pokemones;
	size_t cantidad;
};

/*
 * Retorna la posición de 'linea' en donde encuentra el primer '\n'.
 * 'linea' debe ser un string. 
 */
char *salto_encontrado(char *linea)
{
	return strstr(linea, "\n");
}

/*
 * Devuelve la línea actual del archivo.
 * Debe ser guardada y posteriormente liberada.
 * Si la línea está vacía, devuelve NULL.
 * En caso de error, también devuelve NULL.
 */
char *leer_linea(FILE *archivo)
{
	if (archivo == NULL)
		return NULL;
	size_t tmñ = LETRAS_LEIDAS_POR_ITERACION;
	char *linea = malloc(tmñ * sizeof(char));
	if (linea == NULL)
		return NULL;
	if (fgets(linea, (int)tmñ, archivo) == NULL) {
		free(linea);
		return NULL;
	}
	while (strlen(linea) + 1 == tmñ && strstr(linea, "\n") == NULL) {
		tmñ *= 2;
		char *nueva_linea = realloc(linea, tmñ);
		if (nueva_linea == NULL) {
			free(linea);
			return NULL;
		}
		linea = nueva_linea;
		char *siguiente_linea = &linea[strlen(linea)];
		siguiente_linea = fgets(siguiente_linea,
					(int)tmñ - (int)strlen(linea), archivo);
	}
	char *ultimo_char = &linea[strlen(linea) - 1];
	if (*ultimo_char == '\n')
		linea[strlen(linea) - 1] = '\0';

	return linea;
}

/* 
 * Convierte la primera coma en el string en '\0' y devuelve un puntero al
 * siguiente carácter.
 * Si no encuentra una coma, retorna NULL.
 */
char *desde_coma(char *string)
{
	char *coma = strstr(string, ",");
	if (coma == NULL)
		return NULL;
	*coma = '\0';
	return coma + 1;
}

/*
 * Devuelve el tipo del pokemon ingresado de formato string a formato int según
 * el enum que le corresponde definido en TP1_H_
 */
int texto_a_tipo(char *string)
{
	if (strcmp(string, TEXTO_ELEC) == 0)
		return TIPO_ELEC;
	if (strcmp(string, TEXTO_FUEG) == 0)
		return TIPO_FUEG;
	if (strcmp(string, TEXTO_PLAN) == 0)
		return TIPO_PLAN;
	if (strcmp(string, TEXTO_AGUA) == 0)
		return TIPO_AGUA;
	if (strcmp(string, TEXTO_FANT) == 0)
		return TIPO_FANT;
	if (strcmp(string, TEXTO_LUCH) == 0)
		return TIPO_LUCH;
	if (strcmp(string, TEXTO_NORM) == 0)
		return TIPO_NORM;
	if (strcmp(string, TEXTO_PSI) == 0)
		return TIPO_PSI;
	return ERROR;
}

struct pokemon *parsear_pokemon(char *linea)
{
	if (linea == NULL)
		return NULL;
	int longitud_linea = (int)strlen(linea) + 1;
	char linea_stack[longitud_linea];
	strcpy(linea_stack, linea);

	char *id = linea_stack;
	char *nombre = desde_coma(linea_stack);
	if (nombre == NULL)
		return NULL;
	char *tipo = desde_coma(nombre);
	if (tipo == NULL)
		return NULL;
	char *ataque = desde_coma(tipo);
	if (ataque == NULL)
		return NULL;
	char *defensa = desde_coma(ataque);
	if (defensa == NULL)
		return NULL;
	char *velocidad = desde_coma(defensa);
	if (velocidad == NULL)
		return NULL;

	int tipo_num = texto_a_tipo(tipo);
	if (tipo_num == ERROR)
		return NULL;

	struct pokemon *pokemon = malloc(sizeof(struct pokemon));
	if (pokemon == NULL)
		return NULL;
	pokemon->id = atoi(id);
	pokemon->nombre = malloc(sizeof(char) * (strlen(nombre) + 1));
	if (pokemon->nombre == NULL) {
		free(pokemon);
		return NULL;
	}
	strcpy(pokemon->nombre, nombre);
	pokemon->tipo = tipo_num;
	pokemon->ataque = atoi(ataque);
	pokemon->defensa = atoi(defensa);
	pokemon->velocidad = atoi(velocidad);
	bool id_invalido = pokemon->id < 0;
	bool stats_invalidas = pokemon->ataque == 0 || pokemon->defensa == 0 ||
			       pokemon->velocidad == 0;
	if (id_invalido || stats_invalidas) {
		free(pokemon->nombre);
		free(pokemon);
		return NULL;
	}

	return pokemon;
}

/*
 * Añade el pokemon 'pokemon' al vector dinámico 'pokemones' de tp1.
 * En caso de error, no hace nada.
 */
void añadir_tp1_dinamico(struct pokemon *pokemon, tp1_t *tp1)
{
	if (pokemon == NULL)
		return;
	struct pokemon *pokes_nuevos = realloc(
		tp1->pokemones, sizeof(struct pokemon) * (tp1->cantidad + 1));
	if (pokes_nuevos == NULL)
		return;
	tp1->pokemones = pokes_nuevos;
	tp1->pokemones[tp1->cantidad] = *pokemon;
	tp1->cantidad++;
}

/*
 * Devuelve un puntero a un tp1 con memoria para un pokemon.
 */
tp1_t *crear_tp1()
{
	tp1_t *tp1 = malloc(sizeof(struct tp1));
	if (tp1 == NULL)
		return NULL;
	tp1->pokemones = malloc(sizeof(struct pokemon));
	if (tp1->pokemones == NULL) {
		free(tp1);
		return NULL;
	}
	tp1->cantidad = 0;
	return tp1;
}

tp1_t *tp1_leer_archivo(const char *nombre)
{
	if (nombre == NULL)
		return NULL;
	FILE *archivo = fopen(nombre, "r");
	if (archivo == NULL)
		return NULL;
	tp1_t *tp1 = crear_tp1();
	if (tp1 == NULL) {
		fclose(archivo);
		return NULL;
	}

	char *linea = leer_linea(archivo);
	if (linea == NULL) {
		fclose(archivo);
		return tp1;
	}
	while (linea != NULL) {
		struct pokemon *pokemon = parsear_pokemon(linea);
		free(linea);
		if (pokemon != NULL) {
			if (tp1_buscar_id(tp1, pokemon->id) == NULL) {
				añadir_tp1_dinamico(pokemon, tp1);
			} else {
				free(pokemon->nombre);
			}
			free(pokemon);
		}
		linea = leer_linea(archivo);
	}
	fclose(archivo);
	return tp1;
}

void tp1_destruir(tp1_t *tp1)
{
	if (tp1 == NULL)
		return;
	int i = 0;
	while (tp1->cantidad > 0) {
		free(tp1->pokemones[i].nombre);
		i++;
		if (tp1->cantidad > 0)
			tp1->cantidad--;
	}
	free(tp1->pokemones);
	free(tp1);
}

size_t tp1_cantidad(tp1_t *tp1)
{
	if (tp1 == NULL)
		return 0;
	return tp1->cantidad;
}

const char *tipo_a_texto(int tipo)
{
	switch (tipo) {
	case TIPO_ELEC:
		return TEXTO_ELEC;
	case TIPO_AGUA:
		return TEXTO_AGUA;
	case TIPO_FANT:
		return TEXTO_FANT;
	case TIPO_PSI:
		return TEXTO_PSI;
	case TIPO_FUEG:
		return TEXTO_FUEG;
	case TIPO_LUCH:
		return TEXTO_LUCH;
	case TIPO_NORM:
		return TEXTO_NORM;
	case TIPO_PLAN:
		return TEXTO_PLAN;
	default:
		return TEXTO_ERROR;
	}
}

tp1_t *tp1_guardar_archivo(tp1_t *tp1, const char *nombre)
{
	if (tp1 == NULL)
		return NULL;
	if (nombre == NULL)
		return NULL;
	FILE *archivo = fopen(nombre, "w");
	if (archivo == NULL)
		return NULL;
	for (size_t i = 0; i < tp1->cantidad; i++) {
		fprintf(archivo, "%i,%s,%s,%i,%i,%i", tp1->pokemones[i].id,
			tp1->pokemones[i].nombre,
			tipo_a_texto(tp1->pokemones[i].tipo),
			tp1->pokemones[i].ataque, tp1->pokemones[i].defensa,
			tp1->pokemones[i].velocidad);
		if ((i + 1) < tp1->cantidad)
			fprintf(archivo, "\n");
	}
	fclose(archivo);
	return tp1;
}

tp1_t *tp1_copiar(tp1_t *tp1)
{
	if (tp1 == NULL)
		return NULL;
	tp1_t *tp1_copia = crear_tp1();
	if (tp1_copia == NULL) {
		return NULL;
	}
	size_t i = 0;

	while (i < tp1->cantidad) {
		char *nombre = tp1->pokemones[i].nombre;
		tp1_copia->pokemones[i].nombre =
			malloc((strlen(nombre) + 1) * sizeof(char));
		if (tp1_copia->pokemones[i].nombre == NULL) {
			tp1_destruir(tp1_copia);
			return NULL;
		}
		tp1_copia->cantidad++;

		strcpy(tp1_copia->pokemones[i].nombre, nombre);
		tp1_copia->pokemones[i].id = tp1->pokemones[i].id;
		tp1_copia->pokemones[i].tipo = tp1->pokemones[i].tipo;
		tp1_copia->pokemones[i].ataque = tp1->pokemones[i].ataque;
		tp1_copia->pokemones[i].defensa = tp1->pokemones[i].defensa;
		tp1_copia->pokemones[i].velocidad = tp1->pokemones[i].velocidad;
		i++;
		struct pokemon *pokemones_tmp = realloc(
			tp1_copia->pokemones,
			sizeof(struct pokemon) * i + sizeof(struct pokemon));
		if (pokemones_tmp == NULL) {
			tp1_destruir(tp1_copia);
			return NULL;
		}
		tp1_copia->pokemones = pokemones_tmp;
	}
	return tp1_copia;
}

//Copia la estructura de poke2 en poke1.
// En caso de error devuelve NULL
struct pokemon *pokecpy(struct pokemon *poke1, struct pokemon *poke2)
{
	poke1->nombre = malloc((strlen(poke2->nombre) + 1) * sizeof(char));
	if (poke1->nombre == NULL)
		return NULL;
	strcpy(poke1->nombre, poke2->nombre);
	poke1->id = poke2->id;
	poke1->tipo = poke2->tipo;
	poke1->ataque = poke2->ataque;
	poke1->defensa = poke2->defensa;
	poke1->velocidad = poke2->velocidad;
	return poke1;
}

//Añade todos los elementos de otro_tp a un_tp
tp1_t *tp1_acoplar(tp1_t *un_tp, tp1_t *tp_acoplado)
{
	if (un_tp == NULL || tp_acoplado == NULL)
		return NULL;
	size_t cnt_uno = un_tp->cantidad;
	size_t cnt_acoplado = tp_acoplado->cantidad;
	size_t cnt_final = cnt_uno + cnt_acoplado;
	if (cnt_final > 0) {
		struct pokemon *pokemones_un_tp = realloc(
			un_tp->pokemones, cnt_final * sizeof(struct pokemon));
		if (pokemones_un_tp == NULL)
			return NULL;
		un_tp->pokemones = pokemones_un_tp;
	} else {
		return NULL;
	}
	size_t fallados = 0;
	for (size_t i = 0; i < cnt_acoplado; i++) {
		struct pokemon *pokemon_id =
			tp1_buscar_id(un_tp, tp_acoplado->pokemones[i].id);
		if (pokemon_id == NULL) {
			struct pokemon *nuevo_pokemon = pokecpy(
				&un_tp->pokemones[cnt_uno + i - fallados],
				&tp_acoplado->pokemones[i]);
			if (nuevo_pokemon != NULL)
				un_tp->cantidad++;
		} else {
			fallados++;
		}
	}
	return un_tp;
}

struct pokemon *tp1_buscar_id(tp1_t *tp, int id)
{
	if (tp == NULL)
		return NULL;

	size_t i = 0;
	if (id < 0)
		return NULL;
	if (tp->pokemones == NULL)
		return NULL;
	while (i < tp->cantidad) {
		if (tp->pokemones[i].id == id) {
			return &tp->pokemones[i];
		}
		i++;
	}
	return NULL;
}

struct pokemon *tp1_buscar_nombre(tp1_t *tp, const char *nombre)
{
	if (tp == NULL)
		return NULL;
	if (nombre == NULL)
		return NULL;
	int i = 0;
	while (i < tp->cantidad) {
		if (strcmp(tp->pokemones[i].nombre, nombre) == 0) {
			return &tp->pokemones[i];
		}
		i++;
	}
	return NULL;
}

tp1_t *tp1_interseccion(tp1_t *un_tp, tp1_t *otro_tp)
{
	if (un_tp == NULL || otro_tp == NULL)
		return NULL;
	tp1_t *tp1_interseccion = crear_tp1();
	if (tp1_interseccion == NULL)
		return NULL;
	int i = 0;
	int cantidad = 0;

	while (i < un_tp->cantidad) {
		struct pokemon pokemon = un_tp->pokemones[i];
		struct pokemon *poke_intersecado =
			tp1_buscar_id(otro_tp, pokemon.id);
		struct pokemon *poke_nuevo = malloc(sizeof(struct pokemon));
		if (poke_nuevo == NULL) {
			tp1_destruir(tp1_interseccion);
			return NULL;
		}
		if (poke_intersecado != NULL) {
			struct pokemon *poke_tmp =
				pokecpy(poke_nuevo, &pokemon);
			if (poke_tmp == NULL) {
				tp1_destruir(tp1_interseccion);
				free(poke_nuevo);
				return NULL;
			}
			añadir_tp1_dinamico(poke_nuevo, tp1_interseccion);
			cantidad++;
			if (cantidad != tp1_interseccion->cantidad) {
				tp1_destruir(tp1_interseccion);
				return NULL;
			}
		}
		free(poke_nuevo);
		i++;
	}
	return tp1_interseccion;
}

tp1_t *tp1_union(tp1_t *un_tp, tp1_t *otro_tp)
{
	if (un_tp == NULL || otro_tp == NULL)
		return NULL;
	tp1_t *tp1_union = tp1_copiar(un_tp);
	if (tp1_union == NULL)
		return NULL;
	tp1_acoplar(tp1_union, otro_tp);
	return tp1_union;
}

tp1_t *tp1_diferencia(tp1_t *un_tp, tp1_t *otro_tp)
{
	if (un_tp == NULL || otro_tp == NULL)
		return NULL;
	int i = 0;
	tp1_t *tp1_diferencia = crear_tp1();
	while (i < un_tp->cantidad) {
		if (tp1_buscar_id(otro_tp, un_tp->pokemones[i].id) == NULL) {
			struct pokemon *pokemon =
				malloc(sizeof(struct pokemon));
			if (pokemon == NULL)
				return NULL;
			pokemon = pokecpy(pokemon, &un_tp->pokemones[i]);
			if (pokemon == NULL)
				return NULL;
			añadir_tp1_dinamico(pokemon, tp1_diferencia);
			free(pokemon);
		}
		i++;
	}
	return tp1_diferencia;
}

void burbujeo(int *vector, int tope)
{
	for (int i = 0; i < tope - 1; i++) {
		for (int j = 0; j < tope - 1; j++) {
			if (vector[j] > vector[j + 1]) {
				int aux = vector[j];
				vector[j] = vector[j + 1];
				vector[j + 1] = aux;
			}
		}
	}
}

size_t tp1_con_cada_pokemon(tp1_t *un_tp, bool (*f)(struct pokemon *, void *),
			    void *extra)
{
	size_t i = 0;
	if (f == NULL)
		return i;
	if (un_tp == NULL)
		return i;
	int vector_ids[un_tp->cantidad];
	for (size_t j = 0; j < un_tp->cantidad; j++) {
		vector_ids[j] = un_tp->pokemones[j].id;
	}
	burbujeo(vector_ids, (int)un_tp->cantidad);
	while (i < un_tp->cantidad) {
		struct pokemon *poke = tp1_buscar_id(un_tp, vector_ids[i]);
		bool funcion = f(poke, extra);
		i++;
		if (!funcion)
			return i;
	}

	return i;
}
