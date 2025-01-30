#ifndef VIJAND_H
#define VIJAND_H

#include "Spelobject.h"
#include "CustomVector.h"
class Locatie;

class Vijand {
public:
    Vijand(const char* aNaam, const char* aBeschrijving, int aMinimumobjecten, int aMaximumobjecten, int aLevenspunten, int aAanvalskans, int aAanvalsschadeMin, int aAanvalsschadeMax);
    ~Vijand();
    Vijand(const Vijand& other);
    Vijand& operator=(const Vijand& other);
    Vijand(Vijand&& other) noexcept;
    Vijand& operator=(Vijand&& other) noexcept;

    void verplaats(Locatie* locatie);
    bool isVerslagen() const;
    void ontvangSchade(int schade);
    void bekijk() const;
    void voegSpelobjectToe(Spelobject* spelobject);
    void removeSpelobject(Spelobject* spelobject);
    Spelobject* getSpelobject(int index) const;
    CustomVector<Spelobject*> getSpelobjecten() const;
    int getAantalSpelobjecten() const;
    int getLevenspunten() const;
    int getMinimumObjecten() const;
    int getMaximumObjecten() const;
    int getAanvalskans() const;
    int getAanvalsschadeMin() const;
    int getAanvalsschadeMax() const;
    const char* getNaam() const;
    const char* getBeschrijving() const;
    bool getBewogen() const;
    void setBewogen(bool bewogen);
    int attack() const;
    void setNaam(const char* naam);

private: 
    void copyFrom(const Vijand& other);
    void moveFrom(Vijand&& other) noexcept;
    void clear();
    
private:
    char* mNaam;
    char* mBeschrijving;
    int mMinimumObjecten;
    int mMaximumObjecten;
    int mLevenspunten;
    int mAanvalskans;
    int mAanvalsschadeMin;
    int mAanvalsschadeMax;
    bool mBewogen;
    CustomVector<Spelobject*> mSpelobjecten;
};

#endif // VIJAND_H