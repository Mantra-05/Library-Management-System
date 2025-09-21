# Library Management System

A **console-based Library Management System** developed in **C++** using **OOP principles** and **SQLite** for persistent database storage. The system manages books and students, supports book issue and deposit, calculates fines, and ensures atomic operations using **transactions**.

---

## Features

- **Book Management**
  - Add, modify, delete, and display books.
- **Student Management**
  - Add, modify, delete, and display student records.
- **Book Issue & Deposit**
  - Issue books to students with token tracking.
  - Deposit books with late fine calculation.
  - Atomic operations using SQLite **transactions**.
- **Database Integration**
  - All data is stored in a **SQLite database** (`library.db`).
  - Supports CRUD operations with persistent storage.
- **Console Menu**
  - Interactive menu for **administrators** and **students**.
- **Extensible Design**
  - Built with OOP: **inheritance, polymorphism, encapsulation, abstraction**.
  - Easily extendable for features like search, multiple book issues, and reports.

---

## Tech Stack

- **C++** (Object-Oriented Programming)
- **SQLite** (Database and Transactions)
- **VS Code + MinGW** (Development environment and compiler)

---

## To Compile & Run

```bash
gcc -c sqlite3.c -o sqlite3.o
g++ main.cpp sqlite3.o -o library.exe -std=c++11
