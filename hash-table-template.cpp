// Hash Table Implementation Template
// Instructions: Implement the missing function definitions for each class.
// Each function has comments indicating what it should do.

#include <iostream>
#include <list>
#include <string>
#include <stdexcept>

//-------------------- HASH FUNCTION HELPERS --------------------
// Helper function to hash strings using Horner's rule
int hashString(const std::string& str, int tableSize) {
    // TODO: Implement string hashing function using Horner's rule
    // Use a prime number as the base (e.g., 31)
    // Return hash value modulo tableSize
    const int BASE = 31;
    long long hash = 0;

    for (char c : str) {
        hash = (hash * BASE + static_cast<unsigned char>(c)) % tableSize;
    }

    return static_cast<int>(hash); // Placeholder, replace with correct implementation
}

// Helper function to hash integers
int hashInt(int key, int tableSize) {
    // TODO: Implement simple integer hash function
    // Consider using the division method: h(k) = k mod tableSize
    int hash = key % tableSize;
    if (hash < 0) {
        hash += tableSize;
    }
    return hash ; // Placeholder, replace with correct implementation
}

//-------------------- SEPARATE CHAINING HASH TABLE --------------------
// Generic hash table using separate chaining for collision resolution
template <typename K, typename V>
class SeparateChainingHashTable {
private:
    // Each bucket contains a list of key-value pairs
    struct KeyValuePair {
        K key;
        V value;
        KeyValuePair(const K& k, const V& v) : key(k), value(v) {}
    };

    std::list<KeyValuePair>* buckets; // Array of linked lists
    int tableSize;                    // Number of buckets
    int itemCount;                    // Total number of items in the hash table

    // Hash function - will be defined differently for different key types
    int hash(const K& key) const {
        // TODO: Implement hash function selector based on key type
        if constexpr (std::is_same_v<K, std::string>) {
            return hashString(key, tableSize);
        }
        else if constexpr (std::is_integral_v<K>) {
            return hashInt(key, tableSize);
        }
        else {
            static_assert(!sizeof(K*), "Unsupported key type for hash table");
        }
    }

public:
    // Constructor: Initialize hash table with given size
    SeparateChainingHashTable(int size = 101) : tableSize(size), itemCount(0) {
        // TODO: Implement constructor
        buckets = new std::list<KeyValuePair>[tableSize];
    }

    // Destructor: Free dynamically allocated memory
    ~SeparateChainingHashTable() {
        // TODO: Implement destructor
        delete[] buckets;
    }

    // Insert a key-value pair into the hash table
    // If key already exists, update its value
    void insert(const K& key, const V& value) {
        // TODO: Implement insert function
        int index = hash(key);

        for (auto& pair : buckets[index]) {
            if (pair.key == key) {
                pair.value = value;
                return;
            }
        }

        buckets[index].push_back(KeyValuePair(key, value));
        itemCount++;
    }

    // Remove a key-value pair from the hash table
    // Throw exception if key not found
    void remove(const K& key) {
        // TODO: Implement remove function
        int index = hash(key);

        for (auto it = buckets[index].begin(); it != buckets[index].end(); ++it) {
            if (it->key == key) {
                buckets[index].erase(it);
                itemCount--;
                return;
            }
        }

        throw std::runtime_error("Key not found");
    }

    // Search for a value associated with the given key
    // Throw exception if key not found
    V search(const K& key) const {
        // TODO: Implement search function
        int index = hash(key);

        for (const auto& pair : buckets[index]) {
            if (pair.key == key) {
                return pair.value;
            }
        }

        throw std::runtime_error("Key not found");
    }

    // Check if the hash table contains the given key
    bool contains(const K& key) const {
        // TODO: Implement contains function
        int index = hash(key);

        for (const auto& pair : buckets[index]) {
            if (pair.key == key) {
                return true;
            }
        }

        return false; 
    }

    // Return the current load factor
    float loadFactor() const {
        // TODO: Implement load factor calculation
        return static_cast<float>(itemCount) / tableSize; 
    }

