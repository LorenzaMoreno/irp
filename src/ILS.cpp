#include "ILS.h"
#include <iostream>
#include <stdio.h>
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
    bool bloquear = false;//controlar se é pra "barrar" o customer como o proximo
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
//  if (c!=NULL){
//   // printf("Customer mais proximo: %d\n", c->getIndex());
//  }
    if(c==NULL){
        printf("Nao ha customer mais prox da loc %d!\n",loc->getIndex());
        int a;
        scanf("%d",&a);
    }else{
        printf("O cust mais proximo eh: %d\n",c->getIndex());
    }
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

double ILS::amountSupply(Customer* customer, int time, double instant, double stockAtInstant){
    //determina quantidade a abastecer um customer, tenta garantir o suficiente para nao voltar em x tempo...
    double maxLoader= customer->getCapacity() - stockAtInstant;
    double quantity = 0.0;
    for(int i=instant;i<time+instant;i++){//varrer instantes
        double instConsumption = customer->getForecast()->at(i);
        if((quantity+instConsumption) < maxLoader){//pode abastecer
            quantity= quantity + instConsumption;
        }else{//Não dá para abastecer mais.
            break;
        }
    }
//    printf("O cliente %d precisa de %.3f de gas para garantir %d instantes\n", customer->getIndex(), quantity, time);
    return quantity;
}

Shift* ILS::criarShift(Trailer* trailer, Driver* driver, std::vector<int> locais, double tempoInicial, Solution* solution){
    printf("Visitar: ");
    for(int i = 0 ; i < locais.size() ; i++){
        printf("%d ",locais.at(i));
    }
    printf("\n");


    std::vector<int> jaVisitados;//vetor que indica os customers que já foram visitados no shift
    jaVisitados.clear();
    //A ROTA É CONSTRUIDA CONSIDERANDO SOMENTE AS LOCATIONS DADAS COMO PARAMETRO
    for(Location* l: *(InputData::getInstance()->getLocations())){
        bool aux= false;
        for(int j=0;j< locais.size();j++){
            if(locais.at(j)==l->getIndex()){
                aux=true;
                break;
            }
        }
        if(!aux) jaVisitados.push_back(l->getIndex());
    }
    Shift* shift= new Shift();//shift que será retornado pela função
    shift->setDriver(driver);
    shift->setTrailer(trailer);
    shift->setSolution(solution);
    std::vector<Stop*> stops;//vetor de stops do shift
    Base* baseTrailer= trailer->getBase();//base do trailer
    double tempoTotalShift= 0.0;//tempo acumulado do shift
    double tempoMaximoPermitido= driver->getMaxDriving();//tempo máximo que o shift pode durar
    Location* localAtual= baseTrailer;//location atual
    double arriveTime= tempoInicial;//tempo de chegada nos stops, à partir do tempo inicial
    int qtdeVisitados= 0;//controlar se visitou todos os locations da rota
    bool controleTempo= true;//controlar a restrição de tempo
    int cont=-1;
    int proximoLocalIndex =-1;

    do{//faça
        cont++;
        printf("\niteracao %d\n\n", cont);
        Location* proximoLocal=NULL;
        double quantity=0.0;
        if(trailer->getInicialQuantity() > trailer->getCapacity()*.1){//restrição de quantidade está ok?
            proximoLocal = getCustomerMaisProximo(localAtual,jaVisitados);//vai pro customer mais perto
            if(proximoLocal==NULL) break;///não tem um caminho para o próximo stop
            proximoLocalIndex = proximoLocal->getIndex();
            if(trailer->getInicialQuantity() <= quantity){//nao tem gas o suficiente
                quantity=trailer->getInicialQuantity();//abastece com o que ta no tanque mesmo
                printf("Abastecer com todo o gas do trailer: %.3f\n", quantity);
            }else{
                Customer* customer=(Customer*)proximoLocal;
                double timeToCustomer = InputData::getInstance()->getTime(localAtual->getIndex(),proximoLocalIndex);
                double timeAtCustomer = tempoTotalShift + timeToCustomer;
                double stockAtCustomerInst = solution->getStockLevelInst()->at(proximoLocalIndex).at(timeAtCustomer);
                quantity = amountSupply(customer, 120,timeAtCustomer,stockAtCustomerInst); //descobre o quanto vai abastercer esse customer
                printf("Abastecer com toda necesidade do customer: %.3f\n", quantity);
            }
            jaVisitados.push_back(proximoLocal->getIndex());//não permitir que o mesmo custumer entre num stop mais a frente


        }else{//se o trailer ta quase vazio, tem que procurar a fonte mais proxima...
            printf("Trailer precisa abastecer...\n");
            proximoLocal= getSourceMaisProximo(localAtual);
            proximoLocalIndex = proximoLocal->getIndex();
            if(proximoLocal==NULL) break;///não tem um caminho para o próximo stop??
            double quantity= trailer->getCapacity()- trailer->getInicialQuantity();//procura encher o trailer no source
//            printf("O trailer abastece %.3f e fica com tanque cheio = %f\n", quantity, trailer->getCapacity());
            trailer->setCapacity(trailer->getCapacity());
        }
        //printf("\n %d - %d \n",localAtual->getIndex(), proximoLocalIndex);
        double tempoAteProximo = InputData::getInstance()
        ->getTime(localAtual->getIndex(), proximoLocalIndex);
        if(
           !solAtual->getLocationInstStop()->at(proximoLocal->getIndex()).at(arriveTime+tempoAteProximo).empty()
        ){
          jaVisitados.push_back(proximoLocal->getIndex());//marca o cliente como visitado
          continue;//ignora o cliente que já tem um stop agendado na previsao de chegada
        }

        qtdeVisitados++;//enquando existir rota para o proximo location, atualiza o controle de visitas
        //cria o stop
        arriveTime+= InputData::getInstance()->getTime(localAtual->getIndex(), proximoLocal->getIndex());//adiciona o tempo de viagem no arriveTime acumulado

        Stop* stop= NULL;
        stop= criarStop(proximoLocal,shift,arriveTime,quantity);//cria um stop na location mais próxima do local atual
        solution->insertStopInShift(shift, stop);
        arriveTime+= proximoLocal->getSetupTime();//adiciona o tempo de setUp do location no arriveTime acumulado
        tempoTotalShift+= InputData::getInstance()->getTime(localAtual->getIndex(), proximoLocal->getIndex())+
                          proximoLocal->getSetupTime();//atualiza o tempo acumulado do shift

        localAtual= proximoLocal;//marca a proxima location como atual
        stops.push_back(stop);//inclui o stop criado no vetor de stops do shift
        double tempoAteBase= InputData::getInstance()->getTime(localAtual->getIndex(),baseTrailer->getIndex());
        controleTempo= (tempoTotalShift+tempoAteBase)<tempoMaximoPermitido;
//        cont++;
        }while(controleTempo && (qtdeVisitados< locais.size()));//enquanto a restrição de tempo não for ferida e não tiver visitado todos os locations pretendidos

    if(!controleTempo && !stops.empty())
        stops.pop_back();//remove esse stop inválido do vector
    //tempo do último stop à base
    if(!stops.empty()){
        int auxB=baseTrailer->getIndex();
        int auxL=stops.back()->getLocation()->getIndex();
        tempoTotalShift+= InputData::getInstance()->getTime(auxL, auxB);
    }
    //setando as prop do shift
    shift->setStops(stops);
    shift->setInitialInstant(tempoInicial);
    shift->setFinalInstant(tempoInicial+tempoTotalShift);
    shift->setTrailer(trailer);
    shift->setDriver(driver);
    return shift;
}

