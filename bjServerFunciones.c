#include "funciones.h"

// 2. FUNCIONES DE GESTIÓN DE LA BARAJA
// Nota: los ases no se distinguen hasta la hora de contabilizar los puntos

void inicializarBaraja(Partida *partida)
{
    for (int palo = 0; palo < 4; palo++)
    {
        for (int valor = 0; valor < 13; valor++)
        {
            partida->baraja[palo][valor] = 1;  // hay carta
        }
        partida->cartasPorPalo[palo] = 13;  // todos los palos tienen 13 cartas
    }
    partida->cartaActual = 0;
}

void barajarBaraja(Partida *partida)
{
    int barajaAux[MAX_CARTAS_BARAJA];
    int contador = 0;

    for (int palo = 0; palo < 4; palo++)
    {
        for (int valor = 0; valor < 13; valor++)
        {
            if (partida->baraja[palo][valor] == 1)
            {
                barajaAux[contador++] = palo * 13 + valor; // se incrementa el contador por cada carta nueva que entra
            }
        }
    }

    // Algoritmo de remezcla
    srand(time(NULL));
    for (int i = contador - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int aux = barajaAux[i];
        barajaAux[i] = barajaAux[j];
        barajaAux[j] = aux;
    }

    // Limpio y reseteo
    for (int palo = 0; palo < 4; palo++)
    {
        for (int valor = 0; valor < 13; valor++)
        {
            partida->baraja[palo][valor] = 0; // Se resetea la baraja
        }
    }

    // Reinserto la baraja remezclada
    for (int i = 0; i < MAX_CARTAS_BARAJA; i++)
    {
        int palo = barajaAux[i] / 13;
        int valor = barajaAux[i] % 13;
        partida->baraja[palo][valor] = 1; // Hay carta
    }

    partida->cartaActual = 0;
}

int sacarCarta(Partida *partida)
{
    int palo, valor;

    if (partida->cartaActual >= MAX_CARTAS_BARAJA)
    {
        return -1;  // No hay más cartas
    }

    do {
        palo = rand() % 4;   
        valor = rand() % 13; 
    } while (partida->baraja[palo][valor] == 0);  // Selecciona otra carta si no está disponible

    partida->baraja[palo][valor] = 0;  // Indica que la carta ya ha sido usada
    partida->cartasPorPalo[palo]--;    // Se reduce la cantidad de cartas en el palo
    partida->cartaActual++;             // Se incrementa el contador de cartas sacadas
    
    return palo * 13 + valor;           // Retorna el valor de la carta sacada
}

void reiniciarBaraja(Partida *partida)
{
    inicializarBaraja(partida);  
    partida->cartaActual = 0;    // Reinicia el contador de cartas
}

int todasCartasJugadas(Partida *partida)
{
    return partida->cartaActual >= MAX_CARTAS_BARAJA; // Verifica si todas las cartas han sido jugadas
}

int calcularPuntuacion(ManoJugador *mano)
{
    int puntuacion = 0, ases = 0; // Inicializa puntuación y contador de ases

    for (int i = 0; i < mano->cartasPorMano; i++)
    {
        if (mano->carta[i] >= 10)
        {
            puntuacion += 10; // Para J, Q, K
        } 
        else if (mano->carta[i] == 0)
        {
            puntuacion += 11; // Los ases valen 11 por defecto
            ases++;
        } 
        else 
        {
            puntuacion += mano->carta[i] + 2; // Resto de cartas
        }
    }

    while (puntuacion > 21 && ases > 0) // Verifica si la puntuación sobrepasa 21
    {
        puntuacion -= 10; // Convierte un as de 11 a 1
        ases--;
    }

    mano->puntuacion = puntuacion; // Asigna la puntuación calculada a la mano
    return puntuacion;
}

// 3.   FUNCIONES DE GESTIÓN DEL JUGADOR

void agregarCartaMano(ManoJugador *mano, int carta)
{
    if (mano->cartasPorMano < MAX_CARTAS_MANO)
    {
        mano->carta[mano->cartasPorMano++] = carta; // Añade carta a la mano del jugador
    }
}

void resetearMano(ManoJugador *mano)
{
    mano->cartasPorMano = 0;  // Resetea la cantidad de cartas en mano
    mano->puntuacion = 0;      // Resetea la puntuación
}

// 4. FUNCIONES DE GESTIÓN DE LA PARTIDA

