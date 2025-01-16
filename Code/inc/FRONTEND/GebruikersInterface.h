#ifndef GEBRUIKERSINTERFACE_H
#define GEBRUIKERSINTERFACE_H

#include <string>
#include "Speler.h"
#include "Locatie.h"
class GebruikersInterface {
public:
    GebruikersInterface() = default;
    static void toonMenu();
    static void toonLocatie(Locatie* locatie);
    static void toonSpeler(const Speler& speler);
    static void toonActies();
    static void toonHelp();
    static void toonEindeSpel();
    static std::string vraagSpelerNaam();
    static std::string vraagActie();
};

#endif // GEBRUIKERINTERFACE_H