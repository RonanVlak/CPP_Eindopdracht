#ifndef VIJANDFACTORY_H
#define VIJANDFACTORY_H
#include "Vijand.h"

class VijandFactory {
public:
    static Vijand* CreateVijand(const char* aNaam, const char* aBeschrijving, int aMinimumobjecten, int aMaximumobjecten, int aLevenspunten, int aAanvalskans, int aAanvalsschadeMin, int aAanvalsschadeMax);
};

#endif // VIJANDFACTORY_H