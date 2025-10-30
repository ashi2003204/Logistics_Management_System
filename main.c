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

int main() {
    int choice;
    while (1) {
        printf("\n==== Logistics Management System ====\n");
        printf("1. Add City\n");
        printf("2. Show Cities\n");
        printf("3. Manage Distances\n");
        printf("4. Show Distance Table\n");
        printf("5. Add Delivery\n");
        printf("6. View Reports\n");
        printf("7. Exit\n");
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
            case 7: printf("Exiting program...\n"); exit(0);
            default: printf("Invalid choice.\n");
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
    cities[cityCount][strcspn(cities[cityCount], "\n")] = '\0';
    cityCount++;
    printf("City added successfully.\n");
}

void show_cities() {
    printf("\n--- Cities ---\n");
    for (int i = 0; i < cityCount; i++) {
        printf("%d. %s\n", i, cities[i]);
    }
}


void manage_distance() {
    if (cityCount < 2) {
        printf("Add at least 2 cities first.\n");
        return;
    }

    int i, j, d;
    show_cities();
    printf("Enter source city index: ");
    scanf("%d", &i);
    printf("Enter destination city index: ");
    scanf("%d", &j);
    printf("Enter distance between %s and %s: ", cities[i], cities[j]);
    scanf("%d", &d);

    if (i == j) {
        printf("Distance to itself cannot be set.\n");
        return;
    }

    distanceMatrix[i][j] = distanceMatrix[j][i] = d;
    printf("Distance updated successfully.\n");
}

void show_distance_table() {
    printf("\nDistance Table (km):\n");
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


void add_delivery() {
    if (cityCount < 2) {
        printf("Add cities and distances first.\n");
        return;
    }
    if (deliveryCount >= MAX_DELIVERIES) {
        printf("Maximum deliveries reached.\n");
        return;
    }

    int s, d, w, v;
    show_cities();
    printf("Enter source city index: ");
    scanf("%d", &s);
    printf("Enter destination city index: ");
    scanf("%d", &d);
    printf("Enter weight (kg): ");
    scanf("%d", &w);
    printf("Select vehicle (1=Van, 2=Truck, 3=Lorry): ");
    scanf("%d", &v);

    v--;
    if (v < 0 || v > 2 || s == d || w > VEHICLE_CAPACITY[v]) {
        printf("Invalid delivery details.\n");
        return;
    }

    double dist = distanceMatrix[s][d];
    double cost = dist * VEHICLE_RATE[v] * (1 + w / 10000.0);
    double fuelUsed = dist / VEHICLE_EFFICIENCY[v];
    double fuelCost = fuelUsed * FUEL_PRICE;
    double totalCost = cost + fuelCost;
    double profit = cost * 0.25;
    double customerCharge = totalCost + profit;
    double time = dist / VEHICLE_SPEED[v];

    Delivery newDelivery = {s, d, w, v, dist, cost, fuelUsed, fuelCost, totalCost, profit, customerCharge, time};
    deliveries[deliveryCount++] = newDelivery;

    printf("\nDelivery Added Successfully!\n");
    printf("From: %s To: %s\n", cities[s], cities[d]);
    printf("Distance: %.2f km\nVehicle: %s\n", dist, VEHICLE_NAMES[v]);
    printf("Customer Charge: %.2f LKR\n", customerCharge);
}


void show_reports() {
    if (deliveryCount == 0) {
        printf("No deliveries yet.\n");
        return;
    }

    double totalDistance = 0, totalTime = 0, totalRevenue = 0, totalProfit = 0;
    double longest = 0, shortest = INF;
    for (int i = 0; i < deliveryCount; i++) {
        totalDistance += deliveries[i].distance;
        totalTime += deliveries[i].time;
        totalRevenue += deliveries[i].customerCharge;
        totalProfit += deliveries[i].profit;
        if (deliveries[i].distance > longest) longest = deliveries[i].distance;
        if (deliveries[i].distance < shortest) shortest = deliveries[i].distance;
    }

    printf("\n===== PERFORMANCE REPORT =====\n");
    printf("Total Deliveries: %d\n", deliveryCount);
    printf("Total Distance Covered: %.2f km\n", totalDistance);
    printf("Average Delivery Time: %.2f hours\n", totalTime / deliveryCount);
    printf("Total Revenue: %.2f LKR\n", totalRevenue);
    printf("Total Profit: %.2f LKR\n", totalProfit);
    printf("Longest Route: %.2f km\n", longest);
    printf("Shortest Route: %.2f km\n", shortest);
    printf("===============================\n");
}
