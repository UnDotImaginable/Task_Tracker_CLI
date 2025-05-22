#include "functions.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <ctime>
#include <chrono>
#include <iomanip>


static const std::unordered_set<std::string> glossary {"add", "update", "delete", "mark-in-progress", "mark-done", "list"};
static const std::unordered_set<std::string> list_filters {"done", "todo", "in-progress"};
int id_generator { 0 };

void parse_file(std::string filename, std::map<std::string, std::array<std::string, 4>>& map) {
    std::ifstream receive("tasks.json");
    std::string cur_line;

    if (receive.is_open()) {

        std::string id {};
        std::string description {};
        std::string status {};
        std::string createdAt {};
        std::string modifiedAt {};

        while (getline(receive, cur_line)) {
            if (!(cur_line.length() == 1 || cur_line.length() == 2)) {
            
                // Find the index of the first letter of the label
                int label_start = cur_line.find('\"') + 1; // Start at 0
                int label_end = cur_line.find('\"', label_start); // Start at label_start
                
                std::string label = cur_line.substr(label_start, label_end - label_start);
                std::string content {};

             
                if (label.find("id") != std::string::npos) {
                    content = cur_line.substr(label_end + 3);
                    content.erase(1, 1);
                    id = content;
                    id_generator = std::stoi(content);
                }

                else {
                    
                    if (label.find("description") != std::string::npos) {
                        int content_start = cur_line.find('\"', label_end + 1) + 1;
                        int content_end = cur_line.find('\"', content_start);
                        content = cur_line.substr(content_start, content_end - content_start);

                        description = '\"' + content + "\"";
                    }
                    
                    else if (label.find("status") != std::string::npos) {
                        int content_start = cur_line.find('\"', label_end + 1) + 1;
                        int content_end = cur_line.find('\"', content_start);
                        content = cur_line.substr(content_start, content_end - content_start);

                        status = content;
                    }

                    else if (label.find("createdAt") != std::string::npos) {
                        int content_start = cur_line.find('\"', label_end + 1) + 1;
                        int content_end = cur_line.find('\"', content_start);
                        content = cur_line.substr(content_start, content_end - content_start);

                        createdAt = content;
                    }

                    else if (label.find("modifiedAt") != std::string::npos) {
                        int content_start = cur_line.find('\"', label_end + 1) + 1;
                        int content_end = cur_line.find('\"', content_start);
                        content = cur_line.substr(content_start, content_end - content_start);

                        modifiedAt = content;
                    }
                }

                map[id] = {description, status, createdAt, modifiedAt};

            }



        }
    }

}


std::string getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_c);

    std::ostringstream oss;
    oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

