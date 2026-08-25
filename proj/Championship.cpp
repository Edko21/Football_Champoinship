#include "Championship.h"
#include "Utils.h"

#include <iostream>

void Championship::clearTeams() {
    for (int i = 0; i < (int)teams.size(); i++) {
        delete teams[i];
    }
    teams.clear();
}

void Championship::copyFrom(const Championship& other) {
    year = other.year;
    currentRound = other.currentRound;
    finished = other.finished;
    scheduleGenerated = other.scheduleGenerated;
    matches = other.matches;
    teams.clear();
    for (int i = 0; i < (int)other.teams.size(); i++) {
        teams.push_back(other.teams[i]->clone());
    }
}

bool Championship::teamComesBefore(int firstIndex, int secondIndex) const {
    const Team* first = teams[firstIndex];
    const Team* second = teams[secondIndex];
    if (first->getPoints() != second->getPoints()) {
        return first->getPoints() > second->getPoints();
    }
    if (first->goalDifference() != second->goalDifference()) {
        return first->goalDifference() > second->goalDifference();
    }
    if (first->getGoalsFor() != second->getGoalsFor()) {
        return first->getGoalsFor() > second->getGoalsFor();
    }
    if (first->getGoalsAgainst() != second->getGoalsAgainst()) {
        return first->getGoalsAgainst() < second->getGoalsAgainst();
    }
    return first->getName() < second->getName();
}

Championship::Championship() {
    year = currentCalendarYear();
    currentRound = 1;
    finished = false;
    scheduleGenerated = false;
}

Championship::Championship(int championshipYear) {
    year = championshipYear;
    currentRound = 1;
    finished = false;
    scheduleGenerated = false;
}

Championship::Championship(const Championship& other) {
    copyFrom(other);
}

Championship& Championship::operator=(const Championship& other) {
    if (this != &other) {
        clearTeams();
        copyFrom(other);
    }
    return *this;
}

Championship::~Championship() {
    clearTeams();
}

int Championship::getYear() const {
    return year;
}

bool Championship::isFinished() const {
    return finished;
}

int Championship::getTeamCount() const {
    return (int)teams.size();
}

Team* Championship::getTeam(int index) {
    return teams[index];
}

const Team* Championship::getTeam(int index) const {
    return teams[index];
}

int Championship::totalRounds() const {
    if ((int)teams.size() < 2) {
        return 0;
    }
    return 2 * ((int)teams.size() - 1);
}

int Championship::findTeamIndexByName(const std::string& teamName) const {
    for (int i = 0; i < (int)teams.size(); i++) {
        if (teams[i]->getName() == teamName) {
            return i;
        }
    }
    return -1;
}

bool Championship::addTeam(Team* team, std::string& error) {
    if (team == 0) {
        error = "Invalid team type.";
        return false;
    }
    if (containsFileSeparator(team->getName())) {
        error = "Team name cannot contain the file separator character.";
        delete team;
        return false;
    }
    if (findTeamIndexByName(team->getName()) != -1) {
        error = "A team with this name already exists in the championship.";
        delete team;
        return false;
    }
    if (scheduleGenerated) {
        error = "Cannot add teams after the schedule is generated.";
        delete team;
        return false;
    }
    teams.push_back(team);
    error = "";
    return true;
}

void Championship::addLoadedTeam(Team* team) {
    teams.push_back(team);
}

void Championship::addLoadedMatch(const Match& match) {
    matches.push_back(match);
}

void Championship::setLoadedState(int loadedCurrentRound, bool loadedFinished,
                                  bool loadedScheduleGenerated) {
    currentRound = loadedCurrentRound;
    finished = loadedFinished;
    scheduleGenerated = loadedScheduleGenerated;
}

