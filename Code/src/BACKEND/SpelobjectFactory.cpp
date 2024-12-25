#include "SpelobjectFactory.h"
#include <cstring>
#include <iostream>
CustomUniquePtr<Spelobject> SpelobjectFactory::CreateSpelobject(const char* aNaam, const char* aBeschrijving, const char* aType, int aMinimumwaarde, int aMaximumwaarde, int aBescherming) {
    if (strcmp(aType, "goudstukken") == 0) {
        return CustomUniquePtr<Spelobject>(new GoudstukkenObject(aNaam, aBeschrijving, aMinimumwaarde));
    } else if (strcmp(aType, "wapen") == 0) {
        return CustomUniquePtr<Spelobject>(new WapenObject(aNaam, aBeschrijving, aMinimumwaarde, aMaximumwaarde));
    } else if (strcmp(aType, "wapenrusting") == 0) {
        return CustomUniquePtr<Spelobject>(new WapenrustingObject(aNaam, aBeschrijving, aBescherming));
    } else if ((strcmp(aType, "teleportatiedrank")) || (strcmp(aType, "ervaringsdrank")) || (strcmp(aType, "levenselixer")) == 0) {
        return CustomUniquePtr<Spelobject>(new ConsumeerbaarObject(aNaam, aBeschrijving, aMinimumwaarde));
    } else {
        return CustomUniquePtr<Spelobject>(new Spelobject(aNaam, aBeschrijving));
    }
}