bool is_number(const std::string& s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

void display_help_page() {

    std::cout << '\n' << "q - Quit program and exit" << std::endl;

    std::cout << "add [\"task_to_add\"]" << std::endl;
    std::cout << '\t' << "Auto-generates a task ID which is used for identification later" << std::endl;

    std::cout << "update [task_ID] [\"task_to_update_to\"]" << std::endl;
    std::cout << '\t' << "Update task to new task" << std::endl;
    
    std::cout << "delete [task_ID]" << std::endl;
    std::cout << '\t' << "delete task associated with this ID. If ID invalid, action is cancelled" << std::endl;

    std::cout << "mark-in-progress [task_ID]" << std::endl;
    std::cout << '\t' << "mark this task as being \'in progress\'" << std::endl;

    std::cout << "mark-done [task_ID]" << std::endl;
    std::cout << '\t' << "mark this task as done" << std::endl;

    std::cout << "list [filter]" << std::endl;
    std::cout << '\t' << "done - List only tasks that are done" << std::endl;
    std::cout << '\t' << "todo - List only tasks whose status is unmodified" << std::endl;
    std::cout << '\t' << "in-progress - List only tasks that are \'in progress\'" << std::endl;

    std::cout << '\n';
}

// We're just checking the format of the user's command, not whether or not it can be performed. We'll save that for later...
bool check_validity(
    const std::unordered_set<std::string>& glossary, 
    const std::unordered_set<std::string>& filters, 
    const std::vector<std::string>& pieces
) {
    std::string first_piece { pieces[0] };
    std::string last_piece { pieces[pieces.size() - 1] };

    auto it = glossary.find(first_piece);

    if (it != glossary.end()) {

        if (*it == "add") {
            std::cout << "Keyword " << *it << " is valid" << std::endl;

            if (pieces.size() == 2) {
                if (last_piece.front() != '\"') {
                    std::cout << "Task string is missing an opening quotation mark" << std::endl;
                }
                if (last_piece.back() != '\"') {
                    std::cout << "Task string is missing a closing quotation mark" << std::endl;
                }
            }

            else {
                if (pieces[1].front() != '\"') {
                    std::cout << "Task string is missing an opening quotation mark" << std::endl;
                }
                if (last_piece.back() != '\"') {
                    std::cout << "Task string is missing a closing quotation mark" << std::endl;
                }
            }

        }

        else if (*it == "update") {
            std::cout << "Keyword " << *it << " is valid" << std::endl;

            // Check for negative values will be for a future function
            if (!is_number(pieces[1])) {
                std::cout << "Task ID is not a number" << '\n';
                return false;
            }
            
            // Check format of task string
            if (pieces[2].front() != '\"') {
                std::cout << "Task string missing an opening quotation mark" << std::endl;
                return false;
            }

            if (last_piece.front() == '\"') {
                if (last_piece.back() != '\"' || last_piece[last_piece.length() - 2] != '\"') {
                    std::cout << "Task string is missing a closing quotation mark" << std::endl;
                    return false;
                }
            }

            if (last_piece.back() != '\"') {
                std::cout << "Task string is missing a closing quotation mark" << std::endl;
                return false;
            }
            


            // for (const auto& piece : pieces) {
            //     std::cout << piece << '\n';
            // }
        }
    
        else if (*it == "delete") { 
            std::cout << "Keyword " << *it << " is valid" << '\n';

            if (!is_number(pieces[1])) {
                std::cout << "Task ID is not a number" << '\n';
                return false;
            }
        }
    
        else if (*it == "mark-in-progress") {
            std::cout << "Keyword " << *it << " is valid" << '\n';

            if (!is_number(pieces[1])) {
                std::cout << "Task ID is not a number" << '\n';
                return false;
            }
        }
    
        else if (*it == "mark-done") {
            std::cout << "Keyword " << *it << " is valid" << '\n';

            if (!is_number(pieces[1])) {
                std::cout << "Task ID is not a number" << '\n';
                return false;
            }
        }
    
        else if (*it == "list") {
            std::cout << "Keyword " << *it << " is valid" << '\n';

            if (pieces.size() == 1) {
                std::cout << "No modifier applied" << '\n';
                return true;
            }

            if (filters.count(pieces[1]) == 0) {
                std::cout << "Modifier not found" << '\n';
                return false;
            }
        }

    }

    else {
        std::cout << "Keyword " << pieces[0] << " is invalid" << '\n';
        return false;
    }

    return true;
}

void evaluate_cmd(
    const std::string& usr_cmd, 
    std::map<std::string, std::array<std::string, 4>>& map
) {

    std::vector<std::string> cmd_pieces;

    std::string hold {};
    std::stringstream usr_cmd_stream {usr_cmd};


    while (usr_cmd_stream >> hold) {
        cmd_pieces.push_back(hold);
    }

    bool validity = check_validity(glossary, list_filters, cmd_pieces);

    if (!validity) {
        std::cout << "This isn't valid!" << '\n';
    }
    else {
        perform_command(glossary, list_filters, cmd_pieces, map);
    }

}

// If we get to here, that means that the format is valid and the user's command MAY be performed (except for if the task ID is invalid)
void perform_command(
    const std::unordered_set<std::string>& glossary, 
    const std::unordered_set<std::string>& filters, 
    const std::vector<std::string>& pieces,
    std::map<std::string, std::array<std::string, 4>>& map
) {

// add, update, delete, mark-in-progress, mark-done, list (todo, done, in-progress)
std::string filename{ "tasks.json" };
std::ofstream send(filename);

// In this scenario, "map" is used to hold all of the new tasks that we are creating
if (pieces[0] == "add") {
    //std::cout << "Add command detected" << std::endl;

    std::string task_ID { std::to_string(++id_generator) };
    std::string description { "" };

    for (int i = 1; i < pieces.size(); i++) {
        //std::cout << pieces[i] << " ";
        if (i == pieces.size() - 1) {
            description += pieces[i];
        }
        else {
            description += pieces[i];
            description += " ";
        }

    }

    std::string cur_dt = getCurrentDateTime();

    map[task_ID] = {description, "todo", cur_dt, cur_dt};

    write_to_file(filename, map);

}

// update 1 "Buy groceries and cook dinner"
if (pieces[0] == "update") {

    std::string new_desc { "" };
    for (int i = 2; i < pieces.size(); i++) {
        if (i == pieces.size() - 1) {
            new_desc += pieces[i];
        } 
        else {
            new_desc += pieces[i] + " ";
        }
    }

    std::cout << new_desc << std::endl;
    for (auto& pair : map) {
        if (pieces[1] == pair.first) {
            pair.second[0] = new_desc;
            pair.second[3] = getCurrentDateTime();
        }
    }

    write_to_file(filename, map);

}

if (pieces[0] == "delete") {

    if (map.find(pieces[1]) != map.end()) {
        map.erase(pieces[1]);        
    } else {
        std::cout << "Key does not exist" << std::endl;
    }

    write_to_file(filename, map);

}

if (pieces[0] == "mark-in-progress") {

    if (map.find(pieces[1]) != map.end()) {
        for (auto& pair : map) {
            if (pieces[1] == pair.first) {
                pair.second[1] = "in-progress";
                pair.second[3] = getCurrentDateTime();
            }
        }    
    } else {
        std::cout << "Key does not exist" << std::endl;
    }

    write_to_file(filename, map);    
}

if (pieces[0] == "mark-done") {

    if (map.find(pieces[1]) != map.end()) {
        for (auto& pair : map) {
            if (pieces[1] == pair.first) {
                pair.second[1] = "done";
                pair.second[3] = getCurrentDateTime();
            }
        }    
    } else {
        std::cout << "Key does not exist" << std::endl;
    }

    write_to_file(filename, map);    
}

if (pieces[0] == "list") {
    if(pieces.size() == 1) {

        std::cout << "\n\n";
        for (const auto& pair : map) {
            std::cout << "id: " << pair.first << std::endl;
            std::cout << "description: " << pair.second[0] << std::endl;
            std::cout << "status: " << pair.second[1] << std::endl;
            std::cout << "createdAt: " << pair.second[2] << std::endl;
            std::cout << "modifiedAt: " << pair.second[3] << '\n' << std::endl;

        }
    }
    else {
        if (list_filters.find(pieces[1]) != list_filters.end()) {

            if (pieces[1] == "done") {
                for (const auto& pair : map) {
                    if (pair.second[1] == "done") {
                        std::cout << "id: " << pair.first << std::endl;
                        std::cout << "description: " << pair.second[0] << std::endl;
                        std::cout << "status: " << pair.second[1] << std::endl;
                        std::cout << "createdAt: " << pair.second[2] << std::endl;
                        std::cout << "modifiedAt: " << pair.second[3] << '\n' << std::endl;
                    }
                }
            }

            if (pieces[1] == "todo") {
                for (const auto& pair : map) {
                    if (pair.second[1] == "todo") {
                        std::cout << "id: " << pair.first << std::endl;
                        std::cout << "description: " << pair.second[0] << std::endl;
                        std::cout << "status: " << pair.second[1] << std::endl;
                        std::cout << "createdAt: " << pair.second[2] << std::endl;
                        std::cout << "modifiedAt: " << pair.second[3] << '\n' << std::endl;
                    }
                }
            }

            if (pieces[1] == "in-progress") {
                for (const auto& pair : map) {
                    if (pair.second[1] == "in-progress") {
                        std::cout << "id: " << pair.first << std::endl;
                        std::cout << "description: " << pair.second[0] << std::endl;
                        std::cout << "status: " << pair.second[1] << std::endl;
                        std::cout << "createdAt: " << pair.second[2] << std::endl;
                        std::cout << "modifiedAt: " << pair.second[3] << '\n' << std::endl;
                    }
                }
            }

        }

        else {
            std::cout << "List filter is invalid" << std::endl;
        }
    }

    write_to_file(filename, map);
}

}

void write_to_file(
    std::string filename, 
    std::map<std::string, std::array<std::string, 4>>& map
){

    std::ofstream send(filename);
    send << '[' << '\n';

    auto last = std::prev(map.end());

    for (auto it = map.begin(); it != map.end(); ++it) {
        send << '\t' << '{' << '\n';
        send << "\t\t" << '\"' << "id" << '\"' << ':' << " " << it->first << ',' << '\n';

        //send << "id: " << it->first << std::endl;

        for (int i = 0; i < 4; i++) {
            if (i == 0) {
                send << "\t\t" << '\"' << "description" << '\"' << ':' << " " << it->second[i] << ',' << '\n';
            }
            else if (i == 1) {
                send << "\t\t" << '\"' << "status" << '\"' << ':' << " " << '\"' << it->second[i] << '\"' << ',' << '\n';
            }
            else if (i == 2) {
                send << "\t\t" << '\"' << "createdAt" << '\"' << ':' << " " << '\"' << it->second[i] << '\"' << ',' << '\n';
            }
            else if (i == 3) {
                send << "\t\t" << '\"' << "modifiedAt" << '\"' << ':' << " " << '\"' << it->second[i] << '\"' << '\n';               
            }
        }

        if (it == last) {
            send << '\t' << '}' << '\n';
        }
        else {
            send << '\t' << '}' << '\n';
            send << '\t' << ',' << '\n';
        }


    }
    
    send << ']' << std::endl;



}





