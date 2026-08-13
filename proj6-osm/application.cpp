#include "application.h"

#include <iostream>
#include <limits>
#include <map>
#include <queue> // priority_queue
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "dist.h"
#include "graph.h"
#include "json.hpp"


using namespace std;
using json = nlohmann::json;

double INF = numeric_limits<double>::max();

void buildGraph(istream &input, graph<long long, double> &g,
                vector<BuildingInfo> &buildings,
                unordered_map<long long, Coordinates> &coords) {
  // TODO_STUDENT
  json data;
  input >> data; 

  if(data.contains("buildings") && data["buildings"].is_array()){

    for(auto &b : data["buildings"]){
      long long id = b["id"];
      double lat = b["lat"];
      double lon = b["lon"];
      string name = b["name"];
      string abbr = b["abbr"];

      Coordinates c(lat,lon);

      buildings.emplace_back(id,c,name,abbr);
      g.addVertex(id);
    }
  }

  if(data.contains("waypoints") && data["waypoints"].is_array()){

    for(auto &w : data["waypoints"]){
      long long id = w["id"];
      double lat = w["lat"];
      double lon = w["lon"];

      Coordinates c(lat,lon);
      coords[id] = c;

      g.addVertex(id);
    }
  }

  if(data.contains("footways") && data["footways"].is_array()){

    for(auto &fw : data["footways"]){
      auto &nodes = fw;

      for(size_t i=0; i+1<nodes.size(); i++){
        long long a = nodes[i];
        long long b = nodes[i+1];

        double dist = distBetween2Points(coords[a],coords[b]);

        g.addEdge(a,b,dist);
        g.addEdge(b,a,dist);
      }
    }
  }

  const double maxDist = 0.036;

  for(const BuildingInfo &b : buildings){
    long long bid = b.id;
    const Coordinates &bc = b.location;

    for(const auto &p : coords){
      long long id = p.first;

      if(id == bid){
        continue;
      }

      double dist = distBetween2Points(bc,p.second);

      if(dist <= maxDist){
        g.addEdge(bid,id,dist);
        g.addEdge(id,bid,dist);
      }
    }
  }

}

BuildingInfo getBuildingInfo(const vector<BuildingInfo> &buildings,
                             const string &query) {
  for (const BuildingInfo &building : buildings) {
    if (building.abbr == query) {
      return building;
    } else if (building.name.find(query) != string::npos) {
      return building;
    }
  }
  BuildingInfo fail;
  fail.id = -1;
  return fail;
}

BuildingInfo getClosestBuilding(const vector<BuildingInfo> &buildings,
                                Coordinates c) {
  double minDestDist = INF;
  BuildingInfo ret = buildings.at(0);
  for (const BuildingInfo &building : buildings) {
    double dist = distBetween2Points(building.location, c);
    if (dist < minDestDist) {
      minDestDist = dist;
      ret = building;
    }
  }
  return ret;
}

vector<long long> dijkstra(const graph<long long, double> &G, long long start,
                           long long target,
                           const set<long long> &ignoreNodes) {
  if(start == target){
    return vector<long long>{start};
  }

  unordered_map<long long, double> dist;
  unordered_map<long long, long long> prev;

  for(long long v : G.getVertices()){
    dist[v] = INF;
  }

  dist[start] = 0.0;

  priority_queue<pair<double, long long>,vector<pair<double, long long>>, greater<pair<double, long long>>>worklist;
  worklist.push({0.0,start});

  while(!worklist.empty()){
    auto[d,u] = worklist.top();
    worklist.pop();

    if(d != dist[u]){
      continue;
    }

    if(u == target){
      break;
    }

    for(long long v : G.neighbors(u)){
      if(ignoreNodes.count(v) && v != start && v != target){
        continue;
      }

      double w;
      G.getWeight(u,v,w);
      double newDist = dist[u] + w;

      if(newDist < dist[v]){
        dist[v] = newDist;
        prev[v] = u;
        worklist.push({newDist,v});
      }
    }
  }

  if(dist[target] == INF){
    return vector<long long>{};
  }

  vector<long long> path;
  long long curr = target;

  while(curr != start){
    path.push_back(curr);
    curr = prev[curr];
  }

  path.push_back(start);
  reverse(path.begin(),path.end());


  return path;
}

double pathLength(const graph<long long, double> &G,
                  const vector<long long> &path) {
  double length = 0.0;
  double weight;
  for (size_t i = 0; i + 1 < path.size(); i++) {
    bool res = G.getWeight(path.at(i), path.at(i + 1), weight);
    if (!res) {
      return -1;
    }
    length += weight;
  }
  return length;
}

void outputPath(const vector<long long> &path) {
  for (size_t i = 0; i < path.size(); i++) {
    cout << path.at(i);
    if (i != path.size() - 1) {
      cout << "->";
    }
  }
  cout << endl;
}

// Honestly this function is just a holdover from an old version of the project
void application(const vector<BuildingInfo> &buildings,
                 const graph<long long, double> &G) {
  string person1Building, person2Building;

  set<long long> buildingNodes;
  for (const auto &building : buildings) {
    buildingNodes.insert(building.id);
  }

  cout << endl;
  cout << "Enter person 1's building (partial name or abbreviation), or #> ";
  getline(cin, person1Building);

  while (person1Building != "#") {
    cout << "Enter person 2's building (partial name or abbreviation)> ";
    getline(cin, person2Building);

    // Look up buildings by query
    BuildingInfo p1 = getBuildingInfo(buildings, person1Building);
    BuildingInfo p2 = getBuildingInfo(buildings, person2Building);
    Coordinates P1Coords, P2Coords;
    string P1Name, P2Name;

    if (p1.id == -1) {
      cout << "Person 1's building not found" << endl;
    } else if (p2.id == -1) {
      cout << "Person 2's building not found" << endl;
    } else {
      cout << endl;
      cout << "Person 1's point:" << endl;
      cout << " " << p1.name << endl;
      cout << " " << p1.id << endl;
      cout << " (" << p1.location.lat << ", " << p1.location.lon << ")" << endl;
      cout << "Person 2's point:" << endl;
      cout << " " << p2.name << endl;
      cout << " " << p2.id << endl;
      cout << " (" << p2.location.lon << ", " << p2.location.lon << ")" << endl;

      Coordinates centerCoords = centerBetween2Points(p1.location, p2.location);
      BuildingInfo dest = getClosestBuilding(buildings, centerCoords);

      cout << "Destination Building:" << endl;
      cout << " " << dest.name << endl;
      cout << " " << dest.id << endl;
      cout << " (" << dest.location.lat << ", " << dest.location.lon << ")"
           << endl;

      vector<long long> P1Path = dijkstra(G, p1.id, dest.id, buildingNodes);
      vector<long long> P2Path = dijkstra(G, p2.id, dest.id, buildingNodes);

      // This should NEVER happen with how the graph is built
      if (P1Path.empty() || P2Path.empty()) {
        cout << endl;
        cout << "At least one person was unable to reach the destination "
                "building. Is an edge missing?"
             << endl;
        cout << endl;
      } else {
        cout << endl;
        cout << "Person 1's distance to dest: " << pathLength(G, P1Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P1Path);
        cout << endl;
        cout << "Person 2's distance to dest: " << pathLength(G, P2Path);
        cout << " miles" << endl;
        cout << "Path: ";
        outputPath(P2Path);
      }
    }

    //
    // another navigation?
    //
    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);
  }
}
