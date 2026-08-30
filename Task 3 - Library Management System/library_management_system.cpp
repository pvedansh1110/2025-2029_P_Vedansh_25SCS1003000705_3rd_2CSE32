#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdio>
using namespace std;

class Book {
private:
    int bookId;
    string title;
    string author;
    bool issued;

public:
    Book() : bookId(0), title(""), author(""), issued(false) {}
    Book(int id, string t, string a, bool status = false)
        : bookId(id), title(t), author(a), issued(status) {}

    int getId() const { return bookId; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool isIssued() const { return issued; }
    void setIssued(bool status) { issued = status; }

    void display() const {
        cout << left << setw(8) << bookId
             << setw(25) << title
             << setw(25) << author
             << (issued ? "Issued" : "Available") << endl;
    }
};

class Member {
private:
    int memberId;
    string name;

public:
    Member() : memberId(0), name("") {}
    Member(int id, string n) : memberId(id), name(n) {}

    int getId() const { return memberId; }
    string getName() const { return name; }

    void display() const {
        cout << "Member ID: " << memberId << " | Name: " << name << endl;
    }
};

bool saveBook(const Book &book) {
    ofstream file("books.txt", ios::app);
    if (!file) {
        cout << "Error: Could not open books.txt\n";
        return false;
    }
    file << book.getId() << "|" << book.getTitle() << "|"
         << book.getAuthor() << "|" << book.isIssued() << "\n";
    file.close();
    return true;
}

bool saveMember(const Member &member) {
    ofstream file("members.txt", ios::app);
    if (!file) {
        cout << "Error: Could not open members.txt\n";
        return false;
    }
    file << member.getId() << "|" << member.getName() << "\n";
    file.close();
    return true;
}

bool findBook(int searchId, Book &book) {
    ifstream file("books.txt");
    if (!file) return false;

    string line;
    while (getline(file, line)) {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;

        int id = stoi(line.substr(0, p1));
        string title = line.substr(p1 + 1, p2 - p1 - 1);
        string author = line.substr(p2 + 1, p3 - p2 - 1);
        bool issued = stoi(line.substr(p3 + 1)) == 1;

        if (id == searchId) {
            book = Book(id, title, author, issued);
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

bool findMember(int searchId, Member &member) {
    ifstream file("members.txt");
    if (!file) return false;

    string line;
    while (getline(file, line)) {
        size_t p = line.find('|');
        if (p == string::npos) continue;

        int id = stoi(line.substr(0, p));
        string name = line.substr(p + 1);
        if (id == searchId) {
            member = Member(id, name);
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void updateBookStatus(const Book &updatedBook) {
    ifstream file("books.txt");
    ofstream temp("temp_books.txt");
    string line;

    while (getline(file, line)) {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;

        int id = stoi(line.substr(0, p1));
        if (id == updatedBook.getId()) {
            temp << updatedBook.getId() << "|" << updatedBook.getTitle() << "|"
                 << updatedBook.getAuthor() << "|" << updatedBook.isIssued() << "\n";
        } else {
            temp << line << "\n";
        }
    }
    file.close();
    temp.close();
    remove("books.txt");
    rename("temp_books.txt", "books.txt");
}

void addBook() {
    int id;
    string title, author;
    cout << "\nEnter Book ID: ";
    cin >> id;
    cin.ignore();
    cout << "Enter Book Title: ";
    getline(cin, title);
    cout << "Enter Author Name: ";
    getline(cin, author);

    Book book(id, title, author);
    if (saveBook(book)) cout << "Book added successfully.\n";
    else cout << "Book could not be added.\n";
}

void addMember() {
    int id;
    string name;
    cout << "\nEnter Member ID: ";
    cin >> id;
    cin.ignore();
    cout << "Enter Member Name: ";
    getline(cin, name);

    Member member(id, name);
    if (saveMember(member)) cout << "Member added successfully.\n";
    else cout << "Member could not be added.\n";
}

void issueBook() {
    int bookId, memberId;
    cout << "\nEnter Book ID: ";
    cin >> bookId;

    Book book;
    if (!findBook(bookId, book)) {
        cout << "Book not found.\n";
        return;
    }
    if (book.isIssued()) {
        cout << "Book is already issued.\n";
        return;
    }

    cout << "Enter Member ID: ";
    cin >> memberId;
    Member member;
    if (!findMember(memberId, member)) {
        cout << "Member not found.\n";
        return;
    }

    book.setIssued(true);
    updateBookStatus(book);

    ofstream file("issued.txt", ios::app);
    if (file) {
        file << bookId << "|" << memberId << "\n";
        file.close();
    }
    cout << "Book issued successfully to " << member.getName() << ".\n";
}

void returnBook() {
    int bookId;
    cout << "\nEnter Book ID to return: ";
    cin >> bookId;

    Book book;
    if (!findBook(bookId, book)) {
        cout << "Book not found.\n";
        return;
    }
    if (!book.isIssued()) {
        cout << "This book is already available.\n";
        return;
    }

    book.setIssued(false);
    updateBookStatus(book);

    ifstream file("issued.txt");
    ofstream temp("temp_issued.txt");
    string line;
    while (getline(file, line)) {
        size_t p = line.find('|');
        if (p == string::npos) continue;
        int id = stoi(line.substr(0, p));
        if (id != bookId) temp << line << "\n";
    }
    file.close();
    temp.close();
    remove("issued.txt");
    rename("temp_issued.txt", "issued.txt");

    cout << "Book returned successfully.\n";
}

void searchBook() {
    string keyword;
    cin.ignore();
    cout << "\nEnter title or author to search: ";
    getline(cin, keyword);

    ifstream file("books.txt");
    if (!file) {
        cout << "No books found.\n";
        return;
    }

    string line;
    bool found = false;
    cout << "\n================ SEARCH RESULTS ================\n";
    cout << left << setw(8) << "ID" << setw(25) << "Title"
         << setw(25) << "Author" << "Status\n";
    cout << "-------------------------------------------------\n";

    while (getline(file, line)) {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;

        int id = stoi(line.substr(0, p1));
        string title = line.substr(p1 + 1, p2 - p1 - 1);
        string author = line.substr(p2 + 1, p3 - p2 - 1);
        bool issued = stoi(line.substr(p3 + 1)) == 1;

        if (title.find(keyword) != string::npos || author.find(keyword) != string::npos) {
            Book book(id, title, author, issued);
            book.display();
            found = true;
        }
    }
    file.close();
    if (!found) cout << "No matching books found.\n";
}

void displayBooks() {
    ifstream file("books.txt");
    if (!file) {
        cout << "\nNo books found.\n";
        return;
    }

    string line;
    bool found = false;
    cout << "\n================ ALL BOOKS =================\n";
    cout << left << setw(8) << "ID" << setw(25) << "Title"
         << setw(25) << "Author" << "Status\n";
    cout << "-------------------------------------------------\n";

    while (getline(file, line)) {
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) continue;

        int id = stoi(line.substr(0, p1));
        string title = line.substr(p1 + 1, p2 - p1 - 1);
        string author = line.substr(p2 + 1, p3 - p2 - 1);
        bool issued = stoi(line.substr(p3 + 1)) == 1;

        Book book(id, title, author, issued);
        book.display();
        found = true;
    }
    file.close();
    if (!found) cout << "No books found.\n";
}

void displayMembers() {
    ifstream file("members.txt");
    if (!file) {
        cout << "\nNo members found.\n";
        return;
    }

    string line;
    bool found = false;
    cout << "\n================ ALL MEMBERS ================\n";

    while (getline(file, line)) {
        size_t p = line.find('|');
        if (p == string::npos) continue;
        int id = stoi(line.substr(0, p));
        string name = line.substr(p + 1);
        Member member(id, name);
        member.display();
        found = true;
    }
    file.close();
    if (!found) cout << "No members found.\n";
}

int main() {
    int choice;
    do {
        cout << "\n=========================================\n";
        cout << "       LIBRARY MANAGEMENT SYSTEM\n";
        cout << "=========================================\n";
        cout << "1. Add Book\n";
        cout << "2. Add Member\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. Search Book\n";
        cout << "6. Display All Books\n";
        cout << "7. Display All Members\n";
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addBook(); break;
            case 2: addMember(); break;
            case 3: issueBook(); break;
            case 4: returnBook(); break;
            case 5: searchBook(); break;
            case 6: displayBooks(); break;
            case 7: displayMembers(); break;
            case 8: cout << "Thank you for using the Library Management System!\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 8);

    return 0;
}
