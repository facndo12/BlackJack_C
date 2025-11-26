#ifndef RECURSOS_H
#define RECURSOS_H

#include "tipos.h"
#include "config.h"

// Inicialización de SDL
bool inicializarSDL(Juego *juego);

// Carga de recursos
bool cargarRecursos(Juego *juego);
SDL_Texture* cargarTextura(SDL_Renderer *renderer, const char *ruta);
bool cargarTodasLasCartas(SDL_Renderer *renderer, SDL_Texture *texturaCartas[52]);

// Utilidades de cartas
int obtenerIndiceCarta(tPalo palo, tValor valor);
SDL_Texture* obtenerTexturaCarta(SDL_Texture *texturaCartas[52], tCarta carta);

// Limpieza
void liberarRecursos(Juego *juego);

#endif
