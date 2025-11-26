#ifndef CONFIG_H
#define CONFIG_H

// Configuración de ventana
#define ANCHO_VENTANA 1280
#define ALTO_VENTANA 720
#define FPS 60

// Rutas de recursos
#define RUTA_FUENTE "assets/fuentes/VCR_OSD_MONO_1.001.ttf"
#define RUTA_FONDO_MENU "assets/imagenes/brushwalker437.png"
#define RUTA_FONDO_JUEGO "assets/imagenes/green_mat_texture_-_base_texture.png"
#define RUTA_BOTON_JUGAR "assets/botones/botonjugar.png"
#define RUTA_BOTON_SALIR "assets/botones/botonsalir.png"
#define RUTA_BOTON_HIT "assets/botones/botonhit.png"
#define RUTA_BOTON_STAND "assets/botones/botonstand.png"
#define RUTA_BOTON_NUEVA "assets/botones/botonnuevapartida.png"

// Configuración de juego
#define NUM_MAZOS 2
#define NUM_MEZCLAS 5
#define DEALER_LIMITE 17

// Colores predefinidos
#define COLOR_BLANCO (SDL_Color){255, 255, 255, 255}
#define COLOR_AMARILLO (SDL_Color){255, 255, 0, 255}
#define COLOR_NEGRO (SDL_Color){0, 0, 0, 255}
#define COLOR_ROJO (SDL_Color){200, 0, 0, 255}
#define COLOR_VERDE (SDL_Color){0, 150, 0, 255}
#define COLOR_AZUL (SDL_Color){0, 100, 200, 255}

#endif
