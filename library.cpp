// For Clion IDE
// ReSharper disable CppClassCanBeFinal
// ReSharper disable CppMemberFunctionMayBeConst
// ReSharper disable CppMemberFunctionMayBeStatic
// ReSharper disable CppLocalVariableMayBeConst
// ReSharper disable CppParameterMayBeConst
// ReSharper disable CppPassValueParameterByConstReference

#include "library.h"

// Book

Book::Book(std::string _title, std::string _author, std::string _publisher, int _book_type_id, int _book_id, bool _stored)
{
    title = std::move(_title);
    author = std::move(_author);
    publisher = std::move(_publisher);
    book_type_id = _book_type_id;
    book_id = _book_id;
    stored = _stored;
}

Book::~Book() = default;

std::string Book::get_title() {return title;}
std::string Book::get_author() {return author;}
std::string Book::get_publisher() {return publisher;}
int Book::get_book_type_id() {return book_type_id;}
int Book::get_book_id() {return book_id;}
bool Book::is_stored() {return stored;}

void Book::issue_book()
{
    if (stored)
    {
        std::cout << "[LOG] Book '" << title << "' (" << book_type_id << ") has been issued successfully" << std::endl;
        stored = false;
    }
    else
    {
        std::cout << "[ERROR] Cannot issue book '" << title << "' (" << book_type_id << ")" << std::endl;
        std::cout << "[ERROR] Book '" << title << "' (" << book_type_id << ") is not stored in the library" << std::endl;
    }
}

void Book::return_book()
{
    if (!stored)
    {
        std::cout << "[LOG] Book '" << title << "' (" << book_type_id << ") has been returned successfully" << std::endl;
        stored = true;
    }
    else
    {
        std::cout << "[ERROR] Cannot return book '" << title << "' (" << book_type_id << ")" << std::endl;
        std::cout << "[ERROR] Book '" << title << "' (" << book_type_id << ") is already stored in the library" << std::endl;
    }
}

void Book::display()
{
    std::cout << "[DISPLAY] Book '" << title << "' (" << book_type_id << " | " << book_id << ")" << std::endl;
    std::cout << "[DISPLAY] Author: " << author << std::endl;
    std::cout << "[DISPLAY] Publisher: " << publisher << std::endl;
    std::cout << "[DISPLAY] Stored: " << stored << std::endl;
    std::cout << std::endl;
}

Book* Book::clone() {
    return new Book(*this); // Creates a brand new copy in memory
}

void Book::set_book_id(int new_id)
{
    book_id = new_id;
}

Book* Book::create_from_file(const std::string& line)
{
    std::string tid, bid, tit, aut, pub, sto;

    std::stringstream ss(line);
    std::getline(ss, tid, '|');
    std::getline(ss, bid, '|');
    std::getline(ss, tit, '|');
    std::getline(ss, aut, '|');
    std::getline(ss, pub, '|');
    std::getline(ss, sto, '|');

    try
    {
        return new Book(tit, aut, pub, std::stoi(tid), std::stoi(bid), sto == "True");
    }
    catch (std::invalid_argument& e)
    {
        std::cout << "[ERROR] Invalid input format: " << e.what() << std::endl;
        return nullptr;
    }
}

void Book::fill_from_cmd() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer
    std::cout << "Enter title: "; std::getline(std::cin, title);
    std::cout << "Enter author: "; std::getline(std::cin, author);
    std::cout << "Enter publisher: "; std::getline(std::cin, publisher);
}

std::string Book::to_file_string() {
    return std::to_string(book_type_id) + "|" +
           std::to_string(book_id) + "|" +
           title + "|" +
           author + "|" +
           publisher + "|" +
           (stored ? "True" : "False");
}

void Book::display_in_inventory()
{
    std::cout << std::right << std::setw(id_length) << std::setfill('0') << this->get_book_type_id() << " | " << std::setw(book_id_length) << this->get_book_id() << " | " << std::left << std::setfill(' ') << std::setw(title_length) << this->get_title() << " | " << std::setw(publisher_length) << this->get_publisher() << " | " << (this->is_stored() ? "True" : "False") << std::endl;
}

void display_menu()
{
    std::cout << "1. Display Inventory" << std::endl;
    std::cout << "2. Search by Title or Author" << std::endl;
    std::cout << "3. Add new book" << std::endl;
    std::cout << "4. Issue book" << std::endl;
    std::cout << "5. Return book" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "========================================================" << std::endl;
    std::cout << "Enter your choice: ";
}