#ifndef DATABASELOADER_H
#define DATABASELOADER_H

#include "Spelobject.h"
#include "Vijand.h"
#include "SpelobjectFactory.h"
#include "VijandFactory.h"
#include "CustomUniquePtr.h"
#include "CustomVector.h"
#include "Locatie.h"
#include <sqlite3.h>
#include <random>
#include "LocatieFactory.h"

class DatabaseLoader {
public:
    DatabaseLoader() = default;
    ~DatabaseLoader() = default;

    CustomVector<CustomUniquePtr<Vijand>> laadVijanden(const char* databaseBestand, int& count);
    CustomVector<CustomUniquePtr<Spelobject>> laadSpelobjecten(const char* databaseBestand, int& count);
    CustomVector<CustomUniquePtr<Locatie>> laadLocaties(const char* databaseBestand, int& count);
};

#endif // DATABASELOADER_H