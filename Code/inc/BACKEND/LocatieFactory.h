#ifndef LOCATIEFACTORY_H
#define LOCATIEFACTORY_H

#include "Locatie.h"
#include "VijandFactory.h"
#include "SpelobjectFactory.h"
#include "DatabaseLoader.h"
#include "CustomUniquePtr.h"
#include "CustomVector.h"

class LocatieFactory {
public:
    static Locatie* CreateLocatie(int id, const char* naam, const char* algemeneOmschrijving, const char* gedetailleerdeOmschrijving,
                           const char* noord, const char* oost, const char* zuid, const char* west,
                           const char* vijanden, const char* objectenVerborgen, const char* objectenZichtbaar, const char* databaseBestand);
};

#endif // LOCATIEFACTORY_H