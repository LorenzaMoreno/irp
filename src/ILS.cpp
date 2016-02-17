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
///Os par�metros s�o os �ndices na InputData
Shift* criarShift(int ixTrailer, int ixDriver, std::vector<int> loc){
//rascunho do que deve ser feito

  ///1� passo: Setar a lista negra (ignorar locations que o trailer n atende)
  std::vector<int> lista;


  Trailer* trailer= InputData::getInstance()->findTrailer(ixTrailer);
  Driver* driver= InputData::getInstance()->findDriver(ixDriver);

  for(Customer* c : *InputData::getCustomers()){
    if(!c->isTrailerAllowed(trailer)) lista.push_back(c->getIndex());
  }

  ///2� passo: Definir a rota tomando como base a lista de clientes
  int atual= trailer->getBase()->getIndex();
  int proximo= loc.at(0);//primeiro cliente
  Dijkstra* d= new Dijkstra();
  d->setBlackList(lista);
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

  ///4� passo: Criar o shitft, e para cada cliente da rota criar um stop e inserir neste shift
  Shift* shift= new Shift();
  shift->setDriver(driver);
  shift->setTrailer(trailer);
  std::vector<Stop*> stops;
  for(int i=0; i<rota.size(); i++){
    Stop* stop= new Stop();
    stop->setLocation(InputData::getInstance()->findLocation(rota.at(i)));
    Location* c= InputData::getInstance()->findLocation(i);
    stop->setArriveTime(c->getSetupTime());
    //stop->setQuantity();
    stop->setShift(shift);
    stops.push_back(stop);
  }

  shift->setStops(stops);
  ///5� passo: 'validar?' e retornar o shift
  return shift;
}

void ILS::constructor(){

}

///Busca local
void ILS::localSearch(){
///a definir...
}
