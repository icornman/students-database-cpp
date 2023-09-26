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

const string database_file = "./database.txt";


void info_message (string text) {
    std::cout << "\033[1;33m" << text << "\033[0m" << std::endl;
}

void successful_message (string text) {
    std::cout << "\033[1;32m" << text << "\033[0m" << std::endl;
}

void save_data (string data) {
    std::ofstream database(database_file, std::ios_base::app);
    
    if (!database) {
        std::cout << "Database is not created!";
        return;
    }
    
    database << data << '\n';
    
    database.close();
}

void read_data (vector<string> &data) {
    std::fstream database;
    database.open(database_file, ios::in);
    
    if (!database) {
        info_message("No such database file!");
        return;
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
        std::cout << "Database is not found!";
        return;
    }
    
    for (int i = 0; i < data.size(); i++) {
        database << data[i] << '\n';
    }
    
    database.close();
}

// Функція для відображення більш-менш відформатованої таблиці
void print_table (vector<string> &data) {
    if (data.size() == 0) {
        // Виведемо повідомлення, що свідчить про відсутність записів у БД
        info_message("The list of records is empty.");
    } else {
        // Виводимо таблицю сірого кольору
        std::cout << "\n\033[1;37m========== Records list ==========" << std::endl;
        for (int i = 0; i < data.size(); i++) {
            cout << "| " << i << ". " << data[i] << endl;
        }
        std::cout << "==================================\033[0m" << std::endl << std::endl;
    }
}

int main () {
    while (true) {
        char operation;
        
        std::cout << "\n\033[1;37mChoose operation (0 - list of records, 1 - make new record, 2 - delete a record, 'q' - exit): \033[0m";
        std::cin >> operation;
        
        switch (operation) {
            case '0': {
                vector<string> data;
                read_data(data);
                print_table(data);
                break;
            }
                
            case '1': {
                string entered_data;
                std::cout << "> ";
                // Заміна cin
                std::getline(std::cin >> ws, entered_data);
                
                save_data(entered_data);
                successful_message("The record was created successfully!");
                
                break;
            }
                
            case '2': {
                vector<string> data;
                read_data(data);
                
                int delete_index;
                
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
                
                print_table(data); // Виводимо нову таблицю записів
                update_data(data); // Оноволюємо дані БД
                
                break;
            }
                
            case 'q':
                return 0;
                break;
                
            default:
                info_message("The operation is not support!");
                break;
        }
    }
}
