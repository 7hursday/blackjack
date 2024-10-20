#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MSG_SIZE 250
#define MAX_CARTAS_MANO 11
#define MAX_CARTAS_BARAJA 52

// ESTRUCTURAS PARA JUGAR

typedef struct {
    int valor;             // Valor de la carta (1-10, con figuras valiendo 10 y As siendo 1 u 11).
    char palo[10];        // "Corazones", "Picas", "Tréboles", "Diamantes".
    char nombreCarta[20]; // Suficiente para contener el valor numérico.
} Carta;

typedef struct {
    Carta cartas[MAX_CARTAS_MANO];   // Máximo 11 cartas por mano.
    int cartasPorMano;                // Cantidad actual de cartas en mano.
    int puntuacion;                   // Puntuación actual del jugador.
} ManoJugador;

typedef struct {
    char nombre[MSG_SIZE];            // Nombre del usuario.
    char contrasena[MSG_SIZE];        // Contraseña del usuario.
    ManoJugador mano;                 // Mano del jugador y su puntuación.
    int sesionIniciada;               // Indica si el jugador inició sesión.
    int estaConectado;                // Indica si el jugador está conectado al juego.
} Jugador;

typedef struct {
    Carta baraja[MAX_CARTAS_BARAJA];  // Mazo completo de cartas.
    int cartaActual;                   // Índice de la siguiente carta a sacar del mazo.
    Jugador jugador1;                  // Jugador 1.
    Jugador jugador2;                  // Jugador 2 (dealer o rival).
} Partida;

// FUNCIONES DE GESTIÓN DE LA BARAJA
void inicializarBaraja(Carta baraja[MAX_CARTAS_BARAJA]);
void barajarBaraja(Carta baraja[MAX_CARTAS_BARAJA]);
void robarCarta(Partida *partida, Jugador *jugador);

// FUNCIONES DE GESTIÓN DEL JUGADOR
void mostrarMano(Jugador *jugador);
void calcularPuntuacion(Jugador *jugador);

// FUNCIONES DE GESTIÓN DE LA PARTIDA
void jugarTurno(Partida *partida, Jugador *jugador);
void jugarPartida(Partida *partida);

#endif
