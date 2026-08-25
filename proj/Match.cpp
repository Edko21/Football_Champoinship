#include "Match.h"
#include "Constants.h"
#include "Utils.h"

#include <cstdlib>
#include <iostream>

bool Match::validTeamIndex(int index, const std::vector<Team*>& teams) const {
    return index >= 0 && index < (int)teams.size();
}

Match::Match() {
    round = 0;
    homeIndex = 0;
    awayIndex = 0;
    homeGoals = 0;
    awayGoals = 0;
    played = false;
}

Match::Match(int matchRound, int homeTeamIndex, int awayTeamIndex) {
    round = matchRound;
    homeIndex = homeTeamIndex;
    awayIndex = awayTeamIndex;
    homeGoals = 0;
    awayGoals = 0;
    played = false;
}

int Match::getRound() const {
    return round;
}

bool Match::isPlayed() const {
    return played;
}

int Match::getHomeIndex() const {
    return homeIndex;
}

int Match::getAwayIndex() const {
    return awayIndex;
}

void Match::setLoadedState(int loadedRound, int loadedHome, int loadedAway,
                           bool loadedPlayed, int loadedHomeGoals, int loadedAwayGoals) {
    round = loadedRound;
    homeIndex = loadedHome;
    awayIndex = loadedAway;
    played = loadedPlayed;
    homeGoals = loadedHomeGoals;
    awayGoals = loadedAwayGoals;
}

void Match::addLoadedScorer(const GoalRecord& record) {
    scorers.push_back(record);
}

void Match::printShort(const std::vector<Team*>& teams) const {
    if (!validTeamIndex(homeIndex, teams) || !validTeamIndex(awayIndex, teams)) {
        std::cout << "Invalid match data.\n";
        return;
    }
    std::cout << "Round " << round << ": "
              << teams[homeIndex]->getName() << " vs " << teams[awayIndex]->getName();
    if (played) {
        std::cout << " " << homeGoals << ":" << awayGoals;
    } else {
        std::cout << " (not played)";
    }
    std::cout << "\n";
}

bool Match::play(std::vector<Team*>& teams, int totalRounds, bool automaticLineups, bool automaticResult) {
    if (played) {
        std::cout << "This match is already played.\n";
        return true;
    }
    if (!validTeamIndex(homeIndex, teams) || !validTeamIndex(awayIndex, teams)) {
        std::cout << "Invalid match team index.\n";
        return false;
    }

    Team* home = teams[homeIndex];
    Team* away = teams[awayIndex];
    std::cout << "\nRound " << round << ": " << home->getName()
              << " vs " << away->getName() << "\n";

    bool lateRound = round * 3 > totalRounds * 2 || round > totalRounds - 3;
    Lineup homeLineup(home);
    Lineup awayLineup(away);
    std::string error;

    if (automaticLineups) {
        if (!homeLineup.chooseAutomatically(lateRound, error)) {
            std::cout << error << "\n";
            return false;
        }
        if (!awayLineup.chooseAutomatically(lateRound, error)) {
            std::cout << error << "\n";
            return false;
        }
    } else {
        std::cout << "Choose lineup for home team.\n";
        if (!homeLineup.chooseFromConsole(lateRound)) {
            return false;
        }
        std::cout << "Choose lineup for away team.\n";
        if (!awayLineup.chooseFromConsole(lateRound)) {
            return false;
        }
    }

    if (automaticResult) {
        homeGoals = rand() % 5;
        awayGoals = rand() % 5;
        if (rand() % 10 < 2) {
            homeGoals++;
        }
        if (rand() % 10 < 2) {
            awayGoals++;
        }
        std::cout << "Automatic result: " << home->getName() << " "
                  << homeGoals << ":" << awayGoals << " " << away->getName() << "\n";
    } else {
        do {
            homeGoals = readInt("Home goals: ");
            awayGoals = readInt("Away goals: ");
            if (homeGoals < 0 || awayGoals < 0) {
                std::cout << "Goals cannot be negative.\n";
            }
        } while (homeGoals < 0 || awayGoals < 0);
    }

    scorers.clear();
    int totalGoals = homeGoals + awayGoals;
    for (int i = 0; i < totalGoals; i++) {
        GoalRecord record;
        if (automaticResult) {
            if (i < homeGoals) {
                record.teamIndex = homeIndex;
                record.playerName = homeLineup.pickScorerName();
            } else {
                record.teamIndex = awayIndex;
                record.playerName = awayLineup.pickScorerName();
            }
            scorers.push_back(record);
        } else {
            while (true) {
                std::cout << "Goal " << (i + 1) << " of " << totalGoals << "\n";
                std::string side = readText("Team (home/away): ");
                std::string playerName = safeInputText("Scorer name: ");
                if (side == "home") {
                    if (homeLineup.containsPlayerName(playerName)) {
                        record.teamIndex = homeIndex;
                        record.playerName = playerName;
                        scorers.push_back(record);
                        break;
                    }
                } else if (side == "away") {
                    if (awayLineup.containsPlayerName(playerName)) {
                        record.teamIndex = awayIndex;
                        record.playerName = playerName;
                        scorers.push_back(record);
                        break;
                    }
                }
                std::cout << "Invalid scorer. The scorer must be in the selected lineup.\n";
            }
        }
    }

    homeLineup.markPlayedMatch();
    awayLineup.markPlayedMatch();
    for (int i = 0; i < (int)scorers.size(); i++) {
        int teamIndex = scorers[i].teamIndex;
        int playerIndex = teams[teamIndex]->findPlayerIndexByName(scorers[i].playerName);
        if (playerIndex != -1) {
            teams[teamIndex]->playerAt(playerIndex).addGoal();
        }
    }

    home->updateAfterMatch(homeGoals, awayGoals);
    away->updateAfterMatch(awayGoals, homeGoals);
    played = true;
    std::cout << "Match saved.\n";
    return true;
}

void Match::save(std::ofstream& out) const {
    out << "MATCH" << FILE_SEPARATOR
        << round << FILE_SEPARATOR
        << homeIndex << FILE_SEPARATOR
        << awayIndex << FILE_SEPARATOR
        << boolToFile(played) << FILE_SEPARATOR
        << homeGoals << FILE_SEPARATOR
        << awayGoals << FILE_SEPARATOR
        << scorers.size() << "\n";
    for (int i = 0; i < (int)scorers.size(); i++) {
        out << "SCORER" << FILE_SEPARATOR
            << scorers[i].teamIndex << FILE_SEPARATOR
            << scorers[i].playerName << "\n";
    }
    out << "ENDMATCH\n";
}
