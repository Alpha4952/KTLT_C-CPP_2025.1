// For Clion IDE
// ReSharper disable CppClassCanBeFinal
// ReSharper disable CppMemberFunctionMayBeConst
// ReSharper disable CppMemberFunctionMayBeStatic
// ReSharper disable CppLocalVariableMayBeConst
// ReSharper disable CppParameterMayBeConst
// ReSharper disable CppPassValueParameterByConstReference

#ifndef LIBRARY_LIBRARY_H
#define LIBRARY_LIBRARY_H

#include <algorithm>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// ===== CONFIG =====

constexpr int id_length = 5;
constexpr int book_id_length = 5;
constexpr int title_length = 20;
constexpr int author_length = 20;
constexpr int publisher_length = 20;

class Book {
protected:
    std::string title;
    std::string author;
    std::string publisher;
    int book_type_id;
    int book_id;
    bool stored;
public:
    Book(std::string _title, std::string _author, std::string _publisher, int _book_type_id, int _book_id, bool _stored = true);
    virtual ~Book();

    virtual void fill_from_cmd();

    virtual std::string get_title();
    virtual std::string get_author();
    virtual std::string get_publisher();
    virtual int get_book_type_id();
    virtual int get_book_id();
    virtual bool is_stored();

    virtual void issue_book();
    virtual void return_book();
    virtual void display();
    virtual Book* clone();
    void set_book_id(int new_id);

    static Book* create_from_file(const std::string& line);
    virtual std::string to_file_string();
    virtual void display_in_inventory();
};

template <typename T>
class Library {
    std::vector<T*> inventory;
    int book_count = 0;
    void sort_inventory()
    {
        if (inventory.empty()) return;

        std::sort(inventory.begin(), inventory.end(), [](T* a, T* b) {
            // First, sort by Type ID (the general category)
            if (a->get_book_type_id() != b->get_book_type_id()) {
                return a->get_book_type_id() < b->get_book_type_id();
            }
            // If Type IDs are the same, sort by the specific Book ID (BID)
            return a->get_book_id() < b->get_book_id();
        });
    }

public:
    Library<T>() = default;
    ~Library()
    {
        for (auto book : inventory) delete book;
        inventory.clear();
        inventory.shrink_to_fit();
        std::cout << "[LOG] Library has been destroyed successfully" << std::endl;
    }

    std::vector<T*> get_inventory() { return inventory; };

    void add_book(T* book, int quantity = 1, bool disable_log = false)
    {
        if (!book) return;
        int added = 0;
        int current_bid = book->get_book_id();

        while (added < quantity) {
            bool collision = false;
            for (auto b : inventory) {
                if (b->get_book_type_id() == book->get_book_type_id() && b->get_book_id() == current_bid) {
                    collision = true; break;
                }
            }
            if (collision) { current_bid++; continue; }

            T* copy = book->clone();
            copy->set_book_id(current_bid);
            inventory.push_back(copy);
            book_count++;
            added++;
            current_bid++;
        }
        if (!disable_log) std::cout << "[LOG] Added " << added << " items.\n";
    }

    void display_inventory()
    {
        sort_inventory();

        if (book_count == 0)
        {
            std::cout << "[ERROR] No books in the library" << std::endl;
            return;
        }

        std::cout << "[DISPLAY] Inventory:" << std::endl;
        std::cout << std::left << std::setw(id_length / 2 - 1 + (id_length % 2)) << "" << std::setw(id_length / 2 + 1) << "ID" << " | " << std::setw(book_id_length / 2 - 1) << "" << std::setw(book_id_length / 2 + 2) << "BID" << " | " << std::setw(title_length/2 - 2) << "" << std::setw(title_length / 2 + 2) << "Title" << " | " << std::setw(publisher_length / 2 - 4) << "" << std::setw(publisher_length / 2 + 4) << "Publisher" << " | " << "Stored" << std::endl;

        for (auto book : inventory)
        {
            book->display_in_inventory();
        }

        std::cout << std::endl;
    }

    void read_from_file(const std::string &input_file, const std::function<T*(const std::string &)> &parser)
    {
        std::ifstream input(input_file);
        if (!input.is_open()) return;
        std::string line;
        while (std::getline(input, line)) {
            if (line.empty()) continue;
            T* temp = parser(line);
            if (temp) {
                add_book(temp, 1, true);
                delete temp;
            }
        }
    }

