#include <gtest/gtest.h>
#include "LocatieFactory.h"
#include "Locatie.h"
#include "Vijand.h"
#include "Spelobject.h"
#include "FSConverter.h"

class LocatieFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        FSConverter converter;
        dbPath = converter.getResourcePath("kerkersendraken.db");
    }

    std::string dbPath;
};

TEST_F(LocatieFactoryTest, CreateLocatieWithExits) {
    LocatieFactory locatieFactory;
    Locatie* locatie = locatieFactory.CreateLocatie(1, "Start", "Algemene Omschrijving", "Gedetailleerde Omschrijving", "2", "3", "4", "5", "", "", "", dbPath.c_str());

    EXPECT_EQ(locatie->getNoord(), 2);
    EXPECT_EQ(locatie->getOost(), 3);
    EXPECT_EQ(locatie->getZuid(), 4);
    EXPECT_EQ(locatie->getWest(), 5);

    delete locatie;
}

TEST_F(LocatieFactoryTest, CreateLocatieWithEnemies) {
    LocatieFactory locatieFactory;
    Locatie* locatie = locatieFactory.CreateLocatie(1, "Start", "Algemene Omschrijving", "Gedetailleerde Omschrijving", "", "", "", "", "Goblin;", "", "", dbPath.c_str());

    EXPECT_EQ(locatie->getVijandenCount(), 1);
    EXPECT_STREQ(locatie->getVijand(0)->getNaam(), "Goblin1");

    delete locatie;
}

TEST_F(LocatieFactoryTest, CreateLocatieWithHiddenObjects) {
    LocatieFactory locatieFactory;
    Locatie* locatie = locatieFactory.CreateLocatie(1, "Start", "Algemene Omschrijving", "Gedetailleerde Omschrijving", "", "", "", "", "", "speer;", "", dbPath.c_str());

    EXPECT_EQ(locatie->getVerborgenObjectenCount(), 1);
    EXPECT_STREQ(locatie->getVerborgenObject(0)->getNaam(), "speer1");

    delete locatie;
}

TEST_F(LocatieFactoryTest, CreateLocatieWithVisibleObjects) {
    LocatieFactory locatieFactory;
    Locatie* locatie = locatieFactory.CreateLocatie(1, "Start", "Algemene Omschrijving", "Gedetailleerde Omschrijving", "", "", "", "", "", "", "speer;", dbPath.c_str());

    EXPECT_EQ(locatie->getZichtbareObjectenCount(), 1);
    EXPECT_STREQ(locatie->getZichtbaarObject(0)->getNaam(), "speer1");

    delete locatie;
}

TEST_F(LocatieFactoryTest, CreateLocatieWithAllAttributes) {
    LocatieFactory locatieFactory;
    Locatie* locatie = locatieFactory.CreateLocatie(1, "Start", "Algemene Omschrijving", "Gedetailleerde Omschrijving", "2", "3", "4", "5", "Goblin;", "speer;", "langzwaard;", dbPath.c_str());

    EXPECT_EQ(locatie->getNoord(), 2);
    EXPECT_EQ(locatie->getOost(), 3);
    EXPECT_EQ(locatie->getZuid(), 4);
    EXPECT_EQ(locatie->getWest(), 5);

    EXPECT_EQ(locatie->getVijandenCount(), 1);
    EXPECT_STREQ(locatie->getVijand(0)->getNaam(), "Goblin1");

    EXPECT_EQ(locatie->getVerborgenObjectenCount(), 1);
    EXPECT_STREQ(locatie->getVerborgenObject(0)->getNaam(), "speer1");

    EXPECT_EQ(locatie->getZichtbareObjectenCount(), 1);
    EXPECT_STREQ(locatie->getZichtbaarObject(0)->getNaam(), "langzwaard1");

    delete locatie;
}