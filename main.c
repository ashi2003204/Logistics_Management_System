#include <stdio.h>
#include <string.h>

#define MAX_CITIES 30
#define INF 1000000000

char cities[MAX_CITIES][50];
int distanceMatrix[MAX_CITIES][MAX_CITIES];
int cityCount = 0;

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

int main() {
    int choice;
    while (1) {
        printf("\n===== Logistics Management System =====\n");
        printf("1. Add City\n");
        printf("2. List Cities\n");
        printf("3. Edit Distance\n");
        printf("4. Show Distance Table\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addCity(); break;
            case 2: listCities(); break;
            case 3: editDistance(); break;
            case 4: showDistanceTable(); break;
            case 0: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice.\n");
        }
    }
}
