#ifndef XMLDATALOADER_H
#define XMLDATALOADER_H

#include <map>
#include <string>
#include "util/Markup.h"
#include "Driver.h"

class XMLDataLoader
{
    public:
        XMLDataLoader();
        virtual ~XMLDataLoader();
        void loadInputData(std::string file,std::string folder);
    protected:
    private:
        void loadParameters(CMarkup xml);
        void loadTrailers(CMarkup xml);
        void loadDrivers(CMarkup xml);
        void loadBases(CMarkup xml);
		void loadSources(CMarkup xml);
		void loadCustomers(CMarkup xml);
		void loadTimeAndDistanceMatrices(CMarkup xml);

		std::multimap<int,Driver*> trailerDrivers;
};


#endif // XMLDATALOADER_H
