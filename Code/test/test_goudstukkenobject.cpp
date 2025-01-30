#include <gtest/gtest.h>
#include "SpelobjectFactory.h"
#include "GoudstukkenObject.h"

class GoudstukkenObjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        SpelobjectFactory spelobjectFactory;
        goudstukken = dynamic_cast<GoudstukkenObject*>(spelobjectFactory.CreateSpelobject("Goudstukken", "Een zak met goudstukken", "goudstukken", 100, 0, 0));
    }

    void TearDown() override {
        delete goudstukken;
    }

    GoudstukkenObject* goudstukken;
};

TEST_F(GoudstukkenObjectTest, Constructor) {
    ASSERT_NE(goudstukken, nullptr);
    EXPECT_STREQ(goudstukken->getNaam(), "Goudstukken");
    EXPECT_STREQ(goudstukken->getBeschrijving(), "Een zak met goudstukken");
}

TEST_F(GoudstukkenObjectTest, GetAantalGoudstukken) {
    EXPECT_EQ(goudstukken->getAantalGoudstukken(), 100);
}