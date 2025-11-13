#include <iostream>
#include <vector>
#include <queue>       // For std::priority_queue (used in Prim's)
#include <algorithm>   // For std::sort (used in Kruskal's)
#include <numeric>     // For std::iota (used in DSU)
#include <limits>

const int INF = std::numeric_limits<int>::max();

//-------------------- DATA STRUCTURES --------------------

// Represents a weighted edge in the graph
struct Edge {
    int src;
    int dest;
    int weight;
};

// Represents the graph itself
class Graph {
public:
    int V; // Number of vertices
    
    // Adjacency list representation: vector of {neighbor, weight} pairs
    // Used primarily for Prim's Algorithm
    std::vector<std::vector<std::pair<int, int>>> adj;

    // Edge list representation: vector of all edges
    // Used primarily for Kruskal's Algorithm
    std::vector<Edge> edges;

    Graph(int vertices) : V(vertices) {
        adj.resize(V);
    }

    // Add an undirected edge to the graph
    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
        edges.push_back({u, v, w});
    }
};

/**
 * Disjoint Set Union (DSU) or Union-Find data structure.
 * Required for Kruskal's algorithm to efficiently detect cycles.
 */
class DSU {
    std::vector<int> parent;

public:
    DSU(int n) {
        parent.resize(n);
        // TODO: Initialize the DSU structure.
        // Hint: Set each element's parent to itself.
        // (std::iota(parent.begin(), parent.end(), 0) is a quick way)

        std::iota(parent.begin(), parent.end(), 0);
    }

    /**
     * Finds the representative (root) of the set that element i belongs to.
     * Should implement path compression for efficiency.
     */
    int find(int i) {
        // TODO: Implement the 'find' operation with path compression.
        //
        // 1. If parent[i] is i, then i is the root. Return i.
        // 2. Otherwise, recursively call find(parent[i]) and set
        //    parent[i] to the result before returning it.

        if (parent[i] == i)
        {
            return i;
        }

        parent[i] = find(parent[i]);

        return parent[i]; 
    }

    /**
     * Merges the two sets that x and y belong to.
     */
    void unite(int x, int y) {
        // TODO: Implement the 'unite' (or union) operation.
        //
        // 1. Find the root of x (call it rootX).
        // 2. Find the root of y (call it rootY).
        // 3. If rootX is not equal to rootY, merge them by setting
        //    the parent of one root to the other (e.g., parent[rootX] = rootY).

        int rootX = find(x);
        int rootY = find(y);

        if (rootX != rootY)
        {
            parent[rootX] = rootY; 
        }
    }
};

//-------------------- MST ALGORITHMS CLASS --------------------

class MSTAlgorithms {
public:
    /**
     * Implements Prim's Algorithm to find the MST.
     * @param graph The graph (using its adjacency list).
     * @return The total weight of the Minimum Spanning Tree.
     */
    static int primsAlgorithm(Graph& graph) {
        int mstWeight = 0;
        int startNode = 0; // Start Prim's from node 0

        // TODO: Implement Prim's Algorithm.
        //
        // 1. Create a min-priority queue to store {weight, vertex}.
        //    (Hint: `std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;`)
                
                std::priority_queue<std::pair<int, int>,
                                    std::vector<std::pair<int, int>>,
                                    std::greater<std::pair<int, int>>> pq;

        // 2. Create a `visited` vector (boolean) to track visited vertices.
        
                std::vector<bool> visited(graph.V, false);

        // 3. Push the starting node into the pq with weight 0: `pq.push({0, startNode});`
        
                pq.push({0, startNode});

        // 4. Loop while the priority queue is not empty:
        //    a. Get the top element (lightest edge): {weight, u}. Pop it.
        //    b. If vertex `u` is already visited, `continue`.
        //    c. Mark `u` as visited.
        //    d. Add its `weight` to `mstWeight`.
        //    e. Iterate through all neighbors `v` of `u` (using `graph.adj[u]`):
        //       i. If `v` is not visited, push {neighbor_weight, v} onto the pq.
        //
        // 5. Return `mstWeight`.

                while (!pq.empty())
                {
                    auto [weight, u] = pq.top();
                    pq.pop();

                    if (visited[u])
                    {
                        continue;
                    }

                    visited[u] = true;
                    mstWeight += weight;

                    for (auto [v, w] : graph.adj[u])
                    {
                        if (!visited[v])
                        {
                            pq.push({w, v});
                        }
                    }
                }

        return mstWeight; 
    }

