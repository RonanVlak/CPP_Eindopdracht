#include <gtest/gtest.h>
#include "SpelobjectFactory.h"
#include "WapenObject.h"

class WapenObjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        SpelobjectFactory spelobjectFactory;
        wapen = dynamic_cast<WapenObject*>(spelobjectFactory.CreateSpelobject("Zwaard", "Een scherp zwaard", "wapen", 10, 20, 0));
    }

    void TearDown() override {
        delete wapen;
    }

    WapenObject* wapen;
};

TEST_F(WapenObjectTest, Constructor) {
    ASSERT_NE(wapen, nullptr);
    EXPECT_STREQ(wapen->getNaam(), "Zwaard");
    EXPECT_STREQ(wapen->getBeschrijving(), "Een scherp zwaard");
}

TEST_F(WapenObjectTest, GetMinimumSchade) {
    EXPECT_EQ(wapen->getMinimumSchade(), 10);
}

TEST_F(WapenObjectTest, GetMaximumSchade) {
    EXPECT_EQ(wapen->getMaximumSchade(), 20);
}