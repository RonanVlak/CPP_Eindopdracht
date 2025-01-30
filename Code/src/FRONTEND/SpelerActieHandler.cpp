#include "SpelerActieHandler.h"
#include "Logger.h"
#include "RandomEngine.h"
#include "SpelwereldFacade.h"
#include <algorithm>
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
		Logger::getInstance().logOutput("Voer richting in: ");
		std::cin >> richting;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		Logger::getInstance().logInput("\n" + richting);
		gaNaar(richting);
		break;
	}
	case Actie::Pak:
	{
		std::string objectnaam;
		Logger::getInstance().logOutput("Voer object in: ");
		std::getline(std::cin, objectnaam);
		Logger::getInstance().logInput("\n" + objectnaam);
		pak(objectnaam);
		break;
	}
	case Actie::LegNeer:
	{
		std::string objectnaam;
		Logger::getInstance().logOutput("Voer object in: ");
		std::getline(std::cin, objectnaam);
		Logger::getInstance().logInput("\n" + objectnaam);
		legNeer(objectnaam);
		break;
	}
	case Actie::BekijkObject:
	{
		std::string objectnaam;
		Logger::getInstance().logOutput("Voer object in: ");
		std::getline(std::cin, objectnaam);
		Logger::getInstance().logInput("\n" + objectnaam);
		bekijkObject(objectnaam);
		break;
	}
	case Actie::BekijkVijand:
	{
		std::string vijandnaam;
		Logger::getInstance().logOutput("Voer naam vijand in: ");
		std::getline(std::cin, vijandnaam);
		Logger::getInstance().logInput("\n" + vijandnaam);
		bekijkVijand(vijandnaam);
		break;
	}
	case Actie::BekijkZelf:
		bekijkZelf();
		break;
	case Actie::Sla:
	{
		std::string vijandnaam;
		Logger::getInstance().logOutput("Voer naam vijand in: ");
		std::getline(std::cin, vijandnaam);
		Logger::getInstance().logInput("\n" + vijandnaam);
		sla(vijandnaam);
		break;
	}
	case Actie::DraagWapenrusting:
	{
		std::string wapenrustingnaam;
		Logger::getInstance().logOutput("Voer wapenrusting naam in: ");
		std::getline(std::cin, wapenrustingnaam);
		Logger::getInstance().logInput("\n" + wapenrustingnaam);
		draagWapenrusting(wapenrustingnaam);
		break;
	}
	case Actie::DraagWapen:
	{
		std::string wapennaam;
		Logger::getInstance().logOutput("Voer wapen naam in: ");
		std::getline(std::cin, wapennaam);
		Logger::getInstance().logInput("\n" + wapennaam);
		draagWapen(wapennaam);
		break;
	}
	case Actie::Wacht:
		wacht();
		break;
	case Actie::Consumeer:
	{
		std::string objectnaam;
		Logger::getInstance().logOutput("Voer object in: ");
		std::getline(std::cin, objectnaam);
		Logger::getInstance().logInput("\n" + objectnaam);
		consumeer(objectnaam);
		break;
	}
	case Actie::GodMode:
		godMode();
		break;
	default:
		Logger::getInstance().logOutput("Onbekende actie: " + aActie + "\n");
		break;
	}
}

void SpelerActieHandler::kijk()
{
	if (mSpelwereld.getCurrentLocatie())
	{
		Logger::getInstance().logOutput("Locatie: " + std::string(mSpelwereld.getCurrentLocatie()->getNaam()) + "\n");
		Logger::getInstance().logOutput(
			"Beschrijving: " + std::string(mSpelwereld.getCurrentLocatie()->getAlgemeneOmschrijving()) + "\n");

		Logger::getInstance().logOutput("Zichtbare objecten: \n");
		mSpelwereld.getCurrentLocatie()->printZichtbareObjecten();

		Logger::getInstance().logOutput("Vijanden: \n");
		int vijandenCount = mSpelwereld.getCurrentLocatie()->getVijandenCount();
		for (int i = 0; i < vijandenCount; ++i)
		{
			Vijand* vijand = mSpelwereld.getCurrentLocatie()->getVijand(i);
			if (vijand != nullptr)
			{
				Logger::getInstance().logOutput("  - " + std::string(vijand->getNaam()) + "\n");
			}
		}
		if (vijandenCount == 0)
		{
			Logger::getInstance().logOutput("  Geen vijanden\n");
		}

		Logger::getInstance().logOutput("Uitgangen:\n");
		mSpelwereld.getCurrentLocatie()->listExits();
	}
	else
	{
		Logger::getInstance().logOutput("Je bent niet in een valide locatie\n");
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
			Logger::getInstance().logOutput("Geen verborgen objecten gevonden.\n");
			return;
		}

		// Move collected hidden objects to visible objects
		for (Spelobject* obj : hiddenObjectsToMove)
		{
			mSpelwereld.getCurrentLocatie()->verplaatsVerborgenNaarZichtbaar(obj);
		}

		Logger::getInstance().logOutput("Je hebt verborgen objecten gevonden!\n");
	}
	else
	{
		Logger::getInstance().logOutput("Je bent niet in een valide locatie.\n");
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
			Logger::getInstance().logOutput("Je bent verplaats naar de richting " + aRichting + ".\n");
			mGebruikersInterface.toonLocatie(mSpelwereld.getCurrentLocatie());
		}
		else
		{
			Logger::getInstance().logOutput("Er is geen uitgang naar de richting " + aRichting + ".\n");
		}
	}
	else
	{
		Logger::getInstance().logOutput("Je bent niet in een valide locatie.\n");
	}
}

