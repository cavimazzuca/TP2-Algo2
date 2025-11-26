#include "nodo.h"
#include <stdlib.h>

/*
 * Crea un nodo y le asigna memoria.
 * Debe ser liberado usando destruir_nodo().
 * En caso de error devuelve NULL.
 */
nodo_t *nodo_crear(void *dato, nodo_t *siguiente)
{
	nodo_t *nodo = malloc(sizeof(nodo_t));
	if (nodo == NULL)
		return NULL;
	nodo->dato = dato;
	nodo->siguiente = siguiente;
	return nodo;
}

void nodo_destruir(nodo_t *nodo)
{
	if (nodo == NULL)
		return;
	free(nodo);
}