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
./pruebas_alumno
./tp2
```

- Para ejecutar con valgrind:
```bash
make all
make tp2
```
---
##  Funcionamiento

### Menú  
La estructura principal que desarrollé para el TP2 fué la estructura `menu_t`.
<div align="center">
<img width="70%" src="img/menu_t.svg">
</div>

El campo más importante dentro de `menu_t` es el campo `opciones`, que es un arreglo dinámico de estructuras `opcion_t`. Esta está explicada mas adelante en un diagrama.
El campo `cantidad` indica la cantidad de opciones que tiene el menú, y el campo `abierto` indica si el menú está abierto o cerrado. Este último es puramente una utilidad para el usuario que puede chequear un booleano relacionado al propio menú.  
El campo `ctx` es un puntero a `void` que permite al usuario guardar cualquier tipo de dato que quiera dentro del menú, para luego recuperarlo cuando lo necesite.  
En mi caso, lo utilicé para guardar el estilo actual del menú, que es un enum.
Decidí hacerlo de esta manera y no integrar el estilo al menú para que sea más generico.
El campo `titulo` es un puntero a char que guarda el título del menú. Lo añadí para hacer mas fácil la modularización de la presentación de los menú que hay en el programa.
<div align="center">
<img width="70%" src="img/opcion_t.svg">
</div>  

La estructura `opcion_t` representa una opción del menú y es una estructura dentro de él.   
Tiene 4 campos: 
El campo `texto` es un puntero a char que guarda el texto que se muestra en la opción.  
El campo `comando` es un char que representa el comando que el usuario debe ingresar para seleccionar esa opción.  
El campo `f` es un puntero a función que representa la función que se ejecuta cuando el usuario selecciona esa opción.
El campo `ctx` es un puntero a `void` que es un contexto propio de la opción que se incluirá en la función `f` cuando se llame.

### Interfaz  
Para manejar la ejecución de comandos en consola y la visualización del menú, decidí implementar un TDA `interfaz`. De esta manera modularicé la ejecución de comandos tanto para el juego como para el resto del programa. 
Las funciones del TDA son las siguientes:

`leer_terminal`: Es una función reutilizada del TP1 que lee una línea de un archivo. En este TP la utilizo para leer la línea ingresada por el usuario en consola (stdin). 

`print_estilo`: Imprime un texto con un estilo determinado. La función no es muy buena ya que más adelante creé aplicar_estilo y quitar_estilo que son mas directas para aplicar estilos a textos en general.

`leer_comando`: Es una función que debe ser usada en bucles. Escribe por pantalla una línea reservada para mensajes de error y permite al usuario ingresar un comando. La función pasada por parámetro se encarga de validar dicho comando y modificar el mensaje de error para que en la siguiente iteración del bucle aparezca.

`interfaz_menu_entrar`: Función que genera toda la interfaz del menú automáticamente. Muestra el título, sus opciones y permite seleccionar una opción.

`interfaz_menu_error`: Un menú de error simple que muestra un mensaje de error y espera a que el usuario presione "A" para seguir con la ejecución del programa.  

`aplicar_estilo`: Aplica un estilo determinado a todo el texto siguiente.  

`quitar_estilo`: Reinicia el estilo del texto al estilo por defecto.  

### Juego  
El TDA `juego` maneja toda la lógica del juego de memoria.  
Dentro del struct `juego_t` hay varios campos:  
- `tp1`: Un puntero al TDA `tp1` con los pokemones que se introducen a la tabla hash.  
- `pokemones`: Un hash que contiene las cartas del juego actual.  
- `turno`: Un entero que indica el turno actual.
- `encontrados`: Un entero que indica la cantidad de pares encontrados en el juego actual.
- `estilo`: Un enum que indica el estilo actual del juego.
- `puntos_j1`: Un entero que indica los puntos del jugador 1 en el juego actual.
- `puntos_j2`: Un entero que indica los puntos del jugador 2 en eñ juego actual.
- `ultima_volteada`: Un puntero a la última carta volteada en el juego actual.
- `ultimas_jugadas`: Una lista que contiene las últimas jugadas realizadas en el juego actual.  
- `semilla`: Un entero que indica la semilla utilizada para la generación del juego actual.  

Las funciones del TDA son las siguientes:  
- `juego_crear`: Le asigna memoria para un nuevo juego y lo inicializa. Recibe un tp1.  

- `juego_meter_tp1`: Mete un TDA tp1 dentro del juego y, usando la semilla, crea la tabla hash.  

- `cambiar_semilla`: Cambia la semilla utilizada para la generación del juego.  

- `juego_iniciar`: Inicia un nuevo juego de memoria.  

- `juego_destruir`: Destruye el juego y libera la memoria utilizada.  

- `juego_cambiar_estilo`: Cambia el estilo del juego.  

Dentro del juego, la estructura `carta` representa cada carta del juego.  
<div align="center">
<img width="70%" src="img/carta_t.svg">
</div>  
Esta estructura es usada dentro del hash `pokemones` del juego. Consideré necesaria esta estructura para manejar el estado de cada carta en el juego.  

Otra estructura dentro del juego es la estructura `jugada`, que representa una jugada realizada por un jugador.  
<div align="center">
<img width="70%" src="img/jugada_t.svg">
</div>  

Esta estructura es usada dentro de la lista `ultimas_jugadas` del juego. Fué necesaria para mostrar las últimas jugadas realizadas por los jugadores. El campo `jugador` indica qué jugador realizó la jugada y se calcula usando el campo `turno` de la estructura juego.  
`num1` y `num2` es la primer y segunda carta seleccionada en la jugada, y `coinciden` es un booleano que indica si las cartas coinciden o no.  

#### Iniciar juego  
Una vez creado el juego con `juego_crear` y metido el tp1 correctamente, se puede iniciar el juego con `juego_iniciar`.  
Lo primero que se hace es inicializar la tabla hash `pokemones` y llenar dicha tabla con cartas tomadas al azar del tp1.  
Después, se muestra la interfaz del juego.  
ESta interfaz se va a mostrar repetidamente hasta que todas las cartas hayan sido levantadas.  
Una vez que el juego termina, se muestra el resultado final.
Finalmente, se resetean los campos temporales de la partida como los puntos y las jugadas realizadas para poder iniciar una nueva partida si el usuario lo desea.  

<div align="center">
<img width="70%" src="img/juego_iniciar.svg">
</div>  

#### Lógica del juego  
Cuando inicia el juego, se entra en un bucle donde se muestra la interfaz del juego y se procesa la jugada del jugador actual. Este bucle termina cuando se encuentran todos los pares.  

Para determinar a que jugador le toca, se utiliza el campo `turno` del struct `juego_t`. 
Si es impar, le toca al jugador 1, si es par, al jugador 2. 
Dentro del bucle, se le pide al jugador que ingrese un número correspondiente a las cartas metidas en la tabla hash.  

Se valida que el número ingresado sea válido (que exista en la tabla hash y que no haya sido encontrado ya).  
Si el número es válido, se muestra la carta correspondiente.  
Una vez volteada, se vuelve a iterar el bucle sin pasar de turno, para que el jugador pueda seleccionar la segunda carta.  

Cuando el jugador selecciona la segunda carta, se vuelve a validar el número ingresado.  
Si es válido, se muestra la carta correspondiente y se compara con la primera carta seleccionada.  
Si las cartas coinciden, se le suma un punto al jugador actual y se marca ambas cartas como encontradas.  
En cualquier caso, se guarda la jugada realizada en la lista de últimas jugadas y se pasa el turno al otro jugador.  

Este diagrama muestra la lógica principal del juego: La parte del código que procesa cada jugada.  
<div align="center">
<img width="40%" src="img/bucle_juego.svg">
</div>


## Respuestas a las preguntas teóricas
   - ¿Qué TDAs fueron utilizados para implementar las funcionalidades pedidas? ¿Por qué fue elegido ese TDA y no otro?  
      - **TDA Lista**: Utilicé listas para almacenar las jugadas realizadas durante el juego. Este TDA fue elegido por su flexibilidad y facilidad para agregar y eliminar elementos dinámicamente.  
      Elegí este en vez de cola o pila porque necesitaba acceder a múltiples elementos (las últimas 5 jugadas) y no solo al primero o al último.     
      - **TDA Hash**: Implementé una tabla hash para almacenar y acceder rápidamente a las cartas del juego. Este TDA fue seleccionado por su eficiencia en operaciones de búsqueda y almacenamiento. 
      
      En cualquiera de las opciones, un árbol binario sería menos eficiente o muy poco práctico para este caso. Así que me decanté por no utilizarlo.
   
   - Explicar el TDA menú. Justifique las operaciones incluídas en el TDA.  
    - El TDA Menú está hecho para tener funciones asociadas a un comando específico de manera que este pueda ser implementado en diferentes contextos. Las operaciones incluidas en el TDA son:
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
  <div align="center">
  <img width="70%" src="img/menu_t.svg">
  </div>  

   - Explicar cualquier otro TDA o estructuras auxiliares implementadas.  
    (El resto de TDAs fueron explicados anteriormente en la sección de Juego).
  <div align="center">
  <img width="70%" src="img/tp2_t.svg">
  </div>  
  La estructura TP2 la ví necesaria para poder modificar los componentes en común entre el menú y el juego, como el estilo y el tp1.  
  Esto es así porque mis funciones del menu solo pueden recibir un parámetro de contexto, y necesitaba pasar ambos datos.  
  
   - Explique qué dificultades encontró al implementar el TP2 y cómo pudo solucionarlas.   
    - Una de las principales dificultades que encontré al implementar el TP2 fue manejar la interacción entre los diferentes TDAs, especialmente cuando intenté implementar "Interfaz" como TDA aparte. Me dí cuenta en el camino de las diferencias que podrían haber entre diferentes tipos de entradas requeridas en el menú.
    - Otra dificultad fue que la lógica del juego manejara correctamente todas las posibles situaciones, como la selección de cartas ya encontradas o la gestión de turnos.
    - La gestión de memoria y structs, especialmente al trabajar con estructuras creadas por mí desde 0. A veces no sabía que incluir en cada struct y cómo organizar la información de manera eficiente.

# TP2-Algo2
