#ifndef TEAM_H
#define TEAM_H

#include "Constants.h"
#include "Player.h"

#include <fstream>
#include <string>
#include <vector>

class Team {

public:
    Team();
    Team(const std::string& teamName, const std::string& coach,
        const std::string& stadium, double initialBudget);
    virtual ~Team();

    virtual Team* clone() const = 0;
    virtual std::string typeName() const = 0;
    virtual int minFor(Position position) const = 0;
    virtual bool styleAllowsPurchase(const Player& player, std::string& error) const;

    const std::string& getName() const;
    double getBudget() const;
    int getPoints() const;
    int getGoalsFor() const;
    int getGoalsAgainst() const;
    int goalDifference() const;
    int totalMatches() const;
    int playerCount() const;

    Player& playerAt(int index);
    const Player& playerAt(int index) const;
    int findPlayerIndexByName(const std::string& playerName) const;

    bool addPlayer(const Player& player, std::string& error);
    void addPlayerUnchecked(const Player& player);
    bool removePlayer(const std::string& playerName, std::string& error);
    bool canBuyPlayer(const Player& player, std::string& error) const;
    bool buyPlayer(const Player& player, std::string& error);
    bool swapPlayerWith(Team& other, const std::string& myPlayerName,
        const std::string& otherPlayerName, std::string& error);
    bool rosterIsValid(std::string& error) const;

    void updateAfterMatch(int scored, int conceded);
    void startNewSeason();
    void regulatePlayerSalaries();
    void setStats(int loadedWins, int loadedDraws, int loadedLosses, int loadedPoints,
        int loadedGoalsFor, int loadedGoalsAgainst);
    void printInfo() const;
    void printShortRoster() const;
    void save(std::ofstream& out) const;
protected:
    std::string name;
    std::string coachName;
    std::string stadiumName;
    std::vector<Player> players;
    double budget;
    int wins;
    int draws;
    int losses;
    int points;
    int goalsFor;
    int goalsAgainst;

    int countPositionIn(const std::vector<Player>& list, Position position) const;
    bool hasDuplicateNameOrNumber(const std::vector<Player>& list, std::string& error) const;
    bool rosterListIsValid(const std::vector<Player>& list, std::string& error) const;

};

#endif
