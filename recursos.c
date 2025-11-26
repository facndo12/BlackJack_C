#include "recursos.h"
#include <stdio.h>

bool inicializarSDL(Juego *juego) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Error al inicializar SDL: %s\n", SDL_GetError());
        return false;
    }
    
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Error al inicializar SDL_image: %s\n", IMG_GetError());
        return false;
    }
    
    if (TTF_Init() == -1) {
        printf("Error al inicializar SDL_ttf: %s\n", TTF_GetError());
        return false;
    }
    
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Error al inicializar SDL_mixer: %s\n", Mix_GetError());
        return false;
    }
    
    juego->ventana = SDL_CreateWindow(
        "BLACKJACK",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        ANCHO_VENTANA,
        ALTO_VENTANA,
        SDL_WINDOW_SHOWN
    );
    
    if (!juego->ventana) {
        printf("Error al crear ventana: %s\n", SDL_GetError());
        return false;
    }
    
    juego->renderer = SDL_CreateRenderer(
        juego->ventana,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    
    if (!juego->renderer) {
        printf("Error al crear renderer: %s\n", SDL_GetError());
        return false;
    }
    
    return true;
}

SDL_Texture* cargarTextura(SDL_Renderer *renderer, const char *ruta) {
    SDL_Surface *superficie = IMG_Load(ruta);
    if (!superficie) {
        printf("Error al cargar imagen %s: %s\n", ruta, IMG_GetError());
        return NULL;
    }
    
    SDL_Texture *textura = SDL_CreateTextureFromSurface(renderer, superficie);
    SDL_FreeSurface(superficie);
    
    if (!textura) {
        printf("Error al crear textura: %s\n", SDL_GetError());
        return NULL;
    }
    
    return textura;
}

int obtenerIndiceCarta(tPalo palo, tValor valor) {
    return (palo * 13) + (valor - 1);
}

SDL_Texture* obtenerTexturaCarta(SDL_Texture *texturaCartas[52], tCarta carta) {
    int indice = obtenerIndiceCarta(carta.palo, carta.valor);
    
    if (indice < 0 || indice >= 52) {
        printf("ERROR: Indice de carta fuera de rango: %d\n", indice);
        return NULL;
    }
    
    return texturaCartas[indice];
}

bool cargarTodasLasCartas(SDL_Renderer *renderer, SDL_Texture *texturaCartas[52]) {
    static const char *nombresPalos[] = {"corazones", "diamantes", "picas", "trebol"};
    char rutaCarta[256];
    int indice = 0;
    int palo, valor;
    int cartasFallidas = 0;
    
    printf("\n=== CARGANDO CARTAS ===\n");
    
    for (palo = 0; palo < 4; palo++) {
        for (valor = 1; valor <= 13; valor++) {
            sprintf(rutaCarta, "assets/cartas/%s/%d.png", nombresPalos[palo], valor);
            texturaCartas[indice] = cargarTextura(renderer, rutaCarta);
            
            if (!texturaCartas[indice]) {
                cartasFallidas++;
            }
            
            indice++;
        }
    }
    
    if (cartasFallidas > 0) {
        printf("ADVERTENCIA: %d cartas no se cargaron\n", cartasFallidas);
        return false;
    }
    
    printf("? 52 cartas cargadas exitosamente\n");
    return true;
}

bool cargarRecursos(Juego *juego) {
    // Cargar fuentes
    juego->fuenteTitulo = TTF_OpenFont(RUTA_FUENTE, 72);
    juego->fuenteNormal = TTF_OpenFont(RUTA_FUENTE, 32);
    
    if (!juego->fuenteTitulo || !juego->fuenteNormal) {
        printf("Error al cargar fuentes: %s\n", TTF_GetError());
        return false;
    }
    
    // Cargar fondos
    juego->fondoMenu = cargarTextura(juego->renderer, RUTA_FONDO_MENU);
    juego->fondoJuego = cargarTextura(juego->renderer, RUTA_FONDO_JUEGO);
    
    // Cargar botones
    juego->botonJugar.textura = cargarTextura(juego->renderer, RUTA_BOTON_JUGAR);
    juego->botonSalir.textura = cargarTextura(juego->renderer, RUTA_BOTON_SALIR);
    juego->botonHit.textura = cargarTextura(juego->renderer, RUTA_BOTON_HIT);
    juego->botonStand.textura = cargarTextura(juego->renderer, RUTA_BOTON_STAND);
    juego->botonNuevaPartida.textura = cargarTextura(juego->renderer, RUTA_BOTON_NUEVA);
    
    // Cargar cartas
    if (!cargarTodasLasCartas(juego->renderer, juego->texturaCartas)) {
        return false;
    }
    
    return true;
}

void liberarRecursos(Juego *juego) {
    if (juego->fondoMenu) SDL_DestroyTexture(juego->fondoMenu);
    if (juego->fondoJuego) SDL_DestroyTexture(juego->fondoJuego);
    if (juego->botonJugar.textura) SDL_DestroyTexture(juego->botonJugar.textura);
    if (juego->botonSalir.textura) SDL_DestroyTexture(juego->botonSalir.textura);
    if (juego->botonHit.textura) SDL_DestroyTexture(juego->botonHit.textura);
    if (juego->botonStand.textura) SDL_DestroyTexture(juego->botonStand.textura);
    if (juego->botonNuevaPartida.textura) SDL_DestroyTexture(juego->botonNuevaPartida.textura);
    
    int i;
    for (i = 0; i < 52; i++) {
        if (juego->texturaCartas[i]) {
            SDL_DestroyTexture(juego->texturaCartas[i]);
        }
    }
    
    if (juego->fuenteTitulo) TTF_CloseFont(juego->fuenteTitulo);
    if (juego->fuenteNormal) TTF_CloseFont(juego->fuenteNormal);
    
    if (juego->musicaFondo) Mix_FreeMusic(juego->musicaFondo);
    if (juego->sonidoClick) Mix_FreeChunk(juego->sonidoClick);
    if (juego->sonidoRepartir) Mix_FreeChunk(juego->sonidoRepartir);
    if (juego->sonidoVictoria) Mix_FreeChunk(juego->sonidoVictoria);
    
    vaciarMano(&juego->manoJugador);
    vaciarMano(&juego->manoDealer);
    guardarJugadoresArchivo(juego->arbolJugadores);
    liberarArbol(&juego->arbolJugadores);
    
    if (juego->renderer) SDL_DestroyRenderer(juego->renderer);
    if (juego->ventana) SDL_DestroyWindow(juego->ventana);
    
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
