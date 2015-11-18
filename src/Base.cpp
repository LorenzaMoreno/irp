#include "Base.h"

Base::Base(){
    //ctor
}
Base::Base(int index, std::vector<Base*> &neighborsBases, std::vector<Customer*> &neighborsCustomers, std::vector<Source*> &neighborsSources):
Location(index, neighborsBases, neighborsCustomers, neighborsSources){
    //ctor
}
Base::~Base(){

}
