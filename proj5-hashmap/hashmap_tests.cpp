#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "hashmap.h"

using namespace std;
using namespace testing;

namespace {

class Random {
 private:
  static mt19937 rng;

 public:
  static void seed(int s) {
    Random::rng.seed(s);
  }

  /**
   * Generate a random integer in the range 0 (inclusive) to `max` (exclusive)
   */
  static int randInt(int max) {
    // Even though mt19937 is standardized, STL distributions aren't!
    // So, unfortunately, even though this is biased, we're forced to
    // do something like this. Technically uint32_fast_t isn't...
    // totally consistent across platforms? But within reason it works.
    return rng() % (max + 1);
  }
};

std::mt19937 Random::rng;

TEST(HashMapCore, StartSizeCorrect) {
  HashMap<int, string> map;

  EXPECT_EQ(0, map.size());
  EXPECT_TRUE(map.empty());
}

TEST(HashMapCore, EmptyAlwaysTrue) {
  HashMap<int, string> map;

  EXPECT_TRUE(map.empty());
  map.insert(1, "a");
  EXPECT_FALSE(map.empty());
}

TEST(HashMapCore, SizeUpdatesCorrectly) {
  HashMap<int, string> map;

  map.insert(1, "a");
  map.insert(2, "b");

  EXPECT_EQ(2, map.size());
}

TEST(HashMapCore, InsertDoesNotOverwrite) {
  HashMap<int, string> map;

  map.insert(1, "a");
  map.insert(1, "b");

  EXPECT_EQ("a", map.at(1));
  EXPECT_EQ(1, map.size());
}

TEST(HashMapCore, InsertCollision) {
  HashMap<int, string> map;

  map.insert(1, "a");
  map.insert(11, "b");

  EXPECT_TRUE(map.contains(1));
  EXPECT_TRUE(map.contains(11));
  EXPECT_EQ("a", map.at(1));
  EXPECT_EQ("b", map.at(11));
  EXPECT_EQ(2, map.size());
}

TEST(HashMapCore, ContainsWorks) {
  HashMap<int, string> map;
  map.insert(1, "a");

  EXPECT_TRUE(map.contains(1));
  EXPECT_FALSE(map.contains(2));
}

TEST(HashMapCore, ThrowsOnMissingKey) {
  HashMap<int, string> map;

  map.insert(1, "a");

  EXPECT_NO_THROW(map.at(1));
  EXPECT_THROW(map.at(10), out_of_range);
}

TEST(HashMapCore, CkearResetsSize) {
  HashMap<int, string> map;

  map.insert(1, "a");
  map.insert(2, "b");

  EXPECT_EQ(2, map.size());

  map.clear();

  EXPECT_EQ(0, map.size());
  EXPECT_TRUE(map.empty());
}

TEST(HashMapCore, CopyConstructorCopiesData) {
  HashMap<int, string> map;

  map.insert(1, "a");
  map.insert(2, "b");

  HashMap<int, string> copy(map);

  EXPECT_EQ(2, map.size());
  EXPECT_EQ(2, copy.size());

  EXPECT_TRUE(copy.contains(1));
  EXPECT_TRUE(copy.contains(2));

  EXPECT_EQ("a", copy.at(1));
  EXPECT_EQ("b", copy.at(2));
}

TEST(HashMapCore, AssignmentOperatorCopiesData) {
  HashMap<int, string> map;

  map.insert(1, "a");
  map.insert(2, "b");

  HashMap<int, string> copyMap;
  copyMap = map;

  EXPECT_EQ(2, map.size());
  EXPECT_TRUE(map.contains(1));
  EXPECT_TRUE(map.contains(2));

  EXPECT_EQ(2, copyMap.size());
  EXPECT_TRUE(copyMap.contains(1));
  EXPECT_TRUE(copyMap.contains(2));

  EXPECT_EQ("a", copyMap.at(1));
  EXPECT_EQ("b", copyMap.at(2));
}

TEST(HashMapCore, AssignmentOperatorSelf) {
  HashMap<int, string> map;

  map.insert(1, "a");
  map.insert(2, "b");

  HashMap<int, string>& copyMap = map;

  map = copyMap;

  EXPECT_TRUE(map.contains(1));
  EXPECT_EQ(map.at(1), "a");

  EXPECT_EQ(map.size(), 2);
}

TEST(HashMapCore, EraseThrowsOnMissingKey) {
  HashMap<int, string> map;

  map.insert(1, "a");
  EXPECT_THROW(map.erase(2), out_of_range);
}

TEST(HashMapCore, EraseDoesntNotRemoveCollidingKeys) {
  HashMap<int, string> map;

  map.insert(1, "a");
  map.insert(11, "b");

  map.erase(1);

  EXPECT_TRUE(map.contains(11));
  EXPECT_EQ(map.at(11), "b");
}

TEST(HashMapAugmented, EqualityChecksSizeFirst) {
  HashMap<int, string> mapOne;
  HashMap<int, string> mapTwo;

  mapOne.insert(1, "a");
  mapTwo.insert(1, "a");
  mapTwo.insert(2, "b");

  EXPECT_FALSE(mapOne == mapTwo);
}

TEST(HashMapAugmented, EqualityFailswhenDiffer) {
  HashMap<int, string> mapOne;
  HashMap<int, string> mapTwo;

  mapOne.insert(1, "a");
  mapTwo.insert(1, "b");

  EXPECT_FALSE(mapOne == mapTwo);
}

TEST(HashMapAugmented, BeginNextDoesntWorkEmpty) {
  HashMap<int, string> map;

  map.begin();
  int key;
  string value;

  EXPECT_FALSE(map.next(key, value));
}

TEST(HashMapAugmented, NextCollisions) {
  HashMap<int, string> map;

  map.insert(1, "a");
  map.insert(11, "b");

  map.begin();
  int key;
  string value;
  vector<int> seen;

  while (map.next(key, value)) {
    seen.push_back(key);
  }

  EXPECT_EQ(seen.size(), 2);
}

}  // namespace
