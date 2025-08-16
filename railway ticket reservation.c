#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_TICKETS 100
#define MAX_TRAINS 50
#define DATA_FILE "railway_data.dat"


struct Ticket {
    int ticketId;
    int trainNumber;
    int seatNumber;
    char passengerName[50];
};


struct TrainRoute {
    int trainNumber;
    char source[50];
    char destination[50];
    int availableSeats;
};

struct Ticket tickets[MAX_TICKETS];
struct TrainRoute trains[MAX_TRAINS];


int ticketCount = 0;
int trainCount = 0;


void displayMenu();
void initializeTrains();
void displayTrainRoutes();
void bookTicket();
void cancelTicket();
void searchTicket();
void saveData();
void loadData();



void displayMenu() {
    printf("--- Railway Ticket Reservation System ---\n");
    printf("1. Display Train Routes\n");
    printf("2. Book Ticket\n");
    printf("3. Cancel Ticket\n");
    printf("4. Search Ticket\n");
    printf("5. Exit\n");
    printf("------------------------------------------\n");

}

int main() {
    int choice;
    initializeTrains();
    loadData();

    while (1) {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayTrainRoutes();
                break;
            case 2:
                bookTicket();
                break;
            case 3:
                cancelTicket();
                break;
            case 4:
                searchTicket();
                break;
            case 5:
                saveData();
                printf("Thank you for choosing our system.\n");
                exit(0);
            default:
                printf("Invalid choice. You have to choose from 1 to 5. Please try again.\n");
        }
        printf("\n");
    }

    return 0;
}

void saveData() {
    FILE *file = fopen(DATA_FILE, "wb");
    if (file == NULL) {
        printf("Error: Could not save data to file.\n");
        return;
    }

    fwrite(&ticketCount, sizeof(int), 1, file);
    fwrite(&trainCount, sizeof(int), 1, file);

    fwrite(tickets, sizeof(struct Ticket), ticketCount, file);
    fwrite(trains, sizeof(struct TrainRoute), trainCount, file);

    fclose(file);
    printf("Data saved successfully.\n");
}

void loadData() {
    FILE *file = fopen(DATA_FILE, "rb");
    if (file == NULL) {
        printf("No existing data file found. Starting with initial data.\n");
        return;
    }

    fread(&ticketCount, sizeof(int), 1, file);
    fread(&trainCount, sizeof(int), 1, file);

    fread(tickets, sizeof(struct Ticket), ticketCount, file);
    fread(trains, sizeof(struct TrainRoute), trainCount, file);

    fclose(file);
    printf("Data loaded successfully.\n");
}

void initializeTrains() {
    if (trainCount < MAX_TRAINS) {
        trains[trainCount].trainNumber = 101;
        strcpy(trains[trainCount].source, "MIRPUR");
        strcpy(trains[trainCount].destination, "DAFFODIL");
        trains[trainCount].availableSeats = 50;
        trainCount++;
    }
    if (trainCount < MAX_TRAINS) {
        trains[trainCount].trainNumber = 102;
        strcpy(trains[trainCount].source, "DHAKA");
        strcpy(trains[trainCount].destination, "CHANDPUR");
        trains[trainCount].availableSeats = 60;
        trainCount++;
    }
    if (trainCount < MAX_TRAINS) {
        trains[trainCount].trainNumber = 103;
        strcpy(trains[trainCount].source, "DHAKA");
        strcpy(trains[trainCount].destination, "CHITTAGONG");
        trains[trainCount].availableSeats = 45;
        trainCount++;
    }
}

void displayTrainRoutes() {
    if (trainCount == 0) {
        printf("No train routes available.\n");
        return;
    }
    printf("\n--- Available Train Routes ---\n");
    for (int i = 0; i < trainCount; i++) {
        printf("Train No: %d, From: %s, To: %s, Available Seats: %d\n",
               trains[i].trainNumber, trains[i].source, trains[i].destination,
               trains[i].availableSeats);
    }
    printf("------------------------------\n");
}


void bookTicket() {
    if (ticketCount >= MAX_TICKETS) {
        printf("Sorry, the system is full. Cannot book more tickets.\n");
        return;
    }

    int trainNum;
    char name[50];
    int seatNum;

    printf("Enter passenger name: ");
    scanf("%s", name);

    displayTrainRoutes();
    printf("Enter train number to book: ");
    scanf("%d", &trainNum);

    int trainIndex = -1;
    for (int i = 0; i < trainCount; i++) {
        if (trains[i].trainNumber == trainNum) {
            trainIndex = i;
            break;
        }
    }

    if (trainIndex == -1) {
        printf("Invalid train number.\n");
        return;
    }

    if (trains[trainIndex].availableSeats <= 0) {
        printf("Sorry, no seats are available on this train.\n");
        return;
    }


    tickets[ticketCount].ticketId = 1000 + ticketCount;
    strcpy(tickets[ticketCount].passengerName, name);
    tickets[ticketCount].trainNumber = trainNum;
    tickets[ticketCount].seatNumber = trains[trainIndex].availableSeats;


    trains[trainIndex].availableSeats--;
    ticketCount++;

    printf("Ticket booked successfully! Your ticket ID is %d and seat number is %d.\n", tickets[ticketCount - 1].ticketId, tickets[ticketCount - 1].seatNumber);
}

void cancelTicket() {
    int ticketId;
    printf("Enter ticket ID to cancel: ");
    scanf("%d", &ticketId);

    int ticketFound = 0;
    for (int i = 0; i < ticketCount; i++) {
        if (tickets[i].ticketId == ticketId) {
            for (int j = 0; j < trainCount; j++) {
                if (trains[j].trainNumber == tickets[i].trainNumber) {
                    trains[j].availableSeats++;
                    break;
                }
            }

            for (int k = i; k < ticketCount - 1; k++) {
                tickets[k] = tickets[k + 1];
            }
            ticketCount--;
            ticketFound = 1;
            printf("Ticket with ID %d has been canceled.\n", ticketId);
            break;
        }
    }

    if (!ticketFound) {
        printf("Ticket with ID %d not found.\n", ticketId);
    }
}

void searchTicket() {
    int ticketId;
    printf("Enter ticket ID to search: ");
    scanf("%d", &ticketId);

    int ticketFound = 0;
    for (int i = 0; i < ticketCount; i++) {
        if (tickets[i].ticketId == ticketId) {
            printf("\n--- Ticket Details ---\n");
            printf("Ticket ID: %d\n", tickets[i].ticketId);
            printf("Passenger Name: %s\n", tickets[i].passengerName);
            printf("Train Number: %d\n", tickets[i].trainNumber);
            printf("Seat Number: %d\n", tickets[i].seatNumber);
            printf("----------------------\n");
            ticketFound = 1;
            break;
        }
    }

    if (!ticketFound) {
        printf("Ticket with ID %d not found.\n", ticketId);
    }
}



