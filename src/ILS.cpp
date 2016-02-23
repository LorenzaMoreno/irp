#include "ILS.h"
#include <iostream>
#include "Shift.h"
#include "Driver.h"

ILS::ILS(Solution* s){
  solAtual = s;
}

ILS::~ILS(){
  //dtor
}

Customer* ILS::getCustomerMaisProximo(Location* loc, std::vector<int> ignorar){
  std::vector<Customer*> customers = *loc->getNeighborsCustomers();
  if(customers.empty()) return NULL;
  if(ignorar.empty()) return customers.front();
  Customer* c = NULL;
  for(int i=0; i< customers.size(); i++){
    bool bloquear = false;//controlar se � pra "barrar" o customer como o proximo
    for(int j=0; j< ignorar.size(); j++){
      if(customers.at(i)->getIndex()== ignorar.at(j)){
        bloquear = true;
        break;
      }
    }//fim para j
    if(!bloquear){
      c= customers.at(i);
      break;
    }
  }//fim para i
  printf("Customer mais proximo: %d\n", c->getIndex());
  return c;

}

Source* ILS::getSourceMaisProximo(Location* loc){
  std::vector<Source*> sources = *loc->getNeighborsSources();
  if(sources.empty()) return NULL;
  return sources.front();
}

Stop* ILS::criarStop(Location* location, Shift* shift, double arriveTime, double quantity){
//    printf("\nStop criado:\nLocal=%d\nArriveTime=%.3f\nquantity=%.3f\n", location->getIndex(), arriveTime, quantity);
  Stop* s= new Stop();
  s->setArriveTime(arriveTime);
  s->setLocation(location);
  s->setQuantity(quantity);
  s->setShift(shift);
  return s;
}

double ILS::amountSupply(Customer* customer, int time){
    //determina quantidade a abastecer um customer, tenta garantir o suficiente para nao voltar em x tempo...
    double maxLoader= customer->getCapacity() - customer->getInitialQuantity();
    double quantity = 0.0;
    for(int i=0;i<time;i++){//varrer instantes
        double instConsumption = customer->getForecast()->at(i);
        if((quantity+instConsumption) < maxLoader){//pode abastecer
            quantity= quantity + instConsumption;
        }else{//N�o d� para abastecer mais.
            break;
        }
    }
//    printf("O cliente %d precisa de %.3f de gas para garantir %d tempo\n", customer->getIndex(), quantity, time);
    return quantity;
}

