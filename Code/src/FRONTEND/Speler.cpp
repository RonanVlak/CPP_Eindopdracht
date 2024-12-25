#include "Speler.h"
#include "WapenObject.h"
#include "WapenrustingObject.h"
#include "ConsumeerbaarObject.h"
#include <iostream>
#include <random>

Speler::Speler()
    : mNaam(""), mLevenspunten(20), mAanvalskans(40), mGoudstukken(0) {}
    
Speler::Speler(const std::string& naam)
    : mNaam(naam), mLevenspunten(20), mAanvalskans(40), mGoudstukken(0) {}

Speler::~Speler() {
    clear();
}

Speler::Speler(const Speler& other) {
    copyFrom(other);
}

Speler& Speler::operator=(const Speler& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

Speler::Speler(Speler&& other) noexcept {
    moveFrom(static_cast<Speler&&>(other));
}

Speler& Speler::operator=(Speler&& other) noexcept {
    if (this != &other) {
        clear();
        moveFrom(static_cast<Speler&&>(other));
    }
    return *this;
}

void Speler::voegConsumeerbaarObjectToe(std::unique_ptr<ConsumeerbaarObject> obj) {
    mConsumeerbareObjecten.push_back(std::move(obj));
}

void Speler::verwijderConsumeerbaarObject(const std::string& naam) {
    auto it = std::remove_if(mConsumeerbareObjecten.begin(), mConsumeerbareObjecten.end(),
                             [&naam](const std::unique_ptr<ConsumeerbaarObject>& obj) {
                                 return obj->getNaam() == naam;
                             });
    mConsumeerbareObjecten.erase(it, mConsumeerbareObjecten.end());
}

void Speler::voegWapenToe(std::unique_ptr<WapenObject> obj) {
    mWapenInventaris.push_back(std::move(obj));
}

void Speler::verwijderWapenUitInventaris(const std::string& naam) {
    auto it = std::remove_if(mWapenInventaris.begin(), mWapenInventaris.end(),
                             [&naam](const std::unique_ptr<WapenObject>& obj) {
                                 return obj->getNaam() == naam;
                             });
    mWapenInventaris.erase(it, mWapenInventaris.end());
}

void Speler::voegWapenrustingToe(std::unique_ptr<WapenrustingObject> obj) {
    mWapenrustingInventaris.push_back(std::move(obj));
}

void Speler::voegObjectToe(std::unique_ptr<Spelobject> obj) {
    if (auto consumeerbaar = dynamic_cast<ConsumeerbaarObject*>(obj.get())) {
        voegConsumeerbaarObjectToe(std::unique_ptr<ConsumeerbaarObject>(consumeerbaar));
    } else if (auto wapen = dynamic_cast<WapenObject*>(obj.get())) {
        voegWapenToe(std::unique_ptr<WapenObject>(wapen));
    } else if (auto wapenrusting = dynamic_cast<WapenrustingObject*>(obj.get())) {
        voegWapenrustingToe(std::unique_ptr<WapenrustingObject>(wapenrusting));
    } else if (auto goudstukken = dynamic_cast<GoudstukkenObject*>(obj.get())) {
        voegGoudstukkenToe(goudstukken->getAantalGoudstukken());
    } else {
        std::cout << "Onbekend object type: " << obj->getNaam() << std::endl;
    }           
}

void Speler::verwijderWapenrustingUitInventaris(const std::string& naam) {
    auto it = std::remove_if(mWapenrustingInventaris.begin(), mWapenrustingInventaris.end(),
                             [&naam](const std::unique_ptr<WapenrustingObject>& obj) {
                                 return obj->getNaam() == naam;
                             });
    mWapenrustingInventaris.erase(it, mWapenrustingInventaris.end());
}

void Speler::verwijderWapen(const std::string& naam) {
    if (mWapen && mWapen->getNaam() == naam) {
        mWapen.reset();
    }
}

void Speler::verwijderWapenrusting(const std::string& naam) {
    if (mWapenrusting && mWapenrusting->getNaam() == naam) {
        mWapenrusting.reset();
    }
}

void Speler::draagWapen(std::unique_ptr<WapenObject> wapen) {
    mWapen = std::move(wapen);
}

void Speler::draagWapenrusting(std::unique_ptr<WapenrustingObject> wapenrusting) {
    mWapenrusting = std::move(wapenrusting);
}

void Speler::consumeerObject(const std::string& naam) {
    auto it = std::find_if(mConsumeerbareObjecten.begin(), mConsumeerbareObjecten.end(),
                           [&naam](const std::unique_ptr<ConsumeerbaarObject>& obj) {
                               return obj->getNaam() == naam;
                           });
    if (it != mConsumeerbareObjecten.end()) {
        // Apply the effect of the consumable object
        mLevenspunten += (*it)->getEffect();
        mConsumeerbareObjecten.erase(it);
    }
}

bool Speler::sla(Vijand* vijand) {
    if (vijand) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);

        int kans = dis(gen);
        if (kans <= mAanvalskans) {
            int schade = 0;
            if (mWapen) {
                std::uniform_int_distribution<> schadeDis(mWapen->getMinimumSchade(), mWapen->getMaximumSchade());
                schade = schadeDis(gen);
            } else {
                std::uniform_int_distribution<> schadeDis(1, 2);
                schade = schadeDis(gen);
            }
            vijand->ontvangSchade(schade);
            if (vijand->isVerslagen()) {
                return true;
            }
            else {
                std::cout << "Je hebt "<< vijand->getNaam() << " aangevallen en " << schade << " schade toegebracht." << std::endl;
                std::cout << vijand->getNaam() << " heeft nog " << vijand->getLevenspunten() << " levenspunten over." << std::endl;
                return false;
            }
        }
        else {
            std::cout << "Je hebt " << vijand->getNaam() << " aangevallen, maar je miste." << std::endl;
        }
        
    }
    return false;
}