void registrarJugador(const char *nombre, const char *contrasena)
{
    // Abrir el archivo en modo de añadir
    FILE *file = fopen("jugadores.txt", "a+");
    if (!file) {
        perror("Error al abrir el archivo de jugadores");
        return; // Salir si no se puede abrir el archivo
    }

    char buffer[MSG_SIZE]; // Para almacenar nombres de jugadores existentes
    int usuarioExistente = 0; // Bandera para verificar si el usuario ya existe

    // Leer el archivo para comprobar si el nombre ya está registrado
    while (fgets(buffer, sizeof(buffer), file)) {
        char nombreArchivo[MSG_SIZE];
        if (sscanf(buffer, "%s", nombreArchivo) == 1) {
            if (strcmp(nombreArchivo, nombre) == 0) {
                usuarioExistente = 1; // Usuario ya existe
                break; // Salir del bucle si se encuentra el usuario
            }
        }
    }

    // Si el usuario ya existe, informamos al usuario
    if (usuarioExistente) {
        printf("Error: El nombre de usuario '%s' ya está registrado.\n", nombre);
    } else {
        // Si el nombre no existe, lo agregamos al archivo
        fprintf(file, "%s %s\n", nombre, contrasena); // Guardar el nuevo usuario
        printf("Registro exitoso: Usuario '%s' registrado correctamente.\n", nombre);
    }

    fclose(file); // Cerrar el archivo
}

void iniciarSesion(Jugador *jugador, const char *nombre, const char *contrasena)
{
    FILE *file = fopen("jugadores.txt", "r"); // Abrir el archivo de jugadores
    if (!file) {
        perror("Error al abrir el archivo de jugadores");
        jugador->sesionIniciada = 0; // Asegúrate de que la sesión no esté iniciada
        jugador->estaConectado = 0;  // Asegúrate de que el jugador no esté conectado
        return;
    }

    char buffer[MSG_SIZE * 2]; // Para almacenar el nombre y contraseña
    int credencialesValidas = 0; // Bandera para verificar credenciales

    // Leer cada línea del archivo y verificar las credenciales
    while (fgets(buffer, sizeof(buffer), file)) {
        char nombreArchivo[MSG_SIZE], contrasenaArchivo[MSG_SIZE];
        // Separa el nombre y la contraseña usando un delimitador (puede ser un espacio, coma, etc.)
        if (sscanf(buffer, "%s %s", nombreArchivo, contrasenaArchivo) == 2) {
            if (strcmp(nombreArchivo, nombre) == 0 && strcmp(contrasenaArchivo, contrasena) == 0) {
                credencialesValidas = 1; // Credenciales válidas
                break; // Salir del bucle si se encuentran credenciales válidas
            }
        }
    }

    fclose(file); // Cerrar el archivo después de la lectura

    if (credencialesValidas) {
        strncpy(jugador->nombre, nombre, MSG_SIZE);  // Asigna el nombre al jugador
        strncpy(jugador->contrasena, contrasena, MSG_SIZE); // Asigna la contraseña
        jugador->sesionIniciada = 1;                  // Marca la sesión como iniciada
        jugador->estaConectado = 1;                   // Marca al jugador como conectado
        printf("Sesión iniciada correctamente para %s.\n", jugador->nombre);
    } else {
        printf("Error: Nombre de usuario o contraseña incorrectos.\n");
        jugador->sesionIniciada = 0;                  // Asegúrate de que la sesión no esté iniciada
        jugador->estaConectado = 0;                   // Asegúrate de que el jugador no esté conectado
    }
}

void cambiarTurno(Partida *partida)
{
    // Cambia el turno entre los jugadores
    if (partida->jugador1.sesionIniciada && partida->jugador2.sesionIniciada) {
        if (partida->jugador1.estaConectado) {
            partida->jugador1.estaConectado = 0;
            partida->jugador2.estaConectado = 1; // Cambia el turno al jugador 2
        } else {
            partida->jugador1.estaConectado = 1; // Cambia el turno al jugador 1
            partida->jugador2.estaConectado = 0;
        }
    }
}

void procesarTurno(Partida *partida)
{
    Jugador *jugadorActual = partida->jugador1.estaConectado ? &partida->jugador1 : &partida->jugador2;

    if (jugadorActual->sesionIniciada) {
        printf("Turno de %s\n", jugadorActual->nombre);
        char accion[10];
        printf("¿Deseas 'pedir' carta o 'plantar'? ");
        scanf("%s", accion);

        if (strcmp(accion, "pedir") == 0) {
            int carta = sacarCarta(partida);
            if (carta != -1) {
                agregarCartaMano(&jugadorActual->mano, carta);
                printf("%s ha sacado la carta: %d\n", jugadorActual->nombre, carta);
                calcularPuntuacion(&jugadorActual->mano);
                if (jugadorActual->mano.puntuacion > 21) {
                    printf("%s se ha pasado de 21. Ha perdido su turno.\n", jugadorActual->nombre);
                    resetearMano(&jugadorActual->mano);
                }
            } else {
                printf("No quedan cartas en la baraja.\n");
            }
        } else if (strcmp(accion, "plantar") == 0) {
            printf("%s se planta. Su puntuación es: %d\n", jugadorActual->nombre, jugadorActual->mano.puntuacion);
        } else {
            printf("Acción no válida. Debes elegir 'pedir' o 'plantar'.\n");
        }
    } else {
        printf("El jugador %s no está en sesión.\n", jugadorActual->nombre);
    }
}

