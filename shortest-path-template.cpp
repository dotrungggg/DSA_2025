// Shortest Path Algorithms Implementation Template
// Instructions: Implement the missing function definitions for each class.
// Each function has comments indicating what it should do.

#include <iostream>
#include <vector>
#include <queue>
#include <stdexcept>
#include <limits>   // Required for std::numeric_limits
#include <algorithm>

// Constant for infinity, used to represent non-existent paths
const int INF = std::numeric_limits<int>::max();

//-------------------- GRAPH BASE CLASS --------------------
class Graph
{
protected:
    int numVertices;
    bool directed;

public:
    // Constructor
    Graph(int vertices, bool isDirected = false) : numVertices(vertices), directed(isDirected) {}

    // Virtual destructor
    virtual ~Graph() {}

    // Get the number of vertices
    int getNumVertices() const
    {
        return numVertices;
    }

    // Pure virtual functions - Must be implemented by derived classes
    virtual void addEdge(int source, int destination, int weight) = 0;
    virtual bool hasEdge(int source, int destination) const = 0;
    virtual int getWeight(int source, int destination) const = 0;
    // Returns a list of pairs {adjacent_vertex, weight}
    virtual std::vector<std::pair<int, int>> getNeighbors(int vertex) const = 0;
    virtual void printGraph() const = 0;
};

//-------------------- ADJACENCY MATRIX IMPLEMENTATION --------------------
class AdjacencyMatrixGraph : public Graph
{
private:
    std::vector<std::vector<int>> matrix;

public:
    AdjacencyMatrixGraph(int vertices, bool isDirected = false) : Graph(vertices, isDirected)
    {
        // TODO: Initialize the adjacency matrix.
        // Hint: Use `resize` to create a `numVertices` x `numVertices` matrix.
        // The initial value for non-existent edges should be INF.
        // The path from a vertex to itself has a weight of 0.
        matrix.resize(numVertices, std::vector<int>(numVertices, INF));

        for (int i = 0; i < numVertices; i++)
        {
            matrix[i][i] = 0;
        }
    }

    void addEdge(int source, int destination, int weight) override
    {
        // TODO: Add a weighted edge to the graph.
        // Hint: Set `matrix[source][destination] = weight`.
        // If the graph is undirected, you also need to set `matrix[destination][source]`.
        matrix[source][destination] = weight;

        if (!directed)
        {
            matrix[destination][source] = weight;
        }
    }

    bool hasEdge(int source, int destination) const override
    {
        // TODO: Check if an edge exists from source to destination.
        // Hint: An edge exists if the value in the matrix is not INF.
        return matrix[source][destination];
    }

    int getWeight(int source, int destination) const override
    {
        // TODO: Get the weight of the edge from source to destination.
        // Hint: Return the value from the matrix. If no edge exists, return INF.
        return matrix[source][destination];
    }

    std::vector<std::pair<int, int>> getNeighbors(int vertex) const override
    {
        std::vector<std::pair<int, int>> neighbors;
        // TODO: Return a vector containing pairs of {adjacent_vertex, weight}.
        // Hint: Iterate through the `vertex` row of the matrix. If a value is not INF,
        // it's a neighbor. Add {column_index, value} to the vector.
        for (int i = 0; i < numVertices; i++)
        {
            if (i != vertex && matrix[vertex][i] != INF)
            {
                neighbors.push_back({i, matrix[vertex][i]});
            }
        }

        return neighbors;
    }

    void printGraph() const override
    {
        // TODO: Print the adjacency matrix to the console for debugging.
        // Hint: Use nested loops to iterate through the matrix.
        // If the value is INF, you can print "INF" to make it more readable.
        std::cout << "Adjacency Matrix:\n";
        for (int i = 0; i < numVertices; i++)
        {
            for (int j = 0; j < numVertices; j++)
            {
                if (matrix[i][j] == INF)
                {
                    std::cout << "INF\t";
                }
                else {
                    std::cout << matrix[i][j] << "\t";
                }
            }
            std::cout << "\n";
        }
    }
};

//-------------------- ADJACENCY LIST IMPLEMENTATION --------------------
class AdjacencyListGraph : public Graph
{
private:
    // Each element of the vector is a list of pairs {destination_vertex, weight}
    std::vector<std::vector<std::pair<int, int>>> adjacencyList;

public:
    AdjacencyListGraph(int vertices, bool isDirected = false) : Graph(vertices, isDirected)
    {
        // TODO: Initialize the adjacency list.
        // Hint: Use `resize` to create `numVertices` empty lists.
        adjacencyList.resize(numVertices);
    }

    void addEdge(int source, int destination, int weight) override
    {
        // TODO: Add a weighted edge to the graph.
        // Hint: Add the pair {destination, weight} to `adjacencyList[source]`.
        // If the graph is undirected, do the same for `adjacencyList[destination]`.
        adjacencyList[source].push_back({destination, weight});

        if (!directed)
        {
            adjacencyList[destination].push_back({source, weight});
        }
    }

