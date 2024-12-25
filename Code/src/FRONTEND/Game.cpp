#include "FRONTEND/Game.h"
#include "BACKEND/XMLParser.h"
#include "FSConverter.h"
#include "BACKEND/LocatieFactory.h"
#include <iostream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

Game::Game() 
    : mCurrentState(State::MainMenu),
      mSpelwereld(std::make_unique<Spelwereld>()),
      mSpeler(std::make_unique<Speler>("", 100, 50)), // Initialize Speler with default values
      mGebruikersInterface(std::make_unique<GebruikersInterface>()),
      mSpelerActieHandler(std::make_unique<SpelerActieHandler>(mSpelwereld, mSpeler, mGebruikersInterface)) {
}

Game::~Game() {
    mCurrentState = State::Quit;
}

void Game::start() {
    FSConverter fsConverter;
    std::string resourcesDirectory = fsConverter.getResourcePath("");
    std::string dbFileName = "kerkersendraken.db";
    std::string dbPath = fsConverter.getResourcePath(dbFileName);
    std::cout << "Game started!" << std::endl;
    std::string playerName = mGebruikersInterface->vraagSpelerNaam();
    if (playerName.empty()) {
        std::cout << "Fout: Speler naam is niet ingevuld!" << std::endl;
        return; // Of bied een nieuwe kans om de naam in te voeren
    }
    initSpeler(playerName, dbPath);
    while (mCurrentState != State::Quit) {
        switch (mCurrentState) {
            case State::MainMenu:
                mainMenu();
                break;
            case State::GameMenu:
                gameMenu();
                break;
            case State::Gameplay:
                gameplay();
                break;
            case State::Quit:
                break;
            default:
                std::cout << "Onbekende state." << std::endl;
                break;
        }
    }
}

