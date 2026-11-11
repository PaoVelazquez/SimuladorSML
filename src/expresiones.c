#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAMANO_PILA 100
#define TAMANO_EXPRESION 200

// Estructura de la Pila de Caracteres
typedef struct {
    char items[TAMANO_PILA];
    int tope;
} Pila;

// Funciones para el manejo de la Pila
void inicializarPila(Pila *p);
int estaVacia(Pila *p);
int estaLlena(Pila *p);
void empujar(Pila *p, char caracter);
char sacar(Pila *p);
char obtenerTope(Pila *p);

// Funciones de conversión
int obtenerPrecedencia(char operador);
int esOperador(char c);
void infijoAPostfijo(const char *infijo, char *postfijo);

int main() {
    char expresionInfija[TAMANO_EXPRESION];
    char expresionPostfija[TAMANO_EXPRESION];

    printf("*** Modulo de Conversion Infijo a Postfijo (Evaluador de Expresiones) ***\n\n");

    // Ejemplo de prueba predeterminado o entrada por teclado
    printf("Ingrese una expresion en notacion infija (ejemplo: (A + B) * (C - D) / E ^ F):\n> ");
    if (fgets(expresionInfija, sizeof(expresionInfija), stdin) != NULL) {
        // Eliminar el salto de línea al final si existe
        expresionInfija[strcspn(expresionInfija, "\n")] = '\0';
    }

    infijoAPostfijo(expresionInfija, expresionPostfija);

    printf("\n--------------------------------------------------\n");
    printf("Expresion Infija:   %s\n", expresionInfija);
    printf("Expresion Postfija: %s\n", expresionPostfija);
    printf("--------------------------------------------------\n\n");

    return 0;
}

// Inicializa la pila colocando el tope en -1
void inicializarPila(Pila *p) {
    p->tope = -1;
}

int estaVacia(Pila *p) {
    return p->tope == -1;
}

int estaLlena(Pila *p) {
    return p->tope == TAMANO_PILA - 1;
}

void empujar(Pila *p, char caracter) {
    if (estaLlena(p)) {
        printf("*** ERROR: Desbordamiento de Pila (Stack Overflow) ***\n");
        exit(1);
    }
    p->items[++(p->tope)] = caracter;
}

char sacar(Pila *p) {
    if (estaVacia(p)) {
        printf("*** ERROR: Subdesbordamiento de Pila (Stack Underflow) ***\n");
        exit(1);
    }
    return p->items[(p->tope)--];
}

char obtenerTope(Pila *p) {
    if (estaVacia(p)) {
        return '\0';
    }
    return p->items[p->tope];
}

// Determina la jerarquía de operadores
int obtenerPrecedencia(char operador) {
    switch (operador) {
        case '^':
            return 3; // Exponenciación (Máxima precedencia)
        case '*':
        case '/':
        case '%':
            return 2; // Multiplicación, División, Módulo
        case '+':
        case '-':
            return 1; // Suma y Resta
        default:
            return 0; // Paréntesis u otros
    }
}

int esOperador(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^');
}

// Algoritmo principal de conversión
void infijoAPostfijo(const char *infijo, char *postfijo) {
    Pila pila;
    inicializarPila(&pila);
    int k = 0; // Índice para el arreglo postfijo

    for (int i = 0; infijo[i] != '\0'; i++) {
        char actual = infijo[i];

        // Si es espacio en blanco, lo ignoramos
        if (isspace(actual)) {
            continue;
        }

        // 1. Si es un operando (letra o número), se añade directamente a la salida postfija
        if (isalnum(actual)) {
            postfijo[k++] = actual;
            postfijo[k++] = ' '; // Espacio separador
        }
        // 2. Si es un paréntesis de apertura, se apila
        else if (actual == '(') {
            empujar(&pila, actual);
        }
        // 3. Si es un paréntesis de cierre, desapilamos hasta encontrar el '('
        else if (actual == ')') {
            while (!estaVacia(&pila) && obtenerTope(&pila) != '(') {
                postfijo[k++] = sacar(&pila);
                postfijo[k++] = ' ';
            }
            if (!estaVacia(&pila) && obtenerTope(&pila) == '(') {
                sacar(&pila); // Elimina el '(' de la pila
            }
        }
        // 4. Si es un operador (+, -, *, /, %, ^)
        else if (esOperador(actual)) {
            // Caso especial: la exponenciación '^' suele ser asociativa por la derecha
            while (!estaVacia(&pila) && 
                   ((actual != '^' && obtenerPrecedencia(obtenerTope(&pila)) >= obtenerPrecedencia(actual)) ||
                    (actual == '^' && obtenerPrecedencia(obtenerTope(&pila)) > obtenerPrecedencia(actual)))) {
                postfijo[k++] = sacar(&pila);
                postfijo[k++] = ' ';
            }
            empujar(&pila, actual);
        }
    }

    // Vaciar los operadores restantes en la pila
    while (!estaVacia(&pila)) {
        postfijo[k++] = sacar(&pila);
        postfijo[k++] = ' ';
    }

    // Eliminar espacio sobrante si lo hay y colocar fin de cadena
    if (k > 0 && postfijo[k - 1] == ' ') {
        k--;
    }
    postfijo[k] = '\0';
}