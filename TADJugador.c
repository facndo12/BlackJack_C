#include "TADJugador.h"
#include <stdlib.h>
#include <string.h>

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
            nuevo->izq = NULL;
            nuevo->der = NULL;
            *pArbol = nuevo;
        }
    } else {
        comparacion = strcmp(pJugador.nombre, (*pArbol)->jugador.nombre);
        if (comparacion < 0) {
            insertarJugador(&(*pArbol)->izq, pJugador);
        } else if (comparacion > 0) {
            insertarJugador(&(*pArbol)->der, pJugador);
        }
    }
}

tNodoJugador* buscarJugador(tArbolJugadores pArbol, char *pNombre) {
    int comparacion;
    if (pArbol == NULL || pNombre == NULL) {
        return NULL;
    }
    comparacion = strcmp(pNombre, pArbol->jugador.nombre);

    if (comparacion == 0) {
        return pArbol; // Retorna el nodo
    } else if (comparacion < 0) {
        return buscarJugador(pArbol->izq, pNombre);
    } else {
        return buscarJugador(pArbol->der, pNombre);
    }
}

// Función auxiliar local
tNodoJugador* buscarMinimo(tArbolJugadores pArbol) {
    if (pArbol == NULL) {
        return NULL;
    } else if (pArbol->izq == NULL) {
        return pArbol;
    } else {
        return buscarMinimo(pArbol->izq);
    }
}

void eliminarJugador(tArbolJugadores *pArbol, char *pNombre) {
    tNodoJugador *temp, *minimo;
    int comparacion;

    if (pArbol == NULL || *pArbol == NULL || pNombre == NULL) {
        printf("\nError: parametros invalidos en ELIMINAR JUGADOR\n");
    } else {
        comparacion = strcmp(pNombre, (*pArbol)->jugador.nombre);
        if (comparacion < 0) {
            eliminarJugador(&(*pArbol)->izq, pNombre);
        } else if (comparacion > 0) {
            eliminarJugador(&(*pArbol)->der, pNombre);
        } else {
            if ((*pArbol)->izq == NULL && (*pArbol)->der == NULL) {
                free(*pArbol);
                *pArbol = NULL;
            } else if ((*pArbol)->izq == NULL) {
                temp = *pArbol;
                *pArbol = (*pArbol)->der;
                free(temp);
            } else if ((*pArbol)->der == NULL) {
                temp = *pArbol;
                *pArbol = (*pArbol)->izq;
                free(temp);
            } else {
                minimo = buscarMinimo((*pArbol)->der);
                (*pArbol)->jugador = minimo->jugador;
                eliminarJugador(&(*pArbol)->der, minimo->jugador.nombre);
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
        pJugador->fichas += pCantidad;
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
        mostrarArbolInorden(pArbol->izq);
        mostrarJugador(pArbol->jugador);
        mostrarArbolInorden(pArbol->der);
    }
}

// Auxiliar para mostrar niveles
void mostrarNivel(tArbolJugadores pArbol, int pNivel) {
    if (pArbol == NULL) {
        return;
    }
    if (pNivel == 0) {
        printf("%s ", pArbol->jugador.nombre);
    } else {
        mostrarNivel(pArbol->izq, pNivel - 1);
        mostrarNivel(pArbol->der, pNivel - 1);
    }
}

int alturaArbol(tArbolJugadores pArbol) {
    int alturaIzq, alturaDer;
    if (pArbol == NULL) {
        return 0;
    } else {
        alturaIzq = alturaArbol(pArbol->izq);
        alturaDer = alturaArbol(pArbol->der);
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
        return;
    }
    liberarArbol(&(*pArbol)->izq);
    liberarArbol(&(*pArbol)->der);
    free(*pArbol);
    *pArbol = NULL;
}

// Auxiliar para guardado recursivo
void guardarArbolArchivo(FILE *pArchivo, tArbolJugadores pArbol) {
    if (pArbol != NULL && pArchivo != NULL) {
        fwrite(&pArbol->jugador, sizeof(tJugador), 1, pArchivo);
        guardarArbolArchivo(pArchivo, pArbol->izq);
        guardarArbolArchivo(pArchivo, pArbol->der);
    }
}

void guardarJugadoresArchivo(tArbolJugadores pArbol) {
    FILE *archivo; 
    archivo = fopen(ARCHIVOJUGADORES, "wb");
    if (archivo == NULL) {
        printf("\nError al abrir archivo para guardar...");
    } else {
        guardarArbolArchivo(archivo, pArbol);
        fclose(archivo);
        printf("\nJugadores guardados exitosamente\n");
    }
}

void cargarJugadoresArchivo(tArbolJugadores *pArbol) {
    FILE *archivo; 
    tJugador jugador;

    if (pArbol == NULL) {
        printf("\nError: arbol invalido...");
    } else {
        archivo = fopen(ARCHIVOJUGADORES, "rb");
        if (archivo == NULL) {
            printf("\nNo hay archivo de jugadores previo\n");
        } else {
            while (fread(&jugador, sizeof(tJugador), 1, archivo) == 1) {
                insertarJugador(pArbol, jugador);
            }
            fclose(archivo);
            printf("\nJugadores cargados exitosamente\n");
        }
    }
}

// Funciones para reportes
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
    printf("  Ganadas: %d | Perdidas: %d | Empatadas: %d\n", jugador.partidasGanadas, jugador.partidasPerdidas, jugador.partidasEmpatadas);

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
        generarReporteRecursivo(pArbol->izq, totalJugadores, totalPartidas, totalFichas, totalGanadas, totalPerdidas, totalEmpatadas);
        procesarNodoReporte(pArbol->jugador, totalJugadores, totalPartidas, totalFichas, totalGanadas, totalPerdidas, totalEmpatadas);
        generarReporteRecursivo(pArbol->der, totalJugadores, totalPartidas, totalFichas, totalGanadas, totalPerdidas, totalEmpatadas);
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
