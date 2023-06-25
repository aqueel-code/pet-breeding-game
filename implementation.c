#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "interface.h"

#define MAX_PETS 10
#define MAX_NAME_LENGTH 20
#define MAX_MOOD_LENGTH 50
#define MAX_MOODS 10
#define MAX_EMOTION_LENGTH 10
#define MAX_CONDITION_LENGTH 20
#define LOW_ENERGY_THRESHOLD 5
#define LOW_HAPPINESS_THRESHOLD 5

int gameRunning = 1;
int timeToCheckEnergy = 1;
int timeToCheckHappiness = 1;

bool petDataModified = false;

struct Pet pets[MAX_PETS];
int numPets = 0;

void initialize() { // Initialize the game
    loadPetData();
}

void loadPetData() { // Load pet data from file
    FILE* file = fopen("Pet_Data.txt", "r");
    if (file == NULL) {
        printf("Error opening Pet_Data.txt file.\n");
        return;
    }

    while (numPets < MAX_PETS && fscanf(file, "%[^,],%[^,],%d,%d\n", pets[numPets].type, pets[numPets].name,
    &pets[numPets].energy, &pets[numPets].happiness) == 4) {
        numPets++;
    }

    if (fclose(file) != 0) {
        printf("Error closing Pet_Data.txt file.\n");
    }
}


void savePetData() { // Save pet data to file
    FILE *file = fopen("Pet_Data.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < numPets; i++) {
        fprintf(file, "%s,%s,%d,%d\n", pets[i].type, pets[i].name, pets[i].energy, pets[i].happiness);
        char filename[50];
        sprintf(filename, "Pet_%s.txt", pets[i].name); // Save pet data to pet’s name
        FILE *petFile = fopen(filename, "w");
        if (petFile == NULL) {
            printf("Error opening file.\n");
            return;
        }
        fprintf(petFile, "%s,%s,%d,%d\n", pets[i].type, pets[i].name, pets[i].energy, pets[i].happiness);
        fclose(petFile);
    }

    if (fclose(file) != 0) {
        printf("Error closing Pet_Data.txt file.\n");
    }
}

bool savePetOutput(const char* output) { //Save pet output data
    FILE* file = fopen("Pet_Output.txt", "a");
    if (file == NULL) {
        return false;
    }

    for (int i = 0; i < numPets; i++) {
        fprintf(file, "%s,%s,%d,%d\n", pets[i].type, pets[i].name, pets[i].energy, pets[i].happiness);
    }

    return fclose(file) == 0;
}

void addPet() { // Add a new pet
    if (numPets >= MAX_PETS) {
        printf("Maximum number of pets reached.\n");
        return;
    }

    char type[20];
    char name[20];

    printf("Enter the type of the pet: ");
    fgets(type, sizeof(type), stdin);
    type[strcspn(type, "\n")] = '\0';

    printf("Enter the name of the pet: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    if (strlen(type) == 0 || strlen(name) == 0) {
        printf("Invalid input. Type and name cannot be empty.\n");
        return;
    }

    for (int i = 0; i < numPets; i++) {
        if (strcmp(pets[i].name, name) == 0) {
            printf("A pet with the same name already exists.\n");
            return;
        }
    }

    strcpy(pets[numPets].type, type);
    strcpy(pets[numPets].name, name);
    pets[numPets].energy = 60;
    pets[numPets].happiness = 60;
    strcpy(pets[numPets].mood, getPetMoodTextEmoji(pets[numPets].happiness, pets[numPets].energy));
    numPets++;
    petDataModified = true;
    printf("Pet added successfully.\n");
    savePetData();
    savePetOutput("New pet added.");
    
}

void displayPets() { // Display all pets
    printf("Available pets:\n");
    for (int i = 1; i <= numPets; i++) {
        printf("%d. %s - %s\n", i, pets[i-1].type, pets[i-1].name);
    }
}

