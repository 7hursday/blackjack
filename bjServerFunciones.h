#ifndef BJSERVERFUNCIONES_H
#define BJSERVERFUNCIONES_H

#define MSG_SIZE 250

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructuras necesarias para el juego

/*typedef struct {
    char palo[10]; // tipo de carta en una cadena de caracteres
    int valor; // número de la carta
} Carta;

// Estructura alternativa
typedef enum {Picas, Diamantes, Corazones, Treboles} Palo;

typedef struct {
    Palo palo;
    int valor; // número de la carta
} Carta; 
*/

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
void emparejarValCartas();

int calcularPuntos(int mano[], int numCartas);

#endif
