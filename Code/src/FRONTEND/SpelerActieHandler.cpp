#include "SpelerActieHandler.h"
#include "SpelwereldFacade.h"
#include "RandomEngine.h"
#include <iostream>

SpelerActieHandler::SpelerActieHandler(std::unique_ptr<SpelwereldFacade>& aSpelwereld, std::unique_ptr<Speler>& aSpeler,
									   std::unique_ptr<GebruikersInterface>& aGebruikersInterface)
	: mSpelwereld(*aSpelwereld), mSpeler(*aSpeler), mGebruikersInterface(*aGebruikersInterface)
{
}


SpelerActieHandler::Actie SpelerActieHandler::stringNaarActie(const std::string& aActie)
{
	if (aActie == "Kijk")
		return Actie::Kijk;
	if (aActie == "Zoek")
		return Actie::Zoek;
	if (aActie == "Ga naar")
		return Actie::GaNaar;
	if (aActie == "Pak")
		return Actie::Pak;
	if (aActie == "Leg neer")
		return Actie::LegNeer;
	if (aActie == "Bekijk object")
		return Actie::BekijkObject;
	if (aActie == "Bekijk vijand")
		return Actie::BekijkVijand;
	if (aActie == "Bekijk zelf")
		return Actie::BekijkZelf;
	if (aActie == "Sla")
		return Actie::Sla;
	if (aActie == "Draag wapenrusting")
		return Actie::DraagWapenrusting;
	if (aActie == "Draag wapen")
		return Actie::DraagWapen;
	if (aActie == "Wacht")
		return Actie::Wacht;
	if (aActie == "Consumeer")
		return Actie::Consumeer;
	if (aActie == "God mode")
		return Actie::GodMode;
	return Actie::Onbekend;
}

void SpelerActieHandler::verwerkActie(const std::string& aActie)
{
	Actie actie = stringNaarActie(aActie);
	switch (actie)
	{
	case Actie::Kijk:
		kijk();
		break;
	case Actie::Zoek:
		zoek();
		break;
	case Actie::GaNaar:
	{
		std::string richting;
		std::cout << "Voer richting in: ";
		std::cin >> richting;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		gaNaar(richting);
		break;
	}
	case Actie::Pak:
	{
		std::string objectnaam;
		std::cout << "Voer object in: ";
		std::getline(std::cin, objectnaam);
		pak(objectnaam);
		break;
	}
	case Actie::LegNeer:
	{
		std::string objectnaam;
		std::cout << "Voer object in: ";
		std::getline(std::cin, objectnaam);
		legNeer(objectnaam);
		break;
	}
	case Actie::BekijkObject:
	{
		std::string objectnaam;
		std::cout << "Voer object in: ";
		std::getline(std::cin, objectnaam);
		bekijkObject(objectnaam);
		break;
	}
	case Actie::BekijkVijand:
	{
		std::string vijandnaam;
		std::cout << "Voer naam vijand in: ";
		std::getline(std::cin, vijandnaam);
		bekijkVijand(vijandnaam);
		break;
	}
	case Actie::BekijkZelf:
		bekijkZelf();
		break;
	case Actie::Sla:
	{
		std::string vijandnaam;
		std::cout << "Voer naam vijand in: ";
		std::getline(std::cin, vijandnaam);
		sla(vijandnaam);
		break;
	}
	case Actie::DraagWapenrusting:
	{
		std::string wapenrustingnaam;
		std::cout << "Voer wapenrusting naam in: ";
		std::getline(std::cin, wapenrustingnaam);
		draagWapenrusting(wapenrustingnaam);
		break;
	}
	case Actie::DraagWapen:
	{
		std::string wapennaam;
		std::cout << "Voer wapen naam in: ";
		std::getline(std::cin, wapennaam);
		draagWapen(wapennaam);
		break;
	}
	case Actie::Wacht:
		wacht();
		break;
	case Actie::Consumeer:
	{
		std::string objectnaam;
		std::cout << "Voer objectnaam in: ";
		std::getline(std::cin, objectnaam);
		consumeer(objectnaam);
		break;
	}
	case Actie::GodMode:
		godMode();
		break;
	default:
		std::cout << "Onbekende actie: " << aActie << std::endl;
		break;
	}
}

