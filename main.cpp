#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <conio.h>
#include <stdlib.h>

using namespace std;

class LibraryEntity
{
protected:
    char id[6];

public:
    virtual void create() = 0;
    virtual void show() = 0;
    virtual void report() const = 0;

    const char *getID() const
    {
        return id;
    }

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

    void modify()
    {
        cout << "\nBook Number: " << id;
        cout << "\nModify Book Name: ";
        cin.ignore();
        cin.getline(bname, 50);
        cout << "Modify Author's Name: ";
        cin.getline(aname, 20);
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
        {
            cout << "\nBook Number: " << stbno;
        }
    }

    void report() const override
    {
        cout << "\t" << id << setw(20) << name << setw(10) << token << endl;
    }

    void modify()
    {
        cout << "\nAdmission No.: " << id;
        cout << "\nModify Student Name: ";
        cin.ignore();
        cin.getline(name, 20);
    }

    const char *getStbno() const { return stbno; }
    int getToken() const { return token; }
    void addToken() { token = 1; }
    void resetToken() { token = 0; }
    void setStbno(const char *t) { strcpy(stbno, t); }
};

class FileManager
{
public:
    template <typename T>
    void addRecord(const string &filename)
    {
        ofstream outFile(filename, ios::binary | ios::app);
        if (!outFile)
        {
            cerr << "Cannot open file for writing!" << endl;
            return;
        }
        T record;
        record.create();
        outFile.write(reinterpret_cast<const char *>(&record), sizeof(T));
        outFile.close();
    }

    template <typename T>
    void displayAllRecords(const string &filename, const string &header1, const string &header2, const string &header3)
    {
        ifstream inFile(filename, ios::binary);
        if (!inFile)
        {
            cout << "File could not be opened." << endl;
            getch();
            return;
        }
        cout << "\n\n\t\t" << (filename == "book.dat" ? "Book" : "Student") << " List\n\n";
        cout << "==================================================================\n";
        cout << "\t" << header1 << setw(20) << header2 << setw(25) << header3 << "\n";
        cout << "==================================================================\n";
        T record;
        while (inFile.read(reinterpret_cast<char *>(&record), sizeof(T)))
        {
            record.report();
        }
        inFile.close();
        getch();
    }

    template <typename T>
    bool findRecord(const string &filename, const char *id, T &record)
    {
        ifstream inFile(filename, ios::binary);
        if (!inFile)
            return false;
        bool found = false;
        while (inFile.read(reinterpret_cast<char *>(&record), sizeof(T)))
        {
            if (stricmp(record.getID(), id) == 0)
            {
                found = true;
                break;
            }
        }
        inFile.close();
        return found;
    }

    template <typename T>
    void modifyRecord(const string &filename)
    {
        char id[6];
        cout << "\n\nEnter the ID to Modify: ";
        cin >> id;

        fstream file(filename, ios::binary | ios::in | ios::out);
        if (!file)
        {
            cerr << "File could not be opened." << endl;
            return;
        }

        bool found = false;
        T record;
        while (file.read(reinterpret_cast<char *>(&record), sizeof(T)))
        {
            if (stricmp(record.getID(), id) == 0)
            {
                record.show();
                cout << "\nEnter the new details:\n";
                record.modify();

                long pos = -1 * static_cast<long>(sizeof(T));
                file.seekp(pos, ios::cur);
                file.write(reinterpret_cast<const char *>(&record), sizeof(T));

                cout << "\n\nRecord Updated.";
                found = true;
                break;
            }
        }
        file.close();
        if (!found)
            cout << "\n\nRecord Not Found.";
        getch();
    }

    template <typename T>
    void deleteRecord(const string &filename)
    {
        char id[6];
        cout << "\n\nEnter the ID to Delete: ";
        cin >> id;

        ifstream inFile(filename, ios::binary);
        if (!inFile)
        {
            cerr << "File error." << endl;
            return;
        }

        ofstream outFile("temp.dat", ios::binary);

        T record;
        bool found = false;
        while (inFile.read(reinterpret_cast<char *>(&record), sizeof(T)))
        {
            if (stricmp(record.getID(), id) == 0)
            {
                found = true;
            }
            else
            {
                outFile.write(reinterpret_cast<const char *>(&record), sizeof(T));
            }
        }

        inFile.close();
        outFile.close();

        remove(filename.c_str());
        rename("temp.dat", filename.c_str());

        if (found)
            cout << "\n\n\tRecord Deleted...";
        else
            cout << "\n\nRecord not Found";
        getch();
    }

    void issueBook();
    void depositBook();
};

