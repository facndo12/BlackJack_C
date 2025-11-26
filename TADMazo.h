#ifndef TADMAZO_H
#define TADMAZO_H

#include "TADCarta.h"

#define MAXCARTAS 104

typedef struct {
    tCarta cartas[MAXCARTAS];
    int tope;
} tMazo;

void crearMazo(tMazo *pMazo);
void llenarMazo(tMazo *pMazo);
void mezclarMazo(tMazo *pMazo);
void mezclarMazoProfundo(tMazo *pMazo, int numMezclas);
bool mazoVacio(tMazo pMazo);
bool mazoLleno(tMazo pMazo);
int cantidadCartas(tMazo pMazo);
void reiniciarMazo(tMazo *pMazo);
void llenarMazosMultiples(tMazo *pMazo, int numMazos);

void apilarCarta(tMazo *pMazo, tCarta carta);
void desapilarCarta(tMazo *pMazo, tCarta *pCarta);
void repartirCarta(tMazo *pMazo, tCarta *pCarta);

#endif
