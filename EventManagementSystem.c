#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defining maximum sizes for input data
#define MAX_NAME_LEN 100
#define MAX_DESC_LEN 255
#define MAX_LOCATION_LEN 100
#define MAX_CATEGORY_LEN 50
#define MAX_GUEST_LIST 100
#define MAX_FEEDBACK_LEN 500
#define MAX_EVENTS 10

// Define the structures using typedef
typedef struct {
    char name[MAX_NAME_LEN];
    char description[MAX_DESC_LEN];
    char date[MAX_NAME_LEN];
    char time[MAX_NAME_LEN];
    char location[MAX_LOCATION_LEN];
    char category[MAX_CATEGORY_LEN];
    int publicEvent; // 1 for public, 0 for private
} Event;

typedef struct {// Ticket structure
    float price;
    int Available;
    int Sold;
} Ticket;

typedef struct { // Guest structure
    char name[MAX_NAME_LEN];
    char email[MAX_NAME_LEN];
    char phone[MAX_NAME_LEN];
} Guest;


typedef struct {
    char feedback[MAX_FEEDBACK_LEN];
    int rating; // 1 to 5 rating scale
} Feedback;

// Global variables
int numEvents = 0;
Event eventList[MAX_EVENTS];
Ticket ticketList[MAX_EVENTS];
Guest guestList[MAX_GUEST_LIST];
Feedback feedbackList[MAX_EVENTS];  // Feedback for each event

// Function prototypes
void createEvent();
void viewEventList();
void buyTickets();
void provideFeedback();
void viewGuestList();
void generateReport();
void saveDataToFile();
void loadDataFromFile();