void feedPet() { // Feed a pet
    displayPets();
    int petIndex;
    printf("Select a pet to feed: ");
    scanf("%d", &petIndex);

    if (petIndex >= 1 && petIndex <= numPets) {
        petIndex--;

        pets[petIndex].energy += 15;
        pets[petIndex].happiness += 10;

        if (pets[petIndex].energy > 100) {
            pets[petIndex].energy = 100;
        }

        if (pets[petIndex].happiness > 100) {
            pets[petIndex].happiness = 100;
        }

        printf("Pet %s has been fed.\n", pets[petIndex].name);
        savePetData();

        checkEnergyLevel(petIndex);
    } else {
        printf("Invalid pet selection.\n");
    }
}

void pet_update(int petIndex, const char* newName) { // Update pet's name
    if (petIndex >= 1 && petIndex <= numPets) {
        strncpy(pets[petIndex - 1].name, newName, MAX_NAME_LENGTH);
        printf("Pet %s has been updated.\n", pets[petIndex - 1].name);
        const char* mood = calculatePetMood(petIndex, pets);

        strncpy(pets[petIndex - 1].mood, mood, MAX_MOOD_LENGTH);

        savePetData();
    } else {
        printf("Invalid pet selection.\n");
    }
}


void delete_pet(int petIndex) { // Delete a pet
    if (petIndex >= 1 && petIndex <= numPets) {
        petIndex--;

        for (int i = petIndex; i < numPets - 1; i++) {
            strcpy(pets[i].type, pets[i + 1].type);
            strcpy(pets[i].name, pets[i + 1].name);
            pets[i].energy = pets[i + 1].energy;
            pets[i].happiness = pets[i + 1].happiness;
        }

        numPets--;
        savePetData();
        printf("Pet deleted successfully.\n");
    } else {
        printf("Invalid pet selection.\n");
    }
}

void give_water(int petIndex) { // Give water to a pet
    if (petIndex >= 1 && petIndex <= numPets) {
        petIndex--;

        pets[petIndex].energy += 5;
        pets[petIndex].happiness += 5;

        if (pets[petIndex].energy > 100) {
            pets[petIndex].energy = 100;
        }

        if (pets[petIndex].happiness > 100) {
            pets[petIndex].happiness = 100;
        }

        printf("Pet %s has been given water.\n", pets[petIndex].name);
        savePetData();
    } else {
        printf("Invalid pet selection.\n");
    }
}

void play_games(int petIndex) { // Play games with a pet
    if (petIndex >= 1 && petIndex <= numPets) {
        petIndex--;

        if (pets[petIndex].energy < LOW_ENERGY_THRESHOLD) {
            printf("Pet's energy level is too low. Put the pet to sleep.\n");

        } else {
            pets[petIndex].energy -= 15;
            pets[petIndex].happiness += 15;

            if (pets[petIndex].energy < 0) {
                pets[petIndex].energy = 0;
            }

            if (pets[petIndex].happiness > 100) {
                pets[petIndex].happiness = 100;
            }

            printf("Pet %s has played games.\n", pets[petIndex].name);
            savePetData();
        }
    } else {
        printf("Invalid pet selection.\n");
    }
}


void clean_pet(int petIndex) { // Clean a pet
    if (petIndex >= 1 && petIndex <= numPets) {
        petIndex--;

        pets[petIndex].energy -= 10;
        pets[petIndex].happiness -= 5;

        if (pets[petIndex].energy < 0) {
            pets[petIndex].energy = 0;
        }

        if (pets[petIndex].happiness < 0) {
            pets[petIndex].happiness = 0;
        }

        printf("Pet %s has been cleaned.\n", pets[petIndex].name);
        savePetData();
    } else {
        printf("Invalid pet selection.\n");
    }
}

void love_pet(int petIndex) { // Show love to a pet
    if (petIndex >= 1 && petIndex <= numPets) {
        petIndex--;

        pets[petIndex].happiness += 5;

        if (pets[petIndex].happiness > 100) {
            pets[petIndex].happiness = 100;
        }

        printf("Pet %s feels loved.\n", pets[petIndex].name);
        savePetData();
    } else {
        printf("Invalid pet selection.\n");
    }
}