void Game::mainMenu() {
    while (true) {
        std::cout << "Main Menu" << std::endl;
        std::cout << "1. Start Game" << std::endl;
        std::cout << "2. Quit" << std::endl;

        int choice;
        std::cin >> choice;
        // Check for valid input
        if (std::cin.fail()) {
            std::cin.clear(); // Clear the error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
            std::cout << "Onbekende keuze, probeer het opnieuw." << std::endl;
            continue; // Restart the loop to show the menu again
        }

        switch (choice) {
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

void Game::gameMenu() {
    while (true) { // Keep displaying the menu until a valid option is selected
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
        if (std::cin.fail()) {
            std::cin.clear(); // Clear the error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore bad input
            std::cout << "Onbekende keuze, probeer het opnieuw." << std::endl;
            continue; // Restart the loop to show the menu again
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer

        switch (choice) {
            case 1:
                genereerRandomKerker(dbPath); // Generate random dungeon
                mCurrentState = State::Gameplay; // Set the state to gameplay
                return; // Exit the menu function to proceed to gameplay
            case 2: {
                std::cout << "XML Files in " << resourcesDirectory << ":" << std::endl;
                listFilesInDirectory(resourcesDirectory);

                std::cout << "Voer de naam van een XML File in: ";
                char fileName[256];
                std::cin.getline(fileName, sizeof(fileName));

                std::string filePath = fsConverter.getResourcePath(fileName);
                if (filePath.empty() || filePath.substr(filePath.find_last_of(".") + 1) != "xml") {
                    std::cout << "File kon niet gevonden worden." << std::endl;
                } else {
                    laadKerkerVanXML(filePath, dbPath);
                    mCurrentState = State::Gameplay; // Set the state to gameplay
                    return; // Exit the menu function to proceed to gameplay
                }
                break;
            }
            case 3:
                mCurrentState = State::MainMenu; // Change state back to main menu
                return; // Exit the menu function to return to the main menu
            default:
                std::cout << "Onbekende keuze, probeer het opnieuw." << std::endl;
                break; // Continue to the next iteration
        }
    }
}


void Game::gameplay() {
    mSpelwereld->setCurrentLocatie(mSpelwereld->getLocatieByIndex(0));
    while (mCurrentState == State::Gameplay) {
        std::string action = mGebruikersInterface->vraagActie();
        if (action == "help") {
            mGebruikersInterface->toonHelp();
        } else if (action == "Toon acties") {
            mGebruikersInterface->toonActies();
        } else if (action == "Toon locatie") {
            mGebruikersInterface->toonLocatie();
        } else if (action == "quit") {
            mCurrentState = State::GameMenu;
        } else {
            verwerkActie(action);
            int totalDamage = mSpelwereld->getEnemiesDamage();
            if (totalDamage > 0) {
                 mSpeler->applyDamage(totalDamage);
           
            }
            if (mSpeler->getLevenspunten() <= 0) {
                std::cout << "Je bent verslagen!" << std::endl;
                mCurrentState = State::GameMenu;
            }
            else {
                mSpelwereld->verplaatsVijanden();
            }
        }

    }
    mGebruikersInterface->toonEindeSpel();
}

void Game::laadKerkerVanXML(const std::string& xmlBestand, const std::string& databaseBestand) {
    XMLParser parser;
    parser.parseFile(xmlBestand.c_str(), *mSpelwereld, databaseBestand.c_str());
}

void Game::genereerRandomKerker() {
    std::cout << "Random dungeon generated!" << std::endl;
}

void Game::initSpeler(const std::string& naam, const std::string& dbPath) {
    mSpeler->setNaam(naam);
    DatabaseLoader dbLoader;
    int objectCount = 0;

    // Load all spelobjecten from the database
    CustomVector<Spelobject*> spelobjecten = dbLoader.laadSpelobjecten(dbPath.c_str(), objectCount);
    std::string wapenNaam = "dolk"; 
    
    // Find the item named "Dolk"
    for (int i = 0; i < spelobjecten.size(); ++i) {
        if (strcmp(spelobjecten[i]->getNaam(), wapenNaam.c_str()) == 0) {
            // Convert Spelobject to WapenObject using dynamic casting
            WapenObject* wapenPtr = dynamic_cast<WapenObject*>(spelobjecten[i]);
            if (wapenPtr) {
                std::unique_ptr<WapenObject> uniqueDolk(wapenPtr);
                mSpeler->draagWapen(std::move(uniqueDolk));
                std::cout << "Wapen 'dolk' is ingesteld voor de speler." << std::endl;
                break;
            }
        }
    }

    std::cout << "Speler geinitialiseerd: " << naam << std::endl;
}

void Game::verwerkActie(const std::string& actie) {
    mSpelerActieHandler->verwerkActie(actie);
}

void Game::genereerRandomKerker(const std::string& databaseBestand) {
    DatabaseLoader dbLoader;
    int count = 0;

    // Load locations from the database
    CustomVector<Locatie*> locaties = dbLoader.laadLocaties(databaseBestand.c_str(), count);

    // Load all enemies and objects from the database
    int vijandCount = 0;
    CustomVector<Vijand*> vijanden = dbLoader.laadVijanden(databaseBestand.c_str(), vijandCount);

    int objectCount = 0;
    CustomVector<Spelobject*> objecten = dbLoader.laadSpelobjecten(databaseBestand.c_str(), objectCount);

    // Randomly select a subset of locations
    std::random_device rd;
    std::mt19937 gen(rd());
    int locatiesSize = locaties.size();
    std::shuffle(locaties.begin(), locaties.end(), gen);

    // Add the selected locations to the game world and assign sequential IDs
    for (int i = 0; i < std::min(10, static_cast<int>(locaties.size())); ++i) {
        Locatie* locatie = locaties[i];
        locatie->setID(i); // Assign sequential IDs from 1 to 10

        // Add random enemies to the location
        if (!vijanden.isEmpty()) {
            std::uniform_int_distribution<> vijandDis(0, vijanden.size() - 1);
            int numEnemies = vijandDis(gen) % 2 + 1; // Random number of enemies between 1 and 3
            for (int j = 0; j < numEnemies; ++j) {
                int randomIndex = vijandDis(gen);
                if (randomIndex >= 0 && randomIndex < vijanden.size()) {
                    locatie->voegVijandToe(vijanden[randomIndex]);
                }
            }
        }

        // Add random objects to the location
        if (!objecten.isEmpty()) {
            std::uniform_int_distribution<> objectDis(0, objecten.size() - 1);
            int numObjects = objectDis(gen) % 3 + 1; // Random number of objects between 1 and 3
            for (int j = 0; j < numObjects; ++j) {
                int randomIndex = objectDis(gen);
                if (randomIndex >= 0 && randomIndex < objecten.size()) {
                    locatie->voegZichtbaarObjectToe(objecten[randomIndex]);
                }
            }
        }

        mSpelwereld->voegLocatieToe(locatie);
    }

    // Create the adjacency matrix
    int locatiesCount = mSpelwereld->getLocatiesCount();
    int** adjacencyMatrix = new int*[locatiesCount];
    for (int i = 0; i < locatiesCount; ++i) {
        adjacencyMatrix[i] = new int[locatiesCount];
        for (int j = 0; j < locatiesCount; ++j) {
            adjacencyMatrix[i][j] = 0;
        }
    }

    // Set random exits for the locations
    for (int i = 0; i < locatiesCount; ++i) {
        std::uniform_int_distribution<> exitDis(0, locatiesCount - 1);
        int noord = exitDis(gen);
        int oost = exitDis(gen);
        int zuid = exitDis(gen);
        int west = exitDis(gen);

        if (noord != i && noord >= 0 && noord < locatiesCount) {
            adjacencyMatrix[i][noord] = 1;
            adjacencyMatrix[noord][i] = 1;
        }
        if (oost != i && oost >= 0 && oost < locatiesCount) {
            adjacencyMatrix[i][oost] = 1;
            adjacencyMatrix[oost][i] = 1;
        }
        if (zuid != i && zuid >= 0 && zuid < locatiesCount) {
            adjacencyMatrix[i][zuid] = 1;
            adjacencyMatrix[zuid][i] = 1;
        }
        if (west != i && west >= 0 && west < locatiesCount) {
            adjacencyMatrix[i][west] = 1;
            adjacencyMatrix[west][i] = 1;
        }
    }

    // Set the exits based on the adjacency matrix
    for (int i = 0; i < locatiesCount; ++i) {
        for (int j = 0; j < locatiesCount; ++j) {
            if (adjacencyMatrix[i][j] == 1) {
                Locatie* locatieI = mSpelwereld->getLocatieByIndex(i);
                Locatie* locatieJ = mSpelwereld->getLocatieByIndex(j);
                if (locatieI->getNoord() == -1 && locatieJ->getZuid() == -1) {
                    locatieI->voegUitgangToe("noord", locatieJ->getId());
                    locatieJ->voegUitgangToe("zuid", locatieI->getId());
                } else if (locatieI->getOost() == -1 && locatieJ->getWest() == -1) {
                    locatieI->voegUitgangToe("oost", locatieJ->getId());
                    locatieJ->voegUitgangToe("west", locatieI->getId());
                } else if (locatieI->getZuid() == -1 && locatieJ->getNoord() == -1) {
                    locatieI->voegUitgangToe("zuid", locatieJ->getId());
                    locatieJ->voegUitgangToe("noord", locatieI->getId());
                } else if (locatieI->getWest() == -1 && locatieJ->getOost() == -1) {
                    locatieI->voegUitgangToe("west", locatieJ->getId());
                    locatieJ->voegUitgangToe("oost", locatieI->getId());
                }
            }
        }
    }

    // Clean up the adjacency matrix
    for (int i = 0; i < locatiesCount; ++i) {
        delete[] adjacencyMatrix[i];
    }
    delete[] adjacencyMatrix;

    // Set the initial location
    if (mSpelwereld->getLocatiesCount() > 0) {
        mSpelwereld->setCurrentLocatie(mSpelwereld->getLocatieByIndex(0));
    }

    std::cout << "Random dungeon generated!" << std::endl;
}

void Game::listFilesInDirectory(const std::string& directory) {
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".xml") {
            std::cout << entry.path().filename().string() << std::endl;
        }
    }
}