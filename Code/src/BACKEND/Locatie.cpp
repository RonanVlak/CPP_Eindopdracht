#include "Locatie.h"
#include "Logger.h"
#include <cstdlib>
#include <cstring>
#include <iostream>

Locatie::Locatie(int id, const char* naam, const char* algemeneOmschrijving, const char* gedetailleerdeOmschrijving)
	: mId(id), mNaam(nullptr), mBeschrijving(nullptr), mGedetailleerdeBeschrijving(nullptr), mNoord(-1), mOost(-1),
	  mZuid(-1), mWest(-1)
{
	mNaam = new char[strlen(naam) + 1];
	strcpy(mNaam, naam);
	mBeschrijving = new char[strlen(algemeneOmschrijving) + 1];
	strcpy(mBeschrijving, algemeneOmschrijving);
	mGedetailleerdeBeschrijving = new char[strlen(gedetailleerdeOmschrijving) + 1];
	strcpy(mGedetailleerdeBeschrijving, gedetailleerdeOmschrijving);
}

Locatie::~Locatie() { clear(); }

//Copy constructor
Locatie::Locatie(const Locatie& other) { copyFrom(other); }

//Assignment operator
Locatie& Locatie::operator=(const Locatie& other)
{
	if (this != &other)
	{
		clear();
		copyFrom(other);
	}
	return *this;
}

//Move constructor
Locatie::Locatie(Locatie&& other) noexcept { moveFrom(std::move(other)); }

Locatie& Locatie::operator=(Locatie&& other) noexcept
{
	if (this != &other)
	{
		clear();
		moveFrom(std::move(other));
	}
	return *this;
}

void Locatie::copyFrom(const Locatie& other)
{
	mId = other.mId;
	mNaam = new char[strlen(other.mNaam) + 1];
	strcpy(mNaam, other.mNaam);
	mBeschrijving = new char[strlen(other.mBeschrijving) + 1];
	strcpy(mBeschrijving, other.mBeschrijving);
	mGedetailleerdeBeschrijving = new char[strlen(other.mGedetailleerdeBeschrijving) + 1];
	strcpy(mGedetailleerdeBeschrijving, other.mGedetailleerdeBeschrijving);
	mNoord = other.mNoord;
	mOost = other.mOost;
	mZuid = other.mZuid;
	mWest = other.mWest;
	mVerborgenObjecten = other.mVerborgenObjecten;
	mZichtbareObjecten = other.mZichtbareObjecten;
	mVijanden = other.mVijanden;
	mUitgangen = other.mUitgangen;
	mRichtingen = other.mRichtingen;
}

void Locatie::moveFrom(Locatie&& other) noexcept
{
	mId = other.mId;
	mNaam = other.mNaam;
	mBeschrijving = other.mBeschrijving;
	mGedetailleerdeBeschrijving = other.mGedetailleerdeBeschrijving;
	mNoord = other.mNoord;
	mOost = other.mOost;
	mZuid = other.mZuid;
	mWest = other.mWest;
	mVerborgenObjecten = std::move(other.mVerborgenObjecten);
	mZichtbareObjecten = std::move(other.mZichtbareObjecten);
	mVijanden = std::move(other.mVijanden);
	mUitgangen = std::move(other.mUitgangen);
	mRichtingen = std::move(other.mRichtingen);

	other.mNaam = nullptr;
	other.mBeschrijving = nullptr;
	other.mGedetailleerdeBeschrijving = nullptr;
}

void Locatie::clear() {
    delete[] mNaam;
    mNaam = nullptr;
    delete[] mBeschrijving;
    mBeschrijving = nullptr;
    delete[] mGedetailleerdeBeschrijving;
    mGedetailleerdeBeschrijving = nullptr;

    for (auto& obj : mVerborgenObjecten)
    {
        delete obj;
    }
    mVerborgenObjecten.clear();

    for (auto& obj : mZichtbareObjecten)
    {
        delete obj;
    }
    mZichtbareObjecten.clear();

    for (auto& vijand : mVijanden)
    {
        delete vijand;
    }
    mVijanden.clear();

    for (auto& locatie : mUitgangen)
    {
        delete locatie;
    }
    mUitgangen.clear();

    for (auto& richting : mRichtingen)
    {
        delete[] richting;
    }
    mRichtingen.clear();
}

void Locatie::voegVijandToe(Vijand* vijand) { mVijanden.push_back(vijand); }

void Locatie::voegZichtbaarObjectToe(Spelobject* object) { mZichtbareObjecten.push_back(object); }

void Locatie::voegVerborgenObjectToe(Spelobject* object) { mVerborgenObjecten.push_back(object); }

void Locatie::voegUitgangToe(const char* richting, int locatieId)
{
	if (strcmp(richting, "noord") == 0)
	{
		mNoord = locatieId;
	}
	else if (strcmp(richting, "oost") == 0)
	{
		mOost = locatieId;
	}
	else if (strcmp(richting, "zuid") == 0)
	{
		mZuid = locatieId;
	}
	else if (strcmp(richting, "west") == 0)
	{
		mWest = locatieId;
	}
}

void Locatie::verwijderVijand(Vijand* vijand)
{
	for (std::size_t i = 0; i < mVijanden.size(); ++i)
	{
		if (mVijanden[i] == vijand)
		{
			mVijanden.erase(i);
			return;
		}
	}
}

