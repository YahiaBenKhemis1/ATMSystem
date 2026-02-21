![Main Menu](MainMenuScreenPhoto.png)

# 🏧 ATM Management System — C++

## Overview

The ATM Management System is a console-based application developed in C++ that simulates essential banking operations.  
The project is implemented using a procedural (functional-style) programming approach and focuses on clarity, reliability, and structured logic.

It was developed as a learning project to strengthen my understanding of core C++ concepts, file handling, and system design.

---

## Key Features

- User authentication using account number and PIN
- Quick withdrawal with predefined amounts
- Custom withdrawal option
- Deposit functionality
- Balance inquiry
- Password update
- Secure logout
- Input validation and error handling


---

## Technical Stack & Approach

- Language: C++
- Programming Paradigm: Procedural / Functional Style
- Data Structures: `struct`, `enum`
- Compiler: MSVC / GCC
- IDE: Visual Studio
- Data Storage: Text files
- Version Control: Git & GitHub

The project does not rely on advanced data structures or object-oriented design.  
Instead, it focuses on well-structured functions, clear data organization, and readable logic.

---

## Data Management

All client data is stored in a text file (`Clients.txt`) and managed using `struct`-based records.  
The application reads, validates, updates, and saves this data during each operation.

---

## Security Considerations

- PIN-based authentication
- Balance verification before withdrawals
- Controlled access to sensitive operations
- Input validation to prevent incorrect inputs

Note: This project is designed for educational purposes and does not implement advanced cryptographic security.


