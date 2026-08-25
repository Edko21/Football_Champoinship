#ifndef MATCH_H
#define MATCH_H

#include "Lineup.h"

#include <fstream>
#include <string>
#include <vector>

struct GoalRecord {
    int teamIndex;
    std::string playerName;
};

class Match {
public:
    Match();
    Match(int matchRound, int homeTeamIndex, int awayTeamIndex);

    int getRound() const;
    bool isPlayed() const;
    int getHomeIndex() const;
    int getAwayIndex() const;

    void setLoadedState(int loadedRound, int loadedHome, int loadedAway,
        bool loadedPlayed, int loadedHomeGoals, int loadedAwayGoals);
    void addLoadedScorer(const GoalRecord& record);
    void printShort(const std::vector<Team*>& teams) const;
    bool play(std::vector<Team*>& teams, int totalRounds, bool automaticLineups, bool automaticResult);
    void save(std::ofstream& out) const;
private:
    int round;
    int homeIndex;
    int awayIndex;
    int homeGoals;
    int awayGoals;
    bool played;
    std::vector<GoalRecord> scorers;

    bool validTeamIndex(int index, const std::vector<Team*>& teams) const;
};

#endif
