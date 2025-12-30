#include "library.h"

#include <iostream>

class BorrowableBook : public Book {
    std::string borrower;
    std::string borrow_date;
    std::string return_date;
public:
    BorrowableBook(std::string _title, std::string _author, std::string _publisher, int _book_type_id, int _book_id, bool _stored = true, std::string _borrower = "None", std::string _borrow_date = "0000-00-00", std::string _return_date = "0000-00-00") : Book(std::move(_title), std::move(_author), std::move(_publisher), _book_type_id, _book_id, _stored)
    {
        borrower = std::move(_borrower);
        borrow_date = std::move(_borrow_date);
        return_date = std::move(_return_date);
    }

    BorrowableBook* clone() override { return new BorrowableBook(*this); }

    void display() override {
        Book::display();
        if (!stored) std::cout << "[DISPLAY] Borrower: " << borrower << std::endl;
    }

    void set_borrower(std::string name) { borrower = std::move(name); }

    static BorrowableBook* create_from_file(const std::string& line)
    {
        std::string tid, bid, tit, aut, pub, sto, borer, bordate, retdate;

        std::stringstream ss(line);
        std::getline(ss, tid, '|');
        std::getline(ss, bid, '|');
        std::getline(ss, tit, '|');
        std::getline(ss, aut, '|');
        std::getline(ss, pub, '|');
        std::getline(ss, sto, '|');
        std::getline(ss, borer, '|');
        std::getline(ss, bordate, '|');
        std::getline(ss, retdate, '|');

        try
        {
            return new BorrowableBook(tit, aut, pub, std::stoi(tid), std::stoi(bid), (sto == "True" ? true : false), borer, bordate, retdate);
        }
        catch (std::invalid_argument& e)
        {
            std::cout << "[ERROR] Invalid input format: " << e.what() << std::endl;
            return nullptr;
        }
    }

    virtual bool is_borrowed() { return !borrower.empty(); }
    virtual std::string get_borrower() { return borrower; }
    virtual std::string get_borrow_date() { return borrow_date; }
    virtual std::string get_return_date() { return return_date; }

    void fill_from_cmd() override {
        Book::fill_from_cmd(); // Ask for title, author, publisher first
        std::cout << "Enter borrower name (or 'None'): ";
        std::getline(std::cin, borrower);
        std::cout << "Enter borrow date (YYYY-MM-DD): ";
        std::getline(std::cin, borrow_date);
        std::cout << "Enter return date (YYYY-MM-DD): ";
        std::getline(std::cin, return_date);
    }

    std::string to_file_string() override {
        return Book::to_file_string() + "|" + borrower + "|" + borrow_date + "|" + return_date;
    }

    void issue_book() override
    {
        if (stored) {
            std::cout << "\n--- Issuing Book: " << title << " ---" << std::endl;

            // 1. Capture Borrower Name
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter Borrower Name: ";
            std::getline(std::cin, borrower);

            // 2. Capture Dates (You can use strings for now, or a date library later)
            std::cout << "Enter Issue Date (YYYY-MM-DD): ";
            std::getline(std::cin, borrow_date);

            std::cout << "Enter Return Deadline (YYYY-MM-DD): ";
            std::getline(std::cin, return_date);

            stored = false;
            std::cout << "[SUCCESS] Book issued to " << borrower << " until " << return_date << std::endl;
        } else {
            std::cout << "[ERROR] This book is already out with " << borrower << std::endl;
        }
    }

    void return_book() override
    {
        if (!stored) {
            std::cout << "[LOG] Book returned by " << borrower << std::endl;
            borrower = "None";
            borrow_date = "0000-00-00";
            return_date = "0000-00-00";
            stored = true;
        } else {
            std::cout << "[ERROR] Book was already in the library." << std::endl;
        }
    }

    void display_in_inventory() override
    {
        Book::display_in_inventory(); // Prints the standard line
        if (!stored) {
            std::cout << "        Borrowed by: " << borrower << " (Due: " << return_date << ")" << std::endl;
        }
    }
};

int main()
{
    Library<BorrowableBook> lib;

    lib.read_from_file("borrowable_books.txt", BorrowableBook::create_from_file);

    menu(&lib);

    lib.write_to_file("borrowable_books.txt");
}