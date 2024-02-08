
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <cctype>
#include <map>
#include <sstream>
#include <list>
#include <set>
#include <cstdlib>
#include <ctime>

using namespace std;


// Struct to store word information
struct wordItem {
    string word;
    string documentName;
    int count;

    wordItem(string w, string doc, int c) : word(w), documentName(doc), count(c) {}
    
    // Needed for sorting and binary search
    bool operator<(const wordItem& other) const {
        return word < other.word; // Comparison based on 'word'
    }

    bool operator>(const wordItem& other) const {
        return word > other.word; // Comparison based on 'word'
    }
    
    bool operator==(const wordItem& other) const {
        return word == other.word;  // Equality comparison based on 'word'
    }

    bool operator<=(const wordItem& other) const {
        return *this < other || *this == other;  // Less than or equal comparison
    }
};
// Our Hashtable class
class HashTable {
private:
    vector<list<wordItem>> table;
    int currentSize;

    int getHash(const string &word) const {
        // Simple hash function (You may want to use a more complex one)
        int hash = 0;
        for (char ch : word) {
            hash = hash * 31 + ch;
        }
        return hash % table.size();
    }

    void rehash() {
        cout << "rehashed..." << endl << "previous table size: " << table.size() << ", new table size: ";
        vector<list<wordItem>> oldTable = table;
        table.resize(table.size() * 2);
        cout << table.size();
        currentSize = 0;
        for (auto &chain : oldTable) {
            for (auto &item : chain) {
                insert(item.word, item.documentName, item.count);
            }
        }
        float loadFactor = (float)currentSize / (float)table.size();
        cout << ", current unique word count: " << getUniqueWordCount() << ", current load factor: " << loadFactor << endl;
    }

public:
    HashTable(int size = 101) : table(size), currentSize(0) {}

    void insert(const string &word, const string &documentName, int count) {
        int index = getHash(word);
        bool found = false;

        // Check if word already exists in the hash table
        for (auto &item : table[index]) {
            if (item.word == word && item.documentName == documentName) {
//                item.count += count;
                item.count ++;
                found = true;
                break;
            }
        }

        // If not found, insert new WordItem
        if (!found) {
            table[index].push_back(wordItem(word, documentName, count));
            currentSize++;

            // Check load factor
            float loadFactor = (float)currentSize / (float)table.size();
            if (loadFactor > 0.9) { // Adjust load factor threshold as needed
                rehash();
            }
        }
        
    }

    wordItem* search(const string &word) const {
        int index = getHash(word);
        for (const auto &item : table[index]) {
            if (item.word == word) {
                // Return a pointer to the found wordItem
                return new wordItem(item.word, item.documentName, item.count);
            }
        }
        return nullptr; // Word not found
    }
    
    int getUniqueWordCount() const {
        std::set<string> uniqueWords;
        for (const auto &chain : table) {
            for (const auto &item : chain) {
                uniqueWords.insert(item.word);
            }
        }
        return uniqueWords.size();
    }

    
    void adjustTableSize() {
        float loadFactor = (float)currentSize / (float)table.size();

        if (loadFactor > 0.9) { // Adjust upper load factor threshold as needed
            rehash(true);  // Expand table
        } else if (loadFactor < 0.25 && table.size() > currentSize) {
            rehash(false); // Shrink table
        }
    }

    void rehash(bool increaseSize) {
        vector<list<wordItem>> oldTable = table;
        if (increaseSize) {
            table.resize(table.size() * 2); // Double the size
        } else {
            table.resize(table.size() / 2); // Halve the size
        }
        currentSize = 0;
        for (auto &chain : oldTable) {
            for (auto &item : chain) {
                insert(item.word, item.documentName, item.count);
            }
        }
    }
    
    // Method to print hash table state (for rehashing)
    void printState() {
        float loadFactor = (float)currentSize / (float)table.size();
        cout << ". Current load ratio is " << loadFactor << endl;
    }
};
//


// Our Hashtable class ends here...



//
struct DocumentItem {
    string documentName;
    int count;

    DocumentItem(const string& name, int c) : documentName(name), count(c) {}
};

struct WordItem {
    string word;
    map<string, int> documents; // Map of document names to counts
};


// Our BinaryTree class
template <class Key, class Value>
class BinaryTree {
private:
    struct Node {
        Key key;
        Value value;
        Node *left, *right;
        int height;

        Node(Key k, Value v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
    }; // Main struct

    Node* root;
    
