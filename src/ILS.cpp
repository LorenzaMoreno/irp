#include "ILS.h"
#include "Shift.h"
#include "Driver.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <limits>

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
    //A ROTA É CONSTRUIDA CONSIDERANDO SOMENTE PARA AS LOCATIONS DADAS COMO PARAMETRO
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

/*Construtor que gera a solucao mais trivial: shifts sao sempre da base para o cliente e do cliente para a base. NAO TEM NENHUM VALIDAÇÃO*/
void ILS::constructorOficial(int instanteInicial, int instanteFinal){
  ///calcula o instante em que os customers atingirão o safety level de estoque
  solAtual->calcSafetyLevelInst(InputData::getCustomers(),instanteInicial,instanteFinal);

  ///percorre o multimap gerado pelo calculo acima
  for (std::multimap<int, Customer*>::const_iterator iter = solAtual->getSafetyLevelInst()->begin();
      iter != solAtual->getSafetyLevelInst()->end(); ++iter ){///para cada customer que atingirá o safety level
      Customer* c = iter->second;///guarda o ponteiro para o cliente
      int instanteSafety = iter->first;///guarda int instante onde o safety level é atingido

      Trailer* t = c->getAllowedTrailers()->at(0);///pega o primeiro trailer que pode atender o customer
      Driver* d = t->getDrivers()->at(0);///pega o primeiro driver que pode dirigir o trailer

      double custoShift=0.0;///TODO: VARIAVEL QUE GUARDA O CUSTO PARA CRIAR O SHIFT//NAO ESTAMOS UTILIZANDO CUTOS AINDA...
      ///quantidade a abastecer no customer: o que falta para "encher" o cliente ou o que tem disponível no trailer
      double quantidadeAbastecer = c->getCapacity()- solAtual->getStockLevelInst()->at(c->getIndex()).at(instanteSafety);
      if(quantidadeAbastecer>t->getInicialQuantity()) quantidadeAbastecer= t->getInicialQuantity();
      t->setInitialQuantity(t->getInicialQuantity()-quantidadeAbastecer);//atualiza a qtde de gas no trailer
      ///tempo de ida da base ao cliente
      double tempoIda = InputData::getInstance()->getTime(t->getBase()->getIndex(),c->getIndex());
      ///tempo de volta do cliente à base
      double tempoVolta = InputData::getInstance()->getTime(c->getIndex(), t->getBase()->getIndex());

      ///TODO VERIFICAR OS INSTANTES DOS STOPS.
      ///tempo de chegada no cliente (tem que ser pelo menos o tempo de ida ao cliente)
      int arriveTime= instanteSafety<tempoIda? tempoIda: instanteSafety;
      Stop* stopIda = new Stop(quantidadeAbastecer, arriveTime, c);
      arriveTime += c->getSetupTime()+ tempoVolta;//atualiza o arrive time do próx stop
      Stop* stopVolta = new Stop(0.0, arriveTime, t->getBase());

      Shift* shift = new Shift(custoShift, d, t);
      stopIda->setShift(shift);
      stopVolta->setShift(shift);

      ///***fazendo a inserção dessa forma acho que o controle de estoque vai funcionar corretamente
      solAtual->insertShift(shift);
      solAtual->insertStopInShift(shift,stopIda);
      solAtual->insertStopInShift(shift,stopVolta);

      ///se esvaziou o trailer, cria um shift base->source/source->base para encher o trailer
      if(t->getInicialQuantity()< (t->getCapacity()*.3)){
        Source* source= t->getBase()->getNeighborsSources()->at(0);
        tempoIda= InputData::getInstance()->getTime(t->getBase()->getIndex(),source->getIndex());
        arriveTime+= tempoIda+ d->getMinInterShift();
        quantidadeAbastecer= t->getCapacity() - t->getInicialQuantity();
        t->setInitialQuantity(quantidadeAbastecer);//enche o trailer
        Stop* stopIdaSource = new Stop(quantidadeAbastecer,arriveTime, source);
        tempoVolta= InputData::getInstance()->getTime(source->getIndex(),t->getBase()->getIndex());
        arriveTime+= source->getSetupTime()+tempoVolta;
        Stop* stopVoltaSource = new Stop(0.0,arriveTime, t->getBase());

        Shift* shiftSource = new Shift(custoShift, d, t);
        stopIdaSource->setShift(shiftSource);
        stopVoltaSource->setShift(shiftSource);

        solAtual->insertShift(shiftSource);
        solAtual->insertStopInShift(shiftSource,stopIdaSource);
        solAtual->insertStopInShift(shiftSource,stopVoltaSource);

      }
  }
}

