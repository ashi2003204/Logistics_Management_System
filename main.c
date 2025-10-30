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

#define FUEL_PRICE 310.0


int distanceMatrix[MAX_CITIES][MAX_CITIES];
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
                fprintf(fd, "%d ", distanceMatrix[i][j]);
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
                fscanf(fd, "%d", &distanceMatrix[i][j]);
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


int minDistance(int dist[], int visited[], int n) {
    int min = INF, min_index = -1;
    for (int v = 0; v < n; v++)
        if (!visited[v] && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}

int dijkstra_shortest_distance(int graph[MAX_CITIES][MAX_CITIES], int src, int dest, int n) {
    int dist[MAX_CITIES];
    int visited[MAX_CITIES];

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = 0;
    }
    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        int u = minDistance(dist, visited, n);
        if (u == -1) break;
        visited[u] = 1;

        for (int v = 0; v < n; v++) {
            if (!visited[v] && graph[u][v] && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    return dist[dest];
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
    printf("Enter distance (km): ");
    scanf("%d", &d);
    distanceMatrix[i][j] = distanceMatrix[j][i] = d;
    printf("Distance updated!\n");
}


void display_vehicles() {
    printf("\nAvailable Vehicles:\n");
    printf("Type\tCapacity(kg)\tRate/km\tSpeed(km/h)\tEfficiency(km/l)\n");
    for (int i = 0; i < 3; i++) {
        printf("%s\t%d\t\t%d\t%d\t\t%d\n",
               VEHICLE_NAMES[i], VEHICLE_CAPACITY[i],
               VEHICLE_RATE[i], VEHICLE_SPEED[i], VEHICLE_EFFICIENCY[i]);
    }
}


void suggest_best_vehicle(float distance, float weight) {
    printf("\nVehicle Optimization Analysis:\n");
    float minCost = INF;
    int bestVehicle = -1;

    for (int i = 0; i < 3; i++) {
        if (weight > VEHICLE_CAPACITY[i]) continue;

        float baseCost = distance * VEHICLE_RATE[i] * (1 + (weight / 10000.0));
        float fuelUsed = distance / (float)VEHICLE_EFFICIENCY[i];
        float fuelCost = fuelUsed * FUEL_PRICE;
        float totalCost = baseCost + fuelCost;

        printf("%s → Est. Total Operational Cost: %.2f LKR\n", VEHICLE_NAMES[i], totalCost);

        if (totalCost < minCost) {
            minCost = totalCost;
            bestVehicle = i;
        }
    }

    if (bestVehicle != -1)
        printf("Recommended Vehicle: 🚚 %s (Lowest Cost)\n", VEHICLE_NAMES[bestVehicle]);
    else
        printf("No suitable vehicle for this weight.\n");
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
    printf("Enter weight (kg): ");
    scanf("%f", &d.weight);


    d.distance = dijkstra_shortest_distance(distanceMatrix, d.source, d.destination, city_count);
    if (d.distance == INF) {
        printf("No route available!\n");
        return;
    }

    suggest_best_vehicle(d.distance, d.weight);

    printf("Select vehicle type (0=Van, 1=Truck, 2=Lorry): ");
    scanf("%d", &d.vehicleType);

    if (d.weight > VEHICLE_CAPACITY[d.vehicleType]) {
        printf("Weight exceeds vehicle capacity!\n");
        return;
    }

    d.baseCost = d.distance * VEHICLE_RATE[d.vehicleType] * (1 + (d.weight / 10000.0));
    d.time = d.distance / (float)VEHICLE_SPEED[d.vehicleType];
    d.fuelUsed = d.distance / (float)VEHICLE_EFFICIENCY[d.vehicleType];
    d.fuelCost = d.fuelUsed * FUEL_PRICE;
    d.totalOperationalCost = d.baseCost + d.fuelCost;
    d.profit = d.baseCost * 0.25;
    d.customerCharge = d.totalOperationalCost + d.profit;

    deliveries[delivery_count++] = d;
    save_data();

    printf("\nDelivery created successfully!\n");
}


void show_reports() {
    if (delivery_count == 0) {
        printf("No deliveries available.\n");
        return;
    }

    float totalDistance = 0, totalRevenue = 0, totalProfit = 0;
    for (int i = 0; i < delivery_count; i++) {
        totalDistance += deliveries[i].distance;
        totalRevenue += deliveries[i].customerCharge;
        totalProfit += deliveries[i].profit;
    }

    printf("\n===== Performance Report =====\n");
    printf("Total Deliveries: %d\n", delivery_count);
    printf("Total Distance Covered: %.2f km\n", totalDistance);
    printf("Total Revenue: %.2f LKR\n", totalRevenue);
    printf("Total Profit: %.2f LKR\n", totalProfit);
    printf("==============================\n");
}


void main_menu() {
    int choice;
    while (1) {
        printf("\n====== Logistics Management System ======\n");
        printf("1. Add City\n");
        printf("2. Input Distance\n");
        printf("3. View Vehicles\n");
        printf("4. Create Delivery (with Optimization)\n");
        printf("5. View Reports\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_city(); save_data(); break;
            case 2: input_distance(); save_data(); break;
            case 3: display_vehicles(); break;
            case 4: create_delivery(); break;
            case 5: show_reports(); break;
            case 6: save_data(); printf("Exiting...\n"); exit(0);
            default: printf("Invalid choice!\n");
        }
    }
}

int main() {
    load_data();
    main_menu();
    return 0;
}
