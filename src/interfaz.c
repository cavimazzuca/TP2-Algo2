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

void leer_opciones_menu(menu_t *menu, char **msj_error)
{
	while (menu_esta_abierto(menu)) {
		//printf(ANSI_RESET_SCREEN);
		menu_mostrar(menu, mostrar_opcion, menu_ctx(menu));
		printf("%s", *msj_error);
		char *comando = leer_respuesta(stdin);
		*msj_error = "\n";
		if (!menu_ejecutar(menu, comando))
			*msj_error = ANSI_COLOR_RED
				"No se encontró el comando especificado.\n" ANSI_COLOR_RESET;
		free(comando);
	}
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
}
