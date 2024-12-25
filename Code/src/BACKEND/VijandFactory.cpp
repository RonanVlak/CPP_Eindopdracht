#include "VijandFactory.h"

CustomUniquePtr<Vijand> VijandFactory::CreateVijand(const char* naam, const char* beschrijving, int minimumobjecten, int maximumobjecten, int levenspunten, int aanvalskans, int aanvalsschadeMin, int aanvalsschadeMax) {
    return CustomUniquePtr<Vijand>(new Vijand(naam, beschrijving, minimumobjecten, maximumobjecten, levenspunten, aanvalskans, aanvalsschadeMin, aanvalsschadeMax));
}