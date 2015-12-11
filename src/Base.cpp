#include "Base.h"

Base::Base(){
    //ctor
}
Base::Base(int index, std::vector<Base*> &neighborsBases, std::vector<Customer*> &neighborsCustomers, std::vector<Source*> &neighborsSources, Type type):
Location(index, neighborsBases, neighborsCustomers, neighborsSources, type){
    //ctor
}
Base::~Base(){

}
