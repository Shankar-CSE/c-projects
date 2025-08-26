#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define USERS_FILE "chat_users.txt"
#define MESSAGES_FILE "chat_messages.txt"
#define MAX_LEN 100
#define MAX_MSG_LEN 256

struct User {
    char username[MAX_LEN];
    char password[MAX_LEN];
};

struct Message {
    char sender[MAX_LEN];
    char receiver[MAX_LEN];
    char content[MAX_MSG_LEN];
    long timestamp;
};

// --- Function Prototypes ---
void register_user();
void login();
void logged_in_menu(const char *username);
void send_message(const char *sender);
void view_inbox(const char *username);
void list_users();
int user_exists(const char *username);
void clear_input_buffer();

// --- Main Function ---
int main() {
    int choice;
    while (1) {
        printf("\n===== Offline Chat Simulation =====\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("===================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1: register_user(); break;
            case 2: login(); break;
            case 3: exit(0);
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}

// --- Core Functionality ---

void register_user() {
    struct User u;
    printf("\n--- Register New User ---\n");
    printf("Enter username: ");
    fgets(u.username, MAX_LEN, stdin);
    u.username[strcspn(u.username, "\n")] = 0;

    if (user_exists(u.username)) {
        printf("Username already exists.\n");
        return;
    }

    printf("Enter password: ");
    fgets(u.password, MAX_LEN, stdin);
    u.password[strcspn(u.password, "\n")] = 0;

    FILE *fp = fopen(USERS_FILE, "a");
    fprintf(fp, "%s;%s\n", u.username, u.password);
    fclose(fp);

    printf("User '%s' registered successfully.\n", u.username);
}

void login() {
    char username[MAX_LEN], password[MAX_LEN];
    printf("\n--- User Login ---\n");
    printf("Enter username: ");
    fgets(username, MAX_LEN, stdin);
    username[strcspn(username, "\n")] = 0;

    printf("Enter password: ");
    fgets(password, MAX_LEN, stdin);
    password[strcspn(password, "\n")] = 0;

    FILE *fp = fopen(USERS_FILE, "r");
    if (!fp) {
        printf("No users registered yet.\n");
        return;
    }

    struct User u;
    int logged_in = 0;
    while (fscanf(fp, "%[^;];%[^\n]\n", u.username, u.password) == 2) {
        if (strcmp(u.username, username) == 0 && strcmp(u.password, password) == 0) {
            logged_in = 1;
            break;
        }
    }
    fclose(fp);

    if (logged_in) {
        printf("\nLogin successful. Welcome, %s!\n", username);
        logged_in_menu(username);
    } else {
        printf("\nLogin failed. Invalid username or password.\n");
    }
}

void logged_in_menu(const char *username) {
    int choice;
    while (1) {
        printf("\n--- Chat Menu ---\n");
        printf("1. Send Message\n");
        printf("2. View Inbox\n");
        printf("3. List Users\n");
        printf("4. Logout\n");
        printf("-------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1: send_message(username); break;
            case 2: view_inbox(username); break;
            case 3: list_users(); break;
            case 4: return;
            default: printf("Invalid choice.\n");
        }
    }
}

void send_message(const char *sender) {
    struct Message msg;
    strcpy(msg.sender, sender);

    printf("Enter recipient's username: ");
    fgets(msg.receiver, MAX_LEN, stdin);
    msg.receiver[strcspn(msg.receiver, "\n")] = 0;

    if (!user_exists(msg.receiver)) {
        printf("User '%s' does not exist.\n", msg.receiver);
        return;
    }

    printf("Enter your message:\n");
    fgets(msg.content, MAX_MSG_LEN, stdin);
    msg.content[strcspn(msg.content, "\n")] = 0;
    msg.timestamp = time(NULL);

    FILE *fp = fopen(MESSAGES_FILE, "a");
    fprintf(fp, "%s;%s;%ld;%s\n", msg.sender, msg.receiver, msg.timestamp, msg.content);
    fclose(fp);

    printf("Message sent to %s.\n", msg.receiver);
}

void view_inbox(const char *username) {
    printf("\n--- Your Inbox ---\n");
    FILE *fp = fopen(MESSAGES_FILE, "r");
    if (!fp) {
        printf("No messages.\n");
        return;
    }

    struct Message msg;
    int count = 0;
    while (fscanf(fp, "%[^;];%[^;];%ld;%[^\n]\n", msg.sender, msg.receiver, &msg.timestamp, msg.content) == 4) {
        if (strcmp(msg.receiver, username) == 0) {
            time_t ts = msg.timestamp;
            char time_str[30];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M", localtime(&ts));
            printf("From: %s [%s]\n> %s\n\n", msg.sender, time_str, msg.content);
            count++;
        }
    }
    fclose(fp);

    if (count == 0) {
        printf("Your inbox is empty.\n");
    }
}

// --- Utility Functions ---

void list_users() {
    printf("\n--- Registered Users ---\n");
    FILE *fp = fopen(USERS_FILE, "r");
    if (!fp) {
        printf("No users registered.\n");
        return;
    }
    struct User u;
    while (fscanf(fp, "%[^;];%[^\n]\n", u.username, u.password) == 2) {
        printf("- %s\n", u.username);
    }
    fclose(fp);
}

int user_exists(const char *username) {
    FILE *fp = fopen(USERS_FILE, "r");
    if (!fp) return 0;
    struct User u;
    int found = 0;
    while (fscanf(fp, "%[^;];%[^\n]\n", u.username, u.password) == 2) {
        if (strcmp(u.username, username) == 0) {
            found = 1;
            break;
        }
    }
    fclose(fp);
    return found;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
