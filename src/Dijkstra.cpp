#include "Dijkstra.h"
#include "InputData.h"


#include <limits>


///**FONTE DE CONSULTA**: http://www.inf.ufsc.br/grafos/temas/custo-minimo/dijkstra.html

Dijkstra::Dijkstra(){
  ///inicializa��o das vari�veis
  tamanhoM= InputData::getLocations()->size();
  //matrizDist.clear();
  //matrizDist.resize(tamanhoM);
  vertices.clear();
  vertices.resize(tamanhoM,NULL);
  listaNegra.clear();
}

Dijkstra::~Dijkstra(){
//  for(int i=0; i<matrizDist.size();i++){
//    matrizDist.at(i).clear();
//  }
//  matrizDist.clear();
  vertices.clear();
  listaNegra.clear();
}

/*
Algoritmo para preencher a matriz de distancias de forma r�ndomica.
Essa fun��o s� � usada aqui nos testes. No IRP j� temos essa matriz pronta
*/
void Dijkstra::montarDistancias(){

  for(int i=0; i<tamanhoM;i++){
    matrizDist.at(i).resize(tamanhoM,0);
    for(int j=i; j<tamanhoM;j++){
      if(i==j) continue;
      matrizDist.at(i).at(j)= (rand()%10);///o valor 0 nessa matriz indica que n�o h� caminho direto entre os v�rtices
                                          ///n�o sei como isso t� sendo representado no IRP, mas � uma info importante
    }
  }
  for(int i=0; i<tamanhoM;i++){
    for(int j=i; j<tamanhoM;j++){
      if(i==j) continue;
      matrizDist.at(j).at(i)= matrizDist.at(i).at(j);
    }
  }
}


/*
Fun��o extra, pra exibir o status do q est� sendo feito
S� usada nos testes.
*/
void Dijkstra::show(){
//
//  for(int i=0; i<matrizDist.size();i++){
//    for(int j=0; j<tamanhoM;j++){
//      std::cout<<matrizDist.at(i).at(j)<<"\t";
//    }
//    std::cout<<std::endl;
//  }
  std::cout<<"------------------"<<std::endl;
  for(int i=0; i<tamanhoM; i++){
    if(vertices.at(i)==NULL) continue;
    ponto*p = vertices.at(i);
    std::cout<<"est.: "<<p->estimativa<<std::endl;
    std::cout<<"pred.: "<<p->predecessor<<std::endl;
    std::cout<<"fechado: "<<p->fechado<<std::endl;
    std::cout<<"------------------"<<std::endl;
  }

}

/***************************/
/** ALGORITMO DE DIJKSTRA **/
/***************************/

/*
Retorna o pr�ximo v�rtice em aberto (o de caminho mais pr�ximo)
*/
int Dijkstra::proximoVertice(){

  double menor= std::numeric_limits<double>::max();
  int ixMenor= -1;

  for(int i=0; i<vertices.size(); i++){
    //atualiza os �ndices de menor valor
    if(vertices.at(i)->fechado) continue;
    if(vertices.at(i)->estimativa<menor){
      menor= vertices.at(i)->estimativa;
      ixMenor= i;
    }
  }

  ///lembrar de testar se o retorno da fun��o � -1 -> isso indica que n�o h� caminho desse ponto do algoritmo em diante
  return ixMenor;
}

/*
Verifica se o v�rtice est� ou n�o na lista de ignorados
*/
bool Dijkstra::verificarVertice(int ixVertice){
  if(!listaNegra.empty()){
    for(int i=0; i<listaNegra.size();i++){
      if(listaNegra.at(i)==ixVertice) return false;
    }
  }

  return true;
}

/*
  ATUALIZA��O DAS ESTIMATIVAS
  Dado o v�rtice de origem, recalcula a estimativa de distancia
  de todos os v�rtices abertos adjacentes a ele;

*/
void Dijkstra::atualizarEstimativas(int ixSrc){

  /*
  OBS: Onde o algoritmo usa a matriz de distancia "matrizDist" dever� ser considerada a matriz do IRP
  no momento de adapta��o do algoritmo

  */
  for(int i=0; i<tamanhoM;i++){//para todo v�rtice (location)
    if(i==ixSrc) continue;///ignora o v�rtice de origem
    //if(matrizDist.at(ixSrc).at(i)==0) continue; ///ignora caminhos q n�o existem na matriz de dist.
    if(InputData::getDistance(ixSrc,i)==0) continue; ///ignora caminhos q n�o existem na matriz de dist.
    if(vertices.at(i)->fechado) continue;///ignora v�rtices j� fechados
    if(!verificarVertice(i)) continue;///ignora os v�rtices na lista negra;

    ///nova estimativa= est. do v�rtice de origem + distancia(origem, v�rtice atual)
    //double novaEstimativa= vertices.at(ixSrc)->estimativa+ matrizDist.at(ixSrc).at(i);
    double novaEstimativa= vertices.at(ixSrc)->estimativa+ InputData::getDistance(ixSrc,i);

    if(novaEstimativa> vertices.at(i)->estimativa) continue;///ignora caminhos maiores que o atual

    vertices.at(i)->estimativa= novaEstimativa;///atualiza a estimativa do v�rtice
    vertices.at(i)->predecessor= ixSrc;///atualiza o predecessor do v�rtice

  }

  vertices.at(ixSrc)->fechado= true;///fecha o n� de origem

}