void SpelerActieHandler::kijk()
{
	if (mSpelwereld.getCurrentLocatie())
	{
		std::cout << "Locatie: " << mSpelwereld.getCurrentLocatie()->getNaam() << std::endl;
		std::cout << "Beschrijving: " << mSpelwereld.getCurrentLocatie()->getAlgemeneOmschrijving() << std::endl;

		std::cout << "Zichtbare objecten: " << std::endl;
		mSpelwereld.getCurrentLocatie()->printZichtbareObjecten();

		std::cout << "Vijanden: " << std::endl;
		int vijandenCount = mSpelwereld.getCurrentLocatie()->getVijandenCount();
		for (int i = 0; i < vijandenCount; ++i)
		{
			Vijand* vijand = mSpelwereld.getCurrentLocatie()->getVijand(i);
			if (vijand != nullptr)
			{
				std::cout << "  - " << vijand->getNaam() << std::endl;
			}
		}
		if (vijandenCount == 0)
		{
			std::cout << "  Geen vijanden" << std::endl;
		}

		std::cout << "Uitgangen:" << std::endl;
		mSpelwereld.getCurrentLocatie()->listExits();
	}
	else
	{
		std::cout << "Je bent niet in een valide locatie" << std::endl;
	}
}

void SpelerActieHandler::zoek()
{
	if (mSpelwereld.getCurrentLocatie())
	{
		std::vector<Spelobject*> hiddenObjectsToMove;

		// Collect hidden objects that do not belong to enemies
		for (int i = 0; i < mSpelwereld.getCurrentLocatie()->getVerborgenObjectenCount(); ++i)
		{
			Spelobject* obj = mSpelwereld.getCurrentLocatie()->getVerborgenObject(i);
			hiddenObjectsToMove.push_back(obj);
		}

		if (hiddenObjectsToMove.empty())
		{
			std::cout << "Geen verborgen objecten gevonden." << std::endl;
			return;
		}

		// Move collected hidden objects to visible objects
		for (Spelobject* obj : hiddenObjectsToMove)
		{
			mSpelwereld.getCurrentLocatie()->verplaatsVerborgenNaarZichtbaar(obj);
		}

		std::cout << "Je hebt verborgen objecten gevonden!" << std::endl;
	}
	else
	{
		std::cout << "Je bent niet in een valide locatie." << std::endl;
	}
}

void SpelerActieHandler::gaNaar(const std::string& aRichting)
{
	if (mSpelwereld.getCurrentLocatie())
	{
		int nextLocationId = mSpelwereld.getCurrentLocatie()->getUitgang(aRichting.c_str());
		if (nextLocationId != -1)
		{
			mSpelwereld.getEnemiesDamage();
			mSpelwereld.setCurrentLocatie(mSpelwereld.getLocatieById(nextLocationId));
			std::cout << "Je bent verplaats naar de richting  " << aRichting << "." << std::endl;
			mGebruikersInterface.toonLocatie(mSpelwereld.getCurrentLocatie());
		}
		else
		{
			std::cout << "Er is geen uitgang naar de ricting  " << aRichting << "." << std::endl;
		}
	}
	else
	{
		std::cout << "Je bent niet in een valide locatie." << std::endl;
	}
}

void SpelerActieHandler::pak(const std::string& aObjectnaam)
{
    if (mSpelwereld.getCurrentLocatie())
    {
        // Check if the object is in the current location
        for (int i = 0; i < mSpelwereld.getCurrentLocatie()->getZichtbareObjectenCount(); ++i)
        {
            Spelobject* obj = mSpelwereld.getCurrentLocatie()->getZichtbaarObject(i);
            if (obj && obj->getNaam() == aObjectnaam)
            {
                mSpeler.voegObjectToe(obj);
                mSpelwereld.getCurrentLocatie()->verwijderZichtbaarObject(obj);
                std::cout << "Je hebt een item opgepakt: " << aObjectnaam << "." << std::endl;
                return;
            }
        }
        // Check if the object is with a defeated enemy
        for (int i = 0; i < mSpelwereld.getCurrentLocatie()->getVijandenCount(); ++i)
        {
            Vijand* vijand = mSpelwereld.getCurrentLocatie()->getVijand(i);
            if (vijand && vijand->isVerslagen())
            {
                for (int j = 0; j < vijand->getAantalSpelobjecten(); ++j)
                {
                    Spelobject* obj = vijand->getSpelobject(j);
                    if (obj && obj->getNaam() == aObjectnaam)
                    {
                        mSpeler.voegObjectToe(obj);
                        vijand->removeSpelobject(obj);
                        std::cout << "Je hebt een item opgepakt van " << vijand->getNaam() << ": " << aObjectnaam << "."
                                  << std::endl;
                        return;
                    }
                }
            }
        }

        std::cout << "Object " << aObjectnaam << " is niet gevonden in je huidige locatie." << std::endl;
    }
    else
    {
        std::cout << "Je bent niet in een valide locatie." << std::endl;
    }
}

