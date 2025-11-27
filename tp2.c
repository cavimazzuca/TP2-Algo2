#include "src/ansi.h"
#include "src/menu.h"
#include "src/interfaz.h"
#include "src/tp1.h"
#include "src/juego.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#define ERROR -1

void entrar_al_menu(void *menu_v)
{
	menu_t *menu = (menu_t *)menu_v;
	menu_abrir(menu);
	char *msj_error = "\n";
	esperar_respuesta(menu, &msj_error);
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
		*estilo = ESTILO_3;
		break;
	case ESTILO_3:
		*estilo = ESTILO_NORMAL;
		break;
	default:
		*estilo = ESTILO_NORMAL;
	}
}

void jugar(void *juego_v)
{
	juego_t *juego = (juego_t *)juego_v;
	juego_iniciar(juego);
}

void jugar_con_semilla(void *juego_v)
{
	juego_t *juego = (juego_t *)juego_v;
	printf("EScribe la semilla a utilizar:\n");
	char *semilla = leer_respuesta(stdin);
	cambiar_semilla(juego, atoi(semilla));
	free(semilla);
	jugar(juego);
	return;
}

void cargar_archivo(void *juego_v)
{
	juego_t *juego = (juego_t *)juego_v;
	printf("Escribe el archivo a cargar:\n");
	char *comando = leer_respuesta(stdin);
	tp1_t *tp1 = tp1_leer_archivo(comando);
	if (tp1 == NULL) {
		printf("el archivo no existe.\n"); //no se va a mostrar.
		return;
	}
	printf("%i\n", (int)tp1_cantidad(tp1));
	juego_meter_tp1(juego, tp1);
	tp1_destruir(tp1);
	free(comando);
}

//menu que muestra un error y espera a que vuelvas
void menu_error(const char* mensaje, enum estilo *estilo)
{
	menu_t *menu_error = menu_crear(estilo);
	menu_agregar_opcion(menu_error, "A", "Volver", NULL);
	mostrar_opcion(const char *texto, const char *comando, void *estilo_v)
}

void buscar_nombre(void *tp1)
{
	printf(ANSI_RESET_SCREEN);
	printf("Escribe el nombre de tu pokemon
	printf(ANSI_RESET_SCREEN);
}

void buscar_id(void *tp1)
{
}

int main(int argc, char *argv[])
{
	char *archivo = NULL;
	if (argc > 1) {
		archivo = argv[1];
	}
	enum estilo estilo = ESTILO_NORMAL;
	menu_t *menu_principal = menu_crear(&estilo);
	menu_t *menu_buscar = menu_crear(&estilo);
	menu_t *menu_mostrar = menu_crear(&estilo);
	tp1_t *tp1 = NULL;
	if (archivo != NULL)
		tp1 = tp1_leer_archivo(archivo);
	juego_t *juego = juego_crear(tp1);

	menu_agregar_opcion(menu_principal, "Cargar Archivo.", "C",
			    cargar_archivo, juego);
	menu_agregar_opcion(menu_principal, "Buscar.", "B", entrar_al_menu,
			    menu_buscar);
	menu_agregar_opcion(menu_principal, "Mostrar.", "M", entrar_al_menu,
			    menu_mostrar);
	menu_agregar_opcion(menu_principal, "Jugar.", "J", jugar, juego);
	menu_agregar_opcion(menu_principal, "Jugar con semilla.", "S",
			    jugar_con_semilla, juego);
	menu_agregar_opcion(menu_principal, "Cambiar estilo.", "E",
			    cambiar_estilo, menu_ctx(menu_principal));
	menu_agregar_opcion(menu_principal, "Salir del juego.", "Q",
			    salir_del_menu, menu_principal);
	
	menu_agregar_opcion(menu_buscar, "Buscar por nombre.", "N",
			    buscar_nombre, tp1);
	menu_agregar_opcion(menu_buscar, "Buscar por ID.", "I",
			    buscar_id, tp1);
	menu_agregar_opcion(menu_buscar, "Volver al menú anterior.", "A",
			    salir_del_menu, menu_buscar);
	
	menu_agregar_opcion(menu_mostrar, "Volver al menú anterior.", "A",
			    salir_del_menu, menu_mostrar);
	
	entrar_al_menu(menu_principal);

	tp1_destruir(tp1);
	juego_destruir(juego);
	menu_destruir(menu_principal);
	menu_destruir(menu_buscar);
	menu_destruir(menu_mostrar);
	return 0;
}
