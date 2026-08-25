#include "Team.h"
#include "Utils.h"

#include <iostream>

int Team::countPositionIn(const std::vector<Player>& list, Position position) const {
    int count = 0;
    for (int i = 0; i < (int)list.size(); i++) {
        if (list[i].getPosition() == position) {
            count++;
        }
    }
    return count;
}

bool Team::hasDuplicateNameOrNumber(const std::vector<Player>& list, std::string& error) const {
    for (int i = 0; i < (int)list.size(); i++) {
        for (int j = i + 1; j < (int)list.size(); j++) {
            if (list[i].getName() == list[j].getName()) {
                error = "Duplicate player name: " + list[i].getName();
                return true;
            }
            if (list[i].getNumber() == list[j].getNumber()) {
                error = "Duplicate player number: " + std::to_string(list[i].getNumber());
                return true;
            }
        }
    }
    return false;
}

bool Team::rosterListIsValid(const std::vector<Player>& list, std::string& error) const {
    if ((int)list.size() > MAX_TEAM_PLAYERS) {
        error = "A team cannot have more than 20 players.";
        return false;
    }
    if (hasDuplicateNameOrNumber(list, error)) {
        return false;
    }
    if (countPositionIn(list, Goalkeeper) != 2) {
        error = "A valid team must have exactly 2 goalkeepers.";
        return false;
    }

    Position positions[5] = {Goalkeeper, Defender, Midfielder, Winger, Forward};
    for (int i = 0; i < 5; i++) {
        int required = minFor(positions[i]);
        if (countPositionIn(list, positions[i]) < required) {
            error = "Not enough players at position " + positionToString(positions[i]) + ".";
            return false;
        }
    }
    return true;
}

Team::Team() {
    name = "";
    coachName = "";
    stadiumName = "";
    budget = 0.0;
    wins = draws = losses = points = goalsFor = goalsAgainst = 0;
}

Team::Team(const std::string& teamName, const std::string& coach,
           const std::string& stadium, double initialBudget) {
    name = teamName;
    coachName = coach;
    stadiumName = stadium;
    budget = initialBudget;
    wins = draws = losses = points = goalsFor = goalsAgainst = 0;
}

Team::~Team() {
}

bool Team::styleAllowsPurchase(const Player& player, std::string& error) const {
    error = "";
    return true;
}

const std::string& Team::getName() const {
    return name;
}

double Team::getBudget() const {
    return budget;
}

int Team::getPoints() const {
    return points;
}

int Team::getGoalsFor() const {
    return goalsFor;
}

int Team::getGoalsAgainst() const {
    return goalsAgainst;
}

int Team::goalDifference() const {
    return goalsFor - goalsAgainst;
}

int Team::totalMatches() const {
    return wins + draws + losses;
}

int Team::playerCount() const {
    return (int)players.size();
}

Player& Team::playerAt(int index) {
    return players[index];
}

const Player& Team::playerAt(int index) const {
    return players[index];
}

int Team::findPlayerIndexByName(const std::string& playerName) const {
    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].getName() == playerName) {
            return i;
        }
    }
    return -1;
}

bool Team::addPlayer(const Player& player, std::string& error) {
    if ((int)players.size() >= MAX_TEAM_PLAYERS) {
        error = "The team already has 20 players.";
        return false;
    }
    if (player.getPosition() == UnknownPosition) {
        error = "Unknown player position.";
        return false;
    }
    if (containsFileSeparator(player.getName())) {
        error = "Player name cannot contain the file separator character.";
        return false;
    }
    if (findPlayerIndexByName(player.getName()) != -1) {
        error = "A player with this name already exists in the team.";
        return false;
    }
    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].getNumber() == player.getNumber()) {
            error = "A player with this number already exists in the team.";
            return false;
        }
    }
    if (player.getPosition() == Goalkeeper && countPositionIn(players, Goalkeeper) >= 2) {
        error = "The team cannot have more than 2 goalkeepers.";
        return false;
    }
    players.push_back(player);
    error = "";
    return true;
}

void Team::addPlayerUnchecked(const Player& player) {
    players.push_back(player);
}

bool Team::removePlayer(const std::string& playerName, std::string& error) {
    int index = findPlayerIndexByName(playerName);
    if (index == -1) {
        error = "Player not found.";
        return false;
    }
    players.erase(players.begin() + index);
    error = "";
    return true;
}

