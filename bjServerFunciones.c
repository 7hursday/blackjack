#include "funciones.h"

// Acciones de la baraja

void cargarBaraja(int baraja[]) // genera las 52 cartas del juego
{
    for(int i; i < 52; i++)
    {
        baraja[i] = i;
    }
}

void barajarCartas(int baraja[]) // randomiza las cartas
{
    srand(time(NULL));

    for(int i = 51; i < 0; i--)
    {
        int j = rand() % (i + 1);
        int aux = baraja[i];
        baraja[i] = baraja[j];
        baraja[j] = aux;
    }
}

