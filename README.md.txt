# Mi Simulador de Computadora
Bienvenido a mi simulador de computadora en SML, usando lenguaje c y creado para la materia de Programación Avanzada. A continuación, presentaré algunas instrucciones y tips para sacarle el mayor provecho.
## Descripción
El "Simpletron" es una computadora sintética construida bajo la arquitectura Von Neumann. El presente programa simula el comportamiento físico del Hardware mediante:

1. Memoria Ram: Un arreglo de 100 espacios de memoria
2. Acumulador: Registro principal de operaciones aritméticas y lógicas.
3. Ciclo de instrucción: Ejecución secuencial mediante etapas "Fetch, Decode y Execute" (Extraer, decodificar y ejecutar).

El programa imita la ejecución de una CPU, lee las instrucciones que da el usuario (en Simpletron Machine Language, SML), las carga en la memoria y las ejecuta, utilizando registros como el Acumulador, contador de instrucciones, y registro de instrucción.

> Requisitos: Compilador en C, el programa fue realizado en el IDE Dev-C ++.

## Estructura del código

El archivo principal "SimuladorSML_APVG_v1.c" está dividido en tres módulos principales.
1. Carga en memoria: Ciclo de lectura, el programa recibe "palabras" de 4 dígitos (i.e. 1008), e ignora instrucciones cuando usamos el centinela "9999".
2. Unidad procesadora (CPU): Ciclo que extrae el código de operación y la dirección del operando, para ejecutarlos mediante un ciclo "Switch".
3. Volcado de memoria: Es la función que muestra el estado interno final de los registros, así como las 100 posiciones de la memoria.


## Instrucciones SML soportadas
| Código | Operación | Funcionamiento |
| --- | --- | --- |
| 10 | READ | Lee un valor entero y lo almacena en la memoria |
| 11 | WRITE | Imprime en la pantalla el valor almacenado en la memoria |
| 20 | LOAD | Carga un valor en la memoria al acumulador |
| 21 | STORE | Almacena el contenido del acumulador en la memoria |
| 30 | ADD | Suma un valor de memoria al acumulador |
| 31 | SUBSTRACT | Resta un valor de memoria al acumulador |
| 32 | DIVIDE | Divide el acumulador entre un valor de memoria |
| 33 | MULTIPLY | Multiplica el acumulador por un valor de memoria |
| 40 | BRANCH | Salto incondicional a una posición de memoria |
| 41 | BRANCHNEG | Salta si el acumulador es menor a cero |
| 42 | BRANCHZERO | Salta si el acumulador es igual a cero |
| 43 | HALT | Finaliza la ejecución del programa |


## Compilar y Ejecutar

### Opción 1: Desde un compilador
1. Abrir el archivo "SimuladorSML_APVG_v1.c" en el compilador de su preferencia.
2. Guardar el archivo con terminación .c.
3. Seleccionar la opción de "Compilar y Ejecutar" en el IDE.

### Opción 2: Desde la consola

Abrir la terminal en la carpeta donde se guardó el proyecto, y ejecutar:
``` bash
# 1. Compilar el archivo ejecutable
gcc SimuladorSML_APVG_v1.c

# 2. Ejecutar la aplicación (Windows)
SimuladorSML_APVG_v1.exe

```
## Enlace de consulta
Para aprender más sobre la Simpletron, puedes visitar [The Markdown](https://www.markdownguide.org/)