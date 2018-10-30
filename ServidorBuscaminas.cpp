#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "macros.hpp"
#include "buscaminas.hpp"

#define MSG_SIZE 250
#define MAX_CLIENTS 30

struct clients{
        char user[50];
        char password[50];
        char letra[1];
        char numero[1];
        int socket;
        int estado;
        int login;
        int turno; 
        int partida=-1;
        int AB; ///0 para A y 1 para B
};

std::vector<clients>cola;

std::vector<Buscaminas> juegos;


//SERVIDOR CHAT BUSCAMINAS

/*
 * El servidor ofrece el servicio de un chat
 */

void manejador(int signum);
void salirCliente(int socket, fd_set *readfds, int &numClientes, struct clients *cliente);
bool registrado(char *nombre, char *password);
void jugar(clients a, clients b);

int main(){
    
	/*---------------------------------------------------- 
		Descriptor del socket y dato de datos                
	-----------------------------------------------------*/
	int sd, new_sd;
	struct sockaddr_in sockname, from;
    double size;
	char dato[MSG_SIZE];
	socklen_t from_len;
    fd_set readfds, auxfds;
    int salida;
    //int clientes[MAX_CLIENTS];
    int numClientes = 0;
    bool registerBool=false, registered=false;
    char *f;
    char *primeraPalabra, *usuario, *pass;
    std::string usuarioAux, passwordAux, g;

    std::ifstream fichero;
    std::ofstream ficheroAux;
    clients clientes[MAX_CLIENTS];
    clients clientesAux[MAX_CLIENTS];
    //contadores
    int i,j,k, id=0, oper1, oper2, verificacion=0;
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
		std::cout << CLEAR_SCREEN;
        PLACE(1,0);
        while(1){
            
            //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
            
            auxfds = readfds;
            
            salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
            if(salida > 0){
                
                
                for(i=0; i<FD_SETSIZE; i++){
                    
                    //Buscamos el socket por el que se ha establecido la comunicación
                    if(FD_ISSET(i, &auxfds)){ 
                        if(i == sd){      
                            if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
                                perror("Error aceptando peticiones");
                                exit(1);
                            }else{
                                if(numClientes < MAX_CLIENTS){ //Bienvenido al chat
                                    numClientes++;
                                    clientes[numClientes].socket=new_sd;
                                    clientes[numClientes].estado=0;
                                    clientes[numClientes].login=0;
                                    strcpy(clientes[numClientes].user, "0");
                                    strcpy(clientes[numClientes].password,"0");
                                    clientes[numClientes].turno=1;

                                    //std::cout<<"Cliente socket: "<<clientes[numClientes].socket<<std::endl;
                                    FD_SET(new_sd,&readfds);
                                    
                                    strcpy(dato, "+Ok. Usuario conectado\n");
                                    printf("Cliente conectado\n\n");
                                    send(new_sd,dato,strlen(dato),0);
                                    //for(j=0; j<(numClientes);j++){
                                    bzero(dato,sizeof(dato));
                                    //sprintf(dato, "Nuevo Cliente conectado: %d\n",new_sd);
                                    send(clientes[numClientes].socket,dato,strlen(dato),0);
                                   // }
                                }else{ //CHAT CON MAXIMO
                                    bzero(dato,sizeof(dato));
                                    strcpy(dato,"-Err. Demasiados clientes conectados\n");
                                    send(new_sd,dato,strlen(dato),0);
                                    close(new_sd);
                                }   
                            }                     
                        }else if (i == 0){
                            //Se ha introducido información de teclado
                            bzero(dato, sizeof(dato));
                            fgets(dato, sizeof(dato),stdin);
                        
                        }else{
                            for(j=0;j<(MAX_CLIENTS);j++){
                                if(clientes[j].socket==i){
                                    bzero(dato,sizeof(dato));
                                    recibidos = recv(clientes[j].socket,dato,sizeof(dato),0);   
                                    //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
                                    /* -----------------------------------------------------------------
                                     Comprobamos si hemos recibido alguna informacion 
                                    -------------------------------------------------------------------*/
                                    std::string value;
                                    std::istringstream buffer;
                                    std::cout<<BIBLUE<<"Se recibe del cliente "<<BIYELLOW<<clientes[j].socket<<RESET<<": "<<dato<<std::endl;
                                    if(recibidos > 0){
                                        if((strncmp(dato,"INICIAR-PARTIDA", 15)==0) && (registrado(clientes[j].user, clientes[j].password)) && (clientes[j].login==1) && verificacion==2){
                                            bzero(dato,sizeof(dato));
                                            strcpy(dato, "+Ok. Ha sido introducido a la cola de espera\n");
                                            send(clientes[j].socket,dato,sizeof(dato),0);
                                            cola.push_back(clientes[j]);
                                            /*Recoger el tamaño de la cola, y meter en una nueva estructura, los dos ultimos clientes
                                            mientras que el size de la cola sea mayor que 1
                                            Si hay 4 jugadores, se guardarán 2 clientes con el mismo id de partida
                                            y los otros dos con id++*/
                                            std::cout<<"Size: "<<cola.size()<<std::endl;
                                            if(cola.size()>1){
                                                size=(int)cola.size();
                                                oper1=(size-size);
                                                oper2=(size-(size-1));
                                                clientesAux[oper1]=(cola.back());
                                                cola.pop_back();
                                                clientesAux[oper2]=(cola.back());
                                                cola.pop_back();
                                                printf("%d\n", numClientes);
                                                clientes[numClientes].partida=id;
                                                clientes[numClientes-1].partida=id;
                                                clientes[numClientes].AB=0;
                                                clientes[numClientes-1].AB=1;
                                                //jugar(clientesAux[oper1], clientesAux[oper2]);
                                                

                                                ////cosas nuevas
                                                juegos.resize(juegos.size()+1);
                                                juegos[id].crearMatrizEscondida();
                                                bzero(dato,sizeof(dato));
                                                strcpy(dato, "+Ok. Empieza el juego, eres el jugador A");
                                                send(clientes[numClientes].socket,dato,sizeof(dato),0);
                                                bzero(dato,sizeof(dato));
                                                strcpy(dato, "+Ok. Empieza el juego, eres el jugador B");
                                                send(clientes[numClientes-1].socket,dato,sizeof(dato),0);

                                                clientes[numClientes].turno=1;
                                                clientes[numClientes-1].turno=0;
                                                bzero(dato,sizeof(dato));
                                                strcpy(dato, "+Ok. Es tu turno");
                                                send(clientes[numClientes].socket,dato,sizeof(dato),0);
                                                bzero(dato,sizeof(dato));
                                                strcpy(dato, "+Ok. Es de el turno de A");
                                                send(clientes[numClientes-1].socket,dato,sizeof(dato),0);
                                                printf("ID %d", id);
                                                juegos[id].mostrarMatrizMostrar(); //cambiar para que se muestre en los clientes
                                                ////
                                                

                                                ////enviar matriz
                                                bzero(dato,sizeof(dato));
                                                //std::cout<<"Matriz: "<<(juegos[id].MatrizString(dato))<<std::endl;
                                                juegos[id].MatrizString(dato);
                                                send(clientes[numClientes].socket,dato,sizeof(dato),0);
                                                send(clientes[numClientes-1].socket,dato,sizeof(dato),0);
                                                ////
                                                id++;
                                            }
                                        }else if((strncmp(dato,"INICIAR-PARTIDA", 15)==0) && (!registrado(clientes[j].user, clientes[j].password)) && verificacion==2){
                                            bzero(dato,sizeof(dato));
                                            strcpy(dato, "-Err. Debe registrarse para poder jugar\n");
                                            send(clientes[j].socket,dato,sizeof(dato),0);
                                        
                                        }else if((strncmp(dato,"INICIAR-PARTIDA", 15)==0) && (registrado(clientes[j].user, clientes[j].password)) && clientes[j].login==0 && verificacion==2){
                                                std::cout<<"El usuario "<<clientes[j].user<<" no se ha logueado aún\n";
                                        }

                                        if(strncmp(dato,"USUARIO ",8)==0){
                                            if(clientes[j].login==0){
                                                buffer.str(dato);
                                                for(int k=0;k<2;k++){
                                                    buffer>>value;
                                                    if(k==0 && strcmp(value.c_str(),"USUARIO")!=0){
                                                        bzero(dato,sizeof(dato));
                                                        strcpy(dato, "-Err. comando de USUARIO no reconocido\n"); /*REVISAR*/
                                                        send(clientes[j].socket,dato,sizeof(dato),0);
                                                    }
                                                    if(k==1){
                                                        strcpy(clientes[j].user,(value.c_str())); /*Comprobar que el usuario está registrado*/
                                                        verificacion=1;
                                                        bzero(dato,sizeof(dato));
                                                        strcpy(dato, "+Ok. Usuario correcto\n"); /*REVISAR*/
                                                        send(clientes[j].socket,dato,sizeof(dato),0);
                                                    }
                                                }
                                            }else{
                                                bzero(dato,sizeof(dato));
                                                strcpy(dato, "-Err. Usted ya se ha logueado\n");
                                                send(clientes[j].socket,dato,sizeof(dato),0);
                                            }
                                        }else if(strncmp(dato,"PASSWORD ", 9)==0){
                                            if(verificacion==1){
                                                if(clientes[j].login==0){
                                                    buffer.str(dato);
                                                    for(int k=0;k<2;k++){
                                                        buffer>>value;
                                                        if(k==0 && strcmp(value.c_str(),"PASSWORD")!=0){
                                                            bzero(dato,sizeof(dato));
                                                            strcpy(dato, "-Err. comando de PASSWORD no reconocido\n");
                                                            send(clientes[j].socket,dato,sizeof(dato),0);
                                                        }
                                                        if(k==1){
                                                            strcpy(clientes[j].password,(value.c_str())); 
                                                        }
                                                    }
                                                    if(registrado(clientes[j].user, clientes[j].password)){
                                                        bzero(dato,sizeof(dato));
                                                        strcpy(dato, "+Ok. Se ha logueado correctamente!\n");
                                                        send(clientes[j].socket,dato,sizeof(dato),0);
                                                        verificacion=2;
                                                        clientes[j].login=1;
                                                    }
                                                }else{
                                                    bzero(dato,sizeof(dato));
                                                    strcpy(dato, "-Err. Usted ya se ha logueado\n");
                                                    send(clientes[j].socket,dato,sizeof(dato),0);
                                                }
                                            }else{
                                                bzero(dato,sizeof(dato));
                                                strcpy(dato, "-Err. Error en la validación\n");
                                                send(clientes[j].socket,dato,sizeof(dato),0);
                                            }
                                        }else if((strncmp(dato, "REGISTRO ", 9)==0) && verificacion==0){
                                            registered=false;
                                            if(clientes[j].login==0){
                                                if(!registrado(clientes[j].user, clientes[j].password)){
                                                    buffer.str(dato);
                                                    for(int k=0;k<5;k++){
                                                        buffer>>value;
                                                        if(k==0 && strcmp(value.c_str(),"REGISTRO")!=0){
                                                            bzero(dato,sizeof(dato));
                                                            strcpy(dato, "-Err. Comando de REGISTRO no reconocido\n");
                                                            send(clientes[j].socket,dato,sizeof(dato),0);
                                                        }
                                                        if(k==1 && strcmp(value.c_str(),"USUARIO")!=0){
                                                            bzero(dato,sizeof(dato));
                                                            strcpy(dato, "-Err. Comando de REGISTRO (USUARIO) no reconocido\n");
                                                            send(clientes[j].socket,dato,sizeof(dato),0);
                                                        }
                                                        if(k==2)
                                                            strcpy(clientes[j].user,(value.c_str()));
                                                        if(k==3 && strcmp(value.c_str(),"PASSWORD")!=0){
                                                            bzero(dato,sizeof(dato));
                                                            strcpy(dato, "-Err. Comando de REGISTRO (USUARIO) no reconocido\n");
                                                            send(clientes[j].socket,dato,sizeof(dato),0);
                                                        }
                                                        if(k==4)
                                                            strcpy(clientes[j].password,(value.c_str()));
                                                    }
                                                    fichero.open("BASEDEDATOS.txt");
                                                    fichero.seekg(0, std::ios::end);
                                                    size=fichero.tellg();
                                                    fichero.seekg(0, std::ios::beg);
                                                    if(size==0){
                                                        ficheroAux.open("BASEDEDATOS.txt");
                                                        ficheroAux.close();
                                                    }
                                                    if(size>0){
                                                        while(!fichero.eof()){
                                                            std::getline(fichero, g, '\t');
                                                            usuarioAux=g;
                                                            std::getline(fichero, g, '\n');
                                                            passwordAux=g;
                                                            if(strcmp(usuarioAux.c_str(), clientes[j].user)==0 && strcmp(passwordAux.c_str(), clientes[j].password)==0){
                                                                bzero(dato,sizeof(dato));
                                                                strcpy(dato, "-Err. Usuario ya existente en la base de datos, por favor inicie sesión\n");
                                                                send(clientes[j].socket,dato,sizeof(dato),0);
                                                                registered==true;
                                                                fichero.close();
                                                            }
                                                        }
                                                        if(registered==false){
                                                            ficheroAux.open("BASEDEDATOS.txt", std::ios::app);        
                                                            ficheroAux<<clientes[j].user<<"\t"<<clientes[j].password<<"\n";
                                                            bzero(dato,sizeof(dato));
                                                            strcpy(dato, "+Ok. Se ha registrado correctamente, proceda a loguearse si desea jugar, por favor\n");
                                                            send(clientes[j].socket,dato,sizeof(dato),0);
                                                            ficheroAux.close();
                                                        } 
                                                    }else{
                                                        ficheroAux.open("BASEDEDATOS.txt", std::ios::app);
                                                        ficheroAux<<clientes[j].user<<"\t"<<clientes[j].password<<"\n";
                                                        bzero(dato,sizeof(dato));
                                                        strcpy(dato, "+Ok. Se ha registrado correctamente, proceda a loguearse si desea jugar, por favor\n");
                                                        send(clientes[j].socket,dato,sizeof(dato),0);
                                                        ficheroAux.close();  
                                                    }
                                                }else{
                                                    bzero(dato,sizeof(dato));
                                                    strcpy(dato, "-Err. Usted ya está registrado y logueado\n");
                                                    send(clientes[j].socket,dato,sizeof(dato),0);
                                                }
                                            }else{
                                                bzero(dato,sizeof(dato));
                                                strcpy(dato, "-Err. Usted ya se ha registrado, por favor, logueese\n");
                                                send(clientes[j].socket,dato,sizeof(dato),0);
                                            }
                                        }else if(strcmp(dato,"SALIR\n") == 0){
                                     
                                            for (j = 0; j < j; j++){
                                                send(clientes[j].socket, "Desconexion servidor\n", strlen("Desconexion servidor\n"),0);
                                                close(clientes[j].socket);
                                                exit(-1);   
                                                FD_CLR(clientes[j].socket,&readfds);
                                            }
                                                close(sd);
                                        }else if(strncmp(dato,"DESCUBRIR ", 10)==0){ //FALTARIA poner que esta en partida pero no lo queria meter xq falla ya al no meterse
                                            printf("holi %d \n" ,clientes[j].partida);
                                            int otrocliente;

                                            ///no se si esto xd
                                                
                                            for(int s = 0; s <= numClientes ; s++){
                                                if (clientes[j].partida==clientes[s].partida){
                                                    if(s!=j){
                                                        otrocliente=s;
                                                    }
                                                }
                                            }
                                            ////

                                            if(clientes[j].turno==1){
                                                clientes[j].letra[0]=dato[10];
                                                clientes[j].numero[0]=dato[12];
                                                std::cout<<"letra: "<<clientes[j].letra[0]<<" numero: "<<clientes[j].numero[0]<<std::endl;
                                                if(juegos[clientes[j].partida].coordenadas(*clientes[j].letra, atoi(clientes[j].numero))==true){
                                                    std::cout<<"Hola 1\n";
                                                    
                                                    if(juegos[clientes[j].partida].estaVisitada(*clientes[j].letra, atoi(clientes[j].numero))==true){
                                                        printf("casilla ya visitada\n");
                                                    }else{
                                                        std::cout<<"Hola 2\n";
                                                        juegos[clientes[j].partida].MatrizPinchar(*clientes[j].letra, atoi(clientes[j].numero));
                                                        clientes[j].turno=0;
                                                        clientes[otrocliente].turno=1;
                                                        
                                                        juegos[clientes[j].partida].mostrarMatrizMostrar();
                                                    }

                                                }
                                           }else{
                                                bzero(dato,sizeof(dato));
                                                strcpy(dato, "-Err. No es su turno\n");
                                                send(clientes[j].socket,dato,sizeof(dato),0);
                                            }
                                        }else if(strncmp(dato,"PONER-BANDERA ", 14)==0){
                                            std::cout<<"Bandera jeje\n";
                                            int otrocliente;

                                            ///no se si esto xd
                                                
                                            for (int s = 0; s <= numClientes ; ++s){
                                                if (clientes[j].partida==clientes[s].partida){
                                                    if(s!=j){
                                                        otrocliente=s;
                                                    }
                                                }
                                            }
                                            
                                                ////
                                            if (clientes[j].turno==1){
                                                /* code */
                                                clientes[j].letra[0]=dato[14];
                                                clientes[j].numero[0]=dato[16];
                                                if(juegos[clientes[j].partida].coordenadas(*clientes[j].letra, atoi(clientes[j].numero))){
                                                    if(juegos[clientes[j].partida].estaVisitada(*clientes[j].letra, atoi(clientes[j].numero))==true){
                                                        printf("casilla ya visitada\n");
                                                    }else{
                                                        if(clientes[j].AB==0){
                                                            juegos[clientes[j].partida].MatrizBandera(*clientes[j].letra, atoi(clientes[j].numero), 'A');
                                                            clientes[j].turno=0;
                                                            clientes[otrocliente].turno=1;
                                                            juegos[clientes[j].partida].mostrarMatrizMostrar();

                                                        }else{
                                                            juegos[clientes[j].partida].MatrizBandera(*clientes[j].letra, atoi(clientes[j].numero), 'A');
                                                            clientes[j].turno=0;
                                                            clientes[otrocliente].turno=1;
                                                            juegos[clientes[j].partida].mostrarMatrizMostrar();
                                                        }
                                                    }
                                                }
                                            }else{
                                                bzero(dato,sizeof(dato));
                                                strcpy(dato, "-Err. No es su turno\n");
                                                send(clientes[j].socket,dato,sizeof(dato),0);
                                            }
                                        }else if(recibidos==0){
                                            printf("El socket %d, ha introducido ctrl+c\n", i);
                                            //Eliminar ese socket
                                            salirCliente(i, &readfds,j,clientes);
                                        }
                                    }else{
                                        std::cout<<"-Err. recibiendo datos del cliente\n";
                                    }
                                } 
                            }                                       
                        }
                    }
                }
            }
		}
	close(sd);
	return 0;
}

