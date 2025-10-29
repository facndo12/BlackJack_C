#include <stdio.h>>

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

tCarta crearCarta(tPalo pPalo, tValor pValor){
    tCarta carta;
    carta.palo = pPalo;
    carta.valor = pValor;
    return carta;
}

int valorCartaJuego (tCarta pCarta){
    if (pCarta.valor == AS){
        return 11;
    } else if (pCarta.valor >= DOS && pCarta.valor <= DIEZ) {
        return pCarta.valor
    } else if (pCarta.valor == JOTA || pCarta.valor == REINA || pCarta.valor == REY){
        return 10;
    }
    return 0; //NO DEBERIA RETORNAR 0 NUNCA, si lo hace hay un error en el valor de la carta
}

bool esAs(tCarta pCarta){
    return pCarta.valor == AS;
}
