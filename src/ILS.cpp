#include "ILS.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
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
        }else{//N�o d� para abastecer mais.
            break;
        }
    }
//    printf("O cliente %d precisa de %.3f de gas para garantir %d instantes\n", customer->getIndex(), quantity, time);
    return quantity;
}

Shift* ILS::criarShift(Trailer* trailer, Driver* driver, std::vector<int> locais, double tempoInicial){
    std::vector<int> jaVisitados;//vetor que indica os customers que j� foram visitados no shift
    jaVisitados.clear();
    //A ROTA � CONSTRUIDA CONSIDERANDO SOMENTE AS LOCATIONS DADAS COMO PARAMETRO
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
    Shift* shift= new Shift();//shift que ser� retornado pela fun��o
    shift->setDriver(driver);
    shift->setTrailer(trailer);
    std::vector<Stop*> stops;//vetor de stops do shift
    Base* baseTrailer= trailer->getBase();//base do trailer
    double tempoTotalShift= 0.0;//tempo acumulado do shift
    double tempoMaximoPermitido= driver->getMaxDriving();//tempo m�ximo que o shift pode durar
    Location* localAtual= baseTrailer;//location atual
    double arriveTime= tempoInicial;//tempo de chegada nos stops, � partir do tempo inicial
    int qtdeVisitados= 0;//controlar se visitou todos os locations da rota
    bool controleTempo= true;//controlar a restri��o de tempo
//    int cont=-1;
    int proximoLocalIndex =-1;
    double stockTrailer=trailer->getInicialQuantity();//controlar quantidade de gas no caminhao
    double quantity=0.0;
    do{//fa�a
//        cont++;
//        printf("\niteracao %d\n\n", cont);
        Location* proximoLocal=NULL;

        if(stockTrailer > trailer->getCapacity()*.1){//restri��o de quantidade est� ok?
            proximoLocal = getCustomerMaisProximo(localAtual,jaVisitados);//vai pro customer mais perto
            if(proximoLocal==NULL) break;///n�o tem um caminho para o pr�ximo stop
            Customer* customer=(Customer*)proximoLocal;
            proximoLocalIndex = proximoLocal->getIndex();
            double timeToCustomer = InputData::getInstance()->getTime(localAtual->getIndex(),proximoLocalIndex);
            double timeAtCustomer = tempoTotalShift + timeToCustomer;
            double stockAtCustomerInst = solAtual->getStockLevelInst()->at(proximoLocalIndex).at(timeAtCustomer);
            quantity = amountSupply(customer, 60,timeAtCustomer,stockAtCustomerInst); //descobre o quanto vai abastercer esse customer
            if(stockTrailer <= quantity){//nao tem gas o suficiente
                quantity=stockTrailer;//abastece com o que ta no tanque mesmo
//                printf("Abastecer com todo o gas do trailer: %.3f\n", quantity);
            }
            stockTrailer= stockTrailer - quantity;
            jaVisitados.push_back(proximoLocal->getIndex());//n�o permitir que o mesmo custumer entre num stop mais a frente
        }
        else{//se o trailer ta quase vazio, tem que procurar a fonte mais proxima...
//            printf("Trailer precisa abastecer...\n");
            proximoLocal= getSourceMaisProximo(localAtual);
            proximoLocalIndex = proximoLocal->getIndex();
            if(proximoLocal==NULL) break;///n�o tem um caminho para a prox source??
            quantity= trailer->getCapacity()- stockTrailer;//procura encher o trailer no source
//            printf("O trailer abastece %.3f e fica com tanque cheio = %f\n", quantity, trailer->getCapacity());
            trailer->setInitialQuantity(quantity);
            stockTrailer=trailer->getCapacity();
            qtdeVisitados--;
        }
        //printf("\n %d - %d \n",localAtual->getIndex(), proximoLocalIndex);

        double tempoAteProximo = InputData::getInstance()->getTime(localAtual->getIndex(), proximoLocalIndex);
        if((tempoAteProximo+arriveTime)>716.9) break;
        if(!solAtual->getLocationInstStop()->at(proximoLocal->getIndex()).at(arriveTime+tempoAteProximo).empty()){
          jaVisitados.push_back(proximoLocal->getIndex());//marca o cliente como visitado
          continue;//ignora o cliente que j� tem um stop agendado na previsao de chegada
        }

        qtdeVisitados++;//enquando existir rota para o proximo location, atualiza o controle de visitas
        //cria o stop
        arriveTime= arriveTime+tempoAteProximo;//adiciona o tempo de viagem no arriveTime acumulado
        Stop* stop= NULL;
        stop= criarStop(proximoLocal,shift,arriveTime,quantity);//cria um stop na location mais pr�xima do local atual
        //solution->insertStopInShift(shift, stop);

        arriveTime+= proximoLocal->getSetupTime();//adiciona o tempo de setUp do location no arriveTime acumulado

        tempoTotalShift+= tempoAteProximo+ proximoLocal->getSetupTime();//atualiza o tempo acumulado do shift

        localAtual= proximoLocal;//marca a proxima location como atual
        stops.push_back(stop);//inclui o stop criado no vetor de stops do shift
        double tempoAteBase= InputData::getInstance()->getTime(localAtual->getIndex(),baseTrailer->getIndex());
        controleTempo= (tempoTotalShift+tempoAteBase)<tempoMaximoPermitido;

        solAtual->calcSafetyLevelInst(InputData::getInstance()->getCustomers(),arriveTime,120+arriveTime);
//        cont++;
        }while(controleTempo && (qtdeVisitados< locais.size()));//enquanto a restri��o de tempo n�o for ferida e n�o tiver visitado todos os locations pretendidos
    if(!controleTempo && !stops.empty())//o �ltimo stop inserido feriu uma restri��o
        stops.pop_back();//remove esse stop inv�lido do vector

    //tempo do �ltimo stop � base
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
    shift->setSolution(solAtual);
    return shift;
}

