#ifndef GEBRUIKERSINTERFACE_H
#define GEBRUIKERSINTERFACE_H

#include <string>
#include "Speler.h"
#include "Locatie.h"
class GebruikersInterface {
public:
    GebruikersInterface() = default;
    void toonMenu() const;
    void toonLocatie(Locatie* locatie) const;
    void toonSpeler(const Speler& speler) const;
    void toonActies() const;
    void toonHelp() const;
    void toonEindeSpel() const;
    std::string vraagSpelerNaam();
    std::string vraagActie();
};

#endif // GEBRUIKERINTERFACE_H