    // Helper function for in-order traversal to count nodes
        int countNodes(Node* node) const {
            if (node == nullptr) {
                return 0;
            }
            return 1 + countNodes(node->left) + countNodes(node->right);
        }
    
    Node* minValueNode(Node* node);
    Node* insert(Node* node, Key key, Value value);
    Node* search(Node* node, Key key) const;
    void destroyTree(Node* node);

    // Tree traversal helpers
    void inOrderTraversal(Node* node) const;
    void preOrderTraversal(Node* node) const;
    void postOrderTraversal(Node* node) const;

public:
//    void deleteKey(const Key& key);
    BinaryTree() : root(nullptr) {}
    ~BinaryTree();

    void insert(Key key, Value value) {
        root = insert(root, key, value);
    }

    Value search(Key key) const {
        Node* result = search(root, key);
        if (result == nullptr) return nullptr;
        return result->value;
    }

    // Public traversal methods
    void inOrder() const {
        inOrderTraversal(root);
    }

    void preOrder() const {
        preOrderTraversal(root);
    }

    void postOrder() const {
        postOrderTraversal(root);
    }
    
    int getUniqueWordCount() const {
            return countNodes(root);
        }

};

// Returns the minimum value node, in this case the left most node
template <class Key, class Value>
typename BinaryTree<Key, Value>::Node* BinaryTree<Key, Value>::minValueNode(Node* node) {
    Node* current = node;
    while (current->left != nullptr)
        current = current->left;
    return current;
}

// Main function to insert a node into the tree
template <class Key, class Value>
typename BinaryTree<Key, Value>::Node* BinaryTree<Key, Value>::insert(Node* node, Key key, Value value) {
    if (node == nullptr) {
        return new Node(key, value);
    }

    if (key < node->key) {
        node->left = insert(node->left, key, value);
    } else if (key > node->key) {
        node->right = insert(node->right, key, value);
    } else {
        // Duplicate keys not allowed, or update the existing node's value
        return node;
    }
    return node;
}

// To traverse the tree and look for a certain node in the tree
template <class Key, class Value>
typename BinaryTree<Key, Value>::Node* BinaryTree<Key, Value>::search(Node* node, Key key) const {
    if (node == nullptr || node->key == key) {
        return node;
    }

    if (key < node->key) {
        return search(node->left, key);
    } else {
        return search(node->right, key);
    }
}

// Destructor and helper
template <class Key, class Value>
BinaryTree<Key, Value>::~BinaryTree() {
    destroyTree(root);
}

template <class Key, class Value>
void BinaryTree<Key, Value>::destroyTree(Node* node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

// Tree traversals
template <class Key, class Value>
void BinaryTree<Key, Value>::inOrderTraversal(Node* node) const {
    if (node != nullptr) {
        inOrderTraversal(node->left);
        cout << node->key << " ";
        inOrderTraversal(node->right);
    }
}

template <class Key, class Value>
void BinaryTree<Key, Value>::preOrderTraversal(Node* node) const {
    if (node != nullptr) {
        cout << node->key << " ";
        preOrderTraversal(node->left);
        preOrderTraversal(node->right);
    }
}

template <class Key, class Value>
void BinaryTree<Key, Value>::postOrderTraversal(Node* node) const {
    if (node != nullptr) {
        postOrderTraversal(node->left);
        postOrderTraversal(node->right);
        cout << node->key << " ";
    }
}


// A standard function to convert our string to lowercase
string toLowerCase(const string& str) {
    string lowerCaseStr = str;
    transform(lowerCaseStr.begin(), lowerCaseStr.end(), lowerCaseStr.begin(), ::tolower);
    return lowerCaseStr;
}

// To filter our query from all unnecessary punctuation and numbers
string filterAlpha(const string& str) {
    string filteredStr;
    bool lastWasAlpha = true; // Tracks if the last character added was alphabetic

    for (char ch : str) {
        if (isalpha(ch)) {
            filteredStr += ch;
            lastWasAlpha = true;
        } else if (lastWasAlpha) {
            filteredStr += ' '; // Add a space in place of a non-alphabetic character
            lastWasAlpha = false;
        }
    }

    // Optional: Trim trailing space if any
    if (!filteredStr.empty() && filteredStr.back() == ' ') {
        filteredStr.pop_back();
    }

    return filteredStr;
}


void updateDocumentCount(WordItem* wordItem, const string& documentName) {
    auto& documents = wordItem->documents;
    documents[documentName]++;
}



void preprocessDocument(const string& documentName, BinaryTree<string, WordItem*>& myTree, HashTable& myHashTable, vector <wordItem> &list) {
    ifstream file(documentName);
    if (!file.is_open()) {
        cerr << "Unable to open file: " << documentName << endl;
        return;
    }

    stringstream buffer;
    buffer << file.rdbuf(); // Read the whole file into a string buffer
    string content = buffer.str();

    content = toLowerCase(content); // Convert the whole content to lowercase
    content = filterAlpha(content); // Filter out non-alphabetic characters

    istringstream wordsStream(content);
    string word;
    while (wordsStream >> word) {
        if (!word.empty()) {
            WordItem* wordItem = myTree.search(word);
            if (wordItem == nullptr) {
                // Create a new WordItem if it doesn't exist
                wordItem = new WordItem;
                wordItem->word = word;
                wordItem->documents[documentName] = 1; // Initialize the document count
                myTree.insert(word, wordItem);
                list.emplace_back(word, documentName, 1);
            } else {
                // Update the document count in the WordItem
                updateDocumentCount(wordItem, documentName);
                
            }
            myHashTable.insert(word, documentName, 1);
        }
    }
    file.close();
}



// Function to display the output results
void displayResults(const map<string, map<string, int>>& results) {
    for (const auto& docPair : results) {
        cout << "in Document " << docPair.first << ", ";
        bool firstWord = true;
        for (const auto& wordCountPair : docPair.second) {
            if (!firstWord) {
                cout << ", ";
            }
            cout << wordCountPair.first << " found " << wordCountPair.second << " times";
            firstWord = false;
        }
        cout << "." << endl;
    }

    // If no documents contain the query
    if (results.empty()) {
        cout << "No document contains the given query" << endl;
    }
}

// Function to process the query and call the display results function
void processQuery(const string& query, const BinaryTree<string, WordItem*>& myTree, int x) {
    istringstream iss(query);
    vector<string> words((istream_iterator<string>(iss)), istream_iterator<string>());

    map<string, map<string, int>> allResults; // Map of document names to word counts for all words in the query

    for (const string& word : words) {
        string lowerWord = toLowerCase(word);
        WordItem* wordItem = myTree.search(lowerWord);

        if (wordItem != nullptr) {
            for (const auto& docPair : wordItem->documents) {
                allResults[docPair.first][lowerWord] = docPair.second;
            }
        }
    }
    if (x ==0){
        displayResults(allResults);
        displayResults(allResults);
        displayResults(allResults);
    }
}

void processQueryHashTable(const string& query, const HashTable& myHashTable, int x) {
    istringstream iss(query);
    vector<string> words((istream_iterator<string>(iss)), istream_iterator<string>());

    map<string, map<string, int>> results; // Map of document names to word counts

    for (const string& word : words) {
        string lowerWord = toLowerCase(word);

        // Search in Hash Table
        wordItem* hashTableItem = myHashTable.search(lowerWord);
        if (hashTableItem != nullptr) {
            // Update the count for each document
            results[hashTableItem->documentName][lowerWord] = hashTableItem->count;
            delete hashTableItem; // Important to prevent memory leaks
        }
    }
}

// Binary search function to find a word in the sorted vector
bool binarySearchWord(const vector<wordItem>& list, const string& wordToFind) {
    int left = 0;
    int right = list.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        const wordItem& midWordItem = list[mid];

        if (midWordItem.word == wordToFind) {
            return true; // Word found
        }

        if (midWordItem.word < wordToFind) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return false; // Word not found
}

// Function to search for query words in the vector
void searchQueryWordsInVector(const vector<wordItem>& list, const string& query) {
    istringstream iss(query);
    string word;
    vector<string> foundWords; // To store found words

    while (iss >> word) {
        if (binarySearchWord(list, word)) {
            // Word found in vector, add it to the foundWords vector
            foundWords.push_back(word);
        }
    }

    // Here, you can process the foundWords vector as needed
    // For example, you could print the found words
//    for (const string& foundWord : foundWords) {
//        cout << "Found word: " << foundWord << endl;
//    }
}

// Functions to quick sort first pivot
int partition(vector<wordItem>& list, int low, int high) {
    wordItem pivot = list[low];
    int i = low;

    for (int j = low + 1; j <= high; j++) {
        if (list[j] < pivot) {
            i++;
            std::swap(list[i], list[j]);
        }
    }
    std::swap(list[i], list[low]);
    return i;
}

void quickSort(vector<wordItem>& list, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(list, low, high);

        quickSort(list, low, pivotIndex - 1);
        quickSort(list, pivotIndex + 1, high);
    }
}

vector<wordItem> quickSortFirstPivot(vector<wordItem> list) {
    quickSort(list, 0, list.size() - 1);
    return list;
}


// Functions to quick sort random pivot
int partitionRandom(vector<wordItem>& list, int low, int high) {
    // Randomly select a pivot and swap it with the first element
    int randomPivotIndex = low + std::rand() % (high - low + 1);
    std::swap(list[low], list[randomPivotIndex]);

    wordItem pivot = list[low];
    int i = low;

    for (int j = low + 1; j <= high; j++) {
        if (list[j] < pivot) {
            i++;
            std::swap(list[i], list[j]);
        }
    }
    std::swap(list[i], list[low]);
    return i;
}

void quickSortRandomPivotHelper(vector<wordItem>& list, int low, int high) {
    if (low < high) {
        int pivotIndex = partitionRandom(list, low, high);

        quickSortRandomPivotHelper(list, low, pivotIndex - 1);
        quickSortRandomPivotHelper(list, pivotIndex + 1, high);
    }
}

vector<wordItem> quickSortRandomPivot(vector<wordItem> list) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));  // Seed for random number generator
    quickSortRandomPivotHelper(list, 0, list.size() - 1);
    return list;
}

