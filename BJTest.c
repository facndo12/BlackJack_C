#include <stdio.h>
#include <stdlib.h>
#include "TADCarta.h"
#include "TADMazo.h"
#include "TADMano.h"
#include "TADJugador.h"

void testCarta();
void testMazo();
void testMano();
void testJugador();
void testIntegracion();

int main() {
    printf("\n========================================\n");
    printf("  PROGRAMA DE PRUEBA - BLACKJACK\n");
    printf("========================================\n");
    
    printf("\n\n>>> TEST 1: TAD CARTA <<<\n");
    testCarta();
    
    printf("\n\n>>> TEST 2: TAD MAZO <<<\n");
    testMazo();
    
    printf("\n\n>>> TEST 3: TAD MANO <<<\n");
    testMano();
    
    printf("\n\n>>> TEST 4: TAD JUGADOR <<<\n");
    testJugador();
    
    printf("\n\n>>> TEST 5: INTEGRACION COMPLETA <<<\n");
    testIntegracion();
    
    printf("\n\n========================================\n");
    printf("  PRUEBAS COMPLETADAS\n");
    printf("========================================\n");
    
    return 0;
}

void testCarta() {
    tCarta carta1, carta2, carta3;
    tString valorStr, paloStr;
    
    printf("\n--- Creando cartas ---\n");
    crearCarta(&carta1, CORAZONES, AS);
    crearCarta(&carta2, PICAS, REY);
    crearCarta(&carta3, DIAMANTES, CINCO);
    
    printf("\n--- Mostrando cartas ---\n");
    mostrarCarta(&carta1);
    mostrarCarta(&carta2);
    mostrarCarta(&carta3);
    
    printf("\n--- Valores de juego ---\n");
    printf("AS de Corazones vale: %d\n", valorCartaJuego(carta1));
    printf("Rey de Picas vale: %d\n", valorCartaJuego(carta2));
    printf("5 de Diamantes vale: %d\n", valorCartaJuego(carta3));
    
    printf("\n--- Verificando ases ---\n");
    printf("Carta 1 es AS? %s\n", esAs(carta1) ? "SI" : "NO");
    printf("Carta 2 es AS? %s\n", esAs(carta2) ? "SI" : "NO");
    
    printf("\n? Test de Carta completado\n");
}

void testMazo() {
    tMazo mazo;
    tCarta carta;
    
    printf("\n--- Creando mazo ---\n");
    crearMazo(&mazo);
    printf("Mazo vacio? %s\n", mazoVacio(mazo) ? "SI" : "NO");
    printf("Cantidad de cartas: %d\n", cantidadCartas(mazo));
    
    printf("\n--- Llenando mazo ---\n");
    llenarMazo(&mazo);
    printf("Mazo lleno? %s\n", mazoLleno(mazo) ? "SI" : "NO");
    printf("Cantidad de cartas: %d\n", cantidadCartas(mazo));
    
    printf("\n--- Mezclando mazo ---\n");
    mezclarMazo(&mazo);
    printf("Mazo mezclado exitosamente\n");
    
    printf("\n--- Repartiendo 5 cartas ---\n");
    int i;
    for (i = 0; i < 5; i++) {
        repartirCarta(&mazo, &carta);
        printf("Carta %d: ", i+1);
        mostrarCarta(&carta);
    }
    printf("Cartas restantes en el mazo: %d\n", cantidadCartas(mazo));
    
    printf("\n? Test de Mazo completado\n");
}

void testMano() {
    tMano mano1, mano2;
    tCarta carta;
    
    printf("\n--- Creando manos ---\n");
    crearMano(&mano1);
    crearMano(&mano2);
    printf("Manos creadas exitosamente\n");
    
    printf("\n--- Agregando cartas a mano 1 (AS + REY = Blackjack) ---\n");
    crearCarta(&carta, CORAZONES, AS);
    agregarCartaMano(&mano1, carta);
    crearCarta(&carta, PICAS, REY);
    agregarCartaMano(&mano1, carta);
    
    printf("\nMano 1:\n");
    mostrarMano(&mano1);
    printf("Es Blackjack? %s\n", esBlackjack(&mano1) ? "SI" : "NO");
    printf("Se paso de 21? %s\n", sePasa(&mano1) ? "SI" : "NO");
    
    printf("\n--- Agregando cartas a mano 2 (10 + 9 + 5 = 24 BUST) ---\n");
    crearCarta(&carta, DIAMANTES, DIEZ);
    agregarCartaMano(&mano2, carta);
    crearCarta(&carta, TREBOLES, NUEVE);
    agregarCartaMano(&mano2, carta);
    crearCarta(&carta, CORAZONES, CINCO);
    agregarCartaMano(&mano2, carta);
    
    printf("\nMano 2:\n");
    mostrarMano(&mano2);
    printf("Es Blackjack? %s\n", esBlackjack(&mano2) ? "SI" : "NO");
    printf("Se paso de 21? %s\n", sePasa(&mano2) ? "SI" : "NO");
    
    printf("\n--- Mostrando mano oculta (para dealer) ---\n");
    mostrarManoOculta(&mano1);
    
    printf("\n--- Vaciando manos ---\n");
    vaciarMano(&mano1);
    vaciarMano(&mano2);
    printf("Cantidad de cartas mano 1: %d\n", cantidadCartasMano(&mano1));
    printf("Cantidad de cartas mano 2: %d\n", cantidadCartasMano(&mano2));
    
    printf("\n? Test de Mano completado\n");
}

