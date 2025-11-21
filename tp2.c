#include "src/ansi.h"
#include "src/menu.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include "src/ansi.h"
enum estilo {
	ESTILO_NORMAL,
	ESTILO_2
};

void mostrar_opcion(const char *texto, const char *comando, int estilo)
{
	if (estilo == ESTILO_NORMAL)
		printf("%s. %s\n", comando, texto);
	if (estilo == ESTILO_2)
		printf(ANSI_COLOR_RED "%s. %s\n", comando, texto);
	
}

bool salir_del_menu(void *menu, void *salir)
{
	*(bool *)salir = true;
	return true;
}

void jugar(void *menu, void *juego)
{
	
}

char *leer_linea2(FILE *archivo)
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

void esperar_respuesta(menu_t *menu)
{
	bool salir = false;
	while (!salir) {
		system("clear");
		menu_mostrar(menu, mostrar_opcion, ESTILO_2);
		char *comando = leer_linea2(stdin);
		menu_ejecutar(menu, comando, &salir);
		free(comando);
	}
}

bool procesar_menu(void *menu_v)
{
	menu_t *menu = (menu_t *)menu_v;
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
