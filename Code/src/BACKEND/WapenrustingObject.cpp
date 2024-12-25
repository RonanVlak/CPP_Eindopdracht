#include "WapenrustingObject.h"
#include <cstring>

WapenrustingObject::WapenrustingObject(const char* aNaam, const char* aBeschrijving, int aBescherming)
    : Spelobject(aNaam, aBeschrijving), mBescherming(aBescherming) {}

WapenrustingObject::~WapenrustingObject() {}

int WapenrustingObject::getBescherming() const {
    return mBescherming;
}