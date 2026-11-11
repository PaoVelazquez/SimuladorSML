# Compilador y Simulador Simpletron (Simple to SML)

Este es un proyecto desarrollado para la materia de **Programación Avanzada**. Consiste en un sistema completo de compilación y simulación basado en la arquitectura sintética Von Neumann del entorno **Simpletron**, adaptado del libro *C How to Program* (Deitel & Deitel).

---

## Tabla de Contenidos
- [Descripción General](#descripción-general)
- [Estructura del Proyecto](#estructura-del-proyecto)
- [Arquitectura e Instrucciones SML](#arquitectura-e-instrucciones-sml)
- [Instrucciones de Compilación y Ejecución](#instrucciones-de-compilación-y-ejecución)
- [Manejo de Errores Fatales](#manejo-de-errores-fatales)

---

## Descripción General

El sistema consta de dos componentes principales:

1. **Compilador (`compilador.c` / `expresiones.c`)**:
   - Analiza programas escritos en lenguaje de alto nivel **Simple** (`.simple`).
   - Convierte expresiones aritméticas de formato **infijo a posfijo** utilizando pilas (stack).
   - Realiza la traducción a lenguaje máquina de Simpletron (**SML**), generando un archivo ejecutable con extensión `.simp` (equivalente a `.sml` según la literatura consultada).

2. **Simulador (`simulador.c`)**:
   - Imita el comportamiento físico del procesador y la memoria RAM (100 celdas numeradas `00-99`).
   - Implementa el ciclo de instrucción **Fetch - Decode - Execute** (Extraer, Decodificar, Ejecutar).
   - Gestiona registros clave: **Acumulador**, **Contador de Instrucciones (IC)**, **Registro de Instrucción (IR)**, **Código de Opción (Opcode)** y **Operando**.
   - Genera un **Memory Dump** completo al finalizar la ejecución o ante la presencia de un error fatal.

> **Nota sobre extensiones:** Los archivos `.simp` generados por este compilador contienen las instrucciones numéricas en lenguaje máquina **SML** (*Simpletron Machine Language*) solicitadas en la especificación del proyecto. Aunque no sea un archivo con extensión `.sml`, el archivo `.simp` cumple dicha función y es aceptado en la literatura. 

---

## Estructura del Proyecto

```text
/
├── src/
│   ├── compilador.c       # Traduce codigo Simple a SML (.simp)
│   ├── expresiones.c      # Evaluador e interprete de expresiones infijas a posfijas
│   ├── simulador.c        # Entorno de ejecucion y procesador virtual SML
│   ├── programa.simple    # Codigo de prueba en lenguaje Simple, editable por el usuario
│   └── programa.simp      # Codigo ejecutable en lenguaje maquina SML, generado por compilador.c
├── Evidencias/
│   ├── compilador/        # Pruebas realizadas desde compilador.c
│   ├── expresiones/       # Pruebas realizadas desde expresiones.c para diferentes casos
│   ├── simuladorv1/       # Pruebas realizadas en la primera versión del simulador SML
│   ├── simulador/         # Capturas de Memory Dump y Errores Fatales
│   └── Pruebas varias, del conjunto de programas en funcionamiento
├── .gitignore             # Exclusion de binarios ejecutable (.exe)
└── README.md              # Documentacion general del proyecto

```
---

## Arquitectura e Instrucciones SML
El simulador soporta las siguientes instrucciones base y extendidas de la máquina Simpletron:
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
| 35 | EXP | Eleva el acumulador a la potencia indicada en la memoria |
| 40 | BRANCH | Salto incondicional a una posición de memoria |
| 41 | BRANCHNEG | Salta si el acumulador es menor a cero |
| 42 | BRANCHZERO | Salta si el acumulador es igual a cero |
| 43 | HALT | Finaliza la ejecución del programa |


---

## Instrucciones de Compilación y Ejecución

Requisitos previos: Tener un compilador de C instalado y tener un entorno de desarrollo (Dev C, VS code o terminal)

### Opción 1: Desde un compilador
1. Abrir el archivo a revisar en el compilador de su preferencia.
2. Guardar el archivo con terminación .c.
3. Seleccionar la opción de "Compilar y Ejecutar" en el IDE.

### Opción 2: Desde la consola

Abrir la terminal en la carpeta donde se guardó el proyecto (src/), y ejecutar:
``` 
gcc compilador.c -o compilador.exe
gcc simulador.c -o simulador.exe

```

### El flujo de trabajo consta de dos pasos: Compilar el programa Simple y ejecutar el simulador SML.
1. Traducimos de .simple a .simp, mediante la ejecución del compilador. Nuestro archivo fuente `.simple` puede ser hecho por el usuario, con las instrucciones que se requieran, o en caso de no encontrarse, el compilador hará un archivo `.simple` de prueba. Nuestro compilador generará un archivo traducido de `.simple` a `.simp`
2. Ejecutaremos en el simulador. Nuestro simulador.c tomará el archivo `.simp` generado por nuestro compilador y lo usará como archivo fuente para nuestra máquina Simpletron. 

#### Ejemplo
A continuación, se muestra un ejemplo de un programa .simple y cómo se transformará en nuestra salida final en nuestro simulador Simpletron
``` 
# Codigo en lenguaje Simple
10 input x
20 input y
30 let z = x + y
40 print z
50 end

```
Nuestro compilador leerá este código fuente y lo transformará a .simp:
``` 
# Codigo en SML
+10007   // READ  -> Lee 'x' y lo guarda en memoria 07
+10008   // READ  -> Lee 'y' y lo guarda en memoria 08
+20007   // LOAD  -> Carga 'x' (memoria 07) al Acumulador
+30008   // ADD   -> Suma 'y' (memoria 08) al Acumulador
+21009   // STORE -> Guarda el resultado 'z' en memoria 09
+11009   // WRITE -> Imprime el valor de 'z' (memoria 09)
+43000   // HALT  -> Finaliza la ejecucion

```
Así, se espera que al ejecutarse este programa en nuestro simulador.exe, la consola muestre lo siguiente:

``` 
# Consola de nuestro simulador Simpletron
*** Ingrese un entero: 12
*** Ingrese un entero: 8
*** Resultado: 20

*** Ejecución de Simpletron terminada ***

```

## Manejo de Errores Fatales
El simulador cuenta con validaciones de seguridad que detienen la ejecución de forma segura en los siguientes casos:

#### 1. División entre Cero (Opcode 32):
Aborta cuando el divisor en memoria almacena la constante 0.

#### 2. Código de Opción Inválido (OpCode invalido):
Aborta cuando lee una instrucción fuera de la tabla de opcodes reconocidos.

#### 3. Acceso Fuera de Límites:
Aborta si intenta acceder a direcciones de memoria menores a 0 o mayores a 99.

#### 4. Desbordamiento del Acumulador (Overflow):
Aborta cuando una operación aritmética sobrepasa el límite numérico del acumulador.

###### Autoría: El presente proyecto fue realizado por mí, mediante el uso de herramientas tecnológicas y literatura que abonó al cumplimiento de lo solicitado.