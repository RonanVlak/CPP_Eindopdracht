#include <gtest/gtest.h>
#include "Speler.h"
#include "WapenObject.h"
#include "WapenrustingObject.h"
#include "ConsumeerbaarObject.h"

class SpelerTest : public ::testing::Test {
protected:
    void SetUp() override {
        speler = std::make_unique<Speler>("TestSpeler", 100, 50, false);
    }

    void TearDown() override {
    }

    std::unique_ptr<Speler> speler;
};

TEST_F(SpelerTest, VoegGoudstukkenToe) {
    speler->voegGoudstukkenToe(10);
    EXPECT_EQ(speler->getGoudstukken(), 10);
}

TEST_F(SpelerTest, VoegObjectToeWapen) {
    WapenObject* wapen = new WapenObject("Zwaard", "Een scherp zwaard", 10, 20);
    speler->voegObjectToe(wapen);
    EXPECT_EQ(speler->getWapenInventaris().size(), 1);
    EXPECT_EQ(std::string(speler->getWapenInventaris()[0]->getNaam()), "Zwaard");
}

TEST_F(SpelerTest, VoegObjectToeWapenrusting) {
    WapenrustingObject* wapenrusting = new WapenrustingObject("Harnas", "Een sterk harnas", 5);
    speler->voegObjectToe(wapenrusting);
    EXPECT_EQ(speler->getWapenrustingInventaris().size(), 1);
    EXPECT_EQ(std::string(speler->getWapenrustingInventaris()[0]->getNaam()), "Harnas");
}

TEST_F(SpelerTest, VoegObjectToeConsumeerbaar) {
    ConsumeerbaarObject* potion = new ConsumeerbaarObject("Potion", "Een genezende drank", 20);
    speler->voegObjectToe(potion);
    EXPECT_EQ(speler->getConsumeerbareObjecten().size(), 1);
    EXPECT_EQ(std::string(speler->getConsumeerbareObjecten()[0]->getNaam()), "Potion");
}

TEST_F(SpelerTest, ApplyDamage) {
    speler->applyDamage(20);
    EXPECT_EQ(speler->getLevenspunten(), 80);
}

TEST_F(SpelerTest, DraagWapen) {
    auto wapen = std::make_unique<WapenObject>("Zwaard", "Een scherp zwaard", 10, 20);
    speler->draagWapen(std::move(wapen));
    EXPECT_EQ(std::string(speler->getHuidigWapen()->getNaam()), "Zwaard");
}

TEST_F(SpelerTest, DraagWapenrusting) {
    auto wapenrusting = std::make_unique<WapenrustingObject>("Harnas", "Een sterk harnas", 5);
    speler->draagWapenrusting(std::move(wapenrusting));
    EXPECT_EQ(std::string(speler->getHuidigWapenrusting()->getNaam()), "Harnas");
}

TEST_F(SpelerTest, VoegLevenspuntenToe) {
    auto potion = std::make_unique<ConsumeerbaarObject>("Potion", "Een genezende drank", 20);
    speler->voegLevenspuntenToe(std::move(potion));
    EXPECT_EQ(speler->getLevenspunten(), 100);
}

TEST_F(SpelerTest, VerwijderWapen) {
    WapenObject* wapen = new WapenObject("Zwaard", "Een scherp zwaard", 10, 20);
    speler->voegObjectToe(wapen);
    speler->verwijderWapen("Zwaard");
    EXPECT_EQ(speler->getWapenInventaris().size(), 0);
}

TEST_F(SpelerTest, VerwijderWapenrusting) {
    WapenrustingObject* wapenrusting = new WapenrustingObject("Harnas", "Een sterk harnas", 5);
    speler->voegObjectToe(wapenrusting);
    speler->verwijderWapenrusting("Harnas");
    EXPECT_EQ(speler->getWapenrustingInventaris().size(), 0);
}