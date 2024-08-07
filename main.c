#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 256
#define MAX_USERS 100
#define MAX_ROOMS 100
#define MAX_BOOKINGS 100
#define MAX_RESERVATIONS 100

// User struct
typedef struct {
    char usertype[20];
    char username[20];
    char password[20];
    char status[20];
} User;

// Room Struct
typedef struct 
{
    int roomNum;
    int roomFloor;
    char roomType[20];
    char roomStatus[20];
} Room;

// Booking Struct
typedef struct 
{
    char customerUsername[20];
    int roomNum;
    int roomFloor;
    char roomType[20];
    char checkinDate[20];
    char checkoutDate[20];
} Booking;

// Reservation Struct
typedef struct 
{
    char customerUsername[20];
    int roomNum;
    int roomFloor;
    char roomType[20];
    char reservationStatus[20];
} Reservation;


// read_users function prototype
void read_users(User users[], int *user_count);

// read_rooms function prototype
void read_rooms(Room rooms[], int *room_count);

// read bookings function prototype
void read_bookings(Booking bookings[], int *booking_count);

// read reservations function prototype
void read_reservations(Reservation reservations[], int *reserve_count);

// login function prototype
int login(User users[], int user_count, char *username, char *password, char *usertype, char *status);

// admin register_user function prototype
void register_user(User users[], int *user_count);

// admin update_user_status function prototype
void update_user_status(User users[], int user_count);

// admin create_room function prototype
void create_room(Room rooms[], int *room_count);

// admin update_room function prototype
void update_room(Room rooms[], int room_count); 

// admin remove_room function prototype
void remove_room(Room rooms[], int *room_count);

// admin_menu function prototype
void admin_menu(User users[], int *user_count, Room rooms[], int *room_count);

// staff_menu function prototype
void staff_menu(Room rooms[], int *room_count, Reservation reservations[], int *reserve_count, Booking bookings[], int *booking_count);

// staff view pending reservation prototype
void view_pending_reservations_and_manage(Room rooms[], int *room_count, Reservation reservations[], int *reserve_count, Booking bookings[], int *booking_count);

void view_bookings_and_manage(Room rooms[], int *room_count, Booking bookings[], int *booking_count);

void view_room_availability(Room rooms[], int *room_count);

void view_booking_history(Booking bookings[], int *booking_count);

void customer_menu(char username[20] ,Room rooms[], int *room_count, Reservation reservations[], int *reservation_count, Booking bookings[], int *booking_count);

void view_available_rooms(Room rooms[], int *room_count);

void make_reservation(char username[20], Room rooms[], int *room_count, Reservation reservations[], int *reservation_count);

void view_reservation_history(char username[], Reservation reservations[], int *reservation_count);

void view_current_bookings(char username[], Booking bookings[], int *booking_count);

void cancel_reservation(char username[20], Room rooms[], int *room_count, Reservation reservations[], int *reservation_count);

void guest_menu(Room rooms[], int *room_count);

// Main Function
int main() 
{
    User users[MAX_USERS];  // Array to store user data
    Room rooms[MAX_ROOMS]; // Array to store room data
    Reservation reservations[MAX_RESERVATIONS]; // Array to store reservation data
    Booking bookings[MAX_BOOKINGS]; // Array to store booking data
    int user_count = 0;  // Number of users read from login.txt
    int room_count = 0; // Number of rooms read from rooms.txt
    int reservation_count = 0; // Numbers of reservations from reservations.txt
    int booking_count = 0; // Numbers of bookings from bookings.txt

    read_users(users, &user_count); // Read from login.txt 
    read_rooms(rooms, &room_count); // Read from rooms.txt
    read_reservations(reservations, &reservation_count); //Read from reservations.txt
    read_bookings(bookings, &booking_count); //Read from bookings.txt

    char username[20];
    char password[20];
    char usertype[20];
    char status[20];
    int logged_in = 0;

    while (!logged_in) 
    {
        int choice;
        char choice_input[10];

        printf("\n--- Welcome to ABC Hotel ---");
        printf("\n1. Log in\n2. Continue as guest\n3. Exit\nEnter a choice: ");
        fgets(choice_input, sizeof(choice_input), stdin);
        choice = atoi(choice_input);
        
        if (choice == 1) {
            printf("Enter username: ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = '\0';  // Remove newline character

            printf("Enter password: ");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = '\0';  
            
            if (login(users, user_count, username, password, usertype, status)) 
            {
                printf("\nLogin successful. Welcome %s %s\n", usertype, username);
                logged_in = 1;

                if (strcmp(usertype, "admin") == 0) 
                {
                    admin_menu(users, &user_count, rooms, &room_count);
                } 
                else if (strcmp(usertype, "staff") == 0) 
                {
                    staff_menu(rooms, &room_count, reservations, &reservation_count, bookings, &booking_count);
                } 
                else if (strcmp(usertype, "customer") == 0)
                {
                    customer_menu(username ,rooms, &room_count, reservations, &reservation_count, bookings, &booking_count);
                } 
            } 
            else 
            {
                if (strcmp(status, "active") != 0 && status[0] != '\0') // check if user status is not active and status[0] != '\0' used to check if string is not empty
                {
                    printf("\nLogin failed. Account status is '%s'.\n", status);
                } 
                else 
                {
                    printf("\nLogin failed. Invalid username or password.\n");
                }
            }
        } 
        else if (choice == 2) 
        {
            strcpy(username, "guest");
            strcpy(password, "123123");
            strcpy(status, "active");

            if (login(users, user_count, username, password, usertype, status)) 
            {  
                printf("\nContinuing as guest.");
                logged_in = 1;
                guest_menu(rooms, &room_count);
            }
        } 
        else if (choice == 3) 
        {
            exit(0);
        } 
        else 
        {
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
        }
    }

    return 0;
}

// Function to read users' username, password, usertype, status from the file & store in array users[] 
void read_users(User users[], int *user_count) 
{
    FILE *file = fopen("login.txt", "r");  // Error checking just in case file does not exist

    if (file == NULL) {
        printf("Error: Could not open file login.txt\n");
        exit(1);
    }

    char line[MAX_LINE];
    *user_count = 0;  // start count of users in txt file

    while (fgets(line, sizeof(line), file)) 
    {
        line[strcspn(line, "\n")] = '\0';  // Remove newline '\n' character
        sscanf(line, "%[^:]:%[^:]:%[^:]:%s", users[*user_count].usertype, users[*user_count].username, users[*user_count].password, users[*user_count].status);
        (*user_count)++;  // increase count by 1 after each line read
    }

    fclose(file);
}

// Read Functions
// Function to read rooms' roomNum, roomFloor, roomType, roomStatus(available/booked) & store in array rooms[]
void read_rooms(Room rooms[], int *room_count)
{
    FILE *file = fopen("rooms.txt","r");

    if (file == NULL) {
        printf("Error: Could not open file rooms.txt\n"); // error checking
        exit(1);
    }

    char line[MAX_LINE];
    *room_count = 0;

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0'; // Remove '\n'
        sscanf(line, "%d:%d:%[^:]:%s", &rooms[*room_count].roomNum, &rooms[*room_count].roomFloor, rooms[*room_count].roomType, rooms[*room_count].roomStatus);
        (*room_count)++;
    }

    fclose(file);
}