    /**
     * Implements Kruskal's Algorithm to find the MST.
     * @param graph The graph (using its edge list).
     * @return The total weight of the Minimum Spanning Tree.
     */
    static int kruskalsAlgorithm(Graph& graph) {
        int mstWeight = 0;

        // TODO: Implement Kruskal's Algorithm.
        //
        // 1. Sort the graph's `edges` list in ascending order of weight.
        //    (Hint: Use `std::sort` with a custom lambda comparator).

                std::vector<Edge> sortedEdges = graph.edges;
                std::sort(sortedEdges.begin(), sortedEdges.end(),
                            [](const Edge& a, const Edge& b) {
                                return a.weight < b.weight;
                                });

        // 2. Create a DSU (Disjoint Set Union) structure with `graph.V` elements.
                
                DSU dsu(graph.V);

        // 3. Initialize an edge counter `edgeCount = 0`.
        
                int edgeCount = 0;

        // 4. Iterate through the sorted `edges`:
        //    a. Get the current edge (u, v, w).
        //    b. Find the set of u (call it rootU) and the set of v (call it rootV) using `dsu.find()`.
        //    c. If `rootU` is NOT equal to `rootV`:
        //       i. This edge does not form a cycle.
        //       ii. Add the edge to the MST: `mstWeight += w`.
        //       iii. Merge the two sets: `dsu.unite(rootU, rootV)`.
        //       iv. Increment `edgeCount`.
        //
        // 5. (Optional but good) If `edgeCount` equals `graph.V - 1`, break early.
        //
        // 6. Return `mstWeight`.

                for (const auto& edge : sortedEdges)
                {
                    int u = edge.src;
                    int v = edge.dest;
                    int w = edge.weight;

                    int rootU = dsu.find(u);
                    int rootV = dsu.find(v);

                    if (rootU != rootV)
                    {
                        mstWeight += w;
                        dsu.unite(rootU,rootV);
                        edgeCount++;

                        if (edgeCount == graph.V - 1)
                        {
                            break;
                        }
                    }
                }

        return mstWeight; 
    }
};


//-------------------- MAIN FUNCTION FOR TESTING --------------------
//============== DO NOT MODIFY THIS SECTION ==============//

int main()
{
    // Create a graph with 4 vertices
    int V = 4;
    Graph graph(V);

    // Add edges: (src, dest, weight)
    graph.addEdge(0, 1, 10);
    graph.addEdge(0, 2, 6);
    graph.addEdge(0, 3, 5);
    graph.addEdge(1, 3, 15);
    graph.addEdge(2, 3, 4);

    // --- Test Case 1: Prim's Algorithm ---
    std::cout << "======= TEST CASE 1: PRIM'S ALGORITHM =======\n";
    int primsWeight = MSTAlgorithms::primsAlgorithm(graph);
    std::cout << "Total MST Weight (Prim's): " << primsWeight << std::endl;
    std::cout << "Expected: 19\n";
    std::cout << "--------------------------------------------------\n";

    // --- Test Case 2: Kruskal's Algorithm ---
    std::cout << "\n======= TEST CASE 2: KRUSKAL'S ALGORITHM =======\n";
    int kruskalsWeight = MSTAlgorithms::kruskalsAlgorithm(graph);
    std::cout << "Total MST Weight (Kruskal's): " << kruskalsWeight << std::endl;
    std::cout << "Expected: 19\n";
    std::cout << "--------------------------------------------------\n";

    return 0;
}
//============== END OF DO NOT MODIFY SECTION ==============//