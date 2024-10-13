# Library Management System - README

## Overview
This project is a simple command-line based Library Management System. Users can register, login, enter the library, add books, view books, get details of a specific book, and delete books. The system interacts with a server using HTTP requests and handles responses accordingly.

## Features
1. **Register**: Create a new user account.
2. **Login**: Log into an existing user account.
3. **Enter Library**: Access the library's book collection.
4. **Add Book**: Add a new book to the library.
5. **Get Books**: Retrieve a list of all books in the library.
6. **Get Book**: Retrieve details of a specific book by its ID.
7. **Delete Book**: Remove a specific book by its ID.
8. **Logout**: Logout from the current session.
9. **Exit**: Exit the program.

## Prerequisites
- C compiler (e.g., GCC)
- Make sure the `helpers.h` header file is available and includes the required functions for HTTP requests and responses.

## Compilation
Compile the program using the following command:
```bash
gcc -o client client.c requests.c helpers.c buffer.c -Wall
```
## Usage
Run the compiled program:
```bash
./client
```
Follow the on-screen prompts to interact with the system.

## Detailed Command Descriptions

### Register
Register a new user account:
```bash
Command: register
username=<username>
password=<password>
```
Server Response Handling:

- Success: Proceeds silently.
- Error: Displays the error message from the server.

### Login
Login to an existing user account:
```bash
Command: login
username=<username>
password=<password>
```
Server Response Handling:

- Success: Extracts and stores the authentication token.
- Error: Displays the error message from the server.


### Enter Library
Access the library:
```bash
Command: enter_library
```
Server Response Handling:

- Success: Displays a success message.
- Error: Displays the error message from the server.

### Add Book
Add a new book to the library:
```bash
Command: add_book
Title=<title>
Author=<author>
Genre=<genre>
Page Count=<page_count>
Publisher=<publisher>
```
Server Response Handling:

- Success: Displays a success message.
- Error: Displays the error message from the server.

### Get Books
Retrieve a list of all books:
```bash
Command: get_books
```
Server Response Handling:

- Success: Displays the list of books.
- Error: Displays the error message from the server.


### Get Book
Retrieve details of a specific book by its ID:
```bash
Command: get_book
Book Id=<id>
```
Server Response Handling:

- Success: Displays book details.
- Error: Displays the error message from the server.

### Get Book
Remove a specific book by its ID:
```bash
Command: delete_book
Book Id=<id>
```
Server Response Handling:

- Success: Displays a success message.
- Error: Displays the error message from the server.

### Delete Book
Remove a specific book by its ID:
```bash
Command: delete_book
Book Id=<id>
```
Server Response Handling:

- Success: Displays a success message.
- Error: Displays the error message from the server.

### Logout
Logout from the current session:
```bash
logout
```
Clears session information.

### Exit

Exit the program:
```bash
exit
```

## Code Structure
The main function handles user input and commands, maintaining the session state. Helper functions facilitate HTTP communication and server response parsing.
## Conclusion
This project demonstrates basic client-server interaction via HTTP in a command-line application. It includes functionalities for user registration, login, library access, and book management, with appropriate error handling based on server responses.


