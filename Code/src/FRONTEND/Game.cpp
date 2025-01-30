#include "FRONTEND/Game.h"
#include "BACKEND/LocatieFactory.h"
#include "BACKEND/XMLParser.h"
#include "FRONTEND/Logger.h"
#include "FRONTEND/SpelwereldFacade.h"
#include "FSConverter.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>

namespace fs = std::filesystem;

Game::Game()
	: mCurrentState(State::MainMenu), mSpelwereld(std::make_unique<SpelwereldFacade>()),
	  mSpeler(std::make_unique<Speler>("", 20, 40, false)), // Initialize Speler with default values
	  mGebruikersInterface(std::make_unique<GebruikersInterface>()),
	  mSpelerActieHandler(std::make_unique<SpelerActieHandler>(mSpelwereld, mSpeler, mGebruikersInterface))
{
	mXMLDungeon = false;
	mRandomDungeon = false;
	mPlayerName = "";
	mDbPath = "";
	mFirstRun = true;
}

void Game::start()
{
	try
	{
		FSConverter fsConverter;
		std::string resourcesDirectory = fsConverter.getResourcePath("");
		std::string dbFileName = "kerkersendraken.db";
		mDbPath = fsConverter.getResourcePath(dbFileName);

		std::system("clear");

		Logger::getInstance().logOutput("Game gestart!\n");

		printLeaderboard(mDbPath);
		while (mCurrentState != State::Quit)
		{
			try
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
			catch (const std::exception& e)
			{
				std::cerr << "Error during game loop: " << e.what() << std::endl;
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error starting game: " << e.what() << std::endl;
	}
}

void Game::mainMenu()
{
	while (true)
	{
		try
		{
			Logger::getInstance().logOutput("Main Menu\n");
			Logger::getInstance().logOutput("1. Start Game\n");
			Logger::getInstance().logOutput("2. Quit\n");

			int choice;
			std::cin >> choice;
			Logger::getInstance().logInput(std::to_string(choice) + "\n");

			// Check for valid input
			if (std::cin.fail())
			{
				std::cin.clear();													// Clear the error state
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
				Logger::getInstance().logOutput("Onbekende keuze, probeer het opnieuw.\n");
				continue; // Restart the loop to show the menu again
			}

			switch (choice)
			{
			case 1:
				mCurrentState = State::GameMenu;
				return;
			case 2:
				mCurrentState = State::Quit;
				return;
			default:
				Logger::getInstance().logOutput("Onbekende keuze.\n");
				break;
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error in main menu: " << e.what() << std::endl;
		}
	}
}

void Game::gameMenu()
{
	while (true)
	{
		try
		{
			FSConverter fsConverter;
			std::string resourcesDirectory = fsConverter.getResourcePath("");
			std::string dbFileName = "kerkersendraken.db";
			std::string dbPath = fsConverter.getResourcePath(dbFileName);

			Logger::getInstance().logOutput("Game Menu\n");
			Logger::getInstance().logOutput("1. Start spel met random dungeon\n");
			Logger::getInstance().logOutput("2. Laad XML File\n");
			Logger::getInstance().logOutput("3. Terug naar hoofdmenu\n");

			int choice;
			std::cin >> choice;
			Logger::getInstance().logInput(std::to_string(choice) + "\n");

			// Check for valid input
			if (std::cin.fail())
			{
				std::cin.clear();													// Clear the error state
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
				Logger::getInstance().logOutput("Onbekende keuze, probeer het opnieuw.\n");
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
				std::string filePath = enterXMLFile(resourcesDirectory);
				if (filePath.empty() || filePath.substr(filePath.find_last_of(".") + 1) != "xml")
				{
					Logger::getInstance().logOutput("File kon niet gevonden worden.\n");
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
				Logger::getInstance().logOutput("Onbekende keuze, probeer het opnieuw.\n");
				break; // Continue to the next iteration
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error in game menu: " << e.what() << std::endl;
		}
	}
}

void Game::gameplay()
{
	try
	{
		if (!mFirstRun)
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		do
		{
			mPlayerName = mGebruikersInterface->vraagSpelerNaam();

			if (mPlayerName.empty())
			{
				Logger::getInstance().logOutput("Fout: Speler naam is niet ingevuld!\n");
			}
		} while (mPlayerName.empty());

		initSpeler(mPlayerName, mDbPath);

		mSpelwereld->setCurrentLocatie(mSpelwereld->getLocatieByIndex(0));
		mGebruikersInterface->toonLocatie(mSpelwereld->getCurrentLocatie());

		while (mCurrentState == State::Gameplay)
		{
			try
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
							Logger::getInstance().logOutput("Je bent verslagen!\n");
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
				Logger::getInstance().logOutput("\n");
			}
			catch (const std::exception& e)
			{
				std::cerr << "Error during gameplay: " << e.what() << std::endl;
			}
		}
		mGebruikersInterface->toonEindeSpel();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error in gameplay: " << e.what() << std::endl;
	}
}

void Game::deathMenu()
{
	try
	{
		printLeaderboard(mDbPath);
		mSpelwereld->clear();
		mPlayerName = "";
		mFirstRun = false;

		FSConverter fsConverter;
		std::string resourcesDirectory = fsConverter.getResourcePath("");
		while (true)
		{
			try
			{
				Logger::getInstance().logOutput("Wat wil je doen?\n");
				Logger::getInstance().logOutput("1. Opnieuw spelen\n");
				Logger::getInstance().logOutput("2. Quit\n");

				int choice;
				std::cin >> choice;
				Logger::getInstance().logInput("> " + std::to_string(choice) + "\n");

				// Check for valid input
				if (std::cin.fail())
				{
					std::cin.clear();													// Clear the error state
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
					Logger::getInstance().logOutput("Onbekende keuze, probeer het opnieuw.\n");
					continue; // Restart the loop to show the menu again
				}

				switch (choice)
				{
				case 1:
					if (mRandomDungeon)
					{
						genereerRandomKerker(mDbPath); // Generate random dungeon
					}
					else
					{
						std::string filePath = enterXMLFile(resourcesDirectory);
						if (filePath.empty() || filePath.substr(filePath.find_last_of(".") + 1) != "xml")
						{
							Logger::getInstance().logOutput("File kon niet gevonden worden.\n");
						}
						else
						{
							laadKerkerVanXML(filePath, mDbPath);
							mXMLDungeon = true;				 // Set XML dungeon flag to true
							mRandomDungeon = false;			 // Set random dungeon flag to false
							mCurrentState = State::Gameplay; // Set the state to gameplay
							return;							 // Exit the menu function to proceed to gameplay
						}
					}
					mCurrentState = State::Gameplay; // Set the state to gameplay
					return;							 // Exit the menu function to restart the game
				case 2:
					mCurrentState = State::Quit; // Set the state to main menu
					return;							 // Exit the menu function to return to the main menu
				default:
					Logger::getInstance().logOutput("Onbekende keuze, probeer het opnieuw.\n");
					break; // Continue to the next iteration
				}
			}
			catch (const std::exception& e)
			{
				std::cerr << "Error in death menu: " << e.what() << std::endl;
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error initializing death menu: " << e.what() << std::endl;
	}
}

void Game::laadKerkerVanXML(const std::string& xmlBestand, const std::string& databaseBestand)
{
	try
	{
		XMLParser parser;
		parser.parseFile(xmlBestand.c_str(), *mSpelwereld->getSpelwereld(), databaseBestand.c_str());
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error loading dungeon from XML: " << e.what() << std::endl;
	}
}

void Game::initSpeler(const std::string& naam, const std::string& dbPath)
{
	try
	{
		mSpeler->clear();
		mSpeler->setNaam(naam);
		auto wapenPtr = mSpelwereld->getStartWapen(dbPath.c_str());
		if (wapenPtr)
		{
			mSpeler->draagWapen(std::move(wapenPtr));
			Logger::getInstance().logOutput("Wapen 'dolk' is ingesteld voor de speler.\n");
		}
		else
		{
			throw std::runtime_error("Wapen kon niet worden ingesteld voor de speler.");
		}
		Logger::getInstance().logOutput("Speler geinitialiseerd: " + naam + "\n");
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error initializing player: " << e.what() << std::endl;
	}
}

void Game::verwerkActie(const std::string& actie)
{
	try
	{
		mSpelerActieHandler->verwerkActie(actie);
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error processing action: " << e.what() << std::endl;
	}
}

void Game::genereerRandomKerker(const std::string& databaseBestand)
{
	try
	{
		mSpelwereld->generateRandomKerker(databaseBestand.c_str());
		Logger::getInstance().logOutput("Random dungeon gegenereerd!\n");
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error generating random dungeon: " << e.what() << std::endl;
	}
}

void Game::listFilesInDirectory(const std::string& directory) const
{
	try
	{
		for (const auto& entry : fs::directory_iterator(directory))
		{
			if (entry.is_regular_file() && entry.path().extension() == ".xml")
			{
				Logger::getInstance().logOutput(entry.path().filename().string() + "\n");
			}
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error listing files in directory: " << e.what() << std::endl;
	}
}

void Game::printLeaderboard(const std::string& dbPath) const
{
	try
	{
		DatabaseLoader dbLoader;
		Logger::getInstance().logOutput("Leaderboard:\n");
		dbLoader.printLeaderboard(dbPath.c_str());
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error printing leaderboard: " << e.what() << std::endl;
	}
}

std::string Game::enterXMLFile(const std::string& resourcesDirectory) const
{
	try
	{
		Logger::getInstance().logOutput("XML Files in " + resourcesDirectory + ":\n");
		listFilesInDirectory(resourcesDirectory);

		Logger::getInstance().logOutput("Voer de naam van een XML File in: ");
		char fileName[256];
		std::cin.getline(fileName, sizeof(fileName));
		Logger::getInstance().logInput(std::string(fileName));

		FSConverter fsConverter;
		std::string filePath = fsConverter.getResourcePath(fileName);
		return filePath;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error entering XML file: " << e.what() << std::endl;
		return "";
	}
}