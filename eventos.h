#ifndef EVENTOS_H
#define EVENTOS_H

#include "tipos.h"
#include "config.h"

// Funciones principales de eventos
void manejarEventos(Juego *juego);
void manejarEventosMenu(Juego *juego, SDL_Event *evento);
void manejarEventosJuego(Juego *juego, SDL_Event *evento);

// Utilidades
bool puntoEnRect(int x, int y, SDL_Rect rect);
void actualizarSliderVolumen(Juego *juego, int mouseX, int mouseY, bool mousePresionado);

#endif