void ILS::constructor(int maxInstant){
    FILE* f = fopen("solucaoInicial.csv", "w");
    int cont=0;
    for(int i=0;i<maxInstant; i++){//para cada instante i...
        for(Driver* driver: *(InputData::getDrivers())){
            if(solAtual->getDriverInst()->at(driver->getIndex()).at(i).empty()){//se o motorista estiver livre...
                for(Trailer*t: *(driver->getTrailers())){//atribuir um trailer disponivel para ele travalhar
                    //TODO VERIFICAR SE ESSE MOTORISTA PODE DIRIGIR ESSE CAMINHAO E AINDA SE ESSE CAMINHAO TA LIVRE.
                    if(solAtual->getTrailerInst()->at(t->getIndex()).at(i).empty()){
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
                        Shift* shift= criarShift(t,driver,indices, i);
                        double aaaa=0;
//                        if(solAtual->checkShift(shift,&aaaa)==Penalty::NO_PENALTIES)
                            solAtual->insertShift(shift);
                        cont++;
                        f = fopen("solucaoInicial.csv", "a");
                        /*
                        fprintf(f, "Shift %d:\n", cont);
                        fprintf(f, "%d, ", shift->getTrailer()->getIndex());
                        fprintf(f, "%d, ", shift->getDriver()->getIndex());
                        fprintf(f, "%.3f, ", shift->getInitialInstant());
                        fprintf(f, "%.3f\n", shift->getFinalInstant());
                        for(Stop* st: *shift->getStop())
                            fprintf(f, "%d(%.2f)\t", st->getLocation()->getIndex(),st->getQuantity());
                        */
                        fprintf(f, "Shift %d;%d;%d;%.3f;%.3f\n", cont, shift->getTrailer()->getIndex(),
                                shift->getDriver()->getIndex(),
                                shift->getInitialInstant(),
                                shift->getFinalInstant());
                        for(Stop* st: *shift->getStop()){
                          fprintf(f, ";%d;%.2f;%.2f;%.2f\n",st->getLocation()->getIndex(),
                                  st->getArriveTime(),
                                  st->getLocation()->getSetupTime(),
                                  st->getQuantity());
                        }
                        fprintf(f, "\n");
                        fclose(f);
                    }
                }
            }
        }
    }
    printf("A solucao foi gerada no arquivo solucaoInicial\nCusto: \t\n");

}

