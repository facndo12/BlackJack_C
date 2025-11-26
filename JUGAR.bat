@echo off
title BLACKJACK
color 0A

echo ================================
echo    CONFIGURANDO ENTORNO...
echo ================================

REM --- ESTA ES LA SOLUCION MAGICA ---
REM Forzamos a Windows a usar SOLO la carpeta de tu compilador MSYS2 y Windows basico.
REM Esto evita que se mezclen librerias de otros programas.
set PATH=C:\msys64\mingw64\bin;C:\Windows\System32;C:\Windows
REM ----------------------------------

echo.
echo ================================
echo    BLACKJACK - Compilando...
echo ================================
echo.

REM Compilamos el juego
gcc main.c recursos.c renderizado.c eventos.c logica.c TADCarta.c TADMazo.c TADMano.c TADJugador.c -o blackjack.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

if errorlevel 1 (
    color 0C
    echo.
    echo ================================
    echo ERROR DE COMPILACION
    echo ================================
    echo No se pudo generar el juego.
    pause
    exit /b 1
)

echo Compilacion exitosa!
echo.
echo ================================
echo    EJECUTANDO JUEGO
echo ================================
echo.

blackjack.exe

if errorlevel 1 (
    echo.
    echo El juego se cerro inesperadamente (Quizas faltan DLLs de SDL).
    pause
)