#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "linkedlist.h"

using namespace std;
using namespace testing;

TEST(LinkedListCore, EmptyConstructor) {
  LinkedList<int> list;
  EXPECT_TRUE(list.empty());
  EXPECT_THAT(list.size(), Eq(0));
}

TEST(LinkedListCore, ThrowsWhenOutofRange) {
  LinkedList<int> list;
  list.push_back(1);
  EXPECT_THROW(list.at(5), out_of_range);
}

TEST(LinkedListCore, PopBackThrowsEmpty) {
  LinkedList<int> list;
  EXPECT_THROW(list.pop_back(), runtime_error);
}

TEST(LinkedListCore, PopFrontThrowsEmpty) {
  LinkedList<int> list;
  EXPECT_THROW(list.pop_front(), runtime_error);
}

TEST(LinkedListCore, PopBackDecreasesSize) {
  LinkedList<int> list;
  list.push_back(1);
  list.push_back(2);
  size_t before = list.size();
  list.pop_back();
  EXPECT_THAT(list.size(), Eq(before - 1));
}

TEST(LinkedListCore, PushTogether) {
  LinkedList<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_front(5);

  EXPECT_THAT(list.size(), Eq(3));
  EXPECT_THAT(list.at(0), Eq(5));
  EXPECT_THAT(list.at(1), Eq(1));
  EXPECT_THAT(list.at(2), Eq(2));
}

TEST(LinkedListCore, PushFront) {
  LinkedList<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_front(5);

  EXPECT_THAT(list.size(), Eq(3));
  EXPECT_FALSE(list.empty());

  EXPECT_THAT(list.at(0), Eq(5));
  EXPECT_THAT(list.at(1), Eq(1));
  EXPECT_THAT(list.at(2), Eq(2));

  int first = list.pop_front();
  int second = list.pop_front();
  int third = list.pop_front();

  EXPECT_THAT(first, Eq(5));
  EXPECT_THAT(second, Eq(1));
  EXPECT_THAT(third, Eq(2));
  EXPECT_TRUE(list.empty());
}

TEST(LinkedListCore, PopFront) {
  LinkedList<int> list;
  list.push_back(42);

  EXPECT_THAT(list.size(), Eq(1));
  int val = list.pop_back();

  EXPECT_THAT(val, Eq(42));
  EXPECT_TRUE(list.empty());
}

TEST(LinkedListCore, ResetList) {
  LinkedList<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  list.clear();

  EXPECT_TRUE(list.empty());
  EXPECT_THAT(list.size(), Eq(0));
}

TEST(LinkedListCore, ReturnsValue) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  EXPECT_THAT(list.at(0), Eq(10));
  EXPECT_THAT(list.at(1), Eq(20));
  EXPECT_THAT(list.at(2), Eq(30));
}

TEST(LinkedListAugmented, ToStringOutputs) {
  LinkedList<int> list;
  list.push_back(10);
  list.push_back(20);
  list.push_back(30);

  EXPECT_THAT(list.to_string(), Eq("[10, 20, 30]"));
}

TEST(LinkedListAugmented, FindReturnCorrectIndex) {
  LinkedList<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);

  EXPECT_THAT(list.find(10), Eq(1));
  EXPECT_THAT(list.find(5), Eq(0));
  EXPECT_THAT(list.find(99), Eq((size_t)-1));
}

TEST(LinkedListAugmented, RemoveCorrectElement) {
  LinkedList<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  list.remove_at(1);

  EXPECT_THAT(list.size(), Eq(2));
  EXPECT_THAT(list.at(0), Eq(1));
  EXPECT_THAT(list.at(1), Eq(3));
}

TEST(LinkedListAugmented, CopiesAllElements) {
  LinkedList<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);
  LinkedList<int> copyList = list;

  EXPECT_THAT(copyList.size(), Eq(3));
  EXPECT_THAT(copyList.at(0), Eq(1));
  EXPECT_THAT(copyList.at(1), Eq(2));
  EXPECT_THAT(copyList.at(2), Eq(3));
}

TEST(LinkedListAugmented, CopiesOfEmptyList) {
  LinkedList<int> emptyList;
  LinkedList<int> list = emptyList;

  EXPECT_TRUE(emptyList.empty());
  EXPECT_THAT(emptyList.size(), Eq(0));
}

TEST(LinkedListAugmented, BadBounds) {
  LinkedList<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  EXPECT_THAT(list.find(1), Eq(0));
  EXPECT_THAT(list.find(2), Eq(1));
  EXPECT_THAT(list.find(3), Eq(2));

  EXPECT_THAT(list.find(99), Eq((size_t)-1));
}

TEST(LinkedListAugmented, AssignFromEmptyList) {
  LinkedList<int> emptyList;
  LinkedList<int> mainList;
  mainList.push_back(5);
  mainList.push_back(6);

  mainList = emptyList;

  EXPECT_TRUE(mainList.empty());
  EXPECT_THAT(mainList.size(), Eq(0));
}

TEST(LinkedListAugmented, AssignDoesNotCreateEmptyList) {
  LinkedList<int> list1;
  list1.push_back(4);
  LinkedList<int> list2;
  list2.push_back(1);
  list2.push_back(2);
  list2.push_back(3);

  list1 = list2;

  EXPECT_FALSE(list1.empty());
  EXPECT_THAT(list1.size(), Eq(3));
  EXPECT_THAT(list1.at(0), Eq(1));
  EXPECT_THAT(list1.at(1), Eq(2));
  EXPECT_THAT(list1.at(2), Eq(3));
}

TEST(LinkedListAugmented, AssignmentToSelf) {
  LinkedList<int> list;
  list.push_back(1);
  list.push_back(2);
  list.push_back(3);

  LinkedList<int>& copyList = list;
  list = copyList;

  EXPECT_THAT(list.size(), Eq(3));
  EXPECT_THAT(list.at(0), Eq(1));
  EXPECT_THAT(list.at(1), Eq(2));
  EXPECT_THAT(list.at(2), Eq(3));
}