#include <stdio.h>
#include <string.h>

#define MAX_CITIES 30
#define MAX_DELIVERIES 50
#define INF 1000000000


char cities[MAX_CITIES][50];
int distanceMatrix[MAX_CITIES][MAX_CITIES];
int cityCount = 0;


const char *vehicleNames[3] = {"Van", "Truck", "Lorry"};
const int vehicleCapacity[3] = {1000, 5000, 10000};
const int vehicleRate[3] = {30, 40, 80};
const int vehicleSpeed[3] = {60, 50, 45};
const int vehicleEfficiency[3] = {12, 6, 4};


typedef struct {
    int source;
    int destination;
    int vehicleType;
    double weight;
} Delivery;

Delivery deliveries[MAX_DELIVERIES];
int deliveryCount = 0;


void addCity() {
    if (cityCount >= MAX_CITIES) {
        printf("City limit reached.\n");
        return;
    }
    char name[50];
    printf("Enter city name: ");
    scanf(" %[^\n]", name);

    for (int i = 0; i < cityCount; i++) {
        if (strcmp(cities[i], name) == 0) {
            printf("City already exists.\n");
            return;
        }
    }

    strcpy(cities[cityCount], name);
    for (int j = 0; j <= cityCount; j++) {
        distanceMatrix[cityCount][j] = 0;
        distanceMatrix[j][cityCount] = 0;
    }
    cityCount++;
    printf("City added successfully!\n");
}

void listCities() {
    if (cityCount == 0) {
        printf("No cities available.\n");
        return;
    }
    printf("\n--- Cities ---\n");
    for (int i = 0; i < cityCount; i++) {
        printf("%d. %s\n", i, cities[i]);
    }
}


void editDistance() {
    if (cityCount < 2) {
        printf("Need at least 2 cities.\n");
        return;
    }

    listCities();
    int src, dest, dist;
    printf("Enter source city index: ");
    scanf("%d", &src);
    printf("Enter destination city index: ");
    scanf("%d", &dest);

    if (src < 0 || src >= cityCount || dest < 0 || dest >= cityCount) {
        printf("Invalid index.\n");
        return;
    }
    if (src == dest) {
        printf("Distance from a city to itself must be 0.\n");
        return;
    }

    printf("Enter distance between %s and %s (in km): ", cities[src], cities[dest]);
    scanf("%d", &dist);

    distanceMatrix[src][dest] = dist;
    distanceMatrix[dest][src] = dist;
    printf("Distance updated successfully!\n");
}

void showDistanceTable() {
    if (cityCount == 0) {
        printf("No cities available.\n");
        return;
    }

    printf("\nDistance Table (km)\n");
    printf("%10s", "");
    for (int i = 0; i < cityCount; i++)
        printf("%10s", cities[i]);
    printf("\n");

    for (int i = 0; i < cityCount; i++) {
        printf("%10s", cities[i]);
        for (int j = 0; j < cityCount; j++) {
            printf("%10d", distanceMatrix[i][j]);
        }
        printf("\n");
    }
}


void showVehicles() {
    printf("\n--- Available Vehicles ---\n");
    printf("%-10s %-12s %-12s %-15s %-15s\n", "Type", "Capacity(kg)", "Rate/km", "AvgSpeed", "Efficiency");
    printf("-----------------------------------------------------------------\n");
    for (int i = 0; i < 3; i++) {
        printf("%-10s %-12d %-12d %-15d %-15d\n",
               vehicleNames[i], vehicleCapacity[i], vehicleRate[i],
               vehicleSpeed[i], vehicleEfficiency[i]);
    }
}


void addDelivery() {
    if (cityCount < 2) {
        printf("Add at least 2 cities before creating deliveries.\n");
        return;
    }
    if (deliveryCount >= MAX_DELIVERIES) {
        printf("Delivery record limit reached.\n");
        return;
    }

    listCities();
    int src, dest, vType;
    double weight;

    printf("Enter source city index: ");
    scanf("%d", &src);
    printf("Enter destination city index: ");
    scanf("%d", &dest);

    if (src < 0 || src >= cityCount || dest < 0 || dest >= cityCount) {
        printf("Invalid city index.\n");
        return;
    }
    if (src == dest) {
        printf("Source and destination cannot be the same.\n");
        return;
    }

    showVehicles();
    printf("Select vehicle type (1=Van, 2=Truck, 3=Lorry): ");
    scanf("%d", &vType);

    if (vType < 1 || vType > 3) {
        printf("Invalid vehicle type.\n");
        return;
    }

    printf("Enter weight (kg): ");
    scanf("%lf", &weight);

    if (weight > vehicleCapacity[vType - 1]) {
        printf("Error: Weight exceeds vehicle capacity (%d kg).\n", vehicleCapacity[vType - 1]);
        return;
    }

    deliveries[deliveryCount].source = src;
    deliveries[deliveryCount].destination = dest;
    deliveries[deliveryCount].vehicleType = vType - 1;
    deliveries[deliveryCount].weight = weight;
    deliveryCount++;

    printf("Delivery request added successfully!\n");
}

void listDeliveries() {
    if (deliveryCount == 0) {
        printf("No deliveries recorded yet.\n");
        return;
    }
    printf("\n--- Delivery Requests ---\n");
    for (int i = 0; i < deliveryCount; i++) {
        int v = deliveries[i].vehicleType;
        printf("%d. From %s -> %s | %.2f kg | Vehicle: %s\n",
               i + 1, cities[deliveries[i].source], cities[deliveries[i].destination],
               deliveries[i].weight, vehicleNames[v]);
    }
}


int main() {
    int choice;
    while (1) {
        printf("\n===== Logistics Management System =====\n");
        printf("1. Add City\n");
        printf("2. List Cities\n");
        printf("3. Edit Distance\n");
        printf("4. Show Distance Table\n");
        printf("5. Show Vehicle Details\n");
        printf("6. Add Delivery Request\n");
        printf("7. List All Deliveries\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addCity(); break;
            case 2: listCities(); break;
            case 3: editDistance(); break;
            case 4: showDistanceTable(); break;
            case 5: showVehicles(); break;
            case 6: addDelivery(); break;
            case 7: listDeliveries(); break;
            case 0: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice.\n");
        }
    }
}
