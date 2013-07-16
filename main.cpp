#include <iostream>
#include <vector>
#include "Grafo.h"

int main() {
	Grafo G(false, true, "Grafo.txt");	
	G.Imprimir(std::cout);
	getchar();
	return EXIT_SUCCESS;
}