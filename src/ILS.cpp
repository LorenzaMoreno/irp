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
      if(customers.at(i)->getIndex()!= ignorar.at(j)){
        c = customers.at(i);
        break;
      }
    }
    if(c!=NULL) break;//ja achou o primeiro.
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

double ILS::amountSupply(Customer* customer, int time){
    //determina quantidade a abastecer um customer, tenta garantir o suficiente para nao voltar em x tempo...
    double maxLoader= customer->getCapacity() - customer->getInitialQuantity();
    double quantity = 0.0;
    for(int i=0;i<time;i++){//varrer instantes
        double instConsumption = customer->getForecast()->at(i);
        if((quantity+instConsumption) < maxLoader){//pode abastecer
            quantity= quantity + instConsumption;
        }else{//Não dá para abastecer mais.
            break;
        }
    }
    printf("O cliente %d quer ser abastecido em uma quantidade de %.3f de gas\n", customer->getIndex(), quantity);
    return quantity;
}

Shift* ILS::criarShift(Trailer* trailer, Driver* driver, std::vector<int> locais, double tempoInicial){
    ///1º passo: Variáveis de controle
    Shift* shift= new Shift();//shift que será retornado pela função
    std::vector<Stop*> stops;//vetor de stops do shift
    Base* baseTrailer= trailer->getBase();//base do trailer
    double tempoTotalShift= 0;//tempo acumulado do shift
    double tempoMaximoPermitido= driver->getMaxDriving();//tempo máximo que o shift pode durar
    Location* localAtual= baseTrailer;//location atual
    double arriveTime= tempoInicial;//tempo de chegada nos stops, à partir do tempo inicial
    std::vector<int> jaVisitados;//vetor que indica os customers que já foram visitados no shift
    jaVisitados.clear();
    //coloca na lista de já visitados os locations que não estão na lista dada como parâmetro de entrada

    //A ROTA É CONSTRUIDA CONSIDERANDO SOMENTE AS LOCATIONS DADAS NA ENTRADA
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
    bool controleTempo= true;//controlar a restrição de tempo

    ///2º passo: criação dos stops
    /*O algoritmo sai do loop quando a restrição de tempo for violada(ou seja,
      o último stop inserido é inválido), ou quando não encontrou uma rota para
      todos os locations da lista se o controle de tempo foi violado, o ultimo
      stop inserido no vector é inválido
    */
    do{//faça
        Stop* stop= NULL;
        Location* proximoLocal=NULL;
        double quantity=0.0;
        double qtdAbatecer;
        if(trailer->getInicialQuantity() > trailer->getCapacity()*.1){//restrição de quantidade está ok?
            proximoLocal= getCustomerMaisProximo(localAtual,jaVisitados);//vai pro customer mais perto
            if(proximoLocal==NULL) break;///não tem um caminho para o próximo stop
            Customer* c=(Customer*)proximoLocal;
            qtdAbatecer = amountSupply(c, 120); //descobre o quanto vai abastercer esse customer
            if(trailer->getInicialQuantity() <= qtdAbatecer){
                    printf("Abastecer com todo o gas do trailer\n");
                    qtdAbatecer=trailer->getInicialQuantity();
            }//senao vai abastecer com o retorno da funcao mesmo.
            jaVisitados.push_back(proximoLocal->getIndex());//não permitir que o mesmo custumer entre num stop mais a frente
        }else{//se o trailer ta quase vazio, tem que procurar a fonte mais proxima...
            proximoLocal= getSourceMaisProximo(localAtual);
            if(proximoLocal==NULL) break;///não tem um caminho para o próximo stop??
            quantity= trailer->getCapacity()-trailer->getInicialQuantity();//procura encher o trailer no source
        }
        //std::cout<<"Ok1"<<std::endl;
        qtdeVisitados++;//enquando existir rota para o proximo location, atualiza o controle de visitas
        //cria o stop
        arriveTime+= InputData::getInstance()->getTime(localAtual->getIndex(), proximoLocal->getIndex());//adiciona o tempo de viagem no arriveTime acumulado
        stop= criarStop(proximoLocal,shift,arriveTime,quantity);//cria um stop na location mais próxima do local atual
        arriveTime+= proximoLocal->getSetupTime();//adiciona o tempo de setUp do location no arriveTime acumulado
        tempoTotalShift+= InputData::getInstance()->getTime(localAtual->getIndex(), proximoLocal->getIndex())+
                          proximoLocal->getSetupTime();//atualiza o tempo acumulado do shift

        localAtual= proximoLocal;//marca a proxima location como atual
        stops.push_back(stop);//inclui o stop criado no vetor de stops do shift
        double tempoAteBase= InputData::getInstance()->getTime(localAtual->getIndex(),baseTrailer->getIndex());
        controleTempo= (tempoTotalShift+tempoAteBase)<tempoMaximoPermitido;
        }while(controleTempo && (qtdeVisitados< locais.size()));//enquanto a restrição de tempo não for ferida e não tiver visitado todos os locations pretendidos

    if(!controleTempo && !stops.empty()) stops.pop_back();//remove esse stop inválido do vector
    /*---- Não sei se a base entra como ultimo stop no vetor, temos que verificar isso ----*/

    ///3º passo: montagem do shitf

    /*********TODO ********
    ///daqui pra baixo devemos começar a setar as propriedas do shift
    ///além de incluir nele o vetor de stops criado acima
    **********************/

    ///4º passo: retornar o shit (verificar se a validação será feita aqui)
    shift->setStops(stops);
    return shift;
}

void ILS::constructor(std::vector<Customer*>* customers, int maxInstant){
        //maxInstant deve ser menor ou igual ao total de forecasts
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

        std::vector<int> indices;
        for (std::multimap<int, Customer*>::const_iterator iter = solAtual->getSafetyLevelInst()->begin();
                iter != solAtual->getSafetyLevelInst()->end(); ++iter ){
            if(iter->second->isTrailerAllowed(t)){
                indices.push_back(iter->second->getIndex());//Add the shift on the driver's Instants list
                printf("Customer %d esta na lista do caminhao %d\n",iter->second->getIndex(), t->getIndex());
            }
        }
        ///** SÓ PRA TESTES
        Driver* d= InputData::getInstance()->getDrivers()->at(0);
        double tempoCorrente= 0;
        Shift* shift= criarShift(t,d,indices, tempoCorrente);
        std::cout<<"Nro de stops criados: "<<shift->getStop()->size()<<
        "\n----------------------FIM----------------\n";
      //std::cout<<shift->toString()<<std::endl;
//        for(int h=0;h<indices.size();h++){
//            printf("%d, " ,indices[h]);
//        }
//        printf("\nPara cada motorista livre, atribuir um caminhao..");
    }
}
///Criar um shit;;Os parâmetros são os índices na InputData

///Busca local
void ILS::localSearch(){
///a definir...
}
