#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "accounts.dat"
#define MAX_NAME_LEN 100
#define MAX_PASS_LEN 50

struct Account {
    int acc_no;
    char name[MAX_NAME_LEN];
    char password_hash[MAX_PASS_LEN];
    double balance;
};

// --- Function Prototypes ---
void create_account();
void login();
void main_menu();
void logged_in_menu(int acc_no);
void deposit(int acc_no);
void withdraw(int acc_no);
void check_balance(int acc_no);
int get_next_acc_no();
void hash_password(const char *password, char *hash);
void clear_input_buffer();

// --- Main Function ---
int main() {
    main_menu();
    return 0;
}

void main_menu() {
    int choice;
    while (1) {
        printf("\n===== Bank Management System =====\n");
        printf("1. Create New Account\n");
        printf("2. Login to Existing Account\n");
        printf("3. Exit\n");
        printf("==================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\nError: Invalid input.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1: create_account(); break;
            case 2: login(); break;
            case 3: exit(0);
            default: printf("\nInvalid choice.\n");
        }
    }
}

// --- Core Functionality ---

void create_account() {
    struct Account acc;
    acc.acc_no = get_next_acc_no();
    acc.balance = 0.0;

    printf("\n--- Create New Account (Account No: %d) ---\n", acc.acc_no);
    printf("Enter your full name: ");
    fgets(acc.name, MAX_NAME_LEN, stdin);
    acc.name[strcspn(acc.name, "\n")] = 0;

    char password[MAX_PASS_LEN];
    printf("Create a password: ");
    fgets(password, MAX_PASS_LEN, stdin);
    password[strcspn(password, "\n")] = 0;
    hash_password(password, acc.password_hash);

    FILE *fp = fopen(FILENAME, "ab");
    if (!fp) {
        perror("Error creating account");
        return;
    }
    fwrite(&acc, sizeof(struct Account), 1, fp);
    fclose(fp);

    printf("\nAccount created successfully! Your account number is %d.\n", acc.acc_no);
}

void login() {
    int acc_no;
    char password[MAX_PASS_LEN], hashed_pass[MAX_PASS_LEN];

    printf("\n--- Account Login ---\n");
    printf("Enter account number: ");
    if (scanf("%d", &acc_no) != 1) {
        printf("Invalid account number format.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    printf("Enter password: ");
    fgets(password, MAX_PASS_LEN, stdin);
    password[strcspn(password, "\n")] = 0;
    hash_password(password, hashed_pass);

    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) {
        printf("No accounts found. Please create one first.\n");
        return;
    }

    struct Account acc;
    int found = 0;
    while (fread(&acc, sizeof(struct Account), 1, fp)) {
        if (acc.acc_no == acc_no && strcmp(acc.password_hash, hashed_pass) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (found) {
        printf("\nLogin successful! Welcome, %s.\n", acc.name);
        logged_in_menu(acc_no);
    } else {
        printf("\nLogin failed. Invalid account number or password.\n");
    }
}

void logged_in_menu(int acc_no) {
    int choice;
    while (1) {
        printf("\n--- Account Menu ---\n");
        printf("1. Deposit\n");
        printf("2. Withdraw\n");
        printf("3. Check Balance\n");
        printf("4. Logout\n");
        printf("--------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\nInvalid input.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1: deposit(acc_no); break;
            case 2: withdraw(acc_no); break;
            case 3: check_balance(acc_no); break;
            case 4: return;
            default: printf("\nInvalid choice.\n");
        }
    }
}

void deposit(int acc_no) {
    double amount;
    printf("Enter amount to deposit: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Invalid amount.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    FILE *fp = fopen(FILENAME, "rb+");
    struct Account acc;
    while (fread(&acc, sizeof(struct Account), 1, fp)) {
        if (acc.acc_no == acc_no) {
            acc.balance += amount;
            fseek(fp, -sizeof(struct Account), SEEK_CUR);
            fwrite(&acc, sizeof(struct Account), 1, fp);
            printf("Successfully deposited %.2f. New balance: %.2f\n", amount, acc.balance);
            break;
        }
    }
    fclose(fp);
}

void withdraw(int acc_no) {
    double amount;
    printf("Enter amount to withdraw: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Invalid amount.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    FILE *fp = fopen(FILENAME, "rb+");
    struct Account acc;
    while (fread(&acc, sizeof(struct Account), 1, fp)) {
        if (acc.acc_no == acc_no) {
            if (acc.balance < amount) {
                printf("Insufficient funds. Current balance: %.2f\n", acc.balance);
            } else {
                acc.balance -= amount;
                fseek(fp, -sizeof(struct Account), SEEK_CUR);
                fwrite(&acc, sizeof(struct Account), 1, fp);
                printf("Successfully withdrew %.2f. New balance: %.2f\n", amount, acc.balance);
            }
            break;
        }
    }
    fclose(fp);
}

void check_balance(int acc_no) {
    FILE *fp = fopen(FILENAME, "rb");
    struct Account acc;
    while (fread(&acc, sizeof(struct Account), 1, fp)) {
        if (acc.acc_no == acc_no) {
            printf("\nYour current balance is: %.2f\n", acc.balance);
            break;
        }
    }
    fclose(fp);
}


// --- Utility Functions ---

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_next_acc_no() {
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) return 1001;
    struct Account acc;
    int max_no = 1000;
    while (fread(&acc, sizeof(struct Account), 1, fp)) {
        if (acc.acc_no > max_no) max_no = acc.acc_no;
    }
    fclose(fp);
    return max_no + 1;
}

/**
 * @brief A simple, non-secure hashing function for demonstration.
 * DO NOT use this for real-world applications.
 */
void hash_password(const char *password, char *hash) {
    unsigned long h = 5381;
    int c;
    while ((c = *password++)) {
        h = ((h << 5) + h) + c; // djb2 hash
    }
    sprintf(hash, "%lu", h);
}
