#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

/* --------------------------------------------------------------------------------------
 
 Envía un número aleatorio al servidor, quien el devuelve el número incremetnado

---------------------------------------------------------------------------------------- */
 
int main ( ){
  
	/*---------------------------------------------------- 
		Descriptor del socket y buffer para datos 
	-----------------------------------------------------*/
	int Socket_Cliente;
	char Datos[100];


   	/* -----------------------------------------------------
   		Informaci\ufffdn del Servidor
   	-----------------------------------------------------*/
   	struct sockaddr_in Servidor;  
   	socklen_t Longitud_Servidor;


		/* --------------------------------------------------
		Se abre el socket cliente
	---------------------------------------------------*/
	Socket_Cliente = socket (AF_INET, SOCK_DGRAM, 0);
	if (Socket_Cliente == -1){
		printf ("No se puede abrir el socket cliente\n");
		exit (-1);	
	}

		/*---------------------------------------------------------------------
		Necesitamos una estructura con la informaci\ufffdn del Servidor
		para poder solicitarle un servicio.
		----------------------------------------------------------------------*/
	Servidor.sin_family = AF_INET;
	Servidor.sin_port = htons(2000);
	Servidor.sin_addr.s_addr =  inet_addr("127.0.0.1");
	Longitud_Servidor = sizeof(Servidor);


	printf("Que desea pedir? (INICIAR-PARTIDA, TIME , DAYTIME)\n"); //BLA BLA BLA
	
	fgets(Datos,sizeof(Datos),stdin);



	
	/*-----------------------------------------------------------------------
		Se env\ufffda mensaje al Servidor
	-----------------------------------------------------------------------*/
	int enviado = sendto (Socket_Cliente, Datos, sizeof(Datos), 0,
	(struct sockaddr *) &Servidor, Longitud_Servidor);
   
   	if(enviado < 0){
			printf("Error al solicitar el servicio\n");
	}else{
   				
	   	struct timeval timeout;
		fd_set lectura;
		int salida;
		
		//Inicializar la estructua timeval
		
		for(int i = 0; i < 3; ++i){
			/* code */
			timeout.tv_sec = 5;
			timeout.tv_usec = 0;
			
			//Inicializar los conjuntos fd_set
			FD_ZERO(&lectura);
			FD_SET(Socket_Cliente,&lectura);
			
			
			salida = select(Socket_Cliente+1,&lectura,NULL,NULL,&timeout);
			
			if(salida == -1){
				printf("Se ha producido un error en select\n");
				
		}
		else if(salida == 0){
			printf("Se ha agotado el tiempo\n");
			}else{
				/*----------------------------------------------------------------------
					Esperamos la respuesta del Servidor
					----------------------------------------------------------------------- */ 
					printf("DATOS: %s", Datos);
					char c[80];
					int recibido = recvfrom (Socket_Cliente, c, sizeof(c), 0,
					(struct sockaddr *) &Servidor, &Longitud_Servidor);
					
		   		if (recibido > 0)
		      		printf ("Leido %s\n", c);
		   		else
		      		printf ("Error al leer del servidor\n");
				
				
				
				close(Socket_Cliente);

			return 0;
			}
		}
	}
}

