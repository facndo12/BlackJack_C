#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
#include "TADCarta.h"
#include "TADMazo.h"
#include "TADMano.h"
#include "TADJugador.h"

// CONSTANTES Y ESTRUCTURAS

#define ANCHO_VENTANA 1280
#define ALTO_VENTANA 720
#define FPS 60

// Estados del juego
typedef enum {
    ESTADO_MENU,
    ESTADO_JUGANDO,
    ESTADO_SALIR
} EstadoJuego;

// Estructura para botones
typedef struct {
    SDL_Rect rect;
    SDL_Texture *textura;
    SDL_Texture *texturaHover;
    bool mouseEncima;
} tBoton;

// Estructura para el slider de volumen
typedef struct {
    SDL_Rect barraFondo;
    SDL_Rect barraBola;
    int volumen; 
    bool arrastrando;
} tSliderVolumen;

// Estructura principal del juego
typedef struct {
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
    Boton botonJugar;
    Boton botonSalir;
    SliderVolumen sliderVolumen;
    
    // Estado
    EstadoJuego estado;
    bool ejecutando;
    
    // Datos del juego
    tMazo mazo;
    tMano manoJugador;
    tMano manoDealer;
    tArbolJugadores arbolJugadores;
    
} Juego;


// Inicializa SDL y crea la ventana
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

// Carga una textura desde archivo
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

// Carga todos los recursos del juego
bool cargarRecursos(Juego *juego) {
    // MODIFICAR ESTAS RUTAS según tu estructura de carpetas
    
    // Cargar fuentes
    juego->fuenteTitulo = TTF_OpenFont("assets/fuentes/VCR_OSD_MONO_1.001.ttf", 72);
    juego->fuenteNormal = TTF_OpenFont("assets/fuentes/VCR_OSD_MONO_1.001.ttf", 32);
    
    if (!juego->fuenteTitulo || !juego->fuenteNormal) {
        printf("Error al cargar fuentes: %s\n", TTF_GetError());
        return false;
    }
    
    // Cargar imágenes de fondo
    juego->fondoMenu = cargarTextura(juego->renderer, "assets/imagenes/brushwalker437.png");
    juego->fondoJuego = cargarTextura(juego->renderer, "assets/imagenes/green_mat_texture_-_base_texture.png");
    
    // Cargar botones
    juego->botonJugar.textura = cargarTextura(juego->renderer, "assets/imagenes/boton_jugar.png");
    juego->botonSalir.textura = cargarTextura(juego->renderer, "assets/imagenes/boton_salir.png");
    
    // OPCIONAL: Cargar texturas hover (al pasar el mouse)
    // juego->botonJugar.texturaHover = cargarTextura(juego->renderer, "assets/imagenes/boton_jugar_hover.png");
    // juego->botonSalir.texturaHover = cargarTextura(juego->renderer, "assets/imagenes/boton_salir_hover.png");
    
    // Cargar cartas (ejemplo para algunas cartas)
    // MODIFICAR: Debes cargar las 52 cartas con sus nombres correctos
    juego->texturaCartas[0] = cargarTextura(juego->renderer, "assets/imagenes/cartas/as_corazones.png");
    juego->texturaCartas[1] = cargarTextura(juego->renderer, "assets/imagenes/cartas/dos_corazones.png");
    // ... cargar el resto de las cartas ...
    
    // Cargar música
    juego->musicaFondo = Mix_LoadMUS("assets/audio/musica_fondo.mp3");
    if (!juego->musicaFondo) {
        printf("Error al cargar música: %s\n", Mix_GetError());
    }
    
    // Cargar efectos de sonido
    juego->sonidoClick = Mix_LoadWAV("assets/audio/click.wav");
    juego->sonidoRepartir = Mix_LoadWAV("assets/audio/repartir.wav");
    juego->sonidoVictoria = Mix_LoadWAV("assets/audio/victoria.wav");
    
    return true;
}

// ============================================================================
// PASO 3: FUNCIONES DE INICIALIZACIÓN DEL JUEGO
// ============================================================================

