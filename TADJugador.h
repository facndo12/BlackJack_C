#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "TADCarta.h"
#include "TADMano.h"

#define MAXNOMBRE 30

typedef struct {
    char nombre[MAXNOMBRE];
    int fichas;
    int partidasJugadas;
    int partidasGanadas;
    int partidasPerdidas;
    int partidasEmpatadas;
} tJugador;

typedef struct nodoJugador {
    tJugador jugador;
    struct nodoJugador *izq;
    struct nodoJugador *der;
} tNodoJugador;

tNodoJugador * tArbolJugadores;

void inicializarArbol(tArbolJugadores *pArbol); // Inicializa el arbol vacio
void insertarJugador(tArbolJugadores *pArbol, tJugador pJugador); // Inserta un jugador en el arbol
tNodoJugador* buscarJugador(tArbolJugadores pArbol, char *nombre); // Busca un jugador por nombre
void eliminarJugador(tArbolJugadores *pArbol, char *nombre); // Elimina un jugador del arbol
void crearJugador(tJugador *pJugador, char *nombre, int fichas); // Crea un jugador nuevo

void actualizarEstadisticas(tJugador *pJugador, int resultado); // Actualiza estadisticas (1=gano, 0=empate, -1=perdio)
void modificarFichas(tJugador *pJugador, int cantidad); // Suma o resta fichas
void mostrarJugador(tJugador pJugador); // Muestra datos del jugador
void mostrarArbolInorden(tArbolJugadores pArbol); // Muestra todos los jugadores ordenados
void mostrarArbolPorNivel(tArbolJugadores pArbol); // Muestra el arbol por niveles
void liberarArbol(tArbolJugadores *pArbol); // Libera toda la memoria del arbol

void guardarJugadoresArchivo(tArbolJugadores pArbol); // Guarda jugadores en archivo
void cargarJugadoresArchivo(tArbolJugadores *pArbol); // Carga jugadores desde archivo
void generarReporteEstadisticas(tArbolJugadores pArbol); // Genera reporte con cortes de control

FILE * apuntArchivo;

void inicializarArbol(tArbolJugadores *pArbol) {
    if (pArbol == NULL) {
        printf("\nError al inicializar arbol...");
    } else {
        *pArbol = NULL;
    }
}

void crearJugador(tJugador *pJugador, char *pNombre, int pFichas) {
    if (pJugador == NULL || pNombre == NULL) {
        printf("\nError: parametros invalidos...");
    } else {
        strncpy(pJugador->nombre, pNombre, MAXNOMBRE - 1);
        pJugador->nombre[MAXNOMBRE - 1] = '\0';
        pJugador->fichas = pFichas;
        pJugador->partidasJugadas = 0;
        pJugador->partidasGanadas = 0;
        pJugador->partidasPerdidas = 0;
        pJugador->partidasEmpatadas = 0;
    }
}

void insertarJugador(tArbolJugadores *pArbol, tJugador pJugador) {
    tNodoJugador *nuevo;
    int comparacion;
    if (pArbol == NULL) {
        printf("\nError: arbol invalido...");
    } else if (*pArbol == NULL) {
        nuevo = (tNodoJugador*)malloc(sizeof(tNodoJugador));
        if (nuevo == NULL) {
            printf("\nError: no hay memoria disponible...");
        } else {
            nuevo->jugador = pJugador;
            nuevo->izquierdo = NULL;
            nuevo->derecho = NULL;
            *pArbol = nuevo;
        }
    } else {
        comparacion = strcmp(pJugador.nombre, (*pArbol)->jugador.nombre);
        if (comparacion < 0) {
            insertarJugador(&(*pArbol)->izquierdo, pJugador);
        } else if (comparacion > 0) {
            insertarJugador(&(*pArbol)->derecho, pJugador);
        }
    }
}

tNodoJugador* buscarJugador(tArbolJugadores pArbol, tString *pNombre) {
    int comparacion;
    if (pArbol == NULL || pNombre == NULL) {
        return NULL;
    }
    comparacion = strcmp(pNombre, pArbol->jugador.nombre);

    if (comparacion == 0) {
        return pArbol;
    } else if (comparacion < 0) {
        return buscarJugador(pArbol->izquierdo, pNombre);
    } else {
        return buscarJugador(pArbol->derecho, pNombre);
    }
}

tNodoJugador* buscarMinimo(tArbolJugadores pArbol) {
    if (pArbol == NULL) {
        return NULL;
    } else if (pArbol->izquierdo == NULL) {
        return pArbol;
    } else {
        return buscarMinimo(pArbol->izquierdo);
    }
}