void Speler::toonGegevens() const {
    std::cout << "Naam: " << mNaam << std::endl;
    std::cout << "Levenspunten: " << mLevenspunten << std::endl;
    std::cout << "Aanvalskans: " << mAanvalskans << std::endl;
    std::cout << "Goudstukken: " << mGoudstukken << std::endl;
    if (mWapen) {
        std::cout << "Wapen: " << mWapen->getNaam() << std::endl;
    }
    if (mWapenrusting) {
        std::cout << "Wapenrusting: " << mWapenrusting->getNaam() << std::endl;
    }
    std::cout << "Consumeerbare Objecten: " << std::endl;
    for (const auto& obj : mConsumeerbareObjecten) {
        std::cout << " - " << obj->getNaam() << std::endl;
    }
}

void Speler::applyDamage(int damage) {
    int bescherming = 0;
    if (mWapenrusting) {
        bescherming = mWapenrusting->getBescherming();
    }
    int actualDamage = damage - bescherming;
    if (actualDamage < 0) {
        actualDamage = 0;
    }
    mLevenspunten -= actualDamage;
    if (mLevenspunten < 0) {
        mLevenspunten = 0;
    }
    std::cout << "Je hebt " << damage << " schade ontvangen. Levenspunten: " << mLevenspunten << std::endl;
}

void Speler::voegGoudstukkenToe(int aantal) {
    mGoudstukken += aantal;
}

// Getters
const std::string& Speler::getNaam() const {
    return mNaam;
}

int Speler::getLevenspunten() const {
    return mLevenspunten;
}

int Speler::getAanvalskans() const {
    return mAanvalskans;
}

int Speler::getGoudstukken() const {
    return mGoudstukken;
}

std::vector<std::unique_ptr<ConsumeerbaarObject>>& Speler::getConsumeerbareObjecten() {
    return mConsumeerbareObjecten;
}

std::vector<std::unique_ptr<WapenObject>>& Speler::getWapenInventaris() {
    return mWapenInventaris;
}

std::vector<std::unique_ptr<WapenrustingObject>>& Speler::getWapenrustingInventaris() {
    return mWapenrustingInventaris;
}

std::unique_ptr<WapenObject>& Speler::getHuidigWapen() {
    return mWapen;
}

std::unique_ptr<WapenrustingObject>& Speler::getHuidigWapenrusting() {
    return mWapenrusting;
}


// Setters
void Speler::setNaam(const std::string& naam) {
    mNaam = naam;
}

void Speler::setLevenspunten(int levenspunten) {
    mLevenspunten = levenspunten;
}

void Speler::setAanvalskans(int aanvalskans) {
    mAanvalskans = aanvalskans;
}

void Speler::setGoudstukken(int goudstukken) {
    mGoudstukken = goudstukken;
}

void Speler::copyFrom(const Speler& other) {
    mNaam = other.mNaam;
    mLevenspunten = other.mLevenspunten;
    mAanvalskans = other.mAanvalskans;
    mGoudstukken = other.mGoudstukken;
    if (other.mWapen) {
        mWapen = std::make_unique<WapenObject>(*other.mWapen);
    }
    if (other.mWapenrusting) {
        mWapenrusting = std::make_unique<WapenrustingObject>(*other.mWapenrusting);
    }
    mConsumeerbareObjecten.clear();
    for (const auto& obj : other.mConsumeerbareObjecten) {
        mConsumeerbareObjecten.push_back(std::make_unique<ConsumeerbaarObject>(*obj));
    }
}

void Speler::moveFrom(Speler&& other) noexcept {
    mNaam = std::move(other.mNaam);
    mLevenspunten = other.mLevenspunten;
    mAanvalskans = other.mAanvalskans;
    mGoudstukken = other.mGoudstukken;
    mWapen = std::move(other.mWapen);
    mWapenrusting = std::move(other.mWapenrusting);
    mConsumeerbareObjecten = std::move(other.mConsumeerbareObjecten);

    other.mLevenspunten = 0;
    other.mAanvalskans = 0;
    other.mGoudstukken = 0;
}

void Speler::clear() {
    mWapen.reset();
    mWapenrusting.reset();
    mConsumeerbareObjecten.clear();
}