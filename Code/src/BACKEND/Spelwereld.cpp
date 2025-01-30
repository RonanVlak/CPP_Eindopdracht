#include "Spelwereld.h"
#include "DatabaseLoader.h"
#include "Logger.h"
#include "RandomEngine.h"
#include "WapenObject.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <random>

Spelwereld::Spelwereld() : mCurrentLocatie(nullptr) {}

Spelwereld::~Spelwereld() { clear(); }

//Copy constructor
Spelwereld::Spelwereld(const Spelwereld& other) { copyFrom(other); }

//Assignment operator
Spelwereld& Spelwereld::operator=(const Spelwereld& other)
{
	if (this != &other)
	{
		clear();
		copyFrom(other);
	}
	return *this;
}

//Move constructor
Spelwereld::Spelwereld(Spelwereld&& other) noexcept { moveFrom(std::move(other)); }

//Move assignment operator
Spelwereld& Spelwereld::operator=(Spelwereld&& other) noexcept
{
	if (this != &other)
	{
		clear();
		moveFrom(std::move(other));
	}
	return *this;
}

void Spelwereld::voegLocatieToe(Locatie* locatie) { mLocaties.push_back(locatie); }

Locatie* Spelwereld::getLocatie(const char* naam)
{
	for (int i = 0; i < mLocaties.size(); ++i)
	{
		if (std::strcmp(mLocaties[i]->getNaam(), naam) == 0)
		{
			return mLocaties[i];
		}
	}
	return nullptr;
}

Locatie* Spelwereld::getCurrentLocatie() const { return mCurrentLocatie; }

void Spelwereld::setCurrentLocatie(Locatie* locatie) { mCurrentLocatie = locatie; }

Locatie* Spelwereld::getLocatieById(int id) const
{
	for (int i = 0; i < mLocaties.size(); ++i)
	{
		if (mLocaties[i]->getId() == id)
		{
			return mLocaties[i];
		}
	}
	return nullptr;
}

Locatie* Spelwereld::getLocatieByIndex(int index) const
{
	if (index < 0 || index >= mLocaties.size())
	{
		return nullptr;
	}
	return mLocaties[index];
}

int Spelwereld::getLocatiesCount() const { return mLocaties.size(); }

int Spelwereld::getEnemiesDamage()
{
	int totalDamage = 0;
	Locatie* currentLocation = getCurrentLocatie();
	if (currentLocation)
	{
		for (int i = 0; i < currentLocation->getVijandenCount(); ++i)
		{
			Vijand* vijand = currentLocation->getVijand(i);
			if (vijand && !vijand->isVerslagen())
			{
				int damage = vijand->attack();
				totalDamage += damage;
				if (damage > 0)
				{
					Logger::getInstance().logOutput(vijand->getNaam());
					Logger::getInstance().logOutput(" heeft je " + std::to_string(damage) + " schade toegebracht.");
				}
			}
		}
	}
	return totalDamage;
}

void Spelwereld::verplaatsVijanden()
{
	// Reset move flags for all enemies first
	for (Locatie* locatie : mLocaties)
	{
		for (int j = 0; j < locatie->getVijandenCount(); ++j)
		{
			Vijand* vijand = locatie->getVijand(j);
			if (vijand && !vijand->isVerslagen())
			{
				vijand->setBewogen(false);
			}
		}
	}

	// Move enemies logic
	for (Locatie* locatie : mLocaties)
	{
		for (int j = 0; j < locatie->getVijandenCount(); ++j)
		{
			Vijand* vijand = locatie->getVijand(j);

			// Skip if enemy is defeated or already moved
			if (!vijand || vijand->isVerslagen() || vijand->getBewogen())
				continue;

			int moveChance = RandomEngine::getRandomInt(1, 100);
			if (moveChance <= 50)
			{
				// Get adjacent locations
				CustomVector<Locatie*> adjacentLocations = getAdjacentLocations(locatie);
				if (!adjacentLocations.size() > 0)
				{
					int randomIndex = RandomEngine::getRandomInt(0, adjacentLocations.size() - 1);
					Locatie* newLocation = adjacentLocations[randomIndex];

					// Move enemy
					newLocation->voegVijandToe(vijand);
					Logger::getInstance().logOutput(vijand->getNaam());
					Logger::getInstance().logOutput(" is verplaatst naar ");
					Logger::getInstance().logOutput(newLocation->getNaam());
					Logger::getInstance().logOutput(".\n");
					locatie->verwijderVijand(vijand);

					// Mark enemy as moved
					vijand->setBewogen(true);
				}
			}
		}
	}
}

void Spelwereld::shuffle(CustomVector<Locatie*>& locaties)
{
	std::mt19937& gen = RandomEngine::getInstance();
	for (int i = locaties.size() - 1; i > 0; --i)
	{
		int randomIndex = RandomEngine::getRandomInt(0, locaties.size() - 1);
		int j = RandomEngine::getRandomInt(0, i);
		std::swap(locaties[i], locaties[j]);
	}
}

