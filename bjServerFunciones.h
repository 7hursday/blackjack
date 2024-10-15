#ifndef BJSERVERFUNCIONES_H
#define BJSERVERFUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructuras necesarias para el juego

typedef struct {
    char palo[10]; // tipo de carta en una cadena de caracteres
    int valor; // número de la carta
} Carta;

/* Estructura alternativa
typedef enum {Picas, Diamantes, Corazones, Treboles} Palo;

typedef struct {
    Palo palo;
    int valor; // número de la carta
} Carta; 
*/

typedef struct {
    Carta cartas[11];
    int manoCartas; // cantidad de cartas por mano
    int puntuacion;
} ManoJugador;

typedef struct {
    int id;
    char nombre[50];
    ManoJugador mano;
    int sesionIniciada;
    int estaConectado;
} Jugador;

typedef struct {
    Jugador jugador1;
    Jugador jugador2;
    Carta baraja[52]; // todas las cartas de la baraja
    int cartaActual;
} Partida;

// Funciones de usuario 
// int registrarUsuario(const char* usuario, const char* contrasena);
int comprobarUsuario();
int verificarInicioSesion();

// Acciones de la baraja
void cargarBaraja(Carta baraja[]);
void barajarCartas(Carta baraja[]);
int calcularPuntos(Carta mano[], int numeroCartas);

#endif
