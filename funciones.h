/**
 * @file funciones.h
 * @brief Declaraciones de estructuras y funciones para el juego de Blackjack.
 *
 * Este fichero contiene las definiciones de las estructuras Cliente, 
 * Jugador y Partida, así como las declaraciones necesarias 
 * para gestionar el sistema de usuarios, la baraja, y las partidas.
 *
 * @date 21-10-2024
 * @version 1.0
 */

#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MSG_SIZE 250

// 0.    ESTRUCTURAS PARA EL JUEGO

struct Cliente
{
    char usuario[MSG_SIZE];        // Nombre de usuario
    int puntuacion;                // Puntos acumulados
    int esperando;                 // (0: esperando, 1: en juego)
    int sd;                        // Descriptor de socket
    int posicionTurno;             // Turno del cliente en la partida
    int pierde;                    // Ha perdido el jugador (0: sigue jugando, 1: ha perdido)
    int plantado;                  // Se ha plantado el jugador (0: sigue jugando, 1: se ha plantado)
};

typedef struct {
    int id;                        // ID del jugador
    int estado;                    // Estado de la cuenta (0: inactivo, 1: activo)
    char nombre[MSG_SIZE];         // Nombre del jugador
    char contrasena[MSG_SIZE];     // Contraseña del jugador
    int sesionIniciada;            // Ha iniciado sesión (0: no, 1: sí)
} Jugador;

struct Partida
{
    struct Cliente jugador1;
    struct Cliente jugador2;
    int turno;                    // ID del turno actual (0: jugador1, 1: jugador2)
    int numJugadores;             // Número total de jugadores
};


// 1.    FUNCIONES DE ARCHIVO
int registrarFichero(char buffer[],Jugador *cliente);
int comprobrfichero(Jugador *cliente);

// 2.    FUNCIONES DE USUARIO
int comprobarusuario(char buffer[],Jugador *cliente); 
void comprobarcontrasena(char buffer[],Jugador *cliente); 
void registro(char buffer[250], int new_sd);
char* buscarUsuario(char usuario[250]);

// 3.    FUNCIONES DE LA BARAJA
char* generarCarta(int* corazones, int* diamantes, int* treboles, int *picas, int i, struct Partida* partidaVector, int ordenPartida);

// 4.    FUNCIONES ADICIONALES
int buscarCliente(struct Cliente *clientVector, int new_sd);

#endif // FUNCIONES_H

