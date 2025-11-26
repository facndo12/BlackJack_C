#ifndef RENDERIZADO_H
#define RENDERIZADO_H

#include "tipos.h"
#include "config.h"

// Funciones principales de renderizado
void renderizar(Juego *juego);
void renderizarMenu(Juego *juego);
void renderizarJuego(Juego *juego);

// Funciones auxiliares de renderizado
void renderizarTexto(SDL_Renderer *renderer, TTF_Font *fuente, const char *texto, 
                     int x, int y, SDL_Color color);
void renderizarBoton(SDL_Renderer *renderer, tBoton *boton);
void renderizarBotonSimple(SDL_Renderer *renderer, tBoton *boton, 
                          TTF_Font *fuente, const char *texto,
                          int r, int g, int b, SDL_Color colorTexto);
void renderizarSlider(SDL_Renderer *renderer, tSliderVolumen *slider, TTF_Font *fuente);

// Renderizado específico del juego
void renderizarCartasDealer(Juego *juego);
void renderizarCartasJugador(Juego *juego);
void renderizarBotonesPrincipales(Juego *juego);
void renderizarMensajeResultado(Juego *juego);

#endif