// Functions to quick sort median pivot
int medianOfThree(const vector<wordItem>& list, int low, int high) {
    int mid = low + (high - low) / 2;
    if (list[mid] < list[low])
        std::swap(mid, low);
    if (list[high] < list[low])
        std::swap(high, low);
    if (list[high] < list[mid])
        std::swap(high, mid);
    return mid;
}

int partitionMedian(vector<wordItem>& list, int low, int high) {
    int medianPivotIndex = medianOfThree(list, low, high);
    std::swap(list[low], list[medianPivotIndex]);

    wordItem pivot = list[low];
    int i = low;

    for (int j = low + 1; j <= high; j++) {
        if (list[j] < pivot) {
            i++;
            std::swap(list[i], list[j]);
        }
    }
    std::swap(list[i], list[low]);
    return i;
}

void quickSortMedianPivotHelper(vector<wordItem>& list, int low, int high) {
    if (low < high) {
        int pivotIndex = partitionMedian(list, low, high);

        quickSortMedianPivotHelper(list, low, pivotIndex - 1);
        quickSortMedianPivotHelper(list, pivotIndex + 1, high);
    }
}

vector<wordItem> quickSortMedianPivot(vector<wordItem> list) {
    quickSortMedianPivotHelper(list, 0, list.size() - 1);
    return list;
}

