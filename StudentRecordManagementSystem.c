#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define constants for max lengths and the data filename
#define FILENAME "student_records.dat"
#define MAX_NAME_LEN 50
#define MAX_COURSE_LEN 50

// Structure to represent a student
struct Student {
    int roll_no;
    char name[MAX_NAME_LEN];
    char course[MAX_COURSE_LEN];
    float fees;
};

// Function Prototypes
void add_student();
void delete_student();
void search_student();
void update_student();
void display_all_students();
void generate_dummy_data();
void clear_input_buffer();

int main() {
    int choice;

    while (1) {
        printf("\n===== Student Record Management System =====\n");
        printf("1. Add Student\n");
        printf("2. Delete Student\n");
        printf("3. Search Student\n");
        printf("4. Update Student Record\n");
        printf("5. Display All Students\n");
        printf("6. Generate 100 Dummy Records\n");
        printf("7. Exit\n");
        printf("==========================================\n");
        printf("Enter your choice: ");

        // Basic input validation
        if (scanf("%d", &choice) != 1) {
            printf("\nError: Invalid input. Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer(); // Consume the newline character left by scanf

        switch (choice) {
            case 1:
                add_student();
                break;
            case 2:
                delete_student();
                break;
            case 3:
                search_student();
                break;
            case 4:
                update_student();
                break;
            case 5:
                display_all_students();
                break;
            case 6:
                generate_dummy_data();
                break;
            case 7:
                printf("\nExiting program. Goodbye!\n");
                exit(0);
            default:
                printf("\nInvalid choice. Please enter a number between 1 and 7.\n");
        }
    }

    return 0;
}

/**
 * @brief Clears the standard input buffer.
 * Useful after scanf to consume leftover characters, especially newlines.
 */
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Adds a new student record to the file.
 */
void add_student() {
    struct Student new_student;
    FILE *fp;

    printf("\n--- Add New Student ---\n");
    printf("Enter Roll No: ");
    // Loop until valid integer input is received
    while (scanf("%d", &new_student.roll_no) != 1) {
        printf("Invalid input. Please enter a number for Roll No: ");
        clear_input_buffer();
    }
    clear_input_buffer();

    printf("Enter Name: ");
    fgets(new_student.name, MAX_NAME_LEN, stdin);
    new_student.name[strcspn(new_student.name, "\n")] = 0; // Remove trailing newline

    printf("Enter Course: ");
    fgets(new_student.course, MAX_COURSE_LEN, stdin);
    new_student.course[strcspn(new_student.course, "\n")] = 0;

    printf("Enter Fees: ");
    while (scanf("%f", &new_student.fees) != 1) {
        printf("Invalid input. Please enter a number for Fees: ");
        clear_input_buffer();
    }
    clear_input_buffer();

    // Open file in "append binary" mode
    fp = fopen(FILENAME, "ab");
    if (fp == NULL) {
        printf("Error: Could not open file %s\n", FILENAME);
        return;
    }

    fwrite(&new_student, sizeof(struct Student), 1, fp);
    fclose(fp);

    printf("\nStudent record added successfully!\n");
}

/**
 * @brief Displays all student records from the file.
 */
void display_all_students() {
    struct Student current_student;
    FILE *fp;

    // Open file in "read binary" mode
    fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("\nError: Could not open file or no records exist yet.\n");
        return;
    }

    printf("\n--- All Student Records ---\n");
    printf("%-10s %-30s %-20s %-10s\n", "Roll No", "Name", "Course", "Fees");
    printf("---------------------------------------------------------------------\n");

    int count = 0;
    while (fread(&current_student, sizeof(struct Student), 1, fp) == 1) {
        printf("%-10d %-30s %-20s %-10.2f\n",
               current_student.roll_no,
               current_student.name,
               current_student.course,
               current_student.fees);
        count++;
    }

    if (count == 0) {
        printf("No records found.\n");
    }
    printf("---------------------------------------------------------------------\n");

    fclose(fp);
}

/**
 * @brief Searches for a student by roll number and displays their record.
 */
void search_student() {
    int roll_to_search;
    struct Student current_student;
    FILE *fp;
    int found = 0;

    printf("\n--- Search for a Student ---\n");
    printf("Enter Roll No to search: ");
    if (scanf("%d", &roll_to_search) != 1) {
        printf("\nInvalid input. Please enter a number.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("\nError: Could not open file or no records exist yet.\n");
        return;
    }

    while (fread(&current_student, sizeof(struct Student), 1, fp) == 1) {
        if (current_student.roll_no == roll_to_search) {
            printf("\n--- Record Found ---\n");
            printf("Roll No: %d\n", current_student.roll_no);
            printf("Name:    %s\n", current_student.name);
            printf("Course:  %s\n", current_student.course);
            printf("Fees:    %.2f\n", current_student.fees);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("\nStudent with Roll No %d not found.\n", roll_to_search);
    }

    fclose(fp);
}

/**
 * @brief Updates an existing student's record, identified by roll number.
 */
void update_student() {
    int roll_to_update;
    struct Student current_student;
    FILE *fp;
    int found = 0;

    printf("\n--- Update a Student's Record ---\n");
    printf("Enter Roll No to update: ");
    if (scanf("%d", &roll_to_update) != 1) {
        printf("\nInvalid input. Please enter a number.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    // Open file in "read/write binary" mode
    fp = fopen(FILENAME, "rb+");
    if (fp == NULL) {
        printf("\nError: Could not open file or no records exist yet.\n");
        return;
    }

    while (fread(&current_student, sizeof(struct Student), 1, fp) == 1) {
        if (current_student.roll_no == roll_to_update) {
            found = 1;
            printf("\n--- Enter New Details for Roll No %d ---\n", roll_to_update);

            printf("Enter new Name: ");
            fgets(current_student.name, MAX_NAME_LEN, stdin);
            current_student.name[strcspn(current_student.name, "\n")] = 0;

            printf("Enter new Course: ");
            fgets(current_student.course, MAX_COURSE_LEN, stdin);
            current_student.course[strcspn(current_student.course, "\n")] = 0;

            printf("Enter new Fees: ");
            while (scanf("%f", &current_student.fees) != 1) {
                printf("Invalid input. Please enter a number for Fees: ");
                clear_input_buffer();
            }
            clear_input_buffer();

            // Move file pointer back to the beginning of the current record to overwrite it
            fseek(fp, -sizeof(struct Student), SEEK_CUR);
            fwrite(&current_student, sizeof(struct Student), 1, fp);

            printf("\nRecord updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("\nStudent with Roll No %d not found.\n", roll_to_update);
    }

    fclose(fp);
}

/**
 * @brief Deletes a student record, identified by roll number.
 * This is done by writing all other records to a temporary file,
 * then replacing the original file with the temporary one.
 */
void delete_student() {
    int roll_to_delete;
    struct Student current_student;
    FILE *fp, *temp_fp;
    int found = 0;
    const char *temp_filename = "temp.dat";

    printf("\n--- Delete a Student's Record ---\n");
    printf("Enter Roll No to delete: ");
    if (scanf("%d", &roll_to_delete) != 1) {
        printf("\nInvalid input. Please enter a number.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        printf("\nError: Could not open file or no records exist yet.\n");
        return;
    }

    temp_fp = fopen(temp_filename, "wb");
    if (temp_fp == NULL) {
        printf("Error: Could not create temporary file.\n");
        fclose(fp);
        return;
    }

    while (fread(&current_student, sizeof(struct Student), 1, fp) == 1) {
        if (current_student.roll_no == roll_to_delete) {
            found = 1;
        } else {
            fwrite(&current_student, sizeof(struct Student), 1, temp_fp);
        }
    }

    fclose(fp);
    fclose(temp_fp);

    if (found) {
        remove(FILENAME);
        rename(temp_filename, FILENAME);
        printf("\nRecord for Roll No %d deleted successfully!\n", roll_to_delete);
    } else {
        remove(temp_filename); // No changes needed, so remove temp file
        printf("\nStudent with Roll No %d not found.\n", roll_to_delete);
    }
}

/**
 * @brief Generates and saves 100 dummy student records, overwriting the existing file.
 */
void generate_dummy_data() {
    FILE *fp;
    char confirmation;

    printf("\n--- Generate 100 Dummy Records ---\n");
    printf("WARNING: This will overwrite all existing student records.\n");
    printf("Are you sure you want to continue? (y/n): ");

    // The space before %c is important to consume any leading whitespace, like newlines.
    if (scanf(" %c", &confirmation) != 1) {
        printf("\nInvalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (confirmation != 'y' && confirmation != 'Y') {
        printf("Operation cancelled.\n");
        return;
    }

    // Open file in "write binary" mode to overwrite it
    fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("Error: Could not open file %s for writing.\n", FILENAME);
        return;
    }

    struct Student dummy_student;
    for (int i = 1; i <= 100; i++) {
        dummy_student.roll_no = i;
        sprintf(dummy_student.name, "Student Name %d", i);
        sprintf(dummy_student.course, "Course %d", (i % 5) + 1);
        dummy_student.fees = 1000.0f + (float)(i * 10);

        fwrite(&dummy_student, sizeof(struct Student), 1, fp);
    }

    fclose(fp);
    printf("\nSuccessfully generated and saved 100 dummy student records.\n");
}
