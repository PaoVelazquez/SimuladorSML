#include <stdio.h>

#define TAMANO_MEMORIA 100 // Pidió 100 lugares, se cambia para v2
#define CENTINELA 9999  // 9999

void imprimirDump(int acumulador, int contadorInstrucciones, int registroInstruccion, int codigoOperacion, int operando, int memoria[]);

int main() {
	
	// Dejo todas mis variables vacías (vaciado de memoria)
    int memoria[TAMANO_MEMORIA] = {0};
    int acumulador = 0;
    int contadorInstrucciones = 0;
    int registroInstruccion = 0;
    int codigoOperacion = 0;
    int operando = 0;
    int instruccionEntrada = 0;

    // Encabezado
    printf("** ¡Bienvenido a Simpletron! ***\n");
    printf("** Introduzca su programa, una instruccion ***\n");
    printf("** (o palabra de datos) a la vez en la línea ***\n");
    printf("** de texto de entrada. Yo indicaré el numero ***\n");
    printf("** de posicion y una interrogacion(?). Usted ***\n");
    printf("** tecleara entonces una palabra para esa ***\n");
    printf("** posicion. Capture el numero 9999 para ***\n");
    printf("** terminar de introducir su programa. ***\n\n");
    fflush(stdout);

    // Recepción de instrucciones
    while (contadorInstrucciones < TAMANO_MEMORIA) {
        printf("%02d ? ", contadorInstrucciones);
        fflush(stdout);
        scanf("%d", &instruccionEntrada);

        if (instruccionEntrada == CENTINELA) {
            break;
        }

        memoria[contadorInstrucciones] = instruccionEntrada;
        contadorInstrucciones++;
    }

    printf("*** Se termino de cargar el programa. ***\n");
    printf("*** Comienza la ejecucion del programa. ***\n\n");
    fflush(stdout);

    contadorInstrucciones = 0;
    int ejecutando = 1;

    while (ejecutando && contadorInstrucciones < TAMANO_MEMORIA) {
        registroInstruccion = memoria[contadorInstrucciones];
        codigoOperacion = registroInstruccion / 100;
        operando = registroInstruccion % 100;
        contadorInstrucciones++;

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

            case 20: // LOAD
                acumulador = memoria[operando];
                break;

            case 21: // STORE
                memoria[operando] = acumulador;
                break;

            case 30: // ADD
                acumulador += memoria[operando];
                break;

            case 31: // SUBTRACT
                acumulador -= memoria[operando];
                break;

            case 32: // DIVIDE
                if (memoria[operando] != 0) {
                    acumulador /= memoria[operando];
                } else {
                    printf("*** ERROR: Division por cero ***\n");
                    ejecutando = 0;
                }
                break;

            case 33: // MULTIPLY
                acumulador *= memoria[operando];
                break;

            case 40: // BRANCH
                contadorInstrucciones = operando;
                break;

            case 41: // BRANCHNEG
                if (acumulador < 0) contadorInstrucciones = operando;
                break;

            case 42: // BRANCHZERO
                if (acumulador == 0) contadorInstrucciones = operando;
                break;

            case 43: // HALT
                printf("*** La ejecucion de Simpletron termino normalmente ***\n\n");
                fflush(stdout);
                ejecutando = 0;
                break;

            default: // Este es mi código de errores fatales
                printf("*** ERROR: Codigo invalido ***\n");
                ejecutando = 0;
                break;
        }
    }

    imprimirDump(acumulador, contadorInstrucciones, registroInstruccion, codigoOperacion, operando, memoria);
    return 0;
}

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