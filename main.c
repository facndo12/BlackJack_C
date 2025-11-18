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
    tBoton botonJugar;
    tBoton botonSalir;
    tSliderVolumen sliderVolumen;
    tBoton botonHit;
	tBoton botonStand;
    
    // Estado
    EstadoJuego estado;
    bool ejecutando;
    
    // Datos del juego
    tMazo mazo;
    tMano manoJugador;
    tMano manoDealer;
    tArbolJugadores arbolJugadores;
    bool mostrarMensaje;
    char mensajeResultado[100];
    bool mostrarCartasDealer;
    bool partidaTerminada; 
    tBoton botonNuevaPartida;
    
} Juego;

// PROTOTIPOS DE FUNCIONES
int obtenerIndiceCarta(tPalo palo, tValor valor);
SDL_Texture* obtenerTexturaCarta(SDL_Texture *texturaCartas[52], tCarta carta);
bool cargarTodasLasCartas_Metodo4(SDL_Renderer *renderer, SDL_Texture *texturaCartas[52]);
void renderizarBoton(SDL_Renderer *renderer, tBoton *boton);
void renderizarSlider(SDL_Renderer *renderer, tSliderVolumen *slider, TTF_Font *fuente);

// FUNCIONES DE INICIALIZACIÓN

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

// Funciones para cargar cartas
int obtenerIndiceCarta(tPalo palo, tValor valor) {
    return (palo * 13) + (valor - 1);
}

SDL_Texture* obtenerTexturaCarta(SDL_Texture *texturaCartas[52], tCarta carta) {
    int indice = obtenerIndiceCarta(carta.palo, carta.valor);
    return texturaCartas[indice];
}

bool cargarTodasLasCartas_Metodo4(SDL_Renderer *renderer, SDL_Texture *texturaCartas[52]) {
    const char *nombresPalos[] = {"corazones", "diamantes", "picas", "trebol"};
    char rutaCarta[256];
    int indice = 0;
    int palo, valor;
    int cartasExitosas = 0;
    int cartasFallidas = 0;
    
    printf("\n=== INICIANDO CARGA DE CARTAS ===\n");
    
    for (palo = 0; palo < 4; palo++) {
        printf("\nCargando palo: %s\n", nombresPalos[palo]);
        
        for (valor = 1; valor <= 13; valor++) {
            sprintf(rutaCarta, "assets/cartas/%s/%d.png", nombresPalos[palo], valor);
            
            texturaCartas[indice] = cargarTextura(renderer, rutaCarta);
            
            if (texturaCartas[indice]) {
                cartasExitosas++;
                printf("  OK [%2d] %s\n", indice, rutaCarta);
            } else {
                cartasFallidas++;
                printf("  ERROR [%2d] %s\n", indice, rutaCarta);
            }
            
            indice++;
        }
    }
    
    printf("\n=== RESUMEN DE CARGA ===\n");
    printf("Cartas cargadas exitosamente: %d\n", cartasExitosas);
    printf("Cartas con error: %d\n", cartasFallidas);
    printf("Total: %d/52\n", indice);
    
    if (cartasFallidas > 0) {
        printf("\nADVERTENCIA: Algunas cartas no se cargaron correctamente\n");
        printf("Verifica que existan los archivos en las rutas indicadas\n");
        return false;
    }
    
    printf("\nTodas las cartas cargadas correctamente\n");
    return true;
}

