<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP2

## Repositorio de Ignacio Julián Cavieres Mazzuca - 112966 - icavieres@fi.uba.ar

- Para compilar:

```bash
make compilar_tp2
make pruebas_alumno
```

- Para ejecutar:

```bash
./tp2
./pruebas_alumno
```

- Para ejecutar con valgrind:
```bash
make all
make tp2
```
---
##  Funcionamiento

La estructura principal que desarrollé para el TP2 fué la estructura `menu_t`.
<div align="center">
<img width="70%" src="img/menu_t.svg">
</div>

El campo más importante dentro de `menu_t` es el campo `opciones`, que es un arreglo dinámico de estructuras `opcion_t`, pero esta la voy a explicar mas adelante con un diagrama.
El campo `cantidad` indica la cantidad de opciones que tiene el menú, y el campo `abierto` indica si el menú está abierto o cerrado. Esto es puramente una utilidad para el usuario que puede chequear un booleano relacionado al propio menú.  
EL campo `ctx` es un puntero a `void` que permite al usuario guardar cualquier tipo de dato que quiera dentro del menú, para luego recuperarlo cuando lo necesite.  
En mi caso, lo utilicé para guardar el estilo actual del menú, que es un enum.
Decidí hacerlo de esta manera y no integrar el estilo al menú para que sea más generico.
<div align="center">
<img width="70%" src="img/opcion_t.svg">
</div>  

La estructura `opcion_t` representa una opción y es una estructura dentro del menú.  
Tiene 4 campos: 
El campo `texto` es un puntero a char que guarda el texto que se muestra en la opción.  
El campo `comando` es un char que representa el comando que el usuario debe ingresar para seleccionar esa opción.  
El campo `f` es un puntero a función que representa la función que se ejecuta cuando el usuario selecciona esa opción.
El campo `ctx` es un puntero a `void` que es un contexto propio de la opción que se incluirá en la función `f` cuando se llame.

## Respuestas a las preguntas teóricas
   - ¿Qué TDAs fueron utilizados para implementar las funcionalidades pedidas? ¿Por qué fue elegido ese TDA y no otro?  
    - Para implementar las funcionalidades pedidas, utilicé principalmente los siguientes TDAs:
      - **TDA Juego**: Implementé este TDA para manejar la lógica del juego de memoria, incluyendo la gestión de cartas, turnos de jugadores y puntuaciones. Este TDA fue seleccionado por su capacidad para encapsular la lógica del juego de manera clara y modular.
      - **TDA Lista**: Utilicé listas para almacenar las jugadas realizadas durante el juego. Este TDA fue elegido por su flexibilidad y facilidad para agregar y eliminar elementos dinámicamente.
   
   - Explicar el TDA menú. Justifique las operaciones incluídas en el TDA.  
    - El TDA Menú está hecho para tener funciones asociadas a un texto y a un comando de manera que el TDA pueda ser implementado en diferentes contextos. Las operaciones incluidas en el TDA son:
      - **Crear Menú**: Permite inicializar un nuevo menú con opciones específicas.
      - **Agregar Opción**: Permite añadir nuevas opciones al menú, facilitando la expansión del mismo.
      - **Mostrar Menú**: Itera todas las opciones y permite aplicarles una función con su contexto.
      - **Ejecutar Opción**: Permite ejecutar la función asociada a una opción específica basada en el comando ingresado por el usuario.  
      - **Abrir Menú**: Activa el booleano del struct menu_t para indicar que el menú está en uso.  
      - **Cerrar Menú**: Desactiva el booleano del struct menu_t para indicar que el menú ya no está en uso.  
      - **Menu está abierto**: Permite al usuario chequear si el menú está abierto o cerrado según el booleano.  
      - **Destruir Menú**: Libera la memoria utilizada por el menú y sus opciones.  
      - **Menu Ctx**: Permite al usuario guardar y recuperar un contexto personalizado dentro del menú (Usado para el estilo).  
      - **Cambiar Ctx**: Permite al usuario cambiar el contexto personalizado dentro del menú (Usado para el estilo).  

   - Explicar cualquier otro TDA o estructuras auxiliares implementadas.  
    - Además del TDA Menú, implementé los siguientes TDAs:
      - **TDA Juego**: Este TDA maneja la lógica del juego de memoria, incluyendo la gestión de cartas, turnos de jugadores y puntuaciones. Fue diseñado para encapsular toda la lógica del juego de manera modular y clara.
      - **TDA Interfaz**: Este TDA se encarga de la ejecución de comandos en consola y visualización del menú. Fue elegido para separar la lógica del juego de la ejecución de comandos, facilitando futuras modificaciones en la interfaz sin afectar la lógica del juego.
      - **Estructura Carta**: Representa cada carta en el juego, incluyendo su numero, estado (boca arriba o boca abajo), si ya se encontró su par y el Pokémon asociado.
      <div align="center">
        <img width="70%" src="img/carta_t.svg">
        </div>  
        
      - **Estructura Jugada**: Estructura que representa una jugada realizada por un jugador, incluyendo las cartas seleccionadas y el resultado de la jugada.  
   - Explique qué dificultades encontró al implementar el TP2 y cómo pudo solucionarlas. 
    - Una de las principales dificultades que encontré al implementar el TP2 fue manejar la interacción entre los diferentes TDAs, especialmente cuando intenté implementar "Interfaz" como TDA a parte. Me dí cuenta en el camino de las diferencias que podrían haber entre diferentes tipos de entradas requeridas en el menú.
    - Otra dificultad fue que la lógica del juego manejara correctamente todas las posibles situaciones, como la selección de cartas ya encontradas o la gestión de turnos.
    - La gestión de memoria y structs, especialmente al trabajar con estructuras creadas por mí desde 0. A veces no sabía que incluir en cada struct y cómo organizar la información de manera eficiente.

# TP2-Algo2