void put_to_sleep(int petIndex) { // Put a pet to sleep
    if (petIndex >= 1 && petIndex <= numPets) {
        petIndex--;

        pets[petIndex].energy += 5;

        if (pets[petIndex].energy > 100) {
            pets[petIndex].energy = 100;
        }

        printf("Pet %s has been put to sleep.\n", pets[petIndex].name);
        savePetData();
    } else {
        printf("Invalid pet selection.\n");
    }
}

void reward_pet(int petIndex) { // Reward a pet
    if (petIndex >= 1 && petIndex <= numPets) {
        petIndex--;

        pets[petIndex].happiness += 10;

        if (pets[petIndex].happiness > 100) {
            pets[petIndex].happiness = 100;
        }

        printf("Pet %s has been rewarded.\n", pets[petIndex].name);
        savePetData();
    } else {
        printf("Invalid pet selection.\n");
    }
}

void handleLowEnergy(int petIndex) { // Handle low energy level of a pet
    printf("Warning: Pet %s has low energy.\n", pets[petIndex].name);
    printf("What would you like to do?\n");
    printf("1. Put the pet to sleep\n");
    printf("Enter your choice: ");

    int choice;
    scanf("%d", &petIndex, &choice);

    switch (choice) {
        case 1:
            put_to_sleep(petIndex);
            savePetData(petIndex);
            break;
        default:
            printf("Invalid choice.\n");
            break;
    }
}

void handleLowHappiness(int petIndex) { // Handle low happiness level of a pet
    printf("Warning: Pet %s is unhappy.\n", pets[petIndex].name);
    printf("What would you like to do?\n");
    printf("1. Play games with the pet\n");
    printf("2. Clean the pet\n");
    printf("3. Show love to the pet\n");
    printf("Enter your choice: ");

    int choice;
    scanf("%d", &petIndex, &choice);

    switch (choice) {
        case 1:
            play_games(petIndex);
            savePetData(petIndex);
            break;
        case 2:
            clean_pet(petIndex);
            savePetData(petIndex);
            break;
        case 3:
            love_pet(petIndex);
            savePetData(petIndex);
            break;
        default:
            printf("Invalid choice.\n");
            break;
    }
}

void checkEnergyLevel(int petIndex) { // Check energy level of a pet
    if (pets[petIndex].energy < LOW_ENERGY_THRESHOLD) {
        handleLowEnergy(petIndex);
    }
}

void checkHappinessLevel(int petIndex) { // Check happiness level of a pet
    if (pets[petIndex].happiness < LOW_HAPPINESS_THRESHOLD) {
        handleLowHappiness(petIndex);
    }
}

void printEmotion() { // Print emotions of all pets
    const char* emotionFile = "emotion.txt";

    FILE* file = fopen(emotionFile, "r");
    if (file == NULL) {
        printf("Failed to open emotion file.\n");
        return;
    }

    printf("Emotion Definitions:\n");

    int id;
    char emotion[20];
    char emoji[10];
    char text[20];
    char condition[20];

    while (fscanf(file, "%d,%[^,],%[^,],%[^,],%[^,\n]", &id, emotion, emoji, text, condition) == 5) {
        printf("%d. %s\n", id, emotion);
        printf("   Emoji: %s\n", emoji);
        printf("   Text Provision: %s\n", text);
        printf("   Condition: %s\n\n", condition);
    }

    fclose(file);
}

typedef struct {
    char name[MAX_NAME_LENGTH];
    char emotion[MAX_EMOTION_LENGTH];
    char energyCondition[MAX_CONDITION_LENGTH];
    char happinessCondition[MAX_CONDITION_LENGTH];
} Mood;

Mood moods[MAX_MOODS]; // Array to store mood data
int numMoods = 0;

