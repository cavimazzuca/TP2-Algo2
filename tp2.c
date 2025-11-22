#include "src/ansi.h"
#include "src/menu.h"
#include "src/linea_comandos.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#define ERROR -1

enum estilo {
	ESTILO_NORMAL,
	ESTILO_2
};

void mostrar_opcion(const char *texto, const char *comando, int estilo)
{
	if (estilo == ESTILO_NORMAL)
		printf("%s. %s\n", comando, texto);
	if (estilo == ESTILO_2)
		printf(ANSI_BG_BLUE ANSI_COLOR_RED "%s. %s\n", comando, texto);
	
}

bool salir_del_menu(void *menu, void *salir)
{
	*(bool *)salir = true;
	return true;
}

void jugar(void *menu, void *juego)
{
	
}

void esperar_respuesta(menu_t *menu)
{
	bool salir = false;
	while (!salir) {
		printf(ANSI_RESET_SCREEN);
		menu_mostrar(menu, mostrar_opcion, ESTILO_2);

		char *comando = leer_linea2(stdin);
		if (!menu_ejecutar(menu, comando, &salir))
			printf("No se encontró el comando especificado.\n");
		free(comando);
	}
}

bool abrir_menu(void *menu_v)
{
	menu_t *menu = (menu_t *)menu_v;
	esperar_respuesta(menu);
	return true;
}

int main(int argc, char *argv[])
{
	menu_t *menu_principal = menu_crear();
	menu_agregar_opcion(menu_principal, "Cargar Archivo", "C", NULL);
	menu_agregar_opcion(menu_principal, "Buscar", "B", NULL);
	menu_agregar_opcion(menu_principal, "Mostrar", "M", NULL);
	menu_agregar_opcion(menu_principal, "Jugar", "J", NULL);
	menu_agregar_opcion(menu_principal, "Jugar con semilla", "S", NULL);
	menu_agregar_opcion(menu_principal, "Cambiar estilo", "E", NULL);
	menu_agregar_opcion(menu_principal, "Salir del juego", "Q", NULL);

	abrir_menu(menu_principal);

	menu_destruir(menu_principal);
	return 0;
}
