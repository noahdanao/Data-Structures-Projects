#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "circvector.h"

using namespace std;
using namespace testing;

TEST(CircVectorCore, EmptyConstructor) {
  CircVector<int> vector;
  EXPECT_TRUE(vector.empty());
  EXPECT_THAT(vector.size(), Eq(0));
}

TEST(CircVectorCore, AtThrowsWhenInvalidIndex) {
  CircVector<int> vector;
  vector.push_back(10);

  EXPECT_THROW(vector.at(5), out_of_range);
  EXPECT_THROW(vector.at(-1), out_of_range);
}

TEST(CircVectorCore, PopBackThrowsWhenEmpty) {
  CircVector<int> vector;
  EXPECT_THROW(vector.pop_back(), runtime_error);
}

TEST(CircVectorCore, PopFrontThrowsWhenEmpty) {
  CircVector<int> vector;
  EXPECT_THROW(vector.pop_front(), runtime_error);
}

TEST(CircVectorCore, PushBackResizes) {
  CircVector<int> vector(2);
  vector.push_back(1);
  vector.push_back(2);
  vector.push_back(3);

  EXPECT_THAT(vector.size(), Eq(3));
  EXPECT_THAT(vector.at(0), Eq(1));
  EXPECT_THAT(vector.at(1), Eq(2));
  EXPECT_THAT(vector.at(2), Eq(3));
}

TEST(CircVectorCore, PushFrontResize) {
  CircVector<int> vector(2);
  vector.push_back(1);
  vector.push_back(2);
  vector.push_front(4);

  EXPECT_THAT(vector.size(), Eq(3));
  EXPECT_THAT(vector.at(0), Eq(4));
  EXPECT_THAT(vector.at(1), Eq(1));
  EXPECT_THAT(vector.at(2), Eq(2));
}

TEST(CircVectorCore, PushBackWrapsAround) {
  CircVector<int> vector(3);
  vector.push_back(1);
  vector.push_back(2);
  vector.push_back(3);
  vector.pop_front();
  vector.push_back(4);

  EXPECT_THAT(vector.at(0), Eq(2));
  EXPECT_THAT(vector.at(1), Eq(3));
  EXPECT_THAT(vector.at(2), Eq(4));
}

TEST(CircVectorCore, PopBackWrapsAround) {
  CircVector<int> vector(3);
  vector.push_back(1);
  vector.push_back(2);
  vector.push_back(3);
  vector.pop_front();
  vector.push_back(4);
  int last = vector.pop_back();

  EXPECT_THAT(last, Eq(4));
  EXPECT_THAT(vector.size(), Eq(2));
  EXPECT_THAT(vector.at(0), Eq(2));
  EXPECT_THAT(vector.at(1), Eq(3));
}

TEST(CircVectorCore, PushTogether) {
  CircVector<int> vector;
  vector.push_back(1);
  vector.push_back(2);
  vector.push_front(4);

  EXPECT_THAT(vector.size(), Eq(3));
  EXPECT_THAT(vector.at(0), Eq(4));
  EXPECT_THAT(vector.at(1), Eq(1));
  EXPECT_THAT(vector.at(2), Eq(2));
}

TEST(CircVectorCore, PushFront) {
  CircVector<int> vector;
  vector.push_back(1);
  vector.push_back(2);
  vector.push_front(4);

  EXPECT_THAT(vector.size(), Eq(3));
  EXPECT_FALSE(vector.empty());

  EXPECT_THAT(vector.at(0), Eq(4));
  EXPECT_THAT(vector.at(1), Eq(1));
  EXPECT_THAT(vector.at(2), Eq(2));

  int first = vector.pop_front();
  int second = vector.pop_front();
  int third = vector.pop_front();

  EXPECT_THAT(first, Eq(5));
  EXPECT_THAT(second, Eq(10));
  EXPECT_THAT(third, Eq(20));
  EXPECT_TRUE(vector.empty());
}

TEST(CircVectorCore, PopFront) {
  CircVector<int> vector;
  vector.push_back(0);

  EXPECT_THAT(vector.size(), Eq(1));
  int val = vector.pop_back();

  EXPECT_THAT(val, Eq(0));
  EXPECT_TRUE(vector.empty());
}

