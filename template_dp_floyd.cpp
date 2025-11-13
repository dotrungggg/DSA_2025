//============================================================================
// Purpose: A template for students to complete the Floyd-Warshall algorithm.
// Based on: 10b_FloydWarshall.pdf
//============================================================================

#include <iostream>
#include <vector>
#include <queue>
#include <stdexcept>
#include <limits>   // Required for std::numeric_limits
#include <algorithm>
#include <iomanip>  // For std::setw

// Constant for infinity, used to represent non-existent paths
const int INF = std::numeric_limits<int>::max();

//-------------------- GRAPH BASE CLASS --------------------
// Abstract base class for a graph
class Graph
{
protected:
    int numVertices;
    bool directed;

public:
    Graph(int vertices, bool isDirected = false) : numVertices(vertices), directed(isDirected) {}
    virtual ~Graph() {}

    int getNumVertices() const
    {
        return numVertices;
    }

    // Pure virtual functions - to be implemented by derived classes
    virtual void addEdge(int source, int destination, int weight) = 0;
    virtual bool hasEdge(int source, int destination) const = 0;
    virtual int getWeight(int source, int destination) const = 0;
    virtual std::vector<std::pair<int, int>> getNeighbors(int vertex) const = 0;
    virtual void printGraph() const = 0;
};

//-------------------- ADJACENCY LIST IMPLEMENTATION --------------------
// Adjacency List implementation of a graph.
// (Provided to flexibly initialize graphs for test cases)
class AdjacencyListGraph : public Graph
{
private:
    std::vector<std::vector<std::pair<int, int>>> adjacencyList;

public:
    AdjacencyListGraph(int vertices, bool isDirected = false) : Graph(vertices, isDirected)
    {
        // TODO: Initialize the adjacency list.
        adjacencyList.resize(numVertices);
    }

    void addEdge(int source, int destination, int weight) override
    {
        // TODO: Add a weighted edge.
        if (source < 0 || source >= numVertices || destination < 0 || destination >= numVertices)
        {
            throw std::out_of_range("Vertex index out of range in addEdge.");
        }

        adjacencyList[source].push_back({destination, weight});

        if (!directed)
        {
            adjacencyList[destination].push_back({source, weight});
        }
    }

    bool hasEdge(int source, int destination) const override
    {
        // TODO: Check if an edge exists.
        if (source < 0 || source >= numVertices)
        {
            return false;
        }
        
        for (const auto& neighbor : adjacencyList[source])
        {
            if (neighbor.first == destination)
            {
                return true;
            }
        }

        return false;
    }

    int getWeight(int source, int destination) const override
    {
        // TODO: Get the weight of an edge.
        if (source > 0 || source >= numVertices)
        {
            return INF;
        }

        for (const auto& neighbor : adjacencyList[source])
        {
            if (neighbor.first == destination)
            {
                return neighbor.second;
            }
        }

        return INF;
    }

    std::vector<std::pair<int, int>> getNeighbors(int vertex) const override
    {
        // TODO: Return the list of neighbors for a given vertex.
        if (vertex < 0 || vertex >= numVertices)
        {
            throw std::out_of_range("Vertex out of range in getNeighbors.");
        }

        return adjacencyList[vertex];
    }

    void printGraph() const override
    {
        // TODO: Print the adjacency list to the console.
        for (int i = 0; i < numVertices; i++)
        {
            std::cout << i << "->";

            if (adjacencyList[i].empty())
            {
                std::cout << "(none)";
            }
            else 
            {
                for (const auto& neighbor : adjacencyList[i])
                {
                    std::cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
                }
            }

            std::cout << std::endl;
        }
    }
};

