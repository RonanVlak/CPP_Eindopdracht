#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "Spelwereld.h"
#include "tinyxml2.h"

class XMLParser
{
public:
	XMLParser();
	bool parseFile(const char* filename, Spelwereld& spelwereld, const char* databaseBestand);
	void updateExits(Spelwereld& spelwereld);

private:
	void parseLocatie(tinyxml2::XMLElement* locatieElement, Spelwereld& spelwereld, const char* databaseBestand);
};

#endif // XMLPARSER_H