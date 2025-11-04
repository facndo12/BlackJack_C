#include <stdio.h>
#include "TADMazo.h"

int main (){
	tCarta carta1;
	tMazo mazo1;
	
	crearMazo(&mazo1);
	if (mazoVacio(mazo1)) {
        printf("\nMazo creado correctamente (vacio).");
    } else {
        printf("\nError: el mazo no esta vacio al crear.");
    }
    
	llenarMazo(&mazo1);
	 printf("\nCantidad actual de cartas: %d", cantidadCartas(mazo1));
    if (mazoLleno(mazo1)) {
        printf("\nMazo lleno correctamente.");
    }
    
	mezclarMazo(&mazo1);
	printf("\nMazo mezclado con exito.");

    // Repartir 5 cartas
    printf("\n\n-> Repartiendo 5 cartas...\n");
    int i;
    for (i = 0; i < 5; i++) {
        repartirCarta(&mazo1, &carta1);
        printf("\nCarta #%d repartida: ", i + 1);
        mostrarCarta(&carta1);
    }
    
    printf("\nCartas restantes en el mazo: %d\n", cantidadCartas(mazo1));
	return 0;
}
