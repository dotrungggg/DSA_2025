#include <iostream>
#include <string>
#include <vector>

class KMPAlgorithm
{
public:
    /**
     * Pre-processes the pattern to build the "Longest Proper Prefix which is also a Suffix" (LPS) array.
     * This array is also known as the "failure function" (F) in some literature.
     *
     * @param pattern The pattern string P.
     * @return A vector<int> representing the LPS array. lps[i] will store
     * the length of the longest proper prefix of pattern[0..i]
     * which is also a suffix of pattern[0..i].
     */
    static std::vector<int> computeLPSArray(const std::string& pattern)
    {
        int M = pattern.length();
        std::vector<int> lps(M, 0);

        // TODO: Implement the LPS array (failure function) computation.
        //
        // 1. Initialize `length = 0` (length of the previous longest prefix suffix).

                int length = 0;

        // 2. Initialize `lps[0] = 0` (it's always 0).
        
                lps[0] = 0;

        // 3. Iterate from `i = 1` to `M - 1`.
                int i = 1;

                while (i < M)
                {
                    // 4. If `pattern[i] == pattern[length]`:
                    //    - It means the prefix/suffix match is extended.
                    //    - Increment `length`.
                    //    - Set `lps[i] = length`.

                    if (pattern[i] == pattern[length])
                    {
                        length++;
                        lps[i] = length;
                        i++;
                    }

                    else 
                    {
                    // 5. If `pattern[i] != pattern[length]`:
                    //    - If `length != 0`, we need to "fallback".
                    //      - Set `length = lps[length - 1]`.
                    //      - *Do not* increment `i` yet; re-check this condition in the loop.

                        if (length != 0)
                        {
                            length = lps[length - 1];
                        }

                        else
                        {
                            //    - If `length == 0` (we've fallen all the way back):
                            //      - Set `lps[i] = 0`.
                            //      - Increment `i`.

                            lps[i] = 0;
                            i++;
                        }
                    }
                }

        return lps;
    }

    /**
     * Searches for all occurrences of a `pattern` string within a `text` string
     * using the KMP algorithm.
     *
     * @param text The text string T.
     * @param pattern The pattern string P.
     * @return A vector<int> containing all the 0-based starting indices of
     * all occurrences of the pattern in the text.
     */
    static std::vector<int> kmpSearch(const std::string& text, const std::string& pattern)
    {
        std::vector<int> matchIndices;
        int N = text.length();
        int M = pattern.length();

        if (M == 0) return matchIndices;

        // TODO: Implement the KMP search algorithm.
        //
        // 1. Pre-process the pattern to get the LPS array.
        //    (Hint: `std::vector<int> lps = computeLPSArray(pattern);`)

                std::vector<int> lps = computeLPSArray(pattern);

        // 2. Initialize two pointers:
        //    - `i = 0` (pointer for text)
        //    - `j = 0` (pointer for pattern)
        
                int i = 0;
                int j = 0;

        // 3. Loop while `i < N` (while we still have text to search):

                while (i < N)
                {
                    // 4. If `text[i] == pattern[j]`:
                    //    - Characters match. Increment both `i` and `j`.

                    if (text[i] == pattern[j])
                    {
                        i++;
                        j++;
                    }

                    // 5. Check if `j == M` (a full pattern match):
                    //    - A match is found! Add the starting index (`i - j`) to `matchIndices`.
                    //    - "Shift" the pattern to find the next match: `j = lps[j - 1]`.

                    if (j == M)
                    {
                        matchIndices.push_back(i - j);
                        j = lps[j - 1];
                    }

                    // 6. If a mismatch occurs (`text[i] != pattern[j]`):
                    //    - If `j != 0` (we had some partial match):
                    //      - Use the LPS array to "shift" the pattern: `j = lps[j - 1]`.
                    //      - *Do not* increment `i`. We re-compare `text[i]` with the new `pattern[j]`.
                    //    - If `j == 0` (mismatch at the very first character of the pattern):
                    //      - Just move on to the next character in the text: `i++`.
        
                    else if (i < N && text[i] != pattern[j])
                    {
                        if (j != 0)
                        {
                            j = lps[j - 1];
                        }

                        else
                        {
                            i++;
                        }
                    }
                }


        // 7. After the loop, return `matchIndices`.

        return matchIndices;
    }
};

//-------------------- MAIN FUNCTION FOR TESTING --------------------
//============== DO NOT MODIFY THIS SECTION ==============//

int main()
{
    // Test Case 1: Based on the PDF example (Page 3)
    std::string text1 = "abacaabaccabacabaabb";
    std::string pattern1 = "abacab";

    std::cout << "======= TEST CASE 1: PDF Example =======\n";
    std::cout << "Text: " << text1 << std::endl;
    std::cout << "Pattern: " << pattern1 << std::endl;

    std::vector<int> matches1 = KMPAlgorithm::kmpSearch(text1, pattern1);

    std::cout << "Matches found at indices: ";
    for (int index : matches1) {
        std::cout << index << " ";
    }
    std::cout << "\nExpected: 10\n";
    std::cout << "--------------------------------------------------\n";


    // Test Case 2: Overlapping matches
    std::string text2 = "AAAAABAAABA";
    std::string pattern2 = "AAAA";

    std::cout << "\n======= TEST CASE 2: Overlapping Matches =======\n";
    std::cout << "Text: " << text2 << std::endl;
    std::cout << "Pattern: " << pattern2 << std::endl;

    std::vector<int> matches2 = KMPAlgorithm::kmpSearch(text2, pattern2);

    std::cout << "Matches found at indices: ";
    for (int index : matches2) {
        std::cout << index << " ";
    }
    std::cout << "\nExpected: 0 1 7\n";
    std::cout << "--------------------------------------------------\n";


    // Test Case 3: No matches
    std::string text3 = "abcdefg";
    std::string pattern3 = "xyz";

    std::cout << "\n======= TEST CASE 3: No Matches =======\n";
    std::cout << "Text: " << text3 << std::endl;
    std::cout << "Pattern: " << pattern3 << std::endl;

    std::vector<int> matches3 = KMPAlgorithm::kmpSearch(text3, pattern3);

    std::cout << "Matches found at indices: ";
    for (int index : matches3) {
        std::cout << index << " ";
    }
    std::cout << "\nExpected: \n";
    std::cout << "--------------------------------------------------\n";

    return 0;
}
//============== END OF DO NOT MODIFY SECTION ==============//