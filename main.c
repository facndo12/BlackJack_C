#include "config.h"
#include "tipos.h"
#include "recursos.h"
#include "renderizado.h"
#include "eventos.h"
#include "logica.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    Juego juego = {0};
    
    // Inicializar SDL
    if (!inicializarSDL(&juego)) {
        return 1;
    }
    
    // Cargar recursos
    if (!cargarRecursos(&juego)) {
        printf("Error al cargar recursos\n");
        liberarRecursos(&juego);
        return 1;
    }
    
    // Inicializar juego
    inicializarJuego(&juego);
    
    // Loop principal
    Uint32 tiempoAnterior = SDL_GetTicks();
    const Uint32 TIEMPO_POR_FRAME = 1000 / FPS;
    
    while (juego.ejecutando && juego.estado != ESTADO_SALIR) {
        Uint32 tiempoActual = SDL_GetTicks();
        
        // 1. Manejar inputs
        manejarEventos(&juego);
        
        // 2. Renderizar
        renderizar(&juego);
        
        // 3. Control de FPS (Frame Capping)
        if (tiempoActual - tiempoAnterior < TIEMPO_POR_FRAME) {
            SDL_Delay(TIEMPO_POR_FRAME - (tiempoActual - tiempoAnterior));
        }
        tiempoAnterior = tiempoActual;
    }

    // Limpieza al salir
    liberarRecursos(&juego);
    return 0;
	}
