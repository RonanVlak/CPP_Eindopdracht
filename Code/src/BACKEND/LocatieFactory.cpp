#include "LocatieFactory.h"
#include <cstring>
#include <cstdlib>

Locatie* LocatieFactory::CreateLocatie(int id, const char* naam, const char* algemeneOmschrijving, const char* gedetailleerdeOmschrijving,
                                       const char* noord, const char* oost, const char* zuid, const char* west,
                                       const char* vijanden, const char* objectenVerborgen, const char* objectenZichtbaar, const char* databaseBestand) {
    Locatie* locatie = new Locatie(id, naam, algemeneOmschrijving, gedetailleerdeOmschrijving);

    if (noord && strlen(noord) > 0) {
        locatie->voegUitgangToe("noord", atoi(noord));
    }
    if (oost && strlen(oost) > 0) {
        locatie->voegUitgangToe("oost", atoi(oost));
    }
    if (zuid && strlen(zuid) > 0) {
        locatie->voegUitgangToe("zuid", atoi(zuid));
    }
    if (west && strlen(west) > 0) {
        locatie->voegUitgangToe("west", atoi(west));
    }

    DatabaseLoader dbLoader;
    int count = 0;

    // Add enemies
    if (vijanden && strlen(vijanden) > 0) {
        char vijandNaam[256];
        const char* vijandStart = vijanden;
        CustomVector<Vijand*> vijandenVector = dbLoader.laadVijanden(databaseBestand, count);
        int vijandId = 1;

        while (sscanf(vijandStart, "%255[^;];", vijandNaam) == 1) {
            for (int i = 0; i < vijandenVector.size(); ++i) {
                if (vijandenVector[i] != nullptr && strcmp(vijandenVector[i]->getNaam(), vijandNaam) == 0) {
                    char newVijandNaam[256];
                    snprintf(newVijandNaam, sizeof(newVijandNaam) - 1, "%.245s%d", vijandNaam, vijandId++);
                    newVijandNaam[sizeof(newVijandNaam) - 1] = '\0';   
                    vijandenVector[i]->setNaam(newVijandNaam);
                    locatie->voegVijandToe(vijandenVector[i]);
                    vijandenVector[i] = nullptr;  // Mark as moved
                    break;
                }
            }
            vijandStart = strchr(vijandStart, ';');
            if (!vijandStart) break;
            ++vijandStart;
        }
        for (auto& vijand : vijandenVector) {
            delete vijand;
        }
        vijandenVector.clear();
    }

    // Add hidden objects
    if (objectenVerborgen && strlen(objectenVerborgen) > 0) {
        char objectNaam[256];
        const char* objectStart = objectenVerborgen;
        CustomVector<Spelobject*> verborgenObjectenVector = dbLoader.laadSpelobjecten(databaseBestand, count);
        int objectId = 1;

        while (sscanf(objectStart, "%255[^;];", objectNaam) == 1) {
            for (int i = 0; i < verborgenObjectenVector.size(); ++i) {
                if (verborgenObjectenVector[i] != nullptr && strcmp(verborgenObjectenVector[i]->getNaam(), objectNaam) == 0) {
                    char newObjectNaam[256];
                    snprintf(newObjectNaam, sizeof(newObjectNaam) -1, "%.245s%d", objectNaam, objectId++);
                    newObjectNaam[sizeof(newObjectNaam) - 1] = '\0'; // Ensure null-termination
                    verborgenObjectenVector[i]->setNaam(newObjectNaam);
                    locatie->voegVerborgenObjectToe(verborgenObjectenVector[i]);
                    verborgenObjectenVector[i] = nullptr;  // Mark as moved
                    break;
                }
            }
            objectStart = strchr(objectStart, ';');
            if (!objectStart) break;
            ++objectStart;
        }
        for (auto& object : verborgenObjectenVector) {
            delete object;
        }
        verborgenObjectenVector.clear();
    }

    // Add visible objects
    if (objectenZichtbaar && strlen(objectenZichtbaar) > 0) {
        char objectNaam[256];
        const char* objectStart = objectenZichtbaar;
        CustomVector<Spelobject*> zichtbareObjectenVector = dbLoader.laadSpelobjecten(databaseBestand, count);
        int objectId = 1;

        while (sscanf(objectStart, "%255[^;];", objectNaam) == 1) {
            for (int i = 0; i < zichtbareObjectenVector.size(); ++i) {
                if (zichtbareObjectenVector[i] != nullptr && strcmp(zichtbareObjectenVector[i]->getNaam(), objectNaam) == 0) {
                    char newObjectNaam[256];
                    snprintf(newObjectNaam, sizeof(newObjectNaam) -1, "%.245s%d", objectNaam, objectId++);
                    newObjectNaam[sizeof(newObjectNaam) - 1] = '\0'; // Ensure null-termination
                    zichtbareObjectenVector[i]->setNaam(newObjectNaam);
                    locatie->voegZichtbaarObjectToe(zichtbareObjectenVector[i]);
                    zichtbareObjectenVector[i] = nullptr;  // Mark as moved
                    break;
                }
            }
            objectStart = strchr(objectStart, ';');
            if (!objectStart) break;
            ++objectStart;
        }
        for (auto& object : zichtbareObjectenVector) {
            delete object;
        }
        zichtbareObjectenVector.clear();
    }

    return locatie;
}