void ILS::constructor(int maxInstant, Solution* solution){
    FILE* f = fopen("solucaoInicial.csv", "w");
    int cont=0;
    for(int i=0;i<maxInstant; i++){//para cada instante i...
        for(Driver* driver: *(InputData::getDrivers())){
            if(solution->getDriverInst()->at(driver->getIndex()).at(i).empty()){//se o motorista estiver livre...
                for(Trailer*t: *(driver->getTrailers())){//atribuir um trailer disponivel para ele travalhar
                    //TODO VERIFICAR SE ESSE MOTORISTA PODE DIRIGIR ESSE CAMINHAO E AINDA SE ESSE CAMINHAO TA LIVRE.
                    if(solution->getTrailerInst()->at(t->getIndex()).at(i).empty()){
                        std::vector<int> indices;
//                        printf("Multimap: ");
                        for (std::multimap<int, Customer*>::const_iterator iter = solAtual->getSafetyLevelInst()->begin();
                            iter != solAtual->getSafetyLevelInst()->end(); ++iter ){
//                            printf("%d ",iter->second->getIndex());
                            if(iter->second->isTrailerAllowed(t)){
                                indices.push_back(iter->second->getIndex());//Add the shift on the driver's Instants list
                            }
                        }
//                        printf("\n");
                        Shift* shift= criarShift(t,driver,indices, i, solution);
                        cont++;
                        solution->insertShift(shift);
                        f = fopen("solucaoInicial.csv", "a");
                        fprintf(f, "Shift %d:\n", cont);
                        fprintf(f, "%d, ", shift->getTrailer()->getIndex());
                        fprintf(f, "%d, ", shift->getDriver()->getIndex());
                        fprintf(f, "%.3f, ", shift->getInitialInstant());
                        fprintf(f, "%.3f, ", shift->getFinalInstant());
                        fprintf(f, "%.3f, ", shift->getInitialLoad());
                        fprintf(f, "%.3f, ", shift->getQuantityDelivered());
                        fprintf(f, "%.3f\n ", shift->getRemnantLoad());
    //                    printf(">>>>o shift foi escrito no arquivo solucaoInicial.csv\n");
                        fclose(f);
                    }
                }
            }
        }
    }

}

///Busca local
void ILS::localSearch(){
///a definir...
}
