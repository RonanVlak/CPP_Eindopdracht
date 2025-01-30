#include <gtest/gtest.h>
#include "VijandFactory.h"
#include "Vijand.h"

class VijandFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

    VijandFactory vijandFactory;
};

TEST_F(VijandFactoryTest, CreateVijand) {
    Vijand* vijand = vijandFactory.CreateVijand("Goblin", "Een gevaarlijke goblin", 0, 0, 10, 50, 5, 10);
    ASSERT_NE(vijand, nullptr);
    EXPECT_STREQ(vijand->getNaam(), "Goblin");
    EXPECT_STREQ(vijand->getBeschrijving(), "Een gevaarlijke goblin");
    EXPECT_EQ(vijand->getLevenspunten(), 10);
    EXPECT_EQ(vijand->getAanvalskans(), 50);
    EXPECT_EQ(vijand->getAanvalsschadeMin(), 5);
    EXPECT_EQ(vijand->getAanvalsschadeMax(), 10);
    delete vijand;
}

TEST_F(VijandFactoryTest, CreateVijandWithDifferentAttributes) {
    Vijand* vijand = vijandFactory.CreateVijand("Orc", "Een gevaarlijke orc", 1, 2, 20, 60, 10, 15);
    ASSERT_NE(vijand, nullptr);
    EXPECT_STREQ(vijand->getNaam(), "Orc");
    EXPECT_STREQ(vijand->getBeschrijving(), "Een gevaarlijke orc");
    EXPECT_EQ(vijand->getLevenspunten(), 20);
    EXPECT_EQ(vijand->getAanvalskans(), 60);
    EXPECT_EQ(vijand->getAanvalsschadeMin(), 10);
    EXPECT_EQ(vijand->getAanvalsschadeMax(), 15);
    delete vijand;
}