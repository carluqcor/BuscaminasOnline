#ifndef buscaminas_hpp
#define buscaminas_hpp
#include <iostream>
#include <vector>
#include <string.h>

class Buscaminas{
 
//c_str en la conversion de las cadenas al hacer el send en los sockets, ya que hay problemas con string

private:
	int bombas_=20;
	std::vector<std::vector<std::string> > MatrizEscondida = std::vector< std::vector<std::string> >(10, std::vector<std::string>(10));
	

	std::vector<std::vector<std::string> > MatrizMostrar = std::vector< std::vector<std::string> >(10, std::vector<std::string>(10));

	std::string A_="A";
	std::string B_="B";
	std::string AB_="AB";

public:

	//Funciones GET
	inline std::vector<std::vector<std::string> > getMatrizEscondida(){return MatrizEscondida;}
	inline std::vector<std::vector<std::string> > getMatrizMostrar(){return MatrizMostrar;}
	//FUNCIONES GET DE A B AB, NO SE MODIFICAN
	int getX(char x);
	int getMinas(){return bombas_;}


	//Funciones SET
	inline void setBombas(int bombas){bombas_=bombas;}
	inline void setMatrizEscondida(std::vector<std::vector<std::string> > MatrizEscondidaAux){MatrizEscondida==MatrizEscondidaAux;}
	//void MatrizPinchar(std::string x, int y); //Cambia la matriz a mostrar
	void crearMatrizEscondida();
	void mostrarMatrizEscondida();
	void mostrarMatrizMostrar();

};


#endif