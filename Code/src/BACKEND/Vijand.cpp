#include "Vijand.h"
#include "Logger.h"
#include "RandomEngine.h"
#include <cstring>
#include <iostream>

Vijand::Vijand(const char* aNaam, const char* aBeschrijving, int aMinimumobjecten, int aMaximumobjecten,
			   int aLevenspunten, int aAanvalskans, int aAanvalsschadeMin, int aAanvalsschadeMax)
	: mNaam(nullptr), mBeschrijving(nullptr), mMinimumObjecten(aMinimumobjecten), mMaximumObjecten(aMaximumobjecten),
	  mLevenspunten(aLevenspunten), mAanvalskans(aAanvalskans), mAanvalsschadeMin(aAanvalsschadeMin),
	  mAanvalsschadeMax(aAanvalsschadeMax)
{
	mNaam = new char[strlen(aNaam) + 1];
	strcpy(mNaam, aNaam);
	mBeschrijving = new char[strlen(aBeschrijving) + 1];
	strcpy(mBeschrijving, aBeschrijving);

	mBewogen = false;
}

Vijand::~Vijand() { clear(); }

//Copy constructor
Vijand::Vijand(const Vijand& other) { copyFrom(other); }

//Assignment operator
Vijand& Vijand::operator=(const Vijand& other)
{
	if (this != &other)
	{
		clear();
		copyFrom(other);
	}
	return *this;
}

//Move constructor
Vijand::Vijand(Vijand&& other) noexcept { moveFrom(std::move(other)); }

//Move assignment operator
Vijand& Vijand::operator=(Vijand&& other) noexcept
{
	if (this != &other)
	{
		clear();
		moveFrom(std::move(other));
	}
	return *this;
}

void Vijand::verplaats(Locatie* locatie) {}

void Vijand::ontvangSchade(int schade)
{
	mLevenspunten -= schade;
	if (mLevenspunten <= 0)
	{
		mLevenspunten = 0;
	}

	if (isVerslagen() && strstr(mNaam, "[Verslagen]") == nullptr)
	{
		size_t naamLength = strlen(mNaam);
		const char* verslagenSuffix = " [Verslagen]";
		size_t suffixLength = strlen(verslagenSuffix);

		char* newNaam = new char[naamLength + suffixLength + 1];
		strcpy(newNaam, mNaam);
		strcat(newNaam, verslagenSuffix);

		delete[] mNaam;
		mNaam = newNaam;
	}
}

bool Vijand::isVerslagen() const { return mLevenspunten <= 0; }

void Vijand::bekijk() const
{
	Logger::getInstance().logOutput("Naam: ");
	Logger::getInstance().logOutput(mNaam);
	Logger::getInstance().logOutput("\nBeschrijving: ");
	Logger::getInstance().logOutput(mBeschrijving);
	Logger::getInstance().logOutput("\nLevenspunten: " + std::to_string(mLevenspunten) + "\n");
}

void Vijand::voegSpelobjectToe(Spelobject* spelobject) { mSpelobjecten.push_back(spelobject); }

void Vijand::removeSpelobject(Spelobject* spelobject)
{
	for (int i = 0; i < mSpelobjecten.size(); ++i)
	{
		if (mSpelobjecten[i] == spelobject)
		{
			mSpelobjecten.erase(i);
			return;
		}
	}
}

int Vijand::attack() const
{
	int hitChance = RandomEngine::getRandomInt(1, 100);

	if (hitChance <= mAanvalskans)
	{
		int damage = RandomEngine::getRandomInt(mAanvalsschadeMin, mAanvalsschadeMax);
		return damage;
	}
	else
	{
		Logger::getInstance().logOutput(mNaam);
		Logger::getInstance().logOutput(" mist zijn aanval.\n");
		return 0;
	}
}

void Vijand::setNaam(const char* naam)
{
	delete[] mNaam;
	mNaam = new char[strlen(naam) + 1];
	strcpy(mNaam, naam);
}

CustomVector<Spelobject*> Vijand::getSpelobjecten() const { return mSpelobjecten; }

Spelobject* Vijand::getSpelobject(int index) const
{
	if (index < 0 || index >= mSpelobjecten.size())
	{
		return nullptr;
	}
	return mSpelobjecten[index];
}

int Vijand::getAantalSpelobjecten() const { return mSpelobjecten.size(); }

int Vijand::getLevenspunten() const { return mLevenspunten; }

const char* Vijand::getNaam() const { return mNaam; }

const char* Vijand::getBeschrijving() const { return mBeschrijving; }

int Vijand::getMinimumObjecten() const { return mMinimumObjecten; }

int Vijand::getMaximumObjecten() const { return mMaximumObjecten; }

int Vijand::getAanvalskans() const { return mAanvalskans; }

int Vijand::getAanvalsschadeMin() const { return mAanvalsschadeMin; }

int Vijand::getAanvalsschadeMax() const { return mAanvalsschadeMax; }

bool Vijand::getBewogen() const { return mBewogen; }

void Vijand::setBewogen(bool bewogen) { mBewogen = bewogen; }

void Vijand::copyFrom(const Vijand& other)
{
	mNaam = new char[strlen(other.mNaam) + 1];
	strcpy(mNaam, other.mNaam);
	mBeschrijving = new char[strlen(other.mBeschrijving) + 1];
	strcpy(mBeschrijving, other.mBeschrijving);
	mLevenspunten = other.mLevenspunten;
	mAanvalskans = other.mAanvalskans;
	mAanvalsschadeMin = other.mAanvalsschadeMin;
	mAanvalsschadeMax = other.mAanvalsschadeMax;
	mSpelobjecten = other.mSpelobjecten;
}

void Vijand::moveFrom(Vijand&& other) noexcept
{
	mNaam = other.mNaam;
	mBeschrijving = other.mBeschrijving;
	mLevenspunten = other.mLevenspunten;
	mAanvalskans = other.mAanvalskans;
	mAanvalsschadeMin = other.mAanvalsschadeMin;
	mAanvalsschadeMax = other.mAanvalsschadeMax;
	mSpelobjecten = std::move(other.mSpelobjecten);

	other.mNaam = nullptr;
	other.mBeschrijving = nullptr;
	other.mLevenspunten = 0;
	other.mAanvalskans = 0;
	other.mAanvalsschadeMin = 0;
	other.mAanvalsschadeMax = 0;
}

void Vijand::clear()
{
	delete[] mNaam;
	mNaam = nullptr;
	delete[] mBeschrijving;
	mBeschrijving = nullptr;
	for (auto& obj : mSpelobjecten)
	{
		delete obj;
		obj = nullptr;
	}
	mSpelobjecten.clear();
}