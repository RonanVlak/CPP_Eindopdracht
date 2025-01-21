#ifndef GAME_H
#define GAME_H

#include "DatabaseLoader.h"
#include "FSConverter.h"
#include "GebruikersInterface.h"
#include "LocatieFactory.h"
#include "Speler.h"
#include "SpelerActieHandler.h"
#include "Spelwereld.h"
#include "XMLParser.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

class Game
{
public:
	Game();
	~Game();

	void start();
	void laadKerkerVanXML(const std::string& xmlBestand, const std::string& databaseBestand);
	void genereerRandomKerker();
	void initSpeler(const std::string& naam, const std::string& dbPath);
	void verwerkActie(const std::string& actie);
	void genereerRandomKerker(const std::string& databaseBestand);

private:
	enum class State
	{
		MainMenu,
		GameMenu,
		Gameplay,
		DeathMenu,
		Quit
	};

	void mainMenu();
	void gameMenu();
	void gameplay();
	void deathMenu();
	void printLeaderboard(const std::string& dbPath) const;
	void listFilesInDirectory(const std::string& directory) const;
	std::string enterXMLFile(const std::string& resourcesDirectory) const;

	bool mRandomDungeon;
	bool mXMLDungeon;
	std::string mPlayerName;
    std::string mDbPath;

	State mCurrentState;
	std::unique_ptr<SpelwereldFacade> mSpelwereld;
	std::unique_ptr<Speler> mSpeler;
	std::unique_ptr<SpelerActieHandler> mSpelerActieHandler;
	std::unique_ptr<GebruikersInterface> mGebruikersInterface;
};

#endif // GAME_H