#ifndef VIJANDFACTORY_H
#define VIJANDFACTORY_H
#include "CustomUniquePtr.h"
#include "Vijand.h"

class VijandFactory {
public:
    static CustomUniquePtr<Vijand> CreateVijand(const char* aNaam, const char* aBeschrijving, int aMinimumobjecten, int aMaximumobjecten, int aLevenspunten, int aAanvalskans, int aAanvalsschadeMin, int aAanvalsschadeMax);

};

#endif // VIJANDFACTORY_H