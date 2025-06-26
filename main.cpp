#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<ctime>
#include<map>

// This part deals with the password input hiding
#ifdef _WIN32
    #include <conio.h>          //for Windows
    #include<windows.h>
#else
    #include <termios.h>        //for mac
    #include <unistd.h>
#endif

#include<limits>
#include<sstream>
#include<fstream>

using namespace std;

class Book{
    private:
        string title, author, publisher;
        long long int year;
        long long int ISBN;
        string status;
        long long int borrowerID, reservedBy;         //reservedBy -> to add the reservation for the book
        time_t issuedDate, dueDate;
    
    public:
        Book(): status("Available"){}
        Book(const string &title, const string &author, const string &publisher, long long int year, long long int ISBN) 
            : title(title),
            author(author),
            publisher(publisher),
            year(year), ISBN(ISBN),       //initialization list 
            status("Available"),
            borrowerID(0),
            issuedDate(0), 
            dueDate(0), 
            reservedBy(0){}

        void showDetails(){
            cout << "Title: " << title << endl;
            cout << "Author: " << author << endl;
            cout << "Publisher: " << publisher << endl;
            cout << "Year: " << year << endl;
            cout << "ISBN: " << ISBN << endl;
            cout << "Status: " << status << endl;
            cout<<"--------------------------"<<endl;
        }

        //getters to access private variables
        string getTitle(){
            return title;
        }
        long long int getISBN() const{
            return ISBN;
        }
        string getStatus(){
            return status;
        }
        time_t getBorrowDate(){
            return issuedDate;
        }
        time_t getDueDate(){
            return dueDate;
        }
        long long int getBorrowerID(){
            return borrowerID;
        }
        string getAuthor(){
            return author;
        }
        string getPublisher(){
            return publisher;
        }
        long long int getYear(){
            return year;
        }
        long long int getReservedBy(){
            return reservedBy;
        }
        
        //setters to update the private variables
        void setReservedBy(long long int id){
            reservedBy = id;
        }
        void setStatus(string newStatus){
            status = newStatus;
        }
        void setBorrowerID(long long int id){
            borrowerID = id;
        }
        void setissuedDate(time_t date){
            issuedDate = date;
        }
        void setDueDate(time_t date){
            dueDate = date;
        }
        void setISBN(long long int isbn){
            ISBN = isbn;
        }
        void setAuthor(const string& newAuthor){
            author = newAuthor;
        }
        void setTitle(const string& newTitle){
            title = newTitle;
        }
        void setPublisher(const string& newPublisher){
            publisher = newPublisher;
        }
        void setYear(long long int newYear){
            year = newYear;
        }

        //file handling helper function ( to create storage data )
        string serialize(){
            stringstream ss;
            ss << ISBN << "$" << title << "$" 
            << author << "$" << publisher << "$" 
            << year << "$" << status << "$" 
            << borrowerID << "$" << issuedDate << "$" 
            << dueDate << "$" <<reservedBy;
            return ss.str();
        }

        // to extract data from each line of the book.txt file 
        void deserialize(const string& data){
            stringstream ss(data);
            string temp;  // to store the data temporarily(for long long integers)
            
            getline(ss, temp, '$');
            ISBN = stoi(temp);
            getline(ss, title, '$');
            getline(ss, author, '$');
            getline(ss, publisher, '$');
            getline(ss, temp, '$');
            year = stoi(temp);

            getline(ss, status, '$');
            getline(ss, temp, '$');
            borrowerID = stoi(temp);

            getline(ss, temp, '$');
            issuedDate = stoll(temp);
            getline(ss, temp, '$');
            dueDate = stoll(temp);
            getline(ss, temp, '$');
            reservedBy = stoi(temp);
        }
};

// This is forward declaration used because User class uses Account class.
class Account;

class User {
    protected:      //vairables declared as protected because derived class need them to be inherited
        long long int userID;
        string name, password, role;
        Account *account;

    public:
        User(): userID(0), account(nullptr){}
        User(long long int id, string name, string password, string role)
            :userID(id),
            name(name),         //initialization list 
            password(password),
            role(role) {}

        //when this is not declared as virtual then it will not be called in the derived class
        virtual ~User(){}

        void displayDetails(){
            cout << "User ID: " << userID << endl;
            cout << "Name: " << name << endl;
            cout << "Role: " << role << endl;
            cout<<"---------------------------"<<endl;
        }

        long long int getUserID() {
            return userID;
        }
        string getRole() {
            return role;
        }
        string getName(){
            return name;
        }
        string getPassword(){
            return password;
        }

        Account* getAccount(){
            return account;
        }

        // setters to set the variables
        void setAccount(Account* x){
            account = x;
        }

        virtual bool canBorrowBook() = 0;
        virtual long long int getMaxBooks() = 0;
        virtual long long int getMaxDays() = 0;
        virtual bool hastoPayFine() = 0;

        // Serialize user data to string
    string serialize() const {
        stringstream ss;
        ss << userID << "$" << name << "$" << password << "$" << role;
        return ss.str();
    }

    // this function extract the info from the line of user.txt and allocate to the vairables
    void deserialize(const string& data) {
        stringstream ss(data);
        string token;
        
        string idStr;
        getline(ss, idStr, '$');
        userID = stoi(idStr);
        
        getline(ss, name, '$');
        getline(ss, password, '$');
        getline(ss, role, '$');
    }
};

//Derived student class from the User
class Student: public User{
    public:
        Student(): User(){
            role = "Student";
        }

        Student(long long int id,const string& name,const string& password)
            : User(id, name, password, "Student"){}

        bool canBorrowBook(){
            return true;
        }   

        long long int getMaxBooks(){
            return 3;
        }

        long long int getMaxDays(){
            return 15;
        }

