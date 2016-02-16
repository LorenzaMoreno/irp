#include "ILS.h"
#include "Dijkstra.h"

ILS::ILS(){
  //ctor
}

ILS::~ILS(){
  //dtor
}


///Criar um shit
///Os parâmetros são os índices na InputData
Shift* criarShift(int ixTrailer, int ixDriver, std::vector<int> loc){
//rascunho do que deve ser feito

  ///1º passo: Setar a lista negra (ignorar locations que o trailer n atende)
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

  ///2º passo: Definir as rotas
  int atual= trailer->getBase()->getIndex();
  int proximo= loc.at(0);//primeiro cliente
  Dijkstra* d= new Dijkstra();
  std::vector<int> rota = d->execDijkstra(atual,proximo);
  for(std::<int>::iterator it= loc.begin(); it!=loc.end()-1; it++){
    proximo= (int) *it;
  }
  ///4º passo: Criar o shitft, e para cada cliente da rota criar um stop e inserir neste shift

  ///5º passo: validar e retornar o shift
}

///Construtor da solução
///Verificar se é necessário colocar um parâmetro de horizonte de tempo
void ILS::constructor(){
//rascunho do que deve ser feito
  ///1º passo: Para cada dia do horizonte de tempo, criar um shift usando a função acima
  ///2º passo: Inserir cada shitf criado na solução (fazer verificação da inserção antes)
  ///3º passo: retornar a soução
}

///Busca local
void ILS::localSearch(){
///a definir...
}