void loadMoodsFromFile(const char* fileName) {
    FILE* file = fopen(fileName, "r"); // Open the specified file in read mode
    if (file == NULL) {
        printf("Error opening file %s.\n", fileName);
        return;
    }

    char line[100]; // Buffer to store each line read from the file
    while (fgets(line, sizeof(line), file) != NULL) {
        char name[MAX_NAME_LENGTH];
        char emotion[MAX_EMOTION_LENGTH];
        char energyCondition[MAX_CONDITION_LENGTH];
        char happinessCondition[MAX_CONDITION_LENGTH];

        int fieldsRead = sscanf(line, "%[^,],%[^,],%[^,],%[^,\n]", name, emotion, energyCondition, happinessCondition);
        if (fieldsRead == 4) {
            Mood mood;
            strncpy(mood.name, name, MAX_NAME_LENGTH);
            strncpy(mood.emotion, emotion, MAX_EMOTION_LENGTH);
            strncpy(mood.energyCondition, energyCondition, MAX_CONDITION_LENGTH);
            strncpy(mood.happinessCondition, happinessCondition, MAX_CONDITION_LENGTH);

            moods[numMoods] = mood;
            numMoods++;
        }
    }

    fclose(file);
}

int getPetMoodIndex(int petIndex) {
    for (int i = 0; i < numMoods; i++) {
        Mood mood = moods[i]; // Get the mood at index i from the moods array
        // Check if the pet's energy and happiness conditions satisfy the mood's conditions
        if ((strcmp(mood.energyCondition, "Not important") == 0 ||
             pets[petIndex].energy >= atoi(mood.energyCondition)) &&
            (strcmp(mood.happinessCondition, "Not important") == 0 ||
             pets[petIndex].happiness >= atoi(mood.happinessCondition))) {
            return i; // Return the index of the matching mood
        }
    }
    return -1; // Return -1 if no matching mood is found
}

const char* getPetMoodTextEmoji(int energy, int happiness) {
    if (energy > 25 && happiness > 50) {
        return "Happy :)";
    } else if (energy < 20 && happiness < 20) {
        return "Sad :(";
    } else if (energy < 10 && happiness < 10) {
        return "Crying :'(";
    } else if (energy < 5) {
        return "Sleeping :zz";
    } else if (energy < 20) {
        return "Hungry :-o";
    } else if (energy > 40 || happiness < 30) {
        return "Wants a game <:0)";
    } else {
        return "Unknown";
    }
}

const char* calculatePetMood(int petIndex, struct Pet* pets) {
    if (pets[petIndex - 1].energy > 25 && pets[petIndex - 1].happiness > 50) {
        return "Happy :)";
    } else if (pets[petIndex - 1].energy < 20 && pets[petIndex - 1].happiness < 20) {
        return "Sad :(";
    } else if (pets[petIndex - 1].energy < 10 && pets[petIndex - 1].happiness < 10) {
        return "Crying :'(";
    } else if (pets[petIndex - 1].energy < 5) {
        return "Sleeping :zz";
    } else if (pets[petIndex - 1].energy < 20) {
        return "Hungry :-o";
    } else if (pets[petIndex - 1].energy > 40 || pets[petIndex - 1].happiness < 30) {
        return "Wants a game <:0)";
    } else {
        return "Unknown";
    }
}

void about() { //Information about me and the game
    printf("This programme was developed by Mohamed Ifham Aqueel Imthiyas.\n");
    printf("Contact info: 200504123@ogrenci.harran.edu.tr\n");
    printf("Version: 1.0.0-b.1 Beta\n");
    printf("Release date: 19.06.2023\n");
    printf("Objective: A Pet Breeding and Simulation Game implemented in C. Interact with virtual pets, meet their needs, and monitor their energy and happiness levels.\n");
    printf("\n");
}

