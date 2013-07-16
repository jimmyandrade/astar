#ifndef MATRIZ_H
#define MATRIZ_H

#include <iostream>

class Matriz {

	typedef unsigned int Indice;
	typedef int TipoPeso;

	public:
		inline Indice Linhas() const { return _Linhas; }
		inline Indice Colunas() const { return _Colunas; }
		Matriz(const Indice& Dimensao);
		inline TipoPeso get(const Indice& i, const Indice& j) const { return _Matriz[i][j]; }
		inline void set(const Indice& i, const Indice& j, const TipoPeso x) { _Matriz[i][j] = x; }
		virtual std::ostream& Imprimir(std::ostream& o) const;
	private:
		Indice _Linhas;
		Indice _Colunas;
		TipoPeso** _Matriz;
		void set(const TipoPeso& x);
};

#endif