bool Championship::generateSchedule(std::string& error) {
    if (scheduleGenerated && (int)matches.size() > 0) {
        error = "Schedule is already generated.";
        return false;
    }
    if ((int)teams.size() < 4 || ((int)teams.size()) % 2 != 0) {
        error = "The championship must have at least 4 teams and an even team count.";
        return false;
    }
    for (int i = 0; i < (int)teams.size(); i++) {
        std::string teamError;
        if (!teams[i]->rosterIsValid(teamError)) {
            error = "Invalid roster for " + teams[i]->getName() + ": " + teamError;
            return false;
        }
    }

    for (int i = 0; i < (int)teams.size(); i++) {
        teams[i]->startNewSeason();
    }

    matches.clear();
    int n = (int)teams.size();
    std::vector<int> order;
    for (int i = 0; i < n; i++) {
        order.push_back(i);
    }

    for (int roundNumber = 1; roundNumber <= n - 1; roundNumber++) {
        for (int i = 0; i < n / 2; i++) {
            int first = order[i];
            int second = order[n - 1 - i];
            if (roundNumber % 2 == 1) {
                matches.push_back(Match(roundNumber, first, second));
            } else {
                matches.push_back(Match(roundNumber, second, first));
            }
        }

        int last = order[n - 1];
        for (int i = n - 1; i > 1; i--) {
            order[i] = order[i - 1];
        }
        order[1] = last;
    }

    int firstLegMatches = (int)matches.size();
    for (int i = 0; i < firstLegMatches; i++) {
        int newRound = matches[i].getRound() + n - 1;
        matches.push_back(Match(newRound, matches[i].getAwayIndex(), matches[i].getHomeIndex()));
    }

    currentRound = 1;
    finished = false;
    scheduleGenerated = true;
    error = "";
    return true;
}

bool Championship::playOneRound(bool automaticLineups, bool automaticResult) {
    if (!scheduleGenerated) {
        std::cout << "Generate the schedule first.\n";
        return false;
    }
    if (finished) {
        std::cout << "The championship is already finished.\n";
        return true;
    }

    std::cout << "\nPlaying round " << currentRound << " of " << totalRounds() << "\n";
    for (int i = 0; i < (int)matches.size(); i++) {
        if (matches[i].getRound() == currentRound && !matches[i].isPlayed()) {
            if (!matches[i].play(teams, totalRounds(), automaticLineups, automaticResult)) {
                std::cout << "Round stopped before all matches were played.\n";
                return false;
            }
        }
    }

    currentRound++;
    if (currentRound > totalRounds()) {
        finished = true;
        for (int i = 0; i < (int)teams.size(); i++) {
            teams[i]->regulatePlayerSalaries();
        }
        std::cout << "Season finished. Salaries were regulated.\n";
        printAwards();
    }
    return true;
}

void Championship::playAllRounds(bool automaticLineups, bool automaticResult) {
    while (!finished) {
        if (!playOneRound(automaticLineups, automaticResult)) {
            break;
        }
    }
}

std::vector<int> Championship::standingsOrder() const {
    std::vector<int> order;
    for (int i = 0; i < (int)teams.size(); i++) {
        order.push_back(i);
    }
    for (int i = 0; i < (int)order.size(); i++) {
        for (int j = 0; j + 1 < (int)order.size(); j++) {
            if (teamComesBefore(order[j + 1], order[j])) {
                int temp = order[j];
                order[j] = order[j + 1];
                order[j + 1] = temp;
            }
        }
    }
    return order;
}

void Championship::printStandings() const {
    if ((int)teams.size() == 0) {
        std::cout << "No teams.\n";
        return;
    }
    std::vector<int> order = standingsOrder();
    printSeparator();
    std::cout << "Standings for season " << year << "\n";
    printPadded("#", 4);
    printPadded("Team", 18);
    printPadded("Pts", 6);
    printPadded("GF", 5);
    printPadded("GA", 5);
    printPadded("GD", 5);
    std::cout << "\n";
    for (int i = 0; i < (int)order.size(); i++) {
        const Team* team = teams[order[i]];
        printPadded(std::to_string(i + 1), 4);
        printPadded(team->getName(), 18);
        printPadded(std::to_string(team->getPoints()), 6);
        printPadded(std::to_string(team->getGoalsFor()), 5);
        printPadded(std::to_string(team->getGoalsAgainst()), 5);
        printPadded(std::to_string(team->goalDifference()), 5);
        std::cout << "\n";
    }
}

