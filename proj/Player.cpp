#include "Player.h"
#include "Constants.h"
#include "Utils.h"

#include <iostream>

Player::Player() {
    name = "";
    number = 0;
    position = UnknownPosition;
    salary = 0.0;
    transferFee = 0.0;
    playedMatches = 0;
    goals = 0;
}

Player::Player(const std::string& playerName, int playerNumber, Position playerPosition,
               double playerSalary, double playerTransferFee, int matches, int scoredGoals) {
    name = playerName;
    number = playerNumber;
    position = playerPosition;
    salary = playerSalary;
    transferFee = playerTransferFee;
    playedMatches = matches;
    goals = scoredGoals;
}

const std::string& Player::getName() const {
    return name;
}

int Player::getNumber() const {
    return number;
}

Position Player::getPosition() const {
    return position;
}

double Player::getSalary() const {
    return salary;
}

double Player::getTransferFee() const {
    return transferFee;
}

int Player::getPlayedMatches() const {
    return playedMatches;
}

int Player::getGoals() const {
    return goals;
}

void Player::addPlayedMatch() {
    playedMatches++;
}

void Player::addGoal() {
    goals++;
}

void Player::resetSeasonStats() {
    playedMatches = 0;
    goals = 0;
}

void Player::regulateSalary(int teamMatches) {
    if (teamMatches > 0 && playedMatches * 2 > teamMatches) {
        salary *= 1.20;
    }
    if (playedMatches < 7) {
        salary *= 0.90;
    }
    if (goals > 5) {
        salary *= 1.10;
    }
    if (goals > 10) {
        salary *= 1.20;
    }
    if (goals < 5 && position == Forward) {
        salary *= 0.85;
    }
}

void Player::print() const {
    printPadded(name, 22);
    printPadded(positionToString(position), 13);
    std::cout << "No " << number
              << ", salary " << salary
              << ", transfer " << transferFee
              << ", matches " << playedMatches
              << ", goals " << goals << "\n";
}

void Player::save(std::ofstream& out) const {
    out << "PLAYER" << FILE_SEPARATOR
        << name << FILE_SEPARATOR
        << number << FILE_SEPARATOR
        << positionToString(position) << FILE_SEPARATOR
        << salary << FILE_SEPARATOR
        << transferFee << FILE_SEPARATOR
        << playedMatches << FILE_SEPARATOR
        << goals << "\n";
}
