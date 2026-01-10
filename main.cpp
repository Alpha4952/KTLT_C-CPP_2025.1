#include "library.h"

int main()
{
    Library<Book> lib;

    lib.read_from_file("books.txt", Book::create_from_file);

    menu(&lib);

    lib.write_to_file("books.txt");
}