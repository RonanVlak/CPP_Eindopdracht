#include <gtest/gtest.h>
#include "Locatie.h"
#include "Vijand.h"
#include "Spelobject.h"
#include "LocatieFactory.h"
#include "VijandFactory.h"
#include "SpelobjectFactory.h"

class LocatieTest : public ::testing::Test {
protected:
    void SetUp() override {
        LocatieFactory locatieFactory;
        locatie = locatieFactory.CreateLocatie(1, "Start", "Algemene Omschrijving", "Gedetailleerde Omschrijving", "", "", "", "", "", "", "", "kerkersendraken.db");
    }

    void TearDown() override {
        delete locatie;
    }

    Locatie* locatie;
};

TEST_F(LocatieTest, VoegVijandToe) {
    VijandFactory vijandFactory;
    Vijand* vijand = vijandFactory.CreateVijand("Goblin", "Een gevaarlijke goblin", 0, 0, 10, 50, 5, 10);
    locatie->voegVijandToe(vijand);
    EXPECT_EQ(locatie->getVijandenCount(), 1);
    EXPECT_EQ(std::string(locatie->getVijand(0)->getNaam()), "Goblin");
}

TEST_F(LocatieTest, VoegZichtbaarObjectToe) {
    SpelobjectFactory spelobjectFactory;
    Spelobject* object = spelobjectFactory.CreateSpelobject("Sleutel", "Een oude sleutel", "goudstukken", 10, 0, 0);
    locatie->voegZichtbaarObjectToe(object);
    EXPECT_EQ(locatie->getZichtbareObjectenCount(), 1);
    EXPECT_EQ(std::string(locatie->getZichtbaarObject(0)->getNaam()), "Sleutel");
}

TEST_F(LocatieTest, VoegVerborgenObjectToe) {
    SpelobjectFactory spelobjectFactory;
    Spelobject* object = spelobjectFactory.CreateSpelobject("Schat", "Een verborgen schat", "goudstukken", 100, 0, 0);
    locatie->voegVerborgenObjectToe(object);
    EXPECT_EQ(locatie->getVerborgenObjectenCount(), 1);
    EXPECT_EQ(std::string(locatie->getVerborgenObject(0)->getNaam()), "Schat");
}

TEST_F(LocatieTest, VoegUitgangToe) {
    locatie->voegUitgangToe("noord", 2);
    EXPECT_EQ(locatie->getNoord(), 2);
}

TEST_F(LocatieTest, VerwijderVijand) {
    VijandFactory vijandFactory;
    Vijand* vijand = vijandFactory.CreateVijand("Goblin", "Een gevaarlijke goblin", 0, 0, 10, 50, 5, 10);
    locatie->voegVijandToe(vijand);
    locatie->verwijderVijand(vijand);
    EXPECT_EQ(locatie->getVijandenCount(), 0);
}

TEST_F(LocatieTest, VerplaatsVerborgenNaarZichtbaar) {
    SpelobjectFactory spelobjectFactory;
    Spelobject* object = spelobjectFactory.CreateSpelobject("Schat", "Een verborgen schat", "goudstukken", 100, 0, 0);
    locatie->voegVerborgenObjectToe(object);
    locatie->verplaatsVerborgenNaarZichtbaar(object);
    EXPECT_EQ(locatie->getVerborgenObjectenCount(), 0);
    EXPECT_EQ(locatie->getZichtbareObjectenCount(), 1);
    EXPECT_EQ(std::string(locatie->getZichtbaarObject(0)->getNaam()), "Schat");
}

TEST_F(LocatieTest, VerwijderZichtbaarObject) {
    SpelobjectFactory spelobjectFactory;
    Spelobject* object = spelobjectFactory.CreateSpelobject("Sleutel", "Een oude sleutel", "goudstukken", 10, 0, 0);
    locatie->voegZichtbaarObjectToe(object);
    locatie->verwijderZichtbaarObject(object);
    EXPECT_EQ(locatie->getZichtbareObjectenCount(), 0);
}

TEST_F(LocatieTest, GetVijandByName) {
    VijandFactory vijandFactory;
    Vijand* vijand = vijandFactory.CreateVijand("Goblin", "Een gevaarlijke goblin", 0, 0, 10, 50, 5, 10);
    locatie->voegVijandToe(vijand);
    EXPECT_EQ(std::string(locatie->getVijandByName("Goblin")->getNaam()), "Goblin");
}

TEST_F(LocatieTest, GetZichtbaarObjectByName) {
    SpelobjectFactory spelobjectFactory;
    Spelobject* object = spelobjectFactory.CreateSpelobject("Sleutel", "Een oude sleutel", "goudstukken", 10, 0, 0);
    locatie->voegZichtbaarObjectToe(object);
    EXPECT_EQ(std::string(locatie->getZichtbaarObjectByName("Sleutel")->getNaam()), "Sleutel");
}

TEST_F(LocatieTest, SetUitgang) {
    LocatieFactory locatieFactory;
    Locatie* locatie2 = locatieFactory.CreateLocatie(2, "Noord", "Noordelijke locatie", "Gedetailleerde omschrijving", "", "", "", "", "", "", "", "kerkersendraken.db");
    locatie->setUitgang("noord", locatie2);
    EXPECT_EQ(locatie->getNoord(), 2);
    delete locatie2;
}
