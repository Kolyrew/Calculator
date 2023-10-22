#include <iostream>
#include "ConsReader.h"

int main() {
    ConsReader evaluator;

    while (true) {
        std::cout << "Enter expression (or 'exit' to quit): ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        try {
            double result = evaluator.Evaluate(input);
            std::cout << "Result: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}