#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Roomavail {
    int roomnum;
    int roomavail;
};


void ManageRoomReservation() {
    char cusname[25];
    int cusroom, cusphone;
    
    printf("Welcome to room reservation system\n");

    printf("Enter name of customer: ");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);  // Clear input buffer
    fgets(cusname, sizeof(cusname), stdin);
    cusname[strcspn(cusname, "\n")] = 0;  // Remove newline character

    FILE *rafile = fopen("roomavailability.txt", "r");
    if (rafile == NULL) {
        perror("Error opening roomavailability.txt");
        return;
    }

    printf("Available rooms: ");
    char line[100];
    int rn, ra;     //Room number and Room availability
    int available_rooms = 0;
    while (fgets(line, sizeof(line), rafile)) {
        if (sscanf(line, "%d,%d", &rn, &ra) == 2 && ra == 1) {
            printf("%d| ", rn);
            available_rooms = 1;
        }
    }
    fclose(rafile);

    if (!available_rooms) {
        printf("No rooms available.\n");
        return;
    }

    printf("\nEnter room number to allot to customer: ");
    scanf("%d", &cusroom);

    printf("Enter customer's phone number: ");
    scanf("%d", &cusphone);

    FILE *fp = fopen("manageroomreservation.txt", "a");
    if (fp == NULL) {
        perror("Error opening manageroomreservation.txt");
        return;
    }

    fprintf(fp, "%d, %s, %d\n", cusroom, cusname, cusphone);
    printf("Record of %s created.\n", cusname);
    fclose(fp);

    // Update room availability
    struct Roomavail rooms[100];
    int num_rooms = 0;

    rafile = fopen("roomavailability.txt", "r");
    if (rafile == NULL) {
        perror("Error reopening roomavailability.txt");
        return;
    }
    while (fscanf(rafile, "%d,%d", &rooms[num_rooms].roomnum, &rooms[num_rooms].roomavail) == 2) {
        num_rooms++;
    }
    fclose(rafile);

    fp = fopen("roomavailability.txt", "w");
    if (fp == NULL) {
        perror("Error opening roomavailability.txt for writing");
        return;
    }
    for (int i = 0; i < num_rooms; i++) {
        if (rooms[i].roomnum == cusroom) {
            rooms[i].roomavail = 0;
        }
        fprintf(fp, "%d,%d\n", rooms[i].roomnum, rooms[i].roomavail);
    }
    fclose(fp);
}

void ViewRoomReservations() {
    printf("Current room reservations\n");
    printf("Room number\tName\t\t\tPhone\n");

    FILE *fp = fopen("manageroomreservation.txt", "r");
    if (fp == NULL) {
        perror("Error opening manageroomreservation.txt");
        return;
    }

    char line[100];
    int room;
    char name[25];
    int phone;
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d, %24[^,], %d", &room, name, &phone) == 3) {
            printf("%d\t\t%-24s\t%d\n", room, name, phone);
        }
    }
    fclose(fp);
}

void DeleteReservation() {
    int roomtodelete;
    printf("Enter the room number to delete: ");
    scanf("%d", &roomtodelete);

    FILE *fp = fopen("manageroomreservation.txt", "r");
    if (fp == NULL) {
        perror("Error opening manageroomreservation.txt");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        perror("Error opening temp.txt");
        fclose(fp);
        return;
    }

    char line[100];
    int room, phone;
    char name[25];
    int found = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d, %24[^,], %d", &room, name, &phone) == 3) {
            if (room == roomtodelete) {
                found = 1;
            } else {
                fprintf(temp, "%d, %s, %d\n", room, name, phone);
            }
        }
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove("manageroomreservation.txt");
        rename("temp.txt", "manageroomreservation.txt");
        printf("Reservation deleted successfully.\n");

        // Update room availability
        struct Roomavail rooms[100];
        int num_rooms = 0;

        fp = fopen("roomavailability.txt", "r");
        if (fp == NULL) {
            perror("Error opening roomavailability.txt");
            return;
        }
        while (fscanf(fp, "%d,%d", &rooms[num_rooms].roomnum, &rooms[num_rooms].roomavail) == 2) {
            num_rooms++;
        }
        fclose(fp);

        fp = fopen("roomavailability.txt", "w");
        if (fp == NULL) {
            perror("Error opening roomavailability.txt for writing");
            return;
        }
        for (int i = 0; i < num_rooms; i++) {
            if (rooms[i].roomnum == roomtodelete) {
                rooms[i].roomavail = 1;
            }
            fprintf(fp, "%d,%d\n", rooms[i].roomnum, rooms[i].roomavail);
        }
        fclose(fp);
    } else {
        remove("temp.txt");
        printf("Reservation for room %d not found.\n", roomtodelete);
    }
}

