#include "GoudstukkenObject.h"
#include <cstring>

GoudstukkenObject::GoudstukkenObject(const char* aNaam, const char* aBeschrijving, int aAantalGoudstukken)
    : Spelobject(aNaam, aBeschrijving), mAantalGoudstukken(aAantalGoudstukken) {}


int GoudstukkenObject::getAantalGoudstukken() const {
    return mAantalGoudstukken;
}