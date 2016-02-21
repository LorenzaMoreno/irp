#include "ILS.h"
#include <iostream>
#include "Shift.h"



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

  Customer* c=NULL;
  for(int i=0; i< customers.size(); i++){
    c=NULL;
    for(int j=0; j< ignorar.size(); j++){
      if(customers.at(i)->getIndex()!=ignorar.at(j)){
        c= customers.at(i);
        break;
      }
    }
    if(c!=NULL) break;
  }
  //std::cout<<(c==NULL)<<std::endl;
  return c;

}

Source* ILS::getSourceMaisProximo(Location* loc){

  std::vector<Source*> sources = *loc->getNeighborsSources();
  if(sources.empty()) return NULL;

  return sources.front();
}



Stop* ILS::criarStop(Location* location, Shift* shift, double arriveTime, double quantity){
  Stop* s= new Stop();

  s->setArriveTime(arriveTime);
  s->setLocation(location);
  s->setQuantity(quantity);
  s->setShift(shift);

  return s;

}

///Criar um shit
///Os par�metros s�o os �ndices na InputData
Shift* ILS::criarShift(Trailer* trailer, Driver* driver, std::vector<int> locais, double tempoInicial){

    ///1� passo: Vari�veis de controle
    Shift* shift= new Shift();//shift que ser� retornado pela fun��o
    std::vector<Stop*> stops;//vetor de stops do shift

    Base* baseTrailer= trailer->getBase();//base do trailer

    double tempoTotalShift= 0;//tempo acumulado do shift
    double tempoMaximoPermitido= driver->getMaxDriving();//tempo m�ximo que o shift pode durar

    Location* localAtual= baseTrailer;//location atual
    double arriveTime= tempoInicial;//tempo de chegada nos stops, � partir do tempo inicial
    std::vector<int> jaVisitados;//vetor que indica os customers que j� foram visitados no shift
    jaVisitados.clear();

    //coloca na lista de j� visitados os locations que n�o est�o na lista dada como par�metro de entrada
    //A ROTA � CONSTRUIDA CONSIDERANDO SOMENTE AS LOCATIONS DADAS NA ENTRADA
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

    int qtdeVisitados= 0;//controlar se visitou todos os locations da rota
    bool controleTempo= true;//controlar a restri��o de tempo

    ///2� passo: cria��o dos stops
    do{//fa�a
      Stop* stop= NULL;
      Location* proximoLocal=NULL;
      double quantity=0;
      if(trailer->getInicialQuantity()> trailer->getCapacity()*.1){//restri��o de quantidade est� ok?
        proximoLocal= getCustomerMaisProximo(localAtual,jaVisitados);
        if(proximoLocal==NULL) break;///n�o um caminho para o pr�ximo stop
        /*********TODO*********
        quantity= <f� que define a qtde que ser entregue no customer>
        **********************/
        jaVisitados.push_back(proximoLocal->getIndex());//n�o permitir que o mesmo custumer entre num stop mais frente
      }else{//se n�o, se a qtde est� baixa
        proximoLocal= getSourceMaisProximo(localAtual);
        if(proximoLocal==NULL) break;///n�o um caminho para o pr�ximo stop
        quantity= trailer->getCapacity()-trailer->getInicialQuantity();//procura encher o trailer no source
      }
      //std::cout<<"Ok1"<<std::endl;
      qtdeVisitados++;//enquando existir rota para o proximo location, atualiza o controle de visitas
      //cria o stop
      arriveTime+= InputData::getInstance()->getTime(localAtual->getIndex(), proximoLocal->getIndex());//adiciona o tempo de viagem no arriveTime acumulado
      stop= criarStop(proximoLocal,shift,arriveTime,quantity);//cria um stop na location mais pr�xima do local atual
      arriveTime+= proximoLocal->getSetupTime();//adiciona o tempo de setUp do location no arriveTime acumulado
      tempoTotalShift+= InputData::getInstance()->getTime(localAtual->getIndex(), proximoLocal->getIndex())+
                        proximoLocal->getSetupTime();//atualiza o tempo acumulado do shift

      localAtual= proximoLocal;//marca a proxima location como atual
      stops.push_back(stop);//inclui o stop criado no vetor de stops do shift
      double tempoAteBase= InputData::getInstance()->getTime(localAtual->getIndex(),baseTrailer->getIndex());
      controleTempo= (tempoTotalShift+tempoAteBase)<tempoMaximoPermitido;
    }while(controleTempo && (qtdeVisitados< locais.size()));//enquanto a restri��o de tempo n�o for ferida
                                                            //e n�o tiver visitado todos os locations pretendidos


    /*O algoritmo sai do loop quando a restri��o de tempo for violada(ou seja,
      o �ltimo stop inserido � inv�lido), ou quando n�o encontrou uma rota para
      todos os locations da lista se o controle de tempo foi violado, o ultimo
      stop inserido no vector � inv�lido
    */
    if(!controleTempo && !stops.empty()) stops.pop_back();//remove esse stop inv�lido do vector
    /*---- N�o sei se a base entra como ultimo stop no vetor, temos que verificar isso ----*/

    ///3� passo: montagem do shitf

    /*********TODO ********
    ///daqui pra baixo devemos come�ar a setar as propriedas do shift
    ///al�m de incluir nele o vetor de stops criado acima
    **********************/

    ///4� passo: retornar o shit (verificar se a valida��o ser� feita aqui)
    shift->setStops(stops);

    return shift;


//
//    if(trailer->getInicialQuantity()>0.1*trailer->getCapacity()){
//        //ta vazio, vai pra source mais proxima;
//        Source* source = trailer->getBase()->getNeighborsSources()->at(0);
//        Stop* stop = new Stop();
//        stop->setLocation(source);
//        double tempoChegada=tempoInicial+InputData::getTime(trailer->getBase()->getIndex(), source->getIndex());
//        stop->setArriveTime(tempoChegada);
//        double qtdAbastecer= trailer->getCapacity()-trailer->getInicialQuantity();
//        stop->setQuantity(qtdAbastecer);
////        stop->setShift()//Miguel TODO create shift;
//
//    }else{
//        //vamos visitar os clientes mais proximos que estao incluidos no vector loc.
////        Customer* customer = trailer
//    }
//rascunho do que deve ser feito
    /*

            1 - testar se o caminho ta vazio, se tiver ele vai pra source mais perto.
            2 - se nao ele vai pro customer mais perto. Ele vai fazer isso enquanto o
                trailer nao esvaziar e nem atingir o maxdrivetime.
            3 - O quanto que vai abastercer: criar uma funcao que varre o forecast do customer
                e diz quanto que ele vai gastar nos porximos n dias.
            4 - sempre sabemos quem eh o proximo customer a ser atendido, verificar se a distancia desse customer ate a base �
            v�lida. (Aproveitar aqui e olhar: posso passar numa source primeiro?)


    */

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

  InputData::getInstance()->calcNeighborsLocations();
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
      Driver* d= InputData::getInstance()->getDrivers()->at(0);///** S� PRA TESTES

      Shift* shift= criarShift(t,d,indices, tempoCorrente);

      //std::cout<<shift->getStop()->size()<<"\n----------------------FIM----------------\n";

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