CustomVector<Locatie*> Spelwereld::getAdjacentLocations(Locatie* locatie)
{
	CustomVector<Locatie*> adjacentLocations;
	if (locatie->getNoord() != -1)
	{
		adjacentLocations.push_back(getLocatieById(locatie->getNoord()));
	}
	if (locatie->getOost() != -1)
	{
		adjacentLocations.push_back(getLocatieById(locatie->getOost()));
	}
	if (locatie->getZuid() != -1)
	{
		adjacentLocations.push_back(getLocatieById(locatie->getZuid()));
	}
	if (locatie->getWest() != -1)
	{
		adjacentLocations.push_back(getLocatieById(locatie->getWest()));
	}
	return adjacentLocations;
}

void Spelwereld::generateRandomKerker(const char* databaseBestand)
{
	clear();

	DatabaseLoader dbLoader;
	int count = 0;

	// Load locations from the database
	CustomVector<Locatie*> locaties = dbLoader.laadLocaties(databaseBestand, count);

	// Load all enemies and objects from the database
	int vijandCount = 0;
	CustomVector<Vijand*> vijanden = dbLoader.laadVijanden(databaseBestand, vijandCount);

	int objectCount = 0;
	CustomVector<Spelobject*> objecten = dbLoader.laadSpelobjecten(databaseBestand, objectCount);

	// Randomly select a subset of locations
	std::mt19937& gen = RandomEngine::getInstance();
	int locatiesSize = locaties.size();
	shuffle(locaties);

	// Calculate the number of enemies required
	int numEnemiesRequired = std::ceil(static_cast<float>(locatiesSize) / 3.0f);

	// Add the selected locations to the game world and assign sequential IDs
	for (int i = 0; i < std::min(10, static_cast<int>(locaties.size())); ++i)
	{
		Locatie* locatie = locaties[i];
		locatie->setID(i); // Assign sequential IDs

		// Add exactly one enemy per 3 locations (rounded up)
		if (i % 3 == 0 && i / 3 < numEnemiesRequired && !vijanden.isEmpty())
		{
			int randomIndex = RandomEngine::getRandomInt(0, vijanden.size() - 1);
			if (randomIndex >= 0 && randomIndex < vijanden.size() && vijanden[randomIndex] != nullptr)
			{
				Vijand* vijand = vijanden[randomIndex];

				// Ensure unique name for the vijand
				char vijandNaam[256];
				strcpy(vijandNaam, vijand->getNaam());
				int vijandId = 1;
				for (int k = 0; k < vijanden.size(); ++k)
				{
					if (vijanden[k] != nullptr && strcmp(vijanden[k]->getNaam(), vijandNaam) == 0)
					{
						char newVijandNaam[256];
						snprintf(newVijandNaam, sizeof(newVijandNaam) - 1, "%.245s%d", vijandNaam, vijandId++);
						newVijandNaam[sizeof(newVijandNaam) - 1] = '\0';
						vijand->setNaam(newVijandNaam);
					}
				}

				// Assign random spelobjecten to the vijand
				int numObjects = RandomEngine::getRandomInt(vijand->getMinimumObjecten(), vijand->getMaximumObjecten());
				for (int k = 0; k < numObjects; ++k)
				{
					if (!objecten.isEmpty())
					{
						int objIndex = RandomEngine::getRandomInt(0, objecten.size() - 1);
						if (objIndex >= 0 && objIndex < objecten.size() && objecten[objIndex] != nullptr)
						{
							Spelobject* object = objecten[objIndex];

							// Ensure unique name for the object
							char objectNaam[256];
							strcpy(objectNaam, object->getNaam());
							int objectId = 1;
							for (int l = 0; l < objecten.size(); ++l)
							{
								if (objecten[l] != nullptr && strcmp(objecten[l]->getNaam(), objectNaam) == 0)
								{
									char newObjectNaam[256];
									snprintf(newObjectNaam, sizeof(newObjectNaam) - 1, "%.245s%d", objectNaam,
											 objectId++);
									newObjectNaam[sizeof(newObjectNaam) - 1] = '\0';
									object->setNaam(newObjectNaam);
								}
							}

							vijand->voegSpelobjectToe(object);
							objecten[objIndex] = nullptr; // Mark as moved
						}
					}
				}

				locatie->voegVijandToe(vijand);
				vijanden[randomIndex] = nullptr; // Mark as moved
			}
		}

		// Add random objects to the location
		if (!objecten.isEmpty())
		{
			int numObjects = RandomEngine::getRandomInt(1, 3); // Random number of objects between 1 and 3
			for (int j = 0; j < numObjects; ++j)
			{
				int randomIndex = RandomEngine::getRandomInt(0, objecten.size() - 1);
				if (randomIndex >= 0 && randomIndex < objecten.size() && objecten[randomIndex] != nullptr)
				{
					Spelobject* object = objecten[randomIndex];

					// Ensure unique name for the object
					char objectNaam[256];
					strcpy(objectNaam, object->getNaam());
					int objectId = 1;
					for (int k = 0; k < objecten.size(); ++k)
					{
						if (objecten[k] != nullptr && strcmp(objecten[k]->getNaam(), objectNaam) == 0)
						{
							char newObjectNaam[256];
							snprintf(newObjectNaam, sizeof(newObjectNaam) - 1, "%.245s%d", objectNaam, objectId++);
							newObjectNaam[sizeof(newObjectNaam) - 1] = '\0';
							object->setNaam(newObjectNaam);
						}
					}

					locatie->voegZichtbaarObjectToe(object);
					objecten[randomIndex] = nullptr; // Mark as moved
				}
			}
		}

		// Add random hidden objects to the location
		if (!objecten.isEmpty())
		{
			int numHiddenObjects = RandomEngine::getRandomInt(0, 2); // Random number of hidden objects between 0 and 2
			for (int j = 0; j < numHiddenObjects; ++j)
			{
				int randomIndex = RandomEngine::getRandomInt(0, objecten.size() - 1);
				if (randomIndex >= 0 && randomIndex < objecten.size() && objecten[randomIndex] != nullptr)
				{
					Spelobject* object = objecten[randomIndex];

					// Ensure unique name for the hidden object
					char objectNaam[256];
					strcpy(objectNaam, object->getNaam());
					int objectId = 1;
					for (int k = 0; k < objecten.size(); ++k)
					{
						if (objecten[k] != nullptr && strcmp(objecten[k]->getNaam(), objectNaam) == 0)
						{
							char newObjectNaam[256];
							snprintf(newObjectNaam, sizeof(newObjectNaam) - 1, "%.245s%d", objectNaam, objectId++);
							newObjectNaam[sizeof(newObjectNaam) - 1] = '\0';
							object->setNaam(newObjectNaam);
						}
					}

					locatie->voegVerborgenObjectToe(object);
					objecten[randomIndex] = nullptr; // Mark as moved
				}
			}
		}

		voegLocatieToe(locatie);
		locaties[i] = nullptr; // Mark as moved
	}

	// Clean up remaining locaties
	for (auto& locatie : locaties)
	{
		delete locatie;
	}

	// Clean up remaining vijanden
	for (auto& vijand : vijanden)
	{
		delete vijand;
	}

	// Clean up remaining objecten
	for (auto& object : objecten)
	{
		delete object;
	}

	// Connect locations in a single line using north and south exits
	int locatiesCount = getLocatiesCount();
	for (int i = 0; i < locatiesCount - 1; ++i)
	{
		Locatie* currentLocatie = getLocatieByIndex(i);
		Locatie* nextLocatie = getLocatieByIndex(i + 1);
		currentLocatie->voegUitgangToe("noord", nextLocatie->getId());
		nextLocatie->voegUitgangToe("zuid", currentLocatie->getId());
	}

	// Set the initial location
	if (getLocatiesCount() > 0)
	{
		setCurrentLocatie(getLocatieByIndex(0));
	}
}

