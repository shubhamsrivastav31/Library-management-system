#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <limits>

using namespace std;

// Forward declarations
class Book;
class Member;
class Transaction;

// Date class for handling dates
class Date {
private:
    int day, month, year;
    
public:
    Date(int d = 1, int m = 1, int y = 2000) : day(d), month(m), year(y) {}
    
    static Date getCurrentDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        return Date(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
    }
    
    Date addDays(int days) {
        // Simplified date calculation (not handling month/year boundaries for simplicity)
        Date result = *this;
        result.day += days;
        
        // Basic month overflow handling
        if (result.day > 30) {
            result.day -= 30;
            result.month++;
            if (result.month > 12) {
                result.month = 1;
                result.year++;
            }
        }
        
        return result;
    }
    
    bool operator>(const Date& other) const {
        if (year != other.year) return year > other.year;
        if (month != other.month) return month > other.month;
        return day > other.day;
    }
    
    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }
    
    void display() const {
        cout << setw(2) << setfill('0') << day << "/"
             << setw(2) << setfill('0') << month << "/"
             << year;
    }
};

// Book class
class Book {
private:
    string ISBN;
    string title;
    string author;
    string category;
    bool isAvailable;
    int publicationYear;
    
public:
    Book(string isbn = "", string t = "", string a = "", string cat = "", int year = 2000)
        : ISBN(isbn), title(t), author(a), category(cat), isAvailable(true), publicationYear(year) {}
    
    // Getters
    string getISBN() const { return ISBN; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getCategory() const { return category; }
    bool getAvailability() const { return isAvailable; }
    int getPublicationYear() const { return publicationYear; }
    
    // Setters
    void setAvailability(bool status) { isAvailable = status; }
    
    void display() const {
        cout << "ISBN: " << ISBN << endl;
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "Category: " << category << endl;
        cout << "Year: " << publicationYear << endl;
        cout << "Status: " << (isAvailable ? "Available" : "Borrowed") << endl;
        cout << "-----------------------------------" << endl;
    }
    
    // Search helper functions
    bool matchesISBN(const string& isbn) const { return ISBN == isbn; }
    bool matchesTitle(const string& t) const { 
        string lowerTitle = title;
        transform(lowerTitle.begin(), lowerTitle.end(), lowerTitle.begin(), ::tolower);
        
        string lowerSearch = t;
        transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), ::tolower);
        
        return lowerTitle.find(lowerSearch) != string::npos; 
    }
    bool matchesAuthor(const string& a) const { 
        string lowerAuthor = author;
        transform(lowerAuthor.begin(), lowerAuthor.end(), lowerAuthor.begin(), ::tolower);
        
        string lowerSearch = a;
        transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), ::tolower);
        
        return lowerAuthor.find(lowerSearch) != string::npos; 
    }
};

// Member class
class Member {
private:
    string memberID;
    string name;
    string email;
    string phone;
    int maxBooksAllowed;
    int booksBorrowed;
    
public:
    Member(string id = "", string n = "", string e = "", string p = "")
        : memberID(id), name(n), email(e), phone(p), maxBooksAllowed(5), booksBorrowed(0) {}
    
    // Getters
    string getMemberID() const { return memberID; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    int getBooksBorrowed() const { return booksBorrowed; }
    int getMaxBooksAllowed() const { return maxBooksAllowed; }
    
    // Check if member can borrow more books
    bool canBorrow() const {
        return booksBorrowed < maxBooksAllowed;
    }
    
    // Increment borrowed books count
    void borrowBook() {
        if (canBorrow()) {
            booksBorrowed++;
        }
    }
    
    // Decrement borrowed books count
    void returnBook() {
        if (booksBorrowed > 0) {
            booksBorrowed--;
        }
    }
    
    void display() const {
        cout << "Member ID: " << memberID << endl;
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phone << endl;
        cout << "Books Borrowed: " << booksBorrowed << "/" << maxBooksAllowed << endl;
        cout << "-----------------------------------" << endl;
    }
};

// Transaction class
class Transaction {
private:
    string transactionID;
    string bookISBN;
    string memberID;
    Date borrowDate;
    Date dueDate;
    Date returnDate;
    bool isReturned;
    double fineAmount;
    
public:
    Transaction(string tid = "", string isbn = "", string mid = "", Date bd = Date(), Date dd = Date())
        : transactionID(tid), bookISBN(isbn), memberID(mid), borrowDate(bd), dueDate(dd), 
          isReturned(false), fineAmount(0.0) {}
    
