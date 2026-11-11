#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAMANO_MEMORIA 1000
#define MAX_SIMBOLOS 200
#define MAX_LINEAS 200

typedef struct {
    int simbolo;       // Número de línea o valor entero de constante
    char tipo;         // 'L' = Linea, 'V' = Variable, 'C' = Constante
    int ubicacion;     // Dirección SML asignada (000-999)
    char nombreVar[10];
} Simbolo;

typedef struct {
    int instruccionDireccion;
    int lineaDestino;
} ReferenciaInconclusa;

Simbolo tablaSimbolos[MAX_SIMBOLOS];
int contadorSimbolos = 0;

ReferenciaInconclusa referencias[MAX_LINEAS];
int contadorReferencias = 0;

int codigoSML[TAMANO_MEMORIA];
int contadorInstrucciones = 0;
int ubicacionDatos = 999;

// Pila para Infijo a Postfijo
typedef struct {
    char items[100];
    int tope;
} Pila;

void inicializarPila(Pila *p) { p->tope = -1; }
int estaVacia(Pila *p) { return p->tope == -1; }
void empujar(Pila *p, char c) { p->items[++(p->tope)] = c; }
char sacar(Pila *p) { return p->items[(p->tope)--]; }
char obtenerTope(Pila *p) { return estaVacia(p) ? '\0' : p->items[p->tope]; }

