
#include "buscaminas.hpp"
#include <iostream>
#include <string>

int main(){
	int aux=0;
	int a;
	char s ;
	int opcion;
	char jugador='A';
	Buscaminas buscaminas;
	buscaminas.crearMatrizEscondida();
	std::cout<<"main\n";
	buscaminas.mostrarMatrizEscondida();
	buscaminas.mostrarMatrizMostrar();
	while(aux==0)
	{
	std::cout<<"Introduzca las coordenadas\n";
	std::cout<<"Introduzca una letra\n";
	std::cin>>s;
	std::cout<<"Introduzca un numero\n";
	std::cin>>a;
	std::cout<<"posicion=letra "<<s<<"+ numero "<<a<<"\n";
	std::cout<<"Pulse 1 para descubrir pulse 2 para poner una bandera\n";
	std::cin>>opcion;
	if(opcion==1)
	{
	buscaminas.MatrizPinchar(s, a);
	buscaminas.mostrarMatrizMostrar();
	}
	else if(opcion==2)
	{
		buscaminas.MatrizBandera(s, a , jugador);
		buscaminas.mostrarMatrizMostrar();
	}

	}

}