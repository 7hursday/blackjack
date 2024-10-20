#include "blackjack.h"

void inicializarBaraja(Carta baraja[MAX_CARTAS_BARAJA])
{
    const char *palos[] = {"Corazones", "Picas", "Tréboles", "Diamantes"};
    int i = 0;

    for (int palo = 0; palo < 4; palo++)
    {
        for (int valor = 1; valor <= 13; valor++)
        {
            baraja[i].valor = (valor > 10) ? 10 : valor; // figuras valen 10
            strcpy(baraja[i].palo, palos[palo]);
            // nombres a las cartas
            if (valor == 1) {
                strcpy(baraja[i].nombreCarta, "As");
            } else if (valor == 11) {
                strcpy(baraja[i].nombreCarta, "J");
            } else if (valor == 12) {
                strcpy(baraja[i].nombreCarta, "Q");
            } else if (valor == 13) {
                strcpy(baraja[i].nombreCarta, "K");
            } else {
                sprintf(baraja[i].nombreCarta, "%d", valor); // nombres numéricos
            }
            i++;
        }
    }
}

void barajarBaraja(Carta baraja[MAX_CARTAS_BARAJA])
{
    srand(time(NULL));

    for (int i = 0; i < MAX_CARTAS_BARAJA; i++)
    {
        int j = rand() % MAX_CARTAS_BARAJA; // aleatorio
        Carta aux = baraja[i];
        baraja[i] = baraja[j];
        baraja[j] = aux;
    }
}


void robarCarta(Partida *partida, Jugador *jugador)
{
    if (partida->cartaActual < MAX_CARTAS_BARAJA)
    {
        Carta cartaRobada = partida->baraja[partida->cartaActual];
        partida->cartaActual++;

        // añadir a la mano del jugador
        if (jugador->mano.cartasPorMano < MAX_CARTAS_MANO)
        {
            jugador->mano.cartas[jugador->mano.cartasPorMano] = cartaRobada;
            jugador->mano.cartasPorMano++;
            calcularPuntuacion(jugador); // puntuación del jugador
        } else {
            printf("El jugador ya tiene el máximo de cartas en mano.\n");
        }
    } else {
        printf("No quedan cartas en la baraja para robar.\n");
    }
}


void mostrarMano(Jugador *jugador)
{
    printf("Mano de %s:\n", jugador->nombre);
    for (int i = 0; i < jugador->mano.cartasPorMano; i++)
    {
        printf("%s de %s\n", jugador->mano.cartas[i].nombreCarta, jugador->mano.cartas[i].palo);
    }
    printf("Puntuación actual: %d\n", jugador->mano.puntuacion);
}


void calcularPuntuacion(Jugador *jugador)
{
    int puntuacion = 0;
    int numeroases = 0; // Contador para Ases

    for (int i = 0; i < jugador->mano.cartasPorMano; i++)
    {
        puntuacion += jugador->mano.cartas[i].valor;

        // Contar cuántos Ases hay
        if (strcmp(jugador->mano.cartas[i].nombreCarta, "As") == 0)
        {
            numeroases++;
        }
    }

    while (puntuacion > 21 && numeroases > 0)
    {
        puntuacion -= 10; // contar el as como 1 en vez de 11
        numeroases--;
    }

    jugador->mano.puntuacion = puntuacion; // actualizar la puntuación del jugador
}


void jugarTurno(Partida *partida, Jugador *jugador)
{
    char opcion;
    mostrarMano(jugador);

    // preguntar al jugador si quiere pedir carta o plantarse
    printf("¿Deseas pedir otra carta (p) o plantarte (s)? ");
    scanf(" %c", &opcion);

    while (opcion == 'pedir' || opcion == 'plantarte')
    {
        robarCarta(partida, jugador);
        calcularPuntuacion(jugador);
        mostrarMano(jugador);

        // si el jugador ha perdido
        if (jugador->mano.puntuacion > 21)
        {
            printf("¡Te has pasado de 21! Has perdido.\n");
            return;
        }
        printf("¿Deseas pedir otra carta o plantarte? ");
        scanf(" %c", &opcion);
    }
    printf("%s se ha plantado con una puntuación de %d.\n", jugador->nombre, jugador->mano.puntuacion);
}

void jugarPartida(Partida *partida)
{
    inicializarBaraja(partida->baraja);
    barajarBaraja(partida->baraja);
    
    // dar dos cartas de inicio a los jugadores
    for (int i = 0; i < 2; i++)
    {
        robarCarta(partida, &partida->jugador1);
        robarCarta(partida, &partida->jugador2);
    }

    // jugador 1
    printf("Turno de %s:\n", partida->jugador1.nombre);
    jugarTurno(partida, &partida->jugador1);

    // si 1 no pierde paso a 2
    if (partida->jugador1.mano.puntuacion <= 21)
    {
        printf("Cambio de turno:\n");
        while (partida->jugador2.mano.puntuacion < 17) {
            robarCarta(partida, &partida->jugador2);
            calcularPuntuacion(&partida->jugador2);
        }
        mostrarMano(&partida->jugador2);

        // comparo puntuaciones y determinar el ganador
        if (partida->jugador2.mano.puntuacion > 21) {
            printf("¡Se ha pasado de 21! %s gana.\n", partida->jugador1.nombre);
        } else {
            if (partida->jugador1.mano.puntuacion > partida->jugador2.mano.puntuacion) {
                printf("%s gana con %d contra %d.\n", partida->jugador1.nombre, partida->jugador1.mano.puntuacion, partida->jugador2.mano.puntuacion);
            } else if (partida->jugador1.mano.puntuacion < partida->jugador2.mano.puntuacion) {
                printf("Gana con %d contra %d de %s.\n", partida->jugador2.mano.puntuacion, partida->jugador1.mano.puntuacion, partida->jugador1.nombre);
            } else {
                printf("¡Es un empate! Ambos tienen %d.\n", partida->jugador1.mano.puntuacion);
            }
        }
    }
}

