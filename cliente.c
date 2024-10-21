#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>


int main(){
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int sd_cliente;
	struct sockaddr_in cliente;
	char buffer[250];
	socklen_t len_cliente;
    fd_set readfds, auxfds;
    int salida;
    int fin = 0;
    
	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd_cliente = socket (AF_INET, SOCK_STREAM, 0);
	if (sd_cliente == -1){
		perror("No se puede abrir el socket cliente\n");
    	exit (1);	
	}

	/* ------------------------------------------------------------------
		Se rellenan los campos de la estructura con la IP del 
		servidor y el puerto del servicio que solicitamos
	-------------------------------------------------------------------*/
	cliente.sin_family = AF_INET;
	cliente.sin_port = htons(2000);
	cliente.sin_addr.s_addr =  inet_addr("127.0.0.1");

	/* ------------------------------------------------------------------
		Se solicita la conexión con el servidor
	-------------------------------------------------------------------*/
	len_cliente = sizeof(cliente);
	
	if (connect(sd_cliente, (struct sockaddr *)&cliente, len_cliente) == -1){
		perror ("Error de conexión");
		exit(1);
	}
    
    //Inicializamos las estructuras
    FD_ZERO(&auxfds);
    FD_ZERO(&readfds);
    
    FD_SET(0, &readfds); // 0 --> sd_cliente
    FD_SET(sd_cliente, &readfds);

    printf("Opciones del servidor:\n");
    printf("\tUSUARIO usuario\n");
    printf("\tPASSWORD contraseña\n");
    printf("\tREGISTRO -u usuario -p password\n");
    printf("\tINICIAR-PARTIDA\n");
    printf("\tPEDIR-CARTA\n");
    printf("\tPLANTARME\n");
    printf("\tSALIR\n");
    
	/* ------------------------------------------------------------------
		Se transmite la información
	-------------------------------------------------------------------*/
	do{
        auxfds = readfds;
        salida = select(sd_cliente+1,&auxfds,NULL,NULL,NULL);
        
        //Tengo mensaje desde el servidor
        if(FD_ISSET(sd_cliente, &auxfds)){
            bzero(buffer, sizeof(buffer));
            recv(sd_cliente, buffer, sizeof(buffer), 0);
            
            printf("\n%s\n", buffer);
            
            if(strcmp(buffer, "Demasiados clientes conectados\n") == 0)
                fin =1;
            
            if(strcmp(buffer, "Desconexión servidor\n") == 0)
                fin =1;
            
        }
        else{
            //He introducido información por teclado
            if(FD_ISSET(0,&auxfds)){
                bzero(buffer, sizeof(buffer));
                
                fgets(buffer, sizeof(buffer), stdin);
                
                if(strcmp(buffer,"SALIR\n") == 0){
                    fin = 1;
                }
                
                send(sd_cliente, buffer, sizeof(buffer), 0);
            }
        }		
    }while(fin == 0);
		
    close(sd_cliente);

    return 0;
}
