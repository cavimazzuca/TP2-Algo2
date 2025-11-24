#include "menu.h"
#include <stdlib.h>
#include <ctype.h>

typedef struct opcion {
        const char *texto;
        const char *comando;
        void *ctx;
        void (*f)(void *);
} opcion_t;

struct menu {
        opcion_t *opciones;
        int cantidad;
        bool abierto;
        void *ctx;
};

menu_t *menu_crear(void *ctx)
{
        menu_t *menu = malloc(sizeof(menu_t));
        menu->opciones = malloc(sizeof(opcion_t));
        menu->cantidad = 0;
        menu->abierto = false;
        menu->ctx = ctx;
        return menu;
}

opcion_t opcion_crear(const char *texto, const char *comando, void (*f)(void *), void *ctx)
{
        opcion_t opcion;
        opcion.texto = texto;
        opcion.f = f;
        opcion.ctx = ctx;
        opcion.comando = comando;
        return opcion;
}

bool menu_agregar_opcion(menu_t *menu, const char *opcion, const char * comando, void (*f)(void *), void *ctx)
{
        if (menu == NULL || opcion == NULL)
                return false;
        size_t tamaño = (size_t)menu->cantidad + 1;
        opcion_t *nuevo_bloque = realloc(menu->opciones, sizeof(opcion_t) * tamaño);
        if (nuevo_bloque == NULL)
                return false;
        menu->opciones = nuevo_bloque;
        menu->opciones[menu->cantidad] = opcion_crear(opcion, comando, f, ctx);
        menu->cantidad++;
        return true;
}

int menu_mostrar(menu_t *menu, void (*f)(const char *, const char *, void *), void *ctx)
{
        if (menu == NULL || f == NULL || menu->cantidad == 0)
                return 0;
        int i = 0;
        while (i < menu->cantidad) {
                f(menu->opciones[i].texto, menu->opciones[i].comando, ctx);
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

void a_minusculas(char *texto, size_t tope)
{
        for (int i = 0; i < tope; i++)
                texto[i] = (char)tolower(texto[i]);
}

int strcmp_caps(const char *a, const char *b)
{
        char a_min[strlen(a) + 1];
        strcpy(a_min, a);
        a_minusculas(a_min, strlen(a));

        char b_min[strlen(b) + 1];
        strcpy(b_min, b);
        a_minusculas(b_min, strlen(b));

        return strcmp(a_min, b_min);
}

int menu_buscar(menu_t *menu, const char *comando)
{
        if (menu == NULL)
                return -1;
        int i = 0;
        while (i < menu->cantidad) {
                if (strcmp_caps(comando, menu->opciones[i].comando) == 0)
                        return i;
                i++;
        }
        return -1;
}

bool menu_ejecutar(menu_t *menu, const char *comando)
{
        if (menu == NULL)
                return false;
        int pos = menu_buscar(menu, comando);
        if (pos < 0)
                return false;
        if (menu->opciones[pos].f == NULL)
                return true; // si f es NULL no hace nada.
        if (pos >= 0)
                menu->opciones[pos].f(menu->opciones[pos].ctx);
        return true;
}

void menu_cerrar(menu_t *menu)
{
        if (menu == NULL)
                return;
        menu->abierto = false;
}

void menu_abrir(menu_t *menu)
{
        if (menu == NULL)
                return;
        menu->abierto = true;
}

bool menu_esta_abierto(menu_t *menu)
{
        if (menu == NULL)
                return false;
        return menu->abierto;
}

void menu_cambiar_ctx(menu_t *menu, void *ctx)
{
        if (menu == NULL)       
                return;
        menu->ctx = ctx;
}

void *menu_ctx(menu_t *menu)
{
        return menu->ctx;
}