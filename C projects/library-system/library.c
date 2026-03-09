#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char title[100];
    char author[50];
    int year;
    int available;
    char borrowed_by[50];
} Book;

Book library[100];
int book_count = 0;
int next_book_id = 1;

#define BACKEND_FILE "books.json"
#define FRONTEND_FILE "../frontend/data/books.json"

void printBanner() {
    printf("\n");
    printf("=====================================\n");
    printf("  LIBRARY MANAGEMENT SYSTEM\n");
    printf("  Version 1.0\n");
    printf("=====================================\n");
    printf("\n");
}

void printMenu() {
    printf("\n==== MAIN MENU ====\n");
    printf("1 - Add Book\n");
    printf("2 - List All Books\n");
    printf("3 - Search Book\n");
    printf("4 - Borrow Book\n");
    printf("5 - Return Book\n");
    printf("6 - Delete Book\n");
    printf("0 - Exit\n");
    printf("\nEnter choice: ");
}

void printTableHeader() {
    printf("\nID | Book Title           | Author         | Year | Status\n");
    printf("---+----------------------+----------------+------+-----------\n");
}

void printBook(Book b) {
    char status[15];
    if (b.available == 1) {
        strcpy(status, "Available");
    } else {
        strcpy(status, "Borrowed");
    }
    printf("%2d | %-20s | %-14s | %4d | %s\n", b.id, b.title, b.author, b.year, status);
}

int getIntInput(void) {
    char buffer[100];
    char *endptr = NULL;
    long value;

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }

    value = strtol(buffer, &endptr, 10);

    if (endptr == buffer || (*endptr != '\n' && *endptr != '\0')) {
        return -1;
    }

    if (value < INT_MIN || value > INT_MAX) {
        return -1;
    }

    return (int)value;
}

void saveBooks() {
    FILE *backendFile = fopen(BACKEND_FILE, "w");
    if (backendFile == NULL) {
        printf("ERROR: Cannot write backend file!\n");
        return;
    }

    fprintf(backendFile, "[\n");

    for (int i = 0; i < book_count; i++) {
        fprintf(backendFile, "  {\n");
        fprintf(backendFile, "    \"id\": %d,\n", library[i].id);
        fprintf(backendFile, "    \"title\": \"%s\",\n", library[i].title);
        fprintf(backendFile, "    \"author\": \"%s\",\n", library[i].author);
        fprintf(backendFile, "    \"year\": %d,\n", library[i].year);
        fprintf(backendFile, "    \"available\": %d,\n", library[i].available);
        fprintf(backendFile, "    \"borrowed_by\": \"%s\"\n", library[i].borrowed_by);

        if (i < book_count - 1) {
            fprintf(backendFile, "  },\n");
        } else {
            fprintf(backendFile, "  }\n");
        }
    }

    fprintf(backendFile, "]\n");
    fclose(backendFile);

    FILE *frontendFile = fopen(FRONTEND_FILE, "w");
    if (frontendFile == NULL) {
        printf("WARNING: Cannot write frontend file!\n");
        return;
    }

    fprintf(frontendFile, "[\n");

    for (int i = 0; i < book_count; i++) {
        fprintf(frontendFile, "  {\n");
        fprintf(frontendFile, "    \"id\": %d,\n", library[i].id);
        fprintf(frontendFile, "    \"title\": \"%s\",\n", library[i].title);
        fprintf(frontendFile, "    \"author\": \"%s\",\n", library[i].author);
        fprintf(frontendFile, "    \"year\": %d,\n", library[i].year);
        fprintf(frontendFile, "    \"available\": %d,\n", library[i].available);
        fprintf(frontendFile, "    \"borrowed_by\": \"%s\"\n", library[i].borrowed_by);

        if (i < book_count - 1) {
            fprintf(frontendFile, "  },\n");
        } else {
            fprintf(frontendFile, "  }\n");
        }
    }

    fprintf(frontendFile, "]\n");
    fclose(frontendFile);
}

void loadBooks() {
    FILE *file = fopen(BACKEND_FILE, "r");
    if (file == NULL) {
        printf("INFO: No saved data found.\n");
        return;
    }

    book_count = 0;
    next_book_id = 1;

    char line[200];
    int currentId = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, "\"id\":") != NULL) {
            sscanf(line, "    \"id\": %d,", &currentId);
            if (currentId >= next_book_id) {
                next_book_id = currentId + 1;
            }
            library[book_count].id = currentId;
        }

        if (strstr(line, "\"title\":") != NULL) {
            char *start = strchr(line, '"');
            start++;
            start = strchr(start, '"');
            start++;
            char *end = strchr(start, '"');
            int len = end - start;
            strncpy(library[book_count].title, start, len);
            library[book_count].title[len] = '\0';
        }

        if (strstr(line, "\"author\":") != NULL) {
            char *start = strchr(line, '"');
            start++;
            start = strchr(start, '"');
            start++;
            char *end = strchr(start, '"');
            int len = end - start;
            strncpy(library[book_count].author, start, len);
            library[book_count].author[len] = '\0';
        }

        if (strstr(line, "\"year\":") != NULL) {
            sscanf(line, "    \"year\": %d,", &library[book_count].year);
        }

        if (strstr(line, "\"available\":") != NULL) {
            sscanf(line, "    \"available\": %d,", &library[book_count].available);
        }

        if (strstr(line, "\"borrowed_by\":") != NULL) {
            char *start = strchr(line, '"');
            start++;
            start = strchr(start, '"');
            start++;
            char *end = strchr(start, '"');
            if (end != NULL) {
                int len = end - start;
                strncpy(library[book_count].borrowed_by, start, len);
                library[book_count].borrowed_by[len] = '\0';
            } else {
                strcpy(library[book_count].borrowed_by, "");
            }
            book_count++;
        }
    }

    fclose(file);
    printf("INFO: Loaded %d books.\n", book_count);
}

void addBook() {
    if (book_count >= 100) {
        printf("\nERROR: Library full!\n");
        return;
    }

    Book newBook;
    printf("\n--- ADD NEW BOOK ---\n");

    newBook.id = next_book_id;
    next_book_id++;

    printf("Book Title: ");
    fgets(newBook.title, 100, stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0;

    printf("Author: ");
    fgets(newBook.author, 50, stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0;

    printf("Publication Year: ");
    int year = getIntInput();

    if (year < 0) {
        printf("ERROR: Invalid year!\n");
        return;
    }

    newBook.year = year;
    newBook.available = 1;
    strcpy(newBook.borrowed_by, "");

    library[book_count] = newBook;
    book_count++;

    saveBooks();

    printf("\nSUCCESS: Book added! (ID: %d)\n", newBook.id);
}

void listBooks() {
    if (book_count == 0) {
        printf("\nNo books in library!\n");
        return;
    }

    printTableHeader();

    for (int i = 0; i < book_count; i++) {
        printBook(library[i]);
    }

    printf("\nTotal: %d\n", book_count);
}

void searchBook() {
    if (book_count == 0) {
        printf("\nNo books!\n");
        return;
    }

    char searchTerm[100];
    printf("\nSearch title: ");
    fgets(searchTerm, 100, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0;

    printTableHeader();

    int found = 0;
    for (int i = 0; i < book_count; i++) {
        if (strstr(library[i].title, searchTerm) != NULL) {
            printBook(library[i]);
            found = 1;
        }
    }

    if (found == 0) {
        printf("\nNo books found!\n");
    }
}

void borrowBook() {
    if (book_count == 0) {
        printf("\nNo books!\n");
        return;
    }

    char memberName[50];
    printf("\n--- BORROW BOOK ---\n");
    printf("Book ID: ");
    int bookId = getIntInput();

    if (bookId < 0) {
        printf("ERROR: Invalid ID!\n");
        return;
    }

    for (int i = 0; i < book_count; i++) {
        if (library[i].id == bookId) {
            if (library[i].available == 0) {
                printf("\nERROR: Already borrowed by %s!\n", library[i].borrowed_by);
                return;
            }

            printf("Member Name: ");
            fgets(memberName, 50, stdin);
            memberName[strcspn(memberName, "\n")] = 0;

            library[i].available = 0;
            strcpy(library[i].borrowed_by, memberName);

            saveBooks();

            printf("\nSUCCESS: Book borrowed!\n");
            return;
        }
    }

    printf("\nERROR: ID not found!\n");
}

void returnBook() {
    if (book_count == 0) {
        printf("\nNo books!\n");
        return;
    }

    printf("\n--- RETURN BOOK ---\n");
    printf("Book ID: ");
    int bookId = getIntInput();

    if (bookId < 0) {
        printf("ERROR: Invalid ID!\n");
        return;
    }

    for (int i = 0; i < book_count; i++) {
        if (library[i].id == bookId) {
            if (library[i].available == 1) {
                printf("\nERROR: Not borrowed!\n");
                return;
            }

            printf("\nSUCCESS: Returned!\n");
            printf("(Was borrowed by: %s)\n", library[i].borrowed_by);

            library[i].available = 1;
            strcpy(library[i].borrowed_by, "");

            saveBooks();
            return;
        }
    }

    printf("\nERROR: ID not found!\n");
}

void deleteBook() {
    if (book_count == 0) {
        printf("\nNo books!\n");
        return;
    }

    printf("\n--- DELETE BOOK ---\n");
    printf("Book ID: ");
    int bookId = getIntInput();

    if (bookId < 0) {
        printf("ERROR: Invalid ID!\n");
        return;
    }

    for (int i = 0; i < book_count; i++) {
        if (library[i].id == bookId) {
            for (int j = i; j < book_count - 1; j++) {
                library[j] = library[j + 1];
            }
            book_count--;
            saveBooks();
            printf("\nSUCCESS: Deleted!\n");
            return;
        }
    }

    printf("\nERROR: ID not found!\n");
}

int main() {
    int choice;

    printBanner();
    loadBooks();

    while (1) {
        printMenu();
        choice = getIntInput();

        if (choice < 0) {
            printf("\nERROR: Invalid input!\n");
            continue;
        }

        if (choice == 1) {
            addBook();
        }
        else if (choice == 2) {
            listBooks();
        }
        else if (choice == 3) {
            searchBook();
        }
        else if (choice == 4) {
            borrowBook();
        }
        else if (choice == 5) {
            returnBook();
        }
        else if (choice == 6) {
            deleteBook();
        }
        else if (choice == 0) {
            printf("\nGoodbye!\n\n");
            break;
        }
        else {
            printf("\nERROR: Invalid choice!\n");
        }
    }

    return 0;
}