#ifndef PLAYER_H
#define PLAYER_H

#include "Position.h"

#include <fstream>
#include <string>

class Player {
public:
    Player();
    Player(const std::string& playerName, int playerNumber, Position playerPosition,
        double playerSalary, double playerTransferFee, int matches, int scoredGoals);

    const std::string& getName() const;
    int getNumber() const;
    Position getPosition() const;
    double getSalary() const;
    double getTransferFee() const;
    int getPlayedMatches() const;
    int getGoals() const;

    void addPlayedMatch();
    void addGoal();
    void resetSeasonStats();
    void regulateSalary(int teamMatches);
    void print() const;
    void save(std::ofstream& out) const;
private:
    std::string name;
    int number;
    Position position;
    double salary;
    double transferFee;
    int playedMatches;
    int goals;


};

#endif