void Championship::printAwards() const {
    if ((int)teams.size() < 3) {
        std::cout << "Not enough teams for medals.\n";
        return;
    }
    std::vector<int> order = standingsOrder();
    printSeparator();
    std::cout << "Awards for season " << year << "\n";
    std::cout << "Champion: " << teams[order[0]]->getName() << "\n";
    std::cout << "Runner-up: " << teams[order[1]]->getName() << "\n";
    std::cout << "Bronze: " << teams[order[2]]->getName() << "\n";

    std::vector<ScorerCandidate> winners;
    int bestGoals = -1;
    int bestMatches = 0;
    for (int t = 0; t < (int)teams.size(); t++) {
        for (int p = 0; p < teams[t]->playerCount(); p++) {
            const Player& player = teams[t]->playerAt(p);
            if (player.getPlayedMatches() < 7) {
                continue;
            }
            bool better = false;
            if (bestGoals == -1 || player.getGoals() > bestGoals) {
                better = true;
            } else if (player.getGoals() == bestGoals && player.getPlayedMatches() < bestMatches) {
                better = true;
            }
            if (better) {
                winners.clear();
                ScorerCandidate candidate;
                candidate.teamIndex = t;
                candidate.playerIndex = p;
                winners.push_back(candidate);
                bestGoals = player.getGoals();
                bestMatches = player.getPlayedMatches();
            } else if (player.getGoals() == bestGoals &&
                       player.getPlayedMatches() == bestMatches) {
                ScorerCandidate candidate;
                candidate.teamIndex = t;
                candidate.playerIndex = p;
                winners.push_back(candidate);
            }
        }
    }

    if ((int)winners.size() == 0) {
        std::cout << "Top scorer: no eligible player with at least 7 matches.\n";
    } else {
        std::cout << "Top scorer";
        if ((int)winners.size() > 1) {
            std::cout << "s";
        }
        std::cout << ": ";
        for (int i = 0; i < (int)winners.size(); i++) {
            const Player& player = teams[winners[i].teamIndex]->playerAt(winners[i].playerIndex);
            if (i > 0) {
                std::cout << ", ";
            }
            std::cout << player.getName() << " (" << teams[winners[i].teamIndex]->getName()
                      << ", goals " << player.getGoals()
                      << ", matches " << player.getPlayedMatches() << ")";
        }
        std::cout << "\n";
    }
}

void Championship::printSchedule() const {
    if (!scheduleGenerated) {
        std::cout << "No schedule.\n";
        return;
    }
    for (int i = 0; i < (int)matches.size(); i++) {
        matches[i].printShort(teams);
    }
}

void Championship::save(std::ofstream& out) const {
    out << "CHAMPIONSHIP" << FILE_SEPARATOR
        << year << FILE_SEPARATOR
        << currentRound << FILE_SEPARATOR
        << boolToFile(finished) << FILE_SEPARATOR
        << boolToFile(scheduleGenerated) << FILE_SEPARATOR
        << teams.size() << FILE_SEPARATOR
        << matches.size() << "\n";

    for (int i = 0; i < (int)teams.size(); i++) {
        teams[i]->save(out);
    }

    out << "MATCHES" << FILE_SEPARATOR << matches.size() << "\n";
    for (int i = 0; i < (int)matches.size(); i++) {
        matches[i].save(out);
    }
    out << "ENDMATCHES\n";
    out << "ENDCHAMPIONSHIP\n";
}