// Main function
int main() {
    int choice;

    // Load event data from file
    loadDataFromFile();

    while (1) {
        printf("\nEvent Management System\n");
        printf("---------------------------------------------------------------------------\n");
        printf("1. Create Event\n");
        printf("2. View Event List\n");
        printf("3. Buy Tickets\n");
        printf("4. Provide Feedback\n");
        printf("5. View Guest List\n");
        printf("6. Generate Report\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline left in the input buffer
 printf("-------------------------------------------------------------------\n");
        switch (choice) {
            case 1:
                createEvent();
                break;
            case 2:
                viewEventList();
                break;
            case 3:
                buyTickets();
                break;
            case 4:
                provideFeedback();
                break;
            case 5:
                viewGuestList();
                break;
            case 6:
                generateReport();
                break;
            case 7:
                saveDataToFile();
                printf("Exiting program.\n");
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
                printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
        }
    }

    return 0;
}

// Function to create a new event
void createEvent() {
    if (numEvents >= MAX_EVENTS) {
        printf("Cannot create more events. Maximum limit reached.\n");
        return;
    }

    printf("Enter Event Name: ");
    fgets(eventList[numEvents].name, MAX_NAME_LEN, stdin);
    eventList[numEvents].name[strcspn(eventList[numEvents].name, "\n")] = '\0';  // Remove trailing newline

    printf("Enter Event Description: ");
    fgets(eventList[numEvents].description, MAX_DESC_LEN, stdin);
    eventList[numEvents].description[strcspn(eventList[numEvents].description, "\n")] = '\0';

    printf("Enter Event Date (DD-MM-YYYY): ");
    fgets(eventList[numEvents].date, MAX_NAME_LEN, stdin);
    eventList[numEvents].date[strcspn(eventList[numEvents].date, "\n")] = '\0';

    printf("Enter Event Time (HH:MM): ");
    fgets(eventList[numEvents].time, MAX_NAME_LEN, stdin);
    eventList[numEvents].time[strcspn(eventList[numEvents].time, "\n")] = '\0';

    printf("Enter Event Location: ");
    fgets(eventList[numEvents].location, MAX_LOCATION_LEN, stdin);
    eventList[numEvents].location[strcspn(eventList[numEvents].location, "\n")] = '\0';

    printf("Enter Event Category: ");
    fgets(eventList[numEvents].category, MAX_CATEGORY_LEN, stdin);
    eventList[numEvents].category[strcspn(eventList[numEvents].category, "\n")] = '\0';


int choice; // Variable to store user choice
printf("Any chief guest for the event (yes:1/no:0): ");
scanf("%d", &choice);
getchar(); // Consume newline left in the input buffer

if (choice == 1) {
    printf("Enter Chief Guest Name: ");
    fgets(guestList[numEvents].name, MAX_NAME_LEN, stdin);
    guestList[numEvents].name[strcspn(guestList[numEvents].name, "\n")] = '\0';

    printf("Enter Chief Guest Email: ");
    fgets(guestList[numEvents].email, MAX_NAME_LEN, stdin);
    guestList[numEvents].email[strcspn(guestList[numEvents].email, "\n")] = '\0';

    printf("Enter Chief Guest Phone: ");
    fgets(guestList[numEvents].phone, MAX_NAME_LEN, stdin);
    guestList[numEvents].phone[strcspn(guestList[numEvents].phone, "\n")] = '\0';

    printf("\n\nChief Guest added successfully!\n");  
     printf("-------------------------------------------------------------------\n");  
}

    printf("Enter Tickets Available: ");
    scanf("%d", &ticketList[numEvents].Available);
    getchar(); // Consume newline left in the input buffer

    printf("Enter Tickets Sold: ");
    scanf("%d", &ticketList[numEvents].Sold);
    getchar();

    printf("Enter Ticket Price: ");
    scanf("%f", &ticketList[numEvents].price);
    getchar();

    printf("Is the event Public (1 for Yes, 0 for No): ");
    scanf("%d", &eventList[numEvents].publicEvent);
    getchar();

    printf("\n\nEvent created successfully!\n");
    numEvents++;
     printf("-------------------------------------------------------------------\n");
}

// Function to view all events
void viewEventList() {
    printf("\nEvent List:\n");
    printf("Name\t\t\tLocation\t\t\tDate\t\t\tTime\t\t\tTickets Available\t\tGuest \n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < numEvents; i++) {
        if(eventList[i].publicEvent == 1){
        if (strlen(eventList[i].name) > 0) { // If event exists
            printf("%s\t\t\t%s\t\t\t\t%s\t\t%s\t\t\t%d\t\t\t\t%s\n",
                   eventList[i].name,
                   eventList[i].location,
                   eventList[i].date,
                   eventList[i].time,
                   ticketList[i].Available,
                   guestList[i].name);
        }
         printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        }
    }
}

// Function to allow users to buy tickets
void buyTickets() {
    char eventName[MAX_NAME_LEN];
    printf("Enter Event Name to Buy Tickets: ");
    fgets(eventName, MAX_NAME_LEN, stdin);
    eventName[strcspn(eventName, "\n")] = '\0'; // Remove newline

    int eventFound = 0;
    for (int i = 0; i < numEvents; i++) {
        if (strcmp(eventList[i].name, eventName) == 0) {
            eventFound = 1;
            printf("Event Found: %s\n", eventList[i].name);
            printf("Tickets Available: %d\n", ticketList[i].Available);
            printf("Enter number of tickets to buy: ");
            int numTickets;
            scanf("%d", &numTickets);
            getchar(); // Consume newline
           
            if (numTickets <= ticketList[i].Available) {
                int choice;
                printf("No.of tickets you want to buy: %d\n Total Price:%.2f\n", numTickets, numTickets * ticketList[i].price);
                printf("Confirm Purchase (1 for Yes, 0 for No): ");
                scanf("%d", &choice);
                if(choice == 1){
                ticketList[i].Sold += numTickets;
                ticketList[i].Available -= numTickets;
                printf("You have successfully purchased %d tickets for %s.\n", numTickets, eventList[i].name);
                 printf("-------------------------------------------------------------------\n");
                }else {
                    printf("Purchase cancelled.\n");
                     printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
                    break;
                }
            } else {
                printf("Not enough tickets available!\n");
                 printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
            }
            break;
        }
    }

    if (!eventFound) {
        printf("Event not found.\n");
         printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    }
}

// Function to collect feedback from attendees
void provideFeedback() {
    char eventName[MAX_NAME_LEN];
    printf("Enter Event Name to Provide Feedback: ");
    fgets(eventName, MAX_NAME_LEN, stdin);
    eventName[strcspn(eventName, "\n")] = '\0'; // Remove newline

    int eventFound = 0;
    for (int i = 0; i < numEvents; i++) {
        if (strcmp(eventList[i].name, eventName) == 0) {
            eventFound = 1;
            printf("Event Found: %s\n", eventList[i].name);

            printf("Enter your feedback (up to 500 characters): ");
            fgets(feedbackList[i].feedback, MAX_FEEDBACK_LEN, stdin);
            feedbackList[i].feedback[strcspn(feedbackList[i].feedback, "\n")] = '\0'; // Remove newline

            printf("Rate the event (1-5): ");
            scanf("%d", &feedbackList[i].rating);
            getchar(); // Consume newline

            printf("Thank you for your feedback!\n");
             printf("-------------------------------------------------------------------\n");
            break;
        }
    }

    if (!eventFound) {
        printf("Event not found.\n");
         printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    }
}

// Function to view the guest list
void viewGuestList() {
    printf("\nGuest List:\n");
    printf("Name\t\t\t\tEmail\t\t\t\tPhone\n");
    printf("----------------------------------------------------------------------------------\n");

    for (int i = 0; i < MAX_GUEST_LIST; i++) {
        if (strlen(guestList[i].name) > 0) {
            printf("%s\t\t\t%s\t\t\t%s\n", guestList[i].name, guestList[i].email, guestList[i].phone);
            printf("---------------------------------------------------------------------------------------\n");
        }
    }
}

// Function to create ticket information


// Function to generate a simple event report
void generateReport() {
    printf("\nEvent Report:\n");
    for (int i = 0; i < numEvents; i++) {
        if (strlen(eventList[i].name) > 0) { // If event exists
            printf("Event Name: %s\n", eventList[i].name);
            printf("Date: %s\n", eventList[i].date);
            printf("Location: %s\n", eventList[i].location);
            printf("Tickets Available: %d\n", ticketList[i].Available);
            printf("Tickets Sold: %d\n", ticketList[i].Sold);
            printf("\nFeedback:\n");
            if (strlen(feedbackList[i].feedback) > 0) {
                printf("Rating: %d\n", feedbackList[i].rating);
                printf("Feedback: %s\n", feedbackList[i].feedback);
            } else {
                printf("No feedback provided.\n");
                 printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
            }
            printf("-----------------------------------------------------\n");
        }
    }
}

// Function to save event data to a file
void saveDataToFile() {
    FILE *file = fopen("event_data.dat", "wb");
    if (file != NULL) {
        fwrite(&numEvents, sizeof(int), 1, file); // Write number of events
        fwrite(eventList, sizeof(Event), numEvents, file); // Write actual events
        fwrite(guestList, sizeof(Guest), numEvents, file); // Write guest list
        fwrite(ticketList, sizeof(Ticket), numEvents, file); // Write tickets for events
        fwrite(feedbackList, sizeof(Feedback), numEvents, file); // Write feedback
        fclose(file);
        printf("Data saved to file.\n");
         printf("-------------------------------------------------------------------\n");
    } else {
        printf("Error saving event data.\n");
         printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    }
}

// Function to load event data from a file
void loadDataFromFile() {
    FILE *file = fopen("event_data.dat", "rb");
    if (file != NULL) {
        fread(&numEvents, sizeof(int), 1, file); // Read number of events
        fread(eventList, sizeof(Event), numEvents, file); // Read actual events
        fread(guestList, sizeof(Guest), numEvents, file); // Read guest list
        fread(ticketList, sizeof(Ticket), numEvents, file); // Read tickets for events
        fread(feedbackList, sizeof(Feedback), numEvents, file); // Read feedback
        fclose(file);
    }
}