void salirCliente(int socket, fd_set *readfds, int &numClientes, struct clients *clientes){
    char dato[250];
    int j;
    
    close(socket);
    FD_CLR(socket,readfds);
    
    //Re-estructurar el array de clientes
    for (j = 0; j < (numClientes) - 1; j++)
        if (clientes[j].socket == socket)
            break;
    for (; j < (numClientes) - 1; j++)
        (clientes[j].socket = clientes[j+1].socket);
    
    (numClientes)--;
    
    bzero(dato,sizeof(dato));
    sprintf(dato,"Desconexión del cliente: %d\n", socket);
    
    for(j=0; j<(numClientes); j++)
        if(clientes[j].socket != socket)
            send(clientes[j].socket,dato,strlen(dato),0);


}

void manejador (int signum){
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);
    
    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}


bool registrado(char *nombre, char *password){
    if(strncmp(nombre,"0",1)!=0 && strncmp(password,"0",1)!=0){
        double size;
        std::ifstream fichero;
        std::ofstream ficheroAux;
        fichero.open("BASEDEDATOS.txt");
        std::string usuario, pass, f;
        fichero.seekg(0, std::ios::end);
        size=fichero.tellg();
        fichero.seekg(0, std::ios::beg);
        if(size==0){
            ficheroAux.open("BASEDEDATOS.txt");
            ficheroAux.close();
        }
        if(size>0){
            while(!fichero.eof()){
                std::getline(fichero, f, '\t');
                usuario=f;
                std::getline(fichero, f, '\n');
                pass=f;
                if((strcmp(nombre,usuario.c_str())==0) && (strcmp(password,pass.c_str())==0)){
                    fichero.close();
                    return true;
                }
            }
            return false;
        }else{
            fichero.close();
            return false;   
        }
    }else{
        return false;
    }
}


