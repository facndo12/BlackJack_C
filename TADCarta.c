#include "TADCarta.h"

void crearCarta(tCarta *pCarta, tPalo pPalo, tValor pValor) {
    if (pCarta == NULL){
        printf("\nError al crear la carta...");
    } else {
        pCarta->palo = pPalo;
        pCarta->valor = pValor;
    }
}

int valorCartaJuego(tCarta pCarta){
    if (pCarta.valor == AS){
        return 11;
    } else if (pCarta.valor >= DOS && pCarta.valor <= DIEZ) {
        return pCarta.valor;
    } else if (pCarta.valor == JOTA || pCarta.valor == REINA || pCarta.valor == REY){
        return 10;
    }
    return 0; 
}

bool esAs(tCarta pCarta){
    return pCarta.valor == AS;
}

void obtenerPalo(tPalo pPalo, tString nombrePalo) {
    switch (pPalo) {
        case CORAZONES: strcpy(nombrePalo, "Corazones"); break;
        case DIAMANTES: strcpy(nombrePalo, "Diamantes"); break;
        case PICAS:     strcpy(nombrePalo, "Picas"); break;
        case TREBOLES:  strcpy(nombrePalo, "Treboles"); break;
        default:        strcpy(nombrePalo, "Desconocido"); break;
    }
}

void obtenerValor(tValor pValor, tString nombreValor) {
    switch (pValor) {
        case AS:     strcpy(nombreValor, "AS"); break;
        case DOS:    strcpy(nombreValor, "2"); break;
        case TRES:   strcpy(nombreValor, "3"); break;
        case CUATRO: strcpy(nombreValor, "4"); break;
        case CINCO:  strcpy(nombreValor, "5"); break;
        case SEIS:   strcpy(nombreValor, "6"); break;
        case SIETE:  strcpy(nombreValor, "7"); break;
        case OCHO:   strcpy(nombreValor, "8"); break;
        case NUEVE:  strcpy(nombreValor, "9"); break;
        case DIEZ:   strcpy(nombreValor, "10"); break;
        case JOTA:   strcpy(nombreValor, "J"); break;
        case REINA:  strcpy(nombreValor, "Q"); break;
        case REY:    strcpy(nombreValor, "K"); break;
        default:     strcpy(nombreValor, "Desconocido"); break;
    }
}

void mostrarCarta(tCarta *pCarta) {
    tString nombreValor, nombrePalo;
    
    if (pCarta == NULL) {
        printf("\n\tCarta invalida\n");
    } else {
        obtenerValor(pCarta->valor, nombreValor);
        obtenerPalo(pCarta->palo, nombrePalo);
        printf("\t%s de %s\n", nombreValor, nombrePalo);
    }
}
