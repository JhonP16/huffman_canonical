// include/Huffman.h
#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

// -- INICIO: Definiciones movidas desde Node.h --

// Estructura para un nodo en el árbol de Huffman
struct Node {
    char symbol;
    int frequency;
    Node *left;
    Node *right;

    Node(char symbol, int frequency) : symbol(symbol), frequency(frequency), left(nullptr), right(nullptr) {}
    Node(int frequency, Node* left, Node* right) : symbol('\0'), frequency(frequency), left(left), right(right) {}

    ~Node() {
        delete left;
        delete right;
    }
};

// Comparador para la cola de prioridad (min-heap)
struct CompareNodes {
    bool operator()(const Node* a, const Node* b) const {
        if (a->frequency != b->frequency) {
            return a->frequency > b->frequency;
        }
        return a->symbol > b->symbol;
    }
};

// -- FIN: Definiciones movidas desde Node.h --


// Clase principal para manejar la lógica de Huffman
class Huffman {
public:
    Huffman();
    ~Huffman();
    void run(const std::string& text);

private:
    // ... (el resto de la declaración de la clase no cambia) ...
    std::unordered_map<char, int> frequencies;
    Node* root;
    std::map<char, std::string> huffmanCodes;
    std::map<char, int> codeLengths;
    std::map<char, std::string> canonicalCodes;
    std::string compressedData;

    void calculateFrequencies(const std::string& text);
    void buildTree();
    void generateCodes(Node* node, const std::string& code);
    void generateCanonicalCodes();
    void compressText(const std::string& text);

    void displayResults(const std::string& text);
    void displayFrequencyTable();
    void displayCodeTables();
    void displayTree();
    void printTree(const Node* node, const std::string& prefix, bool isRoot);
    void displayCompressedOutput();

    void clear();
};