void FileManager::issueBook()
{
    char student_id[6], book_id[6];
    cout << "\n\nBOOK ISSUE...\n";
    cout << "\tEnter Admission no.: ";
    cin >> student_id;

    Student student;
    if (!findRecord("student.dat", student_id, student))
    {
        cout << "Student Record Not Found...";
        getch();
        return;
    }

    if (student.getToken() == 1)
    {
        cout << "You Have Not Returned The Last Book.";
        getch();
        return;
    }

    cout << "\n\tEnter The Book No.: ";
    cin >> book_id;

    Book book;
    if (!findRecord("book.dat", book_id, book))
    {
        cout << "Book Does Not Exist.";
        getch();
        return;
    }

    student.addToken();
    student.setStbno(book.getID());

    fstream studentFile("student.dat", ios::binary | ios::in | ios::out);
    Student tempStudent;
    while (studentFile.read(reinterpret_cast<char *>(&tempStudent), sizeof(Student)))
    {
        if (stricmp(tempStudent.getID(), student_id) == 0)
        {
            long pos = -1 * static_cast<long>(sizeof(Student));
            studentFile.seekp(pos, ios::cur);
            studentFile.write(reinterpret_cast<const char *>(&student), sizeof(Student));
            cout << "\n\n\tBook Issued Successfully\n";
            cout << "Please return within 15 days to avoid a fine.\n";
            break;
        }
    }
    studentFile.close();
    getch();
}

void FileManager::depositBook()
{
    char student_id[6];
    cout << "\n\nBOOK DEPOSIT...\n";
    cout << "\tEnter Admission no. Of Student: ";
    cin >> student_id;

    Student student;
    if (!findRecord("student.dat", student_id, student))
    {
        cout << "Student Record Not Found...";
        getch();
        return;
    }

    if (student.getToken() == 0)
    {
        cout << "No Book Has Been Issued To This Student.";
        getch();
        return;
    }

    Book book;
    if (!findRecord("book.dat", student.getStbno(), book))
    {
        cout << "Issued Book Record Not Found in Library (This shouldn't happen!).";
        getch();
        return;
    }

    book.show();
    int days;
    cout << "\n\nBook returned in how many days?: ";
    cin >> days;
    if (days > 15)
    {
        int fine = (days - 15) * 1;
        cout << "\n\nFine is: Rs " << fine;
    }

    student.resetToken();
    student.setStbno("");

    fstream studentFile("student.dat", ios::binary | ios::in | ios::out);
    Student tempStudent;
    while (studentFile.read(reinterpret_cast<char *>(&tempStudent), sizeof(Student)))
    {
        if (stricmp(tempStudent.getID(), student_id) == 0)
        {
            long pos = -1 * static_cast<long>(sizeof(Student));
            studentFile.seekp(pos, ios::cur);
            studentFile.write(reinterpret_cast<const char *>(&student), sizeof(Student));
            cout << "\n\n\tBook Deposited Successfully.";
            break;
        }
    }
    studentFile.close();
    getch();
}

void adminMenu(FileManager &fm)
{
    int ch2;
    cout << "\n\n\n\tADMINISTRATOR MENU";
    cout << "\n\n\t1. CREATE STUDENT RECORD";
    cout << "\n\t2. DISPLAY ALL STUDENT RECORDS";
    cout << "\n\t3. MODIFY STUDENT RECORD";
    cout << "\n\t4. DELETE STUDENT RECORD";
    cout << "\n\t5. CREATE BOOK";
    cout << "\n\t6. DISPLAY ALL BOOKS";
    cout << "\n\t7. MODIFY BOOK RECORD";
    cout << "\n\t8. DELETE BOOK RECORD";
    cout << "\n\t9. BACK TO MAIN MENU";
    cout << "\n\n\tPlease Enter Your Choice (1-9): ";
    cin >> ch2;
    switch (ch2)
    {
    case 1:
        fm.addRecord<Student>("student.dat");
        break;
    case 2:
        fm.displayAllRecords<Student>("student.dat", "Admission No.", "Name", "Book Issued");
        break;
    case 3:
        fm.modifyRecord<Student>("student.dat");
        break;
    case 4:
        fm.deleteRecord<Student>("student.dat");
        break;
    case 5:
        fm.addRecord<Book>("book.dat");
        break;
    case 6:
        fm.displayAllRecords<Book>("book.dat", "Book No.", "Book Name", "Author");
        break;
    case 7:
        fm.modifyRecord<Book>("book.dat");
        break;
    case 8:
        fm.deleteRecord<Book>("book.dat");
        break;
    case 9:
        return;
    default:
        cout << "Invalid choice";
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
        cout << "\n\n\n\t MAIN MENU";
        cout << "\n\n\t1. BOOK ISSUE";
        cout << "\n\n\t2. BOOK DEPOSIT";
        cout << "\n\n\t3. ADMINISTRATOR MENU";
        cout << "\n\n\t4. EXIT";
        cout << "\n\n\tPlease Select Your Option (1-4): ";
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
        default:
            cout << "\a"; 
        }
    } while (ch != '4');

    return 0;
}