void ILS::verificarSolucao(){
  for(Shift* shift : *solAtual->getShifts()){
    double cost;
    std::cout<<Penalties::toString(solAtual->checkShift(shift,&cost))<<std::endl;
  }
}

///Perturba��o
void ILS::perturbation(Solution *s){

  ///para cada trailer
  for(Trailer* trailer : *(InputData::getTrailers())){
    std::vector<int> posicoes;
    posicoes.clear();
    int indice=0;
    ///para cada shift da solu��o

    for(Shift* shift : *(s->getShifts())){
      ///se o shift pertence ao trailer, coloca no vetor de posi��es o �ndice do shift no vetor da solu��o
      if(shift->getTrailer()->getIndex()==trailer->getIndex())
        posicoes.push_back(indice);
      indice++;
    }

    /**----------------------------
      Ap�s esse loop, temos um vetor chamado "posicoes" que cont�m os �ndices de todos os shifts do trailer
      na solu��o S (no vetor de shifts da solu��o). A estrat�gia de perturba��o vai ser trocar pelo menos
      dois shifts dessas posi��es de lugar!
    ----------------------------**/


    ///**TROCA
    //sorteio randomico de 2 posi��es que ser�o trocadas
    int pos1= rand()%posicoes.size();
    int pos2;
    do{
      pos2= rand()%posicoes.size();
    }while(pos1==pos2);//certificar que ser�o sempre 2 valores DIFERENTES sorteados
    //troca propriamente dita
    Shift* shiftAux= s->getShifts()->at(pos1);
    s->getShifts()->at(pos1)= s->getShifts()->at(pos2);
    s->getShifts()->at(pos2)= shiftAux;

    /**TODO:
        - Fun��o para atualizar os tempos dos shifts do trailer(in�cio e t�rmino) al�m dos tempos dos stops
        desses shifts que tamb�m ter�o que ser atualizados, pois isso tudo pode ter sido afetado pela troca
        - Fun��o pra fazer as corre��es de Safety Level dos costumers afetados pela troca e
        corre��o dos tempos dos shifts/stops dos shifts do trailer
    **/



  }///fim para cada trailer


}

///Busca local
void ILS::localSearch(Solution *s){
  /**
    A ID�IA � CRIAR UM SISTEMA DE BUSCA LOCAL FOCADA NOS SHIFTS J� EXISTENTES NA SOLU��O
    SERIA TIPO TENTAR FAZER UMA "MELHORA LOCAL" NOS SHIFTS PRA REDUZIR AS PENALIDADES
    DA SOLU��O COMO UM TODO.

    COMO J� TEMOS UM VECTOR QUE ARMAZENA TODOS OS SHIFTS DA SOLU��O, PODEMOS CRIAR UM CRIT�RIO
    PARA "ORDENAR" ESSES SHIFTS DE MODO QUE AQUELES QUE PARE�AM "RUINS" FIQUEM NA FRENTE.

    A "MELHORA LOCAL" SERIA UMA TENTATIVA DE REDUZIR PENALIDADES DOS SHIFTS E MELHORAR O
    CUSTO/BENEF�CIO DELES NA SOLU��O. PODEMOS FAZER ISSO ALTERANDO ROTAS, TROCANDO CLIENTES,
    QUANTIDADES ENTREGUES NOS STOPS, ETC...

    O NRO DE VEZES QUE O ALGORITMO TENTARIA FAZER ISSO DEVER� SER LIMITADO POR UM NRO DE
    TENTATIVAS, OU POR TEMPO, OU AT� ENCONTRAR UMA SOLU��O QUE CONSIDEREMOS SATISFAT�RIA.
  **/


}

void ILS::exec_ILS(int maxInstant){

  constructor(maxInstant);///construtor

  /** TODO: construtor de c�pia na Solution*/
  //Solution *sBest= new Solution();//c�pia da solAtual
  while(true){//definir crit�rio de parada
    //Solution *sLinha= new Solution();//cria uma c�pia da sBest
    //perturbation(sLinha);//perturba��o em sLinha
    //localSearch(sLinha);//busca local em sLinha
    //if(custo de sLinha < custo de sBest)
      /** TODO: fun��o que cria uma c�pia de ums Solution*/
      //sBest = c�pia de sLinha
    //delete sLinha
  }

}
