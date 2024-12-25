#ifndef SPELOBJECTFACTORY_H
#define SPELOBJECTFACTORY_H

#include "Spelobject.h"
#include "GoudstukkenObject.h"
#include "WapenObject.h"
#include "WapenrustingObject.h"
#include "ConsumeerbaarObject.h"
#include "CustomUniquePtr.h"

class SpelobjectFactory {
public:
    static CustomUniquePtr<Spelobject> CreateSpelobject(const char* aNaam, const char* aBeschrijving, const char* aType, int aMinimumwaarde = 0, int aMaximumwaarde = 0, int aBescherming = 0);
};

#endif // SPELOBJECTFACTORY_H