#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>

#define MSG_SIZE 250
#define MAX_PLAYERS 10

/*
 * El servidor ofrece el servicio de JUGAR AL BUSCAMINAS
 */
 
int main (){
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int Socket_Servidor;
	struct sockaddr_in Servidor; //sockname
	char dato[MSG_SIZE]; //buffer
	bool registered=false;
	std::string f;
	std::string primeraPalabra, opcionUsuario, usuario, opcionPassword, password;

	int sd, new_sd;
   	struct sockaddr_in Cliente; //from
	socklen_t Longitud_Cliente; //from
    fd_set readfds, auxfds;
    int salida;
    int arrayClientes[MAX_PLAYERS];
    int numClientes = 0;

    //contadores
    int i,j,k;
	int recibidos;
    char identificador[MSG_SIZE];
    
    int on, ret;
	
	/* --------------------------------------------------
		Se abre el socket Servidor
	---------------------------------------------------*/
	Socket_Servidor = socket (AF_INET, SOCK_DGRAM, 0);
	if (Socket_Servidor == -1){
		printf ("No se puede abrir socket servidor\n");
		exit (-1);	
	}

	/* ------------------------------------------------------------------
		Se rellenan los campos de la estructura servidor, necesaria
		para la llamada a la funcion bind().
	-------------------------------------------------------------------*/
	Servidor.sin_family = AF_INET;
	Servidor.sin_port = htons(2000);
	Servidor.sin_addr.s_addr = htonl(INADDR_ANY); 

	if (bind (Socket_Servidor, (struct sockaddr *)&Servidor, sizeof (Servidor)) == -1)
	{
		close (Socket_Servidor);
		exit (-1);
	}
	

  	/*---------------------------------------------------------------------
		Del cliente s\ufffdlo necesitamos el tama\ufffdo de su estructura, el 
		resto de informaci\ufffdn (familia, puerto, ip), la obtendremos 
		nos la proporcionar\ufffd el propio m\ufffdtodo recvfrom, cuando
		recibamos la llamada de un cliente.
   ----------------------------------------------------------------------*/
	Longitud_Cliente = sizeof (Cliente);

   	
	/*-----------------------------------------------------------------------
		El servidor espera continuamente los mensajes de los clientes
	------------------------------------------------------------------------ */
	while (1)
	{
    /* -----------------------------------------------------------------
			Esperamos la llamada de alg\ufffdn cliente
		-------------------------------------------------------------------*/
      	int recibido = recvfrom (Socket_Servidor, &dato, sizeof(dato), 0,
			(struct sockaddr *) &Cliente, &Longitud_Cliente);

		/* -----------------------------------------------------------------
			Comprobamos si hemos recibido alguna informaci\ufffdn 
		-------------------------------------------------------------------*/
		if (recibido > 0){	
		/*-----------------------------------------------------------------
				Incrementamos el valor que nos ha enviado el cliente 
				------------------------------------------------------------------*/
	      	printf("Recibido %s\n", dato);

	      	//if por switch

	      	primeraPalabra=strstr(dato, " ");

	      	if((strcmp(primeraPalabra=="INICIAR-PARTIDA\n")==0) && ((usuarioBool==true) && (passwordBool==true)) || (registerBool==true)){

	      	}else{
	      		std::cout<<"Debe registrarse o loguearse para poder jugar\n";
	      	}

	      	else if(strcmp(primeraPalabra=="USUARIO\n")==0){
				sscanf(dato, "%s %s", primeraPalabra, usuario);
	      	}

	      	else if(strcmp(primeraPalabra=="PASSWORD\n")==0){
	      		sscanf(dato, "%s %s", primeraPalabra, password);
	      	}

	      	else if(strcmp(primeraPalabra=="REGISTRO\n")==0){
	      		sscanf(dato, "%s %s %s %s %s", primeraPalabra, opcionUsuario, usuario, opcionPassword, password);
	      		registered=false;
				std::ifstream fich;
				std::ofstream fichAux;
				fich.open("BASEDEDATOS.txt");
				while(!fich.eof()){
					fscanf(fich, "%s\t%s", usuarioAux, passwordAux);
					if(strcmp(usuarioAux,usuario)== && passwordAux==password){
						std::cout<<"Usuario ya registrado\n";
						registered==true;
						fich.close();
					}
				}	
				if(registered==false){
					fichAux.open("BASEDEDATOS.txt");
					fichAux<<usuario<<"\t"<<password<<"\n";
					fichAux.close();
				}		
			}
	      	
	      	else if(strcmp(primeraPalabra=="DESCUBRIR\n")==0){}

	      	else if(strcmp(primeraPalabra=="PONER-BANDERA\n")==0){}

	      	else if(strcmp(primeraPalabra=="SALIR\n")==0){}
      	else{}
	      	      
      	/* ------------------------------------------------------------------
				Devolvemos el n\ufffdmero incrementado al cliente
				--------------------------------------------------------------------*/
			int enviado = sendto (Socket_Servidor, &dato, sizeof(dato), 0,
			(struct sockaddr *) &Cliente, Longitud_Cliente);

		}

    }
	 
	 close(Socket_Servidor);

	return 0;
}