void testJugador() {
    tArbolJugadores arbol;
    tJugador jugador1, jugador2, jugador3;
    tNodoJugador *encontrado;
    tString nombre1 = "Carlos";
    tString nombre2 = "Ana";
    tString nombre3 = "Beatriz";
    tString buscar = "Ana";
    
    printf("\n--- Inicializando arbol de jugadores ---\n");
    inicializarArbol(&arbol);
    printf("Arbol inicializado\n");
    
    printf("\n--- Creando jugadores ---\n");
    crearJugador(&jugador1, &nombre1, 1000);
    crearJugador(&jugador2, &nombre2, 1500);
    crearJugador(&jugador3, &nombre3, 2000);
    printf("Jugadores creados\n");
    
    printf("\n--- Insertando jugadores en el arbol ---\n");
    insertarJugador(&arbol, jugador1);
    insertarJugador(&arbol, jugador2);
    insertarJugador(&arbol, jugador3);
    printf("Jugadores insertados\n");
    
    printf("\n--- Mostrando arbol por niveles ---\n");
    mostrarArbolPorNivel(arbol);
    
    printf("\n--- Mostrando jugadores en orden ---\n");
    mostrarArbolInorden(arbol);
    
    printf("\n--- Buscando jugador 'Ana' ---\n");
    encontrado = buscarJugador(arbol, &buscar);
    if (encontrado != NULL) {
        printf("Jugador encontrado:\n");
        mostrarJugador(encontrado->jugador);
        
        printf("\n--- Actualizando estadisticas de Ana ---\n");
        actualizarEstadisticas(&encontrado->jugador, 1);  // Gano
        actualizarEstadisticas(&encontrado->jugador, -1); // Perdio
        actualizarEstadisticas(&encontrado->jugador, 1);  // Gano
        modificarFichas(&encontrado->jugador, 500);       // Gano 500 fichas
        
        printf("\nEstadisticas actualizadas:\n");
        mostrarJugador(encontrado->jugador);
    }
    
    printf("\n--- Guardando jugadores en archivo ---\n");
    guardarJugadoresArchivo(arbol);
    
    printf("\n--- Liberando arbol ---\n");
    liberarArbol(&arbol);
    
    printf("\n--- Cargando jugadores desde archivo ---\n");
    inicializarArbol(&arbol);
    cargarJugadoresArchivo(&arbol);
    
    printf("\n--- Generando reporte de estadisticas ---\n");
    generarReporteEstadisticas(arbol);
    
    printf("\n--- Limpieza final ---\n");
    liberarArbol(&arbol);
    
    printf("\n? Test de Jugador completado\n");
}

void testIntegracion() {
    tMazo mazo;
    tMano manoJugador, manoDealer;
    tArbolJugadores arbol;
    tJugador jugador;
    tNodoJugador *nodoJugador;
    tCarta carta;
    tString nombre = "Pedro";
    int i;
    
    printf("\n--- SIMULACION DE UNA MANO DE BLACKJACK ---\n");
    
    // Preparar jugador
    printf("\n1. Preparando jugador...\n");
    inicializarArbol(&arbol);
    crearJugador(&jugador, &nombre, 1000);
    insertarJugador(&arbol, jugador);
    
    // Preparar mazo
    printf("\n2. Preparando mazo...\n");
    crearMazo(&mazo);
    llenarMazo(&mazo);
    mezclarMazo(&mazo);
    printf("Mazo listo con %d cartas\n", cantidadCartas(mazo));
    
    // Preparar manos
    printf("\n3. Creando manos...\n");
    crearMano(&manoJugador);
    crearMano(&manoDealer);
    
    // Repartir cartas iniciales
    printf("\n4. Repartiendo cartas iniciales (2 por jugador)...\n");
    for (i = 0; i < 2; i++) {
        repartirCarta(&mazo, &carta);
        agregarCartaMano(&manoJugador, carta);
        
        repartirCarta(&mazo, &carta);
        agregarCartaMano(&manoDealer, carta);
    }
    
    // Mostrar manos
    printf("\n5. Mano del jugador:\n");
    mostrarMano(&manoJugador);
    
    printf("\nMano del dealer (una carta oculta):\n");
    mostrarManoOculta(&manoDealer);
    
    // Verificar blackjack
    printf("\n6. Verificando Blackjack...\n");
    if (esBlackjack(&manoJugador)) {
        printf("¡BLACKJACK del jugador!\n");
    } else {
        printf("No hay Blackjack del jugador\n");
    }
    
    // Actualizar estadisticas (simulamos que gano)
    printf("\n7. Actualizando estadisticas del jugador...\n");
    nodoJugador = buscarJugador(arbol, &nombre);
    if (nodoJugador != NULL) {
        actualizarEstadisticas(&nodoJugador->jugador, 1);
        modificarFichas(&nodoJugador->jugador, 100);
        mostrarJugador(nodoJugador->jugador);
    }
    
    // Guardar progreso
    printf("\n8. Guardando progreso...\n");
    guardarJugadoresArchivo(arbol);
    
    // Limpieza
    printf("\n9. Limpiando memoria...\n");
    vaciarMano(&manoJugador);
    vaciarMano(&manoDealer);
    liberarArbol(&arbol);
    
    printf("\n? Test de Integracion completado\n");
}
