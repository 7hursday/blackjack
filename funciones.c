#include "funciones.h"
#include <stdlib.h>


void registro(char buffer[250], int new_sd){
    char usuario[50];
    char password[50];
    char *token;
    int correcto = 0;
    char msg[250];
    // format: REGISTRO -u <usuario> -p <password>
    token = strtok(buffer, " ");
    token = strtok(NULL, " ");
    if (token != NULL)
    {
        if (strcmp(token, "-u") == 0)
        {
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                if (buscarUsuario(token) != NULL)
                {
                    strcpy(msg, "-Err. Usuario ya registrado\n");
                    send(new_sd, msg, sizeof(msg), 0);
                    return;
                }
                strcpy(usuario, token);
            }
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                if (strcmp(token, "-p") == 0)
                {
                    token = strtok(NULL, " ");
                    if (token != NULL)
                    {
                        strcpy(password, token);
                        correcto = 1;
                        strcpy(msg, "+OK. Usuario registrado correctamente\n");
                        send(new_sd, msg, sizeof(msg), 0);  
                    }
                }
            }
        }
    }
 
    if (correcto == 1)
    {
        printf("fichero\n");
        FILE *fichero = fopen("usuarios.txt", "a");
        if(fichero == NULL)
        {
            printf("Error al abrir el fichero\n");
            exit(-1);
        }
        fprintf(fichero, "%s %s", usuario, password);

        fclose(fichero);
    }
    else 
    {
        strcpy(msg, "-Err. Comando incorrecto\n");
        send(new_sd, msg, sizeof(msg), 0);
    }
}
int registrarFichero(char buffer[],Jugador *cliente)
{
    printf ("h\n");
    char r[MSG_SIZE];
    char u[MSG_SIZE];
    char p[MSG_SIZE];
    int encontrado;
    FILE *ficherosalida;

    encontrado=comprobrfichero(cliente);
    sscanf(buffer,"%s %s %s %s %s",r,u,cliente->nombre,p,cliente->contrasena);

    if(encontrado==0)
    {
        if((ficherosalida=fopen("usuariosycontraseñas.txt","a"))==NULL){
        printf ("Error al abrir el fichero <ususariosycontraseñas>\n");
        }
        fprintf(ficherosalida,"%s %s\n",cliente->nombre,cliente->contrasena);
        fclose(ficherosalida);
    }   
    else {
        printf("Usuario ya registrado.\n");
    }
    return encontrado;
}
int comprobrfichero(Jugador *cliente)
{
    FILE *ficherosalida; 
    char cad[MSG_SIZE];
    char usuariofichero[MSG_SIZE];
    char contrasenafichero[MSG_SIZE];
    int encontrado=0;

if ((ficherosalida = fopen("usuariosycontraseñas.txt", "r")) == NULL) {
        printf("Error al abrir el fichero <usuariosycontraseñas>\n");
        return 0;  // No se encontró el usuario si no se puede abrir el fichero
    }

    // Leer el fichero línea por línea
    while ((fgets(cad, MSG_SIZE, ficherosalida)) != NULL) {
        sscanf(cad, "%s %s", usuariofichero, contrasenafichero);

        // Mostrar lo leído del fichero para depuración
        printf("Leído del fichero: usuario = %s, contraseña = %s\n", usuariofichero, contrasenafichero);

        // Si el usuario y la contraseña coinciden, se marca como encontrado
        if (strcmp(cliente->nombre ,usuariofichero) == 0 && strcmp(cliente->contrasena, contrasenafichero) == 0) {
            encontrado = 1;
            break;
        }
    }

    fclose(ficherosalida);
    return encontrado;
}
int comprobarusuario(char buffer[],Jugador *cliente)
{
    FILE *ficherosalida; 
    char cad[MSG_SIZE];
    char usuariofichero[MSG_SIZE];
    char contrasenafichero[MSG_SIZE];
    int encontrado=0;
    char p[MSG_SIZE];
   printf("Hola\n");
     sscanf(buffer,"%s %s",p,cliente->nombre);

    if ((ficherosalida = fopen("usuariosycontraseñas.txt", "r")) == NULL) {
        printf("Error al abrir el fichero <usuariosycontraseñas>\n");
        return 0;  // No se encontró el usuario si no se puede abrir el fichero
    }
        while ((fgets(cad, MSG_SIZE, ficherosalida)) != NULL) {
        sscanf(cad, "%s %s", usuariofichero, contrasenafichero);

        
        

        // Si el usuario y la contraseña coinciden, se marca como encontrado
        if (strcmp(cliente->nombre, usuariofichero) == 0) {
            encontrado = 1;
            printf ("Usuario encontrado %s",usuariofichero);
            break;
        }
    }
   
    fclose(ficherosalida);
    return encontrado;
}
void comprobarcontrasena(char buffer[],Jugador *cliente)
{
    FILE *ficherosalida; 
    char cad[MSG_SIZE]; 
    char usuariofichero[MSG_SIZE];
    char contrasenafichero[MSG_SIZE];
    cliente->sesionIniciada=0;
    cliente->estado=0;

   printf("%s\n",cliente->nombre);
    char p[MSG_SIZE];
   
        sscanf(buffer,"%s %s",p,cliente->contrasena);
        if ((ficherosalida = fopen("usuariosycontraseñas.txt", "r")) == NULL) {
        printf("Error al abrir el fichero <usuariosycontraseñas>\n");
        exit(-1); // No se encontró el usuario si no se puede abrir el fichero
    }

    // Leer el fichero línea por línea
    while ((fgets(cad, MSG_SIZE, ficherosalida)) != NULL) {
        sscanf(cad, "%s %s", usuariofichero, contrasenafichero);

        // Mostrar lo leído del fichero para depuración
        printf("Leído del fichero: usuario = %s, contraseña = %s\n", usuariofichero, contrasenafichero);

        // Si el usuario y la contraseña coinciden, se marca como encontrado
        if (strcmp(cliente->nombre ,usuariofichero) == 0 && strcmp(cliente->contrasena, contrasenafichero) == 0) {
        
            cliente->sesionIniciada=1;
            cliente->estado=1;
           
            break;

        }
    }
}
char* buscarUsuario(char* usuario)
{
    char usuarioF[50];
    char* password;  

    size_t len = strlen(usuario);
    if (usuario[len - 1] == '\n') {usuario[len - 1] = '\0';}

    FILE* ficheroAux = fopen("usuarios.txt", "a");
    if (ficheroAux == NULL)
    {
        printf("Fichero usuarios.txt no existe\n");
        exit(-1);
    }
    fclose(ficheroAux);

    FILE* fichero = fopen("usuarios.txt", "r");
    if (fichero == NULL)
    {
        printf("Fichero usuarios.txt no existe\n");
        exit(-1);
    }
    else
    {
        while (fscanf(fichero, "%s", usuarioF) != EOF)
        {
            password = malloc(50 * sizeof(char));
            fscanf(fichero, "%s", password);
            if (strcmp(usuario, usuarioF) == 0)
            {
                fclose(fichero);
                
                return password;
            }
            free(password);
        }
        fclose(fichero);

        return NULL;
    }
}

