#include <iostream>
#include <vector>
#include <algorithm> // Required for std::sort
#include <queue>     // Required for std::priority_queue
#include <string>
#include <map>       // Required for std::map

//-------------------- DATA STRUCTURES --------------------

// Represents an activity for the Activity Selection problem
struct Activity {
    int id;
    int start;
    int finish;
};

// Represents a job for the Job Scheduling problem
struct Job {
    int id;
    int weight;
    int length;
    
};

// Represents a node in the Huffman Tree
struct HuffmanNode {
    char data;
    int frequency;
    HuffmanNode *left, *right;

    HuffmanNode(char data, int frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

// Comparator for the min-priority queue used in Huffman coding
struct CompareNodes {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->frequency > r->frequency;
    }
};

//-------------------- GREEDY ALGORITHMS CLASS --------------------

class GreedyAlgorithms
{
public:
    /**
     * Solves the Activity Selection problem.
     * Selects the maximum number of compatible activities.
     * @param activities A vector of all available activities.
     * @return A vector containing the selected compatible activities.
     */
    static std::vector<Activity> activitySelection(std::vector<Activity>& activities)
    {
        std::vector<Activity> selectedActivities;

        // TODO: Implement the Activity Selection algorithm.
        //
        // 1. Sort the activities based on their finish time in ascending order.
        //    (Hint: Use std::sort with a custom lambda function or comparator struct).

            std::sort(activities.begin(), activities.end(),
                        [](const Activity& a, const Activity& b) {
                            return a.finish < b.finish;
                            });
        
        // 2. Select the first activity in the sorted list (it has the earliest finish time).

        if (!activities.empty())
        {
            selectedActivities.push_back(activities[0]);
        }

        // 3. Iterate through the rest of the sorted activities.
        
        for (size_t i = 1; i < activities.size(); i++)
        {
            // 4. If the current activity's start time is greater than or equal to the
            //    finish time of the *last selected activity*, then select the current activity.
                if (activities[i].start >= selectedActivities.back().finish)
                {
                    selectedActivities.push_back(activities[i]);
                }
        }

        return selectedActivities;
    }

    /**
     * Solves the Job Scheduling problem to minimize the weighted sum of completion times.
     * @param jobs A vector of jobs, each with a weight and a length.
     * @return The total weighted completion time (a double).
     */
    static double weightedJobScheduling(std::vector<Job>& jobs)
    {
        double totalWeightedCompletionTime = 0.0;
        double currentCompletionTime = 0.0;

        // TODO: Implement the weighted job scheduling algorithm (minimizing sum of w_j * C_j).
        //
        // 1. Sort the jobs in descending order based on the ratio (weight / length).
        //    (Hint: Be careful with integer division. Cast to double for sorting).

            std::sort(jobs.begin(), jobs.end(),
                        [](const Job& a, const Job& b) {
                            return (static_cast<double>(a.weight) / a.length) >
                                    (static_cast<double>(b.weight) / b.length);
                            });

        // 2. Iterate through the sorted jobs.
        for (const auto& job : jobs)
        {
            // 3. For each job, update the `currentCompletionTime` (C_j = C_{j-1} + length_j).
                currentCompletionTime += job.length;
            // 4. Add the job's contribution (weight_j * C_j) to the `totalWeightedCompletionTime`.
                totalWeightedCompletionTime += job.weight * currentCompletionTime;
        }

        return totalWeightedCompletionTime;
    }

