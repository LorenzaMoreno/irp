#include "ILS.h"
#include "Dijkstra.h"
#include "Shift.h"

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

  ///2º passo: Definir a rota tomando como base a lista de clientes
  int atual= trailer->getBase()->getIndex();
  int proximo= loc.at(0);//primeiro cliente
  Dijkstra* d= new Dijkstra();
  std::vector<int> rota = d->execDijkstra(atual,proximo);
  rota.pop_back();
  for(int i=1; i<loc.size(); i++){
    atual= proximo;
    proximo= loc.at(i);
    std::vector<int> aux = d->execDijkstra(atual,proximo);
    aux.pop_back();
    std::copy(aux.begin(),aux.end(), rota.end());
  }
  rota.push_back(trailer->getBase()->getIndex());

  ///4º passo: Criar o shitft, e para cada cliente da rota criar um stop e inserir neste shift
  Shift* shift= new Shift();
  std::vector<Stop*> stops;
  for(int i=0; i<rota.size(); i++){
    Stop* stop= new Stop();
    //stop->setLocation(InputData::findLocation(rota.at(i)));
    //stop->setArriveTime();
    //stop->setQuantity();
    stop->setShift(shift);
    stops.push_back(stop);
  }

  ///5º passo: validar e retornar o shift
  shift->setStops(stops);
  return shift;
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
