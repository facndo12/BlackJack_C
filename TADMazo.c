#include "TADMazo.h"
#include <stdlib.h> // Para rand
// Nota: removemos time.h y srand porque la semilla debe ir en main

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
        printf("\nError: el mazo esta lleno...");
    } else {
        pMazo->cartas[pMazo->tope] = carta;
        pMazo->tope++;
    }
}

void desapilarCarta(tMazo *pMazo, tCarta *pCarta) {
    if (pMazo == NULL || pCarta == NULL) {
        printf("\nError: parametros invalidos...");
    } else if (mazoVacio(*pMazo)) {
        printf("\nError: el mazo esta vacio...");
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
        // NOTA: Se eliminó srand(time(NULL)) de aquí.
        // Debe llamarse una sola vez en el main.c
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
        printf("\nError: parametros invalidos...");
    } else if (mazoVacio(*pMazo)) {
        printf("\nError: el mazo esta vacio...");
    } else {
        desapilarCarta(pMazo, pCarta);
    }
}

void reiniciarMazo(tMazo *pMazo) {
    if (pMazo == NULL) {
        printf("\nError: mazo invalido...");
    } else {
        pMazo->tope = 0;
        llenarMazo(pMazo);
        mezclarMazo(pMazo);
    }
}

void llenarMazosMultiples(tMazo *pMazo, int numMazos) {
    int i;
    if (pMazo == NULL) {
        printf("\nError: mazo invalido...");
        return;
    }
    pMazo->tope = 0;
    for (i = 0; i < numMazos; i++) {
        llenarMazo(pMazo);
    }
}

void mezclarMazoProfundo(tMazo *pMazo, int numMezclas) {
    int i;
    if (pMazo == NULL) {
        printf("\nError: mazo invalido...");
        return;
    }
    for (i = 0; i < numMezclas; i++) {
        mezclarMazo(pMazo);
    }
}
