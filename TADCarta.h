#include <stdio.h>
#include <stdbool.h>
#define MAXCAR 15
typedef char tString [MAXCAR];

typedef enum {CORAZONES, DIAMANTES, PICAS, TREBOLES} tPalo;
typedef enum
    {AS = 1,
    DOS = 2,
    TRES = 3,
    CUATRO = 4,
    CINCO = 5,
    SEIS = 6,
    SIETE = 7,
    OCHO = 8,
    NUEVE = 9,
    DIEZ = 10,
    JOTA = 11,
    REINA = 12,
    REY = 13 }tValor;

typedef struct {
    tPalo palo;
    tValor valor;
}tCarta;

void crearCarta(tCarta *, tPalo, tValor); // Inicializa una carta con palo y valor
int valorCartaJuego(tCarta); // Retorna el valor de juego de la carta
bool esAs(tCarta ); // Verifica si la carta es un AS

void mostrarCarta(tCarta *pCarta); // Imprime la carta por pantalla
void obtenerPalo(tPalo, tString); // Obtiene el palo de la carta
void obtenerValor(tValor, tString); // Obtiene el valor nominal de la carta

void crearCarta(tCarta *pCarta, tPalo pPalo, tValor pValor) {
    if (pCarta == NULL){
    	printf ("\nError al crear la carta...");
	} else {
    	pCarta->palo = pPalo;
    	pCarta->valor = pValor;
    }
}

int valorCartaJuego (tCarta pCarta){
    if (pCarta.valor == AS){
        return 11;
    } else if (pCarta.valor >= DOS && pCarta.valor <= DIEZ) {
        return pCarta.valor;
    } else if (pCarta.valor == JOTA || pCarta.valor == REINA || pCarta.valor == REY){
        return 10;
    }
    return 0; //NO DEBERIA RETORNAR 0 NUNCA, si lo hace hay un error en el valor de la carta
}

bool esAs(tCarta pCarta){
    return pCarta.valor == AS;
}

void obtenerPalo(tPalo pPalo, tString nombrePalo) {
    switch (pPalo) {
        case CORAZONES: strcpy(nombrePalo, "Corazones"); break;
        case DIAMANTES: strcpy(nombrePalo, "Diamantes"); break;
        case PICAS:     strcpy(nombrePalo, "Picas"); break;
        case TREBOLES:  strcpy(nombrePalo, "Treboles"); break;
        default:        strcpy(nombrePalo, "Desconocido"); break;
    }
}

void obtenerValor(tValor pValor, tString nombreValor) {
    switch (pValor) {
        case AS:     strcpy(nombreValor, "AS"); break;
        case DOS:    strcpy(nombreValor, "2"); break;
        case TRES:   strcpy(nombreValor, "3"); break;
        case CUATRO: strcpy(nombreValor, "4"); break;
        case CINCO:  strcpy(nombreValor, "5"); break;
        case SEIS:   strcpy(nombreValor, "6"); break;
        case SIETE:  strcpy(nombreValor, "7"); break;
        case OCHO:   strcpy(nombreValor, "8"); break;
        case NUEVE:  strcpy(nombreValor, "9"); break;
        case DIEZ:   strcpy(nombreValor, "10"); break;
        case JOTA:   strcpy(nombreValor, "J"); break;
        case REINA:  strcpy(nombreValor, "Q"); break;
        case REY:    strcpy(nombreValor, "K"); break;
        default:     strcpy(nombreValor, "Desconocido"); break;
    }
}

void mostrarCarta(tCarta *pCarta) {
    tString nombreValor, nombrePalo;
    
    if (pCarta == NULL) {
        printf("\n\tCarta invalida\n");
    } else {
        obtenerValor(pCarta->valor, nombreValor);
        obtenerPalo(pCarta->palo, nombrePalo);
        printf("\t%s de %s\n", nombreValor, nombrePalo);
    }
}

