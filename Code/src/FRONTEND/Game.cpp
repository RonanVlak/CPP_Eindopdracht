#include "FRONTEND/Game.h"
#include "BACKEND/LocatieFactory.h"
#include "BACKEND/XMLParser.h"
#include "FSConverter.h"
#include <algorithm>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

Game::Game()
	: mCurrentState(State::MainMenu), mSpelwereld(std::make_unique<Spelwereld>()),
	  mSpeler(std::make_unique<Speler>("", 10, 50, false)), // Initialize Speler with default values
	  mGebruikersInterface(std::make_unique<GebruikersInterface>()),
	  mSpelerActieHandler(std::make_unique<SpelerActieHandler>(mSpelwereld, mSpeler, mGebruikersInterface))
{
	mXMLDungeon = false;
	mRandomDungeon = false;
	mPlayerName = "";
	mDbPath = "";
}

Game::~Game() { mCurrentState = State::Quit; }

void Game::start()
{
	FSConverter fsConverter;
	std::string resourcesDirectory = fsConverter.getResourcePath("");
	std::string dbFileName = "kerkersendraken.db";
	mDbPath = fsConverter.getResourcePath(dbFileName);

	std::system("clear");

	std::cout << "Game started!" << std::endl;

	while (mPlayerName.empty())
	{
		mPlayerName = mGebruikersInterface->vraagSpelerNaam();
		if (mPlayerName.empty())
		{
			std::cerr << "Fout: Speler naam is niet ingevuld!" << std::endl;
		}
	}

	initSpeler(mPlayerName, mDbPath);
	while (mCurrentState != State::Quit)
	{
		switch (mCurrentState)
		{
		case State::MainMenu:
			mainMenu();
			break;
		case State::GameMenu:
			gameMenu();
			break;
		case State::Gameplay:
			gameplay();
			break;
		case State::DeathMenu:
			deathMenu();
			break;
		case State::Quit:
			break;
		default:
			std::cerr << "Onbekende state." << std::endl;
			break;
		}
	}
}

void Game::mainMenu()
{
	while (true)
	{
		std::cout << "Main Menu" << std::endl;
		std::cout << "1. Start Game" << std::endl;
		std::cout << "2. Quit" << std::endl;

		int choice;
		std::cin >> choice;
		// Check for valid input
		if (std::cin.fail())
		{
			std::cin.clear();													// Clear the error state
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
			std::cout << "Onbekende keuze, probeer het opnieuw." << std::endl;
			continue; // Restart the loop to show the menu again
		}

		switch (choice)
		{
		case 1:
			mCurrentState = State::GameMenu;
			return;
			break;
		case 2:
			mCurrentState = State::Quit;
			return;
			break;
		default:
			std::cout << "Onbekende keuze." << std::endl;
			break;
		}
	}
}

void Game::gameMenu()
{
	while (true)
	{ // Keep displaying the menu until a valid option is selected
		FSConverter fsConverter;
		std::string resourcesDirectory = fsConverter.getResourcePath("");
		std::string dbFileName = "kerkersendraken.db";
		std::string dbPath = fsConverter.getResourcePath(dbFileName);

		std::cout << "Game Menu" << std::endl;
		std::cout << "1. Start spel met random dungeon" << std::endl;
		std::cout << "2. Laad XML File" << std::endl;
		std::cout << "3. Terug naar hoofdmenu" << std::endl;

		int choice;
		std::cin >> choice;

		// Check for valid input
		if (std::cin.fail())
		{
			std::cin.clear();													// Clear the error state
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
			std::cout << "Onbekende keuze, probeer het opnieuw." << std::endl;
			continue; // Restart the loop to show the menu again
		}

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer

		switch (choice)
		{
		case 1:
			genereerRandomKerker(dbPath);	 // Generate random dungeon
			mRandomDungeon = true;			 // Set random dungeon flag to true
			mXMLDungeon = false;			 // Set XML dungeon flag to false
			mCurrentState = State::Gameplay; // Set the state to gameplay
			return;							 // Exit the menu function to proceed to gameplay
		case 2:
		{
			std::cout << "XML Files in " << resourcesDirectory << ":" << std::endl;
			listFilesInDirectory(resourcesDirectory);

			std::cout << "Voer de naam van een XML File in: ";
			char fileName[256];
			std::cin.getline(fileName, sizeof(fileName));

			std::string filePath = fsConverter.getResourcePath(fileName);
			if (filePath.empty() || filePath.substr(filePath.find_last_of(".") + 1) != "xml")
			{
				std::cout << "File kon niet gevonden worden." << std::endl;
			}
			else
			{
				laadKerkerVanXML(filePath, dbPath);
				mXMLDungeon = true;				 // Set XML dungeon flag to true
				mRandomDungeon = false;			 // Set random dungeon flag to false
				mCurrentState = State::Gameplay; // Set the state to gameplay
				return;							 // Exit the menu function to proceed to gameplay
			}
			break;
		}
		case 3:
			mCurrentState = State::MainMenu; // Change state back to main menu
			return;							 // Exit the menu function to return to the main menu
		default:
			std::cout << "Onbekende keuze, probeer het opnieuw." << std::endl;
			break; // Continue to the next iteration
		}
	}
}

