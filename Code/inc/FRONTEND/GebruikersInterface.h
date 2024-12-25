#ifndef GEBRUIKERSINTERFACE_H
#define GEBRUIKERSINTERFACE_H

#include <string>
#include "Speler.h"

class GebruikersInterface {
public:
    GebruikersInterface() = default;
    static void toonMenu();
    static void toonLocatie();
    static void toonSpeler(const Speler& speler);
    static void toonActies();
    static void toonHelp();
    static void toonEindeSpel();
    static std::string vraagSpelerNaam();
    static std::string vraagActie();
};

#endif // GEBRUIKERINTERFACE_H