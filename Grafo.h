#ifndef GRAFO_H
#define GRAFO_H

#include <list>
#include <map>
#include "Matriz.h"
#include <string>
#include <vector>

class Grafo {

	typedef unsigned int Indice;
	typedef std::string TipoRotulo;
	typedef int TipoPeso;
	static const int PesoNulo = -1;


	public:
		inline bool Direcionado() const { return _Direcionado; }
		inline bool ComCustos() const { return _ComCustos; }
		inline const Indice QuantidadeNos() const { return _N.size(); }
		inline const Indice QuantidadeArestas() const { Indice returnValue = _A.size(); return (Direcionado() ? returnValue : returnValue / 2); }

	class No {
		
		public:
			No() {}
			inline Indice ID() const { return _ID; }
			inline void ID(const Indice& ID) { _ID = ID; }

			inline TipoRotulo Nome() const { return _Nome; }
			inline void Nome(const TipoRotulo& Nome) { _Nome = Nome; }

			No(const Indice& ID, const TipoRotulo& Nome = "") : _ID(ID), _Nome(Nome) {}

			virtual std::ostream& Imprimir(std::ostream& o) const {
				return o << "No " << ID() << " (" << Nome() << ")";
			}

		private:
			Indice _ID;
			TipoRotulo _Nome;

	};

	typedef No* Link;
	typedef std::map<Indice, No> BancoDeNos;
	typedef std::map<Indice, Link> MapaNos;
	typedef std::map<Indice, MapaNos> Representacao;
	typedef std::map<TipoRotulo, Indice> MapaNomeIndice;

	class Aresta {
		
		private:
			Link _Origem;
			Link _Destino;
			TipoPeso _Peso;
			Aresta();

		public:
			Aresta(const Link& Origem, const Link& Destino, const TipoPeso& Peso = PesoNulo)
			  : _Origem(Origem),
				_Destino(Destino),
				_Peso(Peso) {}

			Aresta(const No& Origem, const No& Destino, const TipoPeso& Peso = PesoNulo);

			inline Link Origem() const { return _Origem; }
			inline Indice OrigemID() const { return Origem()->ID(); }
			inline TipoRotulo OrigemNome() const { return Origem()->Nome(); }
			inline Link Destino() const { return _Destino; }
			inline Indice DestinoID() const { return Destino()->ID(); }
			inline TipoRotulo DestinoNome() const { return Destino()->Nome(); }
			inline TipoPeso Peso() const { return _Peso; }

			virtual std::ostream& Imprimir(std::ostream& o) const {
				o << "Aresta (" << OrigemNome() << "," << DestinoNome() << ") = " << Peso();
				return o;
			}

	};

	typedef std::multimap<Indice, Aresta> BancoDeArestas;
	BancoDeArestas Expandir(const Indice& I);
	inline Indice ID(const TipoRotulo& X) { return _M[X]; }
	inline No getNo(const Indice& X) { return _N[X]; }
	inline No& getNoRef(const Indice& X) { return _N[X]; }
	inline Link getLink(const Indice& X) { Link Retorno = &_N[X]; return Retorno; }
	inline TipoRotulo Nome(const Indice& X) { getNo(X).Nome(); }

	Grafo(const bool& Direcionado = false, const bool& ComCustos = false)
		: _Direcionado(Direcionado), _ComCustos(ComCustos) {}

	Grafo(const bool& Direcionado, const bool& ComCustos, const std::string& NomeArquivo);

	inline bool Igual(const Indice& X, const Indice& Y) { return X == Y; }
	inline bool Igual(const No& X, const No& Y) { return Igual(X.ID(), Y.ID()); }
	inline bool Igual(const Link& X, const No& Y) { return Igual(X->ID(), Y.ID()); }

	std::ostream& Imprimir(std::ostream& o) const;
	std::ostream& ImprimirNos(std::ostream& o) const;

	bool Inserir(const No& N);
	inline bool Inserir(const Link& L) { return Inserir(*L); }

	bool Inserir(const Aresta& A);
	inline bool Inserir(const Aresta* A) { return Inserir(*A); }

	inline Grafo& operator<<(const No& N) { Inserir(N); return *this; }
	inline Grafo& operator<<(const Aresta& A) { Inserir(A); return *this; }
	inline Grafo& operator<<(const Aresta* A) { Inserir(A); return *this; }

	inline BancoDeArestas Expandir(const No& N) { return Expandir(N.ID()); }
	inline BancoDeArestas Expandir(const Link& L) { return Expandir(L->ID()); }

	typedef enum { Solucao = 1, Falha = 0, Corte = -1, Repetido = 2 } DLSResultado;

	DLSResultado DLS(
		const Link& NoAtual,
		const Link& Objetivo,
		const Indice& Limite,
		MapaNos& NosFronteira,
		MapaNos& NosVisitados,
		const Indice& ProfundidadeAtual = 1
	);

	DLSResultado DLS(const Link& NoAtual, const Link& Objetivo, const Indice& Limite);

	inline DLSResultado DLS(const TipoRotulo& NoAtual, const TipoRotulo& Objetivo, const Indice& Limite) {
		return DLS(getLink(ID(NoAtual)), getLink(ID(Objetivo)), Limite);
	}

	private:
		const bool _Direcionado;
		const bool _ComCustos;
	
		Representacao _R;
		BancoDeNos _N;
		BancoDeArestas _A;
		MapaNomeIndice _M;
		void Conectar(const Aresta& A);
		void Conectar(const Link& Origem, const Link& Destino);

		bool Existe(const Indice& i) const;
		bool Existe(const No& N) const;
		Matriz MatrizRepresentativa();

};

#endif