int Locatie::getNoord() const { return mNoord; }

int Locatie::getOost() const { return mOost; }

int Locatie::getZuid() const { return mZuid; }

int Locatie::getWest() const { return mWest; }

void Locatie::setUitgang(const char* richting, Locatie* locatie)
{
	if (strcmp(richting, "noord") == 0)
	{
		mNoord = locatie->mId;
	}
	else if (strcmp(richting, "oost") == 0)
	{
		mOost = locatie->mId;
	}
	else if (strcmp(richting, "zuid") == 0)
	{
		mZuid = locatie->mId;
	}
	else if (strcmp(richting, "west") == 0)
	{
		mWest = locatie->mId;
	}
}

int Locatie::getId() const { return mId; }

const char* Locatie::getNaam() const { return mNaam; }

const char* Locatie::getAlgemeneOmschrijving() const { return mBeschrijving; }

const char* Locatie::getGedetailleerdeOmschrijving() const { return mGedetailleerdeBeschrijving; }

Spelobject* Locatie::getVerborgenObject(int index) const
{
	if (index < 0 || index >= mVerborgenObjecten.size())
	{
		return nullptr;
	}
	return mVerborgenObjecten[index];
}

Spelobject* Locatie::getZichtbaarObject(int index) const
{
	if (index < 0 || index >= mZichtbareObjecten.size())
	{
		return nullptr;
	}
	return mZichtbareObjecten[index];
}

int Locatie::getUitgang(const char* richting) const
{
	if (strcmp(richting, "noord") == 0)
	{
		return mNoord;
	}
	else if (strcmp(richting, "oost") == 0)
	{
		return mOost;
	}
	else if (strcmp(richting, "zuid") == 0)
	{
		return mZuid;
	}
	else if (strcmp(richting, "west") == 0)
	{
		return mWest;
	}
	return -1;
}

Vijand* Locatie::getVijand(int index) const
{
	if (index < 0 || index >= mVijanden.size())
	{
		return nullptr;
	}
	return mVijanden[index];
}

int Locatie::getZichtbareObjectenCount() const { return mZichtbareObjecten.size(); }

int Locatie::getVerborgenObjectenCount() const { return mVerborgenObjecten.size(); }

int Locatie::getVijandenCount() const { return mVijanden.size(); }

Spelobject* Locatie::getZichtbaarObjectByName(const std::string& naam) const
{
	for (std::size_t i = 0; i < mZichtbareObjecten.size(); i++)
	{
		if (mZichtbareObjecten[i]->getNaam() == naam)
		{
			return mZichtbareObjecten[i];
		}
	}
	return nullptr;
}

Vijand* Locatie::getVijandByName(const std::string& naam) const
{
	for (std::size_t i = 0; i < mVijanden.size(); ++i)
	{
		const char* vijandNaam = mVijanden[i]->getNaam();
		const char* verslagenPos = strstr(vijandNaam, " [Verslagen]");
		if (verslagenPos != nullptr)
		{
			// Compare only the part before "[Verslagen]"
			size_t baseNameLength = verslagenPos - vijandNaam;
			if (strncmp(vijandNaam, naam.c_str(), baseNameLength) == 0 && naam[baseNameLength] == '\0')
			{
				return mVijanden[i];
			}
		}
		else
		{
			// Compare the full name
			if (strcmp(vijandNaam, naam.c_str()) == 0)
			{
				return mVijanden[i];
			}
		}
	}
	return nullptr;
}

void Locatie::setID(int id) { mId = id; }

void Locatie::verplaatsVerborgenNaarZichtbaar(Spelobject* object)
{
	for (std::size_t i = 0; i < mVerborgenObjecten.size(); ++i)
	{
		if (mVerborgenObjecten[i] == object)
		{
			voegZichtbaarObjectToe(mVerborgenObjecten[i]);
			mVerborgenObjecten.erase(i);
			return;
		}
	}
}

void Locatie::verwijderZichtbaarObject(Spelobject* object)
{
	for (std::size_t i = 0; i < mZichtbareObjecten.size(); ++i)
	{
		if (mZichtbareObjecten[i] == object)
		{
			mZichtbareObjecten.erase(i);
			return;
		}
	}
}

void Locatie::printZichtbareObjecten() const
{
	if (mZichtbareObjecten.size() == 0)
	{
		Logger::getInstance().logOutput("  Geen zichtbare objecten\n");
		return;
	}

	for (std::size_t i = 0; i < mZichtbareObjecten.size(); ++i)
	{
		Spelobject* obj = mZichtbareObjecten[i];
		if (obj)
		{
			Logger::getInstance().logOutput("  - ");
			Logger::getInstance().logOutput(obj->getNaam());
			Logger::getInstance().logOutput("\n");
		}
	}
}

void Locatie::listExits() const
{
	if (mNoord != -1)
	{
		Logger::getInstance().logOutput("  - noord\n");
	}
	if (mOost != -1)
	{
		Logger::getInstance().logOutput("  - oost\n");
	}
	if (mZuid != -1)
	{
		Logger::getInstance().logOutput("  - zuid\n");
	}
	if (mWest != -1)
	{
		Logger::getInstance().logOutput("  - west\n");
	}
}