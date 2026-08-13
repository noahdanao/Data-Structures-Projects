#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

template <typename T>
class LinkedList {
 private:
  class Node {
   public:
    T data;
    Node* next;

    Node(T data) {
      this->data = data;
      this->next = nullptr;
    }

    Node(T data, Node* next) {
      this->data = data;
      this->next = next;
    }
  };

  size_t list_size;
  Node* list_front;

 public:
  /**
   * Default constructor. Creates an empty `LinkedList`.
   */
  LinkedList() {
    list_size = 0;
    list_front = nullptr;
  }

  /**
   * Returns whether the `LinkedList` is empty (i.e. whether its
   * size is 0).
   */
  bool empty() const {
    return list_size == 0;
  }

  /**
   * Returns the number of elements in the `LinkedList`.
   */
  size_t size() const {
    return list_size;
  }

  /**
   * Adds the given `T` to the front of the `LinkedList`.
   */
  void push_front(T data) {
    list_front = new Node(data, list_front);
    list_size++;
  }

  /**
   * Adds the given `T` to the back of the `LinkedList`.
   */
  void push_back(T data) {
    Node* newNode = new Node(data);
    if (empty()) {
      list_front = newNode;
    } else {
      Node* curr = list_front;
      while (curr->next != nullptr) {
        curr = curr->next;
      }
      curr->next = newNode;
    }
    list_size++;
  }

  /**
   * Removes the element at the front of the `LinkedList`.
   *
   * If the `LinkedList` is empty, throws a `runtime_error`.
   */
  T pop_front() {
    if (empty()) {
      throw runtime_error("pop_front on empty list");
    }
    Node* temp = list_front;
    T value = temp->data;
    list_front = list_front->next;
    delete temp;
    list_size--;

    return value;
  }

  /**
   * Removes the element at the back of the `LinkedList`.
   *
   * If the `LinkedList` is empty, throws a `runtime_error`.
   */
  T pop_back() {
    if (empty()) {
      throw runtime_error("pop_back on empty list");
    }
    if (list_size == 1) {
      T value = list_front->data;
      delete list_front;
      list_front = nullptr;
      list_size = 0;
      return value;
    }
    Node* curr = list_front;
    while (curr->next->next != nullptr) {
      curr = curr->next;
    }

    T value = curr->next->data;
    delete curr->next;
    curr->next = nullptr;
    list_size--;
    return value;
  }

  /**
   * Empties the `LinkedList`, releasing all allocated memory, and resetting
   * member variables appropriately.
   */
  void clear() {
    Node* curr = list_front;
    while (curr != nullptr) {
      Node* temp = curr;
      curr = curr->next;
      delete temp;
    }
    list_front = nullptr;
    list_size = 0;
  }

  /**
   * Destructor. Clears all allocated memory.
   */
  ~LinkedList() {
    clear();
  }

  /**
   * Returns the element at the given index in the `LinkedList`.
   *
   * If the index is invalid, throws `out_of_range`.
   */
  T& at(size_t index) const {
    if (index >= list_size) {
      throw out_of_range("Invalid index");
    }
    Node* curr = list_front;
    for (size_t i = 0; i < index; i++) {
      curr = curr->next;
    }
    return curr->data;
  }

  /**
   * Copy constructor. Creates a deep copy of the given `LinkedList`.
   *
   * Must run in O(N) time.
   */
  LinkedList(const LinkedList& other) {
    list_front = nullptr;
    list_size = 0;
    if (other.list_front == nullptr) {
      return;
    }

    list_front = new Node(other.list_front->data);
    list_size = 1;
    Node* currThis = list_front;
    Node* currOther = other.list_front->next;

    while (currOther != nullptr) {
      currThis->next = new Node(currOther->data);
      currThis = currThis->next;
      currOther = currOther->next;
      list_size++;
    }
  }

  /**
   * Assignment operator. Sets the current `LinkedList` to a deep copy of the
   * given `LinkedList`.
   *
   * Must run in O(N) time.
   */
  LinkedList& operator=(const LinkedList& other) {
    if (this == &other) {
      return *this;
    }
    clear();

    if (other.list_front == nullptr) {
      list_front = nullptr;
      list_size = 0;
      return *this;
    }

    list_front = new Node(other.list_front->data);
    list_size = 1;
    Node* currThis = list_front;
    Node* currOther = other.list_front->next;

    while (currOther != nullptr) {
      currThis->next = new Node(currOther->data);
      currThis = currThis->next;
      currOther = currOther->next;
      list_size++;
    }

    return *this;
  }

  /**
   * Converts the `LinkedList` to a string. Formatted like `[0, 1, 2, 3, 4]`
   * (without the backticks -- hover the function name to see). Runs in O(N)
   * time.
   */
  string to_string() const {
    stringstream ss;
    ss << "[";
    Node* curr = list_front;

    while (curr != nullptr) {
      ss << curr->data;
      if (curr->next != nullptr) {
        ss << ", ";
      }
      curr = curr->next;
    }
    ss << "]";
    return ss.str();
  }

  /**
   * Searches the `LinkedList` for the first matching element, and returns its
   * index. If no match is found, returns "-1".
   */
  size_t find(const T& data) {
    Node* curr = list_front;
    size_t index = 0;

    while (curr != nullptr) {
      if (curr->data == data) {
        return index;
      }
      curr = curr->next;
      index++;
    }
    return (size_t)-1;
  }

  /**
   * Remove the element at the specified index in this list.
   *
   * If the index is invalid, throws `out_of_range`.
   */
  void remove_at(size_t index) {
    if (index >= list_size) {
      throw out_of_range("Invalid index at remove_at");
    }

    if (index == 0) {
      Node* temp = list_front;
      list_front = list_front->next;
      delete temp;
    } else {
      Node* curr = list_front;
      for (size_t i = 0; i < index - 1; i++) {
        curr = curr->next;
      }
      Node* temp = curr->next;
      curr->next = temp->next;
      delete temp;
    }

    list_size--;
    if (list_size == 0) {
      list_front = nullptr;
    }
  }

  /**
   * Inserts the given `T` as a new element in the `LinkedList` after
   * the given index. If the index is invalid, throws `out_of_range`.
   */
  void insert_after(size_t index, T data) {
  }

  /**
   * Remove every element that is currently in an
   * even-numbered position on the list.
   *
   * Must run in O(N).
   *
   * For example, if a list was `[3, 9, 7, 6, 8]`, remove_evens
   * would change the list to `[9, 6]`, since we remove the
   * values in positions 0, 2, and 4 (the even-numbered indices)
   * from the list.
   */
  void remove_evens() {
  }

  /**
   * Returns a pointer to the node at the front of the `LinkedList`. For
   * autograder testing purposes only.
   */
  void* front() const {
    return this->list_front;
  }
};