#include "FSConverter.h"
#include "LocatieFactory.h"
#include "Spelwereld.h"
#include "XMLParser.h"
#include "tinyxml2.h"
#include <gtest/gtest.h>

class XMLParserTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		databaseBestand = fsconverter.getResourcePath("kerkersendraken.db");
		xmlBestand = fsconverter.getResourcePath("kasteelruine.xml");
	}

	void TearDown() override
	{
		std::remove("test_invalid.xml");
		std::remove("test_missing_root.xml");
		std::remove("test_empty.xml");
	}

	XMLParser xmlParser;
	Spelwereld spelwereld;
	FSConverter fsconverter;
	std::string databaseBestand;
	std::string xmlBestand;
};

TEST_F(XMLParserTest, ParseFileSuccess)
{
	EXPECT_NO_THROW(xmlParser.parseFile(xmlBestand.c_str(), spelwereld, databaseBestand.c_str()));
	EXPECT_EQ(spelwereld.getLocatiesCount(), 14);

	Locatie* locatie = spelwereld.getLocatieById(1);
	ASSERT_NE(locatie, nullptr);
	EXPECT_STREQ(locatie->getNaam(), "Kasteelpoort");
	EXPECT_STREQ(locatie->getAlgemeneOmschrijving(),
				 "Je staat voor de poort van de ruine van een kasteel. De poorten staan open en het valhek is omhoog.");
	EXPECT_EQ(locatie->getNoord(), 2);
}

TEST_F(XMLParserTest, ParseFileInvalidXML)
{
	const char* xmlContent = R"(
        <spelwereld>
            <locatie id="1" noord="2" oost="3" zuid="4" west="5" vijand="Goblin;" objectenverborgen="Schat;" objectenzichtbaar="Sleutel;">
                <naam>Start</naam>
                <beschrijving>Algemene Omschrijving</beschrijving>
            <!-- Missing closing tag for spelwereld
    )";

	tinyxml2::XMLDocument doc;
	doc.Parse(xmlContent);
	doc.SaveFile("test_invalid.xml");

	EXPECT_THROW(xmlParser.parseFile("test_invalid.xml", spelwereld, databaseBestand.c_str()), std::runtime_error);
}

TEST_F(XMLParserTest, ParseFileEmpty)
{
	const char* xmlContent = R"(
        <spelwereld>
        </spelwereld>
    )";

	tinyxml2::XMLDocument doc;
	doc.Parse(xmlContent);
	doc.SaveFile("test_empty.xml");

	EXPECT_NO_THROW(xmlParser.parseFile("test_empty.xml", spelwereld, databaseBestand.c_str()));
	EXPECT_EQ(spelwereld.getLocatiesCount(), 0);
}

TEST_F(XMLParserTest, UpdateExits)
{
	LocatieFactory locatieFactory;
	Locatie* locatie1 = locatieFactory.CreateLocatie(1, "Start", "Algemene Omschrijving", "Gedetailleerde Omschrijving",
													 "2", "", "", "", "", "", "", databaseBestand.c_str());
	Locatie* locatie2 = locatieFactory.CreateLocatie(2, "Noord", "Noordelijke locatie", "Gedetailleerde Omschrijving",
													 "", "", "", "", "", "", "", databaseBestand.c_str());
	spelwereld.voegLocatieToe(locatie1);
	spelwereld.voegLocatieToe(locatie2);

	xmlParser.updateExits(spelwereld);

	EXPECT_EQ(locatie1->getNoord(), 2);
	EXPECT_EQ(locatie1->getUitgang("noord"), locatie2->getId());
}