void inicializarBotones(Juego *juego) {
    // Botón Jugar (centrado arriba)
    juego->botonJugar.rect.w = 300;
    juego->botonJugar.rect.h = 100;
    juego->botonJugar.rect.x = (ANCHO_VENTANA - juego->botonJugar.rect.w) / 2;
    juego->botonJugar.rect.y = 300;
    juego->botonJugar.mouseEncima = false;
    
    // Botón Salir (centrado abajo del anterior)
    juego->botonSalir.rect.w = 300;
    juego->botonSalir.rect.h = 100;
    juego->botonSalir.rect.x = (ANCHO_VENTANA - juego->botonSalir.rect.w) / 2;
    juego->botonSalir.rect.y = 450;
    juego->botonSalir.mouseEncima = false;
}

void inicializarSliderVolumen(Juego *juego) {
    // Barra de fondo del slider
    juego->sliderVolumen.barraFondo.x = ANCHO_VENTANA - 350;
    juego->sliderVolumen.barraFondo.y = 50;
    juego->sliderVolumen.barraFondo.w = 300;
    juego->sliderVolumen.barraFondo.h = 10;
    
    // Bola deslizante
    juego->sliderVolumen.barraBola.w = 20;
    juego->sliderVolumen.barraBola.h = 20;
    juego->sliderVolumen.volumen = 50; // Volumen inicial al 50%
    juego->sliderVolumen.barraBola.x = juego->sliderVolumen.barraFondo.x + 
                                        (juego->sliderVolumen.volumen * juego->sliderVolumen.barraFondo.w / 100);
    juego->sliderVolumen.barraBola.y = juego->sliderVolumen.barraFondo.y - 5;
    juego->sliderVolumen.arrastrando = false;
    
    // Establecer volumen inicial
    Mix_VolumeMusic(juego->sliderVolumen.volumen * MIX_MAX_VOLUME / 100);
}

void inicializarJuego(Juego *juego) {
    juego->estado = ESTADO_MENU;
    juego->ejecutando = true;
    
    // Inicializar TADs
    inicializarArbol(&juego->arbolJugadores);
    cargarJugadoresArchivo(&juego->arbolJugadores);
    
    crearMazo(&juego->mazo);
    crearMano(&juego->manoJugador);
    crearMano(&juego->manoDealer);
    
    // Inicializar UI
    inicializarBotones(juego);
    inicializarSliderVolumen(juego);
    
    // Reproducir música de fondo
    if (juego->musicaFondo) {
        Mix_PlayMusic(juego->musicaFondo, -1); // -1 = loop infinito
    }
}

// ============================================================================
// PASO 4: FUNCIONES DE ACTUALIZACIÓN Y LÓGICA
// ============================================================================

bool puntoEnRect(int x, int y, SDL_Rect rect) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

void actualizarSliderVolumen(Juego *juego, int mouseX, int mouseY, bool mousePresionado) {
    SliderVolumen *slider = &juego->sliderVolumen;
    
    if (mousePresionado && puntoEnRect(mouseX, mouseY, slider->barraBola)) {
        slider->arrastrando = true;
    }
    
    if (!mousePresionado) {
        slider->arrastrando = false;
    }
    
    if (slider->arrastrando) {
        // Calcular nueva posición
        int nuevaX = mouseX - slider->barraBola.w / 2;
        
        // Limitar dentro de la barra
        if (nuevaX < slider->barraFondo.x) {
            nuevaX = slider->barraFondo.x;
        } else if (nuevaX > slider->barraFondo.x + slider->barraFondo.w - slider->barraBola.w) {
            nuevaX = slider->barraFondo.x + slider->barraFondo.w - slider->barraBola.w;
        }
        
        slider->barraBola.x = nuevaX;
        
        // Calcular volumen (0-100)
        slider->volumen = ((nuevaX - slider->barraFondo.x) * 100) / slider->barraFondo.w;
        
        // Actualizar volumen de SDL_mixer
        Mix_VolumeMusic(slider->volumen * MIX_MAX_VOLUME / 100);
    }
}

