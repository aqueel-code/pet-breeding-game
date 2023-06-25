#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>

// Structure to represent a pet
struct Pet {
    char type[20]; // Pet's type
    char name[20]; // Pet's name
    int energy; // Pet's energy level
    int happiness; // Pet's happiness level
    char mood[50]; // Pet's current mood
};

// External variables
extern int numPets; // Total number of pets
extern int gameRunning; // Flag to indicate if the game is running
extern int timeToCheckEnergy; // Time interval to check energy levels
extern int timeToCheckHappiness; // Time interval to check happiness levels

// Function declarations
void initialize(); // Initialize the game
void loadPetData(); // Load pet data from file
void savePetData(); // Save pet data to file
void addPet(); // Add a new pet
void displayPets(); // Display all pets
void feedPet(); // Feed a pet
void pet_update(int petIndex, const char* newName); // Update pet's name
void delete_pet(int petIndex); // Delete a pet
void give_water(int petIndex); // Give water to a pet
void play_games(int petIndex); // Play games with a pet
void clean_pet(int petIndex); // Clean a pet
void love_pet(int petIndex); // Show love to a pet
void put_to_sleep(int petIndex); // Put a pet to sleep
void reward_pet(int petIndex); // Reward a pet
void showMenu(int petIndex); // Display the menu
void printEmotion(); // Print emotions of all pets
void checkEnergyLevel(int petIndex); // Check energy level of a pet
void checkHappinessLevel(int petIndex); // Check happiness level of a pet
void handleLowEnergy(int petIndex); // Handle low energy level of a pet
void handleLowHappiness(int petIndex); // Handle low happiness level of a pet
void loadMoodsFromFile(const char* fileName); // Load moods from a file
void executeAction(int option); // Execute an action based on user input
void about(); //Information about me and the game

const char* getPetMoodTextEmoji(int energy, int happiness); // Get text emoji for pet's mood
const char* calculatePetMood(int petIndex, struct Pet* pets); // Calculate pet's mood

#endif