#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_CITIES 30
#define MAX_DELIVERIES 50
#define INF 1000000000


const char *VEHICLE_NAMES[3] = {"Van", "Truck", "Lorry"};
const int VEHICLE_CAPACITY[3] = {1000, 5000, 10000};
const int VEHICLE_RATE[3] = {30, 40, 80};
const int VEHICLE_SPEED[3] = {60, 50, 45};
const int VEHICLE_EFFICIENCY[3] = {12, 6, 4};

#define FUEL_PRICE 310.0r


int distance[MAX_CITIES][MAX_CITIES];
char cities[MAX_CITIES][50];
int city_count = 0;


typedef struct {
    int source;
    int destination;
    int vehicleType;
    float weight;
    float baseCost;
    float time;
    float fuelUsed;
    float fuelCost;
    float totalOperationalCost;
    float profit;
    float customerCharge;
    float distance;
} Delivery;

Delivery deliveries[MAX_DELIVERIES];
int delivery_count = 0;


void save_data() {
    FILE *fc = fopen("cities.txt", "w");
    if (fc) {
        fprintf(fc, "%d\n", city_count);
        for (int i = 0; i < city_count; i++)
            fprintf(fc, "%s\n", cities[i]);
        fclose(fc);
    }

    FILE *fd = fopen("distances.txt", "w");
    if (fd) {
        fprintf(fd, "%d\n", city_count);
        for (int i = 0; i < city_count; i++) {
            for (int j = 0; j < city_count; j++)
                fprintf(fd, "%d ", distance[i][j]);
            fprintf(fd, "\n");
        }
        fclose(fd);
    }

    FILE *fl = fopen("deliveries.txt", "w");
    if (fl) {
        fprintf(fl, "%d\n", delivery_count);
        for (int i = 0; i < delivery_count; i++) {
            Delivery d = deliveries[i];
            fprintf(fl, "%d %d %d %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
                    d.source, d.destination, d.vehicleType, d.weight, d.baseCost,
                    d.time, d.fuelUsed, d.fuelCost, d.totalOperationalCost,
                    d.profit, d.customerCharge, d.distance);
        }
        fclose(fl);
    }
}

void load_data() {
    FILE *fc = fopen("cities.txt", "r");
    if (fc) {
        fscanf(fc, "%d", &city_count);
        for (int i = 0; i < city_count; i++)
            fscanf(fc, "%s", cities[i]);
        fclose(fc);
    }

    FILE *fd = fopen("distances.txt", "r");
    if (fd) {
        int count;
        fscanf(fd, "%d", &count);
        for (int i = 0; i < count; i++)
            for (int j = 0; j < count; j++)
                fscanf(fd, "%d", &distance[i][j]);
        fclose(fd);
    }

    FILE *fl = fopen("deliveries.txt", "r");
    if (fl) {
        fscanf(fl, "%d", &delivery_count);
        for (int i = 0; i < delivery_count; i++) {
            Delivery d;
            fscanf(fl, "%d %d %d %f %f %f %f %f %f %f %f %f",
                   &d.source, &d.destination, &d.vehicleType, &d.weight, &d.baseCost,
                   &d.time, &d.fuelUsed, &d.fuelCost, &d.totalOperationalCost,
                   &d.profit, &d.customerCharge, &d.distance);
            deliveries[i] = d;
        }
        fclose(fl);
    }
}


void add_city() {
    if (city_count >= MAX_CITIES) {
        printf("City limit reached!\n");
        return;
    }
    printf("Enter city name: ");
    scanf("%s", cities[city_count]);
    city_count++;
    printf("City added successfully!\n");
}

void list_cities() {
    if (city_count == 0) {
        printf("No cities available.\n");
        return;
    }
    printf("\nCities:\n");
    for (int i = 0; i < city_count; i++) {
        printf("%d. %s\n", i, cities[i]);
    }
}


