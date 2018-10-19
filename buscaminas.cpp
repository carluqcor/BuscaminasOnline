#include "buscaminas.hpp"
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
			std::cout<<"Error, letra no reconocida, escribala en mayúscula en el rango de A a J\n";
			break;

	}
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
					std::cout<<"numero: "<<numero<<std::endl;
					if(numero<30){
						MatrizAux[i][j]=-1;
						//std::cout<<MatrizAux[i][j]<<"\t";
						contador++;
					}
				std::cout<<"contador: "<<contador<<std::endl;
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

void Buscaminas::MatrizPinchar(char x, int y)
{
	std::vector<std::vector<int > > MatrizAux1 ;
	std::vector<std::vector<int > > MatrizAux2 ;

	MatrizAux1=getMatrizEscondida();
	MatrizAux2=getMatrizMostrar();

	int a= getX(x);
	MatrizAux2[a][y]=MatrizAux1[a][y];

	setMatrizMostrar(MatrizAux2);

}

void Buscaminas::MatrizBandera(char x, int y, char jugador)
{
	std::vector<std::vector<int > > MatrizAux ;
	MatrizAux=getMatrizMostrar();
	int a= getX(x);

	if (jugador=='A')
	{
		MatrizAux[a][y]=-3;
	}
	else if (jugador=='B')
	{
		MatrizAux[a][y]=-4;
	}
	else
	{
		MatrizAux[a][y]=-5;
	}

	setMatrizMostrar(MatrizAux);
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
}

void Buscaminas::mostrarMatrizMostrar(){
	int i, j;
	std::vector<std::vector<int> > MatrizAux = getMatrizMostrar();
	for(i = 0; i < (int) MatrizAux.size(); i++){
		for(j=0;j<(int) MatrizAux.size(); j++){
			if (MatrizAux[i][j]==-2)
			{
			std::cout<<"|"<<"-"<<"|\t";
			}
			else if (MatrizAux[i][j]==-1)
			{
			std::cout<<"|"<<"*"<<"|\t";
			}
			else if (MatrizAux[i][j]==-3)
			{
			std::cout<<"|"<<"A"<<"|\t";
			}
			else if (MatrizAux[i][j]==-4)
			{
			std::cout<<"|"<<"B"<<"|\t";
			}
			else if (MatrizAux[i][j]==-5)
			{
			std::cout<<"|"<<"AB"<<"|\t";
			}
			else
			{
			std::cout<<"|"<<MatrizAux[i][j]<<"|\t";
			}
		}
		std::cout<<"\n";
	}
}