void manejarEventosMenu(Juego *juego, SDL_Event *evento) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    // Actualizar estado hover de botones
    juego->botonJugar.mouseEncima = puntoEnRect(mouseX, mouseY, juego->botonJugar.rect);
    juego->botonSalir.mouseEncima = puntoEnRect(mouseX, mouseY, juego->botonSalir.rect);
    
    if (evento->type == SDL_MOUSEBUTTONDOWN) {
        if (juego->botonJugar.mouseEncima) {
            // Reproducir sonido de click
            if (juego->sonidoClick) {
                Mix_PlayChannel(-1, juego->sonidoClick, 0);
            }
            
            // Iniciar partida
            llenarMazo(&juego->mazo);
            mezclarMazo(&juego->mazo);
            vaciarMano(&juego->manoJugador);
            vaciarMano(&juego->manoDealer);
            
            juego->estado = ESTADO_JUGANDO;
        } else if (juego->botonSalir.mouseEncima) {
            if (juego->sonidoClick) {
                Mix_PlayChannel(-1, juego->sonidoClick, 0);
            }
            juego->estado = ESTADO_SALIR;
        }
    }
    
    // Actualizar slider de volumen
    bool mousePresionado = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
    actualizarSliderVolumen(juego, mouseX, mouseY, mousePresionado);
}

void manejarEventosJuego(Juego *juego, SDL_Event *evento) {
    // Aquí irá la lógica del juego de blackjack
    // Por ahora, presionar ESC vuelve al menú
    if (evento->type == SDL_KEYDOWN && evento->key.keysym.sym == SDLK_ESCAPE) {
        juego->estado = ESTADO_MENU;
    }
}

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

// ============================================================================
// PASO 5: FUNCIONES DE RENDERIZADO
// ============================================================================

void renderizarTexto(SDL_Renderer *renderer, TTF_Font *fuente, const char *texto, 
                     int x, int y, SDL_Color color) {
    SDL_Surface *superficie = TTF_RenderText_Blended(fuente, texto, color);
    if (!superficie) return;
    
    SDL_Texture *textura = SDL_CreateTextureFromSurface(renderer, superficie);
    SDL_Rect destRect = {x, y, superficie->w, superficie->h};
    
    SDL_RenderCopy(renderer, textura, NULL, &destRect);
    
    SDL_FreeSurface(superficie);
    SDL_DestroyTexture(textura);
}

void renderizarBoton(SDL_Renderer *renderer, Boton *boton) {
    // Cambiar color si el mouse está encima
    if (boton->mouseEncima) {
        SDL_SetTextureColorMod(boton->textura, 200, 200, 200); // Más oscuro
    } else {
        SDL_SetTextureColorMod(boton->textura, 255, 255, 255); // Normal
    }
    
    SDL_RenderCopy(renderer, boton->textura, NULL, &boton->rect);
}

void renderizarSlider(SDL_Renderer *renderer, SliderVolumen *slider, TTF_Font *fuente) {
    // Dibujar barra de fondo (gris oscuro)
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &slider->barraFondo);
    
    // Dibujar barra de progreso (verde)
    SDL_Rect progreso = slider->barraFondo;
    progreso.w = (slider->volumen * slider->barraFondo.w) / 100;
    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
    SDL_RenderFillRect(renderer, &progreso);
    
    // Dibujar bola deslizante (blanco)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &slider->barraBola);
    
    // Dibujar borde de la bola (negro)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &slider->barraBola);
    
    // Texto de volumen
    char textoVolumen[20];
    sprintf(textoVolumen, "Volumen: %d%%", slider->volumen);
    SDL_Color colorBlanco = {255, 255, 255, 255};
    renderizarTexto(renderer, fuente, textoVolumen, 
                    slider->barraFondo.x, slider->barraFondo.y - 30, colorBlanco);
}