    bool hasEdge(int source, int destination) const override
    {
        // TODO: Check if an edge exists from source to destination.
        // Hint: Iterate through the adjacency list of the `source` vertex.
        for (const auto& edge : adjacencyList[source])
        {
            if (edge.first == destination)
            {
                return true;
            }
        }

        return false;
    }

    int getWeight(int source, int destination) const override
    {
        // TODO: Get the weight of the edge from source to destination.
        // Hint: Iterate through the `source` vertex's adjacency list, find `destination`, and return its weight.
        for (const auto& edge : adjacencyList[source])
        {
            if (edge.first == destination)
            {
                return edge.second;
            }
        }

        return INF;
    }

    std::vector<std::pair<int, int>> getNeighbors(int vertex) const override
    {
        // TODO: Return the list of neighbors for a given vertex.
        // Hint: Simply return `adjacencyList[vertex]`.
        return adjacencyList[vertex];
    }

    void printGraph() const override
    {
        // TODO: Print the adjacency list to the console.
        // Hint: Iterate through each vertex, then iterate through its adjacency list.
        std::cout << "Adjacency List:\n";
        for (int i = 0; i < numVertices; i++)
        {
            std::cout << "Vertex " << i << ": ";
            for (const auto& edge : adjacencyList[i])
            {
                std::cout << "-> (" << edge.first << ", w=" << edge.second << ") ";
            }
            std::cout << "\n";
        }
    }
};

//-------------------- GRAPH ALGORITHMS --------------------
class GraphAlgorithms
{
public:
    // Dijkstra's Algorithm
    static std::vector<int> dijkstra(const Graph &graph, int startVertex)
    {
        int n = graph.getNumVertices();
        std::vector<int> distances(n, INF);

        // TODO: Implement Dijkstra's algorithm.
        //
        // Key steps:
        // 1. Initialize `distances` array: `distances[startVertex] = 0`, others are `INF`.
        // 2. Use a min-priority queue to store pairs of {distance, vertex}.
        //    In C++: `std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;`
        // 3. Push `{0, startVertex}` into the priority queue.
        // 4. Loop while (!pq.empty()):
        //    a. Get the vertex `u` with the smallest distance from the queue.
        //    b. If the extracted distance is greater than the stored distance (`distances[u]`), skip.
        //    c. For each neighbor `v` of `u`:
        //       i. "Relax" the edge (u, v): If `distances[u] + weight(u, v) < distances[v]`,
        //          update `distances[v]` and push `{distances[v], v}` to the queue.
        // 5. Return the `distances` array.

        distances[startVertex] = 0;

        std::priority_queue<std::pair<int, int>, 
                            std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

        pq.push({0, startVertex});

        while (!pq.empty())
        {
            int currDist = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (currDist > distances[u])
            {
                continue;
            }

            std::vector<std::pair<int, int>> neighbors = graph.getNeighbors(u);

            for (const auto& edge : neighbors)
            {
                int v = edge.first;
                int weight = edge.second;

                if (distances[u] != INF && distances[u] + weight < distances[v])
                {
                    distances[v] = distances[u] + weight;
                    pq.push({distances[v], v});
                }
            }
        }
        
        return distances;
    }

    // Bellman-Ford Algorithm
    static bool bellmanFord(const Graph &graph, int startVertex, std::vector<int> &distances)
    {
        int n = graph.getNumVertices();
        distances.assign(n, INF);

        // TODO: Implement the Bellman-Ford algorithm.
        //
        // Key steps:
        // Part 1: Relax edges repeatedly
        // 1. Initialize `distances` array: `distances[startVertex] = 0`, others are `INF`.
        // 2. Loop `n - 1` times:
        //    a. Iterate through all vertices `u` from 0 to `n-1`.
        //    b. For each vertex `u`, iterate through all its neighbors `v`.
        //    c. "Relax" the edge (u, v): If `distances[u] != INF` and `distances[u] + weight(u, v) < distances[v]`,
        //       update `distances[v]`.
        //
        // Part 2: Check for negative-weight cycles
        // 3. Loop one more time (the n-th iteration):
        //    a. Iterate through all edges (u, v).
        //    b. If any edge can still be relaxed (`distances[u] + weight(u, v) < distances[v]`),
        //       the graph contains a negative-weight cycle. Return `false`.
        //
        // 4. If no negative-weight cycle is found, return `true`. The `distances` array holds the result.

        distances[startVertex] = 0;
        
        for (int i = 0; i < n - 1; i++)
        {
            for (int u = 0; u < n; u++)
            {
                if (distances[u] == INF)
                {
                    continue;
                }

                std::vector<std::pair<int, int>> neighbors;

                for (const auto& edge : neighbors)
                {
                    int v = edge.first;
                    int weight = edge.second;

                    if (distances[u] + weight < distances[v])
                    {
                        distances[v] = distances[u] + weight;
                    }
                }
            }
        }

        for (int u = 0; u < n; u++)
        {
            if (distances[u] == INF)
            {
                continue;
            }

            std::vector<std::pair<int, int>> neighbors = graph.getNeighbors(u);

            for (const auto& edge : neighbors)
            {
                int v = edge.first;
                int weight = edge.second;

                if (distances[u] + weight < distances[v])
                {
                    return false;
                }
            }
        }

        return true;
    }
};

