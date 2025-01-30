#include <gtest/gtest.h>
#include "Vijand.h"
#include "VijandFactory.h"
#include "Spelobject.h"
#include "SpelobjectFactory.h"
#include "WapenObject.h"
#include "RandomEngine.h"

class VijandTest : public ::testing::Test {
protected:
    void SetUp() override {
        VijandFactory vijandFactory;
        vijand = vijandFactory.CreateVijand("Goblin", "Een gevaarlijke goblin", 0, 0, 10, 50, 5, 10);
    }

    void TearDown() override {
        delete vijand;
    }

    Vijand* vijand;
};

TEST_F(VijandTest, Constructor) {
    EXPECT_STREQ(vijand->getNaam(), "Goblin");
    EXPECT_STREQ(vijand->getBeschrijving(), "Een gevaarlijke goblin");
    EXPECT_EQ(vijand->getLevenspunten(), 10);
    EXPECT_EQ(vijand->getAanvalskans(), 50);
    EXPECT_EQ(vijand->getAanvalsschadeMin(), 5);
    EXPECT_EQ(vijand->getAanvalsschadeMax(), 10);
}

TEST_F(VijandTest, OntvangSchade) {
    vijand->ontvangSchade(5);
    EXPECT_EQ(vijand->getLevenspunten(), 5);

    vijand->ontvangSchade(10);
    EXPECT_EQ(vijand->getLevenspunten(), 0);
    EXPECT_TRUE(vijand->isVerslagen());
    EXPECT_STREQ(vijand->getNaam(), "Goblin [Verslagen]");
}

TEST_F(VijandTest, VoegSpelobjectToe) {
    SpelobjectFactory spelobjectFactory;
    Spelobject* object = spelobjectFactory.CreateSpelobject("Zwaard", "Een scherp zwaard", "wapen", 10, 20, 0);
    vijand->voegSpelobjectToe(object);
    EXPECT_EQ(vijand->getAantalSpelobjecten(), 1);
    EXPECT_EQ(vijand->getSpelobject(0), object);
}

TEST_F(VijandTest, RemoveSpelobject) {
    SpelobjectFactory spelobjectFactory;
    Spelobject* object = spelobjectFactory.CreateSpelobject("Zwaard", "Een scherp zwaard", "wapen", 10, 20, 0);
    vijand->voegSpelobjectToe(object);
    vijand->removeSpelobject(object);
    EXPECT_EQ(vijand->getAantalSpelobjecten(), 0);
}

TEST_F(VijandTest, Attack) {
    RandomEngine::getInstance().seed(42); // Set a fixed seed for randomness
    int damage = vijand->attack();
    EXPECT_GE(damage, 5);
    EXPECT_LE(damage, 10);
}

TEST_F(VijandTest, SetNaam) {
    vijand->setNaam("Orc");
    EXPECT_STREQ(vijand->getNaam(), "Orc");
}

TEST_F(VijandTest, CopyConstructor) {
    Vijand copy(*vijand);
    EXPECT_STREQ(copy.getNaam(), "Goblin");
    EXPECT_STREQ(copy.getBeschrijving(), "Een gevaarlijke goblin");
    EXPECT_EQ(copy.getLevenspunten(), 10);
}

TEST_F(VijandTest, MoveConstructor) {
    Vijand moved(std::move(*vijand));
    EXPECT_STREQ(moved.getNaam(), "Goblin");
    EXPECT_STREQ(moved.getBeschrijving(), "Een gevaarlijke goblin");
    EXPECT_EQ(moved.getLevenspunten(), 10);
    EXPECT_EQ(vijand->getNaam(), nullptr);
}

TEST_F(VijandTest, CopyAssignment) {
    VijandFactory vijandFactory;
    Vijand copy = *vijandFactory.CreateVijand("Orc", "Een gevaarlijke orc", 0, 0, 20, 60, 10, 15);
    copy = *vijand;
    EXPECT_STREQ(copy.getNaam(), "Goblin");
    EXPECT_STREQ(copy.getBeschrijving(), "Een gevaarlijke goblin");
    EXPECT_EQ(copy.getLevenspunten(), 10);
}

TEST_F(VijandTest, MoveAssignment) {
    VijandFactory vijandFactory;
    Vijand moved = *vijandFactory.CreateVijand("Orc", "Een gevaarlijke orc", 0, 0, 20, 60, 10, 15);
    moved = std::move(*vijand);
    EXPECT_STREQ(moved.getNaam(), "Goblin");
    EXPECT_STREQ(moved.getBeschrijving(), "Een gevaarlijke goblin");
    EXPECT_EQ(moved.getLevenspunten(), 10);
    EXPECT_EQ(vijand->getNaam(), nullptr);
}