// Function to insert sort
vector<wordItem> insertionSort(vector<wordItem> list) {
    int n = list.size();
    for (int i = 1; i < n; i++) {
        wordItem key = list[i];
        int j = i - 1;

        // Move elements of list[0..i-1], that are greater than key, to one position ahead of their current position
        // Using the '<' operator for comparison instead of '>'
        while (j >= 0 && key < list[j]) {
            list[j + 1] = list[j];
            j = j - 1;
        }
        list[j + 1] = key;
    }
    return list;
}

// Function to merge sort
void mergeInPlace(vector<wordItem>& list, int left, int mid, int right) {
    int start2 = mid + 1;

    // If the direct merge is already sorted
    if (list[mid] <= list[start2]) {
        return;
    }

    // Two pointers to maintain start of both parts to merge
    while (left <= mid && start2 <= right) {
        // If element 1 is in right place
        if (list[left] <= list[start2]) {
            left++;
        } else {
            wordItem value = list[start2];
            int index = start2;

            // Shift all the elements between element 1 and element 2, right by 1.
            while (index != left) {
                list[index] = list[index - 1];
                index--;
            }
            list[left] = value;

            // Update all the pointers
            left++;
            mid++;
            start2++;
        }
    }
}

void mergeSortInPlaceHelper(vector<wordItem>& list, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Sort first and second halves
        mergeSortInPlaceHelper(list, left, mid);
        mergeSortInPlaceHelper(list, mid + 1, right);

        mergeInPlace(list, left, mid, right);
    }
}

vector<wordItem> mergeSortInPlace(vector<wordItem> list) {
    mergeSortInPlaceHelper(list, 0, list.size() - 1);
    return list;
}

// Function to heap sort
void heapify(vector<wordItem>& list, int n, int i) {
    int largest = i; // Initialize largest as root
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // If left child is larger than root
    if (left < n && list[left] > list[largest])
        largest = left;

    // If right child is larger than largest so far
    if (right < n && list[right] > list[largest])
        largest = right;

    // If largest is not root
    if (largest != i) {
        std::swap(list[i], list[largest]);

        // Recursively heapify the affected sub-tree
        heapify(list, n, largest);
    }
}

