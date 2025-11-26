#ifndef TADJUGADOR_H
#define TADJUGADOR_H

#include <stdio.h>
#include "TADCarta.h"
#include "TADMano.h"
#include "TADMazo.h"

#define MAXNOMBRE 30
#define ARCHIVOJUGADORES "jugadores.dat"

typedef struct {
    char nombre[MAXNOMBRE];
    int fichas;
    int partidasJugadas;
    int partidasGanadas;
    int partidasPerdidas;
    int partidasEmpatadas;
} tJugador;

typedef struct nodoJugador {
    tJugador jugador;
    struct nodoJugador *izq;
    struct nodoJugador *der;
} tNodoJugador;

typedef tNodoJugador * tArbolJugadores;

// Prototipos
void inicializarArbol(tArbolJugadores *pArbol);
void insertarJugador(tArbolJugadores *pArbol, tJugador pJugador);
tNodoJugador* buscarJugador(tArbolJugadores pArbol, char *pNombre);
void eliminarJugador(tArbolJugadores *pArbol, char *pNombre);
void crearJugador(tJugador *pJugador, char *pNombre, int pFichas);

void actualizarEstadisticas(tJugador *pJugador, int pResultado);
void modificarFichas(tJugador *pJugador, int pCantidad);
void mostrarJugador(tJugador pJugador);
void mostrarArbolInorden(tArbolJugadores pArbol);
void mostrarArbolPorNivel(tArbolJugadores pArbol);
void liberarArbol(tArbolJugadores *pArbol);

void guardarJugadoresArchivo(tArbolJugadores pArbol);
void cargarJugadoresArchivo(tArbolJugadores *pArbol);
void generarReporteEstadisticas(tArbolJugadores pArbol);

#endif
