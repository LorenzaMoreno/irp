#include "ILS.h"
#include "Dijkstra.h"
#include "Shift.h"
#include "Solution.h"

ILS::ILS(){
  //ctor
}

ILS::~ILS(){
  //dtor
}


///Criar um shit
///Os parâmetros são os índices na InputData
Shift* ILS::criarShift(int ixTrailer, int ixDriver, std::vector<int> loc, Solution* solAtual){
//rascunho do que deve ser feito

  ///1º passo: Setar a lista negra (ignorar locations que o trailer n atende)
  std::vector<int> lista;


  Trailer* trailer= InputData::getInstance()->findTrailer(ixTrailer);
  Driver* driver= InputData::getInstance()->findDriver(ixDriver);

  for(Customer* c : *InputData::getCustomers()){
    if(!c->isTrailerAllowed(trailer)) lista.push_back(c->getIndex());
  }

  ///2º passo: Definir a rota tomando como base a lista de clientes
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

  ///4º passo: Criar o shitft, e para cada cliente da rota criar um stop e inserir neste shift
  Shift* shift= new Shift();
  shift->setDriver(driver);
  shift->setTrailer(trailer);
  std::vector<Stop*> stops;
  double qtdInicial= trailer->getInicialQuantity();
  double qtdFinal= qtdInicial;
  double tempoShift= 0;

  for(int i=1; i<rota.size(); i++){
    Stop* stop= new Stop();
    stop->setLocation(InputData::getInstance()->findLocation(rota.at(i)));
    Customer* c= (Customer*)InputData::getInstance()->findLocation(i);
    stop->setArriveTime(c->getSetupTime());

    //tempo do shift = Distancia da location anterior a atual + tempo de setup da location atual
    tempoShift+= InputData::getInstance()->getDistance(rota.at(i-1),rota.at(i))+ c->getSetupTime();

    //qtde a ser colocada no cliente
    double instanteAtual= tempoShift-c->getSetupTime();
    double qtdAtualCliente= solAtual->getStockLevelInst()->at(c->getIndex()).at(instanteAtual);
    double qtdMinimaCliente= c->getSafetyLevel();
    //inicialmente, só clientes abaixo do safetyLevel recebem carga
    if(qtdAtualCliente<qtdMinimaCliente){
      stop->setQuantity(qtdMinimaCliente-qtdAtualCliente);//coloca o estoque do cliente "na risca" so safetyLevel
      qtdFinal-= qtdMinimaCliente-qtdAtualCliente;//debita a qtde depositada do trailer
    }


    stop->setShift(shift);
    stops.push_back(stop);
  }

  shift->setStops(stops);
  ///5º passo: 'validar?' e retornar o shift
  return shift;
}

void ILS::constructor(){

}

///Busca local
void ILS::localSearch(){
///a definir...
}
