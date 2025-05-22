#include "functions.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <array>
#include <map>

int main() {
    std::cout << "Welcome to Task Tracker, a CLI application!" << '\n';
    std::cout << "Enter \'help\' to see a list of commands" << "\n";

    std::map<std::string, std::array<std::string, 4>> task_lookup {};

    parse_file("tasks.json", task_lookup);

    while(true) {
        std::cout << "\n\n\033[\e[0;92mtask-cli\033[m" << " ";
        std::string user_input{};

        std::getline(std::cin >> std::ws, user_input);
        
        if (user_input == "help") {
            display_help_page();
        }

        if (user_input == "q") {
            break;
        }
        
        evaluate_cmd(user_input, task_lookup);

        
    }

    return 0;
}