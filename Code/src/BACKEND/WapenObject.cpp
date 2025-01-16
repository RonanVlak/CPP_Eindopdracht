#include "WapenObject.h"
#include <cstring>

WapenObject::WapenObject(const char* aNaam, const char* aBeschrijving, int aMinimumSchade, int aMaximumSchade)
    : Spelobject(aNaam, aBeschrijving), mMininmumSchade(aMinimumSchade), mMaximumSchade(aMaximumSchade) {}

int WapenObject::getMinimumSchade() const {
    return mMininmumSchade;
}

int WapenObject::getMaximumSchade() const {
    return mMaximumSchade;
}