#ifndef BJSERVERFUNCIONES_H
#define BJSERVERFUNCIONES_H

#define MSG_SIZE 250

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructuras necesarias para el juego

typedef struct {
    int carta[11];
    int manoCartas; // cantidad de cartas por mano
    int puntuacion;
} ManoJugador;

typedef struct {
    char nombre[MSG_SIZE];
    char contrasena[MSG_SIZE];
    ManoJugador mano;
    int sesionIniciada;
    int estaConectado;
} Jugador;

typedef struct {
    Jugador jugador1;
    Jugador jugador2;
    int baraja[52]; // todas las cartas de la baraja
    int cartaActual;
} Partida;

// Acciones de la baraja

/*El algoritmo funciona de tal manera que se asocian 13 cartas
a cada uno de los correspondientes palos*/

void cargarBaraja(int baraja[]);
void barajarCartas(int baraja[]);
const char *obtenerPalo(int carta);
int obtenerValor(int carta);
void repartirCarta(int baraja[], int *cartaActual, ManoJugador *mano);
void imprimirMano(ManoJugador *mano);

int calcularPuntuacion(ManoJugador *mano);

#endif
