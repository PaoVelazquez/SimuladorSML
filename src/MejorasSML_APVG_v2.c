#include <stdio.h>
#include <stdlib.h>

#define TAMANO_MEMORIA 100
#define CENTINELA 9999

// Prototipos de funciones
void imprimirDump(int acumulador, int contadorInstrucciones, int registroInstruccion, int codigoOperacion, int operando, int memoria[]);
int validarOverflow(int valor);
void cargarDesdeTeclado(int memoria[]);
void cargarDesdeArchivo(int memoria[]);

int main() {
    int memoria[TAMANO_MEMORIA] = {0};
    int acumulador = 0;
    int contadorInstrucciones = 0;
    int registroInstruccion = 0;
    int codigoOperacion = 0;
    int operando = 0;
    int opcionModo = 0;

    printf("*** ¡Bienvenido a Simpletron v2.0! ***\n");
    printf("Seleccione el modo de carga del programa:\n");
    printf("1. Entrada manual por teclado\n");
    printf("2. Cargar desde archivo (.txt / .sml)\n");
    printf("Opcion: ");
    fflush(stdout);
    scanf("%d", &opcionModo);

    if (opcionModo == 2) {
        cargarDesdeArchivo(memoria);
    } else {
        cargarDesdeTeclado(memoria);
    }

    printf("\n*** Comienza la ejecucion del programa. ***\n\n");
    fflush(stdout);

    contadorInstrucciones = 0;
    int ejecutando = 1;

    // BUCLE PRINCIPAL DE EJECUCIÓN (Fetch - Decode - Execute)
    while (ejecutando && contadorInstrucciones < TAMANO_MEMORIA) {
        // FETCH
        registroInstruccion = memoria[contadorInstrucciones];

        // DECODE
        codigoOperacion = registroInstruccion / 100;
        operando = registroInstruccion % 100;

        contadorInstrucciones++;

        // EXECUTE
        switch (codigoOperacion) {
            case 10: // READ
                printf("\nTeclee un Entero: ");
                fflush(stdout);
                scanf("%d", &memoria[operando]);
                break;

            case 11: // WRITE
                printf("%02d => %d\n", operando, memoria[operando]);
                fflush(stdout);
                break;

            case 12: // NEWLINE (Instrucción V2)
                printf("\n");
                fflush(stdout);
                break;

            case 20: // LOAD
                acumulador = memoria[operando];
                break;

            case 21: // STORE
                memoria[operando] = acumulador;
                break;

            case 30: // ADD
                if (validarOverflow(acumulador + memoria[operando])) {
                    ejecutando = 0;
                } else {
                    acumulador += memoria[operando];
                }
                break;

            case 31: // SUBTRACT
                if (validarOverflow(acumulador - memoria[operando])) {
                    ejecutando = 0;
                } else {
                    acumulador -= memoria[operando];
                }
                break;

            case 32: // DIVIDE
                if (memoria[operando] != 0) {
                    acumulador /= memoria[operando];
                } else {
                    printf("*** ERROR FATAL: Intento de division por cero ***\n");
                    ejecutando = 0;
                }
                break;

            case 33: // MULTIPLY
                if (validarOverflow(acumulador * memoria[operando])) {
                    ejecutando = 0;
                } else {
                    acumulador *= memoria[operando];
                }
                break;

            case 34: // MODULO / RESIDUO (Instrucción V2)
                if (memoria[operando] != 0) {
                    acumulador %= memoria[operando];
                } else {
                    printf("*** ERROR FATAL: Intento de modulo por cero ***\n");
                    ejecutando = 0;
                }
                break;

            case 35: // EXPONENCIACION (Instrucción V2)
                {
                    int base = acumulador;
                    int exponente = memoria[operando];
                    int resultado = 1;

                    for (int i = 0; i < exponente; i++) {
                        resultado *= base;
                    }

                    if (validarOverflow(resultado)) {
                        ejecutando = 0;
                    } else {
                        acumulador = resultado;
                    }
                }
                break;

            case 40: // BRANCH
                contadorInstrucciones = operando;
                break;

            case 41: // BRANCHNEG
                if (acumulador < 0) {
                    contadorInstrucciones = operando;
                }
                break;

            case 42: // BRANCHZERO
                if (acumulador == 0) {
                    contadorInstrucciones = operando;
                }
                break;

            case 43: // HALT
                printf("*** La ejecucion de Simpletron termino normalmente ***\n\n");
                fflush(stdout);
                ejecutando = 0;
                break;

            default:
                printf("*** ERROR FATAL: Codigo de operacion invalido (%d) ***\n", codigoOperacion);
                ejecutando = 0;
                break;
        }
    }

    // DUMP FINAL DE MEMORIA Y REGISTROS
    imprimirDump(acumulador, contadorInstrucciones, registroInstruccion, codigoOperacion, operando, memoria);

    return 0;
}

// Revisa si una cifra excede el rango [-9999, 9999]
int validarOverflow(int valor) {
    if (valor < -9999 || valor > 9999) {
        printf("*** ERROR FATAL: Desbordamiento del Acumulador (%d) ***\n", valor);
        return 1;
    }
    return 0;
}

// Carga las instrucciones por teclado
void cargarDesdeTeclado(int memoria[]) {
    int instruccionEntrada = 0;
    int direccion = 0;

    printf("\n*** Introduzca su programa una instruccion ***\n");
    printf("*** Capture el numero 9999 para terminar. ***\n\n");

    while (direccion < TAMANO_MEMORIA) {
        printf("%02d ? ", direccion);
        fflush(stdout);
        scanf("%d", &instruccionEntrada);

        if (instruccionEntrada == CENTINELA) break;

        memoria[direccion] = instruccionEntrada;
        direccion++;
    }
    printf("*** Se termino de cargar el programa. ***\n");
}

// Carga las instrucciones desde un archivo externo (.sml / .txt)
void cargarDesdeArchivo(int memoria[]) {
    char nombreArchivo[100];
    printf("Ingrese el nombre del archivo (ej. prueba.sml): ");
    fflush(stdout);
    scanf("%s", nombreArchivo);

    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("*** ERROR: No se pudo abrir el archivo '%s' ***\n", nombreArchivo);
        printf("*** Iniciando en modo manual por teclado ***\n");
        cargarDesdeTeclado(memoria);
        return;
    }

    int instruccion = 0;
    int direccion = 0;

    while (fscanf(archivo, "%d", &instruccion) != EOF && direccion < TAMANO_MEMORIA) {
        if (instruccion == CENTINELA) break;
        memoria[direccion] = instruccion;
        direccion++;
    }

    fclose(archivo);
    printf("*** Programa cargado exitosamente desde '%s' ***\n", nombreArchivo);
}

// Imprime el estado final de los registros y la matriz de memoria
void imprimirDump(int acumulador, int contadorInstrucciones, int registroInstruccion, int codigoOperacion, int operando, int memoria[]) {
    printf("Registros:\n");
    printf("acumulador:            %+05d\n", acumulador);
    printf("instructionCounter:       %2d\n", contadorInstrucciones);
    printf("instructionRegister:   %+05d\n", registroInstruccion);
    printf("operationcode:            %2d\n", codigoOperacion);
    printf("operand:                  %2d\n\n", operando);

    printf("MEMORIA:\n");
    printf("        0     1     2     3     4     5     6     7     8     9\n");

    for (int i = 0; i < 100; i += 10) {
        printf("%2d ", i);
        for (int j = 0; j < 10; j++) {
            printf("%+05d ", memoria[i + j]);
        }
        printf("\n");
    }
}