// Carga todos los recursos del juego
bool cargarRecursos(Juego *juego) {
    
    // Cargar fuentes
    juego->fuenteTitulo = TTF_OpenFont("assets/fuentes/VCR_OSD_MONO_1.001.ttf", 72);
    juego->fuenteNormal = TTF_OpenFont("assets/fuentes/VCR_OSD_MONO_1.001.ttf", 32);
    
    if (!juego->fuenteTitulo || !juego->fuenteNormal) {
        printf("Error al cargar fuentes: %s\n", TTF_GetError());
        return false;
    }
    
    juego->botonHit.textura = cargarTextura(juego->renderer, "assets/botones/botonhit.png");
	juego->botonStand.textura = cargarTextura(juego->renderer, "assets/botones/botonstand.png");

	if (!juego->botonHit.textura || !juego->botonStand.textura) {
    	printf("ADVERTENCIA: No se cargaron los botones de juego\n");
	}
    
    // Cargar imágenes de fondo
    juego->fondoMenu = cargarTextura(juego->renderer, "assets/imagenes/brushwalker437.png");
    juego->fondoJuego = cargarTextura(juego->renderer, "assets/imagenes/green_mat_texture_-_base_texture.png");
    
    // Cargar botones
    juego->botonJugar.textura = cargarTextura(juego->renderer, "assets/botones/botonjugar.png");
    juego->botonSalir.textura = cargarTextura(juego->renderer, "assets/botones/botonsalir.png");
    juego->botonNuevaPartida.textura = cargarTextura(juego->renderer, "assets/botones/botonnuevapartida.png");

    
    // Cargar todas las cartas
    if (!cargarTodasLasCartas_Metodo4(juego->renderer, juego->texturaCartas)) {
        return false;
    }
    
    // Cargar música
    //juego->musicaFondo = Mix_LoadMUS("assets/audio/musica_fondo.mp3");
    //if (!juego->musicaFondo) {
    //    printf("Error al cargar música: %s\n", Mix_GetError());
    //}
    
    // Cargar efectos de sonido (comentados por ahora)
    //juego->sonidoClick = Mix_LoadWAV("assets/audio/click.wav");
    //juego->sonidoRepartir = Mix_LoadWAV("assets/audio/repartir.wav");
    //juego->sonidoVictoria = Mix_LoadWAV("assets/audio/victoria.wav");
    
    return true;
}

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
    
    // Botón Hit
	juego->botonHit.rect.w = 150;
	juego->botonHit.rect.h = 60;
	juego->botonHit.rect.x = 700;
	juego->botonHit.rect.y = 500;
	juego->botonHit.mouseEncima = false;

	// Botón Stand
	juego->botonStand.rect.w = 150;
	juego->botonStand.rect.h = 60;
	juego->botonStand.rect.x = 900;
	juego->botonStand.rect.y = 500;
	juego->botonStand.mouseEncima = false;
	
	// Botón Nueva Partida
	juego->botonNuevaPartida.rect.w = 250;
	juego->botonNuevaPartida.rect.h = 70;
	juego->botonNuevaPartida.rect.x = (ANCHO_VENTANA - juego->botonNuevaPartida.rect.w) / 2;
	juego->botonNuevaPartida.rect.y = ALTO_VENTANA / 2 + 100;
	juego->botonNuevaPartida.mouseEncima = false;
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
    juego->sliderVolumen.volumen = 50;
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
    juego->mostrarCartasDealer = false;
    juego->partidaTerminada = false;
    
    // Reproducir música de fondo
    if (juego->musicaFondo) {
        Mix_PlayMusic(juego->musicaFondo, -1);
    }
    
    juego->mostrarMensaje = false;
	strcpy(juego->mensajeResultado, "");
}

// FUNCIONES DE ACTUALIZACIÓN Y LÓGICA

bool puntoEnRect(int x, int y, SDL_Rect rect) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

