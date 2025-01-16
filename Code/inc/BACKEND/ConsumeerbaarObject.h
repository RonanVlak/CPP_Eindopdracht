#ifndef CONSUMEERBAAROBJECT_H
#define CONSUMEERBAAROBJECT_H

#include "Spelobject.h"

class ConsumeerbaarObject : public Spelobject {
public:
    ConsumeerbaarObject(const char* aNaam, const char* aBeschrijving, int aEffect);
    int getEffect() const;

private:
    int mEffect;
};

#endif // CONSUMEERBAAROBJECT_H