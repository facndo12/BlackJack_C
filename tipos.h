#ifndef TIPOS_H
#define TIPOS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include "TADCarta.h"
#include "TADMazo.h"
#include "TADMano.h"
#include "TADJugador.h"

// Estados del juego
typedef enum {
    ESTADO_MENU,
    ESTADO_JUGANDO,
    ESTADO_SALIR
} EstadoJuego;

// Botón genérico
typedef struct {
    SDL_Rect rect;
    SDL_Texture *textura;
    SDL_Texture *texturaHover;
    bool mouseEncima;
} tBoton;

// Slider de volumen
typedef struct {
    SDL_Rect barraFondo;
    SDL_Rect barraBola;
    int volumen;
    bool arrastrando;
} tSliderVolumen;

// Estructura principal del juego
typedef struct {
    // SDL
    SDL_Window *ventana;
    SDL_Renderer *renderer;
    
    // Fuentes
    TTF_Font *fuenteTitulo;
    TTF_Font *fuenteNormal;
    
    // Texturas
    SDL_Texture *fondoMenu;
    SDL_Texture *fondoJuego;
    SDL_Texture *texturaCartas[52];
    
    // Audio
    Mix_Music *musicaFondo;
    Mix_Chunk *sonidoClick;
    Mix_Chunk *sonidoRepartir;
    Mix_Chunk *sonidoVictoria;
    
    // UI
    tBoton botonJugar;
    tBoton botonSalir;
    tBoton botonHit;
    tBoton botonStand;
    tBoton botonNuevaPartida;
    tSliderVolumen sliderVolumen;
    
    // Estado del juego
    EstadoJuego estado;
    bool ejecutando;
    bool mostrarMensaje;
    bool mostrarCartasDealer;
    bool partidaTerminada;
    char mensajeResultado[100];
    
    // Datos del juego
    tMazo mazo;
    tMano manoJugador;
    tMano manoDealer;
    tArbolJugadores arbolJugadores;
    
} Juego;

#endif
