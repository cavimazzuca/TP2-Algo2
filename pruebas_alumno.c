#include "pa2m.h"
#include "src/menu.h"
#include "src/tp1.h"
#include "src/juego.h"

void funcion(void *extra)
{
	return;
}

void mostrar(const char *texto, const char *comando, void *ctx)
{
	return;
}

void pruebas_menu_crear()
{
	menu_t *menu = menu_crear(NULL);
	pa2m_afirmar(menu != NULL, "Se crea un menú.");
	menu_destruir(menu);
}

void pruebas_menu_opcion()
{
	menu_t *menu = menu_crear(NULL);
	menu_agregar_opcion(menu, "Opcion A", "A", funcion, NULL);
	pa2m_afirmar(menu_ejecutar(menu, "A"),
		     "Se crea una opción y se ejecuta.");
	menu_destruir(menu);
}

void pruebas_menu_opcion_2()
{
	menu_t *menu = menu_crear(NULL);
	menu_agregar_opcion(menu, "Opcion A", "A", funcion, NULL);
	menu_agregar_opcion(menu, "Opcion B", "B", funcion, NULL);
	pa2m_afirmar(menu_ejecutar(menu, "B"),
		     "Se crean dos opciones y se ejecutan.");
	pa2m_afirmar(
		menu_mostrar(menu, mostrar, NULL) == 2,
		"Se itera sobre los elementos y devuelven bien la cantidad.");
	menu_destruir(menu);
}

void pruebas_menu_opcion_iterar()
{
	menu_t *menu = menu_crear(NULL);
	menu_agregar_opcion(menu, "Opcion A", "A", funcion, NULL);
	menu_agregar_opcion(menu, "Opcion B", "B", funcion, NULL);
	pa2m_afirmar(
		menu_mostrar(menu, mostrar, NULL) == 2,
		"Se itera sobre dos opciones y devuelven bien la cantidad.");
	menu_destruir(menu);
}

void pruebas_menu_mostrar_null()
{
	pa2m_afirmar(menu_mostrar(NULL, mostrar, NULL) == 0,
		     "No se puede mostrar un menú nulo.");
}

void pruebas_menu_opcion_null()
{
	pa2m_afirmar(menu_agregar_opcion(NULL, "Opcion A", "A", funcion,
					 NULL) == false,
		     "No se puede agregar una opción a un menú nulo.");
}

void pruebas_menu_ejecutar_null()
{
	pa2m_afirmar(menu_ejecutar(NULL, "A") == false,
		     "No se puede ejecutar en un menú nulo.");
}

void pruebas_juego_crear()
{
	juego_t *juego = juego_crear(NULL);
	pa2m_afirmar(juego != NULL, "Se crea un juego.");
	juego_destruir(juego);
}

void pruebas_juego_meter_tp()
{
	juego_t *juego = juego_crear(NULL);
	tp1_t *tp1 = tp1_leer_archivo("ejemplos/normal.csv");
	pa2m_afirmar(juego_meter_tp1(juego, tp1) == true,
		     "Se inserta un tp dentro de un juego.");
	tp1_destruir(tp1);
	juego_destruir(juego);
}

void pruebas_juego_meter_tp_null()
{
	juego_t *juego = juego_crear(NULL);
	pa2m_afirmar(juego_meter_tp1(juego, NULL) == false,
		     "No se puede insertar un tp nulo dentro de un juego.");
	juego_destruir(juego);
}

void pruebas_juego_null_meter_tp()
{
	tp1_t *tp1 = tp1_leer_archivo("ejemplos/normal.csv");
	pa2m_afirmar(juego_meter_tp1(NULL, tp1) == false,
		     "No se puede insertar un tp dentro de un juego nulo.");
	tp1_destruir(tp1);
}

void pruebas_juego_semilla()
{
	juego_t *juego = juego_crear(NULL);
	int semilla_anterior = 10;
	cambiar_semilla(juego, 10);
	int semilla_nueva = 20;
	int semilla_cambiada = cambiar_semilla(juego, semilla_nueva);
	pa2m_afirmar(semilla_cambiada == semilla_anterior,
		     "Se cambia la semilla del juego correctamente.");
	juego_destruir(juego);
}

void pruebas_terminal_leer_null()
{
	pa2m_afirmar(leer_terminal(NULL) == NULL,
		     "No se puede leer de un archivo nulo.");
}

void pruebas_terminal_funcion_null()
{
	char mensaje_error[500] = "Error inicial.";
	pa2m_afirmar(leer_comando(NULL, NULL, mensaje_error) == false,
		     "No se puede ejecutar una función nula en leer_comando.");
}

int main()
{
	pa2m_nuevo_grupo("===== PRUEBAS MENU =====");
	pruebas_menu_crear();
	pruebas_menu_opcion();
	pruebas_menu_opcion_2();
	pruebas_menu_opcion_iterar();
	pruebas_menu_mostrar_null();
	pruebas_menu_opcion_null();
	pruebas_menu_ejecutar_null();
	pa2m_nuevo_grupo("===== PRUEBAS JUEGO =====");
	pruebas_juego_crear();
	pruebas_juego_semilla();
	pruebas_juego_meter_tp();
	pruebas_juego_meter_tp_null();
	pruebas_juego_null_meter_tp();
	pa2m_nuevo_grupo("===== PRUEBAS INTERFAZ =====");
	pruebas_terminal_leer_null();
	pruebas_terminal_funcion_null();
}
