#ifndef WAPENRUSTINGOBJECT_H
#define WAPENRUSTINGOBJECT_H

#include "Spelobject.h"

class WapenrustingObject : public Spelobject {
public:
    WapenrustingObject(const char* aNaam, const char* aBeschrijving, int aBescherming);
    int getBescherming() const;

private:
    int mBescherming;
};

#endif // WAPENRUSTINGOBJECT_H