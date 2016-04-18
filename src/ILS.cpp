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

Shift* ILS::criarShift(Trailer* trailer, Driver* driver, std::vector<int> locais, double tempoInicial){
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
    std::vector<Stop*> stops;//vetor de stops do shift
    Base* baseTrailer= trailer->getBase();//base do trailer
    double tempoTotalShift= 0.0;//tempo acumulado do shift
    double tempoMaximoPermitido= driver->getMaxDriving();//tempo máximo que o shift pode durar
    Location* localAtual= baseTrailer;//location atual
    double arriveTime= tempoInicial;//tempo de chegada nos stops, à partir do tempo inicial
    int qtdeVisitados= 0;//controlar se visitou todos os locations da rota
    bool controleTempo= true;//controlar a restrição de tempo
//    int cont=-1;
    int proximoLocalIndex =-1;
    double stockTrailer=trailer->getInicialQuantity();//controlar quantidade de gas no caminhao
    double quantity=0.0;
    do{//faça
//        cont++;
//        printf("\niteracao %d\n\n", cont);
        Location* proximoLocal=NULL;

        if(stockTrailer > trailer->getCapacity()*.1){//restrição de quantidade está ok?
            proximoLocal = getCustomerMaisProximo(localAtual,jaVisitados);//vai pro customer mais perto
            if(proximoLocal==NULL) break;///não tem um caminho para o próximo stop
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
            jaVisitados.push_back(proximoLocal->getIndex());//não permitir que o mesmo custumer entre num stop mais a frente
        }
        else{//se o trailer ta quase vazio, tem que procurar a fonte mais proxima...
//            printf("Trailer precisa abastecer...\n");
            proximoLocal= getSourceMaisProximo(localAtual);
            proximoLocalIndex = proximoLocal->getIndex();
            if(proximoLocal==NULL) break;///não tem um caminho para a prox source??
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
          continue;//ignora o cliente que já tem um stop agendado na previsao de chegada
        }

        qtdeVisitados++;//enquando existir rota para o proximo location, atualiza o controle de visitas
        //cria o stop
        arriveTime= arriveTime+tempoAteProximo;//adiciona o tempo de viagem no arriveTime acumulado
        Stop* stop= NULL;
        stop= criarStop(proximoLocal,shift,arriveTime,quantity);//cria um stop na location mais próxima do local atual
        //solution->insertStopInShift(shift, stop);

        arriveTime+= proximoLocal->getSetupTime();//adiciona o tempo de setUp do location no arriveTime acumulado

        tempoTotalShift+= tempoAteProximo+ proximoLocal->getSetupTime();//atualiza o tempo acumulado do shift

        localAtual= proximoLocal;//marca a proxima location como atual
        stops.push_back(stop);//inclui o stop criado no vetor de stops do shift
        double tempoAteBase= InputData::getInstance()->getTime(localAtual->getIndex(),baseTrailer->getIndex());
        controleTempo= (tempoTotalShift+tempoAteBase)<tempoMaximoPermitido;

        solAtual->calcSafetyLevelInst(InputData::getInstance()->getCustomers(),arriveTime,120+arriveTime);
//        cont++;
        }while(controleTempo && (qtdeVisitados< locais.size()));//enquanto a restrição de tempo não for ferida e não tiver visitado todos os locations pretendidos
    if(!controleTempo && !stops.empty())//o último stop inserido feriu uma restrição
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

///Perturbação
void ILS::perturbation(Solution *s){

  ///para cada trailer
  for(Trailer* trailer : *(InputData::getTrailers())){
    std::vector<int> posicoes;
    posicoes.clear();
    int indice=0;
    ///para cada shift da solução

    for(Shift* shift : *(s->getShifts())){
      ///se o shift pertence ao trailer, coloca no vetor de posições o índice do shift no vetor da solução
      if(shift->getTrailer()->getIndex()==trailer->getIndex())
        posicoes.push_back(indice);
      indice++;
    }

    /**----------------------------
      Após esse loop, temos um vetor chamado "posicoes" que contém os índices de todos os shifts do trailer
      na solução S (no vetor de shifts da solução). A estratégia de perturbação vai ser trocar pelo menos
      dois shifts dessas posições de lugar!
    ----------------------------**/


    ///**TROCA
    //sorteio randomico de 2 posições que serão trocadas
    int pos1= rand()%posicoes.size();
    int pos2;
    do{
      pos2= rand()%posicoes.size();
    }while(pos1==pos2);//certificar que serão sempre 2 valores DIFERENTES sorteados
    //troca propriamente dita
    Shift* shiftAux= s->getShifts()->at(pos1);
    s->getShifts()->at(pos1)= s->getShifts()->at(pos2);
    s->getShifts()->at(pos2)= shiftAux;

    /**TODO:
        - Função para atualizar os tempos dos shifts do trailer(início e término) além dos tempos dos stops
        desses shifts que também terão que ser atualizados, pois isso tudo pode ter sido afetado pela troca
        - Função pra fazer as correções de Safety Level dos costumers afetados pela troca e
        correção dos tempos dos shifts/stops dos shifts do trailer
    **/



  }///fim para cada trailer


}

///Busca local
void ILS::localSearch(Solution *s){
  /**
    A IDÉIA É CRIAR UM SISTEMA DE BUSCA LOCAL FOCADA NOS SHIFTS JÁ EXISTENTES NA SOLUÇÃO
    SERIA TIPO TENTAR FAZER UMA "MELHORA LOCAL" NOS SHIFTS PRA REDUZIR AS PENALIDADES
    DA SOLUÇÃO COMO UM TODO.

    COMO JÁ TEMOS UM VECTOR QUE ARMAZENA TODOS OS SHIFTS DA SOLUÇÃO, PODEMOS CRIAR UM CRITÉRIO
    PARA "ORDENAR" ESSES SHIFTS DE MODO QUE AQUELES QUE PAREÇAM "RUINS" FIQUEM NA FRENTE.

    A "MELHORA LOCAL" SERIA UMA TENTATIVA DE REDUZIR PENALIDADES DOS SHIFTS E MELHORAR O
    CUSTO/BENEFÍCIO DELES NA SOLUÇÃO. PODEMOS FAZER ISSO ALTERANDO ROTAS, TROCANDO CLIENTES,
    QUANTIDADES ENTREGUES NOS STOPS, ETC...

    O NRO DE VEZES QUE O ALGORITMO TENTARIA FAZER ISSO DEVERÁ SER LIMITADO POR UM NRO DE
    TENTATIVAS, OU POR TEMPO, OU ATÉ ENCONTRAR UMA SOLUÇÃO QUE CONSIDEREMOS SATISFATÓRIA.
  **/


}

void ILS::exec_ILS(int maxInstant){

  constructor(maxInstant);///construtor

  /** TODO: construtor de cópia na Solution*/
  //Solution *sBest= new Solution();//cópia da solAtual
  while(true){//definir critério de parada
    //Solution *sLinha= new Solution();//cria uma cópia da sBest
    //perturbation(sLinha);//perturbação em sLinha
    //localSearch(sLinha);//busca local em sLinha
    //if(custo de sLinha < custo de sBest)
      /** TODO: função que cria uma cópia de ums Solution*/
      //sBest = cópia de sLinha
    //delete sLinha
  }

}