vector<wordItem> heapSort(vector<wordItem> list) {
    int n = list.size();

    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(list, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        std::swap(list[0], list[i]);

        // Call max heapify on the reduced heap
        heapify(list, i, 0);
    }

    return list;
}

// Main code starts here...
int main() {
    vector <wordItem> list;
    BinaryTree<string, WordItem*> myTree; // our BinaryTree is created
    HashTable myTable;
        int numOfFiles;
        cout << "Enter number of input files: ";
        cin >> numOfFiles;
    
        cin.ignore(); // To ignore the newline after reading numOfFiles
    
        vector<string> fileNames(numOfFiles);
        // First loop: Collect all filenames
        for (int i = 0; i < numOfFiles; ++i) {
            cout << "Enter " << (i + 1) << ". file name: ";
            getline(cin, fileNames[i]);
        }

        // Second loop: Preprocess each document
        for (int i = 0; i < numOfFiles; ++i) {
            preprocessDocument(fileNames[i], myTree, myTable, list);
        }
        sort(list.begin(), list.end());
        cout << endl << "After preprocessing, the unique word count is " << myTree.getUniqueWordCount();
        myTable.printState();
        string query;
        cout << "Enter queried words in one line: ";
        getline(cin, query);
        query = toLowerCase(query);
        query = filterAlpha(query);
        processQuery(query, myTree, 0);
        processQueryHashTable(query, myTable, 0);
    
        int k = 100;
        // Binary tree time
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < k; i++) {
        processQuery(query, myTree, 1); }
        auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
        cout << "\nBinary Search Tree Time: " << BSTTime.count() / k << "\n";
    
        // Hash Table Time
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < k; i++)
        {
        processQueryHashTable(query, myTable, 1);
        }
        auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
        cout << "Hash Table Time: " << HTTime.count() / k << "\n";
    
        // Binary Search Time
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < k; i++)
        {
            searchQueryWordsInVector(list, query);
        }
        auto BSTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
        cout << "Binary Search Time: " << BSTime.count() / k << "\n";
    
        // Quick Sort Median Time
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < k; i++)
        {
            quickSortMedianPivot(list);
        }
        auto QSMTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
        cout << "Quick Sort(median) Time: " << QSMTime.count() / k << "\n";
        
        // Quick Sort Random Time
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < k; i++)
        {
            quickSortRandomPivot(list);
        }
        auto QSRTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
        cout << "Quick Sort(random) Time: " << QSRTime.count() / k << "\n";
    
        // Quick Sort First Pivot Time
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 2; i++)
        {
            quickSortFirstPivot(list);
        }
        auto QSFPTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
        cout << "Quick Sort(first) Time: " << QSFPTime.count() / 2 << "\n";
    
        // Merge Sort Time
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < k; i++)
        {
            mergeSortInPlace(list);
        }
        auto MSTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
        cout << "Merge Sort Time: " << MSTime.count() / k << "\n";
        
        // Heap Sort Time
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < k; i++)
        {
            heapSort(list);
        }
        auto HSTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
        cout << "Heap Sort Time: " << HSTime.count() / k << "\n";
        
        // Insertion Sort Time
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < k; i++)
        {
            insertionSort(list);
        }
        auto ISTime = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
        cout << "Insertion Sort Time: " << ISTime.count() / k << "\n" << "\n";
        
        // Comparing Speeds now...
        double speed = (double) BSTTime.count()/ (double) HTTime.count();
        cout << "Speed Up BST/HST: " << speed << endl;
        speed = (double) MSTime.count()/ (double) QSMTime.count();
        cout << "Speed Up Merge Sort/Quick Sort(Median): " << speed*100 << endl;
        speed = (double) HSTime.count()/ (double) QSMTime.count();
        cout << "Speed Up Heap Sort/Quick Sort(Median): " << speed << endl;
        speed = (double) ISTime.count()/ (double) QSMTime.count();
        cout << "Speed Up Insertion Sort/Quick Sort(Median): " << speed*100 << endl;
        speed = (double) BSTTime.count()/ (double) BSTime.count();
        cout << "Speed Up Binary Search Tree Time/Binary Search: " << speed << endl;
        speed = (double) HSTime.count()/ (double) BSTTime.count();
        cout << "Speed Up Hash Table Time/Binary Search Tree Time: " << speed << endl;
    
        return 0;
}




