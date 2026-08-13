#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

template <typename KeyT, typename ValT>
class BSTMap {
 private:
  struct BSTNode {
    const KeyT key;  // This member should never change after initialization!
    ValT value;
    BSTNode* parent;
    BSTNode* left;
    BSTNode* right;

    // This is fancy constructor syntax
    // Don't worry too much about it -- it's just necesssary for the const
    // member.
    BSTNode(KeyT key, ValT value, BSTNode* parent)
        : key(key),
          value(value),
          parent(parent),
          left(nullptr),
          right(nullptr) {
    }
  };

  BSTNode* root;
  size_t sz;

  // Utility pointer for begin and next.
  BSTNode* curr;

  // TODO_STUDENT: add private helper function definitions here
  bool insert_recursion(BSTNode*& curr, BSTNode* parent, const KeyT& key,
                        const ValT& value) {
    if (curr == nullptr) {
      curr = new BSTNode(key, value, parent);
      return true;
    }

    if (key == curr->key) {
      return false;
    }

    if (key < curr->key) {
      return insert_recursion(curr->left, curr, key, value);
    } else {
      return insert_recursion(curr->right, curr, key, value);
    }
  }

  BSTNode* find_recursion(BSTNode* curr, BSTNode* parent,
                          const KeyT& key) const {
    if (curr == nullptr) {
      return nullptr;
    }
    if (key == curr->key) {
      return curr;
    }
    if (key < curr->key) {
      return find_recursion(curr->left, curr, key);
    } else {
      return find_recursion(curr->right, curr, key);
    }
  }

  void clear_recursion(BSTNode* node) {
    if (node == nullptr) {
      return;
    }
    clear_recursion(node->left);
    clear_recursion(node->right);
    delete node;
  }

  void inOrder_string(const BSTNode* node, stringstream& ss) const {
    if (node == nullptr) {
      return;
    }

    inOrder_string(node->left, ss);
    ss << node->key << ": " << node->value << "\n";
    inOrder_string(node->right, ss);
  }

  BSTNode* copy_recursion(const BSTNode* copyNode, BSTNode* copyParent) {
    if (copyNode == nullptr) {
      return nullptr;
    }

    BSTNode* new_node = new BSTNode(copyNode->key, copyNode->value, copyParent);
    new_node->left = copy_recursion(copyNode->left, new_node);
    new_node->right = copy_recursion(copyNode->right, new_node);

    return new_node;
  }

  BSTNode* remove_min_node(BSTNode* node) {
    if (node == nullptr) {
      return nullptr;
    }
    if (node->left == nullptr) {
      BSTNode* right = node->right;
      if (right != nullptr) {
        right->parent = node->parent;
      }
      delete node;

      return right;
    }
    node->left = remove_min_node(node->left);
    if (node->left != nullptr) {
      node->left->parent = node;
    }

    return node;
  }

  BSTNode* erase_recursion(BSTNode* node, const KeyT& key, ValT& removed_val) {
    if (node == nullptr) {
      throw out_of_range("error");
    }

    if (key < node->key) {
      node->left = erase_recursion(node->left, key, removed_val);
      if (node->left) {
        node->left->parent = node;
      }
    } else if (key > node->key) {
      node->right = erase_recursion(node->right, key, removed_val);
      if (node->right) {
        node->right->parent = node;
      }
    } else {
      removed_val = node->value;
      if (!node->left) {
        BSTNode* right = node->right;
        if (right) {
          right->parent = node->parent;
        }
        delete node;
        return right;
      }

      if (!node->right) {
        BSTNode* left = node->left;
        if (left) {
          left->parent = node->parent;
        }
        delete node;
        return left;
      }

      BSTNode* successor = node->right;
      while (successor->left) {
        successor = successor->left;
      }

      BSTNode* replacement =
          new BSTNode(successor->key, successor->value, node->parent);
      replacement->left = node->left;
      replacement->right = node->right;

      if (replacement->left) {
        replacement->left->parent = replacement;
      }
      if (replacement->right) {
        replacement->right->parent = replacement;
      }

      ValT dummy;
      replacement->right =
          erase_recursion(replacement->right, successor->key, dummy);
      if (replacement->right) {
        replacement->right->parent = replacement;
      }

      delete node;
      return replacement;
    }
    return node;
  }

