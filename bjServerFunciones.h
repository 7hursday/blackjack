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
    int cartasPorMano;              // cuántas cartas tiene en mano
    int puntuacion;                 // puntuación actual del jugador
} ManoJugador;

typedef struct {
    char nombre[MSG_SIZE];          // nombre del usuario
    char contrasena[MSG_SIZE];      // contraseña del usuario
    ManoJugador mano;               // cartas del jugador y su puntuación
    int sesionIniciada;             // indica si el jugador ha iniciado sesión
    int estaConectado;              // indica si el jugador está conectado
} Jugador;

typedef struct {
    int baraja[4][13];              // 4 palos y 13 cartas (2 a 10, J, Q, K, A)
    int cartasPorPalo[4];           // cantidad de cartas por palo
    int cartaActual;                 // siguiente carta a sacar
    Jugador jugador1;                // jugador 1            
    Jugador jugador2;                // jugador 2            
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
void registrarJugador(const char *nombre, const char *contrasena);
void iniciarSesion(Jugador *jugador, const char *nombre, const char *contrasena);
void cambiarTurno(Partida *partida); // cambia el turno entre jugadores
void procesarTurno(Partida *partida); // procesa el turno del jugador actual

#endif
