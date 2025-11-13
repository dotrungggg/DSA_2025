#include <iostream>
#include <vector>
#include <queue>       // For std::queue (used in BFS)
#include <limits>      // For std::numeric_limits
#include <algorithm>   // For std::min

const int INF = std::numeric_limits<int>::max();

//-------------------- GRAPH DATA STRUCTURE --------------------

/**
 * Represents a directed graph with capacities.
 * We use an adjacency matrix for capacities because it simplifies
 * finding and updating residual capacities for both forward and reverse edges.
 */
class Graph {
public:
    int V; // Number of vertices
    
    // capacity[u][v] = capacity of edge from u to v
    std::vector<std::vector<int>> capacity;

    Graph(int vertices) : V(vertices) {
        // Initialize the capacity matrix with all zeros
        capacity.resize(V, std::vector<int>(V, 0));
    }

    // Add a directed edge from u to v with a given capacity
    void addEdge(int u, int v, int cap) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            capacity[u][v] = cap;
        }
    }
};

//-------------------- MAX FLOW ALGORITHM --------------------

class MaxFlowAlgorithms {
public:

    /**
     * Finds an augmenting path from source 's' to sink 't' in the 'residualGraph' using BFS.
     * @param residualGraph The current residual capacity graph.
     * @param s The source node.
     * @param t The sink node.
     * @param parent An array to store the path found by BFS (parent[v] = u means u comes before v).
     * @param V The number of vertices.
     * @return true if a path from s to t exists, false otherwise.
     */
    static bool bfs(std::vector<std::vector<int>>& residualGraph, int s, int t, std::vector<int>& parent, int V) {
        
        // TODO: Implement the Breadth-First Search (BFS).
        //
        // 1. Create a `visited` vector of size V, initialized to `false`.

                std::vector<bool> visited(V, false);

        // 2. Create a `queue` for BFS.
        
                std::queue<int> q;

        // 3. Enqueue the source node `s`, mark it as visited, and set its parent to -1.
        //    (e.g., `q.push(s); visited[s] = true; parent[s] = -1;`)

                q.push(s);
                visited[s] = true;
                parent[s] = -1;

        // 4. Loop while the queue is not empty:

        while (!q.empty())
        {
            //  a. Dequeue a vertex `u`.

                int u = q.front();
                q.pop();

            //  b. Iterate through all possible neighbors `v` (from 0 to V-1).

                for (int v = 0; v < V; v++)
                {
                    //  c. If `v` is NOT visited AND there is residual capacity (i.e., `residualGraph[u][v] > 0`):

                    if (!visited[v] && residualGraph[u][v] > 0)
                    {
                        // i. Enqueue `v`.

                        q.push(v);

                        // ii. Mark `v` as visited.

                        visited[v] = true;

                        // iii. Set `parent[v] = u`.

                        parent[v] = u;
                    }
                }
        }

        // 5. After the loop, return `visited[t]` (this will be `true` if we reached the sink, `false` otherwise).

        return visited[t];
        
    }


    /**
     * Implements the Edmonds-Karp algorithm (a version of Ford-Fulkerson using BFS)
     * to find the maximum s-t flow.
     * @param graph The graph with original capacities.
     * @param s The source node.
     * @param t The sink node.
     * @return The value of the maximum flow.
     */
    static int edmondsKarp(Graph& graph, int s, int t) {
        int V = graph.V;
        int maxFlow = 0;

        // Create the residual graph, initialized with the original capacities
        std::vector<std::vector<int>> residualGraph = graph.capacity;
        
        // This vector will be filled by BFS to store the augmenting path
        std::vector<int> parent(V);

        // TODO: Implement the main Edmonds-Karp loop.
        //
        // 1. Start a loop that continues as long as BFS finds an augmenting path.
        //    (Hint: `while (bfs(residualGraph, s, t, parent, V))` )

                while (bfs(residualGraph, s, t, parent, V))
                {
                    // 2. Inside the loop, if a path is found (BFS returned true):
                    //   a. Find the bottleneck capacity (pathFlow) of this path.
                    //     - Start at the sink `t`.
                    //     - Initialize `pathFlow = INF`.
                    //     - Trace back to the source `s` using the `parent` array.
                    //     - (e.g., `for (v = t; v != s; v = parent[v])`)
                    //     - `u = parent[v];`
                    //     - `pathFlow = std::min(pathFlow, residualGraph[u][v]);`

                    int pathFlow = INF;

                    for (int v = t; v != s; v = parent[v])
                    {
                        int u = parent[v];
                        pathFlow = std::min(pathFlow, residualGraph[u][v]);
                    }

                    //   b. Update the residual graph capacities along the path.
                    //     - Trace back from `t` to `s` again.
                    //     - `for (v = t; v != s; v = parent[v])`
                    //     - `u = parent[v];`
                    //     - Decrease capacity on the forward edge: `residualGraph[u][v] -= pathFlow;`
                    //     - Increase capacity on the backward edge: `residualGraph[v][u] += pathFlow;`

                    for (int v = t; v != s; v = parent[v])
                    {
                        int u = parent[v];

                        residualGraph[u][v] -= pathFlow;
                        residualGraph[v][u] += pathFlow;
                    }

                    //    c. Add the `pathFlow` to the `maxFlow`.

                    maxFlow += pathFlow;
                }

        // 3. After the loop finishes (no more augmenting paths), return `maxFlow`.

        return maxFlow;
    }
};

//-------------------- MAIN FUNCTION FOR TESTING --------------------
//============== DO NOT MODIFY THIS SECTION ==============//

int main()
{
    // Create a graph with 6 vertices
    // s = 0, t = 5
    // This graph is based on the example from 13_maxflows.pdf, page 3/4
    int V = 6;
    Graph graph(V);
    int s = 0;
    int t = 5;

    graph.addEdge(0, 1, 3);
    graph.addEdge(0, 2, 4);
    graph.addEdge(1, 3, 4);
    graph.addEdge(1, 4, 2);
    graph.addEdge(2, 1, 2);
    graph.addEdge(2, 3, 6);
    graph.addEdge(3, 4, 6);
    graph.addEdge(3, 5, 3); // 3 -> t
    graph.addEdge(4, 2, 3);
    graph.addEdge(4, 5, 6); // 4 -> t

    std::cout << "======= TEST CASE 1: EDMONDS-KARP MAX FLOW =======\n";
    std::cout << "Calculating Max Flow from node " << s << " to node " << t << "...\n";
    
    int maxFlow = MaxFlowAlgorithms::edmondsKarp(graph, s, t);
    
    std::cout << "Total Max Flow: " << maxFlow << std::endl;
    std::cout << "Expected: 7\n";
    std::cout << "--------------------------------------------------\n";

    return 0;
}
//============== END OF DO NOT MODIFY SECTION ==============//