void showMenu(int petIndex) { // Display the menu
    printf(".:: Pet Breeding and Simulation Game ::.\n");
    printf("\n");
    printf("Available Pets:\n");
    printf("-----------------------------------------------------------------------\n");
    printf("| Index | Type     | Name  | Energy | Happiness |          Mood       |\n");
    printf("-----------------------------------------------------------------------\n");

    for (int i = 0; i < numPets; i++) {
        const char* petMood = calculatePetMood(i + 1, pets);  // Calculate the mood for each pet
        printf("|   %d   | %-8s | %-5s |   %d   |      %d   | %-18s |\n",
               i + 1, pets[i].type, pets[i].name, pets[i].energy, pets[i].happiness, petMood);
        printf("-----------------------------------------------------------------------\n");
    }

    printf("\n");
    printf("Select an option:\n");
    printf("1. Add a pet\n");
    printf("2. Update pet\n");
    printf("3. Delete pet\n");
    printf("4. Feed pet\n");
    printf("5. Give water\n");
    printf("6. Play games\n");
    printf("7. Clean pet\n");
    printf("8. Love pet\n");
    printf("9. Put to sleep\n");
    printf("10. Reward pet\n");
    printf("11. Print emotions\n");
    printf("12. About\n");
    printf("0. Exit\n");
    printf("\n");
    printf("Input your selection: ");
}

void executeAction(int option) { // Execute an action based on user input
    int petDataModified = 0;
    fflush(stdin);
    int petIndex;

    switch (option) {
        case 1:
            addPet(); // Add a new pet
            petDataModified = 1;
            break;
        case 2: // Update pet's name
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);

            if (petIndex >= 1 && petIndex <= numPets) {
                fflush(stdin);
                char name[MAX_NAME_LENGTH];
                printf("Enter the new name of the pet: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';
                pet_update(petIndex, name);
                printf("Pet information updated successfully.\n");
                savePetData();
            } else {
                printf("Invalid pet selection.\n");
            }
            break;
        case 3: // Delete a pet
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);
            delete_pet(petIndex);
            printf("Pet deleted successfully.\n");
            break;
        case 4: // Feed a pet
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);
            if (pets[petIndex - 1].energy < 5) {
                printf("Pet's energy level is too low. Put the pet to sleep.\n");
            } else {
                feedPet(petIndex);
                printf("Pet fed successfully.\n");
            }
            break;
        case 5: // Give water to a pet
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);
            if (pets[petIndex - 1].energy < 5) {
                printf("Pet's energy level is too low. Put the pet to sleep.\n");
            } else {
                give_water(petIndex);
                printf("Water given to pet.\n");
            }
            break;
        case 6: // Play games with a pet
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);
            if (pets[petIndex - 1].energy < 5) {
                printf("Pet's energy level is too low. Put the pet to sleep.\n");
            } else {
                play_games(petIndex);
                printf("Played games with pet.\n");
            }
            break;
        case 7: // Clean a pet
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);
            if (pets[petIndex - 1].energy < 5) {
                printf("Pet's energy level is too low. Put the pet to sleep.\n");
            } else {
                clean_pet(petIndex);
                printf("Pet cleaned successfully.\n");
            }
            break;
        case 8: // Love a pet
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);
            if (pets[petIndex - 1].happiness < 5) {
                printf("Pet's happiness level is too low. Take actions to increase happiness.\n");
            } else {
                love_pet(petIndex);
                printf("Pet loved.\n");
            }
            break;
        case 9: // Put a pet to sleep
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);
            put_to_sleep(petIndex);
            printf("Pet put to sleep.\n");
            break;
        case 10: // Reward a pet
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);
            if (pets[petIndex - 1].happiness < 5) {
                printf("Pet's happiness level is too low. Take actions to increase happiness.\n");
            } else {
                reward_pet(petIndex);
                printf("Pet rewarded.\n");
            }
            break;
        case 11: // Print pet's emotion
            printEmotion();
            break;
        case 12: // Display about information
            about();
            break;            
        case 0: // Exit the game
            if (petDataModified) {
                savePetData();
                printf("Pet data saved successfully. Exiting The Game...\n");
            } else {
                printf("Exiting The Game...\n");
            }
            break;
        default:
            printf("Invalid option. Please try again.\n");
            break;
    }
}