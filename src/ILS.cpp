#include "ILS.h"
#include "Dijkstra.h"

ILS::ILS(){
  //ctor
}

ILS::~ILS(){
  //dtor
}


///Criar um shit para um dia inteiro
///(inicialmente um dia inteiro, depois criar m�todos de sobrescrita para per�odos diferentes)
///Os outros par�metros s�o os �ndices na InputData
Shift* criarShift(int dia, int ixTrailer, int ixDriver){
//rascunho do que deve ser feito
  ///1� passo: Gerar uma lista de clientes que pretende-se atender no dia
  ///          (obs: precisamos verificar o crit�rio de ordena��o dessa lista)
  ///2� passo: Usar o algoritmo de dijkistra para ir gerando uma rota
  ///          (obs: definir um crit�rio de parada - ex: atendeu todos da lista ou atingiu
  ///           o limite de tempo do motorista...)
  ///3� passo: Com a rota definida, distribuir as quantidades para cada cliente
  ///4� passo: Criar o shitft, e para cada cliente da rota criar um stop e inserir neste shift

  ///5� passo: validar e retornar o shift
}

///Construtor da solu��o
///Verificar se � necess�rio colocar um par�metro de horizonte de tempo
void ILS::constructor(){
//rascunho do que deve ser feito
  ///1� passo: Para cada dia do horizonte de tempo, criar um shift usando a fun��o acima
  ///2� passo: Inserir cada shitf criado na solu��o (fazer verifica��o da inser��o antes)
  ///3� passo: retornar a sou��o
}

///Busca local
void ILS::localSearch(){
///a definir...
}