void SpelerActieHandler::pak(const std::string& aObjectnaam)
{
	if (mSpelwereld.getCurrentLocatie())
	{
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
						Logger::getInstance().logOutput("Je hebt een item opgepakt van " +
														std::string(vijand->getNaam()) + ": " + aObjectnaam + ".\n");
						return;
					}
				}
			}
		}

		// Check if the object is in the current location
		for (int i = 0; i < mSpelwereld.getCurrentLocatie()->getZichtbareObjectenCount(); ++i)
		{
			Spelobject* obj = mSpelwereld.getCurrentLocatie()->getZichtbaarObject(i);
			if (obj && obj->getNaam() == aObjectnaam)
			{
				mSpeler.voegObjectToe(obj);
				mSpelwereld.getCurrentLocatie()->verwijderZichtbaarObject(obj);
				Logger::getInstance().logOutput("Je hebt een item opgepakt: " + aObjectnaam + ".\n");
				return;
			}
		}

		Logger::getInstance().logOutput("Object " + aObjectnaam + " is niet gevonden in je huidige locatie.\n");
	}
	else
	{
		Logger::getInstance().logOutput("Je bent niet in een valide locatie.\n");
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
				Logger::getInstance().logOutput("Je hebt een item neergelegd: " + aObjectnaam + ".\n");
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
				Logger::getInstance().logOutput("Je hebt een item neergelegd: " + aObjectnaam + ".\n");
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
				Logger::getInstance().logOutput("Je hebt een item neergelegd: " + aObjectnaam + ".\n");
				return;
			}
		}
		Logger::getInstance().logOutput("Object " + aObjectnaam + " is niet gevonden in je inventaris.\n");
	}
	else
	{
		Logger::getInstance().logOutput("Je bent niet in een valide locatie.\n");
	}
}

void SpelerActieHandler::bekijkObject(const std::string& aObjectnaam)
{
	if (mSpelwereld.getCurrentLocatie())
	{
		Spelobject* obj = mSpelwereld.getCurrentLocatie()->getZichtbaarObjectByName(aObjectnaam);
		if (obj)
		{
			Logger::getInstance().logOutput("Object: " + std::string(obj->getNaam()) + "\n");
			Logger::getInstance().logOutput("Beschrijving: " + std::string(obj->getBeschrijving()) + "\n");
		}
		else
		{
			Logger::getInstance().logOutput("Object " + aObjectnaam + " is niet gevonden in je huidige locatie.\n");
		}
	}
	else
	{
		Logger::getInstance().logOutput("Je bent niet in een valide locatie.\n");
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
					Logger::getInstance().logOutput("Vijand heeft geen objecten.\n");
				}
				else
				{
					Logger::getInstance().logOutput("Objecten van " + std::string(vijand->getNaam()) + ":\n");
					for (int i = 0; i < vijand->getAantalSpelobjecten(); i++)
					{
						Logger::getInstance().logOutput(
							"  - " + std::string(vijand->getSpelobject(i)->getNaam()) + "\n");
					}
					// for (int i = 0; i < vijand->getAantalSpelobjecten(); i++)
					// {
					// 	mSpelwereld.getCurrentLocatie()->voegZichtbaarObjectToe(vijand->getSpelobject(i));
					// 	vijand->removeSpelobject(vijand->getSpelobject(i));
					// }
				}
			}
		}
		else
		{
			Logger::getInstance().logOutput("Vijand " + aVijandnaam + " is niet gevonden in je huidige locatie.\n");
		}
	}
	else
	{
		Logger::getInstance().logOutput("Je bent niet in een valide locatie.\n");
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
				Logger::getInstance().logOutput(std::string(vijand->getNaam()) + " is verslagen!\n");
			}
		}
		else
		{
			Logger::getInstance().logOutput("Vijand " + aVijandnaam + " is niet gevonden in je huidige locatie.\n");
		}
	}
	else
	{
		Logger::getInstance().logOutput("Je bent niet in een valide locatie.\n");
	}
}