    /**
     * Builds a Huffman Tree for data compression.
     * @param frequencies A vector of pairs (character, frequency).
     * @return A pointer to the root node of the constructed Huffman Tree.
     */
    static HuffmanNode* buildHuffmanTree(std::vector<std::pair<char, int>>& frequencies)
    {
        // TODO: Implement the Huffman tree construction.
        //
        // 1. Create a min-priority queue of HuffmanNode pointers.
        //    (Hint: `std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNodes> pq;`)

            std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNodes> pq;

        // 2. Create a leaf node (HuffmanNode) for each character and add it to the priority queue.

            for (const auto& pair : frequencies)
            {
                HuffmanNode* node = new HuffmanNode(pair.first, pair.second);
                pq.push(node);
            }
        
        // 3. Loop while the priority queue has more than one node:
        //    a. Extract the two nodes with the minimum frequency (call them `left` and `right`).

                HuffmanNode* left = pq.top();
                pq.pop();
                HuffmanNode* right = pq.top();
                pq.pop();

        //    b. Create a new internal node (e.g., with data '$' or any placeholder).

                HuffmanNode* newNode = new HuffmanNode('$', left -> frequency + right -> frequency);

        //    c. Set the new node's frequency to `left->frequency + right->frequency`.
        //    d. Set the new node's children: `newNode->left = left` and `newNode->right = right`.

                newNode -> left = left;
                newNode -> right = right;

        //    e. Insert the new internal node back into the priority queue.

                pq.push(newNode);

        // 4. When the loop finishes, the single node remaining in the queue is the root. Return it.

        return pq.empty() ? nullptr : pq.top(); // Placeholder
    }
};

//-------------------- HELPER & MAIN FUNCTION --------------------
//============== DO NOT MODIFY THIS SECTION ==============//

/**
 * Helper function to traverse the Huffman tree and store codes in a map.
 */
void getHuffmanCodes(HuffmanNode* root, std::string code, std::map<char, std::string>& codes) {
    if (!root) return;

    // If it's a leaf node
    if (!root->left && !root->right) {
        codes[root->data] = code;
        return;
    }

    getHuffmanCodes(root->left, code + "0", codes);
    getHuffmanCodes(root->right, code + "1", codes);
}

/**
 * Helper function to clean up the allocated memory for the Huffman tree.
 */
void deleteHuffmanTree(HuffmanNode* node) {
    if (node == nullptr) return;
    deleteHuffmanTree(node->left);
    deleteHuffmanTree(node->right);
    delete node;
}


int main()
{
    // --- Test Case 1: Activity Selection ---
    std::cout << "======= TEST CASE 1: ACTIVITY SELECTION =======\n";
    std::vector<Activity> activities = {
        {1, 1, 4}, {2, 3, 5}, {3, 0, 6}, {4, 5, 7}, {5, 3, 9}, {6, 5, 9},
        {7, 6, 10}, {8, 8, 11}, {9, 8, 12}, {10, 2, 14}, {11, 12, 16}
    };
    
    std::vector<Activity> selected = GreedyAlgorithms::activitySelection(activities);
    
    std::cout << "Selected Activities (IDs): ";
    for (const auto& act : selected) {
        std::cout << act.id << " ";
    }
    std::cout << "\nExpected: 1 4 8 11 (Note: 2 4 8 11 is also correct if sort is stable)\n";
    std::cout << "--------------------------------------------------\n";

    // --- Test Case 2: Job Scheduling (Minimizing Weighted Completion Time) ---
    std::cout << "\n======= TEST CASE 2: JOB SCHEDULING (w/l ratio) =======\n";
    std::vector<Job> jobs = { {1, 10, 2}, {2, 5, 1}, {3, 1, 5} }; // {id, weight, length}
    // Ratios: Job 1 (10/2 = 5), Job 2 (5/1 = 5), Job 3 (1/5 = 0.2)
    // Correct Order (e.g., Job 2, Job 1, Job 3):
    // C_2 = 1.         Weight_Sum += 5 * 1 = 5
    // C_1 = 1 + 2 = 3. Weight_Sum += 10 * 3 = 30
    // C_3 = 3 + 5 = 8. Weight_Sum += 1 * 8 = 8
    // Total = 5 + 30 + 8 = 43.
    
    double totalTime = GreedyAlgorithms::weightedJobScheduling(jobs);
    std::cout << "Total Weighted Completion Time: " << totalTime << std::endl;
    std::cout << "Expected: 43\n";
    std::cout << "--------------------------------------------------\n";

    // --- Test Case 3: Huffman Coding ---
    std::cout << "\n======= TEST CASE 3: HUFFMAN CODING =======\n";
    std::vector<std::pair<char, int>> frequencies = {
        {'A', 45}, {'B', 13}, {'C', 12}, {'D', 16}, {'E', 9}, {'F', 5}
    };
    
    HuffmanNode* root = GreedyAlgorithms::buildHuffmanTree(frequencies);
    
    std::map<char, std::string> codes;
    getHuffmanCodes(root, "", codes);
    
    std::cout << "Huffman Codes:\n";
    if (root) { // Only print if root is not null
        for (auto const& [key, val] : codes) {
            std::cout << "  " << key << ": " << val << std::endl;
        }
    } else {
        std::cout << "  (Tree not built)\n";
    }
    std::cout << "Expected (codes may vary based on 0/1 assignment and tie-breaking):\n";
    std::cout << "  A: 0\n  B: 100\n  C: 101\n  D: 111\n  E: 1101\n  F: 1100\n";
    std::cout << "--------------------------------------------------\n";

    // Cleanup Huffman tree memory
    deleteHuffmanTree(root);

    return 0;
}
//============== END OF DO NOT MODIFY SECTION ==============//