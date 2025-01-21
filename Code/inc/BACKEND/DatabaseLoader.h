#ifndef DATABASELOADER_H
#define DATABASELOADER_H

#include "Spelobject.h"
#include "Vijand.h"
#include "SpelobjectFactory.h"
#include "VijandFactory.h"
#include "CustomVector.h"
#include "Locatie.h"
#include <sqlite3.h>
#include <random>
#include "LocatieFactory.h"

class DatabaseLoader {
public:
    DatabaseLoader() = default;
    ~DatabaseLoader() = default;

    CustomVector<Vijand*> laadVijanden(const char* databaseBestand, int& count);
    CustomVector<Spelobject*> laadSpelobjecten(const char* databaseBestand, int& count);
    CustomVector<Locatie*> laadLocaties(const char* databaseBestand, int& count);
    void printLeaderboard(const char* databaseBestand);
    void voegLeaderboardToe(const char* databaseBestand, const char* naam, int goudstukken);
};

#endif // DATABASELOADER_H