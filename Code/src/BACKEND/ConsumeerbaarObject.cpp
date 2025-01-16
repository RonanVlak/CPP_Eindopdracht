#include "ConsumeerbaarObject.h"
#include <cstring>

ConsumeerbaarObject::ConsumeerbaarObject(const char* aNaam, const char* aBeschrijving, int aEffect)
    : Spelobject(aNaam, aBeschrijving), mEffect(aEffect) {}

int ConsumeerbaarObject::getEffect() const {
    return mEffect;
}