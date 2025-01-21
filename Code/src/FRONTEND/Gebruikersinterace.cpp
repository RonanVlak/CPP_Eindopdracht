#include "GebruikersInterface.h"
#include <iostream>

void GebruikersInterface::toonMenu() {
    std::cout << "Choose an option:" << std::endl;
    std::cout << "1. Create a location" << std::endl;
    std::cout << "2. Open a file" << std::endl;
    std::cout << "Enter your choice: ";
}

void GebruikersInterface::toonLocatie(Locatie* locatie) {
    if (locatie != nullptr) {
        std::cout << locatie->getNaam() << std::endl;
        std::cout << locatie->getAlgemeneOmschrijving() << std::endl;
    }
}

void GebruikersInterface::toonSpeler(const Speler& speler) {
    speler.toonGegevens();
}

void GebruikersInterface::toonActies() {
    std::cout << "Beschikbare acties:" << std::endl;
    std::cout << "1. Kijk" << std::endl;
    std::cout << "2. Zoek" << std::endl;
    std::cout << "3. Ga naar" << std::endl;
    std::cout << "4. Pak" << std::endl;
    std::cout << "5. Leg neer" << std::endl;
    std::cout << "6. Bekijk object" << std::endl;
    std::cout << "7. Bekijk vijand" << std::endl;
    std::cout << "8. Bekijk zelf" << std::endl;
    std::cout << "9. Sla" << std::endl;
    std::cout << "10. Draag wapenrusting" << std::endl;
    std::cout << "11. Draag wapen" << std::endl;
    std::cout << "12. Wacht" << std::endl;
    std::cout << "13. Consumeer" << std::endl;
    std::cout << "15. God mode" << std::endl;;
}

void GebruikersInterface::toonHelp() {
    std::cout << "Toon locatie" << std::endl;
    std::cout << "Toon acties" << std::endl;
    std::cout << "help" << std::endl;
    std::cout << "quit" << std::endl;
}

void GebruikersInterface::toonEindeSpel() {
    std::cout << "Game over." << std::endl;
}

std::string GebruikersInterface::vraagSpelerNaam() {
    std::cout << "Voer je naam in: ";
    std::string name;
    std::getline(std::cin, name);
    return name;
}

std::string GebruikersInterface::vraagActie() {
    std::cout << "Voer een actie in: ";
    std::string action;
    std::getline(std::cin, action);
    return action;
}