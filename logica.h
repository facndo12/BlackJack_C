#ifndef LOGICA_H
#define LOGICA_H

#include "tipos.h"
#include "config.h"

// Inicialización del juego
void inicializarJuego(Juego *juego);
void inicializarBotones(Juego *juego);
void inicializarSliderVolumen(Juego *juego);

// Lógica de partida
void iniciarPartida(Juego *juego);
void reiniciarPartida(Juego *juego);
void manejarHit(Juego *juego);
void manejarStand(Juego *juego);
void determinarGanador(Juego *juego);

#endif
