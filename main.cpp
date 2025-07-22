/*
-------------------------------------------
Project 5: OpenStreetMaps using Graphs
Course: CS 251,Summer 2024, UIC
System: Advanced zyLab
Author: Scott Reckinger
-------------------------------------------
*/


/*main.cpp*/

//
// Prof. Scott Reckinger
// University of Illinois Chicago
// CS 251
// Project #05: open street maps, graphs, and Dijkstra's alg
// 
// References:
// TinyXML: https://github.com/leethomason/tinyxml2
// OpenStreetMap: https://www.openstreetmap.org
// OpenStreetMap docs:  
//   https://wiki.openstreetmap.org/wiki/Main_Page
//   https://wiki.openstreetmap.org/wiki/Map_Features
//   https://wiki.openstreetmap.org/wiki/Node
//   https://wiki.openstreetmap.org/wiki/Way
//   https://wiki.openstreetmap.org/wiki/Relation
//

#include <iostream>
#include <iomanip>  /*setprecision*/
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <stack>
#include <queue>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <limits>

#include "tinyxml2.h"
#include "graph.h"
#include "dist.h"
#include "osm.h"

using namespace std;
using namespace tinyxml2;

double INF = numeric_limits<double>::max(); 




//////////////////////////////////////////////////////////////////
//
// main
//
int main()
{
  map<long long, Coordinates>  Nodes;     // maps a Node ID to it's coordinates (lat, lon)
  vector<FootwayInfo>          Footways;  // info about each footway, in no particular order
  vector<BuildingInfo>         Buildings; // info about each building, in no particular order
  XMLDocument                  xmldoc;
  
  cout << "** Navigating UIC open street map **" << endl;
  cout << endl;
  cout << std::setprecision(8);

  string def_filename = "map.osm";
  string filename;

  cout << "Enter map filename> ";
  getline(cin, filename);

  if (filename == "")
  {
    filename = def_filename;
  }

  //
  // 1. Load XML-based map file 
  //
  if (!LoadOpenStreetMap(filename, xmldoc))
  {
    cout << "**Error: unable to load open street map." << endl;
    cout << endl;
    return 0;
  }
  
  //
  // 2. Read the nodes, which are the various known positions on the map:
  //
  int nodeCount = ReadMapNodes(xmldoc, Nodes);

  //
  // 3. Read the footways, which are the walking paths:
  //
  int footwayCount = ReadFootways(xmldoc, Footways);

  //
  // 4. Read the university buildings:
  //
  int buildingCount = ReadUniversityBuildings(xmldoc, Nodes, Buildings);

  //
  // Stats
  //
  assert(nodeCount == (int)Nodes.size());
  assert(footwayCount == (int)Footways.size());
  assert(buildingCount == (int)Buildings.size());

  cout << endl;
  cout << "# of nodes: " << Nodes.size() << endl;
  cout << "# of footways: " << Footways.size() << endl;
  cout << "# of buildings: " << Buildings.size() << endl;


  //
  // TODO - 5&6: build the graph, output stats:
  //

    // 5. Build the graph, output stats:
  graph<long long, double> G;
  
  // Add nodes as vertices
  for (const auto& node : Nodes)
  {
    G.addVertex(node.first);
  }

  // Add edges based on footways
  for (const auto& footway : Footways)
  {
    for (size_t i = 0; i < footway.Nodes.size() - 1; ++i)
    {
      long long from = footway.Nodes[i];
      long long to = footway.Nodes[i + 1];
      double distance = distBetween2Points(Nodes[from].Lat, Nodes[from].Lon, Nodes[to].Lat, Nodes[to].Lon);
      G.addEdge(from, to, distance);
      G.addEdge(to, from, distance);
    }
  }

  cout << "# of vertices: " << G.NumVertices() << endl;
  cout << "# of edges: " << G.NumEdges() << endl;
  cout << endl;



  //
  // Navigation from building to building
  // 7. Input start and dest buildings
  //
  string startBuilding, destBuilding;

  cout << "Enter start (partial name or abbreviation), or #> ";
  getline(cin, startBuilding);

  while (startBuilding != "#") 
  {
    cout << "Enter destination (partial name or abbreviation)> ";
    getline(cin, destBuilding);


    //
    // TODO -  7. lookup buildings, 
    //         8. find nearest start and dest nodes,
    //         9. run Dijkstra's alg, 
    //         10. output distance and path to destination:
    //

    

    //cout << "Start building not found" << endl;
    //cout << "Destination building not found" << endl;

// 7. Lookup buildings
    auto findBuilding = [&](const string& name) -> pair<bool, BuildingInfo> {
      for (const auto& building : Buildings)
      {
        if (building.Abbrev == name || building.Fullname.find(name) != string::npos)
        {
          return {true, building};
        }
      }
      return {false, {}};
    };

    auto start = findBuilding(startBuilding);
    auto dest = findBuilding(destBuilding);

    if (!start.first)
    {
      cout << "Start building not found" << endl;
      cout << "Enter start (partial name or abbreviation), or #> ";
      getline(cin, startBuilding);
      continue;
    }

    if (!dest.first)
    {
      cout << "Destination building not found" << endl;
      cout << "Enter start (partial name or abbreviation), or #> ";
      getline(cin, startBuilding);
      continue;
    }

    // 8. Find nearest start and dest nodes
    auto findNearestNode = [&](const Coordinates& coords) -> long long {
      double minDist = numeric_limits<double>::max();
      long long nearestNode = -1;
      
      for (const auto& footway : Footways)
      {
        for (const auto& node : footway.Nodes)
        {
          double distance = distBetween2Points(coords.Lat, coords.Lon, Nodes[node].Lat, Nodes[node].Lon);
          if (distance < minDist)
          {
            minDist = distance;
            nearestNode = node;
          }
        }
      }

      return nearestNode;
    };

    long long startNode = findNearestNode(start.second.Coords);
    long long destNode = findNearestNode(dest.second.Coords);

    cout << "Starting point:" << endl;
    cout << " "<< start.second.Fullname << endl <<" (" << start.second.Coords.Lat << ", " << start.second.Coords.Lon << ")" << endl;
    cout << "Destination point:" << endl;
    cout << " "<< dest.second.Fullname << endl << " (" << dest.second.Coords.Lat << ", " << dest.second.Coords.Lon << ")" << endl;

cout<<endl;

cout << "Nearest start node:" << endl;
cout << " "<< startNode << endl << " (" << Nodes[startNode].Lat << ", " << Nodes[startNode].Lon << ")" << endl;
cout << "Nearest destination node:" << endl;
cout << " "<< destNode << endl << " (" << Nodes[destNode].Lat << ", " << Nodes[destNode].Lon << ")" << endl;

cout<<endl;

cout << "Navigating with Dijkstra..." << endl;
     // 9. Run Dijkstra's algorithm
    map<long long, double> distances;
    map<long long, long long> predecessors;
    G.Dijkstra(startNode, distances, predecessors);




     // 10. Output distance and path to destination
if (distances.find(destNode) == distances.end() || distances[destNode] == numeric_limits<double>::max()) {
    cout << "Sorry, destination unreachable" << endl;
}
else
{
    double distance = distances[destNode];
    cout << "Distance to dest: " << distance << " miles" << endl;

    stack<long long> path;
    for (long long at = destNode; at != startNode; at = predecessors[at])
    {
        path.push(at);
    }
    path.push(startNode);

    cout << "Path: ";
    while (!path.empty())
    {
        cout << path.top();
        path.pop();
        if (!path.empty())
        {
            cout << "->";
        }
    }
    cout << endl;
}
   
    //
    // another navigation?
    //
    cout << endl;
    cout << "Enter start (partial name or abbreviation), or #> ";
    getline(cin, startBuilding);
  } // 11. repeat if start building is not #

  //
  // done:
  //
  cout << "** Done **" << endl;

  return 0;
}
