// src/Huffman.cpp
#include "Huffman.h" // Note: Use quotes for local headers
#include <iostream>
#include <queue>
#include <algorithm>
#include <iomanip>

// Constructor
Huffman::Huffman() : root(nullptr) {}

// Destructor
Huffman::~Huffman() {
    delete root; // The node's destructor is recursive
}

// Clear previous data for a new compression task
void Huffman::clear() {
    delete root;
    root = nullptr;
    frequencies.clear();
    huffmanCodes.clear();
    codeLengths.clear();
    canonicalCodes.clear();
    compressedData.clear();
}

// Main public method
void Huffman::run(const std::string& text) {
    if (text.length() < 30) {
        std::cerr << "Error: Input text must be at least 30 characters long." << std::endl;
        return;
    }

    clear(); // Clear any previous state

    // 1. Calculate frequencies
    calculateFrequencies(text);

    // 2. Build the Huffman Tree
    buildTree();

    // 3. Generate non-canonical codes and their lengths
    generateCodes(root, "");

    // 4. Generate canonical codes
    generateCanonicalCodes();

    // 5. Compress the text
    compressText(text);

    // 6. Display all results
    displayResults(text);
}

// Step 1: Calculate character frequencies
void Huffman::calculateFrequencies(const std::string& text) {
    for (char c : text) {
        frequencies[c]++;
    }
}

// Step 2: Build the Huffman Tree using a priority queue
void Huffman::buildTree() {
    std::priority_queue<Node*, std::vector<Node*>, CompareNodes> pq;

    for (auto const& [symbol, freq] : frequencies) {
        pq.push(new Node(symbol, freq));
    }

    if (pq.size() == 1) {
        Node* leaf = pq.top();
        pq.pop();
        root = new Node(leaf->frequency, leaf, nullptr);
        return;
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        int combinedFreq = left->frequency + right->frequency;
        Node* internalNode = new Node(combinedFreq, left, right);
        pq.push(internalNode);
    }

    root = pq.top();
}

