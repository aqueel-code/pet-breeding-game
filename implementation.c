#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "interface.h"

#define MAX_PETS 3
#define MAX_NAME_LENGTH 20
#define MAX_MOOD_LENGTH 50
#define MAX_NAME_LENGTH 20
#define MAX_MOODS 10
#define MAX_EMOTION_LENGTH 10
#define MAX_CONDITION_LENGTH 20
#define LOW_ENERGY_THRESHOLD 20
#define LOW_HAPPINESS_THRESHOLD 30

int gameRunning = 1;
int timeToCheckEnergy = 1;
int timeToCheckHappiness = 1;

bool petDataModified = false;

struct Pet {
    char type[20];
    char name[MAX_NAME_LENGTH];
    int energy;
    int happiness;
    char mood[MAX_MOOD_LENGTH];
};

struct Pet pets[MAX_PETS];
int numPets = 0;

void initialize() {
    // Load pet data from file
    loadPetData();
}

void cleanup() {
    // Save pet data to file
    if (petDataModified) {
        savePetData();
    } else {
        remove("pet_data.txt");
    }
}

void loadPetData() {
    FILE* file = fopen("pet_data.txt", "r");
    if (file == NULL) {
        printf("Error opening pet_data.txt file.\n");
        return;
    }

    while (numPets < MAX_PETS && fscanf(file, "%[^,],%[^,],%d,%d\n", pets[numPets].type, pets[numPets].name,
    &pets[numPets].energy, &pets[numPets].happiness) == 4) {
        numPets++;
    }

    if (fclose(file) != 0) {
        printf("Error closing pet_data.txt file.\n");
    }
}


void savePetData() {
    FILE *file = fopen("pet_data.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < numPets; i++) {
        fprintf(file, "%s,%s,%d,%d\n", pets[i].type, pets[i].name, pets[i].energy, pets[i].happiness);
    }

    if (fclose(file) != 0) {
        printf("Error closing pet_data.txt file.\n");
    }
}

void savePetOutput(const char* output) {
    FILE* file = fopen("pet_output.txt", "a");
    if (file == NULL) {
        printf("Error opening pet_output.txt file.\n");
        return;
    }

    fprintf(file, "%s\n", output);

    if (fclose(file) != 0) {
        printf("Error closing pet_output.txt file.\n");
    }
}

void addPet() {
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

    // Input validation
    if (strlen(type) == 0 || strlen(name) == 0) {
        printf("Invalid input. Type and name cannot be empty.\n");
        return;
    }

    // Check if the pet name already exists
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
    // Save the pet data to the file
    savePetData();
}

void displayPets() {
    printf("Available pets:\n");
    for (int i = 1; i <= numPets; i++) {
        printf("%d. %s - %s\n", i, pets[i-1].type, pets[i-1].name);
    }
}

void feedPet() {
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
        savePetData(); // Save pet data after making changes

        checkEnergyLevel(petIndex); // Check energy level after feeding
    } else {
        printf("Invalid pet selection.\n");
    }
}

void pet_update(int petIndex, const char* newName) {
    if (petIndex >= 1 && petIndex <= numPets) {
        strncpy(pets[petIndex - 1].name, newName, MAX_NAME_LENGTH);
        printf("Pet %s has been updated.\n", pets[petIndex - 1].name);
        //calculatePetMood(&pets[petIndex - 1]);
        // Calculate the pet's mood based on energy and happiness levels
if (pets[petIndex - 1].energy > 25 && pets[petIndex - 1].happiness > 50) {
    strcpy(pets[petIndex - 1].mood, "happy   :)");
} else if (pets[petIndex - 1].energy < 20 && pets[petIndex - 1].happiness < 20) {
    strcpy(pets[petIndex - 1].mood, "sad     :(");
} else if (pets[petIndex - 1].energy < 10 && pets[petIndex - 1].happiness < 10) {
    strcpy(pets[petIndex - 1].mood, "crying  :'(");
} else if (pets[petIndex - 1].energy < 5) {
    strcpy(pets[petIndex - 1].mood, "sleeping :zz");
} else if (pets[petIndex - 1].energy < 20) {
    strcpy(pets[petIndex - 1].mood, "hungry  :-o");
} else if (pets[petIndex - 1].energy > 40 || pets[petIndex - 1].happiness < 30) {
    strcpy(pets[petIndex - 1].mood, "wants a game <:0)");
} else {
    strcpy(pets[petIndex - 1].mood, "unknown");
}

        savePetData(); // Save the pet data after updating the name
    } else {
        printf("Invalid pet selection.\n");
    }
}


void delete_pet(int petIndex) {
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

void give_water(int petIndex) {
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

void play_games(int petIndex) {
    if (petIndex >= 1 && petIndex <= numPets) {
        petIndex--;

        if (pets[petIndex].energy < LOW_ENERGY_THRESHOLD) {
            printf("Pet's energy level is too low. Put the pet to sleep.\n");
            // Prompt the user to put the pet to sleep or perform other actions
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


void clean_pet(int petIndex) {
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

void love_pet(int petIndex) {
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

void put_to_sleep(int petIndex) {
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

void reward_pet(int petIndex) {
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

void handleLowEnergy(int petIndex) {
    printf("Warning: Pet %s has low energy.\n", pets[petIndex].name);
    printf("What would you like to do?\n");
    printf("1. Feed the pet\n");
    printf("2. Give water to the pet\n");
    printf("3. Put the pet to sleep\n");
    //printf("4. Return to the main menu\n");
    printf("Enter your choice: ");

    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            feedPet(petIndex);
            break;
        case 2:
            give_water(petIndex);
            break;
        case 3:
            put_to_sleep(petIndex);
            break;
/*      case 4:
            showMenu();
            break;
*/
        default:
            printf("Invalid choice.\n");
            break;
    }
}

void handleLowHappiness(int petIndex) {
    printf("Warning: Pet %s is unhappy.\n", pets[petIndex].name);
    printf("What would you like to do?\n");
    printf("1. Play games with the pet\n");
    printf("2. Clean the pet\n");
    printf("3. Show love and affection to the pet\n");
    //printf("4. Return to the main menu\n");
    printf("Enter your choice: ");

    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            play_games(petIndex);
            break;
        case 2:
            clean_pet(petIndex);
            break;
        case 3:
            love_pet(petIndex);
            break;
/*      case 4:
            showMenu();
            break;
*/
        default:
            printf("Invalid choice.\n");
            break;
    }
}

void checkEnergyLevel(int petIndex) {
    if (pets[petIndex].energy < LOW_ENERGY_THRESHOLD) {
        handleLowEnergy(petIndex);
    }
}

void checkHappinessLevel(int petIndex) {
    if (pets[petIndex].happiness < LOW_HAPPINESS_THRESHOLD) {
        handleLowHappiness(petIndex);
    }
}

void printEmotion() {
    const char* emotionFile = "emotion.txt";  // Predefined file name

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

Mood moods[MAX_MOODS];
int numMoods = 0;

void loadMoodsFromFile(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file %s.\n", fileName);
        return;
    }

    char line[100];
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
        Mood mood = moods[i];
        if ((strcmp(mood.energyCondition, "Not important") == 0 ||
             pets[petIndex].energy >= atoi(mood.energyCondition)) &&
            (strcmp(mood.happinessCondition, "Not important") == 0 ||
             pets[petIndex].happiness >= atoi(mood.happinessCondition))) {
            return i;
        }
    }
    return -1;
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

void about() {
    printf("This programme was developed by Mohamed Ifham Aqueel Imthiyas.\n");
    printf("Contact info: 200504123@ogrenci.harran.edu.tr\n");
    printf("Version: 1.0.0-b.1 Beta\n");
    printf("Release date: 15.06.2023\n");
    printf("Objective: A Pet Breeding and Simulation Game implemented in C. Interact with virtual pets, meet their needs, and monitor their energy and happiness levels.\n");
    printf("\n");
}

void showMenu() {
    printf(".:: Pet Breeding and Simulation Game ::.\n");
    printf("\n");
    printf("Available Pets:\n");
    printf("------------------------------------------------------------------\n");
    printf("| Index | Type     | Name  | Energy | Happiness |       Mood     |\n");
    printf("------------------------------------------------------------------\n");
    
    for (int i = 0; i < numPets; i++) {
        printf("|   %d   | %-8s | %-5s |   %d   |      %d   | %-14s |\n",
               i + 1, pets[i].type, pets[i].name, pets[i].energy, pets[i].happiness, pets[i].mood);
        printf("------------------------------------------------------------------\n");
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

void executeAction(int option) {
    int petDataModified = 0;
    fflush(stdin);
    int petIndex;

    switch (option) {
        case 1:
            // Code to add a pet
            addPet();
            petDataModified = 1;
            break;
        case 2:
            // Code to update pet information
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);

            if (petIndex >= 1 && petIndex <= numPets) {
                // Clear the input buffer
                fflush(stdin);

                char name[MAX_NAME_LENGTH];
                printf("Enter the new name of the pet: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';

                pet_update(petIndex, name);
                printf("Pet information updated successfully.\n");

                savePetData(); // Save the pet data after updating the name
            } else {
                printf("Invalid pet selection.\n");
            }
            break;
        case 3:
            // Code to delete a pet
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);
            delete_pet(petIndex);
            printf("Pet deleted successfully.\n");
            break;
        case 4:
            // Code to feed a pet
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);
            if (pets[petIndex - 1].energy < 5) {
                printf("Pet's energy level is too low. Put the pet to sleep.\n");
            } else {
                feedPet(petIndex);
                printf("Pet fed successfully.\n");
            }
            break;
        case 5:
            // Code to give water to a pet
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);
            if (pets[petIndex - 1].energy < 5) {
                printf("Pet's energy level is too low. Put the pet to sleep.\n");
            } else {
                give_water(petIndex);
                printf("Water given to pet.\n");
            }
            break;
        case 6:
            // Code to play games with a pet
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);
            if (pets[petIndex - 1].energy < 5) {
                printf("Pet's energy level is too low. Put the pet to sleep.\n");
            } else {
                play_games(petIndex);
                printf("Played games with pet.\n");
            }
            break;
        case 7:
            // Code to clean a pet
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);
            if (pets[petIndex - 1].energy < 5) {
                printf("Pet's energy level is too low. Put the pet to sleep.\n");
            } else {
                clean_pet(petIndex);
                printf("Pet cleaned successfully.\n");
            }
            break;
        case 8:
            // Code to love a pet
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);
            if (pets[petIndex - 1].happiness < 5) {
                printf("Pet's happiness level is too low. Take actions to increase happiness.\n");
            } else {
                love_pet(petIndex);
                printf("Pet loved.\n");
            }
            break;
        case 9:
            // Code to put a pet to sleep
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);
            put_to_sleep(petIndex);
            printf("Pet put to sleep.\n");
            break;
        case 10:
            // Code to reward a pet
            printf("Enter the index of the pet: ");
            scanf("%d", &petIndex);
            if (pets[petIndex - 1].happiness < 5) {
                printf("Pet's happiness level is too low. Take actions to increase happiness.\n");
            } else {
                reward_pet(petIndex);
                printf("Pet rewarded.\n");
            }
            break;
        case 11:
            printEmotion();
            break;
        case 12:
            about();
            break;            
        case 0:
            if (petDataModified) {
                savePetData();
                cleanup();
                printf("Pet data saved successfully. Exiting The Game...\n");
            } else {
                cleanup();
                printf("Exiting The Game...\n");
            }
            break;
        default:
            printf("Invalid option. Please try again.\n");
            break;
    }
}