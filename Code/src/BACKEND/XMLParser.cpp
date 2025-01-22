#include "XMLParser.h"
#include "LocatieFactory.h"
#include <iostream>
#include <stdexcept>

XMLParser::XMLParser() {}

XMLParser::~XMLParser() {}

bool XMLParser::parseFile(const char* filename, Spelwereld& spelwereld, const char* databaseBestand) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename) != tinyxml2::XML_SUCCESS) {
        throw std::runtime_error("Could not open file");
        return false;
    }

    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root) {
        throw std::runtime_error("Invalid XML format");
        return false;
    }

    for (tinyxml2::XMLElement* locatieElement = root->FirstChildElement("locatie"); locatieElement; locatieElement = locatieElement->NextSiblingElement("locatie")) {
        parseLocatie(locatieElement, spelwereld, databaseBestand);
    }

    // Update exits after all locations have been parsed
    updateExits(spelwereld);

    return true;
}

void XMLParser::parseLocatie(tinyxml2::XMLElement* locatieElement, Spelwereld& spelwereld, const char* databaseBestand) {
    int id = 0;
    const char* naam = nullptr;
    const char* beschrijving = nullptr;
    const char* gedetailleerdeBeschrijving = nullptr;
    const char* noord = nullptr;
    const char* oost = nullptr;
    const char* zuid = nullptr;
    const char* west = nullptr;
    const char* vijanden = nullptr;
    const char* objectenVerborgen = nullptr;
    const char* objectenZichtbaar = nullptr;

    locatieElement->QueryIntAttribute("id", &id);
    noord = locatieElement->Attribute("noord");
    oost = locatieElement->Attribute("oost");
    zuid = locatieElement->Attribute("zuid");
    west = locatieElement->Attribute("west");
    vijanden = locatieElement->Attribute("vijand");
    objectenVerborgen = locatieElement->Attribute("objectenverborgen");
    objectenZichtbaar = locatieElement->Attribute("objectenzichtbaar");
    naam = locatieElement->GetText();

    tinyxml2::XMLElement* beschrijvingElement = locatieElement->FirstChildElement("beschrijving");
    if (beschrijvingElement) {
        beschrijving = beschrijvingElement->GetText();
    }

    std::cout << "Parsed location: " << (naam ? naam : "unknown") << " with ID: " << id << std::endl;

    LocatieFactory locatieFactory;
    Locatie* locatie = locatieFactory.CreateLocatie(id, naam ? naam : "", beschrijving ? beschrijving : "", gedetailleerdeBeschrijving ? gedetailleerdeBeschrijving : "", noord ? noord : "", oost ? oost : "", zuid ? zuid : "", west ? west : "", vijanden ? vijanden : "", objectenVerborgen ? objectenVerborgen : "", objectenZichtbaar ? objectenZichtbaar : "", databaseBestand);
    spelwereld.voegLocatieToe(locatie);
    locatie = nullptr;
    delete locatie;
}

void XMLParser::updateExits(Spelwereld& spelwereld) {
    for (int i = 0; i < spelwereld.getLocatiesCount(); ++i) {
        Locatie* locatie = spelwereld.getLocatieByIndex(i);
        if (locatie->getNoord() != -1) {
            locatie->setUitgang("noord", spelwereld.getLocatieById(locatie->getNoord()));
        }
        if (locatie->getOost() != -1) {
            locatie->setUitgang("oost", spelwereld.getLocatieById(locatie->getOost()));
        }
        if (locatie->getZuid() != -1) {
            locatie->setUitgang("zuid", spelwereld.getLocatieById(locatie->getZuid()));
        }
        if (locatie->getWest() != -1) {
            locatie->setUitgang("west", spelwereld.getLocatieById(locatie->getWest()));
        }
    }
}