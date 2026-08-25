#include "Lineup.h"
#include "Constants.h"
#include "Utils.h"

#include <cstdlib>
#include <iostream>

bool Lineup::containsIndex(int index) const {
    for (int i = 0; i < (int)selectedIndices.size(); i++) {
        if (selectedIndices[i] == index) {
            return true;
        }
    }
    return false;
}

int Lineup::countPosition(Position position) const {
    int count = 0;
    for (int i = 0; i < (int)selectedIndices.size(); i++) {
        if (team->playerAt(selectedIndices[i]).getPosition() == position) {
            count++;
        }
    }
    return count;
}

int Lineup::findCandidate(Position position, bool preferLowMatches) const {
    int best = -1;
    for (int i = 0; i < team->playerCount(); i++) {
        const Player& player = team->playerAt(i);
        if (player.getPosition() != position || containsIndex(i)) {
            continue;
        }
        if (best == -1) {
            best = i;
        } else if (preferLowMatches &&
                   player.getPlayedMatches() < team->playerAt(best).getPlayedMatches()) {
            best = i;
        }
    }
    return best;
}

int Lineup::findAnyCandidate(bool preferLowMatches) const {
    int best = -1;
    for (int i = 0; i < team->playerCount(); i++) {
        const Player& player = team->playerAt(i);
        if (containsIndex(i) || player.getPosition() == Goalkeeper) {
            continue;
        }
        if (best == -1) {
            best = i;
        } else if (preferLowMatches &&
                   player.getPlayedMatches() < team->playerAt(best).getPlayedMatches()) {
            best = i;
        }
    }
    return best;
}

Lineup::Lineup(Team* lineupTeam) {
    team = lineupTeam;
}

bool Lineup::addPlayerByName(const std::string& playerName, std::string& error) {
    int index = team->findPlayerIndexByName(playerName);
    if (index == -1) {
        error = "Player not found in this team.";
        return false;
    }
    if (containsIndex(index)) {
        error = "This player is already selected.";
        return false;
    }
    selectedIndices.push_back(index);
    error = "";
    return true;
}

bool Lineup::isValid(std::string& error) const {
    if ((int)selectedIndices.size() != LINEUP_PLAYERS) {
        error = "The lineup must contain exactly 11 players.";
        return false;
    }
    if (countPosition(Goalkeeper) != 1) {
        error = "The lineup must contain exactly 1 goalkeeper.";
        return false;
    }
    if (countPosition(Defender) < 2) {
        error = "The lineup must contain at least 2 defenders.";
        return false;
    }
    if (countPosition(Midfielder) < 2) {
        error = "The lineup must contain at least 2 midfielders.";
        return false;
    }
    if (countPosition(Winger) < 2) {
        error = "The lineup must contain at least 2 wingers.";
        return false;
    }
    if (countPosition(Forward) < 2) {
        error = "The lineup must contain at least 2 forwards.";
        return false;
    }
    error = "";
    return true;
}

bool Lineup::chooseAutomatically(bool lateRound, std::string& error) {
    selectedIndices.clear();
    Position requiredPositions[5] = {Goalkeeper, Defender, Midfielder, Winger, Forward};
    int requiredCounts[5] = {1, 2, 2, 2, 2};

    for (int p = 0; p < 5; p++) {
        for (int c = 0; c < requiredCounts[p]; c++) {
            int candidate = findCandidate(requiredPositions[p], lateRound);
            if (candidate == -1) {
                error = "Cannot build an automatic lineup for " + team->getName() + ".";
                return false;
            }
            selectedIndices.push_back(candidate);
        }
    }

    while ((int)selectedIndices.size() < LINEUP_PLAYERS) {
        int candidate = findAnyCandidate(lateRound);
        if (candidate == -1) {
            error = "Not enough field players for " + team->getName() + ".";
            return false;
        }
        selectedIndices.push_back(candidate);
    }

    return isValid(error);
}

bool Lineup::chooseFromConsole(bool lateRound) {
    selectedIndices.clear();
    std::cout << "Roster of " << team->getName() << ":\n";
    team->printShortRoster();

    if (lateRound) {
        std::cout << "Late rounds rule: players with fewer than 3 matches should be preferred when possible:\n";
        for (int i = 0; i < team->playerCount(); i++) {
            const Player& player = team->playerAt(i);
            if (player.getPlayedMatches() < 3) {
                std::cout << "  " << player.getName() << " (" << positionToString(player.getPosition())
                          << ", matches " << player.getPlayedMatches() << ")\n";
            }
        }
    }

    if (askYesNo("Use automatic valid lineup for this team")) {
        std::string error;
        if (!chooseAutomatically(lateRound, error)) {
            std::cout << "Automatic lineup failed: " << error << "\n";
            return false;
        }
        print();
        return true;
    }

    std::cout << "Enter exactly 11 player names.\n";
    while ((int)selectedIndices.size() < LINEUP_PLAYERS) {
        std::string name = safeInputText("Player name: ");
        std::string error;
        if (!addPlayerByName(name, error)) {
            std::cout << error << "\n";
        } else {
            std::cout << "Selected " << selectedIndices.size() << "/" << LINEUP_PLAYERS << "\n";
        }
    }

    std::string error;
    if (!isValid(error)) {
        std::cout << "Invalid lineup: " << error << "\n";
        return false;
    }
    return true;
}

bool Lineup::containsPlayerName(const std::string& playerName) const {
    for (int i = 0; i < (int)selectedIndices.size(); i++) {
        if (team->playerAt(selectedIndices[i]).getName() == playerName) {
            return true;
        }
    }
    return false;
}

std::string Lineup::pickScorerName() const {
    std::vector<int> candidates;
    for (int i = 0; i < (int)selectedIndices.size(); i++) {
        Position position = team->playerAt(selectedIndices[i]).getPosition();
        if (position == Forward || position == Winger || position == Midfielder) {
            candidates.push_back(selectedIndices[i]);
        }
    }
    if ((int)candidates.size() == 0) {
        return team->playerAt(selectedIndices[0]).getName();
    }
    int index = rand() % candidates.size();
    return team->playerAt(candidates[index]).getName();
}

void Lineup::markPlayedMatch() {
    for (int i = 0; i < (int)selectedIndices.size(); i++) {
        team->playerAt(selectedIndices[i]).addPlayedMatch();
    }
}

void Lineup::print() const {
    std::cout << "Lineup for " << team->getName() << ":\n";
    for (int i = 0; i < (int)selectedIndices.size(); i++) {
        std::cout << "  ";
        team->playerAt(selectedIndices[i]).print();
    }
}
