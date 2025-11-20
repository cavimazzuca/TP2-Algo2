#include "src/ansi.h"
#include "src/menu.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

bool mostrar_opcion(const char *opcion, const char *comando, void *extra)
{
	printf("%s. %s\n", comando, opcion);
	return true;
}

void mostrar_menu(menu_t *menu)
{
	menu_iterar(menu, mostrar_opcion, NULL);
}

bool salir_del_menu(void *menu, void *salir)
{
	*(bool *)salir = true;
	return true;
}

void esperar_respuesta(menu_t *menu)
{
	bool salir = false;
	char comando[500];
	while (!salir) {
		int a = scanf("%s", comando);
		a--;
		menu_ejecutar(menu, comando, &salir);
	}
}

bool procesar_menu(void *menu_v)
{
	menu_t *menu = (menu_t *)menu_v;
	mostrar_menu(menu);
	esperar_respuesta(menu);
	return true;
}

int main(int argc, char *argv[])
{
	menu_t *menu_principal = menu_crear();
	menu_agregar_opcion(menu_principal, "Jugar", "S", NULL);
	menu_agregar_opcion(menu_principal, "Opciones", "O", NULL);
	menu_agregar_opcion(menu_principal, "Salir", "Q", salir_del_menu);
	procesar_menu(menu_principal);
	menu_destruir(menu_principal);
	return 0;
}
