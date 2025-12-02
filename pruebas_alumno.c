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

void pruebas_menu()
{
	menu_t *menu = menu_crear(NULL);
	pa2m_afirmar(menu != NULL, "Se crea un menú.");
	menu_agregar_opcion(menu, "Opcion A", "A", funcion, NULL);
	pa2m_afirmar(menu_ejecutar(menu, "A"),
		     "Se crea una opción y se ejecuta.");
	menu_agregar_opcion(menu, "Opcion B", "B", funcion, NULL);
	pa2m_afirmar(menu_ejecutar(menu, "B"),
		     "Se crea una otra opción y se ejecuta.");
	pa2m_afirmar(
		menu_mostrar(menu, mostrar, NULL) == 2,
		"Se itera sobre los elementos y devuelven bien la cantidad.");
	menu_destruir(menu);
}

void pruebas_menu_null()
{
	
	pa2m_afirmar(menu_mostrar(NULL, mostrar, NULL) == 0,
		     "No se puede mostrar un menú nulo.");
	pa2m_afirmar(menu_agregar_opcion(NULL, "Opcion A", "A", funcion, NULL) == false,
		"No se puede agregar una opción a un menú nulo.");
	pa2m_afirmar(menu_ejecutar(NULL, "A") == false,
		     "No se puede ejecutar en un menú nulo.");
}

void pruebas_juego()
{
	juego_t *juego = juego_crear(NULL);
	pa2m_afirmar(juego != NULL, "Se crea un juego.");
	tp1_t *tp1 = tp1_leer_archivo("ejemplos/normal.csv");
	pa2m_afirmar(juego_meter_tp1(juego, tp1) == true, "Se inserta un tp dentro del juego.");
	tp1_destruir(tp1);
	juego_destruir(juego);
}

int main()
{
	pa2m_nuevo_grupo("===== PRUEBAS MENU =====");
	pruebas_menu();
	pruebas_menu_null();
	pa2m_nuevo_grupo("===== PRUEBAS JUEGO =====");
	pruebas_juego();
}
