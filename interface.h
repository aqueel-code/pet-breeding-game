#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>

typedef struct {
    char type[20];
    char name[20];
    int energy;
    int happiness;
    char mood[50];
} Pet;

extern int numPets;
extern int gameRunning;
extern int timeToCheckEnergy;
extern int timeToCheckHappiness;

void initialize();
void cleanup();
void loadPetData();
void savePetData();
void addPet();
void displayPets();
void feedPet();
void pet_update(int petIndex, const char* newName);
void delete_pet(int petIndex);
void give_water(int petIndex);
void play_games(int petIndex);
void clean_pet(int petIndex);
void love_pet(int petIndex);
void put_to_sleep(int petIndex);
void reward_pet(int petIndex);
void showMenu();
void printEmotion();
void checkEnergyLevel(int petIndex);
void checkHappinessLevel(int petIndex);
void handleLowEnergy(int petIndex);
void handleLowHappiness(int petIndex);
void loadMoodsFromFile(const char* fileName);
void executeAction(int option);

const char* getPetMoodTextEmoji(int energyLevel, int happinessLevel);
const char* calculatePetMood(int energyLevel, int happinessLevel);
#endif