#ifndef NODO_H_
#define NODO_H_

typedef struct nodo {
	void *dato;
	struct nodo *siguiente;
} nodo_t;

/* 
 * Crea un nodo y le asigna un dato.
 * Debe estar apuntando a otro nodo o a NULL.
*/
nodo_t *nodo_crear(void *dato, nodo_t *siguiente);

/* 
 * Libera la memoria del nodo.
 * No libera su dato ni su nodo siguiente.
 */
void nodo_destruir(nodo_t *nodo);

#endif // NODO_H_