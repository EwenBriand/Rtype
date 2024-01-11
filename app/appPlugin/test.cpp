#include <iostream>
#include <string>

int main() {
    std::string command;

    while (true) {
        std::cout << "[C++ Terminal] Enter a command: ";
        std::getline(std::cin, command);

        if (command == "exit") {
            break;
        }

        std::cout << "[C++ Terminal] Executing command: " << command << std::endl;
    }

    return 0;
}