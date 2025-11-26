#ifndef NODO_H_
#define NODO_H_

#include <stdlib.h>
#include <stdbool.h>

typedef struct nodo nodo_t;

/* 
 * Crea un nodo y le asigna un dato.
 * Debe estar apuntando a otro nodo o a NULL.
*/
nodo_t *nodo_crear(char *clave, void *dato);

/* 
 * Libera la memoria del nodo.
 * No libera su dato ni su nodo siguiente.
 */
void nodo_destruir(nodo_t *nodo);

/*
 * Recorre los nodos siguientes desde cabecera 
 * hasta encontrar el que tenga la clave o
 * hasta que el nodo sea NULL.
 * Si no encuentra la clave, devuelve NULL.
 */
void *nodo_buscar(nodo_t *cabecera, char *clave);

void nodo_destruir_todos(nodo_t *cabecera, void (*destructor)(void *));

void nodo_insertar(nodo_t **cabecera, nodo_t *insertado, void **reemplazado,
		   size_t *cantidad);

void *nodo_quitar(nodo_t **cabecera, char *clave, size_t *cantidad);

size_t nodo_iterar(nodo_t *nodo, bool (*f)(char *, void *, void *), void *ctx,
		   bool *seguir);

#endif // NODO_H_