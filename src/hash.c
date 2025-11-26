#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nodo.h"

#define CAPACIDAD_MINIMA 3
#define FACTOR_CARGA 0.75
#define MULTIPLICADOR_TAMAÑO 2

struct hash {
	nodo_t **tabla;
	size_t capacidad;
	size_t cantidad;
};

hash_t *hash_crear(size_t capacidad_inicial)
{
	if (capacidad_inicial < CAPACIDAD_MINIMA)
		capacidad_inicial = CAPACIDAD_MINIMA;

	hash_t *hash = calloc(1, sizeof(hash_t));
	if (hash == NULL)
		return NULL;

	hash->tabla = calloc(1, sizeof(void *) * capacidad_inicial);
	if (hash->tabla == NULL) {
		free(hash);
		return NULL;
	}

	hash->capacidad = capacidad_inicial;
	return hash;
}

size_t hash_cantidad(hash_t *hash)
{
	if (hash == NULL)
		return 0;
	return hash->cantidad;
}

int clave_a_int(char *clave, int i)
{
	if (clave[0] == '\0')
		return 0;
	i++;
	return (clave[0] * i) + clave_a_int(clave + 1, i);
}

int hashear(char *clave, size_t capacidad)
{
	int clave_hasheada = clave_a_int(clave, 0);
	return clave_hasheada % (int)capacidad;
}

bool transferir_nodo(char *clave, void *dato, void *nuevo_hash)
{
	return hash_insertar((hash_t *)nuevo_hash, clave, dato, NULL);
}

void hash_destruir_tabla(hash_t *hash, void (*destructor)(void *))
{
	int i = 0;
	while (i < hash->capacidad) {
		nodo_destruir_todos(hash->tabla[i], destructor);
		i++;
	}
	free(hash->tabla);
}

/* 
 * Le asigna la tabla de hash a hash_copia, sobreescribiendo la anterior.
 *
 * Si habían datos en la tabla de hash_copia, NO se liberan.
 * Después, hash y hash_copia compartirán memoria de la tabla.
 */
void hash_transferir_tabla(hash_t *hash, hash_t *hash_copia)
{
	hash_destruir_tabla(hash_copia, NULL);
	hash_copia->tabla = hash->tabla;
	hash_copia->cantidad = hash->cantidad;
	hash_copia->capacidad = hash->capacidad;
}

bool rehash(hash_t *hash)
{
	size_t nueva_capacidad = hash->capacidad * MULTIPLICADOR_TAMAÑO;
	hash_t *hash_agrandado = hash_crear(nueva_capacidad);
	if (hash_agrandado == NULL)
		return false;

	size_t transferidos;
	transferidos = hash_iterar(hash, transferir_nodo, hash_agrandado);
	if (transferidos < hash->cantidad) {
		hash_destruir(hash_agrandado);
		return false;
	}

	hash_transferir_tabla(hash_agrandado, hash);
	if (hash->tabla != hash_agrandado->tabla) {
		hash_destruir(hash_agrandado);
		return false;
	}
	free(hash_agrandado);

	return true;
}

float factor_carga(hash_t *hash)
{
	float cantidad = (float)hash->cantidad;
	float capacidad = (float)hash->capacidad;
	return cantidad / capacidad;
}

bool hash_insertar(hash_t *hash, char *clave, void *valor, void **encontrado)
{
	if (hash == NULL || clave == NULL)
		return false;
	nodo_t *insertado = nodo_crear(clave, valor);
	if (insertado == NULL)
		return false;
	int posicion = hashear(clave, hash->capacidad);
	size_t *cantidad = &hash->cantidad;
	nodo_t **lista = &hash->tabla[posicion];

	nodo_insertar(lista, insertado, encontrado, cantidad);
	if (factor_carga(hash) >= FACTOR_CARGA) {
		if (!rehash(hash)) {
			hash_quitar(hash, clave);
			return false;
		}
	}

	return true;
}

void *hash_buscar(hash_t *hash, char *clave)
{
	if (hash == NULL || clave == NULL)
		return NULL;
	int posicion = hashear(clave, hash->capacidad);
	return nodo_buscar(hash->tabla[posicion], clave);
}

bool hash_contiene(hash_t *hash, char *clave)
{
	if (hash == NULL || clave == NULL)
		return false;
	return hash_buscar(hash, clave) != NULL;
}

void *hash_quitar(hash_t *hash, char *clave)
{
	if (hash == NULL || clave == NULL)
		return NULL;
	int posicion = hashear(clave, hash->capacidad);
	return nodo_quitar(&hash->tabla[posicion], clave, &hash->cantidad);
}

size_t hash_iterar(hash_t *hash, bool (*f)(char *, void *, void *), void *ctx)
{
	if (hash == NULL || f == NULL)
		return 0;

	int i = 0;
	bool seguir = true;
	size_t cantidad = 0;

	while (i < hash->capacidad && seguir) {
		cantidad += nodo_iterar(hash->tabla[i], f, ctx, &seguir);
		i++;
	}

	return cantidad;
}

void hash_destruir(hash_t *hash)
{
	if (hash == NULL)
		return;
	hash_destruir_todo(hash, NULL);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (hash == NULL)
		return;
	hash_destruir_tabla(hash, destructor);
	free(hash);
}