#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 100

typedef struct {
    char usertype[20];
    char username[20];
    char password[20];
} User;

// Function to read users' username, password, usertype from the file and store them in an array
void read_users(User users[], int *user_count) 
{
    FILE *file = fopen("login.txt", "r"); // Error checking just in case file does not exist

    if (file == NULL) {
        printf("Error: Could not open file %s\n", file);
        exit(1);
    }

    char line[MAX_LINE];
    *user_count = 0;  // start count of users in txt file

    while (fgets(line, sizeof(line), file)) 
    {
        line[strcspn(line, "\n")] = '\0';  // Remove newline '\n' character
        sscanf(line, "%[^:]:%[^:]:%s", users[*user_count].usertype, users[*user_count].username, users[*user_count].password);
        (*user_count)++;  // increase count by 1 after each line read
        
    }

    fclose(file);
}


// Function for users to login. User logs in ONLY if username and password matches
int login(User users[], int user_count, char *username, char *password, char *usertype) 
{
    for (int i = 0; i < user_count; i++) 
    {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            strcpy(usertype, users[i].usertype);
            return 1;
        }
    }
    return 0;
}

// Admin function to register users
void register_user() 
{
    User new_user;
    FILE *file = fopen("login.txt", "a");

    if (file == NULL) // Error checking just in case file does not exist
    {
        printf("Error: Could not open file login.txt\n");
        exit(1);
    }

    int valid_usertype = 0;

    while (!valid_usertype) // loop to prevent admin from typing invalid user type
    {
        printf("Enter type of user to register (staff/customer/guest): ");
        fgets(new_user.usertype, sizeof(new_user.usertype), stdin);
        new_user.usertype[strcspn(new_user.usertype, "\n")] = '\0'; // Remove newline '\n' character

        if (strcmp(new_user.usertype, "staff") == 0 || strcmp(new_user.usertype, "customer") == 0 || strcmp(new_user.usertype, "guest") == 0)
        {
            valid_usertype = 1;
        }
        else
        {
            printf("Invalid user type. Please enter 'staff', 'customer', or 'guest'.\n");
        }
    }
    
    printf("Enter username: ");
    fgets(new_user.username, sizeof(new_user.username), stdin);
    new_user.username[strcspn(new_user.username, "\n")] = '\0';  

    printf("Enter password: ");
    fgets(new_user.password, sizeof(new_user.password), stdin);
    new_user.password[strcspn(new_user.password, "\n")] = '\0';  

    fprintf(file, "%s:%s:%s\n", new_user.usertype, new_user.username, new_user.password);

    fclose(file);
    printf("%s %s registered successfully.\n", new_user.usertype, new_user.username);
}

// Admin functions menu
void admin_menu() 
{
    int choice;

    do {
        printf("\n--- Administrator Menu ---\n");
        printf("1. Register new user\n");
        printf("2. Manage existing users\n");
        printf("3. Manage hotel room\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        getchar();  // Remove newline character '\n' left in the input buffer

        switch (choice) 
        {
            case 1:
                printf("Registering a new user...\n");
                register_user();
                break;

            case 2:
                printf("Adding a new user...\n");
                break;

            case 3:
                printf("Removing a user...\n");
                break;

            case 4:
                printf("Logging out...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

    } while (choice != 4);
}

int main() 
{
    User users[100];// Array to store user data
    int user_count;// Number of users read from the file

    read_users(users, &user_count);

    char username[20];
    char password[20];
    char usertype[20];
    int logged_in = 0;

    while (!logged_in) 
    {
        int choice;
        char choice_input[10];

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
            
            if (login(users, user_count, username, password, usertype)) 
            {
                printf("\nLogin successful. Welcome %s %s\n", usertype, username);
                logged_in = 1;

                if (strcmp(usertype, "admin") == 0) 
                {
                    admin_menu();
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
                printf("\nLogin failed. Invalid username or password.\n");
            }

        } 

        else if (choice == 2) 
        {
            if (login(users, user_count, "guest1", "guestpass", usertype)) 
            {  
                printf("\nContinuing as guest.");
                logged_in = 1;
            } 

        }

        else if (choice == 3)
        {
            exit(1);
        }
         
        else 
        {
            printf("Invalid choice. Please enter 1 or 2.\n");
        }
    }

    return 0;
}
