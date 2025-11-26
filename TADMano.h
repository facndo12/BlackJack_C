#ifndef TADMANO_H
#define TADMANO_H

#include "TADCarta.h"

typedef struct nodo {
    tCarta carta;
    struct nodo *siguiente;
} tNodo;

typedef struct {
    tNodo *primero;
    int cantidad;
} tMano;

void crearMano(tMano *pMano);
void agregarCartaMano(tMano *pMano, tCarta pCarta);
int calcularValorMano(tMano *pMano);
bool sePasa(tMano *pMano);
bool esBlackjack(tMano *pMano);
int cantidadCartasMano(tMano *pMano);
void vaciarMano(tMano *pMano);
void mostrarMano(tMano *pMano);
void mostrarManoOculta(tMano *pMano);

#endif
