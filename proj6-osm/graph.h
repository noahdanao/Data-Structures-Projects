#pragma once

#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

/// @brief Simple directed graph using an adjacency list.
/// @tparam VertexT vertex type
/// @tparam WeightT edge weight type
template <typename VertexT, typename WeightT>
class graph {
 private:
  // TODO_STUDENT
  unordered_map<VertexT, unordered_map<VertexT, WeightT>> adjList;
  size_t num_Edges;

 public:
  /// Default constructor
  graph() {
    // TODO_STUDENT
    num_Edges = 0; 
  }

  /// @brief Add the vertex `v` to the graph, must typically be O(1).
  /// @param v
  /// @return true if successfully added; false if it existed already
  bool addVertex(VertexT v) {
    // TODO_STUDENT
    auto[it,inserted] = adjList.emplace(v, unordered_map<VertexT, WeightT>{});
    return inserted;
  }

  /// @brief Add or overwrite directed edge in the graph, must typically be
  /// O(1).
  /// @param from starting vertex
  /// @param to ending vertex
  /// @param weight edge weight / label
  /// @return true if successfully added or overwritten;
  ///         false if either vertices isn't in graph
  bool addEdge(VertexT from, VertexT to, WeightT weight) {
    // TODO_STUDENT
    if(adjList.count(from) == 0 || adjList.count(to) == 0){
      return false;
    }
    bool overWrite = adjList[from].count(to);
    adjList[from][to] = weight;

    if(!overWrite){
      num_Edges++;
    }
    return true;
  }

  /// @brief Maybe get the weight associated with a given edge, must typically
  /// be O(1).
  /// @param from starting vertex
  /// @param to ending vertex
  /// @param weight output parameter
  /// @return true if the edge exists, and `weight` is set;
  ///         false if the edge does not exist
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const {
    // TODO_STUDENT
    auto fromNeighbor = adjList.find(from);
    if(fromNeighbor == adjList.end()){
      return false;
    } 
    auto toNeighbor = fromNeighbor->second.find(to);

    if(toNeighbor == fromNeighbor->second.end()){
      return false;
    }
    weight = toNeighbor->second;
    return true;
  }

  /// @brief Get the out-neighbors of `v`. Must run in at most O(|V|).
  /// @param v
  /// @return vertices that v has an edge to
  set<VertexT> neighbors(VertexT v) const {
    set<VertexT> S;
    // TODO_STUDENT
    auto fromNeighbor = adjList.find(v);
    if(fromNeighbor != adjList.end()){
      for(const auto& pair : fromNeighbor->second){
        S.insert(pair.first);
      }
    }
    return S;
  }

  /// @brief Return a vector containing all vertices in the graph
  vector<VertexT> getVertices() const {
    // TODO_STUDENT
    vector<VertexT> V;
    for(const auto& pair : adjList){
      V.push_back(pair.first);
    }
    return V;
  }

  /// @brief Get the number of vertices in the graph. Runs in O(1).
  size_t numVertices() const {
    // TODO_STUDENT
    return adjList.size();
  }

  /// @brief Get the number of directed edges in the graph. Runs in at most
  /// O(|V|), but should be O(1).
  size_t numEdges() const {
    // TODO_STUDENT
    return num_Edges;
  }
};