 public:
  /**
   * Creates an empty `BSTMap`. Runs in O(1).
   */
  BSTMap() {
    // TODO_STUDENT
    root = nullptr;
    sz = 0;
    curr = nullptr;
  }

  /**
   * Checks if the `BSTMap` is empty. Runs in O(1).
   */
  bool empty() const {
    // TODO_STUDENT
    return sz == 0;
  }

  /**
   * Returns the number of mappings in the `BSTMap`. Runs in O(1).
   */
  size_t size() const {
    // TODO_STUDENT
    return sz;
  }

  /**
   * Adds the mapping `{key -> value}` to the `BSTMap`. If the key already
   * exists, does not update the mapping (like the C++ STL map).
   *
   * Uses the key to determine the location in the underlying BST. Creates
   * exactly one new node.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  void insert(KeyT key, ValT value) {
    // TODO_STUDENT
    if (insert_recursion(root, nullptr, key, value)) {
      sz++;
    }
  }

  /**
   * Return a reference to the value stored for `key` in the map.
   *
   * If key is not present in the map, throw `out_of_range` exception.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  ValT& at(const KeyT& key) const {
    // TODO_STUDENT
    BSTNode* result_node = find_recursion(root, nullptr, key);

    if (result_node == nullptr) {
      throw out_of_range("Key not found");
    }
    return result_node->value;
  }

  /**
   * Returns `true` if the key is present in the map, and false otherwise.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  bool contains(const KeyT& key) const {
    // TODO_STUDENT
    return find_recursion(root, nullptr, key) != nullptr;
  }

  /**
   * Empties the `BSTMap`, freeing all memory it controls.
   *
   * Runs in O(N), where N is the number of mappings.
   */
  void clear() {
    // TODO_STUDENT
    clear_recursion(root);
    root = nullptr;
    curr = nullptr;
    sz = 0;
  }

  /**
   * Destructor, cleans up the `BSTMap`.
   *
   * Runs in O(N), where N is the number of mappings.
   */
  ~BSTMap() {
    // TODO_STUDENT
    clear();
  }

  /**
   * Converts the `BSTMap` to a string representation, with the values
   * in-order by key.
   *
   * Example:
   *
   * ```c++
   * BSTMap<string, int> names;
   * names.insert("Gwen", 10);
   * names.insert("Jen", 5);
   * names.insert("Ben", 10);
   * names.insert("Sven", 7);
   * ```
   *
   * Calling `names.to_string()` would return the following **multi-line**
   * string:
   *
   * ```text
   * Ben: 10
   * Gwen: 10
   * Jen: 5
   * Sven: 7
   * ```
   *
   * Runs in O(N), where N is the number of mappings.
   */
  string to_string() const {
    // TODO_STUDENT
    stringstream ss;

    inOrder_string(root, ss);
    return ss.str();
  }

  /**
   * Copy constructor.
   *
   * Copies the mappings from the provided `BSTMap`.
   * The internal tree structure must be copied exactly.
   *
   * Runs in O(N), where N is the number of mappings in `other`.
   */
  BSTMap(const BSTMap& other) {
    // TODO_STUDENT
    this->sz = other.sz;
    this->curr = nullptr;

    this->root = copy_recursion(other.root, nullptr);
  }

  /**
   * Assignment operator; `operator=`.
   *
   * Clears this tree, and copies the mappings from the provided `BSTMap`.
   * The internal tree structure must be copied exactly.
   *
   * Runs in O(N + O), where N is the number of mappings in `this`, and O is
   * the number of mappings in `other`.
   */
  BSTMap& operator=(const BSTMap& other) {
    // TODO_STUDENT
    if (this == &other) {
      return *this;
    }

    this->clear();
    this->root = copy_recursion(other.root, nullptr);
    this->sz = other.sz;
    this->curr = nullptr;

    return *this;
  }

  // =========================================================================