void eliminarJugador(tArbolJugadores *pArbol, tString *pNombre) {
    tNodoJugador *temp, *minimo;
    int comparacion;

    if (pArbol == NULL || *pArbol == NULL || pNombre == NULL) {
        printf("\nError: parametros invalidos en ELIMINAR JUGADOR\n")
    } else {
        comparacion = strcmp(pNombre, (*pArbol)->jugador.nombre);
        if (comparacion < 0) {
            eliminarJugador(&(*pArbol)->izquierdo, pNombre);
        } else if (comparacion > 0) {
            eliminarJugador(&(*pArbol)->derecho, pNombre);
        } else {
            if ((*pArbol)->izquierdo == NULL && (*pArbol)->derecho == NULL) {
                free(*pArbol);
                *pArbol = NULL;
            } else if ((*pArbol)->izquierdo == NULL) {
                temp = *pArbol;
                *pArbol = (*pArbol)->derecho;
                free(temp);
            } else if ((*pArbol)->derecho == NULL) {
                temp = *pArbol;
                *pArbol = (*pArbol)->izquierdo;
                free(temp);
            } else {
                minimo = buscarMinimo((*pArbol)->derecho);
                (*pArbol)->jugador = minimo->jugador;
                eliminarJugador(&(*pArbol)->derecho, minimo->jugador.nombre);
            }
        }
    }
}

void actualizarEstadisticas(tJugador *pJugador, int pResultado) {
    if (pJugador == NULL) {
        printf("\nError: jugador invalido...");
    } else {
        pJugador->partidasJugadas++;
        if (pResultado == 1) {
            pJugador->partidasGanadas++;
        } else if (pResultado == 0) {
            pJugador->partidasEmpatadas++;
        } else if (pResultado == -1) {
            pJugador->partidasPerdidas++;
        }
    }
}

void modificarFichas(tJugador *pJugador, int pCantidad) {
    if (pJugador == NULL) {
        printf("\nError: jugador invalido...");
    } else {
        pJugador->fichas += cantidad;
        if (pJugador->fichas < 0) {
            pJugador->fichas = 0;
        }
    }
}

void mostrarJugador(tJugador pJugador) {
    float porcentajeVictorias;

    printf("\n=== JUGADOR ===\n");
    printf("Nombre: %s\n", pJugador.nombre);
    printf("Fichas: %d\n", pJugador.fichas);
    printf("Partidas jugadas: %d\n", pJugador.partidasJugadas);
    printf("Partidas ganadas: %d\n", pJugador.partidasGanadas);
    printf("Partidas perdidas: %d\n", pJugador.partidasPerdidas);
    printf("Partidas empatadas: %d\n", pJugador.partidasEmpatadas);

    if (pJugador.partidasJugadas > 0) {
        porcentajeVictorias = (float)pJugador.partidasGanadas / pJugador.partidasJugadas * 100;
        printf("Porcentaje de victorias: %.2f%%\n", porcentajeVictorias);
    }
    printf("===============\n");
}

void mostrarArbolInorden(tArbolJugadores pArbol) {
    if (pArbol != NULL) {
        mostrarArbolInorden(pArbol->izquierdo);
        mostrarJugador(pArbol->jugador);
        mostrarArbolInorden(pArbol->derecho);
    }
}

void mostrarNivel(tArbolJugadores pArbol, int nivel) {
    if (pArbol == NULL) {
        printf("\nERROR en MOSTRARNIVEL");
    } else {
       if (nivel == 0) {
            printf("%s ", pArbol->jugador.nombre);
        } else {
            mostrarNivel(pArbol->izquierdo, nivel - 1);
            mostrarNivel(pArbol->derecho, nivel - 1);
        }
    }
}

int alturaArbol(tArbolJugadores pArbol) {
    int alturaIzq, alturaDer;

    if (pArbol == NULL) {
        return 0;
    } else {
        alturaIzq = alturaArbol(pArbol->izquierdo);
        alturaDer = alturaArbol(pArbol->derecho);
        return (alturaIzq > alturaDer ? alturaIzq : alturaDer) + 1;
    }
}

void mostrarArbolPorNivel(tArbolJugadores pArbol) {
    int altura, i;
    if (pArbol == NULL) {
        printf("\nArbol vacio\n");
    } else {
        altura = alturaArbol(pArbol);
        printf("\n=== ARBOL POR NIVELES ===\n");
        for (i = 0; i < altura; i++) {
            printf("Nivel %d: ", i);
            mostrarNivel(pArbol, i);
            printf("\n");
        }
        printf("=========================\n");
        }
}

void liberarArbol(tArbolJugadores *pArbol) {
    if (pArbol == NULL || *pArbol == NULL) {
        printf("\nLIBERANDO ARBOL...");
    } else {
        liberarArbol(&(*pArbol)->izquierdo);
        liberarArbol(&(*pArbol)->derecho);
        free(*pArbol);
        *pArbol = NULL;
    }
}

void guardarArbolArchivo(FILE * pArchivo, tArbolJugadores pArbol) {
    if (pArbol != NULL && pArchivo != NULL) {
        fwrite(&pArbol->jugador, sizeof(tJugador), 1, pArchivo);
        guardarArbolArchivo(pArchivo, pArbol->izquierdo);
        guardarArbolArchivo(pArchivo, pArbol->derecho);
    }
}

