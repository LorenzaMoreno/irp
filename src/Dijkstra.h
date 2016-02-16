#ifndef DIJKSTRA_H
#define DIJKSTRA_H


#include <iostream>
#include <vector>
#include<cstdlib>

/*
Essa struct é usada como referência na hora de executar o algoritmo,
ela armazena o status de cada vértice na situação atual
*/
struct ponto{
  double estimativa;//parametro pro algoritmo de dijkstra
  int predecessor;//armazenar o vértice anterior (precedente)
  bool fechado;//indicar se o melhor caminho até o vértice já foi calculado
};

class Dijkstra
{
  public:
    Dijkstra();
    virtual ~Dijkstra();
    void montarDistancias();
    void show();

    int proximoVertice();
    bool verificarVertice(int ixVertice);
    void atualizarEstimativas(int ixSrc);
    bool caminhoCompleto(int ixSrc, int ixTgt);
    std::vector<int> obtemCaminhoCompleto(int ixSrc, int ixTgt);
    std::vector<int> execDijkstra(int ixSrc, int ixTgt);

    void setBlackList(std::vector<int> lista);
  protected:
  private:
    std::vector<std::vector<double> > matrizDist;///matriz de distancia entre os vértices (tem isso no IRP)
    std::vector<ponto*> vertices;///vetor de pontos(usado no algoritmo)
    std::vector<int> listaNegra;///lista negra: marca os pontos já visitados em uma sub-rota e que não
                                ///serão usados novamente para a construção de uma nova sub-rota

    int tamanhoM;///quantidade de vértices do grafo (nro de Locations no IRP)
};

#endif // DIJKSTRA_H
