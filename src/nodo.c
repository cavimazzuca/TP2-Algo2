#include "nodo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct nodo {
	void *dato;
	char *clave;
	struct nodo *siguiente;
};

nodo_t *nodo_crear(char *clave, void *dato)
{
	nodo_t *nodo = calloc(1, sizeof(nodo_t));
	if (nodo == NULL)
		return NULL;
	nodo->dato = dato;
	nodo->clave = malloc(strlen(clave) + 1);
	strcpy(nodo->clave, clave);
	return nodo;
}

void nodo_destruir(nodo_t *nodo)
{
	if (nodo == NULL)
		return;
	free(nodo->clave);
	free(nodo);
}

void nodo_destruir_todos(nodo_t *cabecera, void (*destructor)(void *))
{
	if (cabecera == NULL)
		return;
	nodo_t *siguiente = cabecera->siguiente;
	if (destructor != NULL)
		destructor(cabecera->dato);
	nodo_destruir(cabecera);
	nodo_destruir_todos(siguiente, destructor);
}

void *nodo_buscar(nodo_t *cabecera, char *clave)
{
	if (cabecera == NULL)
		return NULL;

	if (strcmp(cabecera->clave, clave) == 0)
		return cabecera->dato;

	return nodo_buscar(cabecera->siguiente, clave);
}

void nodo_insertar(nodo_t **cabecera, nodo_t *insertado, void **reemplazado,
		   size_t *cantidad)
{
	if (insertado == NULL)
		return;

	if (*cabecera == NULL) {
		if (reemplazado != NULL)
			*reemplazado = NULL;
		*cabecera = insertado;
		(*cantidad)++;
		return;
	}

	if (strcmp((*cabecera)->clave, insertado->clave) == 0) {
		if (reemplazado != NULL)
			*reemplazado = (*cabecera)->dato;

		(*cabecera)->dato = insertado->dato;
		nodo_destruir(insertado);
		return;
	}

	nodo_insertar(&((*cabecera)->siguiente), insertado, reemplazado,
		      cantidad);
}

void *nodo_quitar(nodo_t **cabecera, char *clave, size_t *cantidad)
{
	if (clave == NULL || (*cabecera) == NULL)
		return NULL;

	if (strcmp((*cabecera)->clave, clave) == 0) {
		void *dato = (*cabecera)->dato;
		nodo_t *nodo_actal = *cabecera;
		*cabecera = (*cabecera)->siguiente;
		nodo_destruir(nodo_actal);
		(*cantidad)--;
		return dato;
	}

	return nodo_quitar(&(*cabecera)->siguiente, clave, cantidad);
}

size_t nodo_iterar(nodo_t *nodo, bool (*f)(char *, void *, void *), void *ctx,
		   bool *seguir)
{
	if (nodo == NULL || !(*seguir))
		return 0;
	nodo_t *siguiente = nodo->siguiente;
	*seguir = f(nodo->clave, nodo->dato, ctx);
	return 1 + nodo_iterar(siguiente, f, ctx, seguir);
}