WapenObject* Spelwereld::getStartWapen(const char* dbPath)
{
	DatabaseLoader dbLoader;
	int objectCount = 0;

	// Load all spelobjecten from the database
	CustomVector<Spelobject*> spelobjecten = dbLoader.laadSpelobjecten(dbPath, objectCount);
	const char* wapenNaam = "dolk";

	// Find the item named "Dolk"
	for (int i = 0; i < spelobjecten.size(); ++i)
	{
		if (strcmp(spelobjecten[i]->getNaam(), wapenNaam) == 0)
		{
			// Convert Spelobject to WapenObject using dynamic casting
			WapenObject* wapenPtr = dynamic_cast<WapenObject*>(spelobjecten[i]);
			if (wapenPtr)
			{
				spelobjecten[i] = nullptr; // Mark as moved
				// Clean up remaining Spelobjecten
				for (auto& obj : spelobjecten)
				{
					delete obj;
				}
				spelobjecten.clear();
				return wapenPtr;
			}
		}
	}
	for (auto& obj : spelobjecten)
	{
		delete obj;
	}
	spelobjecten.clear();
	return nullptr;
}

void Spelwereld::clear()
{
	for (auto locatie : mLocaties)
	{
		delete locatie;
	}
	mLocaties.clear();
}

void Spelwereld::copyFrom(const Spelwereld& other)
{
	clear();
	for (int i = 0; i < other.mLocaties.size(); ++i)
	{
		mLocaties.push_back(new Locatie(*other.mLocaties[i]));
	}
	mCurrentLocatie = other.mCurrentLocatie;
}

void Spelwereld::moveFrom(Spelwereld&& other) noexcept
{
	mLocaties = std::move(other.mLocaties);
	mCurrentLocatie = other.mCurrentLocatie;
	other.mCurrentLocatie = nullptr;
}