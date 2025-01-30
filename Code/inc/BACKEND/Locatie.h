#ifndef LOCATIE_H
#define LOCATIE_H

#include <cstring>
#include "Spelobject.h"
#include "CustomVector.h"
#include "Vijand.h"

class Locatie {
public:
    Locatie(int id, const char* aNaam, const char* aAlgemeneOmschrijving, const char* aGedetailleerdeOmschrijving);
    ~Locatie();
    Locatie(const Locatie& other);
    Locatie& operator=(const Locatie& other);
    Locatie(Locatie&& other) noexcept;
    Locatie& operator=(Locatie&& other) noexcept;

    void voegZichtbaarObjectToe(Spelobject* object);
    void voegVerborgenObjectToe(Spelobject* object);
    void voegUitgangToe(const char* richting, int locatieId);
    void voegVijandToe(Vijand* vijand);
    void verwijderVijand(Vijand* vijand);

    const char* getNaam() const;
    const char* getAlgemeneOmschrijving() const;
    const char* getGedetailleerdeOmschrijving() const;
    int getNoord() const;
    int getOost() const;
    int getZuid() const;
    int getWest() const;
    int getId() const;
    Spelobject* getZichtbaarObject(int index) const;
    Spelobject* getVerborgenObject(int index) const;
    int getUitgang(const char* richting) const;
    Vijand* getVijand(int index) const;
    int getZichtbareObjectenCount() const;
    int getVerborgenObjectenCount() const;
    int getVijandenCount() const;
    Spelobject* getZichtbaarObjectByName(const std::string& naam) const;
    Vijand* getVijandByName(const std::string& naam) const;

    void setID(int id);
    void verplaatsVerborgenNaarZichtbaar(Spelobject* object);   
    void setUitgang(const char* richting, Locatie* locatie);
    void verwijderZichtbaarObject(Spelobject* object);
    void listExits() const;
    void printZichtbareObjecten() const;

private: 
    void copyFrom(const Locatie& other);
    void moveFrom(Locatie&& other) noexcept;
    void clear();
private:
    int mId;
    char* mNaam;
    char* mBeschrijving;
    char* mGedetailleerdeBeschrijving;
    int mNoord;
    int mOost;
    int mZuid;
    int mWest;
    CustomVector<Spelobject*> mVerborgenObjecten;
    CustomVector<Spelobject*> mZichtbareObjecten;
    CustomVector<Vijand*> mVijanden;
    CustomVector<Locatie*> mUitgangen;
    CustomVector<char*> mRichtingen;
};

#endif // LOCATIE_H