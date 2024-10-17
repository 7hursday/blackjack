#ifndef BJSERVERFUNCIONES_H
#define BJSERVERFUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MSG_SIZE 250
#define MAX_CARTAS_MANO 11
#define MAX_CARTAS_BARAJA 52

// 1.   ESTRUCTURAS DE JUEGO
typedef struct {
    int carta[MAX_CARTAS_MANO];     // cartas que posee el jugador (max 11)
    int cartasPorMano;              // cuantas cartas tiene carta
    int puntuacion;
} ManoJugador;

typedef struct {
    char nombre[MSG_SIZE];          // usuario
    char contrasena[MSG_SIZE];      // contraseña
    ManoJugador mano;               // cartas del jugador y los puntos
    int sesionIniciada;             // ha iniciado sesion
    int estaConectado;              // esta conectado
} Jugador;

typedef struct {
    int baraja[4][13];              // 4 palos y 13 cartas (2 a 10, J, Q, K, A)
    int cartasPorPalo[4];           // Cantidad de cartas por palo
    int cartaActual;                // Siguiente carta a sacar
    Jugador jugador1;               // jugador 1            
    Jugador jugador2;               // jugador 2            
} Partida;

// 2.   FUNCIONES DE GESTIÓN DE LA BARAJA
void inicializarBaraja(Partida *partida);
void barajarBaraja(Partida *partida);
int sacarCarta(Partida *partida);
void reiniciarBaraja(Partida *partida);
int todasCartasJugadas(Partida *partida);
int calcularPuntuacion(ManoJugador *mano);

// 3.   FUNCIONES DE GESTIÓN DEL JUGADOR
void agregarCartaMano(ManoJugador *mano, int carta);
void resetearMano(ManoJugador *mano);

// 4.   FUNCIONES DE GESTIÓN DE LA PARTIDA
#endif