int obtenerPrecedencia(char op) {
    if (op == '^') return 3;
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

int esOperador(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^');
}

// Convierte manteniendo números completos de varios dígitos juntos
void infijoAPostfijo(const char *infijo, char *postfijo) {
    Pila pila;
    inicializarPila(&pila);
    int k = 0;
    int i = 0;

    while (infijo[i] != '\0') {
        char actual = infijo[i];

        if (isspace(actual)) {
            i++;
            continue;
        }

        // Si es número o variable (alfanumérico completo)
        if (isalnum(actual)) {
            while (isalnum(infijo[i])) {
                postfijo[k++] = infijo[i++];
            }
            postfijo[k++] = ' ';
        } 
        else if (actual == '(') {
            empujar(&pila, actual);
            i++;
        } 
        else if (actual == ')') {
            while (!estaVacia(&pila) && obtenerTope(&pila) != '(') {
                postfijo[k++] = sacar(&pila);
                postfijo[k++] = ' ';
            }
            if (!estaVacia(&pila) && obtenerTope(&pila) == '(') sacar(&pila);
            i++;
        } 
        else if (esOperador(actual)) {
            while (!estaVacia(&pila) && 
                   ((actual != '^' && obtenerPrecedencia(obtenerTope(&pila)) >= obtenerPrecedencia(actual)) ||
                    (actual == '^' && obtenerPrecedencia(obtenerTope(&pila)) > obtenerPrecedencia(actual)))) {
                postfijo[k++] = sacar(&pila);
                postfijo[k++] = ' ';
            }
            empujar(&pila, actual);
            i++;
        } else {
            i++;
        }
    }

    while (!estaVacia(&pila)) {
        postfijo[k++] = sacar(&pila);
        postfijo[k++] = ' ';
    }
    if (k > 0 && postfijo[k - 1] == ' ') k--;
    postfijo[k] = '\0';
}

int buscarSimboloLinea(int linea) {
    for (int i = 0; i < contadorSimbolos; i++) {
        if (tablaSimbolos[i].tipo == 'L' && tablaSimbolos[i].simbolo == linea) {
            return tablaSimbolos[i].ubicacion;
        }
    }
    return -1;
}

int obtenerODireccionarVariable(const char *nombre) {
    for (int i = 0; i < contadorSimbolos; i++) {
        if (tablaSimbolos[i].tipo == 'V' && strcmp(tablaSimbolos[i].nombreVar, nombre) == 0) {
            return tablaSimbolos[i].ubicacion;
        }
    }
    int dir = ubicacionDatos--;
    tablaSimbolos[contadorSimbolos].tipo = 'V';
    strcpy(tablaSimbolos[contadorSimbolos].nombreVar, nombre);
    tablaSimbolos[contadorSimbolos].ubicacion = dir;
    contadorSimbolos++;
    return dir;
}

int obtenerODireccionarConstante(int val) {
    for (int i = 0; i < contadorSimbolos; i++) {
        if (tablaSimbolos[i].tipo == 'C' && tablaSimbolos[i].simbolo == val) {
            return tablaSimbolos[i].ubicacion;
        }
    }
    int dir = ubicacionDatos--;
    tablaSimbolos[contadorSimbolos].tipo = 'C';
    tablaSimbolos[contadorSimbolos].simbolo = val;
    tablaSimbolos[contadorSimbolos].ubicacion = dir;
    codigoSML[dir] = val;
    contadorSimbolos++;
    return dir;
}

void agregarLineaSimbolo(int linea, int ubicacion) {
    tablaSimbolos[contadorSimbolos].tipo = 'L';
    tablaSimbolos[contadorSimbolos].simbolo = linea;
    tablaSimbolos[contadorSimbolos].ubicacion = ubicacion;
    contadorSimbolos++;
}

void compilarExpresionPostfija(const char *postfijo, int dirResultado) {
    char copia[100];
    strcpy(copia, postfijo);
    char *token = strtok(copia, " ");
    int pilaTemporal[50];
    int topeTemp = -1;

    while (token != NULL) {
        int esNumero = 1;
        for (int i = 0; token[i] != '\0'; i++) {
            if (!isdigit(token[i])) {
                esNumero = 0;
                break;
            }
        }

        if (esNumero) {
            int dirConst = obtenerODireccionarConstante(atoi(token));
            pilaTemporal[++topeTemp] = dirConst;
        } 
        else if (isalpha(token[0])) {
            int dirVar = obtenerODireccionarVariable(token);
            pilaTemporal[++topeTemp] = dirVar;
        } 
        else if (esOperador(token[0])) {
            int dirOp2 = pilaTemporal[topeTemp--];
            int dirOp1 = pilaTemporal[topeTemp--];
            int dirTemp = ubicacionDatos--;

            codigoSML[contadorInstrucciones++] = 20000 + dirOp1;

            switch (token[0]) {
                case '+': codigoSML[contadorInstrucciones++] = 30000 + dirOp2; break;
                case '-': codigoSML[contadorInstrucciones++] = 31000 + dirOp2; break;
                case '*': codigoSML[contadorInstrucciones++] = 33000 + dirOp2; break;
                case '/': codigoSML[contadorInstrucciones++] = 32000 + dirOp2; break;
                case '%': codigoSML[contadorInstrucciones++] = 34000 + dirOp2; break;
                case '^': codigoSML[contadorInstrucciones++] = 35000 + dirOp2; break;
            }

            codigoSML[contadorInstrucciones++] = 21000 + dirTemp;
            pilaTemporal[++topeTemp] = dirTemp;
        }
        token = strtok(NULL, " ");
    }

    int dirResultadoFinal = pilaTemporal[topeTemp--];
    codigoSML[contadorInstrucciones++] = 20000 + dirResultadoFinal;
    codigoSML[contadorInstrucciones++] = 21000 + dirResultado;
}

int main() {
    FILE *archivoEntrada = fopen("programa.simple", "r");
    if (archivoEntrada == NULL) {
        printf("*** ERROR: No se pudo abrir 'programa.simple'. ***\n");
        return 1;
    }

    printf("*** COMPILADOR SIMPLETRON v2.0 ***\n");
    printf("Iniciando Primera Pasada...\n\n");

    char lineaBuffer[150];
    while (fgets(lineaBuffer, sizeof(lineaBuffer), archivoEntrada) != NULL) {
        if (strlen(lineaBuffer) <= 1) continue;

        int numLinea;
        char comando[20];
        sscanf(lineaBuffer, "%d %s", &numLinea, comando);

        agregarLineaSimbolo(numLinea, contadorInstrucciones);

        if (strcmp(comando, "input") == 0) {
            char var[5];
            sscanf(lineaBuffer, "%*d input %s", var);
            int dir = obtenerODireccionarVariable(var);
            codigoSML[contadorInstrucciones++] = 10000 + dir;
        } 
        else if (strcmp(comando, "print") == 0) {
            char var[5];
            sscanf(lineaBuffer, "%*d print %s", var);
            int dir = obtenerODireccionarVariable(var);
            codigoSML[contadorInstrucciones++] = 11000 + dir;
        }
        else if (strcmp(comando, "goto") == 0) {
            int lineaDestino;
            sscanf(lineaBuffer, "%*d goto %d", &lineaDestino);
            referencias[contadorReferencias].instruccionDireccion = contadorInstrucciones;
            referencias[contadorReferencias].lineaDestino = lineaDestino;
            contadorReferencias++;
            codigoSML[contadorInstrucciones++] = 40000;
        }
        else if (strcmp(comando, "if") == 0) {
            char var1[5], relop[3], var2[10];
            int lineaDestino;
            sscanf(lineaBuffer, "%*d if %s %s %s goto %d", var1, relop, var2, &lineaDestino);

            int dir1 = obtenerODireccionarVariable(var1);
            int dir2 = isdigit(var2[0]) ? obtenerODireccionarConstante(atoi(var2)) : obtenerODireccionarVariable(var2);

            codigoSML[contadorInstrucciones++] = 20000 + dir1;
            codigoSML[contadorInstrucciones++] = 31000 + dir2;

            referencias[contadorReferencias].instruccionDireccion = contadorInstrucciones;
            referencias[contadorReferencias].lineaDestino = lineaDestino;
            contadorReferencias++;

            if (strcmp(relop, "==") == 0) codigoSML[contadorInstrucciones++] = 42000;
            else if (strcmp(relop, "<") == 0) codigoSML[contadorInstrucciones++] = 41000;
        }
        else if (strcmp(comando, "let") == 0) {
            char varDestino[5], expresionInfija[100], expresionPostfija[100];
            char *posIgual = strchr(lineaBuffer, '=');
            sscanf(lineaBuffer, "%*d let %s", varDestino);
            strcpy(expresionInfija, posIgual + 1);

            int dirDestino = obtenerODireccionarVariable(varDestino);
            infijoAPostfijo(expresionInfija, expresionPostfija);
            compilarExpresionPostfija(expresionPostfija, dirDestino);
        }
        else if (strcmp(comando, "end") == 0) {
            codigoSML[contadorInstrucciones++] = 43000;
        }
    }
    fclose(archivoEntrada);

    // SEGUNDA PASADA
    printf("Iniciando Segunda Pasada...\n");
    for (int i = 0; i < contadorReferencias; i++) {
        int dirSalto = buscarSimboloLinea(referencias[i].lineaDestino);
        if (dirSalto != -1) {
            codigoSML[referencias[i].instruccionDireccion] += dirSalto;
        }
    }

    // Escribir archivo ejecutable final .simp
    FILE *archivoSalida = fopen("programa.simp", "w");
    
    // Escribir instrucciones
    for (int i = 0; i < contadorInstrucciones; i++) {
        fprintf(archivoSalida, "%05d\n", codigoSML[i]);
    }
    
    // Rellenar ceros intermedios hasta llegar a las constantes
    int minConstante = 1000;
    for (int i = 0; i < contadorSimbolos; i++) {
        if (tablaSimbolos[i].tipo == 'C' && tablaSimbolos[i].ubicacion < minConstante) {
            minConstante = tablaSimbolos[i].ubicacion;
        }
    }

    if (minConstante < 1000) {
        for (int i = contadorInstrucciones; i < minConstante; i++) {
            fprintf(archivoSalida, "+00000\n");
        }
        for (int i = minConstante; i < 1000; i++) {
            fprintf(archivoSalida, "%+06d\n", codigoSML[i]);
        }
    } else {
        fprintf(archivoSalida, "99999\n");
    }

    fclose(archivoSalida);

    printf("\n--------------------------------------------------\n");
    printf("*** COMPILACION EXITOSA ***\n");
    printf("Se ha generado el archivo 'programa.simp' ejecutable.\n\n");
    printf("Tabla de Simbolos:\n");
    printf("Tipo\tNombre/Linea\tUbicacion SML\n");
    for (int i = 0; i < contadorSimbolos; i++) {
        if (tablaSimbolos[i].tipo == 'L') {
            printf("Linea\t%d\t\t%03d\n", tablaSimbolos[i].simbolo, tablaSimbolos[i].ubicacion);
        } else if (tablaSimbolos[i].tipo == 'V') {
            printf("Var\t%s\t\t%03d\n", tablaSimbolos[i].nombreVar, tablaSimbolos[i].ubicacion);
        } else if (tablaSimbolos[i].tipo == 'C') {
            printf("Const\t%d\t\t%03d\n", tablaSimbolos[i].simbolo, tablaSimbolos[i].ubicacion);
        }
    }
    printf("--------------------------------------------------\n");

    return 0;
}