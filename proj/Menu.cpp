#include "Menu.h"
#include "TeamTypes.h"
#include "Utils.h"

#include <iostream>

Position readPosition() {
    while (true) {
        std::cout << "Positions: Goalkeeper, Defender, Midfielder, Winger, Forward\n";
        std::string value = safeInputText("Position: ");
        Position position = stringToPosition(value);
        if (position != UnknownPosition) {
            return position;
        }
        std::cout << "Invalid position.\n";
    }
}

Player readPlayerFromConsole() {
    std::string name = safeInputText("Player name: ");
    int number = readInt("Number: ");
    Position position = readPosition();
    double salary = readDouble("Salary: ");
    double transferFee = readDouble("Transfer fee: ");
    int matches = readInt("Played matches: ");
    int goals = readInt("Goals: ");
    return Player(name, number, position, salary, transferFee, matches, goals);
}

void addTeamMenu(ChampionshipCollection& collection) {
    if (!collection.hasActive()) {
        std::cout << "Create or load a season first.\n";
        return;
    }

    std::cout << "Team type:\n";
    std::cout << "1. Attacking\n";
    std::cout << "2. Defensive\n";
    std::cout << "3. Balanced\n";
    int typeChoice = readInt("Choice: ");
    std::string type;
    if (typeChoice == 1) type = "Attacking";
    else if (typeChoice == 2) type = "Defensive";
    else if (typeChoice == 3) type = "Balanced";
    else {
        std::cout << "Invalid team type.\n";
        return;
    }

    std::string name = safeInputText("Team name: ");
    std::string coach = safeInputText("Coach name: ");
    std::string stadium = safeInputText("Stadium name: ");
    double budget = readDouble("Budget: ");

    Team* team = createTeamByType(type, name, coach, stadium, budget);
    std::string error;
    if (collection.active().addTeam(team, error)) {
        std::cout << "Team added.\n";
    } else {
        std::cout << "Team was not added: " << error << "\n";
    }
}

void addOrBuyPlayerMenu(ChampionshipCollection& collection) {
    if (!collection.hasActive()) {
        std::cout << "Create or load a season first.\n";
        return;
    }
    std::string teamName = safeInputText("Team name: ");
    int teamIndex = collection.active().findTeamIndexByName(teamName);
    if (teamIndex == -1) {
        std::cout << "Team not found.\n";
        return;
    }
    Player player = readPlayerFromConsole();
    bool purchase = askYesNo("Is this a transfer purchase");
    std::string error;
    bool success;
    if (purchase) {
        success = collection.active().getTeam(teamIndex)->buyPlayer(player, error);
    } else {
        success = collection.active().getTeam(teamIndex)->addPlayer(player, error);
    }
    if (success) {
        std::cout << "Player added.\n";
    } else {
        std::cout << "Operation failed: " << error << "\n";
    }
}

void removePlayerMenu(ChampionshipCollection& collection) {
    if (!collection.hasActive()) {
        std::cout << "Create or load a season first.\n";
        return;
    }
    std::string teamName = safeInputText("Team name: ");
    int teamIndex = collection.active().findTeamIndexByName(teamName);
    if (teamIndex == -1) {
        std::cout << "Team not found.\n";
        return;
    }
    std::string playerName = safeInputText("Player name: ");
    std::string error;
    if (collection.active().getTeam(teamIndex)->removePlayer(playerName, error)) {
        std::cout << "Player removed.\n";
    } else {
        std::cout << "Operation failed: " << error << "\n";
    }
}

void swapPlayersMenu(ChampionshipCollection& collection) {
    if (!collection.hasActive()) {
        std::cout << "Create or load a season first.\n";
        return;
    }
    std::string firstTeamName = safeInputText("First team: ");
    std::string secondTeamName = safeInputText("Second team: ");
    int firstIndex = collection.active().findTeamIndexByName(firstTeamName);
    int secondIndex = collection.active().findTeamIndexByName(secondTeamName);
    if (firstIndex == -1 || secondIndex == -1 || firstIndex == secondIndex) {
        std::cout << "Invalid teams.\n";
        return;
    }
    std::string firstPlayer = safeInputText("Player from first team: ");
    std::string secondPlayer = safeInputText("Player from second team: ");
    std::string error;
    if (collection.active().getTeam(firstIndex)->swapPlayerWith(
            *collection.active().getTeam(secondIndex), firstPlayer, secondPlayer, error)) {
        std::cout << "Players swapped.\n";
    } else {
        std::cout << "Swap failed: " << error << "\n";
    }
}

void teamInfoMenu(ChampionshipCollection& collection) {
    if (!collection.hasActive()) {
        std::cout << "Create or load a season first.\n";
        return;
    }
    std::string teamName = safeInputText("Team name: ");
    int index = collection.active().findTeamIndexByName(teamName);
    if (index == -1) {
        std::cout << "Team not found.\n";
        return;
    }
    collection.active().getTeam(index)->printInfo();
}

void printMainMenu() {
    printSeparator();
    std::cout << "Football Championship - OOP project\n";
    std::cout << "1. Create empty season\n";
    std::cout << "2. Create demo season\n";
    std::cout << "3. Add team to active season\n";
    std::cout << "4. Add or buy player\n";
    std::cout << "5. Remove player\n";
    std::cout << "6. Swap players between teams\n";
    std::cout << "7. Generate schedule\n";
    std::cout << "8. Play next round\n";
    std::cout << "9. Play all remaining rounds\n";
    std::cout << "10. Show standings and awards\n";
    std::cout << "11. Show team info\n";
    std::cout << "12. Show schedule\n";
    std::cout << "13. List seasons\n";
    std::cout << "14. Switch active season\n";
    std::cout << "15. Save all seasons\n";
    std::cout << "16. Load seasons\n";
    std::cout << "0. Exit\n";
}
