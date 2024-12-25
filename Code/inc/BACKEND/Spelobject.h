#ifndef SPELOBJECT_H
#define SPELOBJECT_H

#include <cstring>
#include <cstdlib>

class Spelobject {
public:
    Spelobject(const char* aNaam, const char* aBeschrijving);
    virtual ~Spelobject();
    Spelobject(const Spelobject& other);
    Spelobject& operator=(const Spelobject& other);
    Spelobject(Spelobject&& other) noexcept;
    Spelobject& operator=(Spelobject&& other) noexcept;

    const char* getNaam() const;
    const char* getBeschrijving() const;

    void setNaam(const char* aNaam);
protected:
    void copyFrom(const Spelobject& other);
    void moveFrom(Spelobject&& other) noexcept;
    void clear();

private:
    char* mNaam;
    char* mBeschrijving;
};

#endif // SPELOBJECT_H