// Step 3: Generate non-canonical codes and lengths via DFS traversal
void Huffman::generateCodes(Node* node, const std::string& code) {
    if (!node) return;

    if (!node->left && !node->right) {
        if (code.empty()) { // Handle single-symbol case
            huffmanCodes[node->symbol] = "0";
            codeLengths[node->symbol] = 1;
        } else {
            huffmanCodes[node->symbol] = code;
            codeLengths[node->symbol] = code.length();
        }
        return;
    }
    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

// Step 4: Construct canonical Huffman codes
void Huffman::generateCanonicalCodes() {
    std::vector<std::pair<int, char>> sortedSymbols;
    for (auto const& [symbol, length] : codeLengths) {
        sortedSymbols.push_back({length, symbol});
    }

    std::sort(sortedSymbols.begin(), sortedSymbols.end());

    unsigned long long currentCode = 0;
    int currentLength = 0;

    for (const auto& pair : sortedSymbols) {
        int length = pair.first;
        char symbol = pair.second;

        if (currentLength == 0) {
            currentLength = length;
        } else {
            currentCode++;
            if (length > currentLength) {
                currentCode <<= (length - currentLength);
                currentLength = length;
            }
        }

        std::string binaryCode;
        unsigned long long tempCode = currentCode;
        for (int i = 0; i < currentLength; ++i) {
            binaryCode = (tempCode % 2 == 0 ? "0" : "1") + binaryCode;
            tempCode /= 2;
        }

        while (binaryCode.length() < (size_t)currentLength) {
            binaryCode.insert(0, "0");
        }

        canonicalCodes[symbol] = binaryCode;
    }
}

// Step 5: Encode the input text using canonical codes
void Huffman::compressText(const std::string& text) {
    for (char c : text) {
        compressedData += canonicalCodes[c];
    }
}

// Step 6: Display all required information
void Huffman::displayResults(const std::string& text) {
    size_t originalSize = text.length() * 8;
    size_t compressedSize = compressedData.length();
    double ratio = static_cast<double>(compressedSize) / originalSize;
    double reduction = 100.0 * (1.0 - ratio);

    std::cout << "--- Compression Report ---\n";
    std::cout << "Original Size: " << originalSize << " bits (" << text.length() << " chars)\n";
    std::cout << "Compressed Size: " << compressedSize << " bits\n";
    std::cout << "Compression Ratio: " << std::fixed << std::setprecision(2) << ratio << "\n";
    std::cout << "Space Reduction: " << std::fixed << std::setprecision(2) << reduction << "%\n";
    std::cout << "--------------------------" << std::endl << std::endl;

    displayFrequencyTable();
    displayCodeTables();
    displayTree();
    displayCompressedOutput();
}

void Huffman::displayFrequencyTable() {
    std::cout << "--- Frequency Table ---\n";
    std::cout << std::left << std::setw(10) << "Symbol" << std::setw(10) << "Freq" << "\n";
    std::cout << "---------------------\n";
    std::map<char, int> sortedFreq(frequencies.begin(), frequencies.end());
    for (auto const& [symbol, freq] : sortedFreq) {
        std::cout << std::left << std::setw(10) << (symbol == ' ' ? "'space'" : std::string(1, symbol))
                  << std::setw(10) << freq << "\n";
    }
    std::cout << std::endl;
}

void Huffman::displayCodeTables() {
    std::cout << "--- Code Tables ---\n";
    std::cout << std::left << std::setw(10) << "Symbol"
              << std::setw(10) << "Length"
              << std::setw(20) << "Huffman Code"
              << std::setw(20) << "Canonical Code" << "\n";
    std::cout << "-----------------------------------------------------\n";

    std::vector<std::pair<int, char>> sortedSymbols;
    for (auto const& [symbol, length] : codeLengths) {
        sortedSymbols.push_back({length, symbol});
    }
    std::sort(sortedSymbols.begin(), sortedSymbols.end());

    for (const auto& pair : sortedSymbols) {
        char symbol = pair.second;
        std::cout << std::left << std::setw(10) << (symbol == ' ' ? "'space'" : std::string(1, symbol))
                  << std::setw(10) << codeLengths.at(symbol)
                  << std::setw(20) << huffmanCodes.at(symbol)
                  << std::setw(20) << canonicalCodes.at(symbol) << "\n";
    }
    std::cout << std::endl;
}

void Huffman::displayTree() {
    std::cout << "--- Huffman Tree (Textual Representation) ---\n";
    printTree(root, "", true); // Iniciar la impresión recursiva
    std::cout << std::endl;
}

void Huffman::printTree(const Node* node, const std::string& prefix, bool isRoot) {
    if (!node) return;

    // Imprime el nodo actual
    std::cout << prefix;
    if (!isRoot) {
        std::cout << "----";
    }

    if (node->symbol != '\0') {
        // Es un nodo hoja (representa un caracter)
        std::cout << "'" << (node->symbol == ' ' ? ' ' : node->symbol) << "' (" << node->frequency << ")\n";
    } else {
        // Es un nodo interno
        std::cout << "($) (" << node->frequency << ")\n";
    }

    // Llamadas recursivas para los hijos
    if (node->left || node->right) {
        std::string childPrefix = prefix;
        if (isRoot) {
            childPrefix = " "; // Indentación inicial para los hijos de la raíz
        } else {
            childPrefix += "     "; // Aumenta la indentación para niveles más profundos
        }

        // Imprime primero el hijo izquierdo y luego el derecho
        printTree(node->left, childPrefix + "L", false);
        printTree(node->right, childPrefix + "R", false);
    }
}

void Huffman::displayCompressedOutput() {
    std::cout << "--- Compressed Output ---\n";
    std::cout << "Total bits: " << compressedData.length() << "\n";
    std::cout << "First 128 bits: " << compressedData.substr(0, 128) << (compressedData.length() > 128 ? "..." : "") << "\n";
    std::cout << std::endl;
}