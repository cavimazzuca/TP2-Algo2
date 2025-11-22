#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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