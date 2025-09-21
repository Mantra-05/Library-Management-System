#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <sqlite3.h>
#include <stdlib.h>
#include <conio.h>
using namespace std;

class LibraryEntity
{
protected:
    char id[6];

public:
    virtual void create() = 0;
    virtual void show() = 0;
    virtual void report() const = 0;
    const char *getID() const { return id; }
    virtual ~LibraryEntity() {}
};

class Book : public LibraryEntity
{
private:
    char bname[50];
    char aname[20];

public:
    void create() override
    {
        cout << "\nNEW BOOK ENTRY...\n";
        cout << "\nENTER BOOK NO.: ";
        cin >> id;
        cout << "ENTER BOOK NAME: ";
        cin.ignore();
        cin.getline(bname, 50);
        cout << "ENTER AUTHOR NAME: ";
        cin.getline(aname, 20);
        cout << "\n\nBook Created..";
    }
    void show() override
    {
        cout << "\nBook Number: " << id;
        cout << "\nBook Name: " << bname;
        cout << "\nAuthor Name: " << aname;
    }
    void report() const override
    {
        cout << id << setw(30) << bname << setw(30) << aname << endl;
    }
    const char *getBname() const { return bname; }
    const char *getAname() const { return aname; }
    void setData(const char *bid, const char *name, const char *auth)
    {
        strcpy(id, bid);
        strcpy(bname, name);
        strcpy(aname, auth);
    }
};

class Student : public LibraryEntity
{
private:
    char name[20];
    char stbno[6];
    int token;

public:
    void create() override
    {
        cout << "\nNEW STUDENT ENTRY...\n";
        cout << "\nEnter The Admission No.: ";
        cin >> id;
        cout << "Enter The Student Name: ";
        cin.ignore();
        cin.getline(name, 20);
        token = 0;
        stbno[0] = '\0';
        cout << "\n\nStudent Record Created...";
    }
    void show() override
    {
        cout << "\nAdmission Number: " << id;
        cout << "\nStudent Name: " << name;
        cout << "\nNo of Book Issued: " << token;
        if (token == 1)
            cout << "\nBook Number: " << stbno;
    }
    void report() const override
    {
        cout << "\t" << id << setw(20) << name << setw(10) << token << endl;
    }
    const char *getName() const { return name; }
    const char *getStbno() const { return stbno; }
    int getToken() const { return token; }
    void addToken() { token = 1; }
    void resetToken() { token = 0; }
    void setStbno(const char *t) { strcpy(stbno, t); }
    void setData(const char *sid, const char *n, int t, const char *bookid)
    {
        strcpy(id, sid);
        strcpy(name, n);
        token = t;
        strcpy(stbno, bookid);
    }
};

class FileManager
{
private:
    sqlite3 *db;
    static int displayCallback(void *data, int argc, char **argv, char **azColName)
    {
        for (int i = 0; i < argc; i++)
        {
            cout << argv[i] << "\t";
        }
        cout << endl;
        return 0;
    }

public:
    FileManager()
    {
        if (sqlite3_open("library.db", &db))
        {
            cerr << "Can't open database\n";
            exit(1);
        }
        const char *bookTable = "CREATE TABLE IF NOT EXISTS Books(book_id TEXT PRIMARY KEY,name TEXT,author TEXT);";
        const char *studentTable = "CREATE TABLE IF NOT EXISTS Students(student_id TEXT PRIMARY KEY,name TEXT,token INT,issued_book_id TEXT,FOREIGN KEY(issued_book_id) REFERENCES Books(book_id));";
        char *err = 0;
        sqlite3_exec(db, bookTable, 0, 0, &err);
        sqlite3_exec(db, studentTable, 0, 0, &err);
    }
    ~FileManager() { sqlite3_close(db); }
    void addBook()
    {
        Book b;
        b.create();
        string sql = "INSERT INTO Books VALUES('" + string(b.getID()) + "','" + string(b.getBname()) + "','" + string(b.getAname()) + "');";
        char *err = 0;
        sqlite3_exec(db, sql.c_str(), 0, 0, &err);
    }
    void addStudent()
    {
        Student s;
        s.create();
        string sql = "INSERT INTO Students VALUES('" + string(s.getID()) + "','" + string(s.getName()) + "',0,'');";
        char *err = 0;
        sqlite3_exec(db, sql.c_str(), 0, 0, &err);
    }
    void displayBooks()
    {
        cout << "\n\nBook List\n";
        char *err = 0;
        sqlite3_exec(db, "SELECT * FROM Books;", displayCallback, 0, &err);
        getch();
    }
    void displayStudents()
    {
        cout << "\n\nStudent List\n";
        char *err = 0;
        sqlite3_exec(db, "SELECT * FROM Students;", displayCallback, 0, &err);
        getch();
    }
    void modifyBook()
    {
        string id;
        cout << "\n\nEnter Book ID to Modify: ";
        cin >> id;
        cin.ignore();
        string name, auth;
        cout << "New Book Name: ";
        getline(cin, name);
        cout << "New Author: ";
        getline(cin, auth);
        string sql = "UPDATE Books SET name='" + name + "', author='" + auth + "' WHERE book_id='" + id + "';";
        char *err = 0;
        sqlite3_exec(db, sql.c_str(), 0, 0, &err);
    }
    void modifyStudent()
    {
        string id;
        cout << "\n\nEnter Student ID to Modify: ";
        cin >> id;
        cin.ignore();
        string name;
        cout << "New Student Name: ";
        getline(cin, name);
        string sql = "UPDATE Students SET name='" + name + "' WHERE student_id='" + id + "';";
        char *err = 0;
        sqlite3_exec(db, sql.c_str(), 0, 0, &err);
    }
    void deleteBook()
    {
        string id;
        cout << "\n\nEnter Book ID to Delete: ";
        cin >> id;
        string sql = "DELETE FROM Books WHERE book_id='" + id + "';";
        char *err = 0;
        sqlite3_exec(db, sql.c_str(), 0, 0, &err);
    }
    void deleteStudent()
    {
        string id;
        cout << "\n\nEnter Student ID to Delete: ";
        cin >> id;
        string sql = "DELETE FROM Students WHERE student_id='" + id + "';";
        char *err = 0;
        sqlite3_exec(db, sql.c_str(), 0, 0, &err);
    }
    void issueBook()
    {
        string sid, bid;
        cout << "\nEnter Student ID: ";
        cin >> sid;
        cout << "Enter Book ID: ";
        cin >> bid;

        char *err = 0;
        int rc;

        rc = sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, &err);
        if (rc != SQLITE_OK)
        {
            cout << "Transaction Error\n";
            return;
        }