        bool hastoPayFine(){
            return true;
        }
};

//Derived Faculty class from the user
class Faculty: public User{
    public:
        Faculty(): User(){
            role = "Faculty";
        }

        Faculty(long long int id,const string& name,const string& password)
            : User(id, name, password, "Faculty"){}

        bool canBorrowBook(){
            return true;
        }

        long long int getMaxBooks(){
            return 5;
        }

        long long int getMaxDays(){
            return 30;
        }

        bool hastoPayFine(){
            return false;
        }
};

//Derived Librarian class from the User class
class Librarian: public User{
    public:
        Librarian(): User(){
            role = "Librarian";
        }

        Librarian(long long int id,const string& name,const string& password)
            : User(id, name, password, "Librarian"){}

        bool canBorrowBook(){
            return false;
        }

        long long int getMaxBooks(){
            return 0;       // librarian can't borrow books and also pure virtual function must have an override in derived class
        }

        long long int getMaxDays(){
            return 0;
        }

        bool hastoPayFine(){
            return false;
        }

};

// This class is for any particular transaction of the user account
class Transaction {
    private:
        long long int ISBN;
        time_t borrowDate, returnDate, dueDate;
        long long int fine;
        bool finePaid;

    public:
        Transaction()       //default constructor
            : borrowDate(0),
              returnDate(0), 
              dueDate(0), 
              fine(0), 
              finePaid(false){}

        Transaction(long long int ISBN, time_t borrowDate, time_t dueDate) //parametrized constructor
            : ISBN(ISBN),
             borrowDate(borrowDate),
             dueDate(dueDate), 
             returnDate(0),
             fine(0), 
             finePaid(false){}

        //getters to access private variables
        long long int getISBN() {return ISBN;}
        time_t getreturnDate() {return returnDate;}
        bool isPaid() {return finePaid;}
        long long int getFine() {return fine;}
        time_t getdueDate() {return dueDate;}
        time_t getborrowDate() {return borrowDate;}

        //setters to update the private variables
        void setreturnDate(time_t date) {returnDate = date;}
        void setFine(long long int newFine) {fine = newFine;}

        // function to handle the file 
        string serialize() const {
            // Using pipe '|' as delimiter for Transaction fields.
            stringstream ss;
            ss << ISBN << "|" << borrowDate << "|" << dueDate << "|" << returnDate << "|" << fine << "|" << finePaid;
            return ss.str();
        }
        
        // This function allocates variable all its previous info saved in the .txt files of user account
        void deserialize(const string& data) {
            stringstream ss(data);
            string token;
            getline(ss, token, '|');
            ISBN = stoi(token);
            getline(ss, token, '|');
            borrowDate = stol(token);
            getline(ss, token, '|');
            dueDate = stol(token);
            getline(ss, token, '|');
            returnDate = stol(token);
            getline(ss, token, '|');
            fine = stoi(token);
            getline(ss, token, '|');
            finePaid = (token == "1"); // or however you represent true/false
        }
        
};

//helper function to deal with the time conversion
//function to convert time to string
string timeToString(time_t time){
    char date[20];
    strftime(date, 20, "%d-%m-%Y %H:%M:%S", localtime(&time));
    return string(date);
}

//This class is to store all the transacrtion history and borrowed history of a particular user
class Account{
    private:
        long long int userID;
        vector<long long int> issuedBooks; // ISBN of borrowed books
        vector<Transaction> borrowHistory;
        long long int totalFine; 

    public: 
        Account(): userID(0), totalFine(0){}
        Account(long long int id): userID(id), totalFine(0){}

        void addBorrowedBook(long long int ISBN){
            issuedBooks.push_back(ISBN);
        }
 
        void addTransaction(Transaction& transaction){
            borrowHistory.push_back(transaction);
        }

        // when user return the book then this will remove the book from the issued book list
        void removeBorrowedBook(long long int ISBN){
            auto it = find(issuedBooks.begin(), issuedBooks.end(), ISBN);
            if(it != issuedBooks.end()){
                issuedBooks.erase(it);
            }
        }

        // this will update the info
        void updateTransaction(long long int ISBN, time_t returnDate, long long int fine){
            for(auto &transaction: borrowHistory){
                if(transaction.getISBN() == ISBN and transaction.getreturnDate() ==0){
                    transaction.setreturnDate(returnDate);
                    transaction.setFine(fine);
                    totalFine+=fine;
                    return;
                }
            }
        }
        