void input_distance() {
    if (city_count < 2) {
        printf("Add at least 2 cities first.\n");
        return;
    }
    list_cities();
    int i, j, d;
    printf("Enter index of source city: ");
    scanf("%d", &i);
    printf("Enter index of destination city: ");
    scanf("%d", &j);
    if (i == j) {
        printf("Distance from a city to itself is 0.\n");
        distance[i][j] = 0;
        return;
    }
    printf("Enter distance (km): ");
    scanf("%d", &d);
    distance[i][j] = distance[j][i] = d;
    printf("Distance updated!\n");

void display_vehicles() {
    printf("\nAvailable Vehicles:\n");
    printf("Type\tCapacity(kg)\tRate/km\tSpeed(km/h)\tEfficiency(km/l)\n");
    for (int i = 0; i < 3; i++) {
        printf("%s\t%d\t\t%d\t%d\t\t%d\n",
               VEHICLE_NAMES[i], VEHICLE_CAPACITY[i],
               VEHICLE_RATE[i], VEHICLE_SPEED[i], VEHICLE_EFFICIENCY[i]);
    }
}


float calculate_base_cost(float D, float W, int vehicleType) {
    return D * VEHICLE_RATE[vehicleType] * (1 + (W / 10000.0));
}
float calculate_time(float D, int vehicleType) {
    return D / (float)VEHICLE_SPEED[vehicleType];
}
float calculate_fuel_used(float D, int vehicleType) {
    return D / (float)VEHICLE_EFFICIENCY[vehicleType];
}
float calculate_fuel_cost(float fuelUsed) {
    return fuelUsed * FUEL_PRICE;
}
float calculate_total_operational_cost(float baseCost, float fuelCost) {
    return baseCost + fuelCost;
}
float calculate_profit(float baseCost) {
    return baseCost * 0.25;
}
float calculate_customer_charge(float totalCost, float profit) {
    return totalCost + profit;
}


void create_delivery() {
    if (delivery_count >= MAX_DELIVERIES) {
        printf("Delivery limit reached!\n");
        return;
    }
    if (city_count < 2) {
        printf("Add cities first.\n");
        return;
    }

    Delivery d;
    list_cities();
    printf("Enter source city index: ");
    scanf("%d", &d.source);
    printf("Enter destination city index: ");
    scanf("%d", &d.destination);
    if (d.source == d.destination) {
        printf("Source and destination cannot be same!\n");
        return;
    }

    printf("Enter vehicle type (0=Van, 1=Truck, 2=Lorry): ");
    scanf("%d", &d.vehicleType);
    printf("Enter weight (kg): ");
    scanf("%f", &d.weight);

    if (d.weight > VEHICLE_CAPACITY[d.vehicleType]) {
        printf("Weight exceeds vehicle capacity!\n");
        return;
    }

    d.distance = distance[d.source][d.destination];
    if (d.distance <= 0) {
        printf("Invalid distance! Please enter distance first.\n");
        return;
    }

    d.baseCost = calculate_base_cost(d.distance, d.weight, d.vehicleType);
    d.time = calculate_time(d.distance, d.vehicleType);
    d.fuelUsed = calculate_fuel_used(d.distance, d.vehicleType);
    d.fuelCost = calculate_fuel_cost(d.fuelUsed);
    d.totalOperationalCost = calculate_total_operational_cost(d.baseCost, d.fuelCost);
    d.profit = calculate_profit(d.baseCost);
    d.customerCharge = calculate_customer_charge(d.totalOperationalCost, d.profit);

    deliveries[delivery_count++] = d;

    printf("\nDelivery created successfully!\n");
    printf("---------------------------------\n");
    printf("From: %s\nTo: %s\nVehicle: %s\nWeight: %.2f kg\n",
           cities[d.source], cities[d.destination], VEHICLE_NAMES[d.vehicleType], d.weight);
    printf("Distance: %.2f km\nBase Cost: %.2f LKR\nFuel Used: %.2f L\nFuel Cost: %.2f LKR\n",
           d.distance, d.baseCost, d.fuelUsed, d.fuelCost);
    printf("Total Operational Cost: %.2f LKR\nProfit: %.2f LKR\nCustomer Charge: %.2f LKR\nEstimated Time: %.2f hours\n",
           d.totalOperationalCost, d.profit, d.customerCharge, d.time);
    printf("---------------------------------\n");

    save_data();
}


void show_reports() {
    if (delivery_count == 0) {
        printf("No deliveries available for report.\n");
        return;
    }

    float totalDistance = 0, totalRevenue = 0, totalProfit = 0, totalTime = 0;
    float longest = 0, shortest = INF;
    for (int i = 0; i < delivery_count; i++) {
        totalDistance += deliveries[i].distance;
        totalRevenue += deliveries[i].customerCharge;
        totalProfit += deliveries[i].profit;
        totalTime += deliveries[i].time;
        if (deliveries[i].distance > longest) longest = deliveries[i].distance;
        if (deliveries[i].distance < shortest) shortest = deliveries[i].distance;
    }

    printf("\n===== Performance Report =====\n");
    printf("Total Deliveries: %d\n", delivery_count);
    printf("Total Distance Covered: %.2f km\n", totalDistance);
    printf("Average Delivery Time: %.2f hours\n", totalTime / delivery_count);
    printf("Total Revenue: %.2f LKR\n", totalRevenue);
    printf("Total Profit: %.2f LKR\n", totalProfit);
    printf("Longest Route: %.2f km\n", longest);
    printf("Shortest Route: %.2f km\n", shortest);
    printf("==============================\n");
}


void main_menu() {
    int choice;
    while (1) {
        printf("\n====== Logistics Management System ======\n");
        printf("1. Add City\n");
        printf("2. Input Distance\n");
        printf("3. View Vehicles\n");
        printf("4. Create Delivery\n");
        printf("5. View Reports\n");
        printf("6. Exit (Save & Quit)\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_city(); save_data(); break;
            case 2: input_distance(); save_data(); break;
            case 3: display_vehicles(); break;
            case 4: create_delivery(); break;
            case 5: show_reports(); break;
            case 6: save_data(); printf("Data saved. Exiting...\n"); exit(0);
            default: printf("Invalid choice!\n");
        }
    }
}

int main() {
    load_data();
    main_menu();
    return 0;
}
