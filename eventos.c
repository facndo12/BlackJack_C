#include "eventos.h"
#include "logica.h"
#include <SDL2/SDL.h>

// ============================================================================
// UTILIDADES
// ============================================================================

bool puntoEnRect(int x, int y, SDL_Rect rect) {
    int x2 = rect.x + rect.w;
    int y2 = rect.y + rect.h;
    return (x >= rect.x && x <= x2 && y >= rect.y && y <= y2);
}

void actualizarSliderVolumen(Juego *juego, int mouseX, int mouseY, bool mousePresionado) {
    tSliderVolumen *slider = &juego->sliderVolumen;
    
    // Early return si no hay nada que hacer
    if (!slider->arrastrando && !mousePresionado) {
        return;
    }
    
    // Actualizar estado de arrastre
    if (mousePresionado) {
        if (!slider->arrastrando && puntoEnRect(mouseX, mouseY, slider->barraBola)) {
            slider->arrastrando = true;
        }
    } else {
        slider->arrastrando = false;
    }
    
    // Solo calcular posición si está arrastrando
    if (slider->arrastrando) {
        int nuevaX = mouseX - (slider->barraBola.w >> 1);
        
        int minX = slider->barraFondo.x;
        int maxX = slider->barraFondo.x + slider->barraFondo.w - slider->barraBola.w;
        
        if (nuevaX < minX) nuevaX = minX;
        else if (nuevaX > maxX) nuevaX = maxX;
        
        if (slider->barraBola.x != nuevaX) {
            slider->barraBola.x = nuevaX;
            slider->volumen = ((nuevaX - minX) * 100) / slider->barraFondo.w;
            Mix_VolumeMusic((slider->volumen * MIX_MAX_VOLUME) / 100);
        }
    }
}

// ============================================================================
// MANEJO DE EVENTOS DEL MENÚ
// ============================================================================

void manejarEventosMenu(Juego *juego, SDL_Event *evento) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    juego->botonJugar.mouseEncima = puntoEnRect(mouseX, mouseY, juego->botonJugar.rect);
    juego->botonSalir.mouseEncima = puntoEnRect(mouseX, mouseY, juego->botonSalir.rect);
    
    if (evento->type == SDL_MOUSEBUTTONDOWN) {
        if (juego->botonJugar.mouseEncima) {
            if (juego->sonidoClick) {
                Mix_PlayChannel(-1, juego->sonidoClick, 0);
            }
            iniciarPartida(juego);
        } else if (juego->botonSalir.mouseEncima) {
            if (juego->sonidoClick) {
                Mix_PlayChannel(-1, juego->sonidoClick, 0);
            }
            juego->estado = ESTADO_SALIR;
        }
    }
    
    bool mousePresionado = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
    actualizarSliderVolumen(juego, mouseX, mouseY, mousePresionado);
}

// ============================================================================
// MANEJO DE EVENTOS DEL JUEGO
// ============================================================================

void manejarEventosJuego(Juego *juego, SDL_Event *evento) {
    static int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    // Early return si es ESC
    if (evento->type == SDL_KEYDOWN && evento->key.keysym.sym == SDLK_ESCAPE) {
        juego->mostrarMensaje = false;
        juego->partidaTerminada = false;
        juego->estado = ESTADO_MENU;
        return;
    }
    
    // Solo procesar clicks
    if (evento->type != SDL_MOUSEBUTTONDOWN) {
        // Solo actualizar hover
        if (!juego->partidaTerminada) {
            juego->botonHit.mouseEncima = puntoEnRect(mouseX, mouseY, juego->botonHit.rect);
            juego->botonStand.mouseEncima = puntoEnRect(mouseX, mouseY, juego->botonStand.rect);
        } else {
            juego->botonNuevaPartida.mouseEncima = puntoEnRect(mouseX, mouseY, juego->botonNuevaPartida.rect);
        }
        return;
    }
    
    // Manejo de clicks
    if (!juego->partidaTerminada) {
        if (juego->botonHit.mouseEncima) {
            manejarHit(juego);
        } else if (juego->botonStand.mouseEncima) {
            manejarStand(juego);
        }
    } else if (juego->botonNuevaPartida.mouseEncima) {
        reiniciarPartida(juego);
    }
}

// ============================================================================
// FUNCIÓN PRINCIPAL DE EVENTOS
// ============================================================================

void manejarEventos(Juego *juego) {
    SDL_Event evento;
    
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_QUIT) {
            juego->ejecutando = false;
        }
        
        switch (juego->estado) {
            case ESTADO_MENU:
                manejarEventosMenu(juego, &evento);
                break;
            case ESTADO_JUGANDO:
                manejarEventosJuego(juego, &evento);
                break;
            default:
                break;
        }
    }
}
