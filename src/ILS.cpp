#include "ILS.h"
#include "Dijkstra.h"

ILS::ILS(){
  //ctor
}

ILS::~ILS(){
  //dtor
}


///Criar um shit
///Os par�metros s�o os �ndices na InputData
Shift* criarShift(int ixTrailer, int ixDriver, std::vector<int> loc){
//rascunho do que deve ser feito

  ///1� passo: Setar a lista negra (ignorar locations que o trailer n atende)
  std::vector<int> lista;


  Trailer* trailer;
  for(Trailer* t : *InputData::getTrailers()){
      if(t->getIndex()==ixTrailer){
        trailer= t;
        break;
      }
  }

  for(Customer* c : *InputData::getCustomers()){
    if(!c->isTrailerAllowed(trailer)) lista.push_back(c->getIndex());
  }

  ///2� passo: Definir as rotas
  int atual= trailer->getBase()->getIndex();
  int proximo= loc.at(0);//primeiro cliente
  Dijkstra* d= new Dijkstra();
  std::vector<int> rota = d->execDijkstra(atual,proximo);
  for(std::<int>::iterator it= loc.begin(); it!=loc.end()-1; it++){
    proximo= (int) *it;
  }
  ///4� passo: Criar o shitft, e para cada cliente da rota criar um stop e inserir neste shift

  ///5� passo: validar e retornar o shift
}

///Construtor da solu��o
///Verificar se � necess�rio colocar um par�metro de horizonte de tempo
void ILS::constructor(){
//rascunho do que deve ser feito
  ///1� passo: Para cada dia do horizonte de tempo, criar um shift usando a fun��o acima
  ///2� passo: Inserir cada shitf criado na solu��o (fazer verifica��o da inser��o antes)
  ///3� passo: retornar a sou��o
}

///Busca local
void ILS::localSearch(){
///a definir...
}
