#include <gtest/gtest.h>
#include "Spelwereld.h"
#include "LocatieFactory.h"
#include "VijandFactory.h"
#include "FSConverter.h"
#include "Locatie.h"
#include "Vijand.h"
#include "WapenObject.h"

class SpelwereldTest : public ::testing::Test {
protected:
    void SetUp() override {
        spelwereld = std::make_unique<Spelwereld>();
        FSConverter converter;
        dbPath = converter.getResourcePath("kerkersendraken.db");
    }

    void TearDown() override {
    }

    std::unique_ptr<Spelwereld> spelwereld;
    std::string dbPath;
};

TEST_F(SpelwereldTest, VoegLocatieToe) {
    LocatieFactory locatieFactory;
    Locatie* locatie = locatieFactory.CreateLocatie(0, "Start", "Startlocatie", "Gedetailleerde omschrijving", "", "", "", "", "", "", "", dbPath.c_str());
    spelwereld->voegLocatieToe(locatie);
    EXPECT_EQ(spelwereld->getLocatiesCount(), 1);
    EXPECT_EQ(std::string(spelwereld->getLocatieByIndex(0)->getNaam()), "Start");
}

TEST_F(SpelwereldTest, SetAndGetCurrentLocatie) {
    LocatieFactory locatieFactory;
    Locatie* locatie = locatieFactory.CreateLocatie(0, "Start", "Startlocatie", "Gedetailleerde omschrijving", "", "", "", "", "", "", "", dbPath.c_str());
    spelwereld->voegLocatieToe(locatie);
    spelwereld->setCurrentLocatie(locatie);
    EXPECT_EQ(std::string(spelwereld->getCurrentLocatie()->getNaam()), "Start");
}

TEST_F(SpelwereldTest, GetLocatieById) {
    LocatieFactory locatieFactory;
    Locatie* locatie = locatieFactory.CreateLocatie(1, "Start", "Startlocatie", "Gedetailleerde omschrijving", "", "", "", "", "", "", "", dbPath.c_str());
    spelwereld->voegLocatieToe(locatie);
    EXPECT_EQ(std::string(spelwereld->getLocatieById(1)->getNaam()), "Start");
}

TEST_F(SpelwereldTest, GetLocatieByIndex) {
    LocatieFactory locatieFactory;
    Locatie* locatie = locatieFactory.CreateLocatie(0, "Start", "Startlocatie", "Gedetailleerde omschrijving", "", "", "", "", "", "", "", dbPath.c_str());
    spelwereld->voegLocatieToe(locatie);
    EXPECT_EQ(std::string(spelwereld->getLocatieByIndex(0)->getNaam()), "Start");
}

TEST_F(SpelwereldTest, GetEnemiesDamage) {
    LocatieFactory locatieFactory;
    VijandFactory vijandFactory;
    Locatie* locatie = locatieFactory.CreateLocatie(0, "Start", "Startlocatie", "Gedetailleerde omschrijving", "", "", "", "", "", "", "", dbPath.c_str());
    Vijand* vijand = vijandFactory.CreateVijand("Goblin", "Een gevaarlijke goblin", 0, 0, 10, 100, 5, 10);
    locatie->voegVijandToe(vijand);
    spelwereld->voegLocatieToe(locatie);
    spelwereld->setCurrentLocatie(locatie);

    int damage = spelwereld->getEnemiesDamage();
    EXPECT_GE(damage, 5);
    EXPECT_LE(damage, 10);
}

TEST_F(SpelwereldTest, VerplaatsVijanden) {
    LocatieFactory locatieFactory;
    VijandFactory vijandFactory;
    Locatie* locatie1 = locatieFactory.CreateLocatie(0, "Start", "Startlocatie", "Gedetailleerde omschrijving", "1", "", "", "", "", "", "", dbPath.c_str());
    Locatie* locatie2 = locatieFactory.CreateLocatie(1, "Noord", "Noordlocatie", "Gedetailleerde omschrijving", "", "", "0", "", "", "", "", dbPath.c_str());
    Vijand* vijand = vijandFactory.CreateVijand("Goblin", "Een gevaarlijke goblin", 0, 0, 10, 50, 5, 10);
    locatie1->voegVijandToe(vijand);
    spelwereld->voegLocatieToe(locatie1);
    spelwereld->voegLocatieToe(locatie2);
    spelwereld->setCurrentLocatie(locatie1);
    spelwereld->verplaatsVijanden();
    if (locatie1->getVijandenCount() > 0) {
        EXPECT_EQ(locatie1->getVijandenCount(), 1);
        EXPECT_EQ(std::string(locatie1->getVijand(0)->getNaam()), "Goblin");
    }
    else {
        EXPECT_EQ(locatie2->getVijandenCount(), 1);
        EXPECT_EQ(std::string(locatie2->getVijand(0)->getNaam()), "Goblin");
    }
}

TEST_F(SpelwereldTest, GenerateRandomKerker) {
    spelwereld->generateRandomKerker(dbPath.c_str());
    EXPECT_GT(spelwereld->getLocatiesCount(), 0);
    EXPECT_NE(spelwereld->getCurrentLocatie(), nullptr);
}

TEST_F(SpelwereldTest, GetStartWapen) {
    WapenObject* wapen = spelwereld->getStartWapen(dbPath.c_str());
    EXPECT_NE(wapen, nullptr);
    EXPECT_EQ(std::string(wapen->getNaam()), "dolk");
}