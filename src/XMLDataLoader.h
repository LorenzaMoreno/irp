#ifndef XMLDATALOADER_H
#define XMLDATALOADER_H

#include <string>
#include "util/Markup.h"

class XMLDataLoader
{
    public:
        XMLDataLoader();
        virtual ~XMLDataLoader();
        void LoadInputData();
    protected:
    private:
        void LoadTrailers(CMarkup xml);
        void LoadDrivers(CMarkup xml);
        void LoadBases(CMarkup xml);
		void LoadSources(CMarkup xml);
		void LoadCustomers(CMarkup xml);
		void LoadTimeMatrix(CMarkup xml);
		void LoadDistanceMatrix(CMarkup xml);
};


#endif // XMLDATALOADER_H
