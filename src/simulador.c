#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANO_MEMORIA 1000
#define CENTINELA 99999

void imprimirDump(double acumulador, int contadorInstrucciones, int registroInstruccion, int codigoOperacion, int operando, double memoria[]);
int validarOverflow(double valor);
void cargarDesdeTeclado(double memoria[]);
void cargarDesdeArchivo(double memoria[], const char *nombreArchivo);

int main() {
    double memoria[TAMANO_MEMORIA] = {0.0};
    double acumulador = 0.0;
    int contadorInstrucciones = 0;
    int registroInstruccion = 0;
    int codigoOperacion = 0;
    int operando = 0;

    printf("*** Bienvenido al Simulador Simpletron v2.0 (1000 Memoria / Flotantes / Cadenas) ***\n\n");

    cargarDesdeArchivo(memoria, "programa.simp");

    printf("\n*** Comienza la ejecucion del programa ***\n\n");
    fflush(stdout);

    contadorInstrucciones = 0;
    int ejecutando = 1;

    while (ejecutando && contadorInstrucciones < TAMANO_MEMORIA) {
        // FETCH
        registroInstruccion = (int)memoria[contadorInstrucciones];

        // DECODE (5 dígitos para 1000 palabras)
        codigoOperacion = registroInstruccion / 1000;
        operando = registroInstruccion % 1000;

        contadorInstrucciones++;

        // EXECUTE
        switch (codigoOperacion) {
            case 10: // READ
                printf("Teclee un Numero (entero o flotante): ");
                fflush(stdout);
                scanf("%lf", &memoria[operando]);
                break;

            case 11: // WRITE
                printf("%03d => %.2f\n", operando, memoria[operando]);
                fflush(stdout);
                break;

            case 12: // NEWLINE
                printf("\n");
                fflush(stdout);
                break;

            case 13: // READ_STRING
                {
                    char cadena[200];
                    printf("Teclee una Cadena: ");
                    fflush(stdout);
                    scanf(" %[^\n]", cadena);

                    int longitud = strlen(cadena);
                    int dirActual = operando;

                    memoria[dirActual] = longitud * 1000;
                    dirActual++;

                    for (int i = 0; i < longitud && dirActual < TAMANO_MEMORIA; i++) {
                        memoria[dirActual] = (int)cadena[i];
                        dirActual++;
                    }
                }
                break;

            case 14: // WRITE_STRING
                {
                    int dirActual = operando;
                    int palabraLongitud = (int)memoria[dirActual];
                    int longitud = palabraLongitud / 1000;
                    dirActual++;

                    for (int i = 0; i < longitud && dirActual < TAMANO_MEMORIA; i++) {
                        char c = (char)((int)memoria[dirActual] % 1000);
                        printf("%c", c);
                        dirActual++;
                    }
                    printf("\n");
                    fflush(stdout);
                }
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
                if (memoria[operando] != 0.0) {
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

            case 34: // MODULO / RESIDUO
                if ((int)memoria[operando] != 0) {
                    acumulador = (int)acumulador % (int)memoria[operando];
                } else {
                    printf("*** ERROR FATAL: Intento de modulo por cero ***\n");
                    ejecutando = 0;
                }
                break;

            case 35: // EXPONENCIACION
                {
                    double base = acumulador;
                    int exponente = (int)memoria[operando];
                    double resultado = 1.0;

                    if (exponente < 0) {
                        for (int i = 0; i < -exponente; i++) resultado *= base;
                        resultado = 1.0 / resultado;
                    } else {
                        for (int i = 0; i < exponente; i++) resultado *= base;
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
                if (acumulador < 0.0) {
                    contadorInstrucciones = operando;
                }
                break;

            case 42: // BRANCHZERO
                if (acumulador == 0.0) {
                    contadorInstrucciones = operando;
                }
                break;

            case 43: // HALT
                printf("*** La ejecucion de Simpletron termino normalmente ***\n\n");
                fflush(stdout);
                ejecutando = 0;
                break;

            default:
                printf("*** ERROR FATAL: Codigo de operacion invalido (%d) en la instruccion %05d ***\n", codigoOperacion, registroInstruccion);
                ejecutando = 0;
                break;
        }
    }

    imprimirDump(acumulador, contadorInstrucciones, registroInstruccion, codigoOperacion, operando, memoria);

    return 0;
}

int validarOverflow(double valor) {
    if (valor < -99999.0 || valor > 99999.0) {
        printf("*** ERROR FATAL: Desbordamiento del Acumulador (Overflow) ***\n");
        return 1;
    }
    return 0;
}

void cargarDesdeTeclado(double memoria[]) {
    int instruccionEntrada = 0;
    int direccion = 0;

    printf("\n*** Introduzca su programa una instruccion ***\n");
    printf("*** Capture el numero 99999 para terminar. ***\n\n");

    while (direccion < TAMANO_MEMORIA) {
        printf("%03d ? ", direccion);
        fflush(stdout);
        scanf("%d", &instruccionEntrada);

        if (instruccionEntrada == CENTINELA) break;

        memoria[direccion] = (double)instruccionEntrada;
        direccion++;
    }
    printf("*** Se termino de cargar el programa. ***\n");
}

void cargarDesdeArchivo(double memoria[], const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("*** AVISO: El archivo '%s' no existe ***\n", nombreArchivo);
        printf("*** Iniciando en modo interactivo (por teclado) ***\n");
        cargarDesdeTeclado(memoria);
        return;
    }

    int instruccion = 0;
    int direccion = 0;

    while (fscanf(archivo, "%d", &instruccion) == 1 && direccion < TAMANO_MEMORIA) {
        if (instruccion == CENTINELA || instruccion == 9999) break;

        memoria[direccion] = (double)instruccion;
        direccion++;
    }

    fclose(archivo);
    printf("*** Programa cargado exitosamente desde '%s' ***\n", nombreArchivo);
}

// IMPRESIÓN CORREGIDA PARA LAS 1000 POSICIONES
void imprimirDump(double acumulador, int contadorInstrucciones, int registroInstruccion, int codigoOperacion, int operando, double memoria[]) {
    printf("Registros:\n");
    printf("acumulador:            %+09.2f\n", acumulador);
    printf("instructionCounter:      %03d\n", contadorInstrucciones);
    printf("instructionRegister:   %+06d\n", registroInstruccion);
    printf("operationcode:            %02d\n", codigoOperacion);
    printf("operand:                 %03d\n\n", operando);

    printf("MEMORIA:\n");
    printf("       0      1      2      3      4      5      6      7      8      9\n");

    for (int i = 0; i < TAMANO_MEMORIA; i += 10) {
        printf("%03d ", i);
        for (int j = 0; j < 10; j++) {
            printf("%+06.0f ", memoria[i + j]);
        }
        printf("\n");
    }
}