#include "blackjack.h"

int main()
{
    Partida partida;
    partida.cartaActual = 0;
  
    partida.jugador1.mano.cartasPorMano = 0;
    partida.jugador1.mano.puntuacion = 0;
    partida.jugador2.mano.cartasPorMano = 0;
    partida.jugador2.mano.puntuacion = 0;

    printf("Introdice el nombre del jugador 1: ");
    scanf("%s", partida.jugador1.nombre);
    partida.jugador1.sesionIniciada = 1;
    partida.jugador1.estaConectado = 1;

    printf("Introduce el nombre del jugador 2: ");
    scanf("%s", partida.jugador2.nombre);
    partida.jugador2.sesionIniciada = 1;
    partida.jugador2.estaConectado = 1;

    inicializarBaraja(partida.baraja);
    barajarBaraja(partida.baraja);
    
    for (int i = 0; i < 2; i++)
    {
        robarCarta(&partida, &partida.jugador1);
        robarCarta(&partida, &partida.jugador2);
    }

    jugarTurno(&partida, &partida.jugador1);
    if (partida.jugador1.mano.puntuacion <= 21)
    {
        jugarTurno(&partida, &partida.jugador2);
    }
  
    printf("\nResultados:\n");
    calcularPuntuacion(&partida.jugador1);
    calcularPuntuacion(&partida.jugador2);
    
    printf("%s tiene una puntuación de %d\n", partida.jugador1.nombre, partida.jugador1.mano.puntuacion);
    printf("%s tiene una puntuación de %d\n", partida.jugador2.nombre, partida.jugador2.mano.puntuacion);
    
    if (partida.jugador1.mano.puntuacion > 21)
    {
        printf("%s ha perdido. ¡%s gana!\n", partida.jugador1.nombre, partida.jugador2.nombre);
    } else if (partida.jugador2.mano.puntuacion > 21 || 
               partida.jugador1.mano.puntuacion > partida.jugador2.mano.puntuacion)
    {
        printf("%s gana!\n", partida.jugador1.nombre);
    } else if (partida.jugador1.mano.puntuacion < partida.jugador2.mano.puntuacion)
    {
        printf("%s gana\n", partida.jugador2.nombre);
    } else {
        printf("Hay un empate\n");
    }

    return 0;
}