void checkinout()
{
    int roomnum;
    char checkin[15];
    char checkout[15];
    char viewcheckinout[1000];
    int choice;
    
    FILE * fp;

    printf("1. Check in and out\n2. View Check in and outs\nEnter choice: ");
    scanf("%d", &choice);
    if(choice == 1)
    {
        printf("Enter room number: ");
        scanf("%d", &roomnum);

        printf("Enter check in date (DD-MM-YYYY): ");
        scanf("%s", checkin);

        printf("Enter check out date (DD-MM-YYYY); ");
        scanf("%s", checkout);

        

        fp = fopen("checkinout.txt", "a");
        fprintf(fp, "%d,%s,%s\n",roomnum, checkin, checkout);

        fclose(fp);
    }

    else if(choice == 2)
    {
        int viewroomnum;
        char viewcheckin[15];
        char viewcheckout[15];

        fp = fopen("checkinout.txt", "r");

        printf("Room Num\tCheck-in\tCheck-out\n");
        
        while(fgets(viewcheckinout, 1000, fp) != NULL)
        {
            sscanf(viewcheckinout, "%d,%[^,],%[^,]", &viewroomnum,&viewcheckin,&viewcheckout);
            printf("%d\t\t%s\t%s\n",viewroomnum, viewcheckin, viewcheckout);
        }
        fclose(fp);
    }
}

void viewbookinghistory()
{
    FILE *fp;
    char bookinghistory[1000];
    char checkin[15];
    char checkout[15];
    int roomnum;

    fp = fopen("checkinout.txt", "r");

    if(fp == NULL)
    {
        printf("Error opening file.\n");
        return;
    }
    printf("Booking History: \n");
    while(fgets(bookinghistory, 1000, fp) != NULL)
    {
        sscanf(bookinghistory, "%d,%[^,],%[^,]", &roomnum, checkin, checkout);
        printf("Room number: %d\nCheck in:  %s\nCheck out: %s\n", roomnum, checkin, checkout);
    }
    fclose(fp);
}

void roomavailabilitystatus() {
    char roomavailfile[1000];
    int roomnum, roomavail;

    FILE *fp = fopen("roomavailability.txt", "r");
    if (fp == NULL) {
        printf("Could not open file\n");
        return;
    }

    printf("1. Check Room status\n2. Update Room status\nEnter choice: ");
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Room Number\tAvailability Status\n");

        while (fgets(roomavailfile, 1000, fp)!= NULL) {
            sscanf(roomavailfile, "%d,%d", &roomnum, &roomavail);
            printf("%d\t\t\t %d\n", roomnum, roomavail);
        }

        fclose(fp);
    } else if (choice == 2) {
        int update_roomnum, update_status;
        printf("Enter room number to update: ");
        scanf("%d", &update_roomnum);
        printf("Enter new availability status (0/1): ");
        scanf("%d", &update_status);

        rewind(fp); // Moves file pointer to the beginning of the file so don't have to close file

        int num_rooms = 0;
        while (fscanf(fp, "%d,%d", &roomnum, &roomavail) == 2) {
            num_rooms++;
        }

        rewind(fp); // Moves file pointer to the beginning of the file so don't have to close file

        struct Room {
            int roomnum;
            int roomavail;
        } rooms[num_rooms];

        for (int i = 0; i < num_rooms; i++) {
            fscanf(fp, "%d,%d", &rooms[i].roomnum, &rooms[i].roomavail);
        }

        fclose(fp); // Close the file before reopening in write mode

        fp = fopen("roomavailability.txt", "w"); // Open file in write mode to overwrite

        for (int i = 0; i < num_rooms; i++) {
            if (rooms[i].roomnum == update_roomnum) {
                rooms[i].roomavail = update_status;
            }
            fprintf(fp, "%d,%d\n", rooms[i].roomnum, rooms[i].roomavail);
        }

        fclose(fp); // Close the file after writing to it
    }
}

int main()
{
    int choice;
    struct Roomavail rooms[3];

    FILE *fp = fopen("roomavailability.txt", "r");
    if (fp == NULL) {
        // If file does not exist, create it with default values
        fp = fopen("roomavailability.txt", "w");
        if (fp == NULL) {
            printf("Could not open file\n");
            return 1;
        }
        fprintf(fp, "1,1\n2,0\n3,1\n");
        fclose(fp);
        fp = fopen("roomavailability.txt", "r");
    }

    for (int i = 0; i < 3; i++) {
        fscanf(fp, "%d,%d", &rooms[i].roomnum, &rooms[i].roomavail);
    }

    fclose(fp);

    while (1) 
    {
        printf("1. Manage room reservations \n2. View reservation list\n3. Delete reservations\n4. Check in and out\n5. View booking history\n6. View availability status\n7. Exit\nEnter choice: ");
        scanf("%d", &choice);

        if (choice == 1) 
        {
            ManageRoomReservation();
        } 
        else if (choice == 2) 
        {
            ViewRoomReservations();
        }
        else if (choice == 3) 
        {
            DeleteReservation();
        } 
        else if (choice == 4) 
        {
            checkinout();
        } 
        else if (choice == 5)
        {
            viewbookinghistory();
        } 
        else if (choice == 6) 
        {
            roomavailabilitystatus();
        } 
        else if (choice == 7) 
        {
            break;
        } 
        else
        {
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}