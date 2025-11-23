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

void mostrar_opcion(const char *texto, const char *comando, void *estilo_v)
{
	int estilo = *(int *)estilo_v;
	if (estilo == ESTILO_NORMAL)
		printf("%s. %s\n", comando, texto);
	if (estilo == ESTILO_2)
		printf(ANSI_BG_BLUE ANSI_COLOR_RED "%s. %s\n", comando, texto);
	
}

void jugar(void *menu, void *juego)
{
	
}

void esperar_respuesta(menu_t *menu)
{
	while (menu_esta_abierto(menu)) {
		printf(ANSI_RESET_SCREEN);
		menu_mostrar(menu, mostrar_opcion, menu_ctx(menu));
		char *comando = leer_linea2(stdin);
		if (!menu_ejecutar(menu, comando))
			printf("No se encontró el comando especificado.\n");
		free(comando);
	}
}

void entrar_al_menu(void *menu_v)
{
	menu_t *menu = (menu_t *)menu_v;
	menu_abrir(menu);
	esperar_respuesta(menu);
}

void salir_del_menu(void *menu_v)
{
	menu_cerrar((menu_t *)menu_v);
}

//no puedo cambiar el estilo si solo recibo un parametro.........
void cambiar_estilo(menu_t *menu)

int main(int argc, char *argv[])
{
	menu_t *menu_principal = menu_crear();

	menu_agregar_opcion(menu_principal, "Cargar Archivo", "C", NULL, NULL);
	menu_agregar_opcion(menu_principal, "Buscar", "B", NULL, NULL);
	menu_agregar_opcion(menu_principal, "Mostrar", "M", NULL, NULL);
	menu_agregar_opcion(menu_principal, "Jugar", "J", NULL, NULL);
	menu_agregar_opcion(menu_principal, "Jugar con semilla", "S", NULL, NULL);
	menu_agregar_opcion(menu_principal, "Cambiar estilo", "E", NULL, NULL);
	menu_agregar_opcion(menu_principal, "Salir del juego", "Q", salir_del_menu, menu_principal);

	entrar_al_menu(menu_principal);

	menu_destruir(menu_principal);
	return 0;
}
