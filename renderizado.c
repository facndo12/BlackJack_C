#include "renderizado.h"
#include "recursos.h"
#include <stdio.h>

// ============================================================================
// FUNCIONES AUXILIARES DE RENDERIZADO
// ============================================================================

void renderizarTexto(SDL_Renderer *renderer, TTF_Font *fuente, const char *texto, 
                     int x, int y, SDL_Color color) {
    if (!texto || !fuente || !renderer) return;
    
    SDL_Surface *superficie = TTF_RenderText_Blended(fuente, texto, color);
    if (!superficie) return;
    
    SDL_Texture *textura = SDL_CreateTextureFromSurface(renderer, superficie);
    
    if (textura) {
        SDL_Rect destRect = {x, y, superficie->w, superficie->h};
        SDL_RenderCopy(renderer, textura, NULL, &destRect);
        SDL_DestroyTexture(textura);
    }
    
    SDL_FreeSurface(superficie);
}

void renderizarBoton(SDL_Renderer *renderer, tBoton *boton) {
    if (!boton || !boton->textura) return;
    
    if (boton->mouseEncima) {
        SDL_SetTextureColorMod(boton->textura, 200, 200, 200);
    } else {
        SDL_SetTextureColorMod(boton->textura, 255, 255, 255);
    }
    
    SDL_RenderCopy(renderer, boton->textura, NULL, &boton->rect);
}

void renderizarBotonSimple(SDL_Renderer *renderer, tBoton *boton, 
                          TTF_Font *fuente, const char *texto,
                          int r, int g, int b, SDL_Color colorTexto) {
    if (!boton) return;
    
    // Color según hover
    int brillo = boton->mouseEncima ? -50 : 0;
    int rFinal = r + brillo;
    int gFinal = g + brillo;
    int bFinal = b + brillo;
    
    // Asegurar que no sean negativos
    if (rFinal < 0) rFinal = 0;
    if (gFinal < 0) gFinal = 0;
    if (bFinal < 0) bFinal = 0;
    
    SDL_SetRenderDrawColor(renderer, rFinal, gFinal, bFinal, 255);
    SDL_RenderFillRect(renderer, &boton->rect);
    
    // Borde
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &boton->rect);
    
    // Texto centrado aproximadamente
    if (texto && fuente) {
        renderizarTexto(renderer, fuente, texto, 
                       boton->rect.x + 25, boton->rect.y + 15, colorTexto);
    }
}

void renderizarSlider(SDL_Renderer *renderer, tSliderVolumen *slider, TTF_Font *fuente) {
    if (!slider) return;
    
    // Barra de fondo
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &slider->barraFondo);
    
    // Barra de progreso
    SDL_Rect progreso = slider->barraFondo;
    progreso.w = (slider->volumen * slider->barraFondo.w) / 100;
    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
    SDL_RenderFillRect(renderer, &progreso);
    
    // Bola deslizante
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &slider->barraBola);
    
    // Borde de la bola
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &slider->barraBola);
    
    // Texto de volumen
    char textoVolumen[20];
    sprintf(textoVolumen, "Volumen: %d%%", slider->volumen);
    renderizarTexto(renderer, fuente, textoVolumen, 
                    slider->barraFondo.x, slider->barraFondo.y - 30, COLOR_BLANCO);
}

// ============================================================================
// RENDERIZADO DEL MENÚ
// ============================================================================

