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
        void loadInputData();
    protected:
    private:
        void loadTrailers(CMarkup xml);
        void loadDrivers(CMarkup xml);
        void loadBases(CMarkup xml);
		void loadSources(CMarkup xml);
		void loadCustomers(CMarkup xml);
		void loadTimeMatrix(CMarkup xml);
		void loadDistanceMatrix(CMarkup xml);

		std::multimap<int,Driver*> trailerDrivers;
};


#endif // XMLDATALOADER_H
