#include "buscaminas.hpp"
#include <vector>
#include <string.h>
#include <stdio.h>
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
	std::vector<std::vector<std::string> > MatrizAux = getMatrizEscondida();
	do{
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				if(contador!=getMinas()){
					numero=(rand()%101);
					if(numero<30){
						MatrizAux[i][j]="*";
					}
				}
			}
		}
	}while(contador==getMinas());

	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			if(MatrizAux[i][j]!="*"){
				minas=0;
				for(x=-1;x<3;i++){
					for(y=-1;y<3;j++){
						a=i-x;
						b=j-y;
						if((a > 0 && b > 0)  && (a < 10 && b < 10)){
							if(MatrizAux[a][b]=="*")
								minas++;
						}
					}
				}
				MatrizAux[i][j]=minas;
			}
		}
	}


	setMatrizEscondida(MatrizAux);
}

//void Buscaminas::MatrizPinchar(int x, int y){}

void Buscaminas::mostrarMatrizEscondida(){
	int i, j;
	std::vector<std::vector<std::string> > MatrizAux = getMatrizEscondida();
	for(i = 0; i < (int) MatrizAux.size(); i++){
		for(j=0 ; j<(int) MatrizAux.size(); j++){
			std::cout<<MatrizAux[i][j]<<"\t";
		}
		std::cout<<"\n";
	}
}

void Buscaminas::mostrarMatrizMostrar(){
	int i, j;
	std::vector<std::vector<std::string> > MatrizAux = getMatrizMostrar();
	for(i = 0; i < (int) MatrizAux.size(); i++){
		for(j=0;j<(int) MatrizAux.size(); j++){
			std::cout<<MatrizAux[i][j]<<"\t";
		}
		std::cout<<"\n";
	}
}