void Game::gameplay()
{
	mSpelwereld->setCurrentLocatie(mSpelwereld->getLocatieByIndex(0));
	mGebruikersInterface->toonLocatie(mSpelwereld->getCurrentLocatie());
	while (mCurrentState == State::Gameplay)
	{
		std::string action = mGebruikersInterface->vraagActie();
		if (action == "help")
		{
			mGebruikersInterface->toonHelp();
		}
		else if (action == "Toon acties")
		{
			mGebruikersInterface->toonActies();
		}
		else if (action == "Toon locatie")
		{
			mGebruikersInterface->toonLocatie(mSpelwereld->getCurrentLocatie());
		}
		else if (action == "quit")
		{
			mCurrentState = State::GameMenu;
		}
		else
		{
			verwerkActie(action);
			if (action == "Kijk" || action == "Wacht" || action == "Sla" || action == "Draag wapenrusting")
			{
				int totalDamage = mSpelwereld->getEnemiesDamage();
				if (totalDamage > 0)
				{
					if (!mSpeler->getGodMode())
					{
						mSpeler->applyDamage(totalDamage);
					}
				}
				if (mSpeler->getLevenspunten() <= 0)
				{
					std::cout << "Je bent verslagen!" << std::endl;
					DatabaseLoader dbLoader;
					dbLoader.voegLeaderboardToe(mDbPath.c_str(), mSpeler->getNaam().c_str(),
												mSpeler->getGoudstukken());
					mCurrentState = State::DeathMenu;
				}
				else
				{
					mSpelwereld->verplaatsVijanden();
				}
			}
		}
		std::cout << std::endl;
	}
	mGebruikersInterface->toonEindeSpel();
}

void Game::deathMenu()
{
	initSpeler(mPlayerName, mDbPath);

	while (true)
	{
		std::cout << "Wat wil je doen?" << std::endl;
		std::cout << "1. Opnieuw spelen" << std::endl;
		std::cout << "2. Terug naar Main Menu" << std::endl;

		int choice;
		std::cin >> choice;

		// Check for valid input
		if (std::cin.fail())
		{
			std::cin.clear();													// Clear the error state
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
			std::cout << "Onbekende keuze, probeer het opnieuw." << std::endl;
			continue; // Restart the loop to show the menu again
		}

		switch (choice)
		{
		case 1:
			if (mRandomDungeon)
			{
				genereerRandomKerker(); // Generate random dungeon
			}
			else
			{
				laadKerkerVanXML("kerker.xml", "kerkersendraken.db");
			}
			mCurrentState = State::Gameplay; // Set the state to gameplay
			return;							 // Exit the menu function to restart the game
		case 2:
			mCurrentState = State::MainMenu; // Set the state to main menu
			return;							 // Exit the menu function to return to the main menu
		default:
			std::cout << "Onbekende keuze, probeer het opnieuw." << std::endl;
			break; // Continue to the next iteration
		}
	}
}

void Game::laadKerkerVanXML(const std::string& xmlBestand, const std::string& databaseBestand)
{
	XMLParser parser;
	parser.parseFile(xmlBestand.c_str(), *mSpelwereld, databaseBestand.c_str());
}

void Game::genereerRandomKerker() { std::cout << "Random dungeon generated!" << std::endl; }

void Game::initSpeler(const std::string& naam, const std::string& dbPath)
{
	mSpeler->clear();
	mSpeler->setNaam(naam);
	WapenObject* wapenPtr = mSpelwereld->getStartWapen(dbPath.c_str());
	if (wapenPtr)
	{
		std::unique_ptr<WapenObject> uniqueDolk(wapenPtr);
		mSpeler->draagWapen(std::move(uniqueDolk));
		std::cout << "Wapen 'dolk' is ingesteld voor de speler." << std::endl;
	}
	else
	{
		throw std::runtime_error("Wapen kon niet worden ingesteld voor de speler.");
	}
	std::cout << "Speler geinitialiseerd: " << naam << std::endl;
}

void Game::verwerkActie(const std::string& actie) { mSpelerActieHandler->verwerkActie(actie); }

void Game::genereerRandomKerker(const std::string& databaseBestand)
{
	mSpelwereld->generateRandomKerker(databaseBestand.c_str());
	std::cout << "Random dungeon generated!" << std::endl;
}

void Game::listFilesInDirectory(const std::string& directory)
{
	for (const auto& entry : fs::directory_iterator(directory))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".xml")
		{
			std::cout << entry.path().filename().string() << std::endl;
		}
	}
}