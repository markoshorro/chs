# Codiseño Hardware/Software

## Implementación en una FPGA de Differential Evolution.

Este proyecto consiste en diseñar un circuito capaz de implementar el algoritmo Differential
Evolution en una FPGA. Se propone una función de coste a evaluar utilizando aritmética en
punto flotante con doble precisión.  
Introducción
Differential evolution trata de minimizar el valor de una función de N variables, denominada
función de coste. Se supone que la complejidad de la función y el gran número de variables
implicadas dificulta encontrar una solución analítica, así que se recurre a un proceso de prueba
y error. Se proponen varias combinaciones de valores de variables elegidas al azar, y se evalúa
la función de coste para cada combinación, llamada SOLUCIÓN. El conjunto de soluciones se
denomina POBLACIÓN. A continuación comienza un proceso iterativo mediante el cual cada
solución se hibrida con otras soluciones de la población siguiendo un procedimiento
parcialmente aleatorio. Para cada solución híbrida, se calcula la función de coste. La solución
híbrida sustituirá a la solución original si obtiene un valor más bajo en la función de coste. El
proceso de hibridación está sujeto a que los valores de las variables no pueden exceder unos
límites máximo y mínimo establecidos para cada variable. Tras comparar cada solución de la
población con su híbrida correspondiente, se obtiene una nueva población de segunda
generación. El proceso se repite, mejorando la población con cada GENERACIÓN. Tras un cierto
número de generaciones, la población convergerá hacia una solución óptima huyendo de
mínimos locales.  
Caso de estudio
Se propone un problema de 10 variables en punto flotante de doble precisión y una población
de 256 soluciones. La función de coste es sintética y es la siguiente:  

Las divisiones son por potencias de 2 para simplificar la implementación, que de otra forma
sería muy costosa.

La hibridación está guiada por una serie de valores aleatorios. Obtener números aleatorios de
calidad es difícil, pero existen generadores hardware. En este proyecto supondremos que
disponemos de un generador de esas características que nos proporciona para cada
hibridación los siguientes 4 valores: 3 valores de 8 bits (rand1, rand2, rand3) que nos permiten
elegir 3 soluciones con las que realizar la hibridación, y una máscara de 10 bits cada uno de los
cuales nos indica si la correspondiente variable de la solución se hibrida o mantiene su valor.

### Diseño
El funcionamiento se ha planteado de la siguiente manera. El sistema dispone de una entrada
de 64 bits llamada INTRO para cargar los valores iniciales, y una salida de 64 bits para
transmitir los resultados llamada OUTRO. Además, existen entradas para la máscara y los 3
números aleatorios.
El sistema consta de 5 módulos principales: un controlador, un gestor de los bloques de
memoria, un módulo para hibridar, otro para chequear los límites de las variables hibridadas, y
otro para realizar el cálculo de la función de coste. Además, existen módulos para realizar
cálculos en punto flotante, colas FIFO, y un duplicador de datos.
El esquema general se muestra en la figura, y a continuación se describe el funcionamiento de
cada módulo.

### Controlador:
Carga un primer dato de INTRO, en el que se indica cuantas generaciones de han de calcular.  
A continuación, lee uno por uno de INTRO los valores iniciales de las 10 variables de la primera
solución y las envía al módulo de bloques de memoria. Después lee el valor de la función de
coste y también lo envía. Esta tarea se repite 256 veces hasta llenar la memoria con los valores
iniciales de toda la población.  
El proceso de Differential Evolution consiste en un bucle principal que se repite G
generaciones. Para cada generación se repite el siguiente proceso 256 veces:
  * Se piden al módulo de memorias las variables de las soluciones indicadas por rand1 y
rand2.  
  * Se almacenan esas variables en sendos arrays A y B
  * Se piden al módulo de memorias las variables de la solución original y la indicada por
rand3
  * Se almacenan esas variables en sendos arrays O y C
  * Se lee la máscara de hibridación y, dependiendo del valor de cada bit, se envía al módulo
de hibridación los valores correspondientes de A, B y C ó A, ‐A y O.  
  * Tras procesar todas las soluciones, se espera a recibir la señal listo, que indica que se han
recibido todos los resultados.  
Se repite el proceso para tantas generaciones como se haya indicado
Finalmente, se piden al módulo de memorias los valores de todas las variables y coste de todas
soluciones y se sacan una por una la salida OUTRO.

### Bloque de Memorias:
Este módulo implementa 3 procesos:
  * Atender las peticiones de lectura, recibiendo 2 direcciones de memoria y enviando por
DataA y DataB las 10 variables de las 2 soluciones asociadas a esas direcciones de
memoria. Además, envía por DataA un undécimo valor, el coste de esa solución. Este
último valor es necesario en el proceso final de exportar las soluciones.  
  * Atender las peticiones de escritura de soluciones hibridadas. Estas llegan por nuevo, y se
almacenan hasta que se sepa si van a ser seleccionadas o no.  
  * Atender la escritura de resultados. Este proceso recibe un resultado y comprueba si
mejora al de la solución original. Si lo hace, se almacena junto a las variables de la
solución híbrida y se descarta la solución original. Tras recibir los 256 resultados, este
proceso envía al controlador la señal de listo para que comience una nueva generación.  
La implementación en la FPGA se realizará utilizando 2x11 bloques de RAM, de 256 palabras de
64 bits cada uno. Son necesarios 2 juegos de 11 bloques. Para cada solución, un juego contiene
la solución actual, y otro la híbrida.  
Como la implementación final está todavía muy lejos se utilizará un array de las siguientes
características: sc_uint<64> mem[256][11][2]. Además, será necesario otro array: bool
banco[256][2], que indica, para cada solución, en qué juego de bloques está almacenada la
solución original, y en cual estará la nueva solución cuando finalice la generación en curso.  

### Hibridar:
Es un bloque muy sencillo que hace uso, para cada una de las 10 variables, de 2 sumadores y
un multiplicador en punto flotante para calcular la función de hibridación.

### Chequear híbridas:  
Hace uso de dos módulos que calculan el máximo y el mínimo de 2 números. Para cada una de
las variables hibridadas, se comprueba que su valor está comprendido dentro de los límites
que le corresponden. De lo contrario, se selecciona el límite máximo o el límite mínimo según
corresponda.

### Cálculo de la función de coste:  
Consiste básicamente en enlazar bloques de cálculo en punto flotante para implementar la
función de coste.
