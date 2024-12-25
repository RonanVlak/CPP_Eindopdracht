#include "Spelwereld.h"
#include <iostream>
#include <random>
#include <cstring>

Spelwereld::Spelwereld() : mCurrentLocatie(nullptr) {}

Spelwereld::~Spelwereld() {
    clear();
}

Spelwereld::Spelwereld(const Spelwereld& other) {
    copyFrom(other);
}

Spelwereld& Spelwereld::operator=(const Spelwereld& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

Spelwereld::Spelwereld(Spelwereld&& other) noexcept {
    moveFrom(std::move(other));
}

Spelwereld& Spelwereld::operator=(Spelwereld&& other) noexcept {
    if (this != &other) {
        clear();
        moveFrom(std::move(other));
    }
    return *this;
}

void Spelwereld::voegLocatieToe(Locatie* locatie) {
    mLocaties.push_back(locatie);
}

Locatie* Spelwereld::getLocatie(const char* naam) {
    for (int i = 0; i < mLocaties.size(); ++i) {
        if (std::strcmp(mLocaties[i]->getNaam(), naam) == 0) {
            return mLocaties[i];
        }
    }
    return nullptr;
}

Locatie* Spelwereld::getCurrentLocatie() const {
    return mCurrentLocatie;
}

void Spelwereld::setCurrentLocatie(Locatie* locatie) {
    mCurrentLocatie = locatie;
}

Locatie* Spelwereld::getLocatieById(int id) const {
    for (int i = 0; i < mLocaties.size(); ++i) {
        if (mLocaties[i]->getId() == id) {
            return mLocaties[i];
        }
    }
    return nullptr;
}

Locatie* Spelwereld::getLocatieByIndex(int index) const {
    if (index < 0 || index >= mLocaties.size()) {
        return nullptr;
    }
    return mLocaties[index];
}

int Spelwereld::getLocatiesCount() const {
    return mLocaties.size();
}

int Spelwereld::getEnemiesDamage() {
    int totalDamage = 0;
    Locatie* currentLocation = getCurrentLocatie();
    if (currentLocation) {
        for (int i = 0; i < currentLocation->getVijandenCount(); ++i) {
            Vijand* vijand = currentLocation->getVijand(i);
            if (vijand && !vijand->isVerslagen()) {
                int damage = vijand->attack();
                totalDamage += damage;
                if (damage > 0) {
                    std::cout << vijand->getNaam() << " heeft je " << damage << " schade toegebracht." << std::endl;
                }
            }
        }
    }
    return totalDamage;
}

void Spelwereld::verplaatsVijanden() {
    Locatie* currentLocation = getCurrentLocatie();
    if (currentLocation) {
        for (int i = 0; i < currentLocation->getVijandenCount(); ++i) {
            Vijand* vijand = currentLocation->getVijand(i);
            if (vijand && !vijand->isVerslagen()) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(1, 100);
                int moveChance = dis(gen);

                if (moveChance <= 50) { // 50% chance to move
                    // Get a random adjacent location
                    CustomVector<Locatie*> adjacentLocations = getAdjacentLocations(currentLocation);
                    if (adjacentLocations.size() > 0) {
                        std::uniform_int_distribution<> locDis(0, adjacentLocations.size() - 1);
                        Locatie* newLocation = adjacentLocations[locDis(gen)];
                        newLocation->voegVijandToe(vijand);
                        std::cout << vijand->getNaam() << " is verplaatst naar " << newLocation->getNaam() << "." << std::endl;
                        currentLocation->verwijderVijand(vijand);
                    }
                }
            }
        }
    }
}

CustomVector<Locatie*> Spelwereld::getAdjacentLocations(Locatie* locatie) {
    CustomVector<Locatie*> adjacentLocations;
    if (locatie->getNoord() != -1) {
        adjacentLocations.push_back(getLocatieById(locatie->getNoord()));
    }
    if (locatie->getOost() != -1) {
        adjacentLocations.push_back(getLocatieById(locatie->getOost()));
    }
    if (locatie->getZuid() != -1) {
        adjacentLocations.push_back(getLocatieById(locatie->getZuid()));
    }
    if (locatie->getWest() != -1) {
        adjacentLocations.push_back(getLocatieById(locatie->getWest()));
    }
    return adjacentLocations;
}

void Spelwereld::clear() {
    for (int i = 0; i < mLocaties.size(); ++i) {
        delete mLocaties[i];
    }
    mLocaties.clear();
}

void Spelwereld::copyFrom(const Spelwereld& other) {
    clear();
    for (int i = 0; i < other.mLocaties.size(); ++i) {
        mLocaties.push_back(new Locatie(*other.mLocaties[i]));
    }
    mCurrentLocatie = other.mCurrentLocatie;
}

void Spelwereld::moveFrom(Spelwereld&& other) noexcept {
    mLocaties = std::move(other.mLocaties);
    mCurrentLocatie = other.mCurrentLocatie;
    other.mCurrentLocatie = nullptr;
}