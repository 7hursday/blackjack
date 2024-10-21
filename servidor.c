#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

#include "funciones.h"

#define MSG_SIZE 250
#define MAX_CLIENTS 30

/*
 * El servidor ofrece el servicio de un chat
 */

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);



int main(){
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int sd_servidor, new_sd;
	struct sockaddr_in servidor, from;
	char buffer[MSG_SIZE];
	socklen_t from_len;
    fd_set readfds, auxfds;
   	int salida;
   	int arrayClientes[MAX_CLIENTS];
    int numClientes = 0;
    char* user = malloc(50 * sizeof(char));
    

   	//contadores
    int i, j, k;
	int recibidos;
   	char identificador[MSG_SIZE];
    int on, ret;
    //struct jugador 
        Jugador *usuario=malloc(MAX_CLIENTS*sizeof(Jugador));
        Jugador *Registro=malloc(MAX_CLIENTS*sizeof(Jugador));
        
    //Vector de Struct Cliente
    struct Cliente clientVector[30];
    int ordenCliente = 0;

    for(int i = 0; i < 30; i++)
    {
        clientVector[i].puntuacion = 0;
    }

    //Vector de Struct Partida
    struct Partida partidaVector[10];
    int ordenPartida = -1;

    //inicio de sesion
    int inicioSesion = 0;

    int puntos1 = 0;
    int puntos2 = 0;
    char user1[50];
    char user2[50];

    int plantados = 0;
    
    //baraja de cartas
    int *corazones = (int *)calloc(13, sizeof(int));
    int *diamantes = (int *)calloc(13, sizeof(int));
    int *treboles = (int *)calloc(13, sizeof(int));
    int *picas = (int *)calloc(13, sizeof(int));
    srand(time(NULL));

    char* carta;
    char* carta1;
    char* carta2;
    char* carta3;
    char* carta4;
    char msg[250];

    if(corazones == NULL || diamantes == NULL || treboles == NULL || picas == NULL) 
    {
        perror("Error al asignar memoria para los vectores de la baraja");
        exit(1);
    }

    for (int i = 0; i < 10; i++)
    {
        partidaVector[i].numJugadores = 0;
    }
	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd_servidor = socket(AF_INET, SOCK_STREAM, 0);
	if(sd_servidor == -1){
		perror("No se puede abrir el socket cliente\n");
    	exit (1);	
	}
    
    // Activaremos una propiedad del socket para permitir· que otros
    // sockets puedan reutilizar cualquier puerto al que nos enlacemos.
    // Esto permite· en protocolos como el TCP, poder ejecutar un
    // mismo programa varias veces seguidas y enlazarlo siempre al
   	// mismo puerto. De lo contrario habrÌa que esperar a que el puerto
    // quedase disponible (TIME_WAIT en el caso de TCP)
    on=1;
    ret = setsockopt( sd_servidor, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	servidor.sin_family = AF_INET;
	servidor.sin_port = htons(2000);
	servidor.sin_addr.s_addr =  INADDR_ANY;

	if(bind (sd_servidor, (struct sockaddr *) &servidor, sizeof (servidor)) == -1){
		perror("Error en la operación bind");
		exit(1);
	}
	
   	/*---------------------------------------------------------------------
		Del las peticiones que vamos a aceptar sólo necesitamos el 
		tamaño de su estructura, el resto de información (familia, puerto, 
		ip), nos la proporcionará el método que recibe las peticiones.
   	----------------------------------------------------------------------*/
	from_len = sizeof (from);

	if(listen(sd_servidor, 1) == -1){
		perror("Error en la operación de listen");
		exit(1);
	}

	//Inicializar los conjuntos fd_set
    printf("El servidor está esperando conexiones...\n");	
    	
	FD_ZERO(&readfds);
    FD_ZERO(&auxfds);
    FD_SET(sd_servidor,&readfds);
    FD_SET(0,&readfds);
    
    //Capturamos la señal SIGINT (Ctrl+c)
    signal(SIGINT, manejador);
    
	/*-----------------------------------------------------------------------
		El servidor acepta una petición
	------------------------------------------------------------------------ */
	while(1){
        //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
            
        auxfds = readfds;
            
        salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
            
        if(salida > 0){
                for(i=0; i < FD_SETSIZE; i++){
                    //Buscamos el socket por el que se ha establecido la comunicación
                    if(FD_ISSET(i, &auxfds)){
                        if(i == sd_servidor){
                            if((new_sd = accept(sd_servidor, (struct sockaddr *)&from, &from_len)) == -1){
                                perror("Error aceptando peticiones");
                            }
                            else{
                                if(numClientes < MAX_CLIENTS){
                                    clientVector[numClientes].sd = new_sd;
                                    usuario[numClientes].id=new_sd;
                                    // arrayClientes[numClientes] = new_sd;
                                    numClientes++;
                                    FD_SET(new_sd, &readfds);
                                
                                    strcpy(buffer, "+Ok. Usuario conectado\n");
                                
                                    send(new_sd, buffer, sizeof(buffer), 0);
                                
                                    for(j=0; j<(numClientes-1);j++){
                                        bzero(buffer,sizeof(buffer));
                                        sprintf(buffer, "Nuevo Cliente conectado en <%d>", new_sd);
                                        send(arrayClientes[j], buffer, sizeof(buffer), 0);
                                    }
                                }
                                else{
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer, "Demasiados clientes conectados\n");
                                    send(new_sd, buffer, sizeof(buffer), 0);
                                    close(new_sd);
                                }
                            }
                        }
                        else if(i == 0){
                            //Se ha introducido información de teclado
                            bzero(buffer, sizeof(buffer));
                            fgets(buffer, sizeof(buffer), stdin);

                            //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
                            if(strcmp(buffer,"SALIR\n") == 0){
                                for (j = 0; j < numClientes; j++){
                                    bzero(buffer, sizeof(buffer));
                                    strcpy(buffer,"Desconexión servidor\n"); 
                                    send(arrayClientes[j],buffer , sizeof(buffer),0);
                                    close(arrayClientes[j]);
                                    FD_CLR(arrayClientes[j],&readfds);
                                }

                                close(sd_servidor);
                                exit(-1);
                            }
                            //Mensajes que se quieran mandar a los clientes (implementar)
                            
                        } 
                        else{
                            bzero(buffer,sizeof(buffer));
                            
                            recibidos = recv(i, buffer, sizeof(buffer), 0);
                            
                            if(recibidos > 0)
                            {
                                if(strncmp(buffer,"REGISTRO ",9) == 0){
                                int econtradoRegistro=registrarFichero(buffer,Registro);
                                if(econtradoRegistro==0)
                                {
                                    
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,"Usuario registrado");
                                    send(i,buffer,sizeof(buffer),0);
                                 
                                   
                                }
                                else {
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,"Usuario ya registrado con anterioridad\n");
                                    send(i,buffer,sizeof(buffer),0);
                                }
                                    
                                
                            }

                            for(int nm=0;nm<numClientes;nm++){
                            
                                if(usuario[nm].id==i && clientVector[nm].sd==i){
                            //Usuario
                             if(strncmp(buffer,"USUARIO ",8) == 0){
                                
                              int  econtradoUsuario = comprobarusuario(buffer,&(usuario[nm]));
                               printf("%i\n",econtradoUsuario);
                                if(econtradoUsuario==1)
                                {
                                    
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,"Usuario encontrado,Inserte la contraseña\n");
                                    send(i,buffer,sizeof(buffer),0);
                                    recv(i,buffer,sizeof(buffer),0);

                                }
                                else if(econtradoUsuario==0){
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,"Usuario no encontrado\n");
                                    send(i,buffer,sizeof(buffer),0);
                                }
            
                               

                             }
                              if(strncmp(buffer,"PASSWORD ",8) == 0){
                                
                               comprobarcontrasena(buffer,&(usuario[nm]));

                                printf("%s",usuario[nm].nombre);

                                printf("%i\n",usuario[nm].sesionIniciada);
                                if(usuario[nm].sesionIniciada==1)
                                {
                                    
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,"COntraseña correcta\n");
                                    send(i,buffer,sizeof(buffer),0);
                                   printf("%s,%s\n",usuario[nm].nombre,usuario[nm].contrasena);
                                      struct Cliente cliente;
                                            strcpy(cliente.usuario, usuario[nm].nombre);
                                            cliente.puntuacion = 0;
                                            cliente.esperando = 0;
                                            // cliente.inicioSesion = 1;
                                            clientVector[ordenCliente] = cliente;
                                            inicioSesion=2;
                                            strcpy(buffer, "+OK. Usuario validado\n");
                                            send(i, buffer, sizeof(buffer), 0);
                        
                                            ordenCliente++;

                                }
                                else{
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,"Contraseña incorrecta\n");
                                    send(i,buffer,sizeof(buffer),0);
                                }
                              }
                                                   

                               }
                                }

                                if(strstr(buffer,"INICIAR-PARTIDA") != NULL)
                                {
                                    if(inicioSesion != 2)
                                    {
                                        strcpy(buffer, "-Err. Necesitas iniciar sesion antes\n");
                                        send(i, buffer, sizeof(buffer), 0); 
                                    }
                                    else{
                                        inicioSesion = 3;
                                        int partidaIniciada = 0;
                                        // char user1[50];
                                        // char user2[50];
                                        for (int j = 0; j < 10; j++)
                                        {
                                            if(partidaVector[j].numJugadores == 0)
                                            {
                                                partidaVector[j].jugador1 = clientVector[ordenCliente-1];
                                                partidaVector[j].jugador1.esperando = 1;
                                                partidaVector[j].jugador1.sd = i;
                                                partidaVector[j].jugador1.pierde = 0;
                                                // strcpy(user1, partidaVector[j].jugador1.usuario);
                                                partidaVector[j].numJugadores++;

                                                printf("j = %d\n", j);
                                                printf("ordenPartida = %d\n", ordenPartida);
                                                carta1 = generarCarta(corazones, diamantes, treboles, picas, i, partidaVector, ordenPartida+1);
                                                carta2 = generarCarta(corazones, diamantes, treboles, picas, i, partidaVector, ordenPartida+1);

                                                break;
                                            }
                                            else if(partidaVector[j].numJugadores == 1)
                                            {
                                                partidaVector[j].jugador2 = clientVector[ordenCliente-1];
                                                partidaVector[j].jugador2.esperando = 1;
                                                partidaVector[j].jugador2.sd = i;
                                                partidaVector[j].jugador1.pierde = 0;
                                                // strcpy(user2, partidaVector[i].jugador2.usuario);
                                                partidaVector[j].numJugadores++;
                                                partidaIniciada = 1;

                                                printf("j = %d\n", j);
                                                printf("ordenPartida = %d\n", ordenPartida);
                                                carta3 = generarCarta(corazones, diamantes, treboles, picas, i, partidaVector, ordenPartida+1);
                                                carta4 = generarCarta(corazones, diamantes, treboles, picas, i, partidaVector, ordenPartida+1);

                                                break;
                                            }
                                        }

                                        if(partidaIniciada == 1)
                                        {
                                            strcpy(buffer, "+OK. Empieza la partida\n");
                                            send(i, buffer, sizeof(buffer), 0);
                                            ordenPartida++;

                                            printf("puntos j1 (inicio) = %d\n", partidaVector[ordenPartida].jugador1.puntuacion);
                                            printf("puntos j2 (inicio) = %d\n", partidaVector[ordenPartida].jugador2.puntuacion);

                                            sprintf(msg, "TUS-CARTAS: %s %s. OPONENTE: %s\n", carta1, carta2, carta3);
                                            send(partidaVector[ordenPartida].jugador1.sd, msg, sizeof(msg), 0);
                                            printf("puntos j1 (iniciada) = %d\n", partidaVector[ordenPartida].jugador1.puntuacion);

                                            sprintf(msg, "TUS-CARTAS: %s %s. OPONENTE: %s\n", carta3, carta4, carta1);
                                            send(partidaVector[ordenPartida].jugador2.sd, msg, sizeof(msg), 0);
                                            printf("puntos j2 (iniciada) = %d\n", partidaVector[ordenPartida].jugador2.puntuacion);

                                            partidaVector[ordenPartida].turno = 1;
                                        }
                                        else
                                        {
                                            strcpy(buffer, "+OK. Esperando otro jugador\n");
                                            send(i, buffer, sizeof(buffer), 0);
                                        }
                                    }
                                }

                                if(strstr(buffer,"PEDIR-CARTA") != NULL)
                                {
                                    if (inicioSesion != 3)
                                    {
                                        strcpy(buffer, "-Err. Necesitas iniciar partida antes\n");
                                        send(i, buffer, sizeof(buffer), 0);
                                    }
                                    else
                                    {
                                        if(partidaVector[ordenPartida].turno == 1 && partidaVector[ordenPartida].jugador1.sd == i)
                                        {
                                            if(partidaVector[ordenPartida].jugador1.pierde == 1)
                                            {
                                                strcpy(buffer, "-Err. Has excedido los 21 puntos. No puedes pedir más cartas\n");
                                                send(i, buffer, sizeof(buffer), 0);
                                            }
                                            else if(partidaVector[ordenPartida].jugador1.plantado == 1)
                                            {
                                                strcpy(buffer, "-Err. Te has plantado. No puedes pedir más cartas\n");
                                                send(i, buffer, sizeof(buffer), 0);
                                            }
                                            else
                                            {
                                                carta = generarCarta(corazones, diamantes, treboles, picas, i, partidaVector, ordenPartida);
                                                strcpy(msg, "+OK, ");
                                                strcat(msg, carta);
                                                strcat(msg, "\n");
                                                send(i, msg, sizeof(msg), 0);
                                                if(partidaVector[ordenPartida].jugador1.puntuacion >= 21){
                                                    partidaVector[ordenPartida].jugador1.pierde = 1;
                                                }
                                                printf("puntos j1 = %d\n", partidaVector[ordenPartida].jugador1.puntuacion);
                                            }
                                            if(partidaVector[ordenPartida].jugador2.plantado != 1)
                                            {
                                                partidaVector[ordenPartida].turno = 2;
                                            }
                                        }
                                        else if(partidaVector[ordenPartida].turno == 1 && partidaVector[ordenPartida].jugador2.sd == i)
                                        {
                                            if(partidaVector[ordenPartida].jugador2.plantado == 1)
                                            {
                                                strcpy(buffer, "-Err. Te has plantado. No puedes pedir más cartas\n");
                                                send(i, buffer, sizeof(buffer), 0);
                                            }
                                            else
                                            {
                                                strcpy(buffer, "-Err. No es tu turno\n");
                                                send(i, buffer, sizeof(buffer), 0);
                                            }
                                        }
                                        else if(partidaVector[ordenPartida].turno == 2 && partidaVector[ordenPartida].jugador2.sd == i)
                                        {
                                            if(partidaVector[ordenPartida].jugador2.pierde == 1)
                                            {
                                                strcpy(buffer, "-Err. Has excedido los 21 puntos. No puedes pedir más cartas\n");
                                                send(i, buffer, sizeof(buffer), 0);
                                            }
                                            else if(partidaVector[ordenPartida].jugador2.plantado == 1)
                                            {
                                                strcpy(buffer, "-Err. Te has plantado. No puedes pedir más cartas\n");
                                                send(i, buffer, sizeof(buffer), 0);
                                            }
                                            else
                                            {
                                                carta = generarCarta(corazones, diamantes, treboles, picas, i, partidaVector, ordenPartida);
                                                strcpy(msg, "+OK, ");
                                                strcat(msg, carta);
                                                strcat(msg, "\n");
                                                send(i, msg, sizeof(msg), 0);
                                                if(partidaVector[ordenPartida].jugador2.puntuacion >= 21){
                                                    partidaVector[ordenPartida].jugador2.pierde = 1;
                                                }
                                                printf("puntos j2 = %d\n", partidaVector[ordenPartida].jugador2.puntuacion);
                                            }
                                            if(partidaVector[ordenPartida].jugador1.plantado != 1)
                                            {
                                                partidaVector[ordenPartida].turno = 1;
                                            }
                                        }
                                        else if(partidaVector[ordenPartida].turno == 2 && partidaVector[ordenPartida].jugador1.sd == i)
                                        {
                                            if(partidaVector[ordenPartida].jugador1.plantado == 1)
                                            {
                                                strcpy(buffer, "-Err. Te has plantado. No puedes pedir más cartas\n");
                                                send(i, buffer, sizeof(buffer), 0);
                                            }
                                            else
                                            {
                                                strcpy(buffer, "-Err. No es tu turno\n");
                                                send(i, buffer, sizeof(buffer), 0);
                                            }
                                        }
                                    }
                                }

                                if(strstr(buffer,"PLANTARME") != NULL){
                                    if(partidaVector[ordenPartida].jugador1.sd == i)
                                    {
                                        partidaVector[ordenPartida].jugador1.plantado = 1;
                                        partidaVector[ordenPartida].turno = 2;
                                    }
                                    else if(partidaVector[ordenPartida].jugador2.sd)
                                    {
                                        partidaVector[ordenPartida].jugador2.plantado = 1;
                                        partidaVector[ordenPartida].turno = 1;
                                    }

                                    printf("plantados = %d\n", plantados);

                                    if(plantados == 0)
                                    {
                                        strcpy(buffer, "+OK. Esperando a que finalice el otro jugador\n");
                                        send(i, buffer, sizeof(buffer), 0);
                                    }
                                    else if(plantados == 1)
                                    {
                                        puntos1 = partidaVector[ordenPartida].jugador1.puntuacion;
                                        puntos2 = partidaVector[ordenPartida].jugador2.puntuacion;
                                        strcpy(user1, partidaVector[ordenPartida].jugador1.usuario);
                                        strcpy(user2, partidaVector[ordenPartida].jugador2.usuario);

                                        printf("sd 1 = %d\n", partidaVector[ordenPartida].jugador1.sd);
                                        printf("sd 2 = %d\n", partidaVector[ordenPartida].jugador2.sd);

                                        printf("puntos 1 = %d\n", puntos1);
                                        printf("puntos 2 = %d\n", puntos2);

                                        if(puntos1 > 21 && puntos2 > 21)
                                        {
                                            strcpy(buffer, "+OK. No hay ganadores. Ambos jugadores han excedido los 21 puntos\n");
                                            send(partidaVector[ordenPartida].jugador1.sd, buffer, sizeof(buffer), 0);
                                            send(partidaVector[ordenPartida].jugador2.sd, buffer, sizeof(buffer), 0);
                                        }

                                        if(puntos1 < 21 && puntos2 <21 && puntos1 == puntos2)
                                        {
                                            sprintf(buffer, "+OK. Jugador <%s> y jugador <%s> habéis empatado la partida\n", user1, user2);
                                            send(partidaVector[ordenPartida].jugador1.sd, buffer, sizeof(buffer), 0);
                                            send(partidaVector[ordenPartida].jugador2.sd, buffer, sizeof(buffer), 0);
                                        }

                                        if(puntos1 > puntos2 && puntos1 <= 21)
                                        {
                                            sprintf(buffer, "+OK. Jugador <%s> ha ganado la partida\n", user1);
                                            send(partidaVector[ordenPartida].jugador1.sd, buffer, sizeof(buffer), 0);
                                            send(partidaVector[ordenPartida].jugador2.sd, buffer, sizeof(buffer), 0);
                                        }
                                        else if(puntos2 > puntos1 && puntos2 <= 21)
                                        {
                                            sprintf(buffer, "+OK. Jugador <%s> ha ganado la partida\n", user2);
                                            send(partidaVector[ordenPartida].jugador1.sd, buffer, sizeof(buffer), 0);
                                            send(partidaVector[ordenPartida].jugador2.sd, buffer, sizeof(buffer), 0);
                                        }
                                        else if(puntos1 < puntos2 && puntos2 > 21)
                                        {
                                            sprintf(buffer, "+OK. Jugador <%s> ha ganado la partida\n", user1);
                                            send(partidaVector[ordenPartida].jugador1.sd, buffer, sizeof(buffer), 0);
                                            send(partidaVector[ordenPartida].jugador2.sd, buffer, sizeof(buffer), 0);
                                        }
                                        else if(puntos2 < puntos1 && puntos1 > 21)
                                        {
                                            sprintf(buffer, "+OK. Jugador <%s> ha ganado la partida\n", user2);
                                            send(partidaVector[ordenPartida].jugador1.sd, buffer, sizeof(buffer), 0);
                                            send(partidaVector[ordenPartida].jugador2.sd, buffer, sizeof(buffer), 0);
                                        }
                                    }
                                    plantados++;
                                    printf("plantados = %d\n", plantados);
                                }
                                
                                if(strcmp(buffer, "SALIR\n") == 0)
                                {
                                    salirCliente(i, &readfds, &numClientes, arrayClientes);
                                }
                                else
                                {
                                    sprintf(identificador, "<%d>: %s", i, buffer);
                                    bzero(buffer, sizeof(buffer));

                                    strcpy(buffer,identificador);

                                    printf("%s\n", buffer);

                                    for(j=0; j<numClientes; j++)
                                    {
                                        if(arrayClientes[j] != i)
                                        {
                                            send(arrayClientes[j], buffer, sizeof(buffer), 0);
                                        }
                                    }
                                }    
                            }

                            //Si el cliente introdujo ctrl+c
                            if(recibidos== 0){
                                printf("El socket %d, ha introducido ctrl+c\n", i);
                                //Eliminar ese socket
                                salirCliente(i, &readfds, &numClientes, arrayClientes);
                            }
                        }
                    }
                }
            }
		}

	close(sd_servidor);
	return 0;
	
}

void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]){
  
    char buffer[250];
    int j;
    
    close(socket);
    FD_CLR(socket, readfds);
    
    //Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes)-1; j++)
        if (arrayClientes[j] == socket)
            break;

    for (; j < (*numClientes) - 1; j++)
        (arrayClientes[j] = arrayClientes[j+1]);
    
    (*numClientes)--;
    
    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión del cliente <%d>",socket);
    
    for(j=0; j<(*numClientes); j++)
        if(arrayClientes[j] != socket)
            send(arrayClientes[j],buffer,sizeof(buffer),0);


}

void manejador (int signum){
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT, manejador);
    
    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}
