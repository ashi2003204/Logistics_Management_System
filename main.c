#include <stdio.h>
#include <string.h>

#define MAX_CITIES 30
#define MAX_NAME_LEN 50


char cities[MAX_CITIES][MAX_NAME_LEN];
int cityCount = 0;


void cityMenu();
void addCity();
void listCities();
void removeCity();
void renameCity();

int main() {
    int choice;

    printf("=== Logistics Management System ===\n");

    while (1) {
        printf("\nMain Menu:\n");
        printf("1. City Management\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1)
            cityMenu();
        else if (choice == 0)
            break;
        else
            printf("Invalid choice! Try again.\n");
    }

    printf("Exiting program...\n");
    return 0;
}



void cityMenu() {
    int option;
    while (1) {
        printf("\n--- City Management ---\n");
        printf("1. Add City\n");
        printf("2. List Cities\n");
        printf("3. Remove City\n");
        printf("4. Rename City\n");
        printf("0. Back to Main Menu\n");
        printf("Enter option: ");
        scanf("%d", &option);
        getchar();

        if (option == 1)
            addCity();
        else if (option == 2)
            listCities();
        else if (option == 3)
            removeCity();
        else if (option == 4)
            renameCity();
        else if (option == 0)
            break;
        else
            printf("Invalid option. Try again.\n");
    }
}

void addCity() {
    if (cityCount >= MAX_CITIES) {
        printf("City list full! Cannot add more.\n");
        return;
    }
    printf("Enter city name: ");
    fgets(cities[cityCount], MAX_NAME_LEN, stdin);
    cities[cityCount][strcspn(cities[cityCount], "\n")] = '\0';
    cityCount++;
    printf("City added successfully!\n");
}

void listCities() {
    if (cityCount == 0) {
        printf("No cities available.\n");
        return;
    }
    printf("\nList of Cities:\n");
    for (int i = 0; i < cityCount; i++) {
        printf("%d. %s\n", i + 1, cities[i]);
    }
}

void removeCity() {
    int index;
    if (cityCount == 0) {
        printf("No cities to remove.\n");
        return;
    }
    listCities();
    printf("Enter city number to remove: ");
    scanf("%d", &index);
    getchar();
    if (index < 1 || index > cityCount) {
        printf("Invalid city number.\n");
        return;
    }
    for (int i = index - 1; i < cityCount - 1; i++) {
        strcpy(cities[i], cities[i + 1]);
    }
    cityCount--;
    printf("City removed successfully.\n");
}

void renameCity() {
    int index;
    char newName[MAX_NAME_LEN];
    if (cityCount == 0) {
        printf("No cities to rename.\n");
        return;
    }
    listCities();
    printf("Enter city number to rename: ");
    scanf("%d", &index);
    getchar();
    if (index < 1 || index > cityCount) {
        printf("Invalid city number.\n");
        return;
    }
    printf("Enter new name: ");
    fgets(newName, MAX_NAME_LEN, stdin);
    newName[strcspn(newName, "\n")] = '\0';
    strcpy(cities[index - 1], newName);
    printf("City renamed successfully.\n");
}
