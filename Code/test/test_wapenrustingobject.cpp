#include <gtest/gtest.h>
#include "SpelobjectFactory.h"
#include "WapenrustingObject.h"

class WapenrustingObjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        SpelobjectFactory spelobjectFactory;
        wapenrusting = dynamic_cast<WapenrustingObject*>(spelobjectFactory.CreateSpelobject("Harnas", "Een sterk harnas", "wapenrusting", 0, 0, 5));
    }

    void TearDown() override {
        delete wapenrusting;
    }

    WapenrustingObject* wapenrusting;
};

TEST_F(WapenrustingObjectTest, Constructor) {
    ASSERT_NE(wapenrusting, nullptr);
    EXPECT_STREQ(wapenrusting->getNaam(), "Harnas");
    EXPECT_STREQ(wapenrusting->getBeschrijving(), "Een sterk harnas");
}

TEST_F(WapenrustingObjectTest, GetBescherming) {
    EXPECT_EQ(wapenrusting->getBescherming(), 5);
}