void actualizarSliderVolumen(Juego *juego, int mouseX, int mouseY, bool mousePresionado) {
    tSliderVolumen *slider = &juego->sliderVolumen;
    
    if (mousePresionado && puntoEnRect(mouseX, mouseY, slider->barraBola)) {
        slider->arrastrando = true;
    }
    
    if (!mousePresionado) {
        slider->arrastrando = false;
    }
    
    if (slider->arrastrando) {
        int nuevaX = mouseX - slider->barraBola.w / 2;
        
        if (nuevaX < slider->barraFondo.x) {
            nuevaX = slider->barraFondo.x;
        } else if (nuevaX > slider->barraFondo.x + slider->barraFondo.w - slider->barraBola.w) {
            nuevaX = slider->barraFondo.x + slider->barraFondo.w - slider->barraBola.w;
        }
        
        slider->barraBola.x = nuevaX;
        slider->volumen = ((nuevaX - slider->barraFondo.x) * 100) / slider->barraFondo.w;
        Mix_VolumeMusic(slider->volumen * MIX_MAX_VOLUME / 100);
    }
}

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
    	// Reiniciar mazo y manos
    		juego->mazo.tope = 0;
    		llenarMazo(&juego->mazo);
    		llenarMazo(&juego->mazo);
    		mezclarMazoProfundo(&juego->mazo, 5);
    		vaciarMano(&juego->manoJugador);
    		vaciarMano(&juego->manoDealer);
    
    	// Repartir cartas iniciales (2 para cada uno)
    		tCarta carta;
    		int i;
    		for (i = 0; i < 2; i++) {
        		repartirCarta(&juego->mazo, &carta);
        		agregarCartaMano(&juego->manoJugador, carta);
       	 		repartirCarta(&juego->mazo, &carta);
        		agregarCartaMano(&juego->manoDealer, carta);
    		}
    
    		juego->estado = ESTADO_JUGANDO;
    		juego->mostrarCartasDealer = false;
    		juego->partidaTerminada = false;
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

void manejarEventosJuego(Juego *juego, SDL_Event *evento) {
	int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    
    // Actualizar hover solo si la partida NO ha terminado
    if (!juego->partidaTerminada) {
        juego->botonHit.mouseEncima = puntoEnRect(mouseX, mouseY, juego->botonHit.rect);
        juego->botonStand.mouseEncima = puntoEnRect(mouseX, mouseY, juego->botonStand.rect);
    }
    
    // Actualizar hover del botón nueva partida solo si la partida terminó
    if (juego->partidaTerminada) {
        juego->botonNuevaPartida.mouseEncima = puntoEnRect(mouseX, mouseY, juego->botonNuevaPartida.rect);
    }
    
    if (evento->type == SDL_MOUSEBUTTONDOWN) {
        // Solo permitir HIT/STAND si la partida NO ha terminado
        if (juego->botonHit.mouseEncima && !juego->partidaTerminada) {
            tCarta carta;
            repartirCarta(&juego->mazo, &carta);
            agregarCartaMano(&juego->manoJugador, carta);
            
            if (sePasa(&juego->manoJugador)) {
                strcpy(juego->mensajeResultado, "TE PASASTE! PERDISTE");
                juego->mostrarMensaje = true;
                juego->partidaTerminada = true; // BLOQUEAR BOTONES
                juego->mostrarCartasDealer = true; // Mostrar cartas del dealer
            }
        } else if (juego->botonStand.mouseEncima && !juego->partidaTerminada) {
            juego->mostrarCartasDealer = true;
            juego->partidaTerminada = true; // BLOQUEAR BOTONES
            
            tCarta carta;
            while (calcularValorMano(&juego->manoDealer) < 17) {
                repartirCarta(&juego->mazo, &carta);
                agregarCartaMano(&juego->manoDealer, carta);
            }
            
            int valorJugador = calcularValorMano(&juego->manoJugador);
            int valorDealer = calcularValorMano(&juego->manoDealer);
            
            if (sePasa(&juego->manoDealer)) {
                strcpy(juego->mensajeResultado, "EL DEALER SE PASO! GANASTE!");
            } else if (valorJugador > valorDealer) {
                strcpy(juego->mensajeResultado, "GANASTE!");
            } else if (valorJugador < valorDealer) {
                strcpy(juego->mensajeResultado, "PERDISTE");
            } else {
                strcpy(juego->mensajeResultado, "EMPATE");
            }
            juego->mostrarMensaje = true;
        } else if (juego->botonNuevaPartida.mouseEncima && juego->partidaTerminada) {
            // REINICIAR PARTIDA
            juego->mazo.tope = 0;
            llenarMazo(&juego->mazo);
            llenarMazo(&juego->mazo); // Doble mazo
            
            mezclarMazoProfundo(&juego->mazo, 5);
            
            vaciarMano(&juego->manoJugador);
            vaciarMano(&juego->manoDealer);
            
            tCarta carta;
            int i;
			for (i = 0; i < 2; i++) {
                repartirCarta(&juego->mazo, &carta);
                agregarCartaMano(&juego->manoJugador, carta);
                repartirCarta(&juego->mazo, &carta);
                agregarCartaMano(&juego->manoDealer, carta);
            }
            
            juego->mostrarMensaje = false;
            juego->partidaTerminada = false;
            juego->mostrarCartasDealer = false;
        }
    }
    
    if (evento->type == SDL_KEYDOWN && evento->key.keysym.sym == SDLK_ESCAPE) {
        juego->mostrarMensaje = false;
        juego->partidaTerminada = false;
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

// FUNCIONES DE RENDERIZADO

void renderizarTexto(SDL_Renderer *renderer, TTF_Font *fuente, const char *texto, 
    int x, int y, SDL_Color color) {
    	SDL_Surface *superficie = TTF_RenderText_Blended(fuente, texto, color);
    	if (!superficie){
    		return;
		} 
    
    	SDL_Texture *textura = SDL_CreateTextureFromSurface(renderer, superficie);
    	SDL_Rect destRect = {x, y, superficie->w, superficie->h};
    
    	SDL_RenderCopy(renderer, textura, NULL, &destRect);
    
    	SDL_FreeSurface(superficie);
    	SDL_DestroyTexture(textura);
}

void renderizarBoton(SDL_Renderer *renderer, tBoton *boton) {
    if (boton->mouseEncima) {
        SDL_SetTextureColorMod(boton->textura, 200, 200, 200);
    } else {
        SDL_SetTextureColorMod(boton->textura, 255, 255, 255);
    }
    
    SDL_RenderCopy(renderer, boton->textura, NULL, &boton->rect);
}

void renderizarSlider(SDL_Renderer *renderer, tSliderVolumen *slider, TTF_Font *fuente) {
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &slider->barraFondo);
    
    SDL_Rect progreso = slider->barraFondo;
    progreso.w = (slider->volumen * slider->barraFondo.w) / 100;
    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
    SDL_RenderFillRect(renderer, &progreso);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &slider->barraBola);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &slider->barraBola);
    
    char textoVolumen[20];
    sprintf(textoVolumen, "Volumen: %d%%", slider->volumen);
    SDL_Color colorBlanco = {255, 255, 255, 255};
    renderizarTexto(renderer, fuente, textoVolumen, 
                    slider->barraFondo.x, slider->barraFondo.y - 30, colorBlanco);
}