void SpelerActieHandler::legNeer(const std::string& aObjectnaam)
{
	if (mSpelwereld.getCurrentLocatie())
	{
		for (int i = 0; i < mSpeler.getConsumeerbareObjecten().size(); ++i)
		{
			if (mSpeler.getConsumeerbareObjecten()[i]->getNaam() == aObjectnaam)
			{
				auto temp = std::move(mSpeler.getConsumeerbareObjecten()[i]);
				mSpeler.getConsumeerbareObjecten().erase(mSpeler.getConsumeerbareObjecten().begin() + i);
				mSpelwereld.getCurrentLocatie()->voegZichtbaarObjectToe(temp.release());
				std::cout << "Je hebt een item neergelegd: " << aObjectnaam << "." << std::endl;
				return;
			}
		}
		for (int i = 0; i < mSpeler.getWapenInventaris().size(); ++i)
		{
			if (mSpeler.getWapenInventaris()[i]->getNaam() == aObjectnaam)
			{
				auto temp = std::move(mSpeler.getWapenInventaris()[i]);
				mSpeler.getWapenInventaris().erase(mSpeler.getWapenInventaris().begin() + i);
				mSpelwereld.getCurrentLocatie()->voegZichtbaarObjectToe(temp.release());
				std::cout << "Je hebt een item neergelegd: " << aObjectnaam << "." << std::endl;
				return;
			}
		}
		for (int i = 0; i < mSpeler.getWapenrustingInventaris().size(); ++i)
		{
			if (mSpeler.getWapenrustingInventaris()[i]->getNaam() == aObjectnaam)
			{
				auto temp = std::move(mSpeler.getWapenrustingInventaris()[i]);
				mSpeler.getWapenrustingInventaris().erase(mSpeler.getWapenrustingInventaris().begin() + i);
				mSpelwereld.getCurrentLocatie()->voegZichtbaarObjectToe(temp.release());
				std::cout << "Je hebt een item neergelegd: " << aObjectnaam << "." << std::endl;
				return;
			}
		}
		std::cout << "Object " << aObjectnaam << " is niet gevonden in je inventaris." << std::endl;
	}
	else
	{
		std::cout << "Je bent niet in een valide locatie." << std::endl;
	}
}

void SpelerActieHandler::bekijkObject(const std::string& aObjectnaam)
{
	if (mSpelwereld.getCurrentLocatie())
	{
		Spelobject* obj = mSpelwereld.getCurrentLocatie()->getZichtbaarObjectByName(aObjectnaam);
		if (obj)
		{
			std::cout << "Object: " << obj->getNaam() << std::endl;
			std::cout << "Beschrijving: " << obj->getBeschrijving() << std::endl;
		}
		else
		{
			std::cout << "Object " << aObjectnaam << " is niet gevonden in je huidige locatie." << std::endl;
		}
	}
	else
	{
		std::cout << "Je bent niet in een valide locatie." << std::endl;
	}
}

void SpelerActieHandler::bekijkVijand(const std::string& aVijandnaam)
{
	if (mSpelwereld.getCurrentLocatie())
	{
		Vijand* vijand = mSpelwereld.getCurrentLocatie()->getVijandByName(aVijandnaam);
		if (vijand)
		{
			vijand->bekijk();
			if (vijand->isVerslagen())
			{
				if (vijand->getAantalSpelobjecten() <= 0)
				{
					std::cout << "Vijand heeft geen objecten." << std::endl;
				}
				else
				{
					std::cout << "Objecten van " << vijand->getNaam() << ":" << std::endl;
					for (int i = 0; i < vijand->getAantalSpelobjecten(); i++)
					{
						mSpelwereld.getCurrentLocatie()->voegZichtbaarObjectToe(vijand->getSpelobject(i));
					}
				}
			}
		}
		else
		{
			std::cout << "Vijand " << aVijandnaam << " is niet gevonden in je huidige locatie." << std::endl;
		}
	}
	else
	{
		std::cout << "Je bent niet in een valide locatie." << std::endl;
	}
}

void SpelerActieHandler::bekijkZelf() { mSpeler.toonGegevens(); }

