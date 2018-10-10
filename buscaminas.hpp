#ifndef _buscaminas_hpp_
#define _buscaminas_hpp_
#include <iostream>
#include <vector>
#include <string>

class Buscaminas{
 
//c_str en la conversion de las cadenas al hacer el send en los sockets, ya que hay problemas con string

private:
	int bombas_;
	std::vector< std::vector<int > > MatrizEscondida;
	//std::vector< std::vector<int > > MatrizMostrar;
	//std::vector<std::vector<std::string> > MatrizEscondida = std::vector< std::vector<std::string> >(10, std::vector<std::string>(10));
	std::vector<std::vector<std::string> > MatrizMostrar = std::vector< std::vector<std::string> >(10, std::vector<std::string>(10));
	//std::string A_="A";
	//std::string B_="B";
	//std::string AB_="AB";

public:
	//constructor
	Buscaminas()
	{
		bombas_=20;
		MatrizEscondida.resize(10);
			for (int a = 0; a < 10; a++)
			{
				MatrizEscondida[a].resize(10,0);
			}
	}
	//Funciones GET
	inline std::vector<std::vector<int> > getMatrizEscondida(){return MatrizEscondida;}
	inline std::vector<std::vector<std::string> > getMatrizMostrar(){return MatrizMostrar;}
	//FUNCIONES GET DE A B AB, NO SE MODIFICAN
	int getX(char x);
	int getMinas(){return bombas_;}


	//Funciones SET
	inline void setBombas(int bombas){bombas_=bombas;}
	inline void setMatrizEscondida(std::vector<std::vector<int> > MatrizEscondidaAux){MatrizEscondida==MatrizEscondidaAux;}
	//void MatrizPinchar(std::string x, int y); //Cambia la matriz a mostrar
	void crearMatrizEscondida();
	void mostrarMatrizEscondida();
	void mostrarMatrizMostrar();

};
#endif
