#include "interfaz.h"
#include "ansi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct interfaz {
	char *titulo;
	menu_t *menu;
	char *pie;
	enum estilo estilo;
};


//Función reutiizada de TP1
char *leer_terminal(FILE *archivo)
{
	if (archivo == NULL)
		return NULL;
	size_t tmñ = 123;
	char *linea = malloc(tmñ * sizeof(char));
	if (linea == NULL)
		return NULL;
	if (fgets(linea, (int)tmñ, archivo) == NULL) {
		free(linea);
		return NULL;
	}
	while (strlen(linea) + 1 == tmñ && strstr(linea, "\n") == NULL) {
		tmñ *= 2;
		char *nueva_linea = realloc(linea, tmñ);
		if (nueva_linea == NULL) {
			free(linea);
			return NULL;
		}
		linea = nueva_linea;
		char *siguiente_linea = &linea[strlen(linea)];
		siguiente_linea = fgets(siguiente_linea,
					(int)tmñ - (int)strlen(linea), archivo);
	}
	char *ultimo_char = &linea[strlen(linea) - 1];
	if (*ultimo_char == '\n')
		linea[strlen(linea) - 1] = '\0';

	return linea;
}

bool ejecutar(char *comando, void *menu_v, char *mensaje_error)
{
	menu_t *menu = (menu_t *)menu_v;
	if (!menu_ejecutar(menu, comando))
		strcpy(mensaje_error, ANSI_COLOR_RED "No se encontró el comando especificado." ANSI_COLOR_RESET);
	else
		strcpy(mensaje_error, "");
	return true;	
}

void mostrar_titulo(char *titulo, void *estilo_v)
{
	enum estilo estilo = *(enum estilo *)estilo_v;
	print_estilo(titulo, estilo);
}

void interfaz_menu_entrar(menu_t *menu)
{
	menu_abrir(menu);
	char msj_error[500] = "";
	while (menu_esta_abierto(menu)) {
		printf(ANSI_RESET_SCREEN);
		menu_mostrar_titulo(menu, mostrar_titulo, menu_ctx(menu));
		menu_mostrar(menu, mostrar_opcion, menu_ctx(menu));
		leer_comando(ejecutar, menu, msj_error);
	}
}

void leer_comando(bool (*f)(char *, void *, char *), void *ctx, char *mensaje_error)
{
	printf("%s\n", mensaje_error);
	char *comando = leer_terminal(stdin);
	f(comando, ctx, mensaje_error);
	free(comando);
}


void print_estilo(const char *texto, enum estilo estilo)
{
	if (estilo == ESTILO_NORMAL)
		printf("%s", texto);
	if (estilo == ESTILO_2)
		printf(ANSI_BG_WHITE ANSI_COLOR_BLACK "%s", texto);
	if (estilo == ESTILO_3)
		printf(ANSI_BG_RESET ANSI_COLOR_GREEN "%s", texto);
	printf("\n");
}

void mostrar_opcion(const char *texto, const char *comando, void *estilo_v)
{
	enum estilo estilo = *(enum estilo *)estilo_v;
	if (estilo == ESTILO_NORMAL)
		printf("%s. %s\n", comando, texto);
	if (estilo == ESTILO_2)
		printf(ANSI_BG_WHITE ANSI_COLOR_BLACK "%s. %s\n", comando,
		       texto);
	if (estilo == ESTILO_3)
		printf(ANSI_BG_RESET ANSI_COLOR_GREEN "%s. %s\n", comando,
		       texto);
	printf(ANSI_COLOR_RESET);
}

void interfaz_salir_menu(void *menu_v)
{
	menu_cerrar((menu_t *)menu_v);
}

void interfaz_menu_error(char* mensaje, enum estilo estilo)
{
	menu_t *menu_error = menu_crear(&estilo);
	menu_cambiar_titulo(menu_error, mensaje);
	menu_agregar_opcion(menu_error, "Volver", "A", interfaz_salir_menu, menu_error);
	interfaz_menu_entrar(menu_error);
	menu_destruir(menu_error);
}