bool Team::canBuyPlayer(const Player& player, std::string& error) const {
    if (budget < player.getTransferFee()) {
        error = "The team does not have enough budget for this transfer.";
        return false;
    }
    if ((int)players.size() >= MAX_TEAM_PLAYERS) {
        error = "The roster is full.";
        return false;
    }
    if (findPlayerIndexByName(player.getName()) != -1) {
        error = "A player with this name already exists in the team.";
        return false;
    }
    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].getNumber() == player.getNumber()) {
            error = "A player with this number already exists in the team.";
            return false;
        }
    }
    if (player.getPosition() == Goalkeeper && countPositionIn(players, Goalkeeper) >= 2) {
        error = "The team cannot have more than 2 goalkeepers.";
        return false;
    }
    if (!styleAllowsPurchase(player, error)) {
        return false;
    }
    error = "";
    return true;
}

bool Team::buyPlayer(const Player& player, std::string& error) {
    if (!canBuyPlayer(player, error)) {
        return false;
    }
    players.push_back(player);
    budget -= player.getTransferFee();
    error = "";
    return true;
}

bool Team::swapPlayerWith(Team& other, const std::string& myPlayerName,
                          const std::string& otherPlayerName, std::string& error) {
    int myIndex = findPlayerIndexByName(myPlayerName);
    int otherIndex = other.findPlayerIndexByName(otherPlayerName);
    if (myIndex == -1) {
        error = "Player not found in first team.";
        return false;
    }
    if (otherIndex == -1) {
        error = "Player not found in second team.";
        return false;
    }

    std::vector<Player> myNewPlayers = players;
    std::vector<Player> otherNewPlayers = other.players;

    Player temp = myNewPlayers[myIndex];
    myNewPlayers[myIndex] = otherNewPlayers[otherIndex];
    otherNewPlayers[otherIndex] = temp;

    std::string firstError;
    std::string secondError;
    if (!rosterListIsValid(myNewPlayers, firstError)) {
        error = "First team would become invalid: " + firstError;
        return false;
    }
    if (!other.rosterListIsValid(otherNewPlayers, secondError)) {
        error = "Second team would become invalid: " + secondError;
        return false;
    }

    players = myNewPlayers;
    other.players = otherNewPlayers;
    error = "";
    return true;
}

bool Team::rosterIsValid(std::string& error) const {
    return rosterListIsValid(players, error);
}

void Team::updateAfterMatch(int scored, int conceded) {
    goalsFor += scored;
    goalsAgainst += conceded;
    if (scored > conceded) {
        wins++;
        points += 3;
    } else if (scored == conceded) {
        draws++;
        points += 1;
    } else {
        losses++;
    }
}

void Team::startNewSeason() {
    wins = draws = losses = points = goalsFor = goalsAgainst = 0;
    for (int i = 0; i < (int)players.size(); i++) {
        players[i].resetSeasonStats();
    }
}

void Team::regulatePlayerSalaries() {
    int matches = totalMatches();
    for (int i = 0; i < (int)players.size(); i++) {
        players[i].regulateSalary(matches);
    }
}

void Team::setStats(int loadedWins, int loadedDraws, int loadedLosses, int loadedPoints,
                    int loadedGoalsFor, int loadedGoalsAgainst) {
    wins = loadedWins;
    draws = loadedDraws;
    losses = loadedLosses;
    points = loadedPoints;
    goalsFor = loadedGoalsFor;
    goalsAgainst = loadedGoalsAgainst;
}

void Team::printInfo() const {
    printSeparator();
    std::cout << "Team: " << name << " (" << typeName() << ")\n";
    std::cout << "Coach: " << coachName << "\n";
    std::cout << "Stadium: " << stadiumName << "\n";
    std::cout << "Budget: " << budget << "\n";
    std::cout << "Stats: W " << wins << ", D " << draws << ", L " << losses
              << ", Pts " << points << ", GF " << goalsFor
              << ", GA " << goalsAgainst << "\n";
    std::cout << "Roster (" << players.size() << "/" << MAX_TEAM_PLAYERS << "):\n";
    for (int i = 0; i < (int)players.size(); i++) {
        std::cout << "  ";
        players[i].print();
    }
}

void Team::printShortRoster() const {
    for (int i = 0; i < (int)players.size(); i++) {
        std::cout << "  " << (i + 1) << ". ";
        players[i].print();
    }
}

void Team::save(std::ofstream& out) const {
    out << "TEAM" << FILE_SEPARATOR
        << typeName() << FILE_SEPARATOR
        << name << FILE_SEPARATOR
        << coachName << FILE_SEPARATOR
        << stadiumName << FILE_SEPARATOR
        << budget << FILE_SEPARATOR
        << wins << FILE_SEPARATOR
        << draws << FILE_SEPARATOR
        << losses << FILE_SEPARATOR
        << points << FILE_SEPARATOR
        << goalsFor << FILE_SEPARATOR
        << goalsAgainst << FILE_SEPARATOR
        << players.size() << "\n";
    for (int i = 0; i < (int)players.size(); i++) {
        players[i].save(out);
    }
    out << "ENDTEAM\n";
}
