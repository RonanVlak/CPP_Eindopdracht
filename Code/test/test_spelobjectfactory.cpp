#include <gtest/gtest.h>
#include "SpelobjectFactory.h"
#include "GoudstukkenObject.h"
#include "WapenObject.h"
#include "WapenrustingObject.h"
#include "ConsumeerbaarObject.h"

class SpelobjectFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

    SpelobjectFactory spelobjectFactory;
};

TEST_F(SpelobjectFactoryTest, CreateGoudstukkenObject) {
    Spelobject* object = spelobjectFactory.CreateSpelobject("Goudstukken", "Een zak met goudstukken", "goudstukken", 100, 0, 0);
    ASSERT_NE(object, nullptr);
    EXPECT_STREQ(object->getNaam(), "Goudstukken");
    EXPECT_STREQ(object->getBeschrijving(), "Een zak met goudstukken");
    EXPECT_EQ(dynamic_cast<GoudstukkenObject*>(object)->getAantalGoudstukken(), 100);
    delete object;
}

TEST_F(SpelobjectFactoryTest, CreateWapenObject) {
    Spelobject* object = spelobjectFactory.CreateSpelobject("Zwaard", "Een scherp zwaard", "wapen", 10, 20, 0);
    ASSERT_NE(object, nullptr);
    EXPECT_STREQ(object->getNaam(), "Zwaard");
    EXPECT_STREQ(object->getBeschrijving(), "Een scherp zwaard");
    EXPECT_EQ(dynamic_cast<WapenObject*>(object)->getMinimumSchade(), 10);
    EXPECT_EQ(dynamic_cast<WapenObject*>(object)->getMaximumSchade(), 20);
    delete object;
}

TEST_F(SpelobjectFactoryTest, CreateWapenrustingObject) {
    Spelobject* object = spelobjectFactory.CreateSpelobject("Harnas", "Een sterk harnas", "wapenrusting", 0, 0, 5);
    ASSERT_NE(object, nullptr);
    EXPECT_STREQ(object->getNaam(), "Harnas");
    EXPECT_STREQ(object->getBeschrijving(), "Een sterk harnas");
    EXPECT_EQ(dynamic_cast<WapenrustingObject*>(object)->getBescherming(), 5);
    delete object;
}

TEST_F(SpelobjectFactoryTest, CreateConsumeerbaarObject) {
    Spelobject* object = spelobjectFactory.CreateSpelobject("Levenselixer", "Een genezende drank", "levenselixer", 20, 0, 0);
    ASSERT_NE(object, nullptr);
    EXPECT_STREQ(object->getNaam(), "Levenselixer");
    EXPECT_STREQ(object->getBeschrijving(), "Een genezende drank");
    EXPECT_EQ(dynamic_cast<ConsumeerbaarObject*>(object)->getEffect(), 20);
    delete object;
}

TEST_F(SpelobjectFactoryTest, InvalidTypeThrowsException) {
    EXPECT_THROW(spelobjectFactory.CreateSpelobject("Onbekend", "Een onbekend object", "onbekend", 0, 0, 0), std::invalid_argument);
}