Shift* ILS::criarShift(Trailer* trailer, Driver* driver, std::vector<int> locais, double tempoInicial){
    std::vector<int> jaVisitados;//vetor que indica os customers que j� foram visitados no shift
    jaVisitados.clear();
    //A ROTA � CONSTRUIDA CONSIDERANDO SOMENTE AS LOCATIONS DADAS COMO PARAMETRO
    for(Location* l : *(InputData::getInstance()->getLocations())){
        bool aux= false;
        for(int j=0;j< locais.size();j++){
            if(locais.at(j)==l->getIndex()){
                aux=true;
                break;
            }
        }
        if(!aux) jaVisitados.push_back(l->getIndex());
    }
    Solution*  solution=new Solution;
    Shift* shift= new Shift();//shift que ser� retornado pela fun��o
    shift->setDriver(driver);
    shift->setTrailer(trailer);
    shift->setSolution(solution);
    std::vector<Stop*> stops;//vetor de stops do shift
    Base* baseTrailer= trailer->getBase();//base do trailer
    double tempoTotalShift= 0.0;//tempo acumulado do shift
    double tempoMaximoPermitido= driver->getMaxDriving();//tempo m�ximo que o shift pode durar
    Location* localAtual= baseTrailer;//location atual
    double arriveTime= tempoInicial;//tempo de chegada nos stops, � partir do tempo inicial
    int qtdeVisitados= 0;//controlar se visitou todos os locations da rota
    bool controleTempo= true;//controlar a restri��o de tempo
    int cont=0;
    do{//fa�a
//        printf("\niteracao %d\n\n", cont);
        Location* proximoLocal=NULL;
        double quantity=0.0;
        if(trailer->getInicialQuantity() > trailer->getCapacity()*.1){//restri��o de quantidade est� ok?
            proximoLocal = getCustomerMaisProximo(localAtual,jaVisitados);//vai pro customer mais perto
            if(proximoLocal==NULL) break;///n�o tem um caminho para o pr�ximo stop
            if(trailer->getInicialQuantity() <= quantity){//nao tem gas o suficiente
                quantity=trailer->getInicialQuantity();//abastece com o que ta no tanque mesmo
                printf("Abastecer com todo o gas do trailer: %.3f\n", quantity);
            }else{
                Customer* customer=(Customer*)proximoLocal;
                quantity = amountSupply(customer, 480); //descobre o quanto vai abastercer esse customer
                printf("Abastecer com toda necesidade do customer: %.3f\n", quantity);
            }
            jaVisitados.push_back(proximoLocal->getIndex());//n�o permitir que o mesmo custumer entre num stop mais a frente

        }else{//se o trailer ta quase vazio, tem que procurar a fonte mais proxima...
            printf("Trailer precisa abastecer...\n");
            proximoLocal= getSourceMaisProximo(localAtual);
            if(proximoLocal==NULL) break;///n�o tem um caminho para o pr�ximo stop??
            double quantity= trailer->getCapacity()- trailer->getInicialQuantity();//procura encher o trailer no source
            printf("O trailer abastece %.3f e fica com tanque cheio = %f\n", quantity, trailer->getCapacity());
            trailer->setCapacity(trailer->getCapacity());
        }
        qtdeVisitados++;//enquando existir rota para o proximo location, atualiza o controle de visitas
        //cria o stop
        arriveTime+= InputData::getInstance()->getTime(localAtual->getIndex(), proximoLocal->getIndex());//adiciona o tempo de viagem no arriveTime acumulado
        Stop* stop= NULL;
        stop= criarStop(proximoLocal,shift,arriveTime,quantity);//cria um stop na location mais pr�xima do local atual

        solution->insertStopInShift(shift, stop);
        arriveTime+= proximoLocal->getSetupTime();//adiciona o tempo de setUp do location no arriveTime acumulado
        tempoTotalShift+= InputData::getInstance()->getTime(localAtual->getIndex(), proximoLocal->getIndex())+
                          proximoLocal->getSetupTime();//atualiza o tempo acumulado do shift

        localAtual= proximoLocal;//marca a proxima location como atual
        stops.push_back(stop);//inclui o stop criado no vetor de stops do shift
        double tempoAteBase= InputData::getInstance()->getTime(localAtual->getIndex(),baseTrailer->getIndex());
        controleTempo= (tempoTotalShift+tempoAteBase)<tempoMaximoPermitido;
        cont++;

        }while(controleTempo && (qtdeVisitados< locais.size()));//enquanto a restri��o de tempo n�o for ferida e n�o tiver visitado todos os locations pretendidos

    if(!controleTempo && !stops.empty()) stops.pop_back();//remove esse stop inv�lido do vector
    tempoTotalShift+= InputData::getInstance()->getTime(stops.back()->getLocation()->getIndex(),baseTrailer->getIndex());
    shift->setStops(stops);
    shift->setInitialInstant(tempoInicial);
    shift->setFinalInstant(tempoInicial+tempoTotalShift);
    shift->setTrailer(trailer);
    shift->setDriver(driver);
    return shift;
}

void ILS::constructor(int maxInstant, Solution* solution){
    for(int i=0;i<maxInstant; i++){//para cada instante i...
//            printf("Instante %d... -> ", i);
        for(Driver* driver: *(InputData::getDrivers())){
            if(solution->getDriverInst()->at(driver->getIndex()).at(i).empty()){//se o motorista estiver livre...
//                printf("\nDriver %d esta livre no instante %d\n", driver->getIndex(), i);
                for(Trailer* t: *(InputData::getTrailers())){
                    printf("\nDriver %d esta livre no instante %d e pega o caminhao %d\n", driver->getIndex(), i, t->getIndex());
                    std::vector<int> indices;
                    for (std::multimap<int, Customer*>::const_iterator iter = solution->getSafetyLevelInst()->begin(); iter != solution->getSafetyLevelInst()->end(); ++iter ){
                        if(iter->second->isTrailerAllowed(t)){
                            indices.push_back(iter->second->getIndex());//Add the shift on the driver's Instants list
                        }
                    }

//                    printf("atender os customers: ");
//                    for(int i=1; i<indices.size();i++){
//                        printf("%d , ", indices.at(i));
//                    }

                    Shift* shift= criarShift(t,driver,indices, i);
                    solution->insertShift(shift);
                }
           }
        }
    }
}

///Busca local
void ILS::localSearch(){
///a definir...
}
