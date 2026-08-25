#include "ChampionshipCollection.h"
#include "Menu.h"
#include "Utils.h"

#include <cstdlib>
#include <ctime>
#include <exception>
#include <iostream>
#include <new>

int main() {
    srand((unsigned int)time(0));
    ChampionshipCollection collection;

    while (true) {
        printMainMenu();
        int choice = readInt("Choice: ");

        try {
            if (choice == 0) {
                if (askYesNo("Save before exit")) {
                    std::string filename = safeInputText("File name (example championships.txt): ");
                    std::string error;
                    if (collection.saveToFile(filename, error)) {
                        std::cout << "Saved.\n";
                    } else {
                        std::cout << "Save failed: " << error << "\n";
                    }
                }
                std::cout << "Goodbye.\n";
                break;
            } else if (choice == 1) {
                collection.createEmptySeason();
            } else if (choice == 2) {
                collection.createDemoSeason();
            } else if (choice == 3) {
                addTeamMenu(collection);
            } else if (choice == 4) {
                addOrBuyPlayerMenu(collection);
            } else if (choice == 5) {
                removePlayerMenu(collection);
            } else if (choice == 6) {
                swapPlayersMenu(collection);
            } else if (choice == 7) {
                if (!collection.hasActive()) {
                    std::cout << "Create or load a season first.\n";
                } else {
                    std::string error;
                    if (collection.active().generateSchedule(error)) {
                        std::cout << "Schedule generated.\n";
                    } else {
                        std::cout << "Cannot generate schedule: " << error << "\n";
                    }
                }
            } else if (choice == 8) {
                if (!collection.hasActive()) {
                    std::cout << "Create or load a season first.\n";
                } else {
                    bool automaticLineups = askYesNo("Use automatic lineups");
                    bool automaticResult = askYesNo("Use automatic results");
                    collection.active().playOneRound(automaticLineups, automaticResult);
                }
            } else if (choice == 9) {
                if (!collection.hasActive()) {
                    std::cout << "Create or load a season first.\n";
                } else {
                    bool automaticLineups = askYesNo("Use automatic lineups");
                    bool automaticResult = askYesNo("Use automatic results");
                    collection.active().playAllRounds(automaticLineups, automaticResult);
                }
            } else if (choice == 10) {
                if (!collection.hasActive()) {
                    std::cout << "Create or load a season first.\n";
                } else {
                    collection.active().printStandings();
                    collection.active().printAwards();
                }
            } else if (choice == 11) {
                teamInfoMenu(collection);
            } else if (choice == 12) {
                if (!collection.hasActive()) {
                    std::cout << "Create or load a season first.\n";
                } else {
                    collection.active().printSchedule();
                }
            } else if (choice == 13) {
                collection.listSeasons();
            } else if (choice == 14) {
                collection.switchSeason();
            } else if (choice == 15) {
                std::string filename = safeInputText("File name (example championships.txt): ");
                std::string error;
                if (collection.saveToFile(filename, error)) {
                    std::cout << "Saved.\n";
                } else {
                    std::cout << "Save failed: " << error << "\n";
                }
            } else if (choice == 16) {
                std::string filename = safeInputText("File name: ");
                std::string error;
                if (collection.loadFromFile(filename, error)) {
                    std::cout << "Loaded.\n";
                } else {
                    std::cout << "Load failed: " << error << "\n";
                }
            } else {
                std::cout << "Unknown menu choice.\n";
            }
        } catch (const std::bad_alloc&) {
            std::cout << "Memory allocation failed.\n";
        } catch (const std::exception& ex) {
            std::cout << "Unexpected error: " << ex.what() << "\n";
        }
    }

    return 0;
}
