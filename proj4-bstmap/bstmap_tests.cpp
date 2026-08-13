#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <random>

#include "bstmap.h"

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

namespace {

// C O R E  T E S T S

TEST(BSTMapCore, ConstructorIntializesAtZero) {
  BSTMap<int, string> map;

  EXPECT_THAT(0, map.size());
  EXPECT_TRUE(map.empty());
}

TEST(BSTMapCore, MapIsAlwaysEmpty) {
  BSTMap<int, string> map;

  EXPECT_TRUE(map.empty());
}

TEST(BSTMapCore, IsTrueWhenEmpty) {
  BSTMap<int, string> map;

  map.insert(1, "one");
  EXPECT_FALSE(map.empty());
}

TEST(BSTMapCore, SizeIncreases) {
  BSTMap<int, string> map;

  EXPECT_THAT(map.size(), 0);

  map.insert(1, "one");
  EXPECT_THAT(map.size(), 1);

  map.insert(2, "two");
  EXPECT_THAT(map.size(), 2);
}

TEST(BSTMapCore, InsertDoesNotOverwriteExistingValue) {
  BSTMap<int, string> map;

  map.insert(1, "one");
  map.insert(1, "uno");

  EXPECT_THAT(map.at(1), "one");
  EXPECT_THAT(map.size(), 1);
}

TEST(BSTMapCore, IsTrueAfterInsert) {
  BSTMap<int, string> map;
  map.insert(10, "ten");
  map.insert(20, "twenty");

  EXPECT_TRUE(map.contains(10));
  EXPECT_TRUE(map.contains(20));
  EXPECT_FALSE(map.contains(30));
}

TEST(BSTMapCore, IsFalseAfterNoInsert) {
  BSTMap<int, string> map;

  EXPECT_FALSE(map.contains(0));
  EXPECT_FALSE(map.contains(1));
  EXPECT_FALSE(map.contains(-1));
}

TEST(BSTMapCore, ThrowsKeyWhenMissing) {
  BSTMap<int, string> map;

  EXPECT_THROW(map.at(2), out_of_range);
  map.insert(1, "one");
  EXPECT_THROW(map.at(99), out_of_range);
}

TEST(BSTMapCore, ClearsMap) {
  BSTMap<int, string> map;

  map.insert(1, "a");
  map.insert(2, "b");

  map.clear();

  EXPECT_TRUE(map.empty());
  EXPECT_THAT(map.size(), 0);
  EXPECT_FALSE(map.contains(1));
  EXPECT_THROW(map.at(1), out_of_range);
}

TEST(BSTMapCore, CopyAlwaysEmpty) {
  BSTMap<int, string> map;

  map.insert(0, "zero");
  map.insert(1, "one");

  BSTMap<int, string> copy(map);

  EXPECT_THAT(copy.size(), 2);
  EXPECT_FALSE(copy.empty());
  EXPECT_TRUE(copy.contains(0));
  EXPECT_THAT(copy.at(1), "one");
  EXPECT_THROW(copy.at(99), out_of_range);
}

TEST(BSTMapCore, IsFalseWhenCopyofEmpty) {
  BSTMap<int, string> map;
  BSTMap<int, string> copy(map);

  EXPECT_THAT(copy.size(), 0);
  EXPECT_TRUE(copy.empty());
}

TEST(BSTMapCore, AssignCopiesContent) {
  BSTMap<int, string> map;

  map.insert(0, "zero");
  map.insert(1, "one");
  map.insert(2, "two");

  BSTMap<int, string> targetMap;
  targetMap.insert(10, "ten");
  targetMap = map;

  EXPECT_THAT(targetMap.size(), 3);
  EXPECT_FALSE(targetMap.empty());
  EXPECT_TRUE(targetMap.contains(0));
  EXPECT_THAT(targetMap.at(1), "one");
  EXPECT_FALSE(targetMap.contains(10));
}

TEST(BSTMapCore, AssignEmpty) {
  BSTMap<int, string> map;
  BSTMap<int, string> targetMap;

  targetMap.insert(0, "zero");
  targetMap.insert(1, "one");

  targetMap = map;

  EXPECT_THAT(targetMap.size(), 0);
  EXPECT_TRUE(targetMap.empty());
  EXPECT_FALSE(targetMap.contains(1));
  EXPECT_THROW(targetMap.at(2), out_of_range);
}

TEST(BSTMapCore, AssignSelf) {
  BSTMap<int, string> map;

  map.insert(0, "zero");
  map.insert(1, "one");

  BSTMap<int, string>& map2 = map;

  map = map2;

  EXPECT_THAT(map.size(), 2);
  EXPECT_TRUE(map.contains(0));
  EXPECT_THAT(map.at(1), "one");
  EXPECT_FALSE(map.empty());
}

// A U G U M E N T E D  T E S T S

TEST(BSTMapAugmented, RemoveMinThrows) {
  BSTMap<int, string> map;

  EXPECT_THROW(map.remove_min(), runtime_error);
}

TEST(BSTMapAugmented, RemoveMinNoUpdateSize) {
  BSTMap<int, string> map;

  map.insert(0, "zero");
  map.insert(1, "one");
  map.insert(2, "two");

  size_t before = map.size();
  auto removed = map.remove_min();

  EXPECT_THAT(map.size(), before - 1);
  EXPECT_FALSE(map.contains(0));
}

TEST(BSTMapAugmented, RemoveMinReturnsCorrectPair) {
  BSTMap<int, string> map;

  map.insert(1, "one");
  map.insert(2, "two");
  map.insert(3, "three");

  auto removed = map.remove_min();
  EXPECT_THAT(removed.first, 1);
  EXPECT_THAT(removed.second, "one");
}

TEST(BSTMapAugmented, RemoveMinLosesChildren) {
  BSTMap<int, string> map;

  map.insert(5, "five");
  map.insert(2, "two");
  map.insert(6, "six");
  map.insert(1, "one");
  map.insert(3, "three");

  map.remove_min();

  EXPECT_TRUE(map.contains(2));
  EXPECT_TRUE(map.contains(3));
  EXPECT_TRUE(map.contains(5));
  EXPECT_TRUE(map.contains(6));

  map.begin();
  int key;
  string val;

  EXPECT_TRUE(map.next(key, val));
  EXPECT_THAT(key, 2);
}

TEST(BSTMapAugmented, NotEqualWhenSizeDiffers) {
  BSTMap<int, string> map;
  BSTMap<int, string> copyMap;

  map.insert(0, "zero");
  map.insert(1, "one");

  copyMap.insert(0, "zero");

  EXPECT_FALSE(map == copyMap);
  EXPECT_FALSE(copyMap == map);
}

TEST(BSTMapAugmented, NotEqualWhenValueDiffers) {
  BSTMap<int, string> map;
  BSTMap<int, string> copyMap;

  map.insert(1, "one");
  copyMap.insert(1, "uno");

  EXPECT_FALSE(map == copyMap);
}

TEST(BSTMapAugmented, BeginEmptyTree) {
  BSTMap<int, string> map;

  int key;
  string val;

  map.begin();
  EXPECT_FALSE(map.next(key, val));
}

TEST(BSTMapAugmented, BeginNextInOrder) {
  BSTMap<int, string> map;

  map.insert(5, "five");
  map.insert(3, "three");
  map.insert(7, "seven");
  map.insert(6, "six");
  map.insert(8, "eight");

  map.begin();

  vector<int> visited;
  int key;
  string val;

  while (map.next(key, val)) {
    visited.push_back(key);
  }

  vector<int> expected = {3, 5, 6, 7, 8};
  EXPECT_THAT(visited, expected);
}

// E R A S E  T E S T S

TEST(BSTMapErase, DoesNotThrowKey) {
  BSTMap<int, string> map;

  EXPECT_THROW(map.erase(10), out_of_range);
}

TEST(BSTMapErase, EraseLosesDescendents) {
  BSTMap<int, string> map;

  map.insert(5, "five");
  map.insert(3, "three");
  map.insert(8, "eight");
  map.insert(6, "six");

  const size_t mapBefore = map.size();
  string removedNode = map.erase(5);

  EXPECT_EQ(removedNode, "five");
  EXPECT_EQ(map.size(), mapBefore - 1);

  EXPECT_TRUE(map.contains(3));
  EXPECT_TRUE(map.contains(8));
}

TEST(BSTMapErase, RootLosesNoChildren) {
  BSTMap<int, string> map;

  map.insert(2, "two");

  EXPECT_EQ(map.size(), 1);
  EXPECT_TRUE(map.contains(2));

  string removedNode = map.erase(2);
  EXPECT_EQ(removedNode, "two");

  EXPECT_EQ(map.size(), 0);
  EXPECT_FALSE(map.contains(2));
  EXPECT_EQ(map.getRoot(), nullptr);
}

TEST(BSTMapErase, EraseLosesChildren) {
  BSTMap<int, string> map;

  map.insert(8, "eight");
  map.insert(4, "four");
  map.insert(12, "twelve");
  map.insert(2, "two");
  map.insert(6, "six");
  map.insert(7, "seven");
  map.insert(14, "fourteen");

  EXPECT_TRUE(map.contains(8));
  EXPECT_TRUE(map.contains(6));
  EXPECT_TRUE(map.contains(7));

  string removedNode = map.erase(4);
  EXPECT_EQ(removedNode, "four");
}

TEST(BSTMapErase, ErasesWrongChildren) {
  BSTMap<int, string> map;

  map.insert(5, "five");
  map.insert(3, "three");
  map.insert(8, "eight");
  map.insert(6, "six");
  map.insert(7, "seven");

  const size_t beforeSize = map.size();
  string removedValue = map.erase(5);
  EXPECT_EQ(removedValue, "five");

  EXPECT_FALSE(map.contains(5));
  EXPECT_TRUE(map.contains(7));
  EXPECT_TRUE(map.contains(6));
  EXPECT_TRUE(map.contains(3));
  EXPECT_TRUE(map.contains(8));

  EXPECT_EQ(map.size(), beforeSize - 1);
}
}  // namespace

}  // namespace