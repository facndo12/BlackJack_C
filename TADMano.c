#include "TADMano.h"
#include <stdlib.h> // Para malloc y free

void crearMano(tMano *pMano) {
    if (pMano == NULL) {
        printf("\nError al crear la mano...");
    } else {
        pMano->primero = NULL;
        pMano->cantidad = 0;
    }
}

void agregarCartaMano(tMano *pMano, tCarta pCarta) {
    tNodo *nuevo, *actual;
    if (pMano == NULL) {
        printf("\nError: mano invalida...");
    } else {
        nuevo = (tNodo*)malloc(sizeof(tNodo));
        if (nuevo == NULL) {
            printf("\nError: no hay memoria disponible...");
        } else {
            nuevo->carta = pCarta;
            nuevo->siguiente = NULL;
            if (pMano->primero == NULL) {
                pMano->primero = nuevo;
            } else {
                actual = pMano->primero;
                while (actual->siguiente != NULL) {
                    actual = actual->siguiente;
                }
                actual->siguiente = nuevo;
            }
            pMano->cantidad++;
        }
    }
}

int calcularValorMano(tMano *pMano) {
    tNodo *actual;
    int valor, ases;
    if (pMano == NULL) {
        return 0;
    }
    valor = 0;
    ases = 0;
    actual = pMano->primero;
    while (actual != NULL) {
        if (esAs(actual->carta)) {
            ases++;
            valor += 11;
        } else {
            valor += valorCartaJuego(actual->carta);
        }
        actual = actual->siguiente;
    }
    // Ajuste de Ases si nos pasamos de 21
    while (valor > 21 && ases > 0) {
        valor -= 10;
        ases--;
    }
    return valor;
}

bool sePasa(tMano *pMano) {
    if (pMano == NULL) {
        return false;
    } else {
        return calcularValorMano(pMano) > 21;
    }
}

bool esBlackjack(tMano *pMano) {
    if (pMano == NULL) {
        return false;
    } else {
        return pMano->cantidad == 2 && calcularValorMano(pMano) == 21;
    }
}

int cantidadCartasMano(tMano *pMano) {
    if (pMano == NULL) {
        return 0;
    } else {
        return pMano->cantidad;
    }
}

void vaciarMano(tMano *pMano) {
    tNodo *actual, *siguiente;
    if (pMano == NULL) {
        printf("\nError: mano invalida...");
    } else {
        actual = pMano->primero;
        while (actual != NULL) {
            siguiente = actual->siguiente;
            free(actual);
            actual = siguiente;
        }
        pMano->primero = NULL;
        pMano->cantidad = 0;
    }
}

void mostrarMano(tMano *pMano) {
    tNodo *actual;
    if (pMano == NULL) {
        printf("\nMano invalida\n");
    } else if (pMano->cantidad == 0) {
        printf("Mano vacia\n");
    } else {
        actual = pMano->primero;
        while (actual != NULL) {
            mostrarCarta(&actual->carta);
            actual = actual->siguiente;
        }
        printf("\nValor total: %d\n", calcularValorMano(pMano));
    }
}

void mostrarManoOculta(tMano *pMano) {
    tNodo *actual;
    if (pMano == NULL) {
        printf("\nMano invalida\n");
    } else if (pMano->cantidad == 0) {
        printf("Mano vacia\n");
    } else {
        printf("\t[CARTA OCULTA]\n");
        actual = pMano->primero->siguiente;
        while (actual != NULL) {
            mostrarCarta(&actual->carta);
            actual = actual->siguiente;
        }
    }
}