// Function to read bookings' CustomerUsername, roomNum, roomFloor, type, checkindate, checkoutdate & store in array bookings[]
void read_bookings(Booking bookings[], int *booking_count)
{
    FILE *file = fopen("bookings.txt","r");

    if (file == NULL) {
        printf("Error: Could not open file bookings.txt\n"); // error checking
        exit(1);
    }

    char line[MAX_LINE];
    *booking_count = 0;

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0'; // Remove '\n'
        sscanf(line, "%d:%d:%[^:]:%[^:]:%[^:]:%s", &bookings[*booking_count].roomNum, &bookings[*booking_count].roomFloor, bookings[*booking_count].roomType, bookings[*booking_count].customerUsername, bookings[*booking_count].checkinDate, bookings[*booking_count].checkoutDate);
        (*booking_count)++;
    }

    fclose(file);
}

// Function to read reservations' CustomerUsername, roomNum, roomFloor, type, approval status & store in array reservations[]
void read_reservations(Reservation reservations[], int *reservation_count)
{
    FILE *file = fopen("reservations.txt","r");

    if (file == NULL) {
        printf("Error: Could not open file reservations.txt\n"); // error checking
        exit(1);
    }

    char line[MAX_LINE];
    *reservation_count = 0;

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0'; // Remove '\n'
        sscanf(line, "%d:%d:%[^:]:%[^:]:%s", &reservations[*reservation_count].roomNum, &reservations[*reservation_count].roomFloor, 
            reservations[*reservation_count].roomType, reservations[*reservation_count].customerUsername, reservations[*reservation_count].reservationStatus);

        (*reservation_count)++;
    }

    fclose(file);
}

// Function for ALL users to login. User logs in ONLY if username and password matches
int login(User users[], int user_count, char *username, char *password, char *usertype, char *status) 
{
    for (int i = 0; i < user_count; i++) 
    {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) 
        {
            if (strcmp(users[i].status, "active") == 0)
            {
                strcpy(usertype, users[i].usertype);
                strcpy(status, users[i].status);
                return 1;
            } 
            else 
            {
                strcpy(status, users[i].status); 
                return 0;
            }
        }
    }
    return 0;
}