void SpelerActieHandler::sla(const std::string& aVijandnaam)
{
	if (mSpelwereld.getCurrentLocatie())
	{
		Vijand* vijand = mSpelwereld.getCurrentLocatie()->getVijandByName(aVijandnaam);
		if (vijand)
		{
			mSpeler.sla(vijand);
			if (vijand->isVerslagen())
			{
				std::cout << vijand->getNaam() << " is verslagen!" << std::endl;
			}
		}
		else
		{
			std::cout << "Vijand " << aVijandnaam << " is niet gevonden in je huidige locatie." << std::endl;
		}
	}
	else
	{
		std::cout << "Je bent niet in een valide locatie." << std::endl;
	}
}

void SpelerActieHandler::draagWapenrusting(const std::string& aWapenrustingnaam)
{
	for (int i = 0; i < mSpeler.getWapenrustingInventaris().size(); ++i)
	{
		if (strcmp(mSpeler.getWapenrustingInventaris()[i]->getNaam(), aWapenrustingnaam.c_str()) == 0)
		{
			auto huidigWapen = std::move(mSpeler.getHuidigWapen());
			mSpeler.draagWapenrusting(std::move(mSpeler.getWapenrustingInventaris()[i]));
			mSpeler.getWapenrustingInventaris().erase(mSpeler.getWapenrustingInventaris().begin() + i);
			mSpeler.voegObjectToe(huidigWapen.release());
			std::cout << "Je draagt nu " << aWapenrustingnaam << "." << std::endl;
			return;
		}
	}
	std::cout << "Wapenrusting " << aWapenrustingnaam << " is niet gevonden in je inventaris." << std::endl;
}

void SpelerActieHandler::draagWapen(const std::string& aWapennaam)
{
	for (int i = 0; i < mSpeler.getWapenInventaris().size(); ++i)
	{
		if (strcmp(mSpeler.getWapenInventaris()[i]->getNaam(), aWapennaam.c_str()) == 0)
		{
			mSpeler.draagWapen(std::move(mSpeler.getWapenInventaris()[i]));
			mSpeler.getWapenInventaris().erase(mSpeler.getWapenInventaris().begin() + i);
			std::cout << "Je draagt nu " << aWapennaam << "." << std::endl;
			return;
		}
	}
	std::cout << "Wapen " << aWapennaam << " is niet gevonden in je inventaris." << std::endl;
}

void SpelerActieHandler::wacht() { std::cout << "Je hebt gewacht." << std::endl; }

void SpelerActieHandler::consumeer(const std::string& aObjectnaam)
{
    for (auto it = mSpeler.getConsumeerbareObjecten().begin(); it != mSpeler.getConsumeerbareObjecten().end(); ++it)
    {
        if (std::string((*it)->getNaam()).find(aObjectnaam) != std::string::npos)
        {
            if (aObjectnaam.find("levenselixer") != std::string::npos)
            {
                mSpeler.voegLevenspuntenToe(std::move(*it));
                std::cout << "Je hebt " << aObjectnaam << " geconsumeerd." << std::endl;
            }
            else if (aObjectnaam.find("ervaringsdrank") != std::string::npos)
            {
                int huidigeAanvalskans = mSpeler.getAanvalskans();
                int nieuweAanvalskans = std::min(huidigeAanvalskans + 10, 90);
                mSpeler.setAanvalskans(nieuweAanvalskans);
                std::cout << "Je hebt " << aObjectnaam << " geconsumeerd." << std::endl;
                std::cout << "Je aanvalskans is verhoogd naar " << nieuweAanvalskans << "%." << std::endl;
            }
            else if (aObjectnaam.find("teleportatiedrank") != std::string::npos)
            {
                int nieuweLocatieIndex = RandomEngine::getRandomInt(0, mSpelwereld.getLocatiesCount() - 1);
                Locatie* nieuweLocatie = mSpelwereld.getLocatieByIndex(nieuweLocatieIndex);
                mSpelwereld.setCurrentLocatie(nieuweLocatie);
                std::cout << "Je hebt " << aObjectnaam << " geconsumeerd." << std::endl;
                std::cout << "Je bent geteleporteerd naar een nieuwe locatie." << std::endl;
                mGebruikersInterface.toonLocatie(mSpelwereld.getCurrentLocatie());
            }
            mSpeler.getConsumeerbareObjecten().erase(it);
            return;
        }
    }
    std::cout << "Object " << aObjectnaam << " is niet gevonden in je inventaris." << std::endl;
}

void SpelerActieHandler::godMode()
{
	if (mSpeler.getGodMode())
	{
		mSpeler.setGodMode(false);
		std::cout << "God mode is uitgeschakeld." << std::endl;
		return;
	}
	else
	{
		mSpeler.setGodMode(true);
		std::cout << "God mode is ingeschakeld." << std::endl;
	}
}