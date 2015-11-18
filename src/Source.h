#ifndef SOURCE_H
#define SOURCE_H

#include "Location.h"


class Source : public Location {
    public:
        Source();
        Source(int index, std::vector<Base*> &neighborsBases, std::vector<Customer*> &neighborsCustomers, std::vector<Source*> &neighborsSources, int setupTime);
        virtual ~Source();

        int getSetupTime() const;

        void setSetupTime(int setupTime);

    protected:
    private:
        int setupTime_;
};

#endif // SOURCE_H
