#include "Spelwereld.h"
#include "DatabaseLoader.h"
#include "WapenObject.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <random>

Spelwereld::Spelwereld() : mCurrentLocatie(nullptr) {}

Spelwereld::~Spelwereld() { clear(); }

Spelwereld::Spelwereld(const Spelwereld& other) { copyFrom(other); }

Spelwereld& Spelwereld::operator=(const Spelwereld& other)
{
	if (this != &other)
	{
		clear();
		copyFrom(other);
	}
	return *this;
}

Spelwereld::Spelwereld(Spelwereld&& other) noexcept { moveFrom(std::move(other)); }

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
					std::cout << vijand->getNaam() << " heeft je " << damage << " schade toegebracht." << std::endl;
				}
			}
		}
	}
	return totalDamage;
}

void Spelwereld::verplaatsVijanden()
{
	Locatie* currentLocation = getCurrentLocatie();
	if (currentLocation)
	{
		for (int i = 0; i < currentLocation->getVijandenCount(); ++i)
		{
			Vijand* vijand = currentLocation->getVijand(i);
			if (vijand && !vijand->isVerslagen())
			{
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> dis(1, 100);
				int moveChance = dis(gen);

				if (moveChance <= 50)
				{ // 50% chance to move
					// Get a random adjacent location
					CustomVector<Locatie*> adjacentLocations = getAdjacentLocations(currentLocation);
					if (adjacentLocations.size() > 0)
					{
						std::uniform_int_distribution<> locDis(0, adjacentLocations.size() - 1);
						Locatie* newLocation = adjacentLocations[locDis(gen)];
						newLocation->voegVijandToe(vijand);
						std::cout << vijand->getNaam() << " is verplaatst naar " << newLocation->getNaam() << "."
								  << std::endl;
						currentLocation->verwijderVijand(vijand);
					}
				}
			}
		}
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
	std::random_device rd;
	std::mt19937 gen(rd());
	int locatiesSize = locaties.size();
	std::shuffle(locaties.begin(), locaties.end(), gen);

	// Add the selected locations to the game world and assign sequential IDs
	for (int i = 0; i < std::min(10, static_cast<int>(locaties.size())); ++i)
	{
		Locatie* locatie = locaties[i];
		locatie->setID(i); // Assign sequential IDs from 1 to 10

		// Add random enemies to the location
		if (!vijanden.isEmpty())
		{
			std::uniform_int_distribution<> vijandDis(0, vijanden.size() - 1);
			int numEnemies = vijandDis(gen) % 2 + 1; // Random number of enemies between 1 and 3
			for (int j = 0; j < numEnemies; ++j)
			{
				int randomIndex = vijandDis(gen);
				if (randomIndex >= 0 && randomIndex < vijanden.size())
				{
					locatie->voegVijandToe(vijanden[randomIndex]);
					vijanden[randomIndex] = nullptr;
				}
			}
		}

		// Add random objects to the location
		if (!objecten.isEmpty())
		{
			std::uniform_int_distribution<> objectDis(0, objecten.size() - 1);
			int numObjects = objectDis(gen) % 3 + 1; // Random number of objects between 1 and 3
			for (int j = 0; j < numObjects; ++j)
			{
				int randomIndex = objectDis(gen);
				if (randomIndex >= 0 && randomIndex < objecten.size())
				{
					locatie->voegZichtbaarObjectToe(objecten[randomIndex]);
					objecten[randomIndex] = nullptr;
				}
			}
		}

		voegLocatieToe(locatie);
		locaties[i] = nullptr;
	}

	// Clean up remaining locaties
	for (auto& locatie : locaties)
	{
		delete locatie;
	}
	locaties.clear();

	// Clean up remaining vijanden
	for (auto& vijand : vijanden)
	{
		delete vijand;
	}
	vijanden.clear();

	// Clean up remaining objecten
	for (auto& object : objecten)
	{
		delete object;
	}
	objecten.clear();

	// Create the adjacency matrix
	int locatiesCount = getLocatiesCount();
	int** adjacencyMatrix = new int*[locatiesCount];
	for (int i = 0; i < locatiesCount; ++i)
	{
		adjacencyMatrix[i] = new int[locatiesCount];
		for (int j = 0; j < locatiesCount; ++j)
		{
			adjacencyMatrix[i][j] = 0;
		}
	}

	// Set random exits for the locations
	for (int i = 0; i < locatiesCount; ++i)
	{
		std::uniform_int_distribution<> exitDis(0, locatiesCount - 1);
		int noord = exitDis(gen);
		int oost = exitDis(gen);
		int zuid = exitDis(gen);
		int west = exitDis(gen);

		if (noord != i && noord >= 0 && noord < locatiesCount)
		{
			adjacencyMatrix[i][noord] = 1;
			adjacencyMatrix[noord][i] = 1;
		}
		if (oost != i && oost >= 0 && oost < locatiesCount)
		{
			adjacencyMatrix[i][oost] = 1;
			adjacencyMatrix[oost][i] = 1;
		}
		if (zuid != i && zuid >= 0 && zuid < locatiesCount)
		{
			adjacencyMatrix[i][zuid] = 1;
			adjacencyMatrix[zuid][i] = 1;
		}
		if (west != i && west >= 0 && west < locatiesCount)
		{
			adjacencyMatrix[i][west] = 1;
			adjacencyMatrix[west][i] = 1;
		}
	}

	// Set the exits based on the adjacency matrix
	for (int i = 0; i < locatiesCount; ++i)
	{
		for (int j = 0; j < locatiesCount; ++j)
		{
			if (adjacencyMatrix[i][j] == 1)
			{
				Locatie* locatieI = getLocatieByIndex(i);
				Locatie* locatieJ = getLocatieByIndex(j);
				if (locatieI->getNoord() == -1 && locatieJ->getZuid() == -1)
				{
					locatieI->voegUitgangToe("noord", locatieJ->getId());
					locatieJ->voegUitgangToe("zuid", locatieI->getId());
				}
				else if (locatieI->getOost() == -1 && locatieJ->getWest() == -1)
				{
					locatieI->voegUitgangToe("oost", locatieJ->getId());
					locatieJ->voegUitgangToe("west", locatieI->getId());
				}
				else if (locatieI->getZuid() == -1 && locatieJ->getNoord() == -1)
				{
					locatieI->voegUitgangToe("zuid", locatieJ->getId());
					locatieJ->voegUitgangToe("noord", locatieI->getId());
				}
				else if (locatieI->getWest() == -1 && locatieJ->getOost() == -1)
				{
					locatieI->voegUitgangToe("west", locatieJ->getId());
					locatieJ->voegUitgangToe("oost", locatieI->getId());
				}
			}
		}
	}

	// Clean up the adjacency matrix
	for (int i = 0; i < locatiesCount; ++i)
	{
		delete[] adjacencyMatrix[i];
	}
	delete[] adjacencyMatrix;

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