void renderizarMenu(Juego *juego) {
    // Fondo
    if (juego->fondoMenu) {
        SDL_RenderCopy(juego->renderer, juego->fondoMenu, NULL, NULL);
    } else {
        // Color de fondo por defecto si no hay imagen
        SDL_SetRenderDrawColor(juego->renderer, 0, 100, 0, 255); // Verde oscuro
        SDL_RenderClear(juego->renderer);
    }
    
    // Título
    SDL_Color colorBlanco = {255, 255, 255, 255};
    renderizarTexto(juego->renderer, juego->fuenteTitulo, "BLACKJACK", 
                    ANCHO_VENTANA/2 - 200, 100, colorBlanco);
    
    // Botones
    renderizarBoton(juego->renderer, &juego->botonJugar);
    renderizarBoton(juego->renderer, &juego->botonSalir);
    
    // Slider de volumen
    renderizarSlider(juego->renderer, &juego->sliderVolumen, juego->fuenteNormal);
}

void renderizarJuego(Juego *juego) {
    // Fondo
    if (juego->fondoJuego) {
        SDL_RenderCopy(juego->renderer, juego->fondoJuego, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(juego->renderer, 0, 128, 0, 255); // Verde
        SDL_RenderClear(juego->renderer);
    }
    
    // Aquí renderizarás las cartas, botones de Hit/Stand, etc.
    SDL_Color colorBlanco = {255, 255, 255, 255};
    renderizarTexto(juego->renderer, juego->fuenteNormal, 
                    "Presiona ESC para volver al menu", 50, 50, colorBlanco);
    
    // Ejemplo: renderizar una carta
    if (juego->texturaCartas[0]) {
        SDL_Rect rectCarta = {100, 200, 100, 150};
        SDL_RenderCopy(juego->renderer, juego->texturaCartas[0], NULL, &rectCarta);
    }
}

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

// ============================================================================
// PASO 6: FUNCIONES DE LIMPIEZA
// ============================================================================

void liberarRecursos(Juego *juego) {
    // Liberar texturas
    if (juego->fondoMenu) SDL_DestroyTexture(juego->fondoMenu);
    if (juego->fondoJuego) SDL_DestroyTexture(juego->fondoJuego);
    if (juego->botonJugar.textura) SDL_DestroyTexture(juego->botonJugar.textura);
    if (juego->botonSalir.textura) SDL_DestroyTexture(juego->botonSalir.textura);
    
    // Liberar cartas
    int i;
    for (i = 0; i < 52; i++) {
        if (juego->texturaCartas[i]) {
            SDL_DestroyTexture(juego->texturaCartas[i]);
        }
    }
    
    // Liberar fuentes
    if (juego->fuenteTitulo) TTF_CloseFont(juego->fuenteTitulo);
    if (juego->fuenteNormal) TTF_CloseFont(juego->fuenteNormal);
    
    // Liberar audio
    if (juego->musicaFondo) Mix_FreeMusic(juego->musicaFondo);
    if (juego->sonidoClick) Mix_FreeChunk(juego->sonidoClick);
    if (juego->sonidoRepartir) Mix_FreeChunk(juego->sonidoRepartir);
    if (juego->sonidoVictoria) Mix_FreeChunk(juego->sonidoVictoria);
    
    // Liberar TADs
    vaciarMano(&juego->manoJugador);
    vaciarMano(&juego->manoDealer);
    guardarJugadoresArchivo(juego->arbolJugadores);
    liberarArbol(&juego->arbolJugadores);
    
    // Liberar SDL
    if (juego->renderer) SDL_DestroyRenderer(juego->renderer);
    if (juego->ventana) SDL_DestroyWindow(juego->ventana);
    
    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

// ============================================================================
// PASO 7: FUNCIÓN PRINCIPAL
// ============================================================================

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
        Uint32 deltaTiempo = tiempoActual - tiempoAnterior;
        
        // Manejar eventos
        manejarEventos(&juego);
        
        // Renderizar
        renderizar(&juego);
        
        // Control de FPS
        if (deltaTiempo < TIEMPO_POR_FRAME) {
            SDL_Delay(TIEMPO_POR_FRAME - deltaTiempo);
        }
        
        tiempoAnterior = tiempoActual;
    }
    
    // Limpiar y salir
    liberarRecursos(&juego);
    
    printf("\n¡Gracias por jugar!\n");
    
    return 0;
}
