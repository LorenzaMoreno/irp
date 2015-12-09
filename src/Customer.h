#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "Location.h"
#include "Trailer.h"

class Customer : public Location {
    public:
        Customer();
        Customer(double capacity, int setupTime, std::vector<double>& forecast, double initialQuantity, double safetyLevel, std::vector<double>& totalLoadSafety);
        virtual ~Customer();

        inline double getCapacity() const{return capacity_;}
        inline int getSetupTime() const{return setupTime_;}
        inline std::vector<Trailer*>* getAllowedTrailer(){return &allowedTrailer_;}
        inline std::vector<double>* getForecast(){return &forecast_;}
        inline double getInitialQualtity() const{return initialQuantity_;}
        inline double getSafetyLevel() const{return safetyLevel_;}
        inline std::vector<double>* getTotalLoadSafety(){return &totalLoadSafety_;}

        inline void setCapacity(double capacity){capacity_ = capacity;}
        inline void setSetupTime(int setupTime){setupTime_ = setupTime;}
        inline void setInitialQualtity(double initialQuantity){initialQuantity_ = initialQuantity;}
        inline void setSafetyLevel(double safetyLevel){safetyLevel_ = safetyLevel;}

    protected:
    private:
        double capacity_;
        int setupTime_;
        std::vector<Trailer*> allowedTrailer_;
        std::vector<double> forecast_;
        double initialQuantity_;
        double safetyLevel_;
        std::vector<double> totalLoadSafety_;
};

#endif // CUSTOMER_H
