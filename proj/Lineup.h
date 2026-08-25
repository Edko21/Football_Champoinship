#ifndef LINEUP_H
#define LINEUP_H

#include "Team.h"

#include <string>
#include <vector>

class Lineup {
public:
    Lineup(Team* lineupTeam);

    bool addPlayerByName(const std::string& playerName, std::string& error);
    bool isValid(std::string& error) const;
    bool chooseAutomatically(bool lateRound, std::string& error);
    bool chooseFromConsole(bool lateRound);
    bool containsPlayerName(const std::string& playerName) const;
    std::string pickScorerName() const;
    void markPlayedMatch();
    void print() const;
private:
    Team* team;
    std::vector<int> selectedIndices;

    bool containsIndex(int index) const;
    int countPosition(Position position) const;
    int findCandidate(Position position, bool preferLowMatches) const;
    int findAnyCandidate(bool preferLowMatches) const;
};

#endif