    // Return number of items in the hash table
    int size() const {
        // TODO: Implement size function
        return itemCount; 
    }

    // Display the hash table (for debugging)
    void display() const {
        // TODO: Implement display function
        for (int i = 0; i < tableSize; i++) {
            if (!buckets[i].empty()) {
                std::cout << "Bucket " << i << ": ";
                
                for (const auto& pair : buckets[i]) {
                    std::cout << "[" << pair.key << ": " << pair.value << "] ";
                }

                std::cout << '\n';
            }
        }
    }
};

//-------------------- OPEN ADDRESSING HASH TABLE --------------------
// Generic hash table using open addressing for collision resolution
template <typename K, typename V>
class OpenAddressingHashTable {
private:
    enum EntryStatus { EMPTY, OCCUPIED, DELETED };

    struct Entry {
        K key;
        V value;
        EntryStatus status;
        
        Entry() : status(EMPTY) {}
    };

    Entry* table;      // Array of entries
    int tableSize;     // Size of the table
    int itemCount;     // Number of items in the table

    // Hash function
    int hash(const K& key) const {
        // TODO: Implement hash function selector based on key type
        if constexpr (std::is_same_v<K, std::string>) {
            return hashString(key, tableSize);
        }
        else if constexpr (std::is_integral_v<K>) {
            return hashInt(key, tableSize);
        }
        else {
            static_assert(!sizeof(K*), "Unsupported key type for hash table");
        }
    }

    

    // Probe function for collision resolution (linear probing)
    int probe(int hash, int i) const {
        // TODO: Implement linear probing
        return (hash + i) % tableSize; // Placeholder, replace with correct implementation
    }


public:
    // Constructor: Initialize hash table with given size
    OpenAddressingHashTable(int size = 101) :  tableSize(size), itemCount(0) {
        // TODO: Implement constructor
        table = new Entry[tableSize];
    }

    // Destructor: Free dynamically allocated memory
    ~OpenAddressingHashTable() {
        // TODO: Implement destructor
        delete[] table;
    }

    // Insert a key-value pair into the hash table
    // If key already exists, update its value
    // If table is full, throw an exception
    void insert(const K& key, const V& value) {
        // TODO: Implement insert function
        if (itemCount >= tableSize) {
            throw std::runtime_error("Hash table is full");
        }

        int hashValue = hash(key);
        int i = 0;

        while (i < tableSize) {
            int index = probe(hashValue, i);

            if (table[index].status == EMPTY || table[index].status == DELETED) {
                table[index].key = key;
                table[index].value = value;
                table[index].status = OCCUPIED;
                itemCount++;
                return;
            }
            else if (table[index].status == OCCUPIED && table[index].key == key) {
                table[index].value = value;
                return;
            }

            i++;
        }

        throw std::runtime_error("Hash table is full");
    }

    // Remove a key-value pair from the hash table
    // Throw exception if key not found
    void remove(const K& key) {
        // TODO: Implement remove function
        int hashValue = hash(key);
        int i = 0;

        while (i < tableSize) {
            int index = probe(hashValue, i);

            if (table[index].status == EMPTY) {
                throw std::runtime_error("Key not found");
            }
            else if (table[index].status == OCCUPIED && table[index].key == key) {
                table[index].status = DELETED;
                itemCount--;
                return;
            }

            i++;
        }

        throw std::runtime_error("Key not found");
    }

    // Search for a value associated with the given key
    // Throw exception if key not found
    V search(const K& key) const {
        // TODO: Implement search function
        int hashValue = hash(key);
        int i = 0;

        while (i < tableSize) {
            int index = probe(hashValue, i);

            if (table[index].status == EMPTY) {
                throw std::runtime_error("Key not found");
            }
            else if (table[index].status == OCCUPIED) {
                return table[index].value;
            }

            i++;
        }

        throw std::runtime_error("Key not found");
    }

    // Check if the hash table contains the given key
    bool contains(const K& key) const {
        // TODO: Implement contains function
        int hashValue = hash(key);
        int i = 0;

        while (i < tableSize) {
            int index = probe(hashValue, i);

            if (table[index].status == EMPTY) {
                return false;
            }
            else if (table[index].status == OCCUPIED) {
                return true;
            }

            i++;
        }

        return false; 
    }