    void write_to_file(const std::string& output_file)
    {
        sort_inventory();
        std::ofstream file(output_file);

        if (!file.is_open()) {
            std::cout << "[ERROR] Could not open file for writing" << std::endl;
            return;
        }

        for (auto book : inventory) {
            file << book->to_file_string() << "\n";
        }

        file.close();
        std::cout << "[LOG] Library saved to " << output_file << std::endl;
    }

    bool contains_case_insensitive(std::string source, std::string query)
    {
        std::string s_low = std::move(source);
        std::string q_low = std::move(query);

        std::transform(s_low.begin(), s_low.end(), s_low.begin(), ::tolower);
        std::transform(q_low.begin(), q_low.end(), q_low.begin(), ::tolower);

        return s_low.find(q_low) != std::string::npos;
    }

    T* find_book_by_id(int tid, int bid)
    {
        for (auto book : inventory) {
            if (book->get_book_type_id() == tid && book->get_book_id() == bid) {
                return book; // Return the exact pointer to the book
            }
        }
        return nullptr; // Not found
    }

    void search_by_title_author(const std::string& query)
    {
        bool found = false;
        std::cout << "[SEARCH] Results for: " << query << std::endl;

        for (auto book : inventory) {
            if (contains_case_insensitive(book->get_title(), query) || contains_case_insensitive(book->get_author(), query)) {
                book->display();
                found = true;
            }
        }

        if (!found) {
            std::cout << "[INFO] No titles match your search." << std::endl;
        }
    }
};

void display_menu();

template <typename T>
void menu(Library<T>* lib)
{
    int choice = 0;

    do
    {
        display_menu();
        std::cin >> choice;
        process(choice, lib);
    } while (choice);
}

template <typename T>
void process(int choice, Library<T>* lib)
{
    if (choice == 1)
    {
        std::cout << std::endl;
        lib->display_inventory();
        std::cout << std::endl;
    }
    else if (choice == 2)
    {
        std::cout << std::endl;
        std::cout << "Enter search query: ";
        std::string query;
        std::cin.ignore();
        std::getline(std::cin, query);

        lib->search_by_title_author(query);
        std::cout << std::endl;
    }
    if (choice == 3) {
        int book_type_id, quantity;

        std::cout << std::endl;
        std::cout << "\nEnter book type ID: ";
        if (!(std::cin >> book_type_id)) return;

        T* prototype = nullptr;

        // Check if this type already exists to copy info
        for (auto book : lib->get_inventory()) {
            if (book->get_book_type_id() == book_type_id) {
                std::cout << "[LOG] Book type " << book_type_id << " exists. Copying info...\n";
                prototype = static_cast<T*>(book->clone());
                break;
            }
        }

        std::cout << "Enter quantity: "; std::cin >> quantity;

        if (!prototype) {
            // New book type: Create empty object and let it ask for its own data
            prototype = new T("", "", "", book_type_id, 1);
            prototype->fill_from_cmd();
        }

        lib->add_book(prototype, quantity);
        std::cout << std::endl;
        delete prototype;
    }
    else if (choice == 4)
    {
        int book_type_id, book_id;
        std::cout << "Enter book type ID: "; std::cin >> book_type_id;
        std::cout << "Enter book ID: "; std::cin >> book_id;
        std::cout << std::endl;

        T* found = lib->find_book_by_id(book_type_id, book_id);
        if (found) {
            found->issue_book();
        } else {
            std::cout << "[ERROR] Book ID not found!" << std::endl;
        }

        std::cout << std::endl;
    }
    else if (choice == 5)
    {
        int book_type_id, book_id;
        std::cout << "Enter book type ID: "; std::cin >> book_type_id;
        std::cout << "Enter book ID: "; std::cin >> book_id;
        std::cout << std::endl;

        T* found = lib->find_book_by_id(book_type_id, book_id);
        if (found) {
            found->return_book();
        } else {
            std::cout << "[ERROR] Book ID not found!" << std::endl;
        }
        std::cout << std::endl;
    }
}

#endif //LIBRARY_LIBRARY_H