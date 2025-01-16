#ifndef WAPENOBJECT_H
#define WAPENOBJECT_H

#include "Spelobject.h"

class WapenObject : public Spelobject {
public:
    WapenObject(const char* aNaam, const char* aBeschrijving, int aMinimumSchade, int aMaximumSchade);
public:
    int getMinimumSchade() const;
    int getMaximumSchade() const;
private:
    int mMininmumSchade;
    int mMaximumSchade;
};

#endif // WAPENOBJECT_H