        string sqlCheckStudent = "SELECT token FROM Students WHERE student_id='" + sid + "';";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(db, sqlCheckStudent.c_str(), -1, &stmt, 0);
        int token = -1;
        if (sqlite3_step(stmt) == SQLITE_ROW)
            token = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);

        string sqlCheckBook = "SELECT book_id FROM Books WHERE book_id='" + bid + "';";
        sqlite3_prepare_v2(db, sqlCheckBook.c_str(), -1, &stmt, 0);
        bool bookExists = false;
        if (sqlite3_step(stmt) == SQLITE_ROW)
            bookExists = true;
        sqlite3_finalize(stmt);

        if (token == -1 || !bookExists)
        {
            sqlite3_exec(db, "ROLLBACK;", 0, 0, &err);
            cout << "Student or Book not found. Transaction aborted.\n";
            getch();
            return;
        }
        if (token == 1)
        {
            sqlite3_exec(db, "ROLLBACK;", 0, 0, &err);
            cout << "Student has not returned previous book.\n";
            getch();
            return;
        }

        string sql = "UPDATE Students SET token=1, issued_book_id='" + bid + "' WHERE student_id='" + sid + "';";
        rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err);
        if (rc != SQLITE_OK)
        {
            sqlite3_exec(db, "ROLLBACK;", 0, 0, &err);
            cout << "Error issuing book. Transaction aborted.\n";
            getch();
            return;
        }

        sqlite3_exec(db, "COMMIT;", 0, 0, &err);
        cout << "Book issued successfully.\n";
        getch();
    }

    void depositBook()
    {
        string sid;
        cout << "\nEnter Student ID: ";
        cin >> sid;

        char *err = 0;
        sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, &err);

        string sqlCheck = "SELECT token,issued_book_id FROM Students WHERE student_id='" + sid + "';";
        sqlite3_stmt *stmt;
        sqlite3_prepare_v2(db, sqlCheck.c_str(), -1, &stmt, 0);
        int token = -1;
        string bid = "";
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            token = sqlite3_column_int(stmt, 0);
            bid = (const char *)sqlite3_column_text(stmt, 1);
        }
        sqlite3_finalize(stmt);

        if (token == -1)
        {
            sqlite3_exec(db, "ROLLBACK;", 0, 0, &err);
            cout << "Student not found.\n";
            getch();
            return;
        }
        if (token == 0)
        {
            sqlite3_exec(db, "ROLLBACK;", 0, 0, &err);
            cout << "No book issued.\n";
            getch();
            return;
        }

        int days;
        cout << "Returned in how many days?: ";
        cin >> days;
        if (days > 15)
            cout << "Fine: Rs " << (days - 15) * 1 << "\n";

        string sql = "UPDATE Students SET token=0, issued_book_id='' WHERE student_id='" + sid + "';";
        int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err);
        if (rc != SQLITE_OK)
        {
            sqlite3_exec(db, "ROLLBACK;", 0, 0, &err);
            cout << "Error depositing book. Transaction aborted.\n";
            getch();
            return;
        }

        sqlite3_exec(db, "COMMIT;", 0, 0, &err);
        cout << "Book deposited successfully.\n";
        getch();
    }
};

void adminMenu(FileManager &fm)
{
    int ch2;
    cout << "\n\nADMIN MENU\n1.CREATE STUDENT\n2.DISPLAY STUDENTS\n3.MODIFY STUDENT\n4.DELETE STUDENT\n5.CREATE BOOK\n6.DISPLAY BOOKS\n7.MODIFY BOOK\n8.DELETE BOOK\n9.BACK\nChoice: ";
    cin >> ch2;
    switch (ch2)
    {
    case 1:
        fm.addStudent();
        break;
    case 2:
        fm.displayStudents();
        break;
    case 3:
        fm.modifyStudent();
        break;
    case 4:
        fm.deleteStudent();
        break;
    case 5:
        fm.addBook();
        break;
    case 6:
        fm.displayBooks();
        break;
    case 7:
        fm.modifyBook();
        break;
    case 8:
        fm.deleteBook();
        break;
    case 9:
        return;
    }
    adminMenu(fm);
}

int main()
{
    char ch;
    FileManager fm;
    do
    {
        system("cls");
        cout << "\n\nMAIN MENU\n1.BOOK ISSUE\n2.BOOK DEPOSIT\n3.ADMIN MENU\n4.EXIT\nChoice: ";
        ch = getche();
        switch (ch)
        {
        case '1':
            fm.issueBook();
            break;
        case '2':
            fm.depositBook();
            break;
        case '3':
            adminMenu(fm);
            break;
        case '4':
            exit(0);
        }
    } while (ch != '4');
    return 0;
}