char* generarCarta(int* corazones, int* diamantes, int* treboles, int *picas, int i, struct Partida* partidaVector, int ordenPartida)
{
    int disponible = 0;
    char* carta;
    int palo;
    int numero;
    char msg[250];
    while(disponible == 0)
    {
        carta = malloc(250 * sizeof(char));
        palo = rand() % 4;
        numero = rand() % 13;
        switch(palo)
        {
            case 0:
                if(corazones[numero] == 0)
                {
                    corazones[numero] = 1;
                    disponible = 1;
                    sprintf(carta, "[CORAZONES, %d]", numero + 1);
                    strcpy(msg, carta);
                    // clientVector[indice].puntuacion += numero;
                    // send(i, msg, sizeof(msg), 0);
                    // free(carta);
                }
            break;
            case 1:
                if(diamantes[numero] == 0)
                {
                    diamantes[numero] = 1;
                    disponible = 1;
                    sprintf(carta, "[DIAMANTES, %d]", numero + 1);
                    strcpy(msg, carta);
                    // clientVector[indice].puntuacion += numero;
                    // send(i, msg, sizeof(msg), 0);
                    // free(carta);
                }
            break;
            case 2:
                if(treboles[numero] == 0)
                {
                    treboles[numero] = 1;
                    disponible = 1;
                    sprintf(carta, "[TREBOLES, %d]", numero + 1);
                    strcpy(msg, carta);
                    // clientVector[indice].puntuacion += numero;
                    // send(i, msg, sizeof(msg), 0);
                    // free(carta);
                }
            break;
            case 3:
                if(picas[numero] == 0)
                {
                    picas[numero] = 1;
                    disponible = 1;
                    sprintf(carta, "[PICAS, %d]", numero + 1);
                    strcpy(msg, carta);
                    // clientVector[indice].puntuacion += numero;
                    // send(i, msg, sizeof(msg), 0);
                    // free(carta);
                }
            break;
        }            
    }

    if(partidaVector[ordenPartida].jugador1.sd == i)
    {
        partidaVector[ordenPartida].jugador1.puntuacion += (numero + 1);
    }
    else if(partidaVector[ordenPartida].jugador2.sd == i)
    {
        partidaVector[ordenPartida].jugador2.puntuacion += (numero + 1);
    }

    return carta;
}

int buscarCliente(struct Cliente *clientVector, int new_sd){
    for(int i = 0; i < 30; i++){
        if(clientVector[i].sd == new_sd)
        {
            return i;
        }
    }
    return -1;
}
