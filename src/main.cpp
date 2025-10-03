// src/main.cpp
#include <iostream>
#include <string>
#include <limits>
#include <windows.h> // Incluir la cabecera de Windows

#include "Huffman.h"

// ... (el resto de tus funciones de utilidad) ...

void print_header(const std::string& title) {
    std::cout << "\n\n=====================================================\n";
    std::cout << "     " << title << "\n";
    std::cout << "=====================================================\n" << std::endl;
}

void display_menu() {
    std::cout << "\n--- Menu Principal de Compresion Huffman ---\n";
    std::cout << "1. Ejecutar los 3 casos de prueba requeridos\n";
    std::cout << "2. Ingresar una cadena de texto personalizada\n";
    std::cout << "3. Salir\n";
    std::cout << "-------------------------------------------\n";
    std::cout << "Seleccione una opcion: ";
}

int main() {
    // Configurar la consola para que soporte caracteres UTF-8
    SetConsoleOutputCP(CP_UTF8);

    Huffman huffman;
    int choice = 0;

    while (choice != 3) {
        display_menu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cout << "Error: Por favor, ingrese solo un numero.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                print_header("INICIANDO CASOS DE PRUEBA REQUERIDOS");

                std::string test_spaces = "este es un texto de prueba con muchos espacios para el algoritmo";
                print_header("Caso de prueba 1: Texto con muchos espacios");
                std::cout << "Entrada: \"" << test_spaces << "\"\n\n";
                huffman.run(test_spaces);

                std::string test_few_symbols = "aaaaaaaaaabbbbbbbbbbccccccccccdddddddddd";
                print_header("Caso de prueba 2: Texto con pocos simbolos distintos");
                std::cout << "Entrada: \"" << test_few_symbols << "\"\n\n";
                huffman.run(test_few_symbols);

                std::string test_uniform = "abcdefghijklmnopqrstuvwxyz0123456789.,!?";
                print_header("Caso de prueba 3: Texto con distribucion uniforme");
                std::cout << "Entrada: \"" << test_uniform << "\"\n\n";
                huffman.run(test_uniform);

                break;
            }
            case 2: {
                print_header("INGRESAR TEXTO PERSONALIZADO");
                std::cout << "Escriba el texto a comprimir (>= 30 caracteres) y presione Enter:\n> ";

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::string user_text;
                std::getline(std::cin, user_text);

                huffman.run(user_text);
                break;
            }
            case 3: {
                std::cout << "\nSaliendo del programa. ¡Hasta luego! 👋\n";
                break;
            }
            default: {
                std::cout << "\nOpcion no valida. Por favor, seleccione 1, 2 o 3.\n";
                break;
            }
        }
    }

    return 0;
}
