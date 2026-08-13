#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

template <typename KeyT, typename ValT>
class HashMap {
 private:
  struct ChainNode {
    const KeyT key;
    ValT value;
    ChainNode* next;

    ChainNode(KeyT key, ValT value) : key(key), value(value), next(nullptr) {
    }

    ChainNode(KeyT key, ValT value, ChainNode* next)
        : key(key), value(value), next(next) {
    }
  };

  ChainNode** data;
  size_t sz;
  size_t capacity;

  // Utility members for begin/next
  ChainNode* curr;
  size_t curr_idx;

 public:
  /**
   * Creates an empty `HashMap` with 10 buckets.
   */
  HashMap() {
    // TODO_STUDENT
    this->capacity = 10;
    this->data = new ChainNode*[this->capacity]();
    this->sz = 0;
    this->curr = nullptr;
    this->curr_idx = 0;
  }

  /**
   * Creates an empty `HashMap` with `capacity` buckets.
   */
  HashMap(size_t capacity) {
    // TODO_STUDENT
    if (capacity == 0) {
      capacity = 1;
    }
    this->capacity = capacity;
    this->data = new ChainNode*[this->capacity]();
    this->sz = 0;
    this->curr = nullptr;
    this->curr_idx = 0;
  }

  /**
   * Checks if the `HashMap` is empty. Runs in O(1).
   */
  bool empty() const {
    // TODO_STUDENT
    return this->sz == 0;
  }

  /**
   * Returns the number of mappings in the `HashMap`. Runs in O(1).
   */
  size_t size() const {
    // TODO_STUDENT
    return this->sz;
  }

  // additional helper- resize() function for capacity when buckets overfill in
  // Hash tables

  void resize(size_t new_capacity) {
    ChainNode** new_data = new ChainNode*[new_capacity]();

    for (size_t i = 0; i < this->capacity; i++) {
      ChainNode* curr = this->data[i];
      while (curr != nullptr) {
        ChainNode* next = curr->next;

        size_t new_index = hash<KeyT>{}(curr->key) % new_capacity;

        curr->next = new_data[new_index];
        new_data[new_index] = curr;

        curr = next;
      }
    }

    delete[] this->data;
    this->data = new_data;
    this->capacity = new_capacity;
  }

  /**
   * Adds the mapping `{key -> value}` to the `HashMap`. If the key already
   * exists, does not update the mapping (like the C++ STL map).
   *
   * Uses the hash value of the key to determine the location in the
   * underlying hash table. Creates exactly one new node; resizes by doubling
   * when the load factor exceeds 1.5.
   *
   * On resize, doesn't create new nodes, but rearranges existing ones.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  void insert(KeyT key, ValT value) {
    // TODO_STUDENT
    size_t index = hash<KeyT>{}(key) % this->capacity;
    ChainNode* head = this->data[index];
    ChainNode* curr = head;

    while (curr != nullptr) {
      if (curr->key == key) {
        return;
      }

      curr = curr->next;
    }

    ChainNode* newNode = new ChainNode(key, value, head);
    this->data[index] = newNode;
    this->sz++;

    if ((double)this->sz / this->capacity > 1.5) {
      this->resize(this->capacity * 2);
    }
  }

  /**
   * Return a reference to the value stored for `key` in the map.
   *
   * If key is not present in the map, throw `out_of_range` exception.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  ValT& at(const KeyT& key) const {
    // TODO_STUDENT
    size_t index = hash<KeyT>{}(key) % this->capacity;

    ChainNode* curr = this->data[index];

    while (curr != nullptr) {
      if (curr->key == key) {
        return curr->value;
      }
      curr = curr->next;
    }

    throw out_of_range("error");
  }

  /**
   * Returns `true` if the key is present in the map, and false otherwise.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  bool contains(const KeyT& key) const {
    size_t index = hash<KeyT>{}(key) % this->capacity;

    ChainNode* curr = this->data[index];
    while (curr != nullptr) {
      if (curr->key == key) {
        return true;
      }
      curr = curr->next;
    }

    return false;
  }

  /**
   * Empties the `HashMap`, freeing all nodes. The bucket array may be left
   * alone.
   *
   * Runs in O(N+B), where N is the number of mappings and B is the number of
   * buckets.
   */
  void clear() {
    // TODO_STUDENT
    for (size_t i = 0; i < this->capacity; i++) {
      ChainNode* curr = this->data[i];
      while (curr != nullptr) {
        ChainNode* next = curr->next;
        delete curr;
        curr = next;
      }
      this->data[i] = nullptr;
    }

    this->sz = 0;
  }

  /**
   * Destructor, cleans up the `HashMap`.
   *
   * Runs in O(N+B), where N is the number of mappings and B is the number of
   * buckets.
   */
  ~HashMap() {
    // TODO_STUDENT
    clear();
    delete[] this->data;
  }

  /**
   * Removes the mapping for the given key from the `HashMap`, and returns the
   * value.
   *
   * Throws `out_of_range` if the key is not present in the map. Creates no new
   * nodes, and does not update the key or value of any existing nodes.
   *
   * Runs in O(L), where L is the length of the longest chain.
   */
  ValT erase(const KeyT& key) {
    // TODO_STUDENT
    size_t index = hash<KeyT>{}(key) % this->capacity;

    ChainNode* curr = this->data[index];
    ChainNode* prev = nullptr;

    while (curr != nullptr) {
      if (curr->key == key) {
        ValT removed_value = curr->value;

        if (prev == nullptr) {
          this->data[index] = curr->next;
        } else {
          prev->next = curr->next;
        }

        delete curr;
        this->sz--;

        return removed_value;
      }

      prev = curr;
      curr = curr->next;
    }

    throw out_of_range("key");
  }

