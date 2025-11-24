#include "src/ansi.h"
#include "src/menu.h"
#include "src/linea_comandos.h"
#include "src/tp1.h"
#include "src/juego.h"
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
	enum estilo estilo = *(enum estilo *)estilo_v;
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

void cambiar_estilo(void *estilo_v)
{
	enum estilo *estilo = (enum estilo *)estilo_v;
	switch (*estilo) {
		case ESTILO_NORMAL:
			*estilo = ESTILO_2;
			break;
		case ESTILO_2:
			*estilo = ESTILO_NORMAL;
			break;
		default:
			*estilo = ESTILO_NORMAL;
	}
	
}

int main(int argc, char *argv[])
{
	tp1_t *tp1 = tp1_leer_archivo("ejemplos/normal.csv");
	if (tp1 == NULL) {
		printf("Hubo un error al leer tu archivo de pokemones.\n");
		return ERROR;
	}
	enum estilo estilo = ESTILO_NORMAL;
	menu_t *menu_principal = menu_crear(&estilo);
	menu_t *menu_buscar = menu_crear(&estilo);
	menu_t *menu_mostrar = menu_crear(&estilo);
	juego_t *juego = juego_crear(tp1);

	menu_agregar_opcion(menu_principal, "Cargar Archivo.", "C", NULL, NULL);
	menu_agregar_opcion(menu_principal, "Buscar.", "B", entrar_al_menu, menu_buscar);
	menu_agregar_opcion(menu_principal, "Mostrar.", "M", entrar_al_menu, menu_mostrar);
	menu_agregar_opcion(menu_principal, "Jugar.", "J", NULL, NULL);
	menu_agregar_opcion(menu_principal, "Jugar con semilla.", "S", NULL, NULL);
	menu_agregar_opcion(menu_principal, "Cambiar estilo.", "E", cambiar_estilo, menu_ctx(menu_principal));
	menu_agregar_opcion(menu_principal, "Salir del juego.", "Q", salir_del_menu, menu_principal);

	menu_agregar_opcion(menu_buscar, "Volver al menú anterior.", "A", salir_del_menu, menu_buscar);
	menu_agregar_opcion(menu_buscar, "Volver al menú anterior.", "A", salir_del_menu, menu_mostrar);
	entrar_al_menu(menu_principal);



	menu_destruir(menu_principal);
	menu_destruir(menu_buscar);
	return 0;
}
