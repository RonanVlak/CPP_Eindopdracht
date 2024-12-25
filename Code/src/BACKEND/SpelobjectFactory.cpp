#include "SpelobjectFactory.h"
#include <cstring>
#include <iostream>

Spelobject* SpelobjectFactory::CreateSpelobject(const char* aNaam, const char* aBeschrijving, const char* aType, int aMinimumwaarde, int aMaximumwaarde, int aBescherming) {
    if (strcmp(aType, "goudstukken") == 0) {
        return new GoudstukkenObject(aNaam, aBeschrijving, aMinimumwaarde);
    } else if (strcmp(aType, "wapen") == 0) {
        return new WapenObject(aNaam, aBeschrijving, aMinimumwaarde, aMaximumwaarde);
    } else if (strcmp(aType, "wapenrusting") == 0) {
        return new WapenrustingObject(aNaam, aBeschrijving, aBescherming);
    } else if ((strcmp(aType, "teleportatiedrank") == 0) || (strcmp(aType, "ervaringsdrank") == 0) || (strcmp(aType, "levenselixer") == 0)) {
        return new ConsumeerbaarObject(aNaam, aBeschrijving, aMinimumwaarde);
    } else {
        return new Spelobject(aNaam, aBeschrijving);
    }
}