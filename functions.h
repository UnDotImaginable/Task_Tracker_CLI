#ifndef FUNCTIONS_FILE
#define FUNCTIONS_FILE

#include <unordered_map>
#include <string>
#include <array>
#include <unordered_set>
#include <map>

std::string getCurrentDateTime();

void parse_file(std::string filename, std::map<std::string, std::array<std::string, 4>>& map);

bool is_number(const std::string& s);

void display_help_page();

void evaluate_cmd(
    const std::string& usr_cmd, 
    std::map<std::string, 
    std::array<std::string, 4>>& map
);

bool check_validity(
    const std::unordered_set<std::string>& glossary, 
    const std::unordered_set<std::string>& filters, 
    const std::vector<std::string>& pieces
);

void perform_command(
    const std::unordered_set<std::string>& glossary, 
    const std::unordered_set<std::string>& filters, 
    const std::vector<std::string>& pieces,
    std::map<std::string, std::array<std::string, 4>>& map
);

void write_to_file(
    std::string filename, 
    std::map<std::string, std::array<std::string, 4>>& map
);

#endif