  /**
   * Copy constructor.
   *
   * Copies the mappings from the provided `HashMap`.
   *
   * Runs in O(N+B), where N is the number of mappings in `other`, and B is the
   * number of buckets.
   */
  HashMap(const HashMap& other) {
    // TODO_STUDENT
    this->capacity = other.capacity;
    this->sz = other.sz;

    this->data = new ChainNode*[this->capacity]();

    this->curr = nullptr;
    this->curr_idx = 0;

    for (size_t i = 0; i < this->capacity; i++) {
      ChainNode* curr_other = other.data[i];
      ChainNode* prev_new = nullptr;

      while (curr_other != nullptr) {
        ChainNode* new_node = new ChainNode(curr_other->key, curr_other->value);

        if (prev_new == nullptr) {
          this->data[i] = new_node;
        } else {
          prev_new->next = new_node;
        }

        prev_new = new_node;
        curr_other = curr_other->next;
      }
    }
  }

  /**
   * Assignment operator; `operator=`.
   *
   * Clears this table, and copies the mappings from the provided `HashMap`.
   *
   * Runs in O((N1+B1) + (N2+B2)), where N1 and B1 are the number of mappings
   * and buckets in `this`, and N2 and B2 are the number of mappings and buckets
   * in `other`.
   */
  HashMap& operator=(const HashMap& other) {
    // TODO_STUDENT
    if (this == &other) {
      return *this;
    }

    clear();
    delete[] this->data;

    this->capacity = other.capacity;
    this->sz = other.sz;
    this->curr = nullptr;
    this->curr_idx = 0;

    // allocate new buckets
    this->data = new ChainNode*[this->capacity]();

    // deep copy each bucket
    for (size_t i = 0; i < this->capacity; i++) {
      ChainNode* curr_other = other.data[i];
      ChainNode* prev_new = nullptr;

      while (curr_other != nullptr) {
        ChainNode* new_node = new ChainNode(curr_other->key, curr_other->value);

        if (prev_new == nullptr) {
          this->data[i] = new_node;
        } else {
          prev_new->next = new_node;
        }

        prev_new = new_node;
        curr_other = curr_other->next;
      }
    }

    return *this;
  }

  // =====================

  /**
   *
   * Checks if the contents of `this` and `other` are equivalent.
   *
   * Two `HashMap` objects are equivalent if they contain the same
   * mappings from key to value. To be equivalent, the two `HashMap`
   * objects need not have the elements saved in the same order
   * inside of the buckets.
   *
   * Runs in worst-case O(B*L) time, where B is the maximum number
   * of buckets in either of the `HashMap` objects and L is the length
   * of the  largest chain on any of the buckets.
   *
   */
  bool operator==(const HashMap& other) const {
    if (this->sz != other.sz) {
      return false;
    }

    for (size_t i = 0; i < this->capacity; i++) {
      ChainNode* curr = this->data[i];

      while (curr != nullptr) {
        if (!other.contains(curr->key)) {
          return false;
        }

        if (other.at(curr->key) != curr->value) {
          return false;
        }

        curr = curr->next;
      }
    }
    return true;
  }

  /**
   * Resets internal state for an iterative traversal.
   *
   * See `next` for usage details. Modifies nothing except for `curr` and
   * `curr_idx`.
   *
   * Runs in worst-case O(B), where B is the number of buckets.
   */
  void begin() {
    // TODO_STUDENT
    curr_idx = 0;
    curr = nullptr;

    while (curr_idx < capacity && data[curr_idx] == nullptr) {
      curr_idx++;
    }

    if (curr_idx == capacity) {
      curr = nullptr;
    } else {
      curr = data[curr_idx];
      curr_idx++;
    }
  }

  /**
   * Uses the internal state to return the "next" key and value
   * by reference, and advances the internal state. Returns `true` if the
   * reference parameters were set, and `false` otherwise.
   *
   * Example usage:
   *
   * ```c++
   * HashMap<string, int> hm;
   * hm.begin();
   * string key;
   * int value;
   * while (hm.next(key, val)) {
   *   cout << key << ": " << val << endl;
   * }
   * ```
   *
   * Does not visit the mappings in any defined order.
   *
   * Modifies nothing except for `curr` and `curr_idx`.
   *
   * Runs in worst-case O(B) where B is the number of buckets.
   */
  bool next(KeyT& key, ValT& value) {
    // TODO_STUDENT
    while (curr == nullptr && curr_idx < capacity) {
      curr = data[curr_idx];
      curr_idx++;
    }

    if (curr == nullptr) {
      return false;
    }

    key = curr->key;
    value = curr->value;
    curr = curr->next;

    return true;
  }

  // ===============================================

  /**
   * Returns a pointer to the underlying memory managed by the `HashMap`.
   * For autograder testing purposes only.
   */
  void* get_data() const {
    return this->data;
  }

  /**
   * Returns the capacity of the underlying memory managed by the `HashMap`. For
   * autograder testing purposes only.
   */
  size_t get_capacity() {
    return this->capacity;
  }
};
