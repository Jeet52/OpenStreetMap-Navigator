/*
-------------------------------------------
Project 5: OpenStreetMaps using Graphs
Course: CS 251,Summer 2024, UIC
System: Advanced zyLab
Author: Scott Reckinger
-------------------------------------------
*/

#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <map>
#include <limits>

using namespace std;

template<typename VertexT, typename WeightT>
class graph
{
private:
    // Adjacency list representation using map for efficient lookups
    map<VertexT, map<VertexT, WeightT>> AdjList;

    //
    // _LookupVertex
    //
    // Checks if the vertex exists in the graph.
    //
    bool _LookupVertex(VertexT v) const
    {
        return AdjList.find(v) != AdjList.end();
    }

public:
    //
    // constructor:
    //
    // Constructs an empty graph with no fixed size limit.
    //
    graph() {}

    //
    // NumVertices
    //
    // Returns the number of vertices currently in the graph.
    //
    int NumVertices() const
    {
        return static_cast<int>(AdjList.size());
    }

    //
    // NumEdges
    //
    // Returns the number of edges currently in the graph.
    //
    int NumEdges() const
    {
        int count = 0;
        for (const auto& pair : AdjList)
        {
            count += pair.second.size();
        }
        return count;
    }

    //
    // addVertex
    //
    // Adds the vertex v to the graph if it doesn't already exist, and returns true.
    // If the vertex already exists in the graph, then false is returned.
    //
    bool addVertex(VertexT v)
    {
        if (_LookupVertex(v))
            return false;

        // Insert the vertex with an empty adjacency map
        AdjList[v] = map<VertexT, WeightT>();
        return true;
    }

    //
    // addEdge
    //
    // Adds the edge (from, to, weight) to the graph, and returns true.
    // If the vertices do not exist, false is returned.
    // If the edge already exists, the existing edge weight is overwritten with the new edge weight.
    //
    bool addEdge(VertexT from, VertexT to, WeightT weight)
    {
        if (!_LookupVertex(from) || !_LookupVertex(to))
            return false;

        // Insert or update the edge weight
        AdjList[from][to] = weight;
        return true;
    }

    //
    // getWeight
    //
    // Returns the weight associated with a given edge. If the edge exists, the weight is returned via the reference
    // parameter and true is returned. If the edge does not exist, the weight parameter is unchanged and false is returned.
    //
    bool getWeight(VertexT from, VertexT to, WeightT& weight) const
    {
        if (!_LookupVertex(from) || !_LookupVertex(to))
            return false;

        auto it = AdjList.find(from);
        if (it != AdjList.end())
        {
            auto edgeIt = it->second.find(to);
            if (edgeIt != it->second.end())
            {
                weight = edgeIt->second;
                return true;
            }
        }
        return false;
    }

    //
    // neighbors
    //
    // Returns a set containing the neighbors of v, i.e. all vertices that can be reached from v along one edge.
    // Since a set is returned, the neighbors are returned in sorted order; use foreach to iterate through the set.
    //
    set<VertexT> neighbors(VertexT v) const
    {
        set<VertexT> S;
        if (!_LookupVertex(v))
            return S;

        for (const auto& edge : AdjList.at(v))
        {
            S.insert(edge.first);
        }
        return S;
    }

    //
    // getVertices
    //
    // Returns a vector containing all the vertices currently in the graph.
    //
    vector<VertexT> getVertices() const
    {
        vector<VertexT> vertices;
        for (const auto& pair : AdjList)
        {
            vertices.push_back(pair.first);
        }
        return vertices;
    }

    //
    // dump
    //
    // Dumps the internal state of the graph for debugging purposes.
    //
    void dump(ostream& output) const
    {
        output << "***************************************************" << endl;
        output << "********************* GRAPH ***********************" << endl;

        output << "**Num vertices: " << this->NumVertices() << endl;
        output << "**Num edges: " << this->NumEdges() << endl;

        output << endl;
        output << "**Vertices:" << endl;
        int i = 0;
        for (const auto& pair : AdjList)
        {
            output << " " << i++ << ". " << pair.first << endl;
        }

        output << endl;
        output << "**Edges:" << endl;
        for (const auto& pair : AdjList)
        {
            output << " " << pair.first << ": ";
            for (const auto& edge : pair.second)
            {
                output << "(" << pair.first << "," << edge.first << "," << edge.second << ") ";
            }
            output << endl;
        }

        output << "**************************************************" << endl;
    }

    void Dijkstra(VertexT startV, map<VertexT, WeightT>& distances, map<VertexT, VertexT>& predecessors) const
    {
        // Initialize distances and predecessors
        for (const auto& vertex : this->getVertices()) {
            distances[vertex] = numeric_limits<WeightT>::max();
            predecessors[vertex] = VertexT(); // Assuming default constructor indicates no predecessor
        }

        // Start vertex distance is 0
        distances[startV] = 0;

        // Priority queue to select the vertex with the smallest distance
        set<pair<WeightT, VertexT>> unvisitedQueue; // (distance, vertex)
        unvisitedQueue.insert(make_pair(0, startV));

        while (!unvisitedQueue.empty()) {
            // Visit vertex with minimum distance from startV
            VertexT currentV = unvisitedQueue.begin()->second;
            unvisitedQueue.erase(unvisitedQueue.begin());

            // Get neighbors of the current vertex
            for (const auto& neighbor : this->neighbors(currentV)) {
                WeightT edgeWeight;
                if (this->getWeight(currentV, neighbor, edgeWeight)) {
                    WeightT alternativePathDistance = distances[currentV] + edgeWeight;

                    // If a shorter path from startV to neighbor is found,
                    // update neighbor's distance and predecessor
                    if (alternativePathDistance < distances[neighbor]) {
                        // Remove the neighbor from the queue if it exists
                        unvisitedQueue.erase(make_pair(distances[neighbor], neighbor));
                        
                        // Update distance and predecessor
                        distances[neighbor] = alternativePathDistance;
                        predecessors[neighbor] = currentV;
                        
                        // Reinsert the neighbor with the updated distance
                        unvisitedQueue.insert(make_pair(alternativePathDistance, neighbor));
                    }
                }
            }
        }
    }
};
