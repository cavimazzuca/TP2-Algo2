#include "nodo_lista/nodo_lista.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define ERROR -1

typedef struct lista {
	nodo_t *cabecera;
	nodo_t *final;
	size_t cantidad;
} lista_t;

typedef struct lista_iterador {
	nodo_t *nodo;
} lista_iterador_t;

lista_t *lista_crear()
{
	lista_t *lista = malloc(sizeof(lista_t));
	if (lista == NULL)
		return NULL;
	lista->cabecera = NULL;
	lista->final = NULL;
	lista->cantidad = 0;
	return lista;
}

bool lista_vacia(lista_t *lista)
{
	if (lista == NULL)
		return true;
	if (lista->cabecera == NULL && lista->cantidad == 0)
		return true;
	return false;
}

bool lista_agregar(lista_t *lista, void *dato)
{
	if (lista == NULL)
		return false;
	nodo_t *nuevo_nodo = nodo_lista_crear(dato, NULL);
	if (nuevo_nodo == NULL)
		return NULL;
	if (lista->cabecera == NULL) {
		lista->cabecera = nuevo_nodo;
		lista->final = nuevo_nodo;
		lista->cantidad++;
		return true;
	}

	lista->final->siguiente = nuevo_nodo;
	lista->final = nuevo_nodo;
	lista->cantidad++;
	return true;
}

size_t lista_cantidad(lista_t *lista)
{
	if (lista == NULL)
		return 0;
	return lista->cantidad;
}

void lista_destruir(lista_t *lista)
{
	if (lista == NULL)
		return;
	nodo_t *nodo = lista->cabecera;
	while (nodo != NULL) {
		lista->cabecera = nodo->siguiente;
		nodo_lista_destruir(nodo);
		nodo = lista->cabecera;
	}
	free(lista);
}

void lista_destruir_todo(lista_t *lista, void (*destructor)(void *))
{
	if (lista == NULL)
		return;
	nodo_t *nodo = lista->cabecera;
	while (nodo != NULL && destructor != NULL) {
		destructor(nodo->dato);
		nodo = nodo->siguiente;
	}
	lista_destruir(lista);
}

nodo_t *lista_buscar_nodo(lista_t *lista, size_t posicion)
{
	if (lista == NULL)
		return NULL;
	nodo_t *nodo = lista->cabecera;
	if (nodo == NULL)
		return NULL;
	size_t i = 0;
	while (i < posicion) {
		nodo = nodo->siguiente;
		if (nodo == NULL)
			return NULL;
		i++;
	}
	return nodo;
}

bool lista_insertar(lista_t *lista, void *elemento, size_t posicion)
{
	// anterior->siguiente === nodo actual   ...   tiene sentido?
	if (lista == NULL)
		return false;
	if (posicion > lista_cantidad(lista) - 1)
		return false;
	nodo_t *anterior = NULL;
	nodo_t *nodo_nuevo;
	nodo_t *apuntado = lista->cabecera;
	if (apuntado == NULL)
		return false;
	if (posicion != 0) {
		anterior = lista_buscar_nodo(lista, posicion - 1);
		if (anterior == NULL)
			return NULL;
		apuntado = anterior->siguiente;
	}
	nodo_nuevo = nodo_lista_crear(elemento, apuntado);
	if (nodo_nuevo == NULL)
		return false;
	if (posicion != 0)
		anterior->siguiente = nodo_nuevo;
	else
		lista->cabecera = nodo_nuevo;
	lista->cantidad++;
	return true;
}

void *lista_buscar_elemento(lista_t *lista, size_t posicion)
{
	if (lista == NULL)
		return NULL;
	nodo_t *nodo = lista->cabecera;
	size_t i = 0;
	if (nodo == NULL)
		return NULL;
	while (i < posicion) {
		nodo = nodo->siguiente;
		if (nodo == NULL)
			return NULL;
		i++;
	}
	return nodo->dato;
}

void *lista_eliminar_elemento(lista_t *lista, size_t posicion)
{
	if (lista == NULL)
		return NULL;
	if (lista->cabecera == NULL)
		return NULL;
	nodo_t *nodo_eliminado = NULL;
	if (posicion == 0) {
		nodo_eliminado = lista->cabecera;
		lista->cabecera = nodo_eliminado->siguiente;
	}
	if (posicion > 0) {
		nodo_t *nodo_anterior = lista_buscar_nodo(lista, posicion - 1);
		if (nodo_anterior == NULL)
			return NULL;
		nodo_eliminado = nodo_anterior->siguiente;
		if (nodo_eliminado == NULL)
			return NULL;

		nodo_anterior->siguiente = nodo_eliminado->siguiente;
	}
	void *dato = nodo_eliminado->dato;
	nodo_lista_destruir(nodo_eliminado);
	lista->cantidad--;
	return dato;
}

int lista_buscar_posicion(lista_t *lista, void *elemento,
			  int (*comparador)(const void *, const void *))
{
	if (lista == NULL || comparador == NULL)
		return ERROR;
	int posicion = -1;
	int i = 0;
	nodo_t *nodo_busqueda = lista->cabecera;
	while (nodo_busqueda != NULL && posicion == -1) {
		if (comparador(elemento, nodo_busqueda->dato) == 0)
			posicion = i;
		nodo_busqueda = nodo_busqueda->siguiente;
		i++;
	}
	return posicion;
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*f)(void *, void *),
			       void *extra)
{
	size_t cantidad = 0;
	if (lista == NULL || f == NULL)
		return cantidad;
	nodo_t *nodo = lista->cabecera;
	while (nodo != NULL) {
		cantidad++;
		if (f(nodo->dato, extra) == false)
			return cantidad;
		nodo = nodo->siguiente;
	}
	return cantidad;
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (lista == NULL)
		return NULL;
	lista_iterador_t *it = malloc(sizeof(lista_iterador_t));
	if (it == NULL)
		return NULL;
	it->nodo = lista->cabecera;
	return it;
}

bool lista_iterador_hay_mas_elementos(lista_iterador_t *it)
{
	if (it == NULL)
		return false;
	return it->nodo != NULL;
}

void lista_iterador_siguiente(lista_iterador_t *it)
{
	if (it == NULL)
		return;
	if (it->nodo == NULL)
		return;
	it->nodo = it->nodo->siguiente;
}

void *lista_iterador_obtener_actual(lista_iterador_t *it)
{
	if (it == NULL)
		return NULL;
	if (it->nodo == NULL)
		return NULL;
	return it->nodo->dato;
}

void lista_iterador_destruir(lista_iterador_t *it)
{
	if (it == NULL)
		return;
	free(it);
}