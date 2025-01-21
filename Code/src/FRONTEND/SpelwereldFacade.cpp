#include "SpelwereldFacade.h"
#include "Spelwereld.h"
#include "Locatie.h"
#include "WapenObject.h"
#include "Vijand.h"

SpelwereldFacade::SpelwereldFacade() : mSpelwereld(std::make_unique<Spelwereld>()) {}

SpelwereldFacade::~SpelwereldFacade() = default;

SpelwereldFacade::SpelwereldFacade(SpelwereldFacade&&) noexcept = default;

SpelwereldFacade& SpelwereldFacade::operator=(SpelwereldFacade&&) noexcept = default;

void SpelwereldFacade::generateRandomKerker(const std::string& databasePath) {
    mSpelwereld->generateRandomKerker(databasePath.c_str());
}

std::unique_ptr<WapenObject> SpelwereldFacade::getStartWapen(const std::string& databasePath) {
    WapenObject* rawWapen = mSpelwereld->getStartWapen(databasePath.c_str());
    return std::unique_ptr<WapenObject>(rawWapen); // Takes ownership of the weapon
}

Locatie* SpelwereldFacade::getCurrentLocatie() const {
    return mSpelwereld->getCurrentLocatie();
}

void SpelwereldFacade::setCurrentLocatie(Locatie* locatie) {
    mSpelwereld->setCurrentLocatie(locatie);
}

Locatie* SpelwereldFacade::getLocatie(const std::string& naam) {
    return mSpelwereld->getLocatie(naam.c_str());
}

Locatie* SpelwereldFacade::getLocatieById(int id) const {
    return mSpelwereld->getLocatieById(id);
}

Locatie* SpelwereldFacade::getLocatieByIndex(int index) const {
    return mSpelwereld->getLocatieByIndex(index);
}

void SpelwereldFacade::voegLocatieToe(std::unique_ptr<Locatie> locatie) {
    mSpelwereld->voegLocatieToe(locatie.release()); // Transfer ownership to Spelwereld
}

std::vector<Locatie*> SpelwereldFacade::getAllLocaties() const {
    std::vector<Locatie*> locaties;
    for (int i = 0; i < mSpelwereld->getLocatiesCount(); ++i) {
        locaties.push_back(mSpelwereld->getLocatieByIndex(i));
    }
    return locaties;
}

int SpelwereldFacade::getLocatiesCount() const {
    return mSpelwereld->getLocatiesCount();
}

int SpelwereldFacade::getEnemiesDamage() {
    return mSpelwereld->getEnemiesDamage();
}

Spelwereld* SpelwereldFacade::getSpelwereld() {
    return mSpelwereld.get();
}

void SpelwereldFacade::verplaatsVijanden() {
    mSpelwereld->verplaatsVijanden();
}

std::vector<Locatie*> SpelwereldFacade::getAdjacentLocations(Locatie* locatie) {
    auto rawAdjacent = mSpelwereld->getAdjacentLocations(locatie);
    std::vector<Locatie*> adjacent;
    for (int i = 0; i < rawAdjacent.size(); ++i) {
        adjacent.push_back(rawAdjacent[i]);
    }
    return adjacent;
}

void SpelwereldFacade::clear() {
    mSpelwereld->clear();
}