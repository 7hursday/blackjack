#include "funciones.h"

// Acciones de la baraja

void cargarBaraja(int baraja[])
{
    for (int i = 0; i < 52; i++)
    {
        baraja[i] = i; 
    }
}

void barajarCartas(int baraja[])
{
    srand(time(NULL)); 

    for (int i = 51; i > 0; i--)
    {  
        int j = rand() % (i + 1);
        int aux = baraja[i];
        baraja[i] = baraja[j];
        baraja[j] = aux;
    }
}

const char *obtenerPalo(int carta)
{
    const char *palos[] = {"Picas", "Diamantes", "Corazones", "Tréboles"};
    return palos[carta / 13];
}

int obtenerValor(int carta)
{
    int valor = (carta % 13) + 1; 
    return (valor > 10) ? 10 : valor;
}

void repartirCarta(int baraja[], int *cartaActual, ManoJugador *mano)
{
    if (*cartaActual >= 52)
    {
        printf("No quedan más cartas en la baraja.\n");
        return;
    }

    int carta = baraja[*cartaActual];  // siguiente carta disponible
    mano->carta[mano->manoCartas++] = carta;  // añade la carta a la mano
    mano->puntuacion += obtenerValor(carta);  // suma el valor a la puntuación
    (*cartaActual)++; 
}

void imprimirMano(ManoJugador *mano)
{
    printf("Mano actual: ");
    for (int i = 0; i < mano->manoCartas; i++)
    {
        int carta = mano->carta[i];
        printf("[%s, %d] ", obtenerPalo(carta), obtenerValor(carta));
    }
    printf("\nPuntuación: %d\n", mano->puntuacion);
}

int calcularPuntuacion(ManoJugador *mano)
{
    int total = 0;
    int ases = 0;

    for (int i = 0; i < mano->manoCartas; i++)
    {
        int valor = obtenerValor(mano->carta[i]);

        if (valor == 1)
        {
            ases++;  
            total += 11;  // consideramos el As como 11
        } else {
            total += valor;
        }
    }

    while (total > 21 && ases > 0)
    {
        total -= 10;  // cambiamos un As de 11 a 1
        ases--;
    }

    return total;
}