        //check if there is any overdues books
        bool hasOverdueBooks(time_t currentTime, long long int userType){
            // 1 -> Faculty and 2 -> Student
            long long int maxDays = (userType == 1)? 30:15;
            long long int maxOverdueDays = (userType == 1)? 60:0; 

            for(auto &ISBN: issuedBooks){
                for(auto &transaction: borrowHistory){
                    if(transaction.getISBN() == ISBN and transaction.getreturnDate() == 0){
                        // calculate days overdues
                        long long int daysOverdue = static_cast<long long int>(difftime(currentTime,transaction.getdueDate())/86400);
                        if(userType == 2 and daysOverdue > 0){
                            return true;
                        }
                        else if(userType == 1 and daysOverdue > maxOverdueDays){
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        //display the details of the account of the user
        void showDetails(map<long long int, Book> &books){
            cout<<"Account for User ID: "<<userID<<endl;
            cout<<"Currently Borrowed Books: "<<issuedBooks.size()<<endl;
            if(!issuedBooks.empty()){
                cout<<"Borrowed Books: "<<endl;
                for(auto book:issuedBooks){
                    auto it = books.find(book);
                    cout<<"- "<<it->second.getTitle()<<" (ISBN: "<<book<<")"<<endl;
                    for(auto& transaction: borrowHistory){
                        if(transaction.getISBN() == book and transaction.getreturnDate() == 0){
                            cout<<"Due Date: "<<timeToString(transaction.getdueDate())<<endl;
                            break;
                        }
                    }
                }
            }

            if(totalFine > 0){
                cout<<"Outstanding Fine: "<<totalFine<<" rupees"<<endl;
            }

            if(!borrowHistory.empty()){
                cout<<"\n Issued Books History: "<<endl;
                for(auto& transaction: borrowHistory){
                    cout<<"- ISBN: "<<transaction.getISBN()<<endl;
                    cout<<"  Borrowed: "<<timeToString(transaction.getborrowDate())<<endl;
                    cout<<"  Due: "<<timeToString(transaction.getdueDate())<<endl;
                    if(transaction.getreturnDate() > 0){
                        cout<<"  Returned: "<<timeToString(transaction.getreturnDate())<<endl;
                        if(transaction.getFine() > 0){
                            cout<<"  Fine: "<<transaction.getFine()<<" rupees";
                            if(transaction.isPaid()){
                                cout<<" (Paid)"<<endl;
                            }
                            else{
                                cout<<" (Unpaid)"<<endl;
                            }
                        }
                    }
                    else{
                        cout<<"  Status: Not returned yet"<<endl;
                        time_t currentTime = time(0);
                        if(currentTime > transaction.getdueDate()){
                            long long int daysOverdue = static_cast<long long int>(difftime(currentTime, transaction.getdueDate())/86400);
                            cout<<"  Overdue by "<<daysOverdue<<" days"<<endl;
                        }
                    }
                }
            }
        }

        // when a user pays fine (only full payment allowed)
        void clearAllDues(){
            totalFine = 0;
            for(auto &transaction: borrowHistory){
                if(!transaction.isPaid() and transaction.getFine() > 0){
                    transaction.setFine(0);
                }
            }
        }

        // Recalculate overdue fines for ongoing transactions (only applicable for Student accounts)
        void updateFine(time_t currentTime, const string &userRole) {
            long long int updatedFine = 0;
            for(auto &transaction: borrowHistory) {
                // If the book is still not returned and is overdue, calculate fine based on days late.
                if(transaction.getreturnDate() == 0 && currentTime > transaction.getdueDate()){
                    long long int daysLate = static_cast<long long int>(difftime(currentTime, transaction.getdueDate())/86400);
                    if(userRole == "Student"){
                        transaction.setFine(daysLate * 10);
                        updatedFine += daysLate * 10;
                    }
                }
                // For a returned transaction that is unpaid, add its fine
                else if(transaction.getreturnDate() > 0 && !transaction.isPaid()){
                    updatedFine += transaction.getFine();
                }
            }
            totalFine = updatedFine;
        }

        //getter functions to access private variables
        vector<long long int> getIssuedBooks(){
            return issuedBooks;
        }
        long long int getTotalFine(){
            return totalFine;
        }
        long long int getUserID(){
            return userID;
        }

        //function to handle the file data
        string serialize() const {
            stringstream ss;
            ss << userID << "$" 
               << totalFine << "$" 
               << issuedBooks.size() << "$";
            
            // Serialize issuedBooks, separated by commas.
            for (size_t i = 0; i < issuedBooks.size(); i++){
                ss << issuedBooks[i];
                if(i != issuedBooks.size() - 1)
                    ss << ",";
            }
            
            ss << "$" << borrowHistory.size() << "$";
            // Serialize each transaction using the custom serialize() from Transaction.
            for (size_t i = 0; i < borrowHistory.size(); i++){
                ss << borrowHistory[i].serialize();
                if(i != borrowHistory.size() - 1)
                    ss << "#"; // using '#' to separate multiple transaction records.
            }
            return ss.str();
        }
        
        void deserialize(const string& data) {
            stringstream ss(data);
            string temp;
            // Read basic account data separated by '$'
            getline(ss, temp, '$');
            userID = stoi(temp);
            getline(ss, temp, '$');
            totalFine = stoi(temp);
        
            getline(ss, temp, '$');
            long long int bookCount = stoi(temp);
            getline(ss, temp, '$');
            stringstream bookStream(temp);
            for (long long int i = 0; i < bookCount; i++) {
                getline(bookStream, temp, ',');
                issuedBooks.push_back(stoi(temp));
            }
            
            getline(ss, temp, '$');
            long long int transactionCount = stoi(temp);
            getline(ss, temp, '$');  // This now gets the entire transaction record string.
            stringstream transactionStream(temp);
            for (long long int i = 0; i < transactionCount; i++) {
                string transactionData;
                // Use '#' to separate individual transactions.
                if(getline(transactionStream, transactionData, '#')){
                    Transaction transaction;
                    transaction.deserialize(transactionData);
                    borrowHistory.push_back(transaction);
                }
            }
        }
};

class Library{
    private:
        map<long long int, Book> books;  //linking ISBN to Book
        map<long long int, User*> users; //linking userID to User
        map<long long int, Account> accounts; //linking userID to Account
        long long int userID;

    public:
        Library() : userID(0){}

        //function to add a book to the library
        void addBook(const Book &book){
            books[book.getISBN()] = book;
        }

        //function to add a user to the library
        void addUser(User* user){
            users[user->getUserID()] = user;
            if(user->getRole() != "Librarian"){
                accounts[user->getUserID()] = Account(user->getUserID());
                user->setAccount(&accounts[user->getUserID()]);
            }
        }

        //function to search a user by their ID
        User* getUser(long long int userId){
            auto it = users.find(userId);
            if(it != users.end()){
                return it->second;
            }
            return NULL;
        }

        long long int getNextUserId(){
            return userID;
        }

        long long int setNextUserId(){
            return ++userID;
        }

        //function to remove a book from the library
        bool removeBook(long long int& isbn){
            auto it = books.find(isbn);
            if(it!=books.end()){
                if(it->second.getStatus() == "Borrowed"){
                    cout<<"Book is currently issued. Cannot remove 🫤"<<endl;
                    return false;
                }
                books.erase(it);
                return true;
            }
            return false;
        }

        //function to remove a user account from the library
        bool removeUser(long long int userID){
            auto it = users.find(userID);

            if(it->second->getRole()=="Librarian"){
                cout<<"Librarian(Admin) account cannot be removed 🙅‍♂️"<<endl;
                return false;
            }

            if(it != users.end()){
                if(it->second->getRole() != "Librarian" and !accounts[userID].getIssuedBooks().empty()){
                    cout<<"User have some issued books. Can't deregister 🙄"<<endl;
                    return false;
                }
                delete it->second;
                users.erase(it);
                accounts.erase(userID);
                return true;
            }
            return false;
        }

        //function to reserve the book if the book is not issued already
        bool reserveBook(long long int userId, long long int isbn){
            auto userIt = users.find(userId);
            auto bookIt = books.find(isbn);
            
            if(userIt == users.end() or bookIt == books.end()){
                cout<<"Oops! User or Book not found in Database 🙅‍♂️"<<endl;
                return false;
            }
            Book &book = bookIt->second;
            if(book.getStatus()=="Available"){
                cout<<"Book is available. No need to reserve 😇"<<endl;
                return false;
            }
            if(book.getReservedBy() != 0){
                cout<<"Book is already reserved by someone else, cannot reserve 🤨"<<endl;
                return false;
            }

            if(userId==book.getBorrowerID()){
                cout<<"You can't reserve a book that you have already issued 🤦"<<endl;
                return false;
            }

            // set the reservation for book
            book.setReservedBy(userId);
            cout<<"Book reserved successfully. It will automatically issued to your account. You will be informed 👍"<<endl;
            return true;
        }

        // function to issue a book to a user
        bool issueBook(long long int userID, const long long int& isbn, time_t currentTime){
            // finding if they exist in the database
            auto userIt = users.find(userID);
            auto bookIt = books.find(isbn);

            if(userIt == users.end() or bookIt == books.end()){
                cout<<"Oops! User or Book not found in Database 🙅‍♂️"<<endl;
                return false;
            }

            User* user = userIt->second;
            Book& book = bookIt->second;

            //checking if the user can borrow books
            if(!user->canBorrowBook()){         //if the user is not permitted to issue any books
                cout<<"You are not permitted to issue any books 🚫"<<endl;
                return false;
            }

            if(book.getStatus()!="Available" && book.getBorrowerID()==userID){ //if the book is already issued to the user
                cout<<"You have already issued this book 🤨"<<endl;
                return false;
            }

            if(book.getStatus() != "Available"){    //if the book is already issued to someone else
                cout<<"Sorry! someone else borrowed it. Not Available 🥲"<<endl;
                return false;
            }

            Account& account = accounts[userID];

            if(account.getIssuedBooks().size() >= user->getMaxBooks()){     //if the user has already borrowed the maximum number of books
                cout<<"Books borrowing limit exceed. Return to issue this one 🫠"<<endl;
                return false;
            }

            if(account.getTotalFine() > 0){
                cout<<"Clear Your Dues to issue any books 😇"<<endl;
                return false;
            }

            if(user->getRole() == "Faculty" and account.hasOverdueBooks(currentTime, 1)){
                cout<<"User has books overdue for more than 60 days. Cannot borrow more 🚫"<<endl;
                return false;
            }
            
            //updating the book status and other details
            book.setStatus("Borrowed");
            book.setBorrowerID(userID);
            book.setissuedDate(currentTime);
            book.setDueDate(currentTime+user->getMaxDays()*86400);

            //updating the account details
            account.addBorrowedBook(book.getISBN());

            //adding the transaction to the account
            Transaction newRecord(isbn, currentTime, book.getDueDate());
            account.addTransaction(newRecord);

            //confirmation message
            cout<<"Book Issued Successfully 👍. Due date: "<<timeToString(book.getDueDate())<<endl;
            return true;
        }

        //function to return the book to the library
        void returnBook(long long int userID, const long long int& isbn, time_t currentTime){
            auto userIt = users.find(userID);
            auto bookIt = books.find(isbn);

            if(userIt == users.end() or bookIt == books.end()){
                cout<<"User or Book not found"<<endl;
                return;
            }

            User* user = userIt->second;
            Book& book = bookIt->second;

            if(book.getStatus() != "Borrowed" or book.getBorrowerID() != userID){
                cout<<"This book is not borrowed by this user or not issued yet 🥱"<<endl;
                return;
            }

            long long int fine = 0;
            Account& account = accounts[userID];
            if(currentTime > book.getDueDate() and user->hastoPayFine()){
                long long int daysLate = static_cast<long long int>(difftime(currentTime, book.getDueDate())/86400);
                fine = daysLate*10;
                cout<<"Book is "<<daysLate<<" days late. Fine: "<<fine<<" ₨"<<endl;
            }

            //reseting the book details to mark it as retured
            book.setStatus("Available");
            book.setBorrowerID(-1);
            book.setissuedDate(0);
            book.setDueDate(0);

            account.removeBorrowedBook(isbn);
            account.updateTransaction(isbn, currentTime, fine);

            cout<<"Book returned successfully 👍"<<endl;
            if(fine > 0){
                cout<<"Fine added to account: "<<fine<<" rupees "<<endl;
            }
            //checking if the book has a reservation or not
            if(book.getReservedBy()!=0){
                long long int reservedUserID = book.getReservedBy();
                User* reservedUser = getUser(reservedUserID);
                if(reservedUser!=NULL and reservedUser->canBorrowBook()){
                    //Automatically issue the book
                    issueBook(reservedUserID, isbn, currentTime);
                    book.setReservedBy(0);
                    cout<<"Book has been successfully issued to reserved"<<" User - "<<reservedUserID<<"👍"<<endl;
                }
            }

        }

        //functions used to display the details
        // To show all the books
        void showAllBooks(){
            cout<<"\n-----------------> All Books <-------------------"<<endl;
            if(books.empty()){
                cout<<"No books in the library 😕"<<endl;
            }
            else{
                for(auto book = books.begin(); book != books.end(); book++){
                    book->second.showDetails();
                }
            }
        }

        // To show all the available books
        void showAvailableBooks(){
            cout<<"\n-----------------> Available Books <-----------------"<<endl;
            bool checker = false;
            for(auto book = books.begin(); book != books.end(); book++){
                if(book->second.getStatus() == "Available"){
                    book->second.showDetails();
                    checker = true;
                }
            }
            if(!checker) cout<<"No available books in the library ☹️"<<endl;
        }

        // To show all the users registered in the library
        void showAllUsers(){
            cout<<"\n==== All Users ===="<<endl;
            if(users.empty()){
                cout<<"Nobody registered as users 🥲"<<endl;
            }
            else{
                for(auto user = users.begin(); user != users.end(); user++){
                    user->second->displayDetails();
                }
            }
        }

        // To display particular users according to their roles
        void showUsersByRole(const string& role){
            cout<<"\n-----------------> "<<role<<"s <-----------------"<<endl;
            bool checker = false;
            for(auto user = users.begin(); user != users.end(); user++){
                if(user->second->getRole()==role){
                    user->second->displayDetails();
                    checker = true;
                }
            }
            if(!checker) cout<<"None of this role present in the system 😕"<<endl;
        }

        // To display the account details of a particular user
        void showUserAccount(long long int userID){
            auto userIt = users.find(userID);
            auto accountIt = accounts.find(userID);
            if(userIt != users.end() and accountIt!=accounts.end()){
                cout<<"\n==== Account Details for "<<userIt->second->getName()<<" ===="<<endl;
                accountIt->second.showDetails(books);
            }else{
                cout<<"Either User or Account not found 😕"<<endl;
            }
        }

        // to display the details of the book & this also ensures that the book is being searched by the ISBN number
        void showBookDetails(long long int isbn){
            auto it = books.find(isbn);
            if(it != books.end()){
                it->second.showDetails();
            }else{
                cout<<"Book not found in the system 🙅‍♂️"<<endl;
            }
        }

        //function to search book by title
        void searchBooksByTitle(const string &title){
            cout<<"\n==== Search Results for Title: "<<title<<" ===="<<endl;
            bool checker = false;
            for(auto book = books.begin(); book != books.end(); book++){
                if(book->second.getTitle().find(title)!= string::npos){
                    book->second.showDetails();
                    checker = true;
                }
            }
            if(!checker) cout<<"No books found with that title 🙅‍♂️"<<endl;
        }

        //function to search book by author name in the system
        void searchBooksByAuthor(const string& author){
            cout<<"\n==== Search Results for Author: "<<author<<" ===="<<endl;
            bool checker = false;
            for(auto book = books.begin(); book != books.end(); book++){
                if(book->second.getAuthor().find(author)!= string::npos){
                    book->second.showDetails();
                    checker = true;  //this is to check if any book is found or not
                }
            }
            if(!checker) cout<<"No books found with that author 🙅‍♂️"<<endl;
        }

        // function to authenticate users
        bool authenticateUser(long long int userID, const string& password){
            auto it = users.find(userID);
            if(it != users.end()){
                return it->second->getPassword()==password;
            }
            return false;
        }

        // function if the book exists in the library
        bool bookExists(long long int isbn){
            return books.find(isbn) != books.end();
        }

        //function to get the books
        Book& getBook(long long int isbn){
            return books[isbn];
        }

        //function to save the data to the files
        void saveData(){
            // First, update overdue fines for all student accounts using current time.
            time_t currentTime = time(0);
            for(auto &pair: accounts){
                User* user = getUser(pair.first);
                if(user && user->getRole() == "Student"){
                    pair.second.updateFine(currentTime, user->getRole());
                }
            }

            ofstream bookFile("books.txt");
            if(bookFile.is_open()){
                for(auto &pair: books){
                    bookFile<<pair.second.serialize()<<endl;
                }
                bookFile.close();
            }

            ofstream userFile("users.txt");
            if(userFile.is_open()){
                for(auto &pair: users){
                    userFile<<pair.second->getRole()<<"$"<<pair.second->serialize()<<endl;
                }
                userFile.close();
            }

            ofstream accountFile("accounts.txt");
            if(accountFile.is_open()){
                for(auto &pair: accounts){
                    accountFile<<pair.second.serialize()<<endl;
                }
                accountFile.close();
            }
            cout<<"Data Saved Successfully ✅"<<endl;
        }

        // load data from existing files
        void loadData(){
            // removing existing data from memory
            for(auto &pair: users){
                delete pair.second;
            }
            users.clear();
            books.clear();
            accounts.clear();

            // loading books 
            ifstream bookFile("books.txt");
            if(bookFile.is_open()){
                string line;
                while(getline(bookFile, line)){
                    Book book;
                    book.deserialize(line);
                    books[book.getISBN()] = book;
                }
                bookFile.close();
            }

            //load Data of users
            ifstream userFile("users.txt");
            if(userFile.is_open()){
                string line;
                while(getline(userFile, line)){
                    stringstream ss(line);
                    string role;
                    getline(ss, role, '$');
                
                    string userData;
                    getline(ss, userData);
                    
                    User* user = nullptr;
                    if (role == "Student") {
                        user = new Student();
                    } else if (role == "Faculty") {
                        user = new Faculty();
                    } else if (role == "Librarian") {
                        user = new Librarian();
                    }
                    
                    if (user) {
                        user->deserialize(userData);
                        users[user->getUserID()] = user;
                        
                        // Update nextUserID
                        if (user->getUserID() >= userID) {
                            userID = user->getUserID() + 1;
                        }
                    }
                }
                userFile.close();
            }

            // Load accounts
            ifstream accountFile("accounts.txt");
            if (accountFile.is_open()) {
                string line;
                while (getline(accountFile, line)) {
                    Account account;
                    account.deserialize(line);
                    accounts[account.getUserID()] = account;

                    auto userIt = users.find(account.getUserID());
                    if(userIt != users.end()){
                        userIt->second->setAccount(&accounts[account.getUserID()]);
                    }
                }
                accountFile.close();
            }
            cout<<"Past Data Loaded Successfully ✅"<<endl;
        }

        // function to  initialize the data of books and usersd
        void initialize(){
            //here these literals can only call the const functions
            addBook(Book("Title 1", "Author 1", "Publisher 1", 2000, 10000));
            addBook(Book("Title 2", "Author 2", "Publisher 2", 2001, 10001));
            addBook(Book("Title 3", "Author 3", "Publisher 3", 2002, 10002));
            addBook(Book("Title 4", "Author 4", "Publisher 4", 2003, 10003));
            addBook(Book("Title 5", "Author 5", "Publisher 5", 2004, 10004));
            addBook(Book("Title 6", "Author 6", "Publisher 6", 2006, 10005));
            addBook(Book("Title 7", "Author 7", "Publisher 7", 2007, 10006));
            addBook(Book("Title 8", "Author 8", "Publisher 8", 2008, 10007));
            addBook(Book("Title 9", "Author 9", "Publisher 9", 2009, 10008));
            addBook(Book("Title 10", "Author 10", "Publisher 10", 2010, 10009));

            //adding default users
            addUser(new Student(++userID, "Student 1", "student"));
            addUser(new Student(++userID, "Student 2", "student"));
            addUser(new Student(++userID, "Student 3", "student"));
            addUser(new Student(++userID, "Student 4", "student"));
            addUser(new Student(++userID, "Student 5", "student"));

            addUser(new Faculty(++userID, "Faculty 1", "faculty"));
            addUser(new Faculty(++userID, "Faculty 2", "faculty"));
            addUser(new Faculty(++userID, "Faculty 3", "faculty"));

            addUser(new Librarian(++userID, "Librarian", "admin"));
        }
};

// consolelong long interface class to handle user long long interface
class consoleInterface{
    private:
        Library library;
        User* currentUser;

        //function to get the choice of the user
        long long int userWant(long long int low, long long int high){
            long long int choice;
            while(1){
                cout<<"Enter your choice: ";
                while(!(cin >> choice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "🥲Oops!! Choice should be an long long integer. \nEnter Your Choice: ";
                }
                if(choice >= low and choice <= high){
                    return choice;
                }
                else{
                    cout<<"🥲Invalid choice. Please try again "<<endl;
                }
            }
        }

        // A cross-platform function to get a password with masked input.
        string getPassword() {
            string password;
        #ifdef _WIN32
            // Windows implementation using conio.h
            char ch;
            while((ch = _getch()) != '\r') { // Enter pressed?
                if(ch == '\b') { // Backspace
                    if(!password.empty()){
                        password.pop_back();
                        cout << "\b \b"; // Erase the last asterisk
                    }
                } else {
                    password.push_back(ch);
                    cout << "*";
                }
            }
        #else
            // macOS / POSIX implementation using termios
            termios oldt, newt;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ECHO | ICANON);
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            tcflush(STDIN_FILENO, TCIFLUSH);
            
            char ch;
            while ((ch = getchar()) != '\n' && ch != '\r' && ch != EOF) {
                if(ch == 127 || ch == 8) { // Handle backspace
                    if(!password.empty()){
                        password.pop_back();
                        cout << "\b \b";
                    }
                } else {
                    password.push_back(ch);
                    cout << "*";
                }
            }
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        #endif
            cout << "\n";
            return password;
        }

        // for security password prlong long int in the form of ******
        string getLine(const string& prompt) {   
            cout << prompt;
            if (prompt.find("Password")!=string::npos) {
                return getPassword();
            }else{
                string input;
                if(cin.peek() == '\n')
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, input);
                return input;
            }
        }

    public:
        consoleInterface(): currentUser(NULL){};

        void showLoginMenu(){
            cout<<"===== Library Management System ====="<<endl;
            cout<<"1. Login"<<endl;
            cout<<"2. Quit"<<endl;

            long long int choice = userWant(1, 2);
            if(choice==1) login();
            else if(choice==2) exit(0);
        }
        
        void login(){
            cout << "===== Login Window =====" << endl;
        
            long long int userID;
            cout << "Enter User ID: ";
            while(!(cin >> userID)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Oops!! User ID should be an long long integer. \nEnter User ID: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Flush newline
        
            string password = getLine("Enter Password: ");
        
            if(library.authenticateUser(userID, password)){
                currentUser = library.getUser(userID);
                cout << "Login Successful. Welcome, " << currentUser->getName() << "! 😊" << endl;
            }
            else{
                cout << "Login Failed. Invalid User ID or Password 👎" << endl;
            }
        }

        void logout(){
            currentUser = nullptr;
            cout<<"Logged out successfully ✅"<<endl;
            // waitForInput();
        }

        void searchMenu(){
            cout<<"===== Book Search Menu ====="<<endl;
            cout<<"1. Search using Title"<<endl;
            cout<<"2. Search using Author"<<endl;
            cout<<"3. Search using ISBN"<<endl;
            cout<<"4. Back"<<endl;

            long long int choice = userWant(1, 4);
            if(choice==1){
                string title;
                cout<<"Enter Title: ";
                cin.ignore();       // to ignore the whitespace in the title
                getline(cin, title);
                library.searchBooksByTitle(title);
            }
            else if(choice==2){
                string author;
                cout<<"Enter Author: ";
                cin.ignore();
                getline(cin, author);
                library.searchBooksByAuthor(author);
            }
            else if(choice==3){
                long long int isbn;
                cout<<"Enter ISBN: ";
                while(!(cin >> isbn)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "ISBN should be an long long integer.\n Enter ISBN: ";
                }
                library.showBookDetails(isbn);
            }else return;
        }

        void issueBook(){
            cout<<"======To Issue a Book ======= "<<endl;
            long long int isbn; 
            cout<<"Enter ISBN of the book to issue: ";
            cin>>isbn;
            if(!library.issueBook(currentUser->getUserID(), isbn, time(0))){
                cout<<"Book issued Failed 🚫"<<endl;
            }
        }

        void reserveBook(){
            cout<<"======To Reserve a Book ======= "<<endl;
            long long int isbn; 
            cout<<"Enter ISBN of the book to reserve: ";
            cin>>isbn;
            if(!library.bookExists(isbn)){
                cout<<"Book not found in the library 🙅‍♂️"<<endl;
                return;
            }
            if(!library.reserveBook(currentUser->getUserID(), isbn)){
                cout<<"Book reservation failed 🚫"<<endl;
            }
        }

        void returnBook(){
            cout<<"======Return a Book ======="<<endl;
            library.showUserAccount(currentUser->getUserID());
            cout<<"Enter ISBN of issued book: ";
            long long int isbn; cin>>isbn;
            library.returnBook(currentUser->getUserID(), isbn, time(0));
        }

        void payFines(){
            cout<<"====== Clear Your Dues ======"<<endl;
            library.showUserAccount(currentUser->getUserID());
            Account* account = currentUser->getAccount();
            if(account == nullptr){
                cout<<"Account not exists 👎! Try Again"<<endl;
                return;
            }

            long long int totalFine = account->getTotalFine();
            cout<<totalFine<<endl;
            if(totalFine <= 0){ //handling not possible case of negative fines
                cout<<"You have no outstanding fines 😇"<<endl;
                return;
            }

            cout<<"Total Fine: "<<totalFine<<" rupees"<<endl;
            cout<<"1. Pay All Fines"<<endl;
            cout<<"2. Back"<<endl;

            long long int choice = userWant(1, 2);
            if(choice==1){
                account->clearAllDues();
                cout<<"All fines paid successfully ✅"<<endl;
            }else{
                studentMenu();
            }
        }

        void studentMenu(){
            cout << "\n=====--- Student Menu ----=====" << endl;
            cout << "Welcome, " << currentUser->getName() << " <---------------------(Student)" << endl;
            cout << "1. View Available Books" << endl;
            cout << "2. View My Account" << endl;
            cout << "3. Search Books" << endl;
            cout << "4. Issue a Book" << endl;
            cout << "5. Reserve a Book" << endl;
            cout << "6. Return a Book" << endl;
            cout << "7. Pay Fines" << endl;
            cout << "8. Logout" << endl;

            long long int choice = userWant(1, 8);
            switch(choice){
                case 1:
                    library.showAvailableBooks();
                    break;
                case 2:
                    library.showUserAccount(currentUser->getUserID());
                    break;
                case 3:
                    searchMenu();
                    break;
                case 4:
                    issueBook();
                    break;
                case 5: 
                    reserveBook();
                    break;
                case 6:
                    returnBook();
                    break;
                case 7:
                    payFines();
                    break;
                case 8:
                    logout();
                    break;
            }
        }

        void facultyMenu(){
            cout << "\n=====---- Faculty Menu ----=====" << endl;
            cout << "Welcome, " << currentUser->getName() << " <---------------------(Faculty)" << endl;
            cout << "1. View Available Books" << endl;
            cout << "2. View My Account" << endl;
            cout << "3. Search Books" << endl;
            cout << "4. Issue a Book" << endl;
            cout << "5. Reserve a Book" << endl;
            cout << "6. Return a Book" << endl;
            cout << "7. Logout" << endl;

            long long int choice = userWant(1, 7);
            switch(choice){
                case 1:
                    library.showAvailableBooks();
                    break;
                case 2:
                    library.showUserAccount(currentUser->getUserID());
                    break;
                case 3:
                    searchMenu();
                    break;
                case 4:
                    issueBook();
                    break;
                case 5:
                    reserveBook();
                    break;
                case 6:
                    returnBook();
                    break;
                case 7:
                    logout();
                    break;
            }
        }

        //librarian section

        // function to add a book to the library
        void addBook(){
            cout<<"\n====== Add a Book ======"<<endl;
            string title = getLine("Enter Title: ");
            string author = getLine("Enter Author: ");
            string publisher = getLine("Enter Publisher: ");
            cout<<"Enter Year: ";
            long long int year; cin>>year;
            cout<<"\nEnter ISBN: ";
            long long int isbn; cin>>isbn;

            //checking if the book already exists
            if(library.bookExists(isbn)){
                cout<<"Book with this ISBN already exists"<<endl;
                return;
            }
            Book book(title, author, publisher, year, isbn);
            library.addBook(book);
            cout<<"Book added successfully"<<endl;
        }

        // function to remove a book from the library
        void removeBook(){
            cout<<"\n====== Remove a Book ======"<<endl;
            cout<<"Enter ISBN of the book: ";
            long long int isbn; cin>>isbn;
            if(library.removeBook(isbn)){
                cout<<"Book removed successfully"<<endl;
            }else{
                cout<<"Book not found"<<endl;
            }
        }

        // function to update the details of the book
        void updateBook(){
            cout<<"\n====== Update a Book ======"<<endl;
            cout<<"Enter ISBN of the book: ";
            long long int isbn; cin>>isbn;
            if(!library.bookExists(isbn)){
                cout<<"Book not found"<<endl;
                return;
            }

            library.showBookDetails(isbn);

            string title = getLine("Enter new Title: ");
            string author = getLine("Enter new Author: ");
            string publisher = getLine("Enter new Publisher: ");
            long long int year = -1;
            cout<<"Enter new Year(enter -1 to leave unchanged): ";
            cin>>year;

            Book& book = library.getBook(isbn);
            cout<<title<<"-"<<author<<"-"<<publisher<<"-"<<year<<endl;
            book.setTitle(title.empty()? book.getTitle(): title);
            book.setAuthor(author.empty()? book.getAuthor(): author);
            book.setPublisher(publisher.empty()? book.getPublisher(): publisher);
            book.setYear(year == -1? book.getYear(): year);
            cout<<"Book updated successfully"<<endl;

        }

        void showUserTypes(){
            cout<<"\n==== Users in the System ===="<<endl;
            cout<<"1. All Users"<<endl;
            cout<<"2. Students"<<endl;
            cout<<"3. Faculty"<<endl;
            cout<<"4. Librarian"<<endl;
            cout<<"5. Back"<<endl;

            long long int choice = userWant(1, 5);
            switch(choice){
                case 1:
                    library.showAllUsers();
                    break;
                case 2:
                    library.showUsersByRole("Student");
                    break;
                case 3:
                    library.showUsersByRole("Faculty");
                    break;
                case 4:
                    library.showUsersByRole("Librarian");
                    break;
                case 5:
                    librarianMenu();
                    break;
            }
        }

        //this function allow the librarian to add the user
        void addUser(){
            cout<<"\n====== Add a User ======"<<endl;
            string name = getLine("Enter Name: ");
            string password = getLine("Enter Password: ");
            cout<<"Choose who you are: "<<endl;
            cout<<"1. Student"<<endl;
            cout<<"2. Faculty"<<endl;
            cout<<"3. Back" <<endl;

            long long int roleChoice = userWant(1, 3);
            User* newUser = NULL;
            long long int newID = library.getNextUserId()+1;
            library.setNextUserId();
            if(roleChoice==1){
                newUser = new Student(newID, name, password);
            }else if(roleChoice==2){
                newUser = new Faculty(newID, name, password);
            }else if(roleChoice==3){
                librarianMenu();
                return;
            }
            if(newUser !=NULL){
                library.addUser(newUser);
                cout<<"User added successfully"<<endl;
            }
        }

        void removeUser(){
            cout<<"====== Remove a User ======"<<endl;
            cout<<"Enter User ID: ";
            long long int userID; cin>>userID;

            bool checkUser = currentUser->getUserID() == userID;

            if(library.removeUser(userID)){
                cout<<"User removed successfully ✅"<<endl;
                if(checkUser){
                    logout();
                }
            }else{
                cout<<"User not found 🙅‍♂️"<<endl;
            }
        }

        void showUserAccountDetails(){
            cout<<"====== View User Account ======"<<endl;
            cout<<"Enter User ID: ";
            long long int userID; cin>>userID;

            if(library.getUser(userID) == NULL){
                cout<<"User not found 🙅‍♂️"<<endl;
                return;
            }
            library.showUserAccount(userID);
        }

        void librarianMenu(){
            cout << "\n=====---- Librarian Menu ----=====" << endl;
            cout << "Welcome, " << currentUser->getName() << " <---------------------(Librarian)" << endl;
            cout << "1. View All Books" << endl;
            cout << "2. Add a Book" << endl;
            cout << "3. Remove a Book" << endl;
            cout << "4. Update a Book" << endl;
            cout << "5. View All Users" << endl;
            cout << "6. Add a User" << endl;
            cout << "7. Remove a User" << endl;
            cout << "8. View User Account" << endl;
            cout << "9. Search Books" << endl;
            cout << "10. Logout" << endl;

            long long int choice = userWant(1, 10);
            switch(choice){
                case 1:
                    library.showAllBooks();
                    break;
                case 2:
                    addBook();
                    break;
                case 3:
                    removeBook();
                    break;
                case 4:
                    updateBook();
                    break;
                case 5:
                    showUserTypes();
                    break;
                case 6:
                    addUser();
                    break;
                case 7:
                    removeUser();
                    break;
                case 8:
                    showUserAccountDetails();
                    break;
                case 9:
                    searchMenu();
                    break;
                case 10:
                    logout();
                    break;
            }
        }

        //to start the system
        void run(){
            // try to access data from the saved files
            ifstream file("books.txt");
            if(file.good()){
                file.close();
                library.loadData();
            }else{
                library.initialize();
            }

            while(1){
                if(!currentUser){
                    showLoginMenu();
                }else if(currentUser->getRole()=="Student"){
                    studentMenu();
                }else if(currentUser->getRole()=="Faculty"){
                    facultyMenu();
                }else if(currentUser->getRole()=="Librarian"){
                    librarianMenu();
                }
                library.saveData();
                cout<<"=================================(^_^)=================================="<<endl;
            }
        }
};

int main(){
    #ifdef _WIN32
    // Setting Windows console output to UTF-8 for emoji support
    SetConsoleOutputCP(CP_UTF8);
    #endif
    consoleInterface lib;
    lib.run();
    return 0;
}