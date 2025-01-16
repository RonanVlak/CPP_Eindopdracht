#ifndef SPELWERELD_H
#define SPELWERELD_H

#include "Locatie.h"
#include <random>
#include "WapenObject.h"

class Spelwereld {
public:
    Spelwereld();
    ~Spelwereld();
    Spelwereld(const Spelwereld& other);
    Spelwereld& operator=(const Spelwereld& other);
    Spelwereld(Spelwereld&& other) noexcept;
    Spelwereld& operator=(Spelwereld&& other) noexcept;

public:
    void voegLocatieToe(Locatie* locatie);
    void verplaatsVijanden();
    Locatie* getLocatie(const char* naam);
    Locatie* getCurrentLocatie() const;
    void setCurrentLocatie(Locatie* locatie);
    Locatie* getLocatieById(int id) const;
    Locatie* getLocatieByIndex(int index) const;
    int getLocatiesCount() const;
    int getEnemiesDamage();
    void generateRandomKerker(const char* databaseBestand);
    WapenObject* getStartWapen(const char* dbPath);
private: 
    void clear();
    void copyFrom(const Spelwereld& other);
    void moveFrom(Spelwereld&& other) noexcept;
    CustomVector<Locatie*> getAdjacentLocations(Locatie* locatie);
private:
    CustomVector<Locatie*> mLocaties;
    Locatie* mCurrentLocatie;
};

#endif // SPELWERELD_H