#include "Matriz.h"

Matriz::Matriz(const Indice& Dimensao) : _Linhas(Dimensao), _Colunas(Dimensao) {

	_Matriz = new TipoPeso*[_Linhas];
	set(0);

}

void Matriz::set(const TipoPeso& x) {

	for (Indice i = 0; i < Linhas(); ++i) {
		_Matriz[i] = new TipoPeso[Colunas()];
		for (Indice j = 0; j < Colunas(); ++j) {
			set(i, j, x);
		}
	}

}

std::ostream& Matriz::Imprimir(std::ostream& os) const {

	for (Indice i = 0; i < Linhas(); ++i) {
		for (Indice j = 0; j < Colunas(); ++j) {
			os << get(i, j) << "\t";
		}
		os << std::endl;
	}

	return os;
}

std::ostream& operator<<(std::ostream& os, const Matriz& obj) {
	obj.Imprimir(os);
	return os;
}