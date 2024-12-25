#include "Speler.h"
#include "Vijand.h"
#include <iostream>
#include <random>

Speler::Speler() : mLevenspunten(0), mAanvalskans(0), mGoudstukken(0) {}

Speler::Speler(const std::string& naam, int levenspunten, int aanvalskans)
    : mNaam(naam), mLevenspunten(levenspunten), mAanvalskans(aanvalskans), mGoudstukken(0) {}

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
    moveFrom(std::move(other));
}

Speler& Speler::operator=(Speler&& other) noexcept {
    if (this != &other) {
        clear();
        moveFrom(std::move(other));
    }
    return *this;
}

void Speler::verwijderWapenrusting(const std::string& naam) {
    for (auto it = mWapenrustingInventaris.begin(); it != mWapenrustingInventaris.end(); ++it) {
        if ((*it)->getNaam() == naam) {
            mWapenrustingInventaris.erase(it);
            return;
        }
    }
}

void Speler::draagWapen(std::unique_ptr<WapenObject> wapen) {
    mWapen = std::move(wapen);
}

void Speler::verwijderWapen(const std::string& naam) {
    for (auto it = mWapenInventaris.begin(); it != mWapenInventaris.end(); ++it) {
        if ((*it)->getNaam() == naam) {
            mWapenInventaris.erase(it);
            return;
        }
    }
}
void Speler::draagWapenrusting(std::unique_ptr<WapenrustingObject> wapenrusting) {
    mWapenrusting = std::move(wapenrusting);
}

void Speler::consumeerObject(ConsumeerbaarObject* obj) {
    int effect = obj->getEffect();
    mLevenspunten += effect;
    if (mLevenspunten > 100) { // Assuming 100 is the maximum health
        mLevenspunten = 100;
    }
}

void Speler::toonGegevens() const {
    std::cout << "Naam: " << mNaam << "\nLevenspunten: " << mLevenspunten << "\nAanvalskans: " << mAanvalskans << "\nGoudstukken: " << mGoudstukken << std::endl;
}

void Speler::voegGoudstukkenToe(int aantal) {
    mGoudstukken += aantal;
}

void Speler::voegObjectToe(std::unique_ptr<Spelobject> obj) {
    if (auto consumeerbaar = dynamic_cast<ConsumeerbaarObject*>(obj.release())) {
        mConsumeerbareObjecten.push_back(std::unique_ptr<ConsumeerbaarObject>(consumeerbaar));
    } else if (auto wapen = dynamic_cast<WapenObject*>(obj.release())) {
        mWapenInventaris.push_back(std::unique_ptr<WapenObject>(wapen));
    } else if (auto wapenrusting = dynamic_cast<WapenrustingObject*>(obj.release())) {
        mWapenrustingInventaris.push_back(std::unique_ptr<WapenrustingObject>(wapenrusting));
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

void Speler::applyDamage(int damage) {
    mLevenspunten -= damage;
    if (mLevenspunten < 0) {
        mLevenspunten = 0;
    }
}

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

    for (const auto& obj : other.mConsumeerbareObjecten) {
        mConsumeerbareObjecten.push_back(std::make_unique<ConsumeerbaarObject>(*obj));
    }

    for (const auto& obj : other.mWapenInventaris) {
        mWapenInventaris.push_back(std::make_unique<WapenObject>(*obj));
    }

    for (const auto& obj : other.mWapenrustingInventaris) {
        mWapenrustingInventaris.push_back(std::make_unique<WapenrustingObject>(*obj));
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
    mWapenInventaris = std::move(other.mWapenInventaris);
    mWapenrustingInventaris = std::move(other.mWapenrustingInventaris);

    other.mLevenspunten = 0;
    other.mAanvalskans = 0;
    other.mGoudstukken = 0;
}

void Speler::clear() {
    mWapen.reset();
    mWapenrusting.reset();
    mConsumeerbareObjecten.clear();
    mWapenInventaris.clear();
    mWapenrustingInventaris.clear();
}