void ILS::constructor(int maxInstant){
    FILE* f = fopen("solucaoInicial.csv", "w");
    int cont=0;
     f = fopen("solucaoInicial.csv", "a");
                        fprintf(f, "shift; trailer; driver;arriveTime;SetupTime;Qantity");
                        fprintf(f, "\n");
                        printf("uai;....\n");

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
                        Shift* shift= criarShift(t,driver,indices, i);
                        double aaaa=0;
                        //CHECK SHIFT
                        solAtual->insertShift(shift);
                        cont++;//shift

                        for(Stop* st: *shift->getStop()){
                            fprintf(f, "%d;%d;%d;%.2f;%.2f;%.2f\n",cont,
                            shift->getTrailer()->getIndex(),
                            shift->getDriver()->getIndex(),
                            st->getArriveTime(),
                            st->getLocation()->getSetupTime(),
                            st->getQuantity());
                        }
                        fprintf(f, "\n");
                        printf("a-k-bo");
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


std::vector<Penalty> ILS::avaliarShift(Shift* shift){
/**
VERIFICA:
  -SE EM ALGUM STOP DO SHIFT O CUSTOMER NÃO FOI ATENDIDO O SUFICIENTE PARA ESTAR ACIMA DO SAFETYLEVEL
  -SE O ARRIVETIME DE ALGUM STOP SOBRESCREVEU O STOP SEGUINTE
  -SE ALGUM SHIFT DO TRAILLER SOBRESCREVEU O SHIFT SEGUINTE (ESTOU CONSIDERANDO QUE NÃO HÁ TROCA
                                                             DE DRIVER NO TRAILER, LOGO A VERIFICAÇÃO
                                                             TAMBÉM VALE PARA OS MOTORISTAS A PRINCÍPIO)
RETORNA:
  -UM VECTOR COM AS PENALIDADES ENCONTRADAS NO SHIFT PARA SER USADA POSTERIORMENTE EM UMA FÇ DE CLASSIFICAÇÃO
  DE QUALIDADE DOS SHIFTS DA SOLUTION;
**/
  std::vector<Penalty> penalidades;
  penalidades.clear();
  ///para cada stop no shift
  for(std::vector<Stop*>::iterator it=shift->getStop()->begin();
      it!=shift->getStop()->end();it++){
    Stop* stop= (Stop*) *it;
    if(instanceof<Customer>(stop->getLocation())){///verifica o safetyLevel dos Customers
      Customer* c= (Customer*)stop->getLocation();
      ///quantidade disponível no cliente contando a entrega do stop analisado
      double qtde= solAtual->getStockLevelInst()->at(c->getIndex()).at((int)stop->getArriveTime())+stop->getQuantity();
      ///se cliente está abaixo do safety level no instante
      if(qtde<c->getSafetyLevel())
        penalidades.push_back(Penalty::CUSTOMER_SAFETY_LEVEL);///adiciona a penalidade
    }
    Stop* proximoStop= (Stop*) *(it+1);
    if(proximoStop==*shift->getStop()->end()) continue;
    ///verifica se o stop atual sobrescreveu o stop seguinte
    if(stop->getArriveTime()>proximoStop->getArriveTime())
      penalidades.push_back(Penalty::STOP_ARRIVAL_TIME);///adiciona a penalidade
  }///fim para cada stop



  Trailer* trailer = shift->getTrailer();
  std::vector<int> posicoes;
  posicoes.clear();
  int indice=0;
  int indiceShift=0;
  ///para cada shift da solução

  for(Shift* s : *(solAtual->getShifts())){
    ///se o shift pertence ao trailer, coloca no vetor de posições o índice do shift no vetor da solução
    if(s->getTrailer()->getIndex()==trailer->getIndex()){
      posicoes.push_back(indice);
      if(s==shift) indiceShift= posicoes.size()-1;
    }
    indice++;
  }
  double anterior= indiceShift==0?-1:solAtual->getShifts()->at(posicoes.at(indiceShift-1))->getFinalInstant();///final do shift anterior
  double atualIni= solAtual->getShifts()->at(posicoes.at(indiceShift))->getInitialInstant();///início do shift atual
  double atualFim= solAtual->getShifts()->at(posicoes.at(indiceShift))->getFinalInstant();///final do shift atual
  double proximo= indiceShift==posicoes.size()-1?std::numeric_limits<double>::max():
                    solAtual->getShifts()->at(posicoes.at(indiceShift+1))->getInitialInstant();///início do shift seguinte
  ///verifica se houve uma sobreposição de shifts no trailer
  if((anterior>atualIni)||(atualFim>proximo))
    penalidades.push_back(Penalty::TRAILER_SHIFTS_OVERLAP);///adiciona a penalidade

  return penalidades;
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