    // Getters
    string getTransactionID() const { return transactionID; }
    string getBookISBN() const { return bookISBN; }
    string getMemberID() const { return memberID; }
    Date getBorrowDate() const { return borrowDate; }
    Date getDueDate() const { return dueDate; }
    Date getReturnDate() const { return returnDate; }
    bool getIsReturned() const { return isReturned; }
    double getFineAmount() const { return fineAmount; }
    
    // Mark as returned and calculate fine if applicable
    void returnBook(Date returnDt) {
        isReturned = true;
        returnDate = returnDt;
        
        // Calculate fine if returned after due date (fine: $1 per day)
        if (returnDate > dueDate) {
            // Simplified calculation: assume 1 day difference per day after due
            // In real implementation, calculate actual days difference
            fineAmount = 1.0; // Placeholder
        }
    }
    
    void display() const {
        cout << "Transaction ID: " << transactionID << endl;
        cout << "Book ISBN: " << bookISBN << endl;
        cout << "Member ID: " << memberID << endl;
        cout << "Borrow Date: "; borrowDate.display(); cout << endl;
        cout << "Due Date: "; dueDate.display(); cout << endl;
        if (isReturned) {
            cout << "Return Date: "; returnDate.display(); cout << endl;
            cout << "Fine: $" << fixed << setprecision(2) << fineAmount << endl;
        } else {
            cout << "Status: Not Returned" << endl;
        }
        cout << "-----------------------------------" << endl;
    }
};

// Library class to manage everything
class Library {
private:
    vector<Book> books;
    vector<Member> members;
    vector<Transaction> transactions;
    int nextTransactionID;
    
    // Helper functions
    Book* findBookByISBN(const string& isbn) {
        for (auto& book : books) {
            if (book.matchesISBN(isbn)) {
                return &book;
            }
        }
        return nullptr;
    }
    
    Member* findMemberByID(const string& memberID) {
        for (auto& member : members) {
            if (member.getMemberID() == memberID) {
                return &member;
            }
        }
        return nullptr;
    }
    
    Transaction* findTransactionByBookISBN(const string& isbn, bool onlyActive = true) {
        for (auto& transaction : transactions) {
            if (transaction.getBookISBN() == isbn) {
                if (onlyActive && !transaction.getIsReturned()) {
                    return &transaction;
                } else if (!onlyActive) {
                    return &transaction;
                }
            }
        }
        return nullptr;
    }
    
    string generateTransactionID() {
        return "TXN" + to_string(nextTransactionID++);
    }
    
public:
    Library() : nextTransactionID(1001) {
        // Add some sample data
        initializeSampleData();
    }
    
    void initializeSampleData() {
        // Add sample books
        books.push_back(Book("978-0131103627", "The C Programming Language", "Brian Kernighan", "Programming", 1978));
        books.push_back(Book("978-0201633610", "Design Patterns", "Erich Gamma", "Computer Science", 1994));
        books.push_back(Book("978-0321714114", "C++ Primer", "Stanley Lippman", "Programming", 2012));
        books.push_back(Book("978-0596007126", "Head First Design Patterns", "Eric Freeman", "Computer Science", 2004));
        books.push_back(Book("978-0262033848", "Introduction to Algorithms", "Thomas Cormen", "Algorithms", 2009));
        
        // Add sample members
        members.push_back(Member("MEM1001", "John Doe", "john@example.com", "123-456-7890"));
        members.push_back(Member("MEM1002", "Jane Smith", "jane@example.com", "234-567-8901"));
        members.push_back(Member("MEM1003", "Bob Johnson", "bob@example.com", "345-678-9012"));
    }
    
