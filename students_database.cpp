//
//  students_database.cpp
//  Students Database
//
//  Created by Vlad Ishchuk on 26.09.2023.
//

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void info_message (string text) {
    std::cout << "\033[1;33m" << text << "\033[0m" << std::endl;
}

void successful_message (string text) {
    std::cout << "\033[1;32m" << text << "\033[0m" << std::endl;
}

class Database {
private:
    string database_file;
public:
    Database(string db_file) {
        database_file = db_file;
    }
    
    void save_data (string data) {
        std::ofstream database(database_file, std::ios_base::app);
        
        if (!database) {
            throw std::runtime_error("Database is not created!");
        }
        
        database << data << '\n';
        
        database.close();
    }

    void read_data (vector<string> &data) {
        std::fstream database;
        database.open(database_file, ios::in);
        
        if (!database) {
            throw std::runtime_error("No such database file!");
        }
        
        std::string line;
        while (std::getline(database, line)) {
            data.push_back(line);
        }
        
        database.close();
    }

    void update_data (vector<string> &data) {
        std::ofstream database(database_file, ios::out);

        if (!database) {
            throw std::runtime_error("Database is not found!");
        }
        
        for (int i = 0; i < data.size(); i++) {
            database << data[i] << '\n';
        }
        
        database.close();
    }
};

// ============ Форматування таблиць ============
void print_table (vector<string> &data) {
    if (data.size() == 0) {
        info_message("The list of records is empty.");
    } else {
        std::cout << "\n\033[1;37m========== Records list ==========" << std::endl;
        for (int i = 0; i < data.size(); i++) {
            cout << "| " << i << ". " << data[i] << endl;
        }
        std::cout << "==================================\033[0m" << std::endl << std::endl;
    }
}

void print_table_with_cursor (vector<string> &data, int pointer, string pointer_message) {
    if (data.size() == 0) {
        info_message("The list of records is empty.");
    } else {
        std::cout << "\n" << "\033[1;37m" << "========== Records list ==========" << std::endl;
        for (int i = 0; i < data.size(); i++) {
            if (pointer == i) {
                cout << "| " << i << ". " << data[i] << "\033[3;90m <- " << pointer_message << "\033[0m\033[1;37m" << endl;
            } else {
                cout << "| " << i << ". " << data[i] << endl;
            }
        }
        std::cout << "==================================\033[0m" << std::endl << std::endl;
    }
}
// ==============================================


int main () {
    Database db("./database.txt");
    
    while (true) {
        try {
            char operation;
            
            std::cout << "\n\033[1;37mChoose operation ('c' - create a new record, 'r' - list of records, 'u' - update a record, 'd' - delete a record, 'q' - exit): \033[0m";
            std::cin >> operation;
            
            switch (operation) {
                case 'r': {
                    vector<string> data;
                    
                    db.read_data(data);
                    print_table(data);
                    
                    break;
                }
                    
                case 'c': {
                    string entered_data;
                    std::cout << "> ";
                    std::getline(std::cin >> ws, entered_data);
                    
                    db.save_data(entered_data);
                    successful_message("The record was created successfully!");
                    
                    break;
                }
                    
                case 'd': {
                    vector<string> data;
                    int delete_index;
                    
                    db.read_data(data);
                    
                    std::cout << "Enter index of record you want to delete: ";
                    std::cin >> delete_index;
                    
                    // Перевіряємо, чи даний індекс існує
                    if (delete_index >= 0 && delete_index < data.size()) {
                        // Видаляємо елемент за вказаним індексом
                        data.erase(data.begin() + delete_index);
                        successful_message("Element at index " + to_string(delete_index) + " has been deleted.");
                    } else {
                        info_message("Invalid index.");
                    }
                    
                    db.update_data(data); // Оноволюємо дані БД
                    print_table(data); // Виводимо нову таблицю записів
                    
                    break;
                }
                    
                case 'u': {
                    vector<string> data;
                    int update_index;
                    
                    db.read_data(data);
                    
                    std::cout << "Enter index of record you want to update: ";
                    std::cin >> update_index;
                    
                    // Перевіряємо, чи даний індекс існує
                    if (update_index >= 0 && update_index < data.size()) {
                        string entered_data;
                        std::cout << "> ";
                        std::getline(std::cin >> ws, entered_data);
                        
                        string old_data = data[update_index];
                        
                        data[update_index] = entered_data;
                        
                        successful_message("Element at index " + to_string(update_index) + " has been updated from '" + old_data + "' to '" + entered_data + "'.");
                    } else {
                        info_message("Invalid index.");
                    }
                    
                    db.update_data(data); // Оноволюємо дані БД
                    print_table_with_cursor(data, update_index, "Updated"); // Виводимо нову таблицю записів
                    
                    break;
                }
                    
                case 'q':
                    return 0;
                    break;
                    
                default:
                    info_message("The operation is not support!");
                    break;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}
