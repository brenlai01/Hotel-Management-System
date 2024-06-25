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
    char reserveStatus[20];
} Reservations;


// read_users function prototype
void read_users(User users[], int *user_count);

// read_rooms function prototype
void read_rooms(Room rooms[], int *room_count);

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

// admin_menu function prototype
void admin_menu(User users[], int *user_count, Room rooms[], int *room_count);

int main() 
{
    User users[MAX_USERS];  // Array to store user data
    Room rooms[MAX_ROOMS]; // Array to store room data
    int user_count = 0;  // Number of users read from login.txt
    int room_count = 0; // Number of rooms read from rooms.txt

    read_users(users, &user_count);
    read_rooms(rooms, &room_count);

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
                    //staff_menu();
                } 
                else if (strcmp(usertype, "customer") == 0)
                {
                    //customer_menu();
                } 
                else if (strcmp(usertype, "guest") == 0) 
                {
                    //guest_menu();
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
                //guest_menu();
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