    // Book management
    void addBook() {
        string isbn, title, author, category;
        int year;
        
        cout << "\n--- Add New Book ---" << endl;
        cout << "Enter ISBN: ";
        cin.ignore();
        getline(cin, isbn);
        
        // Check if book already exists
        if (findBookByISBN(isbn) != nullptr) {
            cout << "Error: Book with this ISBN already exists!" << endl;
            return;
        }
        
        cout << "Enter Title: ";
        getline(cin, title);
        
        cout << "Enter Author: ";
        getline(cin, author);
        
        cout << "Enter Category: ";
        getline(cin, category);
        
        cout << "Enter Publication Year: ";
        cin >> year;
        
        books.push_back(Book(isbn, title, author, category, year));
        cout << "Book added successfully!" << endl;
    }
    
    void searchBooks() {
        int choice;
        string searchTerm;
        
        cout << "\n--- Search Books ---" << endl;
        cout << "1. Search by Title" << endl;
        cout << "2. Search by Author" << endl;
        cout << "3. Search by ISBN" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        cout << "Enter search term: ";
        cin.ignore();
        getline(cin, searchTerm);
        
        vector<Book> results;
        
        switch(choice) {
            case 1:
                for (const auto& book : books) {
                    if (book.matchesTitle(searchTerm)) {
                        results.push_back(book);
                    }
                }
                break;
            case 2:
                for (const auto& book : books) {
                    if (book.matchesAuthor(searchTerm)) {
                        results.push_back(book);
                    }
                }
                break;
            case 3:
                for (const auto& book : books) {
                    if (book.matchesISBN(searchTerm)) {
                        results.push_back(book);
                    }
                }
                break;
            default:
                cout << "Invalid choice!" << endl;
                return;
        }
        
        if (results.empty()) {
            cout << "No books found!" << endl;
        } else {
            cout << "\nFound " << results.size() << " book(s):" << endl;
            for (const auto& book : results) {
                book.display();
            }
        }
    }
    
    void displayAllBooks() {
        cout << "\n--- All Books ---" << endl;
        if (books.empty()) {
            cout << "No books in the library!" << endl;
        } else {
            for (const auto& book : books) {
                book.display();
            }
        }
    }
    
    // Member management
    void addMember() {
        string id, name, email, phone;
        
        cout << "\n--- Add New Member ---" << endl;
        cout << "Enter Member ID: ";
        cin.ignore();
        getline(cin, id);
        
        // Check if member already exists
        if (findMemberByID(id) != nullptr) {
            cout << "Error: Member with this ID already exists!" << endl;
            return;
        }
        
        cout << "Enter Name: ";
        getline(cin, name);
        
        cout << "Enter Email: ";
        getline(cin, email);
        
        cout << "Enter Phone: ";
        getline(cin, phone);
        
        members.push_back(Member(id, name, email, phone));
        cout << "Member added successfully!" << endl;
    }
    
    void displayAllMembers() {
        cout << "\n--- All Members ---" << endl;
        if (members.empty()) {
            cout << "No members registered!" << endl;
        } else {
            for (const auto& member : members) {
                member.display();
            }
        }
    }
    
    // Borrow and return operations
    void borrowBook() {
        string memberID, bookISBN;
        
        cout << "\n--- Borrow Book ---" << endl;
        cout << "Enter Member ID: ";
        cin.ignore();
        getline(cin, memberID);
        
        Member* member = findMemberByID(memberID);
        if (!member) {
            cout << "Error: Member not found!" << endl;
            return;
        }
        
        if (!member->canBorrow()) {
            cout << "Error: Member has reached the maximum borrowing limit!" << endl;
            return;
        }
        
        cout << "Enter Book ISBN: ";
        getline(cin, bookISBN);
        
        Book* book = findBookByISBN(bookISBN);
        if (!book) {
            cout << "Error: Book not found!" << endl;
            return;
        }
        
        if (!book->getAvailability()) {
            cout << "Error: Book is already borrowed!" << endl;
            return;
        }
        
        // Create transaction
        Date currentDate = Date::getCurrentDate();
        Date dueDate = currentDate.addDays(14); // 14 days borrowing period
        
        Transaction transaction(generateTransactionID(), bookISBN, memberID, currentDate, dueDate);
        transactions.push_back(transaction);
        
        // Update book and member status
        book->setAvailability(false);
        member->borrowBook();
        
        cout << "Book borrowed successfully!" << endl;
        cout << "Transaction ID: " << transaction.getTransactionID() << endl;
        cout << "Due Date: "; dueDate.display(); cout << endl;
    }
    