//-------------------- GRAPH ALGORITHMS --------------------
class GraphAlgorithms
{
public:
    /**
     * Floyd-Warshall Algorithm (All-Pairs, handles negative weights)
     * Returns `true` if no negative cycle is detected.
     * Returns `false` if a negative cycle is found.
     * The `distances` matrix will be populated with the shortest path results.
     */
    static bool floydWarshall(const Graph &graph, std::vector<std::vector<int>> &distances)
    {
        int n = graph.getNumVertices();

        // TODO: Implement the Floyd-Warshall algorithm.
        //
        // 1. Initialize the `distances` matrix based on the graph's edges.
        //    - dist[i][i] = 0
        //    - dist[i][j] = weight(i, j) if edge (i,j) exists
        //    - dist[i][j] = INF otherwise
        //
        // 2. Implement the main triple loop (k, i, j) for dynamic programming.
        //    - k is the intermediate vertex.
        //    - i is the source vertex.
        //    - j is the destination vertex.
        //
        // 3. Apply the relaxation formula:
        //    distances[i][j] = min(distances[i][j], distances[i][k] + distances[k][j])
        //
        // 4. Check for negative-weight cycles.
        //    - If distances[i][i] < 0 for any i, a negative cycle exists.
        //
        // 5. Return `false` if a negative cycle is found, otherwise return `true`.

        distances.resize(n, std::vector<int>(n));

        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (i == j)
                {
                    distances[i][j] = 0;
                }
                else
                {
                    distances[i][j] = graph.getWeight(i, j);
                }
            }
        }

        for (int k = 0; k < n; ++k)
        {
            for (int i = 0; i < n; ++i)
            {
                for (int j = 0; j < n; ++j)
                {
                    if (distances[i][k] != INF && distances[k][j] != INF)
                    {
                        distances[i][j] = std::min(distances[i][j], distances[i][k] + distances[k][j]);
                    }
                }
            }
        }

        for (int i = 0; i < n; ++i)
        {
            if (distances[i][i] < 0 )
            {
                return false;
            }
        }

        return true;
    }
};

//-------------------- MAIN FUNCTION FOR TESTING --------------------
//============== DO NOT MODIFY THIS SECTION ==============//

void printAllPairsDistances(const std::vector<std::vector<int>>& distances) {
    std::cout << "\n[Floyd-Warshall] All-Pairs Shortest Path Matrix:\n";
    int n = distances.size();
    
    // Print header row
    std::cout << "   |";
    for (int j = 0; j < n; ++j) {
        std::cout << std::setw(4) << j;
    }
    std::cout << "\n---|";
    for (int j = 0; j < n; ++j) {
        std::cout << "----";
    }
    std::cout << "\n";

    // Print matrix content
    for (int i = 0; i < n; ++i) {
        std::cout << std::setw(3) << i << "|";
        for (int j = 0; j < n; ++j) {
            if (distances[i][j] == INF) {
                std::cout << std::setw(4) << "INF";
            } else {
                std::cout << std::setw(4) << distances[i][j];
            }
        }
        std::cout << "\n";
    }
}


int main()
{
    // --- Test Case 1: Floyd-Warshall (no negative cycle) ---
    std::cout << "\n======= TEST CASE 1: FLOYD-WARSHALL (NO NEGATIVE CYCLE) =======\n";
    AdjacencyListGraph graph1(5, true);
    graph1.addEdge(0, 1, -1);
    graph1.addEdge(0, 2, 4);
    graph1.addEdge(1, 2, 3);
    graph1.addEdge(1, 3, 2);
    graph1.addEdge(1, 4, 2);
    graph1.addEdge(3, 2, 5);
    graph1.addEdge(3, 1, 1);
    graph1.addEdge(4, 3, -3);

    std::cout << "Graph for Test Case 1:\n";
    graph1.printGraph();

    std::vector<std::vector<int>> fwDistances;
    if (GraphAlgorithms::floydWarshall(graph1, fwDistances)) {
        printAllPairsDistances(fwDistances);
        std::cout << "Expected Matrix (example row): 0 -> {0, -1, 2, -2, 1}\n";
        std::cout << "Expected Matrix (example row): 4 -> {INF, -2, 1, -3, 0}\n";
    } else {
        std::cout << "\n[Floyd-Warshall] Error: Detected a negative cycle when none was expected.\n";
    }
    std::cout << "--------------------------------------------------\n";

    // --- Test Case 2: Floyd-Warshall (with negative cycle) ---
    std::cout << "\n======= TEST CASE 2: FLOYD-WARSHALL (WITH NEGATIVE CYCLE) =======\n";
    AdjacencyListGraph negCycleGraph(4, true);
    negCycleGraph.addEdge(0, 1, 1);
    negCycleGraph.addEdge(1, 2, -2);
    negCycleGraph.addEdge(2, 3, 3);
    negCycleGraph.addEdge(3, 1, -2); // Negative cycle: 1 -> 2 -> 3 -> 1 (total weight -1)

    std::cout << "Graph for Negative Cycle test:\n";
    negCycleGraph.printGraph();

    std::vector<std::vector<int>> fwNegDistances;
    if (GraphAlgorithms::floydWarshall(negCycleGraph, fwNegDistances)) {
        std::cout << "\n[Floyd-Warshall] Error: Failed to detect a negative cycle.\n";
    } else {
        std::cout << "\n[Floyd-Warshall] Success: Graph contains a negative weight cycle as expected.\n";
    }
    std::cout << "--------------------------------------------------\n";

    return 0;
}
