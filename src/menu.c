#include "menu.h"
#include <stdlib.h>
#include <ctype.h>

typedef struct opcion {
        const char *texto;
        const char *comando;
        bool (*f)(void *);
} opcion_t;

struct menu {
        opcion_t *opciones;
        int cantidad;
};

menu_t *menu_crear()
{
        menu_t *menu = malloc(sizeof(menu_t));
        menu->opciones = malloc(sizeof(opcion_t));
        menu->cantidad = 0;
        return menu;
}

opcion_t opcion_crear(const char *texto, const char *comando, bool (*f)(void *))
{
        opcion_t opcion;
        opcion.texto = texto;
        opcion.f = f;
        opcion.comando = comando;
        return opcion;
}

bool menu_agregar_opcion(menu_t *menu, const char *opcion, const char * comando, bool (*f)(void *))
{
        if (menu == NULL || opcion == NULL)
                return false;
        size_t tamaño = (size_t)menu->cantidad + 1;
        opcion_t *nuevo_bloque = realloc(menu->opciones, sizeof(opcion_t) * tamaño);
        if (nuevo_bloque == NULL)
                return false;
        menu->opciones = nuevo_bloque;
        menu->opciones[menu->cantidad] = opcion_crear(opcion, comando, f);
        menu->cantidad++;
        return true;
}

int menu_mostrar(menu_t *menu, void (*f)(const char *, const char *, int), int estilo)
{
        if (menu == NULL || f == NULL || menu->cantidad == 0)
                return 0;
        int i = 0;
        while (i < menu->cantidad) {
                f(menu->opciones[i].texto, menu->opciones[i].comando, estilo);
                i++;
        }
        return i;
}

void menu_destruir(menu_t *menu)
{
        if (menu == NULL)
                return;
        free(menu->opciones);
        free(menu);
}

int menu_buscar(menu_t *menu, const char *comando)
{
        char cmd[strlen(comando) + 1];
        strcpy(cmd, comando);
        if (menu == NULL)
                return -1;
        int i = 0;
        while (i < menu->cantidad) {
                cmd[0] = (char)tolower(comando[0]);
                if (strcmp(comando, menu->opciones[i].comando) == 0)
                        return i;
                i++;
        }
        return -1;
}

bool menu_ejecutar(menu_t *menu, const char *comando, void *extra)
{
        if (menu == NULL)
                return false;
        int pos = menu_buscar(menu, comando);
        if (pos < 0)
                return false;
        if (menu->opciones[pos].f == NULL)
                return true; // true pero no hace nada.
        if (pos >= 0)
                menu->opciones[pos].f(extra);
        return true;
}