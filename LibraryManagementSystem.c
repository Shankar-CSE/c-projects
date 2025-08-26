#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define constants
#define FILENAME "library_books.dat"
#define MAX_TITLE_LEN 100
#define MAX_AUTHOR_LEN 100

// Structure to represent a book
struct Book {
    int id;
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
    int is_available; // 1 for available, 0 for borrowed
};

// Function Prototypes
void add_book();
void display_all_books();
void search_book();
void update_book_status(int book_id, int new_status, const char* action_str);
void borrow_book();
void return_book();
int get_next_book_id();
void clear_input_buffer();

int main() {
    int choice;

    while (1) {
        printf("\n===== Library Management System =====\n");
        printf("1. Add New Book\n");
        printf("2. Display All Books\n");
        printf("3. Search for a Book\n");
        printf("4. Borrow a Book\n");
        printf("5. Return a Book\n");
        printf("6. Exit\n");
        printf("=====================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\nError: Invalid input. Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1: add_book(); break;
            case 2: display_all_books(); break;
            case 3: search_book(); break;
            case 4: borrow_book(); break;
            case 5: return_book(); break;
            case 6:
                printf("\nExiting the system. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice. Please enter a number between 1 and 6.\n");
        }
    }

    return 0;
}

/**
 * @brief Clears the standard input buffer.
 */
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Finds the highest existing book ID and returns the next sequential ID.
 * @return The next available unique book ID.
 */
int get_next_book_id() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        return 1; // If file doesn't exist, start with ID 1
    }

    struct Book book;
    int max_id = 0;
    while (fread(&book, sizeof(struct Book), 1, fp) == 1) {
        if (book.id > max_id) {
            max_id = book.id;
        }
    }
    fclose(fp);
    return max_id + 1;
}

/**
 * @brief Adds a new book to the library file.
 */
void add_book() {
    struct Book new_book;
    new_book.id = get_next_book_id();
    new_book.is_available = 1; // New books are available by default

    printf("\n--- Add New Book (ID: %d) ---\n", new_book.id);
    printf("Enter Title: ");
    fgets(new_book.title, MAX_TITLE_LEN, stdin);
    new_book.title[strcspn(new_book.title, "\n")] = 0;

    printf("Enter Author: ");
    fgets(new_book.author, MAX_AUTHOR_LEN, stdin);
    new_book.author[strcspn(new_book.author, "\n")] = 0;

    FILE *fp = fopen(FILENAME, "ab");
    if (fp == NULL) {
        perror("Error: Could not open file for writing");
        return;
    }

    fwrite(&new_book, sizeof(struct Book), 1, fp);
    fclose(fp);

    printf("\nBook '%s' added successfully!\n", new_book.title);
}

/**
 * @brief Displays all books in the library.
 */
void display_all_books() {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("\nNo books in the library yet. Add one!\n");
        return;
    }

    printf("\n--- All Library Books ---\n");
    printf("%-5s %-40s %-30s %-15s\n", "ID", "Title", "Author", "Status");
    printf("----------------------------------------------------------------------------------------\n");

    struct Book book;
    int count = 0;
    while (fread(&book, sizeof(struct Book), 1, fp) == 1) {
        printf("%-5d %-40s %-30s %-15s\n",
               book.id, book.title, book.author,
               (book.is_available ? "Available" : "Borrowed"));
        count++;
    }

    if (count == 0) {
        printf("No books found.\n");
    }
    printf("----------------------------------------------------------------------------------------\n");
    fclose(fp);
}

/**
 * @brief Searches for books by title or author.
 */
void search_book() {
    char query[MAX_TITLE_LEN];
    printf("\nEnter search term (title or author): ");
    fgets(query, MAX_TITLE_LEN, stdin);
    query[strcspn(query, "\n")] = 0;

    // Convert query to lowercase for case-insensitive search
    for(int i = 0; query[i]; i++){
        query[i] = tolower(query[i]);
    }

    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("\nNo books in the library to search.\n");
        return;
    }

    printf("\n--- Search Results ---\n");
    printf("%-5s %-40s %-30s %-15s\n", "ID", "Title", "Author", "Status");
    printf("----------------------------------------------------------------------------------------\n");

    struct Book book;
    int found = 0;
    char lower_title[MAX_TITLE_LEN];
    char lower_author[MAX_AUTHOR_LEN];

    while (fread(&book, sizeof(struct Book), 1, fp) == 1) {
        // Copy and convert title to lowercase
        strcpy(lower_title, book.title);
        for(int i = 0; lower_title[i]; i++) lower_title[i] = tolower(lower_title[i]);

        // Copy and convert author to lowercase
        strcpy(lower_author, book.author);
        for(int i = 0; lower_author[i]; i++) lower_author[i] = tolower(lower_author[i]);

        if (strstr(lower_title, query) || strstr(lower_author, query)) {
            printf("%-5d %-40s %-30s %-15s\n",
                   book.id, book.title, book.author,
                   (book.is_available ? "Available" : "Borrowed"));
            found = 1;
        }
    }

    if (!found) {
        printf("No books found matching your search term.\n");
    }
    printf("----------------------------------------------------------------------------------------\n");
    fclose(fp);
}

/**
 * @brief Core logic to find a book by ID and update its status.
 * @param book_id The ID of the book to update.
 * @param new_status The new availability status (1 for available, 0 for borrowed).
 * @param action_str A string describing the action (e.g., "borrow", "return").
 */
void update_book_status(int book_id, int new_status, const char* action_str) {
    FILE *fp = fopen(FILENAME, "rb+");
    if (fp == NULL) {
        printf("\nError: Could not open library file.\n");
        return;
    }

    struct Book book;
    int found = 0;
    while (fread(&book, sizeof(struct Book), 1, fp) == 1) {
        if (book.id == book_id) {
            // Check if the action is valid
            if (book.is_available == new_status) {
                printf("\nError: Book '%s' is already %s.\n", book.title, (new_status ? "available" : "borrowed"));
            } else {
                book.is_available = new_status;
                fseek(fp, -sizeof(struct Book), SEEK_CUR);
                fwrite(&book, sizeof(struct Book), 1, fp);
                printf("\nBook '%s' successfully %s.\n", book.title, action_str);
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\nError: Book with ID %d not found.\n", book_id);
    }

    fclose(fp);
}

/**
 * @brief Handles the user flow for borrowing a book.
 */
void borrow_book() {
    int book_id;
    printf("\nEnter the ID of the book to borrow: ");
    if (scanf("%d", &book_id) != 1) {
        printf("Invalid ID format.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();
    update_book_status(book_id, 0, "borrowed");
}

/**
 * @brief Handles the user flow for returning a book.
 */
void return_book() {
    int book_id;
    printf("\nEnter the ID of the book to return: ");
    if (scanf("%d", &book_id) != 1) {
        printf("Invalid ID format.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();
    update_book_status(book_id, 1, "returned");
}

