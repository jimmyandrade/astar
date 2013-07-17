#include <iostream>
#include <vector>
#include "Grafo.h"

int main() {
	Grafo G(false, true, "Grafo.txt");	
	G.Imprimir(std::cout);
	
	
	G.DLS("Arad", "Bucharest", 4);

	getchar();
	return EXIT_SUCCESS;
}