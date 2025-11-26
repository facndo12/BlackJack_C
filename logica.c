#include "logica.h"
#include <string.h>
#include <SDL2/SDL_mixer.h>

// ============================================================================
// INICIALIZACIÓN DE BOTONES
// ============================================================================

void inicializarBotones(Juego *juego) {
    // Botón Jugar
    juego->botonJugar.rect.w = 300;
    juego->botonJugar.rect.h = 100;
    juego->botonJugar.rect.x = (ANCHO_VENTANA - juego->botonJugar.rect.w) / 2;
    juego->botonJugar.rect.y = 300;
    juego->botonJugar.mouseEncima = false;
    
    // Botón Salir
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
    juego->sliderVolumen.barraFondo.x = ANCHO_VENTANA - 350;
    juego->sliderVolumen.barraFondo.y = 50;
    juego->sliderVolumen.barraFondo.w = 300;
    juego->sliderVolumen.barraFondo.h = 10;
    
    juego->sliderVolumen.barraBola.w = 20;
    juego->sliderVolumen.barraBola.h = 20;
    juego->sliderVolumen.volumen = 50;
    juego->sliderVolumen.barraBola.x = juego->sliderVolumen.barraFondo.x + 
                                        (juego->sliderVolumen.volumen * juego->sliderVolumen.barraFondo.w / 100);
    juego->sliderVolumen.barraBola.y = juego->sliderVolumen.barraFondo.y - 5;
    juego->sliderVolumen.arrastrando = false;
    
    Mix_VolumeMusic(juego->sliderVolumen.volumen * MIX_MAX_VOLUME / 100);
}

// ============================================================================
// INICIALIZACIÓN DEL JUEGO
// ============================================================================

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
    
    // Estados del juego
    juego->mostrarCartasDealer = false;
    juego->partidaTerminada = false;
    juego->mostrarMensaje = false;
    strcpy(juego->mensajeResultado, "");
    
    // Reproducir música
    if (juego->musicaFondo) {
        Mix_PlayMusic(juego->musicaFondo, -1);
    }
}

// ============================================================================
// LÓGICA DE PARTIDA
// ============================================================================

void iniciarPartida(Juego *juego) {
    // Preparar mazo
    juego->mazo.tope = 0;
    llenarMazo(&juego->mazo);
    llenarMazo(&juego->mazo); // Doble mazo
    mezclarMazoProfundo(&juego->mazo, NUM_MEZCLAS);
    
    // Limpiar manos
    vaciarMano(&juego->manoJugador);
    vaciarMano(&juego->manoDealer);
    
    // Repartir cartas iniciales
    tCarta carta;
    int i;
    for (i = 0; i < 2; i++) {
        repartirCarta(&juego->mazo, &carta);
        agregarCartaMano(&juego->manoJugador, carta);
        repartirCarta(&juego->mazo, &carta);
        agregarCartaMano(&juego->manoDealer, carta);
    }
    
    // Estados
    juego->estado = ESTADO_JUGANDO;
    juego->mostrarCartasDealer = false;
    juego->partidaTerminada = false;
    juego->mostrarMensaje = false;
}

void reiniciarPartida(Juego *juego) {
    iniciarPartida(juego);
}

void manejarHit(Juego *juego) {
    tCarta carta;
    repartirCarta(&juego->mazo, &carta);
    agregarCartaMano(&juego->manoJugador, carta);
    
    if (sePasa(&juego->manoJugador)) {
        strcpy(juego->mensajeResultado, "TE PASASTE! PERDISTE");
        juego->mostrarMensaje = true;
        juego->partidaTerminada = true;
        juego->mostrarCartasDealer = true;
    }
}

void determinarGanador(Juego *juego) {
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
}

void manejarStand(Juego *juego) {
    juego->mostrarCartasDealer = true;
    juego->partidaTerminada = true;
    
    // Dealer saca cartas hasta tener 17 o más
    tCarta carta;
    while (calcularValorMano(&juego->manoDealer) < DEALER_LIMITE) {
        repartirCarta(&juego->mazo, &carta);
        agregarCartaMano(&juego->manoDealer, carta);
    }
    
    determinarGanador(juego);
    juego->mostrarMensaje = true;
}
