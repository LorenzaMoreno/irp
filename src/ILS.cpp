#include "ILS.h"
#include "Dijkstra.h"

ILS::ILS(){
  //ctor
}

ILS::~ILS(){
  //dtor
}


///Criar um shit para um dia inteiro
///(inicialmente um dia inteiro, depois criar métodos de sobrescrita para períodos diferentes)
///Os outros parâmetros são os índices na InputData
Shift* criarShift(int dia, int ixTrailer, int ixDriver){
//rascunho do que deve ser feito
  ///1º passo: Gerar uma lista de clientes que pretende-se atender no dia
  ///          (obs: precisamos verificar o critério de ordenação dessa lista)
  ///2º passo: Usar o algoritmo de dijkistra para ir gerando uma rota
  ///          (obs: definir um critério de parada - ex: atendeu todos da lista ou atingiu
  ///           o limite de tempo do motorista...)
  ///3º passo: Com a rota definida, distribuir as quantidades para cada cliente
  ///4º passo: Criar o shitft, e para cada cliente da rota criar um stop e inserir neste shift

  ///5º passo: validar e retornar o shift
}

///Construtor da solução
///Verificar se é necessário colocar um parâmetro de horizonte de tempo
void ILS::constructor(){
//rascunho do que deve ser feito
  ///1º passo: Para cada dia do horizonte de tempo, criar um shift usando a função acima
  ///2º passo: Inserir cada shitf criado na solução (fazer verificação da inserção antes)
  ///3º passo: retornar a soução
}

///Busca local
void ILS::localSearch(){
///a definir...
}
