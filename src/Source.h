#ifndef SOURCE_H
#define SOURCE_H

#include "Location.h"


class Source : Location {
    public:
        Source();
        Source(int index, std::vector<Base*> neighborsBases,
                std::vector<Customer*> neighborsCustomers,
                std::vector<Source*> neighborsSources,
                int setupTime);
        virtual ~Source();

        void setSetupTime(int setupTime);
        int getSetupTime();


    protected:
    private:
        int setupTime_;
};

#endif // SOURCE_H