    // Return the current load factor
    float loadFactor() const {
        // TODO: Implement load factor calculation
        return static_cast<float>(itemCount) / tableSize; 
    }

    // Return number of items in the hash table
    int size() const {
        // TODO: Implement size function
        return itemCount; 
    }
    // Display the hash table (for debugging)
    void display() const {
        // TODO: Implement display function
        for (int i = 0; i < tableSize; i++) {
            if (table[i].status == OCCUPIED) {
                std::cout << "Index " << i  << ": [" << table[i].key << ": " << table[i].value  << "]" << '\n';
            }
            else if (table[i].status == DELETED) {
                std::cout << "Index " << i << " [DELETED]" << '\n';
            }
        }
    }
};
  
//-------------------- TESTING FUNCTION --------------------
void testHashTables() {
    // Test separate chaining hash table with string keys
    std::cout << "Testing Separate Chaining Hash Table with string keys:\n";
    SeparateChainingHashTable<std::string, int> scHashTable(10);
    
    // Insert some key-value pairs
    scHashTable.insert("apple", 5);
    scHashTable.insert("banana", 8);
    scHashTable.insert("cherry", 12);
    scHashTable.insert("date", 15);
    scHashTable.insert("elderberry", 20);
    scHashTable.insert("fig", 25);
    
    // Display the table
    std::cout << "After insertions:\n";
    scHashTable.display();
    
    // Search for some keys
    try {
        std::cout << "Value for 'apple': " << scHashTable.search("apple") << std::endl;
        std::cout << "Value for 'banana': " << scHashTable.search("banana") << std::endl;
        std::cout << "Value for 'grape': " << scHashTable.search("grape") << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    
    // Remove a key
    std::cout << "Removing 'banana'..." << std::endl;
    scHashTable.remove("banana");
    
    // Display the table again
    std::cout << "After removal:\n";
    scHashTable.display();
    
    // Test contains method
    std::cout << "Contains 'apple': " << (scHashTable.contains("apple") ? "Yes" : "No") << std::endl;
    std::cout << "Contains 'banana': " << (scHashTable.contains("banana") ? "Yes" : "No") << std::endl;
    
    // Test load factor
    std::cout << "Load factor: " << scHashTable.loadFactor() << std::endl;
    std::cout << "Size: " << scHashTable.size() << std::endl;
    
    // Test open addressing hash table with integer keys
    std::cout << "\nTesting Open Addressing Hash Table with integer keys:\n";
    OpenAddressingHashTable<int, std::string> oaHashTable(10);
    
    // Insert some key-value pairs
    oaHashTable.insert(5, "five");
    oaHashTable.insert(15, "fifteen");
    oaHashTable.insert(25, "twenty-five");
    oaHashTable.insert(35, "thirty-five");
    oaHashTable.insert(45, "forty-five");
    
    // Display the table
    std::cout << "After insertions:\n";
    oaHashTable.display();
    
    // Search for some keys
    try {
        std::cout << "Value for 5: " << oaHashTable.search(5) << std::endl;
        std::cout << "Value for 15: " << oaHashTable.search(15) << std::endl;
        std::cout << "Value for 100: " << oaHashTable.search(100) << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    
    // Remove a key
    std::cout << "Removing 15..." << std::endl;
    oaHashTable.remove(15);
    
    // Display the table again
    std::cout << "After removal:\n";
    oaHashTable.display();
    
    // Test contains method
    std::cout << "Contains 5: " << (oaHashTable.contains(5) ? "Yes" : "No") << std::endl;
    std::cout << "Contains 15: " << (oaHashTable.contains(15) ? "Yes" : "No") << std::endl;
    
    // Test load factor
    std::cout << "Load factor: " << oaHashTable.loadFactor() << std::endl;
    std::cout << "Size: " << oaHashTable.size() << std::endl;
}

int main() {
    testHashTables();
    return 0;
}
