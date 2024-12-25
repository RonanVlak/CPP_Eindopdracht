#ifndef SPELER_H
#define SPELER_H

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "CustomUniquePtr.h"
#include "Vijand.h"
#include "Spelobject.h"
#include "GoudstukkenObject.h"
#include "WapenObject.h"
#include "WapenrustingObject.h"
#include "ConsumeerbaarObject.h"


class WapenObject;
class WapenrustingObject;
class ConsumeerbaarObject;

class Speler {
public:
    Speler();
    Speler(const std::string& naam);
    ~Speler();
    Speler(const Speler& other);
    Speler& operator=(const Speler& other);
    Speler(Speler&& other) noexcept;
    Speler& operator=(Speler&& other) noexcept;

    void voegConsumeerbaarObjectToe(std::unique_ptr<ConsumeerbaarObject> obj);
    void voegWapenToe(std::unique_ptr<WapenObject> obj);
    void voegWapenrustingToe(std::unique_ptr<WapenrustingObject> obj);
    void verwijderConsumeerbaarObject(const std::string& naam);
    void draagWapen(std::unique_ptr<WapenObject> wapen);
    void verwijderWapenUitInventaris(const std::string& naam);
    void verwijderWapenrustingUitInventaris(const std::string& naam);
    void verwijderWapen(const std::string& naam);
    void verwijderWapenrusting(const std::string& naam);
    void draagWapenrusting(std::unique_ptr<WapenrustingObject> wapenrusting);
    void consumeerObject(const std::string& naam);
    void toonGegevens() const;
    void voegGoudstukkenToe(int aantal);
    void voegObjectToe(std::unique_ptr<Spelobject> obj);
    bool sla(Vijand* vijand);
    void applyDamage(int damage);

    const std::string& getNaam() const;
    int getLevenspunten() const;
    int getAanvalskans() const;
    int getGoudstukken() const;
    std::vector<std::unique_ptr<ConsumeerbaarObject>>& getConsumeerbareObjecten();
    std::vector<std::unique_ptr<WapenObject>>& getWapenInventaris();
    std::vector<std::unique_ptr<WapenrustingObject>>& getWapenrustingInventaris();
    std::unique_ptr<WapenObject>& getHuidigWapen();
    std::unique_ptr<WapenrustingObject>& getHuidigWapenrusting();

    void setNaam(const std::string& naam);
    void setLevenspunten(int levenspunten);
    void setAanvalskans(int aanvalskans);
    void setGoudstukken(int goudstukken);

private:
    void copyFrom(const Speler& other);
    void moveFrom(Speler&& other) noexcept;
    void clear();
private:
    std::string mNaam;
    int mLevenspunten;
    int mAanvalskans;
    std::unique_ptr<WapenObject> mWapen;
    std::unique_ptr<WapenrustingObject> mWapenrusting;
    int mGoudstukken;
    std::vector<std::unique_ptr<ConsumeerbaarObject>> mConsumeerbareObjecten;
    std::vector<std::unique_ptr<WapenObject>> mWapenInventaris;
    std::vector<std::unique_ptr<WapenrustingObject>> mWapenrustingInventaris;
};

#endif // SPELER_H