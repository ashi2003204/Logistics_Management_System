#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_CITIES 30
#define MAX_CITY_NAME 50
#define MAX_DELIVERIES 50
#define INF 1000000000


const char *VEHICLE_NAMES[3] = {"Van", "Truck", "Lorry"};
const int VEHICLE_CAPACITY[3] = {1000, 5000, 10000};
const double VEHICLE_RATE[3] = {30, 40, 80};
const double VEHICLE_SPEED[3] = {60, 50, 45};
const double VEHICLE_EFFICIENCY[3] = {12, 6, 4};
const double FUEL_PRICE = 310.0;


typedef struct {
    int source;
    int destination;
    int weight;
    int vehicleType;
    double distance;
    double cost;
    double fuelUsed;
    double fuelCost;
    double operationalCost;
    double profit;
    double customerCharge;
    double time;
} Delivery;


char cities[MAX_CITIES][MAX_CITY_NAME];
int distanceMatrix[MAX_CITIES][MAX_CITIES];
int cityCount = 0;
Delivery deliveries[MAX_DELIVERIES];
int deliveryCount = 0;


void add_city();
void show_cities();
void manage_distance();
void show_distance_table();
void add_delivery();
void show_reports();
void save_data();
void load_data();

int main() {
    int choice;

    load_data();

    while (1) {
        printf("\n==== Logistics Management System ====\n");
        printf("1. Add City\n");
        printf("2. Show Cities\n");
        printf("3. Manage Distances\n");
        printf("4. Show Distance Table\n");
        printf("5. Add Delivery\n");
        printf("6. View Reports\n");
        printf("7. Save & Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: add_city(); break;
            case 2: show_cities(); break;
            case 3: manage_distance(); break;
            case 4: show_distance_table(); break;
            case 5: add_delivery(); break;
            case 6: show_reports(); break;
            case 7:
                save_data();
                printf("Data saved successfully. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}


void add_city() {
    if (cityCount >= MAX_CITIES) {
        printf("City limit reached.\n");
        return;
    }
    printf("Enter city name: ");
    fgets(cities[cityCount], MAX_CITY_NAME, stdin);
    cities[cityCount][strcspn(cities[cityCount], "\n")] =