  /**
   * Locates the smallest key in the BSTMap, and removes and returns its
   * mapping. If the `BSTMap` is empty, throws a `runtime_error`.
   *
   * Creates no new nodes, and does not update the key or value of any
   * existing nodes.
   *
   * Runs in O(H), where H is the height of the tree.
   */
  pair<KeyT, ValT> remove_min() {
    // TODO_STUDENT
    if (root == nullptr) {
      throw runtime_error("error");
    }

    BSTNode* min_node = root;
    while (min_node->left != nullptr) {
      min_node = min_node->left;
    }

    KeyT min_key = min_node->key;
    ValT min_value = min_node->value;

    root = remove_min_node(root);
    if (root != nullptr) {
      root->parent = nullptr;
    }

    sz--;
    curr = nullptr;

    return {min_key, min_value};
  }

  /**
   * Checks if the contents of `this` and `other` are equivalent.
   *
   * Two `BSTMap` objects are equivalent if they contain the same
   * mappings from key to value. To be equivalent, the two `BSTMap`
   * objects need not have the same internal tree structure.
   *
   *
   * Two different approaches could be used here.
   *
   * Approach #1:
   * Runs in worst-case O(N) time, using worst-case O(N) heap space,
   * where N is the maximum number of nodes in either of the `BSTMap`
   * objects. Since you cannot include other C++ classes, the extra
   * space in this approach would have to be used with arrays or
   * with `string` objects.
   *
   * Approach #2:
   * Runs in worst-case O(N*H) time, using worst-case O(1) space on
   * the heap, where N is the maximum number of nodes in either of
   * the `BSTMap` objects and H is the largest height between the two
   * `BSTMap` objects.
   *
   */
  bool operator==(const BSTMap& other) const {
    // TODO_STUDENT
    if (this->sz != other.sz) {
      return false;
    }
    return this->to_string() == other.to_string();
  }

  /**
   * Resets internal state for an iterative inorder traversal.
   *
   * See `next` for usage details. Modifies nothing except for `curr`.
   *
   * O(H), where H is the maximum height of the tree.
   */
  void begin() {
    // TODO_STUDENT
    curr = root;

    if (curr != nullptr) {
      while (curr->left != nullptr) {
        curr = curr->left;
      }
    }
  }

  /**
   * Uses the internal state to return the next in-order key and value
   * by reference, and advances the internal state. Returns `true` if the
   * reference parameters were set, and `false` otherwise.
   *
   * Example usage:
   *
   * ```c++
   * BSTMap<string, int> bst;
   * bst.begin();
   * string key;
   * int value;
   * while (bst.next(key, val)) {
   *   cout << key << ": " << val << endl;
   * }
   * ```
   *
   * Modifies nothing except for `curr`.
   *
   * Runs in worst-case O(H) where H is the height of the tree.
   */
  bool next(KeyT& key, ValT& val) {
    // TODO_STUDENT
    if (curr == nullptr) {
      return false;
    }

    BSTNode* node = curr;
    key = node->key;
    val = node->value;

    BSTNode* nextNode = nullptr;
    if (node->right != nullptr) {
      nextNode = node->right;
      while (nextNode->left != nullptr) {
        nextNode = nextNode->left;
      }
    } else {
      BSTNode* pNode = node->parent;
      while (pNode != nullptr && node == pNode->right) {
        node = pNode;
        pNode = pNode->parent;
      }
      nextNode = pNode;
    }
    curr = nextNode;
    return true;
  }

  // =========================================================================

  /**
   * Removes the mapping for the given key from the `BSTMap`, and returns the
   * value.
   *
   * Throws `out_of_range` if the key is not present in the map. Creates no new
   * nodes, and does not update the key or value of any existing nodes.
   *
   * Runs in worst-case O(H) where H is the height of the tree.
   */
  ValT erase(const KeyT& key) {
    // TODO_STUDENT
    if (root == nullptr) {
      throw out_of_range("error");
    }

    ValT removed_val{};
    root = erase_recursion(root, key, removed_val);

    if (root != nullptr) {
      root->parent = nullptr;
    }

    sz--;
    curr = nullptr;

    return removed_val;
  }

  // =========================================================================

  /**
   * Returns a pointer to the root node of the `BSTMap`. For autograder testing
   * purposes only.
   */
  void* getRoot() const {
    return this->root;
  }
};