void renderizarMenu(Juego *juego) {
    // Fondo
    if (juego->fondoMenu) {
        SDL_RenderCopy(juego->renderer, juego->fondoMenu, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(juego->renderer, 0, 100, 0, 255);
        SDL_RenderClear(juego->renderer);
    }
    
    // Título
    renderizarTexto(juego->renderer, juego->fuenteTitulo, "BLACKJACK", 
                    ANCHO_VENTANA/2 - 200, 100, COLOR_BLANCO);
    
    // Botones
    renderizarBoton(juego->renderer, &juego->botonJugar);
    renderizarBoton(juego->renderer, &juego->botonSalir);
    
    // Slider de volumen
    renderizarSlider(juego->renderer, &juego->sliderVolumen, juego->fuenteNormal);
}

// ============================================================================
// RENDERIZADO DE CARTAS
// ============================================================================

void renderizarCartasDealer(Juego *juego) {
    renderizarTexto(juego->renderer, juego->fuenteNormal, "DEALER:", 100, 100, COLOR_BLANCO);
    
    tNodo *actual = juego->manoDealer.primero;
    int x = 100;
    int cartaNumero = 0;
    bool mostrarCartas = juego->mostrarCartasDealer;
    
    while (actual != NULL) {
        SDL_Rect rectCarta = {x, 150, 100, 150};
        
        if (cartaNumero == 0 && !mostrarCartas) {
            // Primera carta oculta
            SDL_SetRenderDrawColor(juego->renderer, 50, 50, 150, 255);
            SDL_RenderFillRect(juego->renderer, &rectCarta);
            SDL_SetRenderDrawColor(juego->renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(juego->renderer, &rectCarta);
            
            // Patrón del dorso
            SDL_RenderDrawLine(juego->renderer, x + 10, 160, x + 90, 290);
            SDL_RenderDrawLine(juego->renderer, x + 90, 160, x + 10, 290);
        } else {
            SDL_Texture *texturaCarta = obtenerTexturaCarta(juego->texturaCartas, actual->carta);
            if (texturaCarta) {
                SDL_RenderCopy(juego->renderer, texturaCarta, NULL, &rectCarta);
            }
        }
        
        x += 120;
        cartaNumero++;
        actual = actual->siguiente;
    }
    
    // Valor del dealer
    char buffer[50];
    if (mostrarCartas) {
        sprintf(buffer, "Valor: %d", calcularValorMano(&juego->manoDealer));
    } else {
        strcpy(buffer, "Valor: ?");
    }
    renderizarTexto(juego->renderer, juego->fuenteNormal, buffer, 100, 320, COLOR_BLANCO);
}

void renderizarCartasJugador(Juego *juego) {
    renderizarTexto(juego->renderer, juego->fuenteNormal, "JUGADOR:", 100, 400, COLOR_BLANCO);
    
    tNodo *actual = juego->manoJugador.primero;
    int x = 100;
    
    while (actual != NULL) {
        SDL_Texture *texturaCarta = obtenerTexturaCarta(juego->texturaCartas, actual->carta);
        if (texturaCarta) {
            SDL_Rect rectCarta = {x, 450, 100, 150};
            SDL_RenderCopy(juego->renderer, texturaCarta, NULL, &rectCarta);
        }
        x += 120;
        actual = actual->siguiente;
    }
    
    // Valor del jugador
    char buffer[50];
    sprintf(buffer, "Valor: %d", calcularValorMano(&juego->manoJugador));
    renderizarTexto(juego->renderer, juego->fuenteNormal, buffer, 100, 620, COLOR_BLANCO);
}

// ============================================================================
// RENDERIZADO DE BOTONES DEL JUEGO
// ============================================================================

void renderizarBotonesPrincipales(Juego *juego) {
    if (!juego->partidaTerminada) {
        renderizarBotonSimple(juego->renderer, &juego->botonHit, 
                             juego->fuenteNormal, "HIT", 
                             200, 0, 0, COLOR_BLANCO);
        
        renderizarBotonSimple(juego->renderer, &juego->botonStand, 
                             juego->fuenteNormal, "STAND", 
                             0, 150, 0, COLOR_BLANCO);
    }
}

void renderizarMensajeResultado(Juego *juego) {
    if (!juego->mostrarMensaje || !juego->partidaTerminada) return;
    
    // Fondo semi-transparente
    SDL_SetRenderDrawBlendMode(juego->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(juego->renderer, 0, 0, 0, 180);
    SDL_Rect fondoMensaje = {ANCHO_VENTANA/2 - 300, ALTO_VENTANA/2 - 100, 600, 200};
    SDL_RenderFillRect(juego->renderer, &fondoMensaje);
    
    // Mensaje
    renderizarTexto(juego->renderer, juego->fuenteTitulo, 
                   juego->mensajeResultado, 
                   ANCHO_VENTANA/2 - 250, ALTO_VENTANA/2 - 50, COLOR_AMARILLO);
    
    renderizarTexto(juego->renderer, juego->fuenteNormal, 
                   "Presiona ESC para volver al menu", 
                   ANCHO_VENTANA/2 - 250, ALTO_VENTANA/2 + 50, COLOR_BLANCO);
    
    // Botón nueva partida
    renderizarBotonSimple(juego->renderer, &juego->botonNuevaPartida, 
                         juego->fuenteNormal, "NUEVA PARTIDA", 
                         0, 100, 200, COLOR_BLANCO);
}

// ============================================================================
// RENDERIZADO PRINCIPAL DEL JUEGO
// ============================================================================

void renderizarJuego(Juego *juego) {
    // Fondo
    if (juego->fondoJuego) {
        SDL_RenderCopy(juego->renderer, juego->fondoJuego, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(juego->renderer, 0, 128, 0, 255);
        SDL_RenderClear(juego->renderer);
    }
    
    // Título
    renderizarTexto(juego->renderer, juego->fuenteNormal, 
                    "BLACKJACK - Presiona ESC para volver", 50, 20, COLOR_BLANCO);
    
    // Renderizar elementos del juego
    renderizarCartasDealer(juego);
    renderizarCartasJugador(juego);
    renderizarBotonesPrincipales(juego);
    renderizarMensajeResultado(juego);
}

// ============================================================================
// FUNCIÓN PRINCIPAL DE RENDERIZADO
// ============================================================================

void renderizar(Juego *juego) {
    SDL_RenderClear(juego->renderer);
    
    switch (juego->estado) {
        case ESTADO_MENU:
            renderizarMenu(juego);
            break;
        case ESTADO_JUGANDO:
            renderizarJuego(juego);
            break;
        default:
            break;
    }
    
    SDL_RenderPresent(juego->renderer);
}