/*
  VERIFICA��O DO CAMINHO
  Verifica se j� existe um caminho da origem (ixSrc) ao destino (ixTgt)
  completo no vetor de pontos
*/
bool Dijkstra::caminhoCompleto(int ixSrc, int ixTgt){

  if(vertices.at(ixTgt)->predecessor==ixSrc) return true;
  if(vertices.at(ixTgt)->predecessor<0) return false;

  int pred= vertices.at(ixTgt)->predecessor;
  while(pred!=ixSrc && vertices.at(pred)->fechado){
    pred= vertices.at(pred)->predecessor;
  }

  return pred==ixSrc;
}

/*
  OBTEN��O DO CAMINHO
  Verifica se j� existe um caminho da origem (ixSrc) ao destino (ixTgt)
  completo no vetor de pontos e retorna esse caminho caso exista
  OBS: Esta fun��o, na verdade, retorna um caminho do destino (ixTgt) �
  origem (ixSrc) "caminho inverso"
*/
std::vector<int> Dijkstra::obtemCaminhoCompleto(int ixSrc, int ixTgt){

  std::vector<int> s;
  s.clear();

  if(vertices.at(ixTgt)->predecessor<0) return s;

  if(vertices.at(ixTgt)->predecessor==ixSrc){
    s.push_back(ixTgt);
    s.push_back(ixSrc);
    return s;
  }


  s.push_back(ixTgt);
  int pred= vertices.at(ixTgt)->predecessor;
  while(pred!=ixSrc && vertices.at(pred)->fechado){
    s.push_back(pred);
    pred= vertices.at(pred)->predecessor;
  }
  s.push_back(ixSrc);
  if(pred!=ixSrc) s.clear();

  return s;
}

/*
 EXECU��O DO ALGORITMO DE DIJKSTRA
*/
std::vector<int> Dijkstra::execDijkstra(int ixSrc, int ixTgt){

  /*
  Pr�-processamento necess�rio para tornar poss�vel o fechamento da rota:
  Remover sempre o destino da lista negra, caso o mesmo j� esteja nela.
  */
  if(!listaNegra.empty()){
    int t=0;
    for(std::vector<int>::iterator it=listaNegra.begin(); it!=listaNegra.end();it++){
      if((int)*it==ixTgt) break;
      t++;
    }
    if(t<listaNegra.size()) listaNegra.erase(listaNegra.begin()+t);
  }

  /* 1- INICIALIZA��O DOS PONTOS*/
  for(int i=0; i<tamanhoM; i++){
    ponto*p = new ponto;
    p->estimativa= std::numeric_limits<double>::max();
    p->predecessor=-1;
    p->fechado=false;
    vertices.at(i)=p;
  }
  vertices.at(ixSrc)->estimativa= 0;


  /* 2- EXECU��O*/
  int contador=0;
  int ixAtual= ixSrc;
  while(contador< tamanhoM || !caminhoCompleto(ixSrc,ixTgt)){
    atualizarEstimativas(ixAtual);
    ixAtual= proximoVertice();
    contador++;
    if(ixAtual<0) break;///testa se retornou -1 (vide observa��o na f� proximoVertice)
  }


  /* 3- RESULTADOS*/
  std::vector<int> caminho = obtemCaminhoCompleto(ixSrc,ixTgt);///obt�m o caminho
  if(!caminho.empty()){
    for(int i=0; i<caminho.size(); i++){
      listaNegra.push_back(caminho.at(i));///acrescenta na lista negra os n�s visitados
    }
  }
  return caminho;///retorna o caminho (obs: esse caminho pode ser vazio, caso n�o seja poss�vel
                                    ///criar uma rota entre os pontos)
}

void Dijkstra::setBlackList(std::vector<int> lista){
    std::copy(lista.begin(),lista.end(),listaNegra.begin());
}