// Admin Functions menu
void admin_menu(User users[], int *user_count, Room rooms[], int *room_count) 
{
    int choice;

    do {
        printf("\n--- Administrator Menu ---\n");
        printf("1. Register new user\n");
        printf("2. Manage existing users\n");
        printf("3. Create hotel rooms\n");
        printf("4. Update hotel rooms\n");
        printf("5. Remove hotel rooms\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar();  // Remove newline character '\n' left in the input buffer

        switch (choice) 
        {
            case 1:
                printf("Registering a new user...\n");
                register_user(users, user_count);
                break;

            case 2:
                printf("Updating users...\n");
                update_user_status(users, *user_count);
                break;
            
            case 3:
                printf("Creating hotel room...\n");
                create_room(rooms, room_count);
                break;

            case 4:
                printf("Updating hotel room...\n");
                update_room(rooms, *room_count);
                break;

            case 5:
                printf("Removing hotel room...\n");
                remove_room(rooms, room_count);
                break;

            case 6:
                printf("Logging out...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

    } while (choice != 6);
}

// Admin Function to register users
void register_user(User users[], int *user_count) 
{
    User new_user;
    FILE *file = fopen("login.txt", "a");

    if (file == NULL)  // Error checking just in case file does not exist
    {
        printf("Error: Could not open file login.txt\n");
        exit(1);
    }

    int valid_usertype = 0;

    while (!valid_usertype)  // loop to prevent admin from typing invalid user type
    {
        printf("Enter type of user to register (admin/staff/customer/guest): ");
        fgets(new_user.usertype, sizeof(new_user.usertype), stdin);
        new_user.usertype[strcspn(new_user.usertype, "\n")] = '\0';  // Remove newline '\n' character

        if (strcmp(new_user.usertype, "admin") == 0 || strcmp(new_user.usertype, "staff") == 0 || strcmp(new_user.usertype, "customer") == 0 || strcmp(new_user.usertype, "guest") == 0) 
        {
            valid_usertype = 1;
            strcpy(new_user.status, "active");
        } 
        else 
        {
            printf("Invalid user type. Please enter 'admin', 'staff', 'customer', or 'guest'.\n");
        }
    }
    
    printf("Enter username: ");
    fgets(new_user.username, sizeof(new_user.username), stdin);
    new_user.username[strcspn(new_user.username, "\n")] = '\0';  

    printf("Enter password: ");
    fgets(new_user.password, sizeof(new_user.password), stdin);
    new_user.password[strcspn(new_user.password, "\n")] = '\0';  

    fprintf(file, "%s:%s:%s:%s\n", new_user.usertype, new_user.username, new_user.password, new_user.status);
    fclose(file);

    printf("%s %s registered successfully.\n", new_user.usertype, new_user.username);
    users[*user_count] = new_user;  // Add the new user to the users array
    (*user_count)++;
}

// Admin Function to update user status (active/inactive)
void update_user_status(User users[], int user_count) 
{
    char username[20];
    char new_status[20];

    printf("Enter the username of the account to update: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    int user_found = 0;
    int valid_status = 0;

    for (int i = 0; i < user_count; i++) 
    {
        if (strcmp(users[i].username, username) == 0) 
        {
            user_found = 1;
            while (!valid_status) // loop to prevent admin from typing wrong status
            {
                printf("Enter new status (active/inactive): ");
                fgets(new_status, sizeof(new_status), stdin);
                new_status[strcspn(new_status, "\n")] = '\0';

                if (strcmp(new_status, "active") == 0 || strcmp(new_status, "inactive") == 0 )
                {
                    valid_status = 1;
                    strcpy(users[i].status, new_status);
                
                }
                else
                {
                    printf("Invalid status. Please enter 'active' or 'inactive'.\n");
                }
            }
            break;
        }
    }

    if (!user_found) 
    {
        printf("User not found.\n");
    } 
    else 
    {
        FILE *file = fopen("login.txt", "w");
        if (file == NULL) 
        {
            printf("Error: Could not open file login.txt\n");
            exit(1);
        }

        for (int i = 0; i < user_count; i++) 
        {
            fprintf(file, "%s:%s:%s:%s\n", users[i].usertype, users[i].username, users[i].password, users[i].status);
        }

        fclose(file);
        printf("User status updated successfully.\n");
    }
}

// Function to save updated room[] array to txt file
void write_rooms(Room rooms[], int room_count) 
{
    FILE *file = fopen("rooms.txt", "w");

    if (file == NULL) 
    {
        printf("Error: Could not open file rooms.txt\n");
        exit(1);
    }

    for (int i = 0; i < room_count; i++) 
    {
        fprintf(file, "%d:%d:%s:%s\n", rooms[i].roomNum, rooms[i].roomFloor, rooms[i].roomType, rooms[i].roomStatus);
    }

    fclose(file);
}

// Admin Function to create hotel rooms
void create_room(Room rooms[], int *room_count) 
{
    Room new_room;

    printf("Enter room number: ");
    scanf("%d", &new_room.roomNum);
    getchar();  // gets rid \n 

    // Check if room number already exists
    for (int i = 0; i < *room_count; i++) 
    {
        if (rooms[i].roomNum == new_room.roomNum) 
        {
            printf("Room number %d already exists. Please enter a different room number.\n", new_room.roomNum);
            return;  // Exit the function
        }
    }

    int valid_room = 0;

    while (!valid_room)
    {
        printf("Enter room floor: ");
        scanf("%d", &new_room.roomFloor);
        getchar();  // gets rid \n

        if (new_room.roomFloor >= 1 && new_room.roomFloor <= 10)
        {
            valid_room = 1;
        }
        else
        {
            printf("Invalid room floor. Please enter floor within 1-10.\n");
        }
    }

    int valid_type = 0;

    while (!valid_type)
    {
        printf("Enter room type: ");
        fgets(new_room.roomType, sizeof(new_room.roomType), stdin);
        new_room.roomType[strcspn(new_room.roomType, "\n")] = '\0';

        if (strcmp(new_room.roomType, "single") == 0 || strcmp(new_room.roomType, "double") == 0 || strcmp(new_room.roomType, "suite") == 0)
        {
            valid_type = 1;
        }
        else
        {
            printf("Invalid room type. Please enter 'single', 'double', or 'suite'.\n");
        }
    }

    strcpy(new_room.roomStatus, "available");

    rooms[*room_count] = new_room;
    (*room_count)++;

    write_rooms(rooms, *room_count);
    printf("Room created successfully.\n", new_room.roomNum);
}

// Admin Function to update hotel rooms details (roomNum, roomFloor, roomType, roomStatus) if entered wrongly
void update_room(Room rooms[], int room_count) 
{
    int roomNum;
    char new_roomType[20];
    char new_roomStatus[20];

    printf("Enter room number to update: ");
    scanf("%d", &roomNum);
    getchar();  // get rid \n

    int room_found = 0;
    for (int i = 0; i < room_count; i++) // Check if room number exists
    {
        if (rooms[i].roomNum == roomNum) 
        {
            room_found = 1;

            printf("\n--- Current room details ---");
            printf("\n1. Room Number: %d", rooms[i].roomNum);
            printf("\n2. Room Floor: %d", rooms[i].roomFloor);
            printf("\n3. Room Type: %s", rooms[i].roomType);
            printf("\n4. Room Status: %s\n", rooms[i].roomStatus);

            int valid_room = 0;

            while (!valid_room)
            {
                printf("Enter room floor: ");
                scanf("%d", &rooms[i].roomFloor);
                getchar();  // gets rid \n

                if (rooms[i].roomFloor >= 1 && rooms[i].roomFloor <= 10)
                {
                    valid_room = 1;
                }
                else
                {
                    printf("Invalid room floor. Please enter floor within 1-10.\n");
                }
            }

            int valid_type = 0;

            while (!valid_type)
            {
                printf("Enter room type: ");
                fgets(new_roomType, sizeof(new_roomType), stdin);
                new_roomType[strcspn(new_roomType, "\n")] = '\0';

                if (strcmp(new_roomType, "single") == 0 || strcmp(new_roomType, "double") == 0 || strcmp(new_roomType, "suite") == 0)
                {
                    valid_type = 1;
                    strcpy(rooms[i].roomType, new_roomType); 
                }
                else
                {
                    printf("Invalid room type. Please enter 'single', 'double', or 'suite'.\n");
                }
            }

            int valid_status = 0;

            while (!valid_status)
            {
                printf("Enter new room status (available/booked): ");
                fgets(new_roomStatus, sizeof(new_roomStatus), stdin);
                new_roomStatus[strcspn(new_roomStatus, "\n")] = '\0';

                if (strcmp(new_roomStatus, "available") == 0 || strcmp(new_roomType, "booked") == 0)
                {
                    valid_status = 1;
                    strcpy(rooms[i].roomStatus, new_roomStatus); 
                }
                else
                {
                    printf("Invalid room type. Please enter 'available', or 'booked'.\n");
                }
            }

            write_rooms(rooms, room_count);
            printf("Room updated successfully.\n");
            break;
        }
    }

    if (!room_found) 
    {
        printf("Room not found. Please enter a valid room number.\n");
    }
}

// Admin Function to remove hotel rooms
void remove_room(Room rooms[], int *room_count) 
{
    int roomNum;

    printf("Enter room number to remove: ");
    scanf("%d", &roomNum);
    getchar();  // Consume newline

    int room_found = 0;
    for (int i = 0; i < *room_count; i++) 
    {
        if (rooms[i].roomNum == roomNum) // check if entered room number matches any existing room number in array rooms[] 
        {
            room_found = 1;

            for (int j = i; j < *room_count - 1; j++) // j < *room_count - 1 is used to move elements the array left and the last element in array will be temporarily duplicated 
            {
                rooms[j] = rooms[j + 1];
            }
            (*room_count)--; // by decreasing the size of array, this removes the duplicated element which is the last element in the array
            write_rooms(rooms, *room_count);
            printf("Room removed successfully.\n");
            break;
        }
    }

    if (!room_found) 
    {
        printf("Room not found.\n");
    }
}

// Staff Function menu
void staff_menu(Room rooms[], int *room_count, Reservation reservations[], int *reservation_count, Booking bookings[], int *booking_count) 
{
    int choice;

    do {
        printf("\n--- Staff Menu ---\n");
        printf("1. Manage Pending Reservations\n");
        printf("2. Manage Bookings\n");
        printf("3. View Room Availability\n");
        printf("4. View Booking History\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar(); 

        switch (choice) 
        {
            case 1:
                view_pending_reservations_and_manage(rooms, room_count, reservations, reservation_count, bookings, booking_count);
                break;

            case 2:
                view_bookings_and_manage(rooms, room_count, bookings, booking_count);
                break;

            case 3:
                view_room_availability(rooms, room_count);
                break;

            case 4:
                view_booking_history(bookings, booking_count);
                break;

            case 5:
                printf("Logging out...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

    } while (choice != 5);
}

// Staff Function to view pending reservation of customers
void view_pending_reservations_and_manage(Room rooms[], int *room_count, Reservation reservations[], int *reservation_count, Booking bookings[], int *booking_count) 
{
    printf("%-3s %-12s %-10s %-10s %-20s %-15s\n", "No.", "Room Number", "Floor", "Type", "Customer Username", "Status"); 
    printf("-----------------------------------------------------------------------\n");

    int display_number = 1; // Sequential number for displaying reservations
    int actual_indices[*reservation_count]; // To map display index to actual reservation index

    for (int i = 0; i < *reservation_count; i++)  // View reservations that haven't been approved or rejected
    {
        if (strcmp(reservations[i].reservationStatus, "processing") == 0) 
        {
            printf("%-3d %-12d %-10d %-10s %-20s %-15s\n", 
                   display_number, reservations[i].roomNum, reservations[i].roomFloor, 
                   reservations[i].roomType, reservations[i].customerUsername, 
                   reservations[i].reservationStatus);
            actual_indices[display_number - 1] = i; // Store the actual reservation index
            display_number++;
        }
    }

    if (display_number == 1) 
    {
        printf("No pending reservations.\n");
        return;
    }

    int choice;
    printf("Enter the number of the reservation to manage (or 0 to exit): ");
    scanf("%d", &choice);
    getchar(); 

    if (choice == 0) 
    {
        return;
    }

    if (choice < 1 || choice >= display_number) // Error checking
    {
        printf("Invalid choice or reservation is not pending.\n");
        return;
    }

    int actual_index = actual_indices[choice - 1]; // Get the actual reservation index
    int roomNum = reservations[actual_index].roomNum; // Room Selected from entered number

    char action;
    printf("Do you want to approve (a) or reject (r) this reservation? "); 
    scanf("%c", &action);
    getchar(); 

    if (action == 'a') 
    {
        for (int l = 0; l < *room_count; l++) // Check availability of room
        {
            if (rooms[l].roomNum == roomNum && strcmp(rooms[l].roomStatus, "unavailable") == 0) 
            {
                printf("Room number %d is unavailable. Unable to approve reservation.\n", roomNum);
                return;
            }
        }

        strcpy(reservations[actual_index].reservationStatus, "approved"); // change reservation status from "processing" to "approved"

        strcpy(bookings[*booking_count].customerUsername, reservations[actual_index].customerUsername); // copy reservations details to bookings array
        bookings[*booking_count].roomNum = reservations[actual_index].roomNum;
        bookings[*booking_count].roomFloor = reservations[actual_index].roomFloor;
        strcpy(bookings[*booking_count].roomType, reservations[actual_index].roomType);
        strcpy(bookings[*booking_count].checkinDate, "Incomplete"); 
        strcpy(bookings[*booking_count].checkoutDate, "Incomplete"); 
        (*booking_count)++;

        FILE *fileBooking = fopen("bookings.txt", "w"); // rewrite updated booking information to bookings.txt
        if (fileBooking == NULL) 
        {
            printf("Error: Could not open file bookings.txt\n");
            exit(1);
        }

        for (int j = 0; j < *booking_count; j++) 
        {
            fprintf(fileBooking, "%d:%d:%s:%s:%s:%s\n", bookings[j].roomNum, bookings[j].roomFloor,
                    bookings[j].roomType, bookings[j].customerUsername,
                    bookings[j].checkinDate, bookings[j].checkoutDate);
        }

        fclose(fileBooking);

        FILE *fileReserve = fopen("reservations.txt", "w"); // rewrite updated reservation information to reservations.txt
        if (fileReserve == NULL) 
        {
            printf("Error: Could not open file reservations.txt\n");
            exit(1);
        }

        for (int k = 0; k < *reservation_count; k++) 
        {
            fprintf(fileReserve, "%d:%d:%s:%s:%s\n", reservations[k].roomNum, reservations[k].roomFloor, 
            reservations[k].roomType, reservations[k].customerUsername, reservations[k].reservationStatus);
        }

        fclose(fileReserve);

        for (int l = 0; l < *room_count; l++)  // Change approved room status from "available" to "unavailable" 
        {
            if (rooms[l].roomNum == roomNum) {
                strcpy(rooms[l].roomStatus, "unavailable");
                break;
            }
        }

        FILE *fileRoom = fopen("rooms.txt", "w"); // rewrite updated room information to rooms.txt
        if (fileRoom == NULL) {
            printf("Error: Could not open file rooms.txt\n");
            exit(1);
        }

        for (int m = 0; m < *room_count; m++) 
        {
            fprintf(fileRoom, "%d:%d:%s:%s\n", rooms[m].roomNum, rooms[m].roomFloor, rooms[m].roomType, rooms[m].roomStatus);
        }

        fclose(fileRoom);

        printf("Reservation approved for room number %d.\n", roomNum);

    } 
    else if (action == 'r') 
    {
        strcpy(reservations[actual_index].reservationStatus, "rejected"); // change reservation status from "processing" to "rejected"

        FILE *fileReserve = fopen("reservations.txt", "w"); // rewrite updated reservation information to reservations.txt
        if (fileReserve == NULL) 
        {
            printf("Error: Could not open file reservations.txt\n");
            exit(1);
        }

        for (int k = 0; k < *reservation_count; k++) 
        {
            fprintf(fileReserve, "%d:%d:%s:%s:%s\n", reservations[k].roomNum, reservations[k].roomFloor, 
            reservations[k].roomType, reservations[k].customerUsername, reservations[k].reservationStatus);
        }

        fclose(fileReserve);

        printf("Reservation rejected for room number %d.\n", roomNum);
    } 
    else 
    {
        printf("Invalid action. Please choose 'a' to approve or 'r' to reject.\n");
    }
}

// Staff Function to check-in or check-out customer
void view_bookings_and_manage(Room rooms[], int *room_count, Booking bookings[], int *booking_count) 
{
    printf("%-3s %-12s %-10s %-10s %-20s %-20s %-20s\n", "No.", "Room Number", "Floor", "Type", "Customer Username", "Check-in Date", "Check-out Date"); 
    printf("-----------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < *booking_count; i++) 
    {
        printf("%-3d %-12d %-10d %-10s %-20s %-20s %-20s\n", 
               (i + 1), bookings[i].roomNum, bookings[i].roomFloor, 
               bookings[i].roomType, bookings[i].customerUsername, 
               bookings[i].checkinDate, bookings[i].checkoutDate);
    }

    int choice;
    printf("Enter the number of the booking to manage (or 0 to exit): ");
    scanf("%d", &choice);
    getchar(); // Consume newline character

    if (choice == 0) {
        return;
    }

    if (choice < 1 || choice > *booking_count) 
    {
        printf("Invalid choice.\n");
        return;
    }

    int roomNum = bookings[choice - 1].roomNum;

    char action;
    printf("Do you want to check in (i) or check out (o) this booking? ");
    scanf("%c", &action);
    getchar(); // Consume newline character

    if (action == 'i') 
    {
        if (strcmp(bookings[choice - 1].checkinDate, "Incomplete") != 0) {
            printf("Guest already checked in.\n");
            return;
        }

        char checkinDate[20];
        printf("Enter check-in date (YYYY-MM-DD): ");
        fgets(checkinDate, sizeof(checkinDate), stdin);
        checkinDate[strcspn(checkinDate, "\n")] = '\0'; // Remove trailing newline

        strcpy(bookings[choice - 1].checkinDate, checkinDate);

        // Write updated bookings to file
        FILE *fileBooking = fopen("bookings.txt", "w");
        if (fileBooking == NULL) 
        {
            printf("Error: Could not open file bookings.txt\n");
            exit(1);
        }

        for (int j = 0; j < *booking_count; j++) 
        {
            fprintf(fileBooking, "%d:%d:%s:%s:%s:%s\n", bookings[j].roomNum, bookings[j].roomFloor,
                    bookings[j].roomType, bookings[j].customerUsername,
                    bookings[j].checkinDate, bookings[j].checkoutDate);
        }

        fclose(fileBooking);

        printf("Guest checked in to room number %d on %s.\n", roomNum, checkinDate);

    } 
    else if (action == 'o') 
    {
        if (strcmp(bookings[choice - 1].checkinDate, "Incomplete") == 0) {
            printf("Guest has not checked in yet.\n");
            return;
        }

        if (strcmp(bookings[choice - 1].checkoutDate, "Incomplete") != 0) {
            printf("Guest already checked out.\n");
            return;
        }

        char checkoutDate[20];
        printf("Enter check-out date (YYYY-MM-DD): ");
        fgets(checkoutDate, sizeof(checkoutDate), stdin);
        checkoutDate[strcspn(checkoutDate, "\n")] = '\0'; // Remove trailing newline

        strcpy(bookings[choice - 1].checkoutDate, checkoutDate);

        FILE *fileBooking = fopen("bookings.txt", "w"); // Write updated bookings to file
        if (fileBooking == NULL) 
        {
            printf("Error: Could not open file bookings.txt\n");
            exit(1);
        }

        for (int j = 0; j < *booking_count; j++) 
        {
            fprintf(fileBooking, "%d:%d:%s:%s:%s:%s\n", bookings[j].roomNum, bookings[j].roomFloor,
                    bookings[j].roomType, bookings[j].customerUsername,
                    bookings[j].checkinDate, bookings[j].checkoutDate);
        }

        fclose(fileBooking);

        for (int k = 0; k < *room_count; k++)  // Change approved room status from "available" to "unavailable" 
        {
            if (rooms[k].roomNum == roomNum) 
            {
                strcpy(rooms[k].roomStatus, "available");
                break;
            }
        }

        FILE *fileRoom = fopen("rooms.txt", "w"); // rewrite updated room information to rooms.txt
        if (fileRoom == NULL) 
        {
            printf("Error: Could not open file rooms.txt\n");
            exit(1);
        }

        for (int l = 0; l < *room_count; l++) 
        {
            fprintf(fileRoom, "%d:%d:%s:%s\n", rooms[l].roomNum, rooms[l].roomFloor, rooms[l].roomType, rooms[l].roomStatus);
        }

        fclose(fileRoom);

        printf("Guest checked out from room number %d on %s.\n", roomNum, checkoutDate);
        printf("Room %d has been made available.\n", roomNum);

    } 
    else 
    {
        printf("Invalid action. Please choose 'i' to check in or 'o' to check out.\n");
    }
}

// Staff Function to view room availability
void view_room_availability(Room rooms[], int *room_count) 
{
    printf("%-3s %-12s %-10s %-10s %-15s\n", "No.", "Room Number", "Floor", "Type", "Status"); 
    printf("------------------------------------------------------------\n");
    
    for (int i = 0; i < *room_count; i++) 
    {
        printf("%-3d %-12d %-10d %-10s %-15s\n", 
               (i + 1), rooms[i].roomNum, rooms[i].roomFloor, 
               rooms[i].roomType, rooms[i].roomStatus);
    }
}

// Staff Function to view all bookings
void view_booking_history(Booking bookings[], int *booking_count) 
{
    printf("%-3s %-12s %-10s %-10s %-20s %-20s %-20s\n", "No.", "Room Number", "Floor", "Type", "Customer Username", "Check-in Date", "Check-out Date"); 
    printf("---------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < *booking_count; i++) 
    {
        printf("%-3d %-12d %-10d %-10s %-20s %-20s %-20s\n", 
               (i + 1), bookings[i].roomNum, bookings[i].roomFloor, 
               bookings[i].roomType, bookings[i].customerUsername, 
               bookings[i].checkinDate, bookings[i].checkoutDate);
    }
}

// Customer Menu
void customer_menu(char username[20] ,Room rooms[], int *room_count, Reservation reservations[], int *reservation_count, Booking bookings[], int *booking_count) 
{
    int choice;

    do {
        printf("\n--- Customer Menu ---\n");
        printf("1. Search for Available Rooms\n");
        printf("2. View Reservations History\n");
        printf("3. View Current bookings\n");
        printf("4. Make Reservations\n");
        printf("5. Cancel Reservations\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar(); 

        switch (choice) 
        {
            case 1:
                view_available_rooms(rooms, room_count);
                break;

            case 2:
                view_reservation_history(username, reservations, reservation_count);
                break;

            case 3:
                view_current_bookings(username, bookings, booking_count);
                break;

            case 4:
                make_reservation(username, rooms, room_count, reservations, reservation_count);
                break;

            case 5:
                cancel_reservation(username, rooms, room_count, reservations, reservation_count);
                break;

            case 6:
                printf("Logging out...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

    } while (choice != 6);
}

// Customer Function to view available rooms
void view_available_rooms(Room rooms[], int *room_count) 
{
    printf("%-3s %-12s %-10s %-10s\n", "No.", "Room Number", "Floor", "Type"); 
    printf("----------------------------------\n");
    
    for (int i = 0; i < *room_count; i++) // Loop through array and display available rooms
    {
        if (strcmp(rooms[i].roomStatus, "available") == 0)
        {
            printf("%-3d %-12d %-10d %-10s\n", (i + 1), rooms[i].roomNum, rooms[i].roomFloor, rooms[i].roomType);
        }
    }
}

// Customer Function to make reservations
void make_reservation(char username[20], Room rooms[], int *room_count, Reservation reservations[], int *reservation_count)
{
    int roomNum;
    printf("Enter the room number you want to reserve: ");
    scanf("%d", &roomNum);
    getchar();

    for (int i = 0; i < *room_count; i++) 
    {
        if (rooms[i].roomNum == roomNum) 
        {
            if (strcmp(rooms[i].roomStatus, "available") == 0) // Check if room is available
            {
                reservations[*reservation_count].roomNum = rooms[i].roomNum; // Add reservation
                reservations[*reservation_count].roomFloor = rooms[i].roomFloor;
                strcpy(reservations[*reservation_count].roomType, rooms[i].roomType);
                strcpy(reservations[*reservation_count].customerUsername, username);
                strcpy(reservations[*reservation_count].reservationStatus, "processing");
                (*reservation_count)++;

                FILE *fileReserve = fopen("reservations.txt", "w"); // Write updated reservations to file
                if (fileReserve == NULL) 
                {
                    printf("Error: Could not open file reservations.txt\n");
                    exit(1);
                }

                for (int i = 0; i < *reservation_count; i++) 
                {
                    fprintf(fileReserve, "%d:%d:%s:%s:%s\n", reservations[i].roomNum, reservations[i].roomFloor, 
                    reservations[i].roomType, reservations[i].customerUsername, reservations[i].reservationStatus);
                }

                fclose(fileReserve);

                printf("You have made a reservation for room number %d.\n", rooms[i].roomNum);
                return;
            } 
            else 
            {
                printf("Room number %d is unavailable. Reservation cannot be made.\n", roomNum);
                return;
            }
        }
    }
    printf("Invalid room number.\n");
}

//view current booking
void view_current_bookings(char username[], Booking bookings[], int *booking_count) {
    FILE *fileBooking = fopen("bookings.txt", "r"); // Open file for reading
    if (fileBooking == NULL) {
        printf("Error: Could not open file bookings.txt\n");
        return;
    }

    char line[200];
    int found = 0;

    while (fgets(line, sizeof(line), fileBooking)) {
        Booking booking;
        sscanf(line, "%d:%d:%19[^:]:%19[^:]:%19[^:]:%19s",
               &booking.roomNum,
               &booking.roomFloor,
               booking.roomType,
               booking.customerUsername,
               booking.checkinDate,
               booking.checkoutDate);

        if (strcmp(booking.customerUsername, username) == 0) 
        {
            printf("Room Number: %d\n", booking.roomNum);
            printf("Room Floor: %d\n", booking.roomFloor);
            printf("Room Type: %s\n", booking.roomType);
            printf("Check-in Date: %s\n", booking.checkinDate);
            printf("Check-out Date: %s\n", booking.checkoutDate);
            
            found = 1;
        }
    }

    if (!found) {
        printf("No current bookings found for user %s.\n", username);
    }

    fclose(fileBooking);
}

//view reservation history
void view_reservation_history(char username[20], Reservation reservations[], int *reservation_count) 
{
    FILE *fileReserve = fopen("reservations.txt", "r"); // Open file for reading
    if (fileReserve == NULL) {
        printf("Error: Could not open file reservations.txt\n");
        return;
    }

    char line[100];
    int found = 0;

    while (fgets(line, sizeof(line), fileReserve)) 
    {
        Reservation reservation;
        sscanf(line, "%d:%d:%19[^:]:%19[^:]:%19s",
               &reservation.roomNum,
               &reservation.roomFloor,
               reservation.roomType,
               reservation.customerUsername,
               reservation.reservationStatus);

        if (strcmp(reservation.customerUsername, username) == 0) 
        {
            printf("\n------------------------------\n");
            printf("Room Number: %d\n", reservation.roomNum);
            printf("Room Floor: %d\n", reservation.roomFloor);
            printf("Room Type: %s\n", reservation.roomType);
            printf("Reservation Status: %s\n", reservation.reservationStatus);
            
            found = 1;
        }
    }

    if (!found) 
    {
        printf("No reservations found for user %s.\n", username);
    }

    fclose(fileReserve);
}

//cancel reservation
void cancel_reservation(char username[20], Room rooms[], int *room_count, Reservation reservations[], int *reservation_count) 
{   
    printf("%-3s %-12s %-10s %-10s %-20s\n", "No.", "Room Number", "Floor", "Type", "Status"); 
    printf("-----------------------------------------------------------------------\n");
    
    int processing_reservation_found = 0;
    int customer_reservations = 0;
    for (int i = 0; i < *reservation_count; i++)  // View reservations that haven't been approved or rejected
    {
        if (strcmp(reservations[i].customerUsername, username) == 0 && strcmp(reservations[i].reservationStatus, "processing") == 0) 
        {
            processing_reservation_found = 1;
            customer_reservations++;
            printf("%-3d %-12d %-10d %-10s %-20s\n", (customer_reservations), reservations[i].roomNum, reservations[i].roomFloor, reservations[i].roomType, reservations[i].reservationStatus);
        }
    }

    if (!processing_reservation_found) {
        printf("No processing reservations found for user %s.\n", username);
        return;
    }

    int roomNum;
    printf("Enter the room number of the reservation to cancel (0 to exit): ");
    scanf("%d", &roomNum);
    getchar(); 

    if (roomNum == 0) 
    {
        printf("Operation canceled by the user.\n");
        return;
    }

    for (int i = 0; i < *reservation_count; i++) 
    {
        if (reservations[i].roomNum == roomNum && strcmp(reservations[i].customerUsername, username) == 0 && strcmp(reservations[i].reservationStatus, "processing") == 0) 
        {
            for (int j = i; j < *reservation_count - 1; j++) // Shift all reservations after the one to be removed to the left
            {
                reservations[j] = reservations[j + 1];
            }
            (*reservation_count)--;

            FILE *fileReserve = fopen("reservations.txt", "w"); // Write updated reservations to file
            if (fileReserve == NULL) 
            {
                printf("Error: Could not open file reservations.txt\n");
                exit(1);
            }

            for (int i = 0; i < *reservation_count; i++) 
            {
                fprintf(fileReserve, "%d:%d:%s:%s:%s\n", reservations[i].roomNum, reservations[i].roomFloor, 
                reservations[i].roomType, reservations[i].customerUsername, reservations[i].reservationStatus);
            }

            fclose(fileReserve);

            printf("Reservation for room number %d has been canceled.\n", roomNum);
            return;
        }
    }
    printf("Processing reservation for room number %d not found.\n", roomNum);
}

//Guest Menu 
void guest_menu(Room rooms[], int *room_count) 
{
    int choice;
    char choice_input[10];

    do {
        printf("\n--- Guest Menu ---");
        printf("\n1. View available rooms\n2. search for room details\n3. Exit\nEnter a choice: ");
        fgets(choice_input, sizeof(choice_input), stdin);
        choice = atoi(choice_input);

        switch (choice) {
            case 1:
                view_available_rooms(rooms, room_count);
                break;
            case 2:
                printf("----Room Details----\nRoomType   Price    Rates\nsingle     RM250    4.2/5.0\ndouble     RM300    4.3/5.0\nsuite      RM450    4.5/5.0\n");
                break;
            case 3:
                printf("Exiting guest menu.\n");
                break;
            default:
                printf("Invalid choice. Please enter 1 or 2 or 3\n");
                break;
        }
    } while (choice != 3);
}