    void returnBook() {
        string bookISBN;
        
        cout << "\n--- Return Book ---" << endl;
        cout << "Enter Book ISBN: ";
        cin.ignore();
        getline(cin, bookISBN);
        
        Transaction* transaction = findTransactionByBookISBN(bookISBN, true);
        if (!transaction) {
            cout << "Error: No active borrowing record found for this book!" << endl;
            return;
        }
        
        Book* book = findBookByISBN(bookISBN);
        if (!book) {
            cout << "Error: Book not found in catalog!" << endl;
            return;
        }
        
        Member* member = findMemberByID(transaction->getMemberID());
        if (!member) {
            cout << "Error: Member not found!" << endl;
            return;
        }
        
        // Update transaction
        Date returnDate = Date::getCurrentDate();
        transaction->returnBook(returnDate);
        
        // Update book and member status
        book->setAvailability(true);
        member->returnBook();
        
        cout << "Book returned successfully!" << endl;
        if (transaction->getFineAmount() > 0) {
            cout << "Fine to be paid: $" << fixed << setprecision(2) << transaction->getFineAmount() << endl;
        }
    }
    
    void displayActiveTransactions() {
        cout << "\n--- Active Borrowings ---" << endl;
        bool found = false;
        
        for (const auto& transaction : transactions) {
            if (!transaction.getIsReturned()) {
                transaction.display();
                found = true;
            }
        }
        
        if (!found) {
            cout << "No active borrowings!" << endl;
        }
    }
    
    void displayOverdueBooks() {
        cout << "\n--- Overdue Books ---" << endl;
        Date currentDate = Date::getCurrentDate();
        bool found = false;
        
        for (const auto& transaction : transactions) {
            if (!transaction.getIsReturned() && currentDate > transaction.getDueDate()) {
                transaction.display();
                found = true;
            }
        }
        
        if (!found) {
            cout << "No overdue books!" << endl;
        }
    }
    
    // Statistics
    void displayStatistics() {
        cout << "\n--- Library Statistics ---" << endl;
        cout << "Total Books: " << books.size() << endl;
        
        int availableBooks = 0;
        for (const auto& book : books) {
            if (book.getAvailability()) availableBooks++;
        }
        
        cout << "Available Books: " << availableBooks << endl;
        cout << "Borrowed Books: " << books.size() - availableBooks << endl;
        cout << "Total Members: " << members.size() << endl;
        cout << "Total Transactions: " << transactions.size() << endl;
        
        int activeTransactions = 0;
        for (const auto& transaction : transactions) {
            if (!transaction.getIsReturned()) activeTransactions++;
        }
        
        cout << "Active Borrowings: " << activeTransactions << endl;
    }
    
    // Main menu
    void displayMenu() {
        int choice;
        
        do {
            cout << "\n===== LIBRARY MANAGEMENT SYSTEM =====" << endl;
            cout << "1. Add New Book" << endl;
            cout << "2. Search Books" << endl;
            cout << "3. Display All Books" << endl;
            cout << "4. Add New Member" << endl;
            cout << "5. Display All Members" << endl;
            cout << "6. Borrow Book" << endl;
            cout << "7. Return Book" << endl;
            cout << "8. Display Active Borrowings" << endl;
            cout << "9. Display Overdue Books" << endl;
            cout << "10. Display Statistics" << endl;
            cout << "0. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            
            switch(choice) {
                case 1: addBook(); break;
                case 2: searchBooks(); break;
                case 3: displayAllBooks(); break;
                case 4: addMember(); break;
                case 5: displayAllMembers(); break;
                case 6: borrowBook(); break;
                case 7: returnBook(); break;
                case 8: displayActiveTransactions(); break;
                case 9: displayOverdueBooks(); break;
                case 10: displayStatistics(); break;
                case 0: cout << "Thank you for using Library Management System!" << endl; break;
                default: cout << "Invalid choice! Please try again." << endl;
            }
        } while (choice != 0);
    }
};

int main() {
    Library library;
    library.displayMenu();
    return 0;
}





