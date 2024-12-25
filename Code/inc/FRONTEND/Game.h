#ifndef GAME_H
#define GAME_H

#include <memory>
#include <string>
#include "Spelwereld.h"
#include "Speler.h"
#include "SpelerActieHandler.h"
#include "GebruikersInterface.h"
#include "XMLParser.h"
#include "FSConverter.h"
#include "LocatieFactory.h"
#include <iostream>
#include <filesystem>
#include "DatabaseLoader.h"
class Game {
public:
    Game();
    ~Game();

    void start();
    void laadKerkerVanXML(const std::string& xmlBestand, const std::string& databaseBestand);
    void genereerRandomKerker();
    void initSpeler(const std::string& naam, const std::string& dbPath);
    void verwerkActie(const std::string& actie);
    void genereerRandomKerker(const std::string& databaseBestand);
    void listFilesInDirectory(const std::string& directory);

private:

    enum class State {
        MainMenu,
        GameMenu,
        Gameplay,
        Quit
    };

    void mainMenu();
    void gameMenu();    
    void gameplay();

    State mCurrentState;
    std::unique_ptr<Spelwereld> mSpelwereld;
    std::unique_ptr<Speler> mSpeler;
    std::unique_ptr<SpelerActieHandler> mSpelerActieHandler;
    std::unique_ptr<GebruikersInterface> mGebruikersInterface;
};

#endif // GAME_H