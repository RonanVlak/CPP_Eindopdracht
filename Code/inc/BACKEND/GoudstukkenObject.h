#ifndef GOUDSTUKKENOBJECT_H
#define GOUDSTUKKENOBJECT_H

#include "Spelobject.h"

class GoudstukkenObject : public Spelobject {
public:
    GoudstukkenObject(const char* aNaam, const char* aBeschrijving, int aAantalGoudstukken);
    virtual ~GoudstukkenObject();
    int getAantalGoudstukken() const;

private:
    int mAantalGoudstukken;
};

#endif // GOUDSTUKKENOBJECT_H