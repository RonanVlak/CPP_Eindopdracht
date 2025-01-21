#ifndef SPELERACTIEHANDLER_H
#define SPELERACTIEHANDLER_H

#include <string>
#include <memory>
#include "SpelwereldFacade.h"
#include "Speler.h"
#include "GebruikersInterface.h"
#include <limits>
#include "Spelobject.h"
#include "ConsumeerbaarObject.h"
#include "WapenObject.h"
#include "WapenrustingObject.h"
#include "GoudstukkenObject.h"

class SpelerActieHandler {
public:
    SpelerActieHandler(std::unique_ptr<SpelwereldFacade>& aSpelwereld, std::unique_ptr<Speler>& aSpeler, std::unique_ptr<GebruikersInterface>& aGebruikersInterface);
    virtual ~SpelerActieHandler() = default;

    void verwerkActie(const std::string& aActie);
    void kijk();
    void zoek();
    void gaNaar(const std::string& aRichting);
    void pak(const std::string& aObjectnaam);
    void legNeer(const std::string& aObjectnaam);
    void bekijkObject(const std::string& aObjectnaam);
    void bekijkVijand(const std::string& aVijandnaam);
    void bekijkZelf();
    void sla(const std::string& aVijandnaam);
    void draagWapenrusting(const std::string& aWapenrustingnaam);
    void draagWapen(const std::string& aWapennaam);
    void wacht();
    void consumeer(const std::string& aObjectnaam);
    void godMode();

private:
    SpelwereldFacade& mSpelwereld;
    Speler& mSpeler;
    GebruikersInterface& mGebruikersInterface;
    
    enum class Actie {
        Kijk,
        Zoek,
        GaNaar,
        Pak,
        LegNeer,
        BekijkObject,
        BekijkVijand,
        BekijkZelf,
        Sla,
        DraagWapenrusting,
        DraagWapen,
        Wacht,
        Consumeer,
        GodMode,
        Onbekend
    };
    Actie stringNaarActie(const std::string& aActie);
};

#endif // SPELERACTIEHANDLER_H