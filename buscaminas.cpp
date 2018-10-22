#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale> 
#include "buscaminas.hpp"

int Buscaminas::getX(char x){
	switch (x){
		case 'A':
			return 0;
			break;

		case 'B':
			return 1;
			break;

		case 'C':
			return 2;
			break;

		case 'D':
			return 3;
			break;

		case 'E':
			return 4;
			break;
		case 'F':
			return 5;
			break;

		case 'G':
			return 6;
			break;

		case 'H':
			return 7;
			break;

		case 'I':
			return 8;
			break;

		case 'J':
			return 9;
			break;

		default:
			std::cout<<"Error, letra no reconocida, escribala en mayúscula en el rango de A a J\n\n";
			return -1;
			break;
	}
}

bool Buscaminas::estaVisitada(char x , int y)
{
	
	std::vector< std::vector<int > > aux;
	int a=getX(x);
	aux=getVisitadas();
	

	if(aux[y][a]==0)
	{
		
		return false;
	}
	else
	{
		
		return true;
	}
}

bool Buscaminas::estaVisitada2(int x, int y)
{
	
	std::vector< std::vector<int > > aux;
	aux=getVisitadas();
	

	if(aux[x][y]==0)
	{
		
		return false;
	}
	else
	{
		
		return true;
	}
}



void Buscaminas::ganar()
{
	if(getEncontradas()==10)
	{
		printf("Has ganado");
		exit(0);
	}
}

void Buscaminas::Encontrar()
{
	int aux;
	aux=getEncontradas();
	aux++;
	setEncontradas(aux);
}

int Buscaminas::getNumero(int y){
	if(y>=0 && y<10)
		return 1;
	else{
		std::cout<<"Error, elija un número en el rango de 0 a 9\n\n";
		return -1;
	}
}

void Buscaminas::coordenadas(char &x, int &y){
	int comprobador;
	std::cout<<"Introduzca las coordenadas (Letra, Numero)\n\n";
	do{
		std::cout<<"Introduzca una letra: ";
		std::cin>>x;
		comprobador=getX(x);
	}while((std::isupper(x))==0 || (comprobador==-1));

	do{
		std::cout<<"\nIntroduzca un numero: ";
		std::cin>>y;
		comprobador=getNumero(y);
	}while((std::isdigit(y))==0 && (comprobador==-1));
	std::cout<<"\nCoordenada: ("<<x<<", "<<y<<") \n";
}

void Buscaminas::crearMatrizEscondida(){
	srand(time(NULL));
	int i, j, x, y, contador=0, numero=0, minas, a, b;
	std::vector<std::vector<int > > MatrizAux ;
	/*
	MatrizAux.resize(10);
			for (int a = 0; a < 10; a++)
			{
				MatrizEscondida[a].resize(10,0);
			}
		*/
			MatrizAux= getMatrizEscondida();

	do{
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				if(contador!=getMinas()){
					numero=(rand()%101);
					if(numero<30){
						MatrizAux[i][j]=-1;
						//std::cout<<MatrizAux[i][j]<<"\t";
						contador++;
					}
				}
			}
		}
	}while(contador!=getMinas());

	std::cout<<"Se han colocado las minas\n";
	setMatrizEscondida(MatrizAux);
	mostrarMatrizEscondida();
	

	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			if(MatrizAux[i][j]!=-1){
				minas=0;
				for(x=-1;x<2;x++){
					for(y=-1;y<2;y++){
						a=i-x;
						b=j-y;
						if((a >= 0 && b >= 0)  && (a < 10 && b < 10)){
							if(MatrizAux[a][b]==-1)
								minas++;
						}
					}
				}
				MatrizAux[i][j]=minas;
				//std::cout<<"Se han colocado un valor\n";
			}
		}
	}
	//std::cout<<"Se han colocado un valor\n";
	/*
	for(i = 0; i < 10; i++){
		for(j=0 ; j<10; j++){
			std::cout<<"|"<<MatrizAux[i][j]<<"|\t";
		}
		std::cout<<"\n";
	}
	*/
	setMatrizEscondida(MatrizAux);
}

void Buscaminas::MatrizPinchar(char x, int y){
	punto xd;
	std::vector<std::vector<int > > MatrizAux1=getMatrizEscondida();
	std::vector<std::vector<int > > MatrizAux2=getMatrizMostrar();
	int a=getX(x);
	MatrizAux2[y][a]=MatrizAux1[y][a];
	setMatrizMostrar(MatrizAux2);

	visitar(y , a);

	if (MatrizAux2[y][a]==-1)
	{
		printf("Has explotado una mina, has perdido" );
		exit(0); //supongo que habra que salirse con el cliente o algo, esto es provicional
		//llamar a funcion que le diga al cliente que se vaya o yo que se xd
	}

	if (MatrizAux2[y][a]==0)
	{
		//xd=abrirZeros(y, a , xd);
	}
}

punto Buscaminas::abrirZeros(int m , int n , punto aux)
{

	std::vector<std::vector<int > > MatrizAux2=getMatrizEscondida();
	std::vector<std::vector<int > > MatrizAux;
	int a ,b ;
	punto p;
	for(p.x=-1;p.x<2;p.x++){
		printf("x %d\n",p.x );
					for(p.y=-1;p.y<2;p.y++){
						printf("y %d\n",p.y );
						a=m-p.x;
						b=n-p.y;
						printf("hey %d %d %d %d\n" , a , b , m , n);
						if((a >= 0 && b >= 0)  && (a < 10 && b < 10)){
							MatrizAux=getMatrizMostrar();
							MatrizAux[a][b]=MatrizAux2[a][b];
							setMatrizMostrar(MatrizAux);
							printf("%d\n", MatrizAux[a][b] );
							if (MatrizAux[a][b]==0)
							 
							{
								
								if ( estaVisitada2(a,b)==false )
								{
									//printf("a:%d b:%d m:%d n:%d \n", a , b , m ,n);
								
								p=abrirZeros(a , b , p);
								//printf("holi\n");
								}
							}
						}
						visitar(a,b);
					}
				}

	return aux;
		
}

void Buscaminas::MatrizBandera(char x, int y, char jugador){
	std::vector<std::vector<int > > MatrizAux ;
	std::vector<std::vector<int > > MatrizAux2 ;
	MatrizAux=getMatrizMostrar();
	MatrizAux2=getMatrizEscondida();
	int a=getX(x);

	if (jugador=='A'){
		MatrizAux[y][a]=-3;
	}else if (jugador=='B'){
		MatrizAux[y][a]=-4;
	}
	else{
		MatrizAux[y][a]=-5;
	}
	setMatrizMostrar(MatrizAux);

	//se van contando las banderas que acierta
	if(MatrizAux2[y][a]==-1)
	{
		Encontrar();
		ganar();
	}

	visitar(y,a);
}


void Buscaminas::mostrarMatrizEscondida(){
	int i, j;
	std::vector<std::vector<int> > MatrizAux = getMatrizEscondida();
	for(i = 0; i < 10; i++){
		for(j=0 ; j<10; j++){
			std::cout<<"|"<<MatrizAux[i][j]<<"|\t";
		}
		std::cout<<"\n";
	}
	std::cout<<std::endl;
	std::cout<<std::endl;
}

void Buscaminas::mostrarMatrizMostrar(){
	int i, j;
	std::vector<std::vector<int> > MatrizAux = getMatrizMostrar();
	for(i = 0; i < (int) MatrizAux.size(); i++){
		for(j=0;j<(int) MatrizAux.size(); j++){
			if (MatrizAux[i][j]==-2){
				std::cout<<"|"<<"-"<<"|\t";
			}
			else if (MatrizAux[i][j]==-1){
				std::cout<<"|"<<"*"<<"|\t";
			}
			else if (MatrizAux[i][j]==-3){
				std::cout<<"|"<<"A"<<"|\t";
			}
			else if (MatrizAux[i][j]==-4){
				std::cout<<"|"<<"B"<<"|\t";
			}
			else if (MatrizAux[i][j]==-5){
				std::cout<<"|"<<"AB"<<"|\t";
			}else{
				std::cout<<"|"<<MatrizAux[i][j]<<"|\t";
			}
		}
		std::cout<<"\n";
	}
	std::cout<<std::endl;
	std::cout<<std::endl;
}

void Buscaminas::buscaminasGame(){
	int aux=0, a, y, opcion;
	char s, x, jugador='A';
	crearMatrizEscondida();

	mostrarMatrizEscondida();

	
	mostrarMatrizMostrar();

	while(aux==0){
		coordenadas(x,y);

		if(estaVisitada(x,y)==true)
		{
			printf("casilla ya visitada\n");
		}
		else{
		std::cout<<"Pulse 1 para descubrir pulse 2 para poner una bandera\n";
		std::cin>>opcion;
		switch(opcion){
			case 1:
				MatrizPinchar(x, y);
				mostrarMatrizMostrar();
				break;

			case 2:
				MatrizBandera(x, y, jugador);
				mostrarMatrizMostrar();
				break;

			default: 
				std::cout<<"Opción errónea, eliga entre 1 o 2\n";
				break;
			}	
		}
	}
}