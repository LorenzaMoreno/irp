#include "ILS.h"
#include "Dijkstra.h"
#include "Shift.h"



ILS::ILS(Solution* s){
  solAtual = s;
}

ILS::~ILS(){
  //dtor
}


///Criar um shit
///Os parâmetros são os índices na InputData
Shift* ILS::criarShift(Trailer* trailer, Driver* driver, std::vector<int> loc, double tempoInicial){
    std::vector<Stop*> stops;
    if(trailer.getInicialQuantity>0.1*trailer.getCapacity){
        //ta vazio, vai pra source mais proxima;
        Source* source = trailer->getBase()->getNeighborsSources()->at(0);
        Stop* stop = new Stop();
        stop->setLocation = source;
        double tempoChegada=tempoInicial+InputData::getTime(trailer->getBase()->getIndex(), source->getIndex());
        stop->setArriveTime(tempoChegada);
        double qtdAbastecer= trailer->getCapacity-trailer->getInicialQuantity;
        stop->setQuantity(qtdAbastecer);
//        stop->setShift()//Miguel TODO create shift;

    }else{
        //vamos visitar os clientes mais proximos que estao incluidos no vector loc.
//        Customer* customer = trailer
    }
//rascunho do que deve ser feito
    /*

            1 - testar se o caminho ta vazio, se tiver ele vai pra source mais perto.
            2 - se nao ele vai pro customer mais perto. Ele vai fazer isso enquanto o
                trailer nao esvaziar e nem atingir o maxdrivetime.
            3 - O quanto que vai abastercer: criar uma funcao que varre o forecast do customer
                e diz quanto que ele vai gastar nos porximos n dias.
            4 - sempre sabemos quem eh o proximo customer a ser atendido, verificar se a distancia desse customer ate a base é
            válida. (Aproveitar aqui e olhar: posso passar numa source primeiro?)


    */


  ///1º passo: Setar a lista negra (ignorar locations que o trailer n atende)
  std::vector<int> lista;

    std::cout<<"ok1---------"<<std::endl;

  Trailer* trailer= InputData::getInstance()->findTrailer(ixTrailer);
  Driver* driver= InputData::getInstance()->findDriver(ixDriver);

  for(Customer* c : *InputData::getCustomers()){
    if(!c->isTrailerAllowed(trailer)) lista.push_back(c->getIndex());
  }

    std::cout<<"ok2---------"<<std::endl;
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

    std::cout<<"ok3---------"<<std::endl;
  ///3º passo: Criar o shitft, e para cada cliente da rota criar um stop e inserir neste shift
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
    if(c->getCapacity()>qtdAtualCliente+0.1*qtdMinimaCliente){
      stop->setQuantity(0.1*qtdMinimaCliente);//coloca o estoque do cliente "na risca" so safetyLevel
      qtdFinal-= 0.1*qtdMinimaCliente;//debita a qtde depositada do trailer
    }


    std::cout<<"ok4---------"<<std::endl;
    stop->setShift(shift);
    stops.push_back(stop);
  }

  shift->setStops(stops);
  shift->setInitialInstant(tempoInicial);
  shift->setFinalInstant(tempoInicial+tempoShift);
  ///5º passo: 'validar?' e retornar o shift
  return shift;
}

void ILS::constructor(std::vector<Customer*>* customers, int maxInstant){
        //maxInstant deve ser menor ou igual ao total de forecasts
        //TODO: ATUALIZAR ESSA FUNCAO PARA ELA NAO PARTIR SOMENTO DO INSTANTE INICIAL
    for(Customer* customer: *(customers)){
        double safetyLevel= customer->getSafetyLevel();
        double quantity = customer->getInitialQuantity();
        int i;
        for(i=0;i<maxInstant;i++){//varrer instantes
            double instConsumption= customer->getForecast()->at(i);
            quantity= quantity - instConsumption;
            if(quantity < safetyLevel){//estorou
                solAtual->getSafetyLevelInst()->insert (std::make_pair(i,customer));
                printf("O customer %d atingiu o safety no instante %d\n", customer->getIndex(), i);
                break;
            }
        }
    }

// Dividir customers por trailers
    for(Trailer* t: *(InputData:: getTrailers())){
        double tempoCorrente= 0;
        std::vector<int> indices;
        for (std::multimap<int, Customer*>::const_iterator iter = solAtual->getSafetyLevelInst()->begin();
                iter != solAtual->getSafetyLevelInst()->end(); ++iter ){
            if(iter->second->isTrailerAllowed(t)){
                indices.push_back(iter->second->getIndex());//Add the shift on the driver's Instants list
                printf("Customer %d foi inserido na lista do caminhao %d\n",iter->second->getIndex(), t->getIndex());
            }
        }

      Shift* shift= criarShift(t->getIndex(),0,indices, tempoCorrente);

      //validar shift
      double custoShift=0;
//      if(solAtual->checkShift(shift,&custoShift)==Penalty::NO_PENALTIES){
//
//
//      }

      //std::cout<<shift->toString()<<std::endl;
      //chamar a funcao do rondinelli (indices, t) que retorna um shift;
        for(int h=0;h<indices.size();h++){
            printf(" %d " ,indices[h]);
        }
        printf("\n");
    }


}



///Busca local
void ILS::localSearch(){
///a definir...
}