void guardarJugadoresArchivo(tArbolJugadores pArbol) {
    pArchivo = fopen(ARCHIVO_JUGADORES, "wb");
    if (pArchivo == NULL) {
        printf("\nError al abrir archivo para guardar...");
    } else {
        guardarArbolArchivo(pArchivo, pArbol);
        fclose(pArchivo);
        printf("\nJugadores guardados exitosamente\n");
    }
}

void cargarJugadoresArchivo(tArbolJugadores *pArbol) {
    tJugador jugador;

    if (pArbol == NULL) {
        printf("\nError: arbol invalido...");
    } else {
        pArchivo = fopen(ARCHIVOJUGADORES, "rb");
        if (pArchivo == NULL) {
            printf("\nNo hay archivo de jugadores previo\n");
        } else {
            while (fread(&jugador, sizeof(tJugador), 1, pArchivo) == 1) {
                insertarJugador(pArbol, jugador);
            }
            fclose(pArchivo);
            printf("\nJugadores cargados exitosamente\n");
        }
    }
}

void imprimirSeparador() {
    printf("\n========================================\n");
}

void imprimirEncabezadoReporte() {
    imprimirSeparador();
    printf("     REPORTE DE ESTADISTICAS\n");
    imprimirSeparador();
}

void imprimirTotalesGeneral(int totalJugadores, int totalPartidas, int totalFichas) {
    imprimirSeparador();
    printf("TOTALES GENERALES:\n");
    printf("Total de jugadores: %d\n", totalJugadores);
    printf("Total de partidas: %d\n", totalPartidas);
    printf("Total de fichas: %d\n", totalFichas);
    imprimirSeparador();
}

void procesarNodoReporte(tJugador jugador, int *totalJugadores, int *totalPartidas, int *totalFichas, int *totalGanadas, int *totalPerdidas, int *totalEmpatadas) {
    float porcentaje;

    printf("\nJugador: %s\n", jugador.nombre);
    printf("  Fichas: %d\n", jugador.fichas);
    printf("  Partidas jugadas: %d\n", jugador.partidasJugadas);
    printf("  Ganadas: %d | Perdidas: %d | Empatadas: %d\n",
           jugador.partidasGanadas, jugador.partidasPerdidas,
           jugador.partidasEmpatadas);

    if (jugador.partidasJugadas > 0) {
        porcentaje = (float)jugador.partidasGanadas / jugador.partidasJugadas * 100;
        printf("  Porcentaje victorias: %.2f%%\n", porcentaje);
    }

    (*totalJugadores)++;
    *totalPartidas += jugador.partidasJugadas;
    *totalFichas += jugador.fichas;
    *totalGanadas += jugador.partidasGanadas;
    *totalPerdidas += jugador.partidasPerdidas;
    *totalEmpatadas += jugador.partidasEmpatadas;
}

void generarReporteRecursivo(tArbolJugadores pArbol, int *totalJugadores, int *totalPartidas, int *totalFichas, int *totalGanadas, int *totalPerdidas, int *totalEmpatadas) {
    if (pArbol != NULL) {
        generarReporteRecursivo(pArbol->izquierdo, totalJugadores, totalPartidas, totalFichas, totalGanadas, totalPerdidas, totalEmpatadas);
        procesarNodoReporte(pArbol->jugador, totalJugadores, totalPartidas, totalFichas, totalGanadas, totalPerdidas, totalEmpatadas);
        generarReporteRecursivo(pArbol->derecho, totalJugadores, totalPartidas, totalFichas, totalGanadas, totalPerdidas, totalEmpatadas);
    }
}

void generarReporteEstadisticas(tArbolJugadores pArbol) {
    int totalJugadores, totalPartidas, totalFichas;
    int totalGanadas, totalPerdidas, totalEmpatadas;
    float promedioPartidas, promedioFichas;

    if (pArbol == NULL) {
        printf("\nNo hay jugadores registrados\n");
    } else {
        totalJugadores = 0;
        totalPartidas = 0;
        totalFichas = 0;
        totalGanadas = 0;
        totalPerdidas = 0;
        totalEmpatadas = 0;
        imprimirEncabezadoReporte();
        generarReporteRecursivo(pArbol, &totalJugadores, &totalPartidas, &totalFichas, &totalGanadas, &totalPerdidas, &totalEmpatadas);
        imprimirTotalesGeneral(totalJugadores, totalPartidas, totalFichas);

        if (totalJugadores > 0) {
            promedioPartidas = (float)totalPartidas / totalJugadores;
            promedioFichas = (float)totalFichas / totalJugadores;

            printf("\nPROMEDIOS:\n");
            printf("Promedio de partidas por jugador: %.2f\n", promedioPartidas);
            printf("Promedio de fichas por jugador: %.2f\n", promedioFichas);
            imprimirSeparador();

            printf("\nDISTRIBUCION DE RESULTADOS:\n");
            printf("Total ganadas: %d\n", totalGanadas);
            printf("Total perdidas: %d\n", totalPerdidas);
            printf("Total empatadas: %d\n", totalEmpatadas);
            imprimirSeparador();
        }
    }
}

