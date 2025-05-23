
// produced by 3TI (10-2-3-8-20-13-17)

#include <iostream>
#include <vector>
#include <string>
using namespace std;

//Mohamed Mostafa

// Abstract User class
class User {
protected:
    string username, password, userID;

public:
    User(string u, string p, string id) : username(u), password(p), userID(id) {}

    string getUsername() const { return username; }

    virtual void displayRole() = 0;
};
// -------------------------------------------------------------------------------------------------------
//Ahmed Khaled  
// Member class (Handles borrow/return operations)
class Member : public User {
public:
    Member(string u, string p, string id) : User(u, p, id) {}

    void displayRole() override {
        cout << "Member Access Granted: " << username << " (ID: " << userID << ")\n";
	}//displayRole
};

// Librarian class (Manages books)
class Librarian : public User {
public:
    Librarian(string u, string p, string id) : User(u, p, id) {}

    void displayRole() override {
        cout << "Librarian Access Granted: " << username << " (ID: " << userID << ")\n";
    }
};
// -------------------------------------------------------------------------------------------------------


//Gedo

// Book class with Borrow Tracking
class Book {
protected:
    string title, author, borrowedBy;
    int ISBN;

public:
    Book(string t, string a, int isbn) : title(t), author(a), ISBN(isbn), borrowedBy("") {}

    string getTitle() const { return title; }
    string getBorrowedBy() const { return borrowedBy; }

    bool isAvailable() const { return borrowedBy.empty(); }

    void borrowBook(string username) {
        if (isAvailable()) {
            borrowedBy = username;
            cout << "Book \"" << title << "\" successfully borrowed by " << username << "!\n";
        }
        else {
            cout << "Book \"" << title << "\" is already borrowed by " << borrowedBy << ". Try another book.\n";
        }
    }

    void returnBook() {
        if (!isAvailable()) {
            cout << "Book \"" << title << "\" returned successfully!\n";
            borrowedBy = ""; // Reset borrowed status
        }
        else {
            cout << "Book \"" << title << "\" is not currently borrowed.\n";
        }
    }

    virtual void displayInfo() {
        cout << "Title: " << title << ", Author: " << author << ", ISBN: " << ISBN;
        cout << " | Status: " << (isAvailable() ? "Available" : "Borrowed by " + borrowedBy) << endl;
    }
};
//---------------------------------------------------------------------------------------------

//Abdo
// Library Collection (Managed by Librarians)
class LibraryCollection {
private:
    vector<Book*> books;

public:
    void addBook(Book* book) {
        books.push_back(book);
        cout << "Book added successfully!\n";
    }

    void removeBook(string title) {
        for (size_t i = 0; i < books.size(); ++i) {
            if (books[i]->getTitle() == title) {
                books.erase(books.begin() + i);
                cout << "Book removed successfully!\n";
                return;
            }
        }
        cout << "Book not found!\n";
    }

    Book* search(string title) {
        for (Book* book : books) {
            if (book->getTitle() == title) {
                book->displayInfo();
                return book;
            }
        }
        cout << "Book not found!\n";
        return nullptr;
    }
};


// Function to validate User ID
bool isValidUserID(const string& userID) {
    return userID.length() == 14 && userID.find_first_not_of("0123456789") == string::npos;
}
//-------------------------------------------------------------------
//Mina
// Function to select user role
User* selectRole(string username, string password, string userID) {
    int roleChoice;
    cout << "Select role:\n1. Member\n2. Librarian\nEnter choice: ";
    cin >> roleChoice;

    if (roleChoice == 2) {
        string libPassword;
        cout << "Enter Library Password: ";
        cin >> libPassword;

        if (libPassword == "12345678") {
            cout << "Librarian access granted.\n";
            return new Librarian(username, password, userID);
        }
        else {
            cout << "Incorrect library password! Access denied.\n";
            return selectRole(username, password, userID); // Retry selection
        }
    }
    else {
        cout << "Member access granted.\n";
        return new Member(username, password, userID);
    }
}
//------------------------------------------------------------------
// Mario - fam
// Main function
int main() {
    string username, password, userID;

    cout << "Enter username: ";
    cin >> username;

    do {
        cout << "Enter password (at least 6 characters): ";
        cin >> password;
    } while (password.length() < 6);

    do {
        cout << "Enter user ID (14-digit number): ";
        cin >> userID;
    } while (!isValidUserID(userID));

    User* user = selectRole(username, password, userID);
    LibraryCollection library;

    int choice;
    do {
        cout << "\nLibrary System Menu:\n";
        if (dynamic_cast<Member*>(user)) {
            cout << "1. Search for a book\n";
            cout << "2. Borrow a book\n";
            cout << "3. Return a book\n";
        }
        else {
            cout << "1. Search for a book\n";
            cout << "2. Add a book\n";
            cout << "3. Remove a book\n";
            
        }
        cout << "4. Switch role\n";
        cout << "5. Exit\nEnter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: {
            string searchTitle;
            cout << "Enter book title to search: ";
            getline(cin, searchTitle);
            library.search(searchTitle);
            break;
        }
        case 2: {
            if (dynamic_cast<Member*>(user)) {
                string bookTitle;
                cout << "Enter book title to borrow: ";
                getline(cin, bookTitle);
                Book* book = library.search(bookTitle);
                if (book) book->borrowBook(username);
            }
            else {
                string title, author;
                int isbn;
                cout << "Enter book title: ";
                getline(cin, title);
                cout << "Enter book author: ";
                getline(cin, author);
                cout << "Enter ISBN: ";
                cin >> isbn;
                Book* book = new Book(title, author, isbn);
                library.addBook(book);
            }
            break;
        }
        case 3: {
            if (dynamic_cast<Member*>(user)) {
                string bookTitle;
                cout << "Enter book title to return: ";
                getline(cin, bookTitle);
                Book* book = library.search(bookTitle);
                if (book) book->returnBook();
            }
            else {
                string removeTitle;
                cout << "Enter book title to remove: ";
                getline(cin, removeTitle);
                library.removeBook(removeTitle);
            }
            break;
        }
        case 4:
            cout << "Switching roles...\n";
            delete user;
            user = selectRole(username, password, userID);
            break;
        case 5:
            cout << "Exiting Library System. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 5);

    delete user;
    return 0;
}