#include "GebruikersInterface.h"
#include "Logger.h"
#include <iostream>

void GebruikersInterface::toonMenu() const
{
	Logger::getInstance().logOutput("Choose an option:\n");
	Logger::getInstance().logOutput("1. Create a location\n");
	Logger::getInstance().logOutput("2. Open a file\n");
	Logger::getInstance().logOutput("Enter your choice: ");
}

void GebruikersInterface::toonLocatie(Locatie* locatie) const
{
    if (locatie != nullptr)
    {
        Logger::getInstance().logOutput(std::string(locatie->getNaam()) + "\n");
        Logger::getInstance().logOutput(std::string(locatie->getAlgemeneOmschrijving()) + "\n");
    }
}

void GebruikersInterface::toonSpeler(const Speler& speler) const { speler.toonGegevens(); }

void GebruikersInterface::toonActies() const
{
	Logger::getInstance().logOutput("Beschikbare acties:\n");
	Logger::getInstance().logOutput("1. Kijk\n");
	Logger::getInstance().logOutput("2. Zoek\n");
	Logger::getInstance().logOutput("3. Ga naar\n");
	Logger::getInstance().logOutput("4. Pak\n");
	Logger::getInstance().logOutput("5. Leg neer\n");
	Logger::getInstance().logOutput("6. Bekijk object\n");
	Logger::getInstance().logOutput("7. Bekijk vijand\n");
	Logger::getInstance().logOutput("8. Bekijk zelf\n");
	Logger::getInstance().logOutput("9. Sla\n");
	Logger::getInstance().logOutput("10. Draag wapenrusting\n");
	Logger::getInstance().logOutput("11. Draag wapen\n");
	Logger::getInstance().logOutput("12. Wacht\n");
	Logger::getInstance().logOutput("13. Consumeer\n");
	Logger::getInstance().logOutput("14. God mode\n");
}

void GebruikersInterface::toonHelp() const
{
	Logger::getInstance().logOutput("Toon locatie\n");
	Logger::getInstance().logOutput("Toon acties\n");
	Logger::getInstance().logOutput("help\n");
	Logger::getInstance().logOutput("quit\n");
}

void GebruikersInterface::toonEindeSpel() const { Logger::getInstance().logOutput("Game over.\n"); }

std::string GebruikersInterface::vraagSpelerNaam()
{
	Logger::getInstance().logOutput("Voer je naam in: ");
	std::string name;
	std::getline(std::cin, name);
	Logger::getInstance().logInput(name);
	return name;
}

std::string GebruikersInterface::vraagActie()
{
	Logger::getInstance().logOutput("Voer een actie in: \n");
	std::string action;
	std::getline(std::cin, action);
	Logger::getInstance().logInput(action);
	return action;
}