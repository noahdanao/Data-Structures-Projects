#include "include/lyrics.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string cleanToken(const string& token) {
  // TODO student
  string lowerToken = token;

  for (char& ch : lowerToken) {
    ch = tolower(ch);
  }

  bool hasLetter = false;
  for (char ch : lowerToken) {
    if (isalpha(ch)) {
      hasLetter = true;
      break;
    }
  }
  if (!hasLetter) {
    return "";
  }

  size_t start = 0;
  while (start < lowerToken.size() && ispunct(lowerToken[start])) {
    ++start;
  }
  size_t end = lowerToken.size();
  while (end > start && ispunct(lowerToken[end - 1])) {
    --end;
  }

  return lowerToken.substr(start, end - start);
}

set<string> gatherTokens(const string& text) {
  // TODO student
  set<string> tokens;
  stringstream ss(text);
  string word;

  while (ss >> word) {
    string cleanedWord = cleanToken(word);
    if (!cleanedWord.empty()) {
      tokens.insert(cleanedWord);
    }
  }

  return tokens;
}

int buildIndices(const string& filename,
                 map<string, set<string>>& inverted_index,
                 map<string, string>& artist_index) {
  // TODO student
  ifstream infile(filename);
  if (!infile.is_open()) {
    return 0;
  }

  string title;
  string artist;
  string lyrics;
  int count = 0;

  while (getline(infile, title)) {
    if (title.empty()) {
      continue;
    }
    if (!getline(infile, artist)) {
      break;
    }
    if (!getline(infile, lyrics)) {
      break;
    }

    artist_index[title] = artist;
    set<string> tokens = gatherTokens(lyrics);
    for (const string& t : tokens) {
      inverted_index[t].insert(title);
    }
    count++;
  }

  infile.close();
  return count;
}

set<string> findQueryMatches(const map<string, set<string>>& index,
                             const string& sentence) {
  // TODO student
  stringstream ss(sentence);
  string token;
  set<string> results;
  bool firstTerm = true;

  while (ss >> token) {
    char modifer = '\0';

    if (token[0] == '+' || token[0] == '-') {
      modifer = token[0];
      token = token.substr(1);
    }

    string cleaned = cleanToken(token);
    if (cleaned.empty()) {
      continue;
    }
    set<string> matches;
    auto it = index.find(cleaned);

    if (it != index.end()) {
      matches = it->second;
    }
    if (firstTerm) {
      results = matches;
      firstTerm = false;
      continue;
    }
    if (modifer == '+') {
      set<string> temp;
      if (results.size() <= matches.size()) {
        for (const auto& t : results) {
          if (matches.find(t) != matches.end()) {
            temp.insert(t);
          }
        }
      } else {
        for (const auto& t : matches) {
          if (results.find(t) != results.end()) {
            temp.insert(t);
          }
        }
      }
      results.swap(temp);
    } else if (modifer == '-') {
      set<string> temp;
      for (const auto& t : results) {
        if (matches.find(t) == matches.end()) {
          temp.insert(t);
        }
      }
      results.swap(temp);
    } else {
      for (const auto& t : matches) {
        results.insert(t);
      }
    }
  }
  return results;
}

void searchEngine(const string& filename) {
  // TODO student

  map<string, set<string>> inverted_index;
  map<string, string> artist_index;

  int numSongs = buildIndices(filename, inverted_index, artist_index);

  if (numSongs == 0) {
    cout << "Invalid filename." << endl;
    cout << "Stand by while building indices..." << endl;
    cout << "Indexed 0 songs containing 0 unique terms and 0 artists." << endl;
  } else {
    cout << "Stand by while building indices..." << endl;
    int numUniqueWords = inverted_index.size();

    set<string> artists;
    for (const auto& entry : artist_index) {
      artists.insert(entry.second);
    }

    cout << "Indexed " << numSongs << " songs containing " << numUniqueWords
         << " unique terms and " << artists.size() << " artists." << endl;
  }

  while (true) {
    cout << "Enter query sentence (press enter to quit): ";
    string query;
    getline(cin, query);

    if (query.empty()) {
      cout << "Thank you for searching our Lyrics DB!" << endl;
      break;
    }

    set<string> results = findQueryMatches(inverted_index, query);

    cout << "Found " << results.size() << " matching songs" << endl;

    vector<string> sortedResults(results.begin(), results.end());
    sort(sortedResults.begin(), sortedResults.end());

    for (const auto& title : sortedResults) {
      cout << title << " by " << artist_index[title] << endl;
    }
  }
}
