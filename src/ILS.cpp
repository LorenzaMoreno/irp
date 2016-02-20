#include "ILS.h"
//#include "Dijkstra.h"
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

  return c;
}

Source* ILS::getSourceMaisProximo(Location* loc){

  std::vector<Source*> sources = *loc->getNeighborsSources();
  if(sources.empty()) return NULL;

  return sources.front();
}

///Criar um shit
///Os parâmetros são os índices na InputData
Shift* ILS::criarShift(Trailer* trailer, Driver* driver, std::vector<int> loc, double tempoInicial){
    std::vector<Stop*> stops;







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
            4 - sempre sabemos quem eh o proximo customer a ser atendido, verificar se a distancia desse customer ate a base é
            válida. (Aproveitar aqui e olhar: posso passar numa source primeiro?)


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

      Shift* shift= criarShift(t,0,indices, tempoCorrente);

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
