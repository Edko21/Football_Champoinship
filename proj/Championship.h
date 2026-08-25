#ifndef CHAMPIONSHIP_H
#define CHAMPIONSHIP_H

#include "Match.h"

#include <fstream>
#include <vector>

struct ScorerCandidate {
    int teamIndex;
    int playerIndex;
};

class Championship {
public:
    Championship();
    Championship(int championshipYear);
    Championship(const Championship& other);
    Championship& operator=(const Championship& other);
    ~Championship();

    int getYear() const;
    bool isFinished() const;
    int getTeamCount() const;
    Team* getTeam(int index);
    const Team* getTeam(int index) const;
    int totalRounds() const;
    int findTeamIndexByName(const std::string& teamName) const;

    bool addTeam(Team* team, std::string& error);
    void addLoadedTeam(Team* team);
    void addLoadedMatch(const Match& match);
    void setLoadedState(int loadedCurrentRound, bool loadedFinished, bool loadedScheduleGenerated);
    bool generateSchedule(std::string& error);
    bool playOneRound(bool automaticLineups, bool automaticResult);
    void playAllRounds(bool automaticLineups, bool automaticResult);
    std::vector<int> standingsOrder() const;
    void printStandings() const;
    void printAwards() const;
    void printSchedule() const;
    void save(std::ofstream& out) const;
private:
    int year;
    std::vector<Team*> teams;
    std::vector<Match> matches;
    int currentRound;
    bool finished;
    bool scheduleGenerated;

    void clearTeams();
    void copyFrom(const Championship& other);
    bool teamComesBefore(int firstIndex, int secondIndex) const;


};

#endif