void jugar(clients a, clients b){
    std::cout<<"Holo\n";
    Buscaminas juego;
    int recibido=-1, numero=0;
    int aux=0;
    char dato[MSG_SIZE], valor[10], *letra;
    fd_set readfds;



    juego.crearMatrizEscondida();
    bzero(dato,sizeof(dato));
    strcpy(dato, "Empieza el juego, eres el jugador A");
    send( a.socket,dato,sizeof(dato),0);
    bzero(dato,sizeof(dato));
    strcpy(dato, "Empieza el juego, eres el jugador B");
    send( b.socket,dato,sizeof(dato),0);
    
        

    while(aux==0){ //WHILE JUEGO
        a.turno=1;
        bzero(dato,sizeof(dato));
        strcpy(dato, "Es tu turno");
        send( a.socket,dato,sizeof(dato),0);
        bzero(dato,sizeof(dato));
        strcpy(dato, "Es de el turno de A");
        send(b.socket,dato,sizeof(dato),0);
        juego.mostrarMatrizMostrar();
        while(a.turno==1){ //TURNO A
            std::istringstream buffer;
            std::string value, g;
            bzero(dato,sizeof(dato));
            recv(a.socket,dato,sizeof(dato),0);
            std::cout<<"dato: "<<dato<<std::endl;
            if(strncmp(dato,"DESCUBRIR ", 10)==0){
                a.letra[0]=dato[10];
                a.numero[0]=dato[12];
                std::cout<<"letra: "<<a.letra[0]<<" numero: "<<a.numero[0]<<std::endl;
                if(juego.coordenadas(*a.letra, atoi(a.numero))==true){
                    std::cout<<"Hola 1\n";
                    if(juego.estaVisitada(*a.letra, atoi(a.numero))==true){
                        printf("casilla ya visitada\n");
                    }
                    std::cout<<"Hola 2\n";
                    juego.MatrizPinchar(*a.letra, atoi(a.numero));
                    a.turno=0;
                }
            }else if(strncmp(dato,"PONER-BANDERA ", 16)==0){
                a.letra[0]=dato[10];
                a.numero[0]=dato[12];
                if(juego.coordenadas(*a.letra, atoi(a.numero))){
                    if(juego.estaVisitada(*a.letra, atoi(a.numero))==true){
                        printf("casilla ya visitada\n");
                    }

                    juego.MatrizBandera(*a.letra, atoi(a.numero), 'A');
                    a.turno=0;
                }
            }else if(strcmp(dato,"SALIR\n") == 0){
                send(a.socket, "Desconexion servidor\n", strlen("Desconexion servidor\n"),0);
                close(a.socket);
                exit(-1);   
                FD_CLR(a.socket,&readfds);
                close(a.socket);
            }else{
                bzero(dato,sizeof(dato));
                strcpy(dato, "-Err\n");
                send(a.socket,dato,sizeof(dato),0);
            }
        }
        //Cuando se muestree casilla o se ponga bandera, a.turno=0;


        //bloquear b
        //mandar string a A con todas las matrices
        //el cliente hace un segundo contructor con 3 parametros como las 3 matrices
        // en el cliente se ejecuta buscaminas juego.
        //el cliente A despues de ejecutar buscaminas juego envia las cadenas con las matrices modificadas en el siguiente recv
        b.turno=1;
        bzero(dato,sizeof(dato));
        strcpy(dato, "Es de el turno de B");
        send( a.socket,dato,sizeof(dato),0);
        bzero(dato,sizeof(dato));
        strcpy(dato, "Es tu turno");
        send( b.socket,dato,sizeof(dato),0);
        juego.mostrarMatrizMostrar();

        while(b.turno==1){ //TURNO A
            std::istringstream buffer;
            std::string value, g;
            bzero(dato,sizeof(dato));
            recv(b.socket,dato,sizeof(dato),0);
            std::cout<<"dato: "<<dato<<std::endl;
            if(strncmp(dato,"DESCUBRIR ", 10)==0){
                b.letra[0]=dato[10];
                b.numero[0]=dato[12];
                if(juego.coordenadas(*b.letra, atoi(b.numero))){                    
                    std::cout<<"Hola 1\n";
                    if(juego.estaVisitada(*b.letra, atoi(b.numero))==true){
                        printf("casilla ya visitada\n");
                    }
                    std::cout<<"Hola 2\n";
                    juego.MatrizPinchar(*b.letra, atoi(b.numero));
                    b.turno=0;
                }
            }else if(strncmp(dato,"PONER-BANDERA ", 16)==0){
                b.letra[0]=dato[10];
                b.numero[0]=dato[12];
                if(juego.coordenadas(*b.letra, atoi(b.numero))){
                    if(juego.estaVisitada(*b.letra, atoi(b.numero))==true){
                        printf("casilla ya visitada\n");
                    }

                    juego.MatrizBandera(*b.letra, atoi(b.numero), 'A');
                    b.turno=0;
                }
            }else if(strcmp(dato,"SALIR\n") == 0){
                send(b.socket, "Desconexion servidor\n", strlen("Desconexion servidor\n"),0);
                close(b.socket);
                exit(-1);   
                FD_CLR(b.socket,&readfds);
                close(b.socket);
            }else{
                bzero(dato,sizeof(dato));
                strcpy(dato, "-Err\n");
                send(b.socket,dato,sizeof(dato),0);
            }
        }
        //bloquear a
        //mandar string a B con la matriz, en el cliente se ejecuta buscaminas juego.
        //el cliente a despues de ejecutar buscaminas juego envia una cadena con la matriz modificada en el siguiente recv
    }
}



/*
Se debe comprobar que un cliente no pueda introducir 
un user que ya esté con otra contraseña

Ponerle a cada jugador una variable para las flags que deberán restarse


*/