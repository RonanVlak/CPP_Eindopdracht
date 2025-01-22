#ifndef SPELWERELD_H
#define SPELWERELD_H

#include "Locatie.h"
#include "WapenObject.h"
#include <random>

class Spelwereld
{
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
	CustomVector<Locatie*> getAdjacentLocations(Locatie* locatie);

	void clear();

private:
	void shuffle(CustomVector<Locatie*>& locaties);
	void copyFrom(const Spelwereld& other);
	void moveFrom(Spelwereld&& other) noexcept;

private:
	CustomVector<Locatie*> mLocaties;
	Locatie* mCurrentLocatie;
};

#endif // SPELWERELD_H