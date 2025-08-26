#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Define constants
#define HISTORY_FILENAME "calculator_history.txt"
#define HISTORY_SIZE 10
#define MAX_EXPR_LEN 100

// Structure to store a single calculation
struct Calculation {
    char expression[MAX_EXPR_LEN];
    double result;
};

// Global array to store history and a counter
struct Calculation history[HISTORY_SIZE] = {0}; // Zero-initialize the array
int history_count = 0;
int current_history_index = 0;

// Function Prototypes
void add_to_history(const char *expr, double res);
void save_history();
void load_history();
void display_history();
void perform_calculation(char op);
void perform_scientific(const char* func_name);
void clear_input_buffer();
double get_double_input();

int main() {
    int choice;

    load_history();

    while (1) {
        printf("\n===== C Calculator =====\n");
        printf("1. Add (+)\n");
        printf("2. Subtract (-)\n");
        printf("3. Multiply (*)\n");
        printf("4. Divide (/)\n");
        printf("5. Sine (sin)\n");
        printf("6. Cosine (cos)\n");
        printf("7. Log (natural log)\n");
        printf("8. View History\n");
        printf("9. Exit\n");
        printf("========================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\nError: Invalid input. Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1: perform_calculation('+'); break;
            case 2: perform_calculation('-'); break;
            case 3: perform_calculation('*'); break;
            case 4: perform_calculation('/'); break;
            case 5: perform_scientific("sin"); break;
            case 6: perform_scientific("cos"); break;
            case 7: perform_scientific("log"); break;
            case 8: display_history(); break;
            case 9:
                printf("\nExiting calculator. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice. Please enter a number between 1 and 9.\n");
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
 * @brief Gets a valid double from the user.
 * @return The double value entered by the user.
 */
double get_double_input() {
    double num;
    while (scanf("%lf", &num) != 1) {
        printf("Invalid input. Please enter a number: ");
        clear_input_buffer();
    }
    clear_input_buffer();
    return num;
}

/**
 * @brief Adds a calculation to the history array and saves to file.
 * @param expr The expression string.
 * @param res The result of the calculation.
 */
void add_to_history(const char *expr, double res) {
    // Update in-memory circular buffer
    snprintf(history[current_history_index].expression, MAX_EXPR_LEN, "%s", expr);
    history[current_history_index].result = res;

    current_history_index = (current_history_index + 1) % HISTORY_SIZE;

    if (history_count < HISTORY_SIZE) {
        history_count++;
    }

    // Persist the change to the file
    save_history();
}

/**
 * @brief Saves the current calculation history to a text file.
 */
void save_history() {
    FILE *fp = fopen(HISTORY_FILENAME, "w");
    if (fp == NULL) {
        perror("Warning: Could not save history to file");
        return;
    }

    // Save the state of the circular buffer
    fprintf(fp, "%d\n", history_count);
    fprintf(fp, "%d\n", current_history_index);

    // Write all records currently in the buffer array
    for (int i = 0; i < HISTORY_SIZE; i++) {
        fprintf(fp, "%s;%f\n", history[i].expression, history[i].result);
    }

    fclose(fp);
}

/**
 * @brief Loads the calculation history from a text file on startup.
 */
void load_history() {
    FILE *fp = fopen(HISTORY_FILENAME, "r");
    if (fp == NULL) {
        // File doesn't exist yet, which is fine on the first run.
        return;
    }

    // Read the state of the circular buffer
    if (fscanf(fp, "%d\n%d\n", &history_count, &current_history_index) != 2) {
        fclose(fp);
        return; // Assume file is corrupt/empty, start fresh.
    }

    // Read the records
    for (int i = 0; i < HISTORY_SIZE; i++) {
        // Use a format that reads all characters up to the delimiter
        if (fscanf(fp, "%[^;];%lf\n", history[i].expression, &history[i].result) != 2) {
            break; // Stop if a line is malformed.
        }
    }

    fclose(fp);
}

/**
 * @brief Displays the last N calculations from history.
 */
void display_history() {
    printf("\n--- Calculation History (Last %d) ---\n", history_count);

    if (history_count == 0) {
        printf("No history yet.\n");
        return;
    }

    // Display from the oldest to the newest in the circular buffer
    for (int i = 0; i < history_count; i++) {
        int index = (current_history_index - history_count + i + HISTORY_SIZE) % HISTORY_SIZE;
        printf("%d: %s = %.4f\n", i + 1, history[index].expression, history[index].result);
    }
    printf("-------------------------------------\n");
}

/**
 * @brief Performs a basic arithmetic calculation (+, -, *, /).
 * @param op The operator character.
 */
void perform_calculation(char op) {
    double num1, num2, result;
    char expression[MAX_EXPR_LEN];

    printf("Enter first number: ");
    num1 = get_double_input();
    printf("Enter second number: ");
    num2 = get_double_input();

    switch (op) {
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '*': result = num1 * num2; break;
        case '/':
            if (num2 == 0) {
                printf("Error: Division by zero is not allowed.\n");
                return;
            }
            result = num1 / num2;
            break;
    }

    snprintf(expression, MAX_EXPR_LEN, "%.2f %c %.2f", num1, op, num2);
    printf("Result: %s = %.4f\n", expression, result);
    add_to_history(expression, result);
}

/**
 * @brief Performs a scientific calculation (sin, cos, log).
 * @param func_name The name of the function as a string.
 */
void perform_scientific(const char* func_name) {
    double num, result;
    char expression[MAX_EXPR_LEN];

    printf("Enter the number: ");
    num = get_double_input();

    if (strcmp(func_name, "sin") == 0) {
        result = sin(num);
        snprintf(expression, MAX_EXPR_LEN, "sin(%.2f)", num);
    } else if (strcmp(func_name, "cos") == 0) {
        result = cos(num);
        snprintf(expression, MAX_EXPR_LEN, "cos(%.2f)", num);
    } else if (strcmp(func_name, "log") == 0) {
        if (num <= 0) {
            printf("Error: Logarithm is only defined for positive numbers.\n");
            return;
        }
        result = log(num);
        snprintf(expression, MAX_EXPR_LEN, "log(%.2f)", num);
    } else {
        printf("Internal error: Unknown scientific function.\n");
        return;
    }

    printf("Result: %s = %.4f\n", expression, result);
    add_to_history(expression, result);
}