//-------------------- MAIN FUNCTION FOR TESTING --------------------
//============== DO NOT MODIFY THIS SECTION ==============//
void printDistances(const std::string& algorithmName, int startNode, const std::vector<int>& distances) {
    std::cout << "\n[" << algorithmName << "] Shortest distances from node " << startNode << ":\n";
    for (size_t i = 0; i < distances.size(); ++i) {
        std::cout << "  To " << i << ": ";
        if (distances[i] == INF) {
            std::cout << "INF\n";
        } else {
            std::cout << distances[i] << "\n";
        }
    }
}

int main()
{
    // --- Test Case 1: Dijkstra's Algorithm ---
    std::cout << "======= TEST CASE 1: DIJKSTRA'S ALGORITHM =======\n";
    AdjacencyListGraph dijkstraGraph(5, true);
    dijkstraGraph.addEdge(0, 1, 10);
    dijkstraGraph.addEdge(0, 2, 3);
    dijkstraGraph.addEdge(1, 2, 1);
    dijkstraGraph.addEdge(1, 3, 2);
    dijkstraGraph.addEdge(2, 1, 4);
    dijkstraGraph.addEdge(2, 3, 8);
    dijkstraGraph.addEdge(2, 4, 2);
    dijkstraGraph.addEdge(3, 4, 7);
    dijkstraGraph.addEdge(4, 3, 9);
    
    std::cout << "Graph for Dijkstra test:\n";
    dijkstraGraph.printGraph();
    
    int startNodeDijkstra = 0;
    std::vector<int> distancesDijkstra = GraphAlgorithms::dijkstra(dijkstraGraph, startNodeDijkstra);
    printDistances("Dijkstra", startNodeDijkstra, distancesDijkstra);
    std::cout << "Expected: 0->0:0, 0->1:7, 0->2:3, 0->3:9, 0->4:5" << std::endl;
    std::cout << "--------------------------------------------------\n";

    // --- Test Case 2: Bellman-Ford (no negative cycle) ---
    std::cout << "\n======= TEST CASE 2: BELLMAN-FORD (NO NEGATIVE CYCLE) =======\n";
    AdjacencyListGraph bellmanGraph(5, true);
    bellmanGraph.addEdge(0, 1, -1);
    bellmanGraph.addEdge(0, 2, 4);
    bellmanGraph.addEdge(1, 2, 3);
    bellmanGraph.addEdge(1, 3, 2);
    bellmanGraph.addEdge(1, 4, 2);
    bellmanGraph.addEdge(3, 2, 5);
    bellmanGraph.addEdge(3, 1, 1);
    bellmanGraph.addEdge(4, 3, -3);

    std::cout << "Graph for Bellman-Ford test:\n";
    bellmanGraph.printGraph();

    int startNodeBellman = 0;
    std::vector<int> distancesBellman;
    if (GraphAlgorithms::bellmanFord(bellmanGraph, startNodeBellman, distancesBellman)) {
        printDistances("Bellman-Ford", startNodeBellman, distancesBellman);
        std::cout << "Expected: 0->0:0, 0->1:-1, 0->2:2, 0->3:-2, 0->4:1" << std::endl;
    } else {
        std::cout << "\n[Bellman-Ford] Error: Detected a negative cycle when none was expected.\n";
    }
    std::cout << "--------------------------------------------------\n";
    
    // --- Test Case 3: Bellman-Ford (with negative cycle) ---
    std::cout << "\n======= TEST CASE 3: BELLMAN-FORD (WITH NEGATIVE CYCLE) =======\n";
    AdjacencyListGraph negCycleGraph(4, true);
    negCycleGraph.addEdge(0, 1, 1);
    negCycleGraph.addEdge(1, 2, -2);
    negCycleGraph.addEdge(2, 3, 3);
    negCycleGraph.addEdge(3, 1, -2); // Negative cycle: 1 -> 2 -> 3 -> 1 (total weight -1)

    std::cout << "Graph for Negative Cycle test:\n";
    negCycleGraph.printGraph();

    std::vector<int> distancesNegCycle;
    if (GraphAlgorithms::bellmanFord(negCycleGraph, 0, distancesNegCycle)) {
        std::cout << "\n[Bellman-Ford] Error: Failed to detect a negative cycle.\n";
    } else {
        std::cout << "\n[Bellman-Ford] Success: Graph contains a negative weight cycle as expected.\n";
    }
    std::cout << "--------------------------------------------------\n";

    return 0;
}
