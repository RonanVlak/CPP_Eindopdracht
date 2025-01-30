#include <gtest/gtest.h>
#include "CustomVector.h"

class CustomVectorTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(CustomVectorTest, DefaultConstructor) {
    CustomVector<int> vec;
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.getCapacity(), 0);
    EXPECT_TRUE(vec.isEmpty());
}

TEST_F(CustomVectorTest, PushBack) {
    CustomVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}

TEST_F(CustomVectorTest, PopBack) {
    CustomVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.pop_back();
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec[0], 1);
    EXPECT_NO_THROW(vec.pop_back());
    EXPECT_THROW(vec.pop_back(), std::out_of_range);
}

TEST_F(CustomVectorTest, Erase) {
    CustomVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.erase(1);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 3);
    EXPECT_THROW(vec.erase(2), std::out_of_range);
}

TEST_F(CustomVectorTest, CopyConstructor) {
    CustomVector<int> vec1;
    vec1.push_back(1);
    vec1.push_back(2);
    CustomVector<int> vec2(vec1);
    EXPECT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2[0], 1);
    EXPECT_EQ(vec2[1], 2);
}

TEST_F(CustomVectorTest, CopyAssignment) {
    CustomVector<int> vec1;
    vec1.push_back(1);
    vec1.push_back(2);
    CustomVector<int> vec2;
    vec2 = vec1;
    EXPECT_EQ(vec2.size(), 2);
    EXPECT_EQ(vec2[0], 1);
    EXPECT_EQ(vec2[1], 2);
}

TEST_F(CustomVectorTest, Resize) {
    CustomVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.resize(10);
    EXPECT_EQ(vec.getCapacity(), 10);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
}

TEST_F(CustomVectorTest, Clear) {
    CustomVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.clear();
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.getCapacity(), 0);
    EXPECT_TRUE(vec.isEmpty());
}

TEST_F(CustomVectorTest, BeginEnd) {
    CustomVector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    auto begin = vec.begin();
    auto end = vec.end();
    EXPECT_EQ(end - begin, 2);
    EXPECT_EQ(*begin, 1);
    EXPECT_EQ(*(begin + 1), 2);
}