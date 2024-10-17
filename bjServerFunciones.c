#include "funciones.h"

// 2. FUNCIONES DE GESTIÓN DE LA BARAJA
// nota: los ases no se distinguen hasta la hora de contabilizar los puntos

void inicializarBaraja(Partida *partida)
{
    for (int palo = 0; palo < 4; palo++)
    {
        for (int valor = 0; valor < 13; valor++)
        {
            partida -> baraja[palo][valor] = 1;  // hay carta
        }
        partida -> cartasPorPalo[palo] = 13;  // todos los palos tienen 13 cartas
    }
    partida -> cartaActual = 0;
}

void barajarBaraja(Partida *partida)
{
    int barajaAux[MAX_CARTAS_BARAJA];
    int contador = 0;

    for (int palo = 0; palo < 4; palo++)
    {
        for (int valor = 0; valor < 13; valor++)
        {
            if (partida -> baraja[palo][valor] == 1)
            {
                barajaAux[contador++] = palo * 13 + valor; // se incrementa el contador por cada carta nueva que entre
            }
        }
    }

    // algoritmo de remezcla
    srand(time(NULL));
    for (int i = contador - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int aux = barajaAux[i];
        barajaAux[i] = barajaAux[j];
        barajaAux[j] = aux;
    }

    // limpio y reseteo
    for (int palo = 0; palo < 4; palo++)
    {
        for (int valor = 0; valor < 13; valor++)
        {
            partida -> baraja[palo][valor] = 0;
        }
    }

    // reinserto la baraja remezclada
    for (int i = 0; i < MAX_CARTAS_BARAJA; i++)
    {
        int palo = barajaAux[i] / 13;
        int valor = barajaAux[i] % 13;
        partida -> baraja[palo][valor] = 1; // hay carta
    }

    partida->cartaActual = 0;
}

int sacarCarta(Partida *partida)
{
    int palo, valor;

    if (partida -> cartaActual >= MAX_CARTAS_BARAJA)
    {
        return -1;  // no hay mas cartas
    }

    do {
        palo = rand() % 4;   
        valor = rand() % 13; 
    } while (partida -> baraja[palo][valor] == 0);  // selecciona otra carta si no esta disponible

    partida -> baraja[palo][valor] = 0;  // indica que la carta ya ha sido usada
    partida -> cartasPorPalo[palo]--;    // se reduce la cantidad de cartas en el palo
    partida -> cartaActual++;            // mas una partida
    
    return valor;
}

void reiniciarBaraja(Partida *partida)
{
    inicializarBaraja(partida);  
    partida -> cartaActual = 0;    // reinicia el contador
}

int todasCartasJugadas(Partida *partida)
{
    return partida -> cartaActual >= MAX_CARTAS_BARAJA;
}

int calcularPuntuacion(ManoJugador *mano)
{
    int puntuacion, ases = 0;

    for (int i = 0; i < mano -> cartasPorMano; i++)
    {
        if (mano -> carta[i] >= 10)
        {
            puntuacion += 10; // para J, Q, K
            
        } else if (mano -> carta[i] == 0) {

            puntuacion += 11; // los ases valen 11 ahora mismo
            ases++;

        } else {

            puntuacion += mano -> carta[i] + 2; // resto de cartas
        }
    }

    while (puntuacion > 21 && ases > 0) // sobrepasamos max puntos
    {
        puntuacion -= 10; // los ases valen 1
        ases--;
    }

    mano -> puntuacion = puntuacion;
    return puntuacion;
}

// 3.   FUNCIONES DE GESTIÓN DEL JUGADOR

void agregarCartaMano(ManoJugador *mano, int carta)
{
    if (mano -> cartasPorMano < MAX_CARTAS_MANO)
    {
        mano -> carta[mano -> cartasPorMano++] = carta;
    }
}

void resetearMano(ManoJugador *mano)
{
    mano -> cartasPorMano = 0; 
    mano -> puntuacion = 0; 
}