TEST(CircVectorCore, ResetList) {
  CircVector<int> vector;
  vector.push_back(1);
  vector.push_back(2);
  vector.push_back(3);

  vector.clear();

  EXPECT_TRUE(vector.empty());
  EXPECT_THAT(vector.size(), Eq(0));
}

TEST(CircVectorAugmented, ReturnsValue) {
  CircVector<int> vector;
  vector.push_back(10);
  vector.push_back(20);
  vector.push_back(30);

  EXPECT_THAT(vector.at(0), Eq(10));
  EXPECT_THAT(vector.at(1), Eq(20));
  EXPECT_THAT(vector.at(2), Eq(30));
}

TEST(CircVectorAugmented, ToStringOutputs) {
  CircVector<int> vector;
  vector.push_back(10);
  vector.push_back(20);
  vector.push_back(30);

  EXPECT_THAT(vector.to_string(), Eq("[10, 20, 30]"));
}

TEST(CircVectorAugmented, FindReturnCorrectIndex) {
  CircVector<int> vector;
  vector.push_back(5);
  vector.push_back(10);
  vector.push_back(15);

  EXPECT_THAT(vector.find(10), Eq(1));
  EXPECT_THAT(vector.find(5), Eq(0));
  EXPECT_THAT(vector.find(99), Eq((size_t)-1));
}

TEST(CircVectorAugmented, RemoveCorrectElement) {
  CircVector<int> vector;
  vector.push_back(1);
  vector.push_back(2);
  vector.push_back(3);

  vector.remove_at(1);

  EXPECT_THAT(vector.size(), Eq(2));
  EXPECT_THAT(vector.at(0), Eq(1));
  EXPECT_THAT(vector.at(1), Eq(3));
}

TEST(CircVectorAugmented, CopiesAllElements) {
  CircVector<int> vector;
  vector.push_back(1);
  vector.push_back(2);
  vector.push_back(3);
  CircVector<int> copyVector = vector;

  EXPECT_THAT(copyVector.size(), Eq(3));
  EXPECT_THAT(copyVector.at(0), Eq(1));
  EXPECT_THAT(copyVector.at(1), Eq(2));
  EXPECT_THAT(copyVector.at(2), Eq(3));
}

TEST(CircVectorAugmented, CopiesOfEmptyList) {
  CircVector<int> emptyVector;
  CircVector<int> vector = emptyVector;

  EXPECT_TRUE(emptyVector.empty());
  EXPECT_THAT(emptyVector.size(), Eq(0));
}

TEST(CircVectorAugmented, BadBounds) {
  CircVector<int> vector;
  vector.push_back(1);
  vector.push_back(2);
  vector.push_back(3);

  EXPECT_THAT(vector.find(1), Eq(0));
  EXPECT_THAT(vector.find(2), Eq(1));
  EXPECT_THAT(vector.find(3), Eq(2));

  EXPECT_THAT(vector.find(99), Eq((size_t)-1));
}

TEST(CircVectorAugmented, AssignFromEmptyList) {
  CircVector<int> emptyVector;
  CircVector<int> vector;
  vector.push_back(5);
  vector.push_back(6);

  vector = emptyVector;

  EXPECT_TRUE(vector.empty());
  EXPECT_THAT(vector.size(), Eq(0));
}

TEST(CircVectorAugmented, AssignDoesNotCreateEmptyList) {
  CircVector<int> Vec1;
  Vec1.push_back(4);
  CircVector<int> Vec2;
  Vec2.push_back(1);
  Vec2.push_back(2);
  Vec2.push_back(3);

  Vec1 = Vec2;

  EXPECT_FALSE(Vec1.empty());
  EXPECT_THAT(Vec1.size(), Eq(3));
  EXPECT_THAT(Vec1.at(0), Eq(1));
  EXPECT_THAT(Vec1.at(1), Eq(2));
  EXPECT_THAT(Vec1.at(2), Eq(3));
}

TEST(CircVectorAugmented, AssignmentToSelf) {
  CircVector<int> vector;
  vector.push_back(1);
  vector.push_back(2);
  vector.push_back(3);

  CircVector<int>& copyVector = vector;
  vector = copyVector;

  EXPECT_THAT(vector.size(), Eq(3));
  EXPECT_THAT(vector.at(0), Eq(1));
  EXPECT_THAT(vector.at(1), Eq(2));
  EXPECT_THAT(vector.at(2), Eq(3));
}