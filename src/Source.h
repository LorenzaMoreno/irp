#ifndef SOURCE_H
#define SOURCE_H

#include "Location.h"


class Source : public Location {
    public:
        Source();
        Source(int index, std::vector<Base*> &neighborsBases, std::vector<Customer*> &neighborsCustomers, std::vector<Source*> &neighborsSources, int setupTime, Type type);
        virtual ~Source();

        inline int getSetupTime() const{return setupTime_;}

        inline void setSetupTime(int setupTime){setupTime_ = setupTime;}

    protected:
    private:
        int setupTime_;
};

#endif // SOURCE_H