void SpelerActieHandler::draagWapenrusting(const std::string& aWapenrustingnaam)
{
    auto it = std::find_if(mSpeler.getWapenrustingInventaris().begin(), mSpeler.getWapenrustingInventaris().end(),
                           [&aWapenrustingnaam](const std::unique_ptr<WapenrustingObject>& obj)
                           { return strcmp(obj->getNaam(), aWapenrustingnaam.c_str()) == 0; });

    if (it != mSpeler.getWapenrustingInventaris().end())
    {
        if (mSpeler.getHuidigWapenrusting())
        {
			mSpeler.voegObjectToe(mSpeler.getHuidigWapenrusting().release());
        }

        mSpeler.draagWapenrusting(std::move(*it));
        mSpeler.getWapenrustingInventaris().erase(it);

        Logger::getInstance().logOutput("Je draagt nu " + aWapenrustingnaam + ".\n");
    }
    else
    {
        Logger::getInstance().logOutput("Wapenrusting " + aWapenrustingnaam + " is niet gevonden in je inventaris.\n");
    }
}
void SpelerActieHandler::draagWapen(const std::string& aWapennaam)
{
	auto it = std::find_if(mSpeler.getWapenInventaris().begin(), mSpeler.getWapenInventaris().end(),
						   [&aWapennaam](const std::unique_ptr<WapenObject>& obj)
						   { return strcmp(obj->getNaam(), aWapennaam.c_str()) == 0; });

	if (it != mSpeler.getWapenInventaris().end())
	{
		auto huidigWapen = std::move(mSpeler.getHuidigWapen());
		mSpeler.draagWapen(std::move(*it));
		mSpeler.getWapenInventaris().erase(it);
		mSpeler.voegObjectToe(huidigWapen.release());
		Logger::getInstance().logOutput("Je draagt nu " + aWapennaam + ".\n");
	}
	else
	{
		Logger::getInstance().logOutput("Wapen " + aWapennaam + " is niet gevonden in je inventaris.\n");
	}
}

void SpelerActieHandler::wacht() { Logger::getInstance().logOutput("Je hebt gewacht.\n"); }

void SpelerActieHandler::consumeer(const std::string& aObjectnaam)
{
	for (auto it = mSpeler.getConsumeerbareObjecten().begin(); it != mSpeler.getConsumeerbareObjecten().end(); ++it)
	{
		if (std::string((*it)->getNaam()).find(aObjectnaam) != std::string::npos)
		{
			if (aObjectnaam.find("levenselixer") != std::string::npos)
			{
				mSpeler.voegLevenspuntenToe(std::move(*it));
				Logger::getInstance().logOutput("Je hebt " + aObjectnaam + " geconsumeerd.\n");
			}
			else if (aObjectnaam.find("ervaringsdrank") != std::string::npos)
			{
				int huidigeAanvalskans = mSpeler.getAanvalskans();
				int nieuweAanvalskans = std::min(huidigeAanvalskans + 10, 90);
				mSpeler.setAanvalskans(nieuweAanvalskans);
				Logger::getInstance().logOutput("Je hebt " + aObjectnaam + " geconsumeerd.\n");
				Logger::getInstance().logOutput(
					"Je aanvalskans is verhoogd naar " + std::to_string(nieuweAanvalskans) + "%.\n");
			}
			else if (aObjectnaam.find("teleportatiedrank") != std::string::npos)
			{
				int nieuweLocatieIndex = RandomEngine::getRandomInt(0, mSpelwereld.getLocatiesCount() - 1);
				Locatie* nieuweLocatie = mSpelwereld.getLocatieByIndex(nieuweLocatieIndex);
				mSpelwereld.setCurrentLocatie(nieuweLocatie);
				Logger::getInstance().logOutput("Je hebt " + aObjectnaam + " geconsumeerd.\n");
				Logger::getInstance().logOutput("Je bent geteleporteerd naar een nieuwe locatie.\n");
				mGebruikersInterface.toonLocatie(mSpelwereld.getCurrentLocatie());
			}
			mSpeler.getConsumeerbareObjecten().erase(it);
			return;
		}
	}
	Logger::getInstance().logOutput("Object " + aObjectnaam + " is niet gevonden in je inventaris.\n");
}

void SpelerActieHandler::godMode()
{
	if (mSpeler.getGodMode())
	{
		mSpeler.setGodMode(false);
		Logger::getInstance().logOutput("God mode is uitgeschakeld.\n");
		return;
	}
	else
	{
		mSpeler.setGodMode(true);
		Logger::getInstance().logOutput("God mode is ingeschakeld.\n");
	}
}
