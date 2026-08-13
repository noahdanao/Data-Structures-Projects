#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "include/lyrics.h"

using namespace std;
using namespace testing;

TEST(CleanToken, handlesBasicWord) {
  EXPECT_THAT(cleanToken("Hello"), "hello");
}

TEST(CleanToken, handlesPunctuationBothEnds) {
  EXPECT_THAT(cleanToken("!!Hello!!"), "hello");
}

TEST(CleanToken, punctuationBothEnds) {
  EXPECT_THAT(cleanToken("co-op"), "co-op");
}

TEST(CleanToken, punctuationInMiddleOnly) {
  EXPECT_THAT(cleanToken("!!co-op"), "co-op");
}

TEST(CleanToken, punctuationMiddleAndStart) {
  EXPECT_THAT(cleanToken("!!co-op"), "co-op");
}

TEST(CleanToken, punctuationMiddleAndEnd) {
  EXPECT_THAT(cleanToken("co-op!!"), "co-op");
}

TEST(CleanToken, singleLetter) {
  EXPECT_THAT(cleanToken("X"), "x");
}

TEST(CleanToken, heavyPunctuation) {
  EXPECT_THAT(cleanToken("......a......"), "a");
}

TEST(GatherTokens, handlesBasicSentence) {
  set<string> expected = {"hello", "world"};
  EXPECT_THAT(gatherTokens("hello world"), expected);
}

TEST(GatherTokens, removesDuplicates) {
  set<string> expected = {"hello", "world"};
  EXPECT_THAT(gatherTokens("hello hello world"), expected);
}

TEST(GatherTokens, handlesLeadingSpaces) {
  set<string> expected = {"hello", "world"};
  EXPECT_THAT(gatherTokens("  hello world"), expected);
}

TEST(GatherTokens, handlesTrailingSpaces) {
  set<string> expected = {"hello", "world"};
  EXPECT_THAT(gatherTokens("hello world  "), expected);
}

TEST(GatherTokens, handlesMultipleSpaceBetween) {
  set<string> expected = {"hello", "world"};
  EXPECT_THAT(gatherTokens("hello   world"), expected);
}

TEST(BuildIndices, TinyTxt) {
  map<string, set<string>> invertedIndex;
  map<string, string> artistIndex;

  int count = buildIndices("data/tiny.txt", invertedIndex, artistIndex);
  EXPECT_THAT(count, 4);
  map<string, string> expectedArtistIndex = {
      {"Jingle bells", "James Pierpont"},
      {"Brother John", "Unknown"},
      {"Baa baa Black Sheep", "Unknown"},
      {"Carol of the bells", "Mykola Leontovych"}};

  EXPECT_THAT(artistIndex, expectedArtistIndex);
}

TEST(BuildIndices, TinerTxt) {
  map<string, set<string>> invertedIndex;
  map<string, string> artistIndex;

  int count = buildIndices("data/tinier.txt", invertedIndex, artistIndex);
  EXPECT_THAT(count, 2);

  map<string, string> expectedArtistIndex = {{"Song 1", "Someone"},
                                             {"Song 2", "Someone Else"}};
  EXPECT_THAT(artistIndex, expectedArtistIndex);
}

TEST(BuildIndices, FileNotFound) {
  map<string, set<string>> invertedIndex;
  map<string, string> artistIndex;

  int count =
      buildIndices("data/does_not_exist.txt", invertedIndex, artistIndex);

  EXPECT_THAT(count, 0);
  EXPECT_TRUE(invertedIndex.empty());
  EXPECT_TRUE(artistIndex.empty());
}

TEST(FindQueryMatches, FirstTermNotInIndex) {
  map<string, set<string>> index = {
      {"day", {"Song2", "Song3"}},
  };

  set<string> result = findQueryMatches(index, "missing +day");
  EXPECT_THAT(result, ContainerEq(set<string>{}));
}

TEST(FindQueryMatches, LaterTermNotInIndexUnion) {
  map<string, set<string>> index = {
      {"eyes", {"Song1", "Song3"}},
  };

  set<string> result = findQueryMatches(index, "eyes missing");
  EXPECT_THAT(result, ContainerEq(set<string>{"Song1", "Song3"}));
}

TEST(FindQueryMatches, LaterTermNotInIndexIntersection) {
  map<string, set<string>> index = {{"day", {"Song2", "Song3"}}};

  set<string> result = findQueryMatches(index, "day +missing");
  EXPECT_THAT(result, ContainerEq(set<string>{}));
}

TEST(FindQueryMatches, LaterTermNotInINdexDifference) {
  map<string, set<string>> index = {{"eyes", {"Song1", "Song3"}}};

  set<string> result = findQueryMatches(index, "eyes -missing");
  EXPECT_THAT(result, ContainerEq(set<string>{"Song1", "Song3"}));
}
