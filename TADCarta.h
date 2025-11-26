#ifndef TADCARTA_H
#define TADCARTA_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAXCAR 15
typedef char tString[MAXCAR];

typedef enum {CORAZONES, DIAMANTES, PICAS, TREBOLES} tPalo;

typedef enum {
    AS = 1,
    DOS = 2,
    TRES = 3,
    CUATRO = 4,
    CINCO = 5,
    SEIS = 6,
    SIETE = 7,
    OCHO = 8,
    NUEVE = 9,
    DIEZ = 10,
    JOTA = 11,
    REINA = 12,
    REY = 13
} tValor;

typedef struct {
    tPalo palo;
    tValor valor;
} tCarta;

// Prototipos
void crearCarta(tCarta *pCarta, tPalo pPalo, tValor pValor);
int valorCartaJuego(tCarta pCarta);
bool esAs(tCarta pCarta);
void mostrarCarta(tCarta *pCarta);
void obtenerPalo(tPalo pPalo, tString nombrePalo);
void obtenerValor(tValor pValor, tString nombreValor);

#endif
