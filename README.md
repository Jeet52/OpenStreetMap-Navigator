# OpenStreetMaps using Graphs

## Overview
This project implements a graph-based navigation system for University of Illinois Chicago (UIC) campus map data using OpenStreetMap (OSM) files. The system parses OSM XML data to build a graph of nodes and footways (paths), allowing users to find shortest walking routes between campus buildings via Dijkstra’s algorithm.

---

## Features

- **Graph Data Structure:** Implements a generic weighted directed graph with adjacency list representation.
- **OSM Parsing:** Reads nodes (locations), footways (walkable paths), and buildings from OSM XML files using TinyXML2.
- **Graph Construction:** Vertices represent nodes; edges represent footways between nodes weighted by geographic distance.
- **Navigation:** Supports user input of building names (partial or abbreviations) to find start and destination points.
- **Shortest Path:** Uses Dijkstra’s algorithm to compute the shortest path (by distance) between buildings on the campus.
- **Output:** Displays distances and paths with detailed geographic coordinates.
- **Robust Input Handling:** Validates user input and handles missing or unreachable destinations.

---

## Technologies Used

- Language: C++
- XML Parsing: TinyXML2 library for parsing OSM XML files
- Data Structures: STL containers (`map`, `vector`, `set`, `stack`, `queue`)
- Algorithms: Dijkstra’s shortest path algorithm
- Coordinate System: Latitude/Longitude with Haversine formula for distance

---

## Code Structure

### `graph.h`
- Template class `graph<VertexT, WeightT>` implementing a weighted directed graph.
- Supports adding vertices and edges, retrieving neighbors, and querying edge weights.
- Implements Dijkstra’s algorithm to compute shortest paths.

### `main.cpp`
- Loads and parses OSM data using TinyXML2.
- Reads nodes, footways, and buildings, storing them in appropriate data structures.
- Builds a graph representing the campus map.
- Accepts user input for navigation between buildings.
- Finds nearest graph nodes for start/destination buildings.
- Runs Dijkstra’s algorithm and prints the shortest path and distance.
- Allows repeated queries until user exits.

---

## How to Build and Run

### Dependencies
- C++ compiler (supporting C++11 or newer)
- TinyXML2 library (https://github.com/leethomason/tinyxml2)

### Build
Assuming you have all source files (`main.cpp`, `graph.h`, `osm.h`, `dist.h`, and TinyXML2):

```bash
g++ -std=c++11 main.cpp tinyxml2.cpp -o openstreetmap_nav
```

### Run
```bash
./openstreetmap_nav
```

- When prompted, enter the OSM XML map filename (default `map.osm` if empty).
- Follow prompts to input partial or abbreviated building names.
- The program outputs the shortest walking distance and path between selected buildings.
- Enter `#` at the start prompt to exit.

---

## Sample Interaction

```
** Navigating UIC open street map **

Enter map filename> map.osm

# of nodes: 12345
# of footways: 678
# of buildings: 100

Enter start (partial name or abbreviation), or #> UIC Library
Enter destination (partial name or abbreviation)> Student Center

Starting point:
 UIC Library
 (41.870800, -87.650500)
Destination point:
 Student Center
 (41.871200, -87.649800)

Nearest start node:
 1234567
 (41.870900, -87.650600)
Nearest destination node:
 7654321
 (41.871300, -87.649900)

Navigating with Dijkstra...
Distance to dest: 0.25 miles
Path: 1234567->1234570->1234573->7654321

Enter start (partial name or abbreviation), or #> #
** Done **
```

---

## Author
CS 251, Summer 2024  
University of Illinois Chicago  
Author: Scott Reckinger