void renderizarMenu(Juego *juego) {
    if (juego->fondoMenu) {
        SDL_RenderCopy(juego->renderer, juego->fondoMenu, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(juego->renderer, 0, 100, 0, 255);
        SDL_RenderClear(juego->renderer);
    }
    
    SDL_Color colorBlanco = {255, 255, 255, 255};
    renderizarTexto(juego->renderer, juego->fuenteTitulo, "BLACKJACK", 
                    ANCHO_VENTANA/2 - 200, 100, colorBlanco);
    
    renderizarBoton(juego->renderer, &juego->botonJugar);
    renderizarBoton(juego->renderer, &juego->botonSalir);
    
    renderizarSlider(juego->renderer, &juego->sliderVolumen, juego->fuenteNormal);
}

void renderizarJuego(Juego *juego) {
    if (juego->fondoJuego) {
        SDL_RenderCopy(juego->renderer, juego->fondoJuego, NULL, NULL);
    } else {
        SDL_SetRenderDrawColor(juego->renderer, 0, 128, 0, 255);
        SDL_RenderClear(juego->renderer);
    }
    
    SDL_Color colorBlanco = {255, 255, 255, 255};
    
    // Título
    renderizarTexto(juego->renderer, juego->fuenteNormal, 
                    "BLACKJACK - Presiona ESC para volver", 50, 20, colorBlanco);
    
    // Mostrar mano del dealer
    renderizarTexto(juego->renderer, juego->fuenteNormal, 
                    "DEALER:", 100, 100, colorBlanco);
    
    // Mostrar cartas del dealer 
    tNodo *actualDealer = juego->manoDealer.primero;
    int xDealer = 100;
    int cartaNumero = 0;
    while (actualDealer != NULL) {
        SDL_Rect rectCartaDealer = {xDealer, 150, 100, 150};
        if (cartaNumero == 0 && !juego->mostrarCartasDealer) {
        // Primera carta oculta (mostrar dorso de carta)
        	SDL_SetRenderDrawColor(juego->renderer, 50, 50, 150, 255); // Azul oscuro
        	SDL_RenderFillRect(juego->renderer, &rectCartaDealer);
        	SDL_SetRenderDrawColor(juego->renderer, 255, 255, 255, 255); // Borde blanco
        	SDL_RenderDrawRect(juego->renderer, &rectCartaDealer);
        
        // Dibujar patrón en el dorso
        	SDL_SetRenderDrawColor(juego->renderer, 255, 255, 255, 255);
        	SDL_RenderDrawLine(juego->renderer, xDealer + 10, 160, xDealer + 90, 290);
        	SDL_RenderDrawLine(juego->renderer, xDealer + 90, 160, xDealer + 10, 290);
    	} else {
        	// Mostrar carta normal
        	SDL_Texture *texturaCartaDealer = obtenerTexturaCarta(juego->texturaCartas, actualDealer->carta);
        	SDL_RenderCopy(juego->renderer, texturaCartaDealer, NULL, &rectCartaDealer);
    	}
        xDealer += 120;
        actualDealer = actualDealer->siguiente;
    }
    
    // Mostrar valor del dealer
    char textoValorDealer[50];
    if (juego->mostrarCartasDealer) {
    	sprintf(textoValorDealer, "Valor: %d", calcularValorMano(&juego->manoDealer));
	} else {
    	sprintf(textoValorDealer, "Valor: ?");
	}
    renderizarTexto(juego->renderer, juego->fuenteNormal, 
                    textoValorDealer, 100, 320, colorBlanco);
    
    // Mostrar mano del jugador
    renderizarTexto(juego->renderer, juego->fuenteNormal, 
                    "JUGADOR:", 100, 400, colorBlanco);
    
    // Mostrar cartas del jugador
    tNodo *actualJugador = juego->manoJugador.primero;
    int xJugador = 100;
    while (actualJugador != NULL) {
        SDL_Texture *texturaCartaJugador = obtenerTexturaCarta(juego->texturaCartas, actualJugador->carta);
        SDL_Rect rectCartaJugador = {xJugador, 450, 100, 150};
        SDL_RenderCopy(juego->renderer, texturaCartaJugador, NULL, &rectCartaJugador);
        xJugador += 120;
        actualJugador = actualJugador->siguiente;
    }
    
    // Mostrar valor del jugador
    char textoValorJugador[50];
    sprintf(textoValorJugador, "Valor: %d", calcularValorMano(&juego->manoJugador));
    renderizarTexto(juego->renderer, juego->fuenteNormal, 
                    textoValorJugador, 100, 620, colorBlanco);
                    
	if (!juego->partidaTerminada) {
    	// Dibujar botón HIT
    	if (juego->botonHit.mouseEncima) {
        	SDL_SetRenderDrawColor(juego->renderer, 150, 0, 0, 255);
    	} else {
        	SDL_SetRenderDrawColor(juego->renderer, 200, 0, 0, 255);
    	}
    	SDL_RenderFillRect(juego->renderer, &juego->botonHit.rect);
    	SDL_SetRenderDrawColor(juego->renderer, 0, 0, 0, 255);
    	SDL_RenderDrawRect(juego->renderer, &juego->botonHit.rect);
    	renderizarTexto(juego->renderer, juego->fuenteNormal, 
                    	"HIT", juego->botonHit.rect.x + 50, 
                    	juego->botonHit.rect.y + 15, colorBlanco);
    
    	// Dibujar botón STAND
    	if (juego->botonStand.mouseEncima) {
        	SDL_SetRenderDrawColor(juego->renderer, 0, 100, 0, 255);
    	} else {
        	SDL_SetRenderDrawColor(juego->renderer, 0, 150, 0, 255);
    	}
    	SDL_RenderFillRect(juego->renderer, &juego->botonStand.rect);
    	SDL_SetRenderDrawColor(juego->renderer, 0, 0, 0, 255);
    	SDL_RenderDrawRect(juego->renderer, &juego->botonStand.rect);
    	renderizarTexto(juego->renderer, juego->fuenteNormal, 
                    	"STAND", juego->botonStand.rect.x + 25, 
                    	juego->botonStand.rect.y + 15, colorBlanco);
}
    
    // Mostrar mensaje de resultado
	if (juego->mostrarMensaje && juego->partidaTerminada) {
    	// Fondo semi-transparente
    	SDL_SetRenderDrawBlendMode(juego->renderer, SDL_BLENDMODE_BLEND);
    	SDL_SetRenderDrawColor(juego->renderer, 0, 0, 0, 180);
    	SDL_Rect fondoMensaje = {ANCHO_VENTANA/2 - 300, ALTO_VENTANA/2 - 100, 600, 200};
    	SDL_RenderFillRect(juego->renderer, &fondoMensaje);
    
    	// Mensaje
    	SDL_Color colorAmarillo = {255, 255, 0, 255};
    	renderizarTexto(juego->renderer, juego->fuenteTitulo, 
                    	juego->mensajeResultado, 
                    	ANCHO_VENTANA/2 - 250, ALTO_VENTANA/2 - 50, colorAmarillo);
    
    	renderizarTexto(juego->renderer, juego->fuenteNormal, 
                    	"Presiona ESC para volver al menu", 
                    	ANCHO_VENTANA/2 - 250, ALTO_VENTANA/2 + 50, colorBlanco);
                    	
        if (juego->botonNuevaPartida.mouseEncima) {
        	SDL_SetRenderDrawColor(juego->renderer, 0, 150, 255, 255);
    	} else {
        	SDL_SetRenderDrawColor(juego->renderer, 0, 100, 200, 255);
    	}
    	SDL_RenderFillRect(juego->renderer, &juego->botonNuevaPartida.rect);
    	SDL_SetRenderDrawColor(juego->renderer, 255, 255, 255, 255);
    	SDL_RenderDrawRect(juego->renderer, &juego->botonNuevaPartida.rect);
    	renderizarTexto(juego->renderer, juego->fuenteNormal, 
                    	"NUEVA PARTIDA", 
                    	juego->botonNuevaPartida.rect.x + 30, 
                    	juego->botonNuevaPartida.rect.y + 20, colorBlanco);
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

// FUNCIONES DE LIMPIEZA

void liberarRecursos(Juego *juego) {
    if (juego->fondoMenu) SDL_DestroyTexture(juego->fondoMenu);
    if (juego->fondoJuego) SDL_DestroyTexture(juego->fondoJuego);
    if (juego->botonJugar.textura) SDL_DestroyTexture(juego->botonJugar.textura);
    if (juego->botonSalir.textura) SDL_DestroyTexture(juego->botonSalir.textura);
    
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

// FUNCIÓN PRINCIPAL

int main(int argc, char *argv[]) {
    Juego juego = {0};
    
    if (!inicializarSDL(&juego)) {
        return 1;
    }
    
    if (!cargarRecursos(&juego)) {
        printf("Error al cargar recursos\n");
        liberarRecursos(&juego);
        return 1;
    }
    
    inicializarJuego(&juego);
    
    Uint32 tiempoAnterior = SDL_GetTicks();
    const Uint32 TIEMPO_POR_FRAME = 1000 / FPS;
    
    while (juego.ejecutando && juego.estado != ESTADO_SALIR) {
        Uint32 tiempoActual = SDL_GetTicks();
        Uint32 deltaTiempo = tiempoActual - tiempoAnterior;
        
        manejarEventos(&juego);
        renderizar(&juego);
        
        if (deltaTiempo < TIEMPO_POR_FRAME) {
            SDL_Delay(TIEMPO_POR_FRAME - deltaTiempo);
        }
        
        tiempoAnterior = tiempoActual;
    }
    
    liberarRecursos(&juego);
    
    printf("\nGracias por jugar!\n");
    
    return 0;
}
