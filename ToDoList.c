#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define constants
#define TASKS_FILENAME "tasks.txt"
#define MAX_TASKS 100
#define MAX_DESC_LEN 256

// Structure to represent a single task
struct Task {
    char description[MAX_DESC_LEN];
    int is_complete; // 0 for incomplete, 1 for complete
};

// Global array to store tasks and a counter
struct Task tasks[MAX_TASKS];
int task_count = 0;

// Function Prototypes
void load_tasks();
void save_tasks();
void display_tasks();
void add_task();
void remove_task();
void mark_task_complete();
void clear_input_buffer();

int main() {
    int choice;
    load_tasks();

    while (1) {
        printf("\n===== To-Do List CLI =====\n");
        printf("1. Display Tasks\n");
        printf("2. Add Task\n");
        printf("3. Remove Task\n");
        printf("4. Mark Task as Complete\n");
        printf("5. Exit\n");
        printf("==========================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\nError: Invalid input. Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1: display_tasks(); break;
            case 2: add_task(); break;
            case 3: remove_task(); break;
            case 4: mark_task_complete(); break;
            case 5:
                printf("\nExiting. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice. Please enter a number between 1 and 5.\n");
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
 * @brief Loads tasks from the text file into memory.
 */
void load_tasks() {
    FILE *fp = fopen(TASKS_FILENAME, "r");
    if (fp == NULL) {
        // File doesn't exist, which is fine on the first run.
        return;
    }

    task_count = 0;
    while (task_count < MAX_TASKS &&
           fscanf(fp, "%d;%[^\n]\n", &tasks[task_count].is_complete, tasks[task_count].description) == 2) {
        task_count++;
    }

    fclose(fp);
}

/**
 * @brief Saves all tasks from memory to the text file.
 */
void save_tasks() {
    FILE *fp = fopen(TASKS_FILENAME, "w");
    if (fp == NULL) {
        perror("Error saving tasks");
        return;
    }

    for (int i = 0; i < task_count; i++) {
        fprintf(fp, "%d;%s\n", tasks[i].is_complete, tasks[i].description);
    }

    fclose(fp);
}

/**
 * @brief Displays all current tasks to the console.
 */
void display_tasks() {
    printf("\n--- Your Tasks ---\n");
    if (task_count == 0) {
        printf("No tasks to show. Add one!\n");
    } else {
        for (int i = 0; i < task_count; i++) {
            printf("%d. [%c] %s\n",
                   i + 1,
                   (tasks[i].is_complete ? 'X' : ' '),
                   tasks[i].description);
        }
    }
    printf("------------------\n");
}

/**
 * @brief Adds a new task to the list.
 */
void add_task() {
    if (task_count >= MAX_TASKS) {
        printf("Cannot add more tasks. The list is full.\n");
        return;
    }

    printf("Enter the description for the new task: ");
    fgets(tasks[task_count].description, MAX_DESC_LEN, stdin);
    // Remove trailing newline character from fgets
    tasks[task_count].description[strcspn(tasks[task_count].description, "\n")] = 0;

    tasks[task_count].is_complete = 0; // New tasks are incomplete by default
    task_count++;

    save_tasks();
    printf("Task added successfully.\n");
}

/**
 * @brief Removes a task from the list by its number.
 */
void remove_task() {
    if (task_count == 0) {
        printf("No tasks to remove.\n");
        return;
    }

    display_tasks();
    printf("Enter the task number to remove: ");
    int task_num;
    if (scanf("%d", &task_num) != 1 || task_num < 1 || task_num > task_count) {
        printf("Invalid task number.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Shift tasks to the left to fill the gap
    for (int i = task_num - 1; i < task_count - 1; i++) {
        tasks[i] = tasks[i + 1];
    }

    task_count--;
    save_tasks();
    printf("Task %d removed successfully.\n", task_num);
}

/**
 * @brief Marks a specific task as complete.
 */
void mark_task_complete() {
    if (task_count == 0) {
        printf("No tasks to mark.\n");
        return;
    }

    display_tasks();
    printf("Enter the task number to mark as complete: ");
    int task_num;
    if (scanf("%d", &task_num) != 1 || task_num < 1 || task_num > task_count) {
        printf("Invalid task number.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (tasks[task_num - 1].is_complete) {
        printf("Task %d is already marked as complete.\n", task_num);
    } else {
        tasks[task_num - 1].is_complete = 1;
        save_tasks();
        printf("Task %d marked as complete.\n", task_num);
    }
}

