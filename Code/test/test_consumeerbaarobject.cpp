#include <gtest/gtest.h>
#include "SpelobjectFactory.h"
#include "ConsumeerbaarObject.h"

class ConsumeerbaarObjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        SpelobjectFactory spelobjectFactory;
        consumeerbaar = dynamic_cast<ConsumeerbaarObject*>(spelobjectFactory.CreateSpelobject("Levenselixer", "Een genezende drank", "levenselixer", 20, 0, 0));
    }

    void TearDown() override {
        delete consumeerbaar;
    }

    ConsumeerbaarObject* consumeerbaar;
};

TEST_F(ConsumeerbaarObjectTest, Constructor) {
    ASSERT_NE(consumeerbaar, nullptr);
    EXPECT_STREQ(consumeerbaar->getNaam(), "Levenselixer");
    EXPECT_STREQ(consumeerbaar->getBeschrijving(), "Een genezende drank");
}

TEST_F(ConsumeerbaarObjectTest, GetEffect) {
    EXPECT_EQ(consumeerbaar->getEffect(), 20);
}