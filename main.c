#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"

int main() {
    loadMoodsFromFile("emotion.txt");
    int option;
    int petIndex;

    do {
        showMenu(petIndex);
        scanf("%d", &option);
        getchar();

        executeAction(option);

        if (option != 0) {
            while (gameRunning) {
                showMenu(petIndex);
                scanf("%d", &option);
                getchar();

                executeAction(option);

                if (option == 0) {
                    break;
                }

                if (timeToCheckEnergy) {
                    for (int i = 0; i < numPets; i++) {
                        checkEnergyLevel(i);
                    }
                }
                if (timeToCheckHappiness) {
                    for (int i = 0; i < numPets; i++) {
                        checkHappinessLevel(i);
                    }
                }
            }
        }
    } while (option != 0);

    return 0;
}
