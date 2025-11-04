#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "TADCarta.h"

#define MAXCARTAS 52

typedef struct {
    tCarta cartas[MAXCARTAS];
    int tope;
} tMazo;


void crearMazo(tMazo *);    // Inicializa el mazo vacío
void llenarMazo(tMazo *);   // Llena el mazo con las 52 cartas
void mezclarMazo(tMazo *);  // Mezcla aleatoriamente las cartas
bool mazoVacio(tMazo);      // Verifica si el mazo está vacío
bool mazoLleno(tMazo);      // Verifica si el mazo está lleno
int cantidadCartas(tMazo);  // Retorna la cantidad actual de cartas

void apilarCarta(tMazo *, tCarta);      // Apila una carta 
void desapilarCarta(tMazo *, tCarta *); // Desapila una carta 

void repartirCarta(tMazo *, tCarta *);  // Reparte la carta del tope

void crearMazo(tMazo *pMazo) {
    if (pMazo == NULL) {
        printf("\nError al crear el mazo...");
    } else {
        pMazo->tope = 0;
    }
}

bool mazoVacio(tMazo pMazo) {
    return pMazo.tope == 0;
}

bool mazoLleno(tMazo pMazo) {
    return pMazo.tope == MAXCARTAS;
}

int cantidadCartas(tMazo pMazo) {
    return pMazo.tope;
}

void apilarCarta(tMazo *pMazo, tCarta carta) {
    if (pMazo == NULL) {
        printf("\nError: mazo invalido...");
    } else if (mazoLleno(*pMazo)) {
        printf("\nError: el mazo está lleno...");
    } else {
        pMazo->cartas[pMazo->tope] = carta;
        pMazo->tope++;
    }
}

void desapilarCarta(tMazo *pMazo, tCarta *pCarta) {
    if (pMazo == NULL || pCarta == NULL) {
        printf("\nError: parámetros invalidos...");
    } else if (mazoVacio(*pMazo)) {
        printf("\nError: el mazo está vacío...");
    } else {
        pMazo->tope--;
        *pCarta = pMazo->cartas[pMazo->tope];
    }
}

void llenarMazo(tMazo *pMazo) {
    int i, j;
    tCarta nuevaCarta;

    if (pMazo == NULL) {
        printf("\nError: mazo invalido...");
    } else {
        for (i = CORAZONES; i <= TREBOLES; i++) {
            for (j = AS; j <= REY; j++) {
                crearCarta(&nuevaCarta, (tPalo)i, (tValor)j);
                apilarCarta(pMazo, nuevaCarta);
            }
        }
    }
}

void mezclarMazo(tMazo *pMazo) {
    int i, indiceAleatorio;
    tCarta temp;

    if (pMazo == NULL) {
        printf("\nError: mazo invalido...");
    } else {
        srand(time(NULL));
        for (i = pMazo->tope - 1; i > 0; i--) {
            indiceAleatorio = rand() % (i + 1);
            temp = pMazo->cartas[i];
            pMazo->cartas[i] = pMazo->cartas[indiceAleatorio];
            pMazo->cartas[indiceAleatorio] = temp;
        }
    }
}

void repartirCarta(tMazo *pMazo, tCarta *pCarta) {
    if (pMazo == NULL || pCarta == NULL) {
        printf("\nError: parámetros invalidos...");
    } else if (mazoVacio(*pMazo)) {
        printf("\nError: el mazo está vacío...");
    } else {
        desapilarCarta(pMazo, pCarta);
    }
}
