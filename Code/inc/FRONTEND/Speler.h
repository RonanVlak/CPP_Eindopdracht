#ifndef SPELER_H
#define SPELER_H

#include "ConsumeerbaarObject.h"
#include "Spelobject.h"
#include "Vijand.h"
#include "WapenObject.h"
#include "WapenrustingObject.h"
#include <memory>
#include <string>
#include <vector>

class Speler
{
public:
	Speler();
	Speler(const std::string& naam, int levenspunten, int aanvalskans, bool godMode);
	~Speler();
	Speler(const Speler& other);
	Speler& operator=(const Speler& other);
	Speler(Speler&& other) noexcept;
	Speler& operator=(Speler&& other) noexcept;

	void verwijderWapenrusting(const std::string& naam);
	void draagWapenrusting(std::unique_ptr<WapenrustingObject> wapenrusting);
	void draagWapen(std::unique_ptr<WapenObject> wapen);
	void verwijderWapen(const std::string& naam);
	void voegLevenspuntenToe(ConsumeerbaarObject* obj);
	void toonGegevens() const;
	void voegGoudstukkenToe(int aantal);
	void voegObjectToe(std::unique_ptr<Spelobject> obj);
	void sla(Vijand* vijand);
	void applyDamage(int damage);

	const std::string& getNaam() const;
	int getLevenspunten() const;
	int getAanvalskans() const;
	int getGoudstukken() const;
	bool getGodMode() const;
	std::vector<std::unique_ptr<ConsumeerbaarObject>>& getConsumeerbareObjecten();
	std::vector<std::unique_ptr<WapenObject>>& getWapenInventaris();
	std::vector<std::unique_ptr<WapenrustingObject>>& getWapenrustingInventaris();
	std::unique_ptr<WapenObject>& getHuidigWapen();
	std::unique_ptr<WapenrustingObject>& getHuidigWapenrusting();

	void setNaam(const std::string& naam);
	void setLevenspunten(int levenspunten);
	void setAanvalskans(int aanvalskans);
	void setGoudstukken(int goudstukken);
	void setGodMode(bool godMode);
	void clear();

private:
	void copyFrom(const Speler& other);
	void moveFrom(Speler&& other) noexcept;

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
	bool mGodMode;
};

#endif // SPELER_H