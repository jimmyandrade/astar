#include "Grafo.h"
#include <fstream>
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Grafo& obj) {
	obj.Imprimir(os);
	return os;
}

std::ostream& operator<<(std::ostream& os, const Grafo::Link& obj) {
	obj->Imprimir(os);
	return os;
}

std::ostream& operator<<(std::ostream& os, const Grafo::No& obj) {
	obj.Imprimir(os);
	return os;
}

std::ostream& operator<<(std::ostream& os, const Grafo::Aresta& obj) {
	obj.Imprimir(os);
	return os;
}

std::ostream& operator<<(std::ostream& os, const Grafo::Aresta* obj) {
	obj->Imprimir(os);
	return os;
}

std::ostream& operator<<(std::ostream& os, const Grafo::MapaNomeIndice& obj) {

	os << "ID\tNome" << std::endl;

	for (auto& x : obj) {
		os << x.second << "\t" << x.first << std::endl;
	}

	return os;

}

std::ostream& operator<<(std::ostream& os, const Grafo::MapaNos& obj) {
	
	os << "[";

	for (auto& x : obj) {
		os << x.second << " ";
	}

	os << "]";

	return os;

}

std::ostream& Grafo::ImprimirNos(std::ostream& os) const {

	for (auto& x : _A) {
		os << x.first << " -> " << x.second << std::endl;
	}
	
	return os;

}

std::string Plural(int x) {
	if (x > 1) {
		return "s";
	}
	else {
		return "";
	}
}

std::ostream& Grafo::Imprimir(std::ostream& os) const {

	os << std::endl << "Grafo com " << QuantidadeNos() << " no" << Plural(QuantidadeNos());
	os << " e " << QuantidadeArestas() << " aresta" << Plural(QuantidadeArestas()) << std::endl;
	ImprimirNos(os);

	return os;
}

bool Grafo::Existe(const Indice& i) const {
	if (!_R.empty() && _R.find(i) != _R.end()) {
		return true;
	}
	else {
		return false;
	}
}

bool Grafo::Existe(const No& N) const {

	if (Existe(N.ID())) {
		std::cout << "Ja existe " << N << std::endl;
	}
	else {
		std::cout << "Ainda nao existe " << N << std::endl;
	}
	return Existe(N.ID());
}

bool Grafo::Inserir(const No& N) {

	std::cout << "Preparando insercao de " << N << std::endl;

	if (Existe(N)) { return false; }

	_N[N.ID()] = N;

	MapaNos Vazios;
	_R[N.ID()] = Vazios;

	std::cout << N << " foi inserido" << std::endl;

	return true;
}

void Grafo::Conectar(const Link& Origem, const Link& Destino) {

	MapaNos ListaRecuperada = _R[Origem->ID()];
	ListaRecuperada[Destino->ID()] = Destino;
	_R[Origem->ID()] = ListaRecuperada;

}

void Grafo::Conectar(const Aresta& A) {
	Conectar(A.Origem(), A.Destino());
}

bool Grafo::Inserir(const Aresta& A) {
	std::cout << "Inserindo " << A << std::endl;

	if (!Existe(A.OrigemID())) {
		Inserir(A.Origem());
	}

	if (!Existe(A.DestinoID())) {
		Inserir(A.Destino());
	}

	Conectar(A);
	_A.insert(BancoDeArestas::value_type(A.OrigemID(), A));

	if (!Direcionado()) {
		Conectar(A.Destino(), A.Origem());
		_A.insert(BancoDeArestas::value_type(A.DestinoID(), A));
	}

	return true;
}

Grafo::BancoDeArestas::iterator Grafo::Expandir(const Indice& I) {
	return _A.find(I);
}

Matriz Grafo::MatrizRepresentativa() {

	const Indice n = QuantidadeNos();
	const Indice a = QuantidadeArestas();
	Matriz m(n);

	for (BancoDeArestas::iterator i = _A.begin(); i != _A.end(); ++i) {

		Indice Linha = (*i).second.OrigemID() - 1;
		Indice Coluna = (*i).second.DestinoID() - 1;
		Indice Valor = (*i).second.Peso();

		m.set(Linha, Coluna, Valor);

		if (!Direcionado()) {
			m.set(Coluna, Linha, Valor);
		}
	}

	return m;

}

Grafo::Aresta::Aresta(const Grafo::No& Origem, const Grafo::No& Destino, const Grafo::TipoPeso& Peso) : _Peso(Peso) {
	_Origem = new No(Origem);
	_Destino = new No(Destino);
}

Grafo::Grafo(const bool& Direcionado, const bool& ComCustos, const std::string& NomeArquivo)
	: _Direcionado(Direcionado), _ComCustos(true) {

		using namespace std;

		ifstream Entrada(NomeArquivo, ifstream::in);

		if (!Entrada.is_open()) {
			std::cerr << "O arquivo " << NomeArquivo << " nao existe.";
		}

		Indice QuantidadeNosLidos;
		Indice IDLido;
		TipoRotulo NomeLido, NomeOrigem, NomeDestino;
		TipoPeso PesoLido;

		Entrada >> QuantidadeNosLidos;

		for (Indice i = 0; i < QuantidadeNosLidos; ++i) {
			Entrada >> IDLido >> NomeLido;
			cout << "Lido no de ID " << IDLido << " e nome " << NomeLido << std::endl;
			Inserir(new No(IDLido, NomeLido));
			_M[NomeLido] = IDLido;
			cout << "Atribuindo identificador #" << IDLido << " ao no '" << NomeLido << "'" << std::endl;
		}

		cout << _M << std::endl;

		while(!Entrada.eof()) {

			Entrada >> NomeOrigem >> NomeDestino >> PesoLido;
			Indice IDOrigem = ID(NomeOrigem);

			if (!IDOrigem) {
				cerr << "Nao existe no com nome " << NomeOrigem << "." << " Verifique o arquivo " << NomeArquivo << std::endl;
			}
			else {
				Aresta* Temporaria = new Aresta(IDOrigem, ID(NomeDestino), PesoLido);
				Inserir(Temporaria);
				cout << "Lida " << Temporaria << std::endl;
			}

		}

		Entrada.close();

		MatrizRepresentativa().Imprimir(std::cout);

}