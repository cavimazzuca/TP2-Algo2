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

int main()
{
	pa2m_nuevo_grupo("===== PRUEBAS MENU =====");
	pruebas_menu();
	pa2m_nuevo_grupo("===== PRUEBAS JUEGO =====");
}
