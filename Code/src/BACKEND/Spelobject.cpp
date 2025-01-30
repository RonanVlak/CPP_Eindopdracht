#include "Spelobject.h"
#include <cstring>
#include <cstdlib>

Spelobject::Spelobject(const char* naam, const char* beschrijving) {
    mNaam = new char[strlen(naam) + 1];
    strcpy(mNaam, naam);
    mBeschrijving = new char[strlen(beschrijving) + 1];
    strcpy(mBeschrijving, beschrijving);
}

Spelobject::~Spelobject() {
    clear();
}

//Copy constructor
Spelobject::Spelobject(const Spelobject& other) {
    copyFrom(other);
}

//Assignment operator
Spelobject& Spelobject::operator=(const Spelobject& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

//Move constructor
Spelobject::Spelobject(Spelobject&& other) noexcept {
    moveFrom(static_cast<Spelobject&&>(other));
}

//Move assignment operator
Spelobject& Spelobject::operator=(Spelobject&& other) noexcept {
    if (this != &other) {
        clear();
        moveFrom(static_cast<Spelobject&&>(other));
    }
    return *this;
}

void Spelobject::setNaam(const char* naam) {
    delete[] mNaam;
    mNaam = new char[strlen(naam) + 1];
    strcpy(mNaam, naam);
}

const char* Spelobject::getNaam() const {
    return mNaam;
}

const char* Spelobject::getBeschrijving() const {
    return mBeschrijving;
}

void Spelobject::copyFrom(const Spelobject& other) {
    mNaam = new char[strlen(other.mNaam) + 1];
    strcpy(mNaam, other.mNaam);
    mBeschrijving = new char[strlen(other.mBeschrijving) + 1];
    strcpy(mBeschrijving, other.mBeschrijving);
}

void Spelobject::moveFrom(Spelobject&& other) noexcept {
    mNaam = other.mNaam;
    mBeschrijving = other.mBeschrijving;

    other.mNaam = nullptr;
    other.mBeschrijving = nullptr;
}

void Spelobject::clear() {
    delete[] mNaam;
    delete[] mBeschrijving;
    mNaam = nullptr;
    mBeschrijving = nullptr;
}