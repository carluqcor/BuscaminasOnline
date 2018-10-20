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
#include <iostream>
#include <fstream>


#define MSG_SIZE 250
#define MAX_CLIENTS 50


//SERVIDOR CHAT BUSCAMINAS

/*
 * El servidor ofrece el servicio de un chat
 */

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);



int main(){
  
	/*---------------------------------------------------- 
		Descriptor del socket y dato de datos                
	-----------------------------------------------------*/
	int sd, new_sd;
	struct sockaddr_in sockname, from;
	char dato[MSG_SIZE];
	socklen_t from_len;
    fd_set readfds, auxfds;
    int salida;
    int arrayClientes[MAX_CLIENTS];
    int numClientes = 0;
    bool registered=false, usuarioBool=false, passwordBool=false, registerBool=false;
    char *f;
    char *primeraPalabra, *opcionUsuario, *usuario, *opcionPassword, *password, *usuarioAux, *passwordAux;
    std::ifstream fich;
    std::ofstream fichAux;

    //contadores
    int i,j,k;
	int recibidos;
    char identificador[MSG_SIZE];
    
    int on, ret;

    
    
	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if(sd == -1){
		perror("No se puede abrir el socket cliente\n");
    		exit (1);	
	}
    
    // Activaremos una propiedad del socket que permitir· que otros
    // sockets puedan reutilizar cualquier puerto al que nos enlacemos.
    // Esto permitir· en protocolos como el TCP, poder ejecutar un
    // mismo programa varias veces seguidas y enlazarlo siempre al
    // mismo puerto. De lo contrario habrÌa que esperar a que el puerto
    // quedase disponible (TIME_WAIT en el caso de TCP)
    on=1;
    ret = setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));



	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2000);
	sockname.sin_addr.s_addr =  INADDR_ANY;

	if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1){
		perror("Error en la operación bind");
		exit(1);
	}
	

   	/*---------------------------------------------------------------------
		Del las peticiones que vamos a aceptar sólo necesitamos el 
		tamaño de su estructura, el resto de información (familia, puerto, 
		ip), nos la proporcionará el método que recibe las peticiones.
   	----------------------------------------------------------------------*/
		from_len = sizeof (from);


		if(listen(sd,1) == -1){
			perror("Error en la operación de listen");
			exit(1);
		}
    
    //Inicializar los conjuntos fd_set
    FD_ZERO(&readfds);
    FD_ZERO(&auxfds);
    FD_SET(sd,&readfds);
    FD_SET(0,&readfds);
    
   	
    //Capturamos la señal SIGINT (Ctrl+c)
    signal(SIGINT,manejador);
    
	/*-----------------------------------------------------------------------
		El servidor acepta una petición
	------------------------------------------------------------------------ */
		while(1){
            
            //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
            
            auxfds = readfds;
            
            salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
            
            if(salida > 0){
                
                
                for(i=0; i<FD_SETSIZE; i++){
                    
                    //Buscamos el socket por el que se ha establecido la comunicación
                    if(FD_ISSET(i, &auxfds)) {
                        
                        if( i == sd){
                            
                            if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
                                perror("Error aceptando peticiones");
                                exit(1);
                            }else{
                                if(numClientes < MAX_CLIENTS){ //Bienvenido al chat
                                    arrayClientes[numClientes] = new_sd;
                                    numClientes++;
                                    FD_SET(new_sd,&readfds);
                                
                                    strcpy(dato, "Bienvenido al chat\n");
                                
                                    send(new_sd,dato,strlen(dato),0);
                                    for(j=0; j<(numClientes);j++){
                                        bzero(dato,sizeof(dato));
                                        sprintf(dato, "Nuevo Cliente conectado: %d\n",new_sd);
                                        send(arrayClientes[j],dato,strlen(dato),0);
                                    }
                                }else{ //CHAT CON MAXIMO
                                    bzero(dato,sizeof(dato));
                                    strcpy(dato,"Demasiados clientes conectados\n");
                                    send(new_sd,dato,strlen(dato),0);
                                    close(new_sd);
                                }   
                            }              

                            bzero(dato, sizeof(dato));
                            int recibido = recvfrom (new_sd, &dato, sizeof(dato), 0,(struct sockaddr *) &from, &from_len);

                            /* -----------------------------------------------------------------
                                Comprobamos si hemos recibido alguna informacion 
                            -------------------------------------------------------------------*/
                            if (recibido > 0){  
                                primeraPalabra=strstr(dato, " ");

                                if((strcmp(primeraPalabra,"INICIAR-PARTIDA\n")==1) && ((usuarioBool==true) && (passwordBool==true)) || (registerBool==true)){
                                    bzero(dato,sizeof(dato));
                                    sprintf(dato, "Puede Jugar\n");
                                    send(arrayClientes[0],dato,strlen(dato),0);
                                }else{
                                    bzero(dato,sizeof(dato));
                                    sprintf(dato, "Debe registrarse o loguearse para poder jugar\n");
                                    send(arrayClientes[0],dato,strlen(dato),0);
                                }
                                if(strcmp(primeraPalabra,"USUARIO\n")==0){
                                    sscanf(dato, "%s %s", primeraPalabra, usuario);
                                }else if(strcmp(primeraPalabra,"PASSWORD\n")==0){
                                    sscanf(dato, "%s %s", primeraPalabra, password);
                                }else if(strcmp(primeraPalabra, "REGISTRO\n")==0){
                                    sscanf(dato, "%s %s %s %s %s", primeraPalabra, opcionUsuario, usuario, opcionPassword, password);
                                    registered=false;
                                    fich.open("BASEDEDATOS.txt");
                                    while(!fich.eof()){
                                        //fscanf(fich, "%s\t%s", usuarioAux, passwordAux);
                                        if(strcmp(usuarioAux,usuario)==0 && strcmp(passwordAux,password)==0){
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
                            }else{
                                std::cout<<"Error recibiendo datos del cliente\n";
                            }              
                        }else if (i == 0){
                            //Se ha introducido información de teclado
                            bzero(dato, sizeof(dato));
                            fgets(dato, sizeof(dato),stdin);
                            
                            //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
                            if(strcmp(dato,"SALIR\n") == 0){
                             
                                for (j = 0; j < numClientes; j++){
                                    send(arrayClientes[j], "Desconexion servidor\n", strlen("Desconexion servidor\n"),0);
                                    close(arrayClientes[j]);
                                    FD_CLR(arrayClientes[j],&readfds);
                                }
                                    close(sd);
                                    exit(-1);   
                            }                            
                        }else{
                            bzero(dato,sizeof(dato));
                            
                            recibidos = recv(i,dato,sizeof(dato),0);
                            
                            if(recibidos > 0){
                                
                                if(strcmp(dato,"SALIR\n") == 0){
                                    
                                    salirCliente(i,&readfds,&numClientes,arrayClientes);
                                    
                                }
                                else{
                                    
                                    sprintf(identificador,"%d: %s",i,dato);
                                    bzero(dato,sizeof(dato));
                                    strcpy(dato,identificador);
                                    
                                    for(j=0; j<numClientes; j++)
                                        if(arrayClientes[j] != i)
                                            send(arrayClientes[j],dato,strlen(dato),0);

                                    
                                }
                                                                
                                
                            }
                            //Si el cliente introdujo ctrl+c
                            if(recibidos== 0)
                            {
                                printf("El socket %d, ha introducido ctrl+c\n", i);
                                //Eliminar ese socket
                                salirCliente(i,&readfds,&numClientes,arrayClientes);
                            }
                        }
                    }
                }
            }
		}
	close(sd);
	return 0;
}

void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]){
  
    char dato[250];
    int j;
    
    close(socket);
    FD_CLR(socket,readfds);
    
    //Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes) - 1; j++)
        if (arrayClientes[j] == socket)
            break;
    for (; j < (*numClientes) - 1; j++)
        (arrayClientes[j] = arrayClientes[j+1]);
    
    (*numClientes)--;
    
    bzero(dato,sizeof(dato));
    sprintf(dato,"Desconexión del cliente: %d\n",socket);
    
    for(j=0; j<(*numClientes); j++)
        if(arrayClientes[j] != socket)
            send(arrayClientes[j],dato,strlen(dato),0);


}


void manejador (int signum){
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);
    
    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}
