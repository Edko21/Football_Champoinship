#include "ChampionshipCollection.h"
#include "TeamTypes.h"
#include "Utils.h"

#include <cstdlib>
#include <fstream>
#include <iostream>

int ChampionshipCollection::findYear(int year) const {
    for (int i = 0; i < (int)championships.size(); i++) {
        if (championships[i].getYear() == year) {
            return i;
        }
    }
    return -1;
}

int ChampionshipCollection::nextUniqueYear() const {
    int year = currentCalendarYear();
    while (findYear(year) != -1) {
        year++;
    }
    return year;
}

void ChampionshipCollection::addDemoPlayers(Team* team, const std::string& prefix, const int counts[5]) {
    int number = 1;
    Position positions[5] = {Goalkeeper, Defender, Midfielder, Winger, Forward};
    for (int p = 0; p < 5; p++) {
        for (int i = 0; i < counts[p]; i++) {
            std::string name = prefix + "_" + positionToString(positions[p]) + "_" + std::to_string(i + 1);
            double salary = 1200.0 + number * 75.0;
            double transfer = 50000.0 + number * 2500.0;
            Player player(name, number, positions[p], salary, transfer, 0, 0);
            std::string error;
            team->addPlayer(player, error);
            number++;
        }
    }
}

ChampionshipCollection::ChampionshipCollection() {
    activeIndex = -1;
}

bool ChampionshipCollection::hasActive() const {
    return activeIndex >= 0 && activeIndex < (int)championships.size();
}

Championship& ChampionshipCollection::active() {
    return championships[activeIndex];
}

const Championship& ChampionshipCollection::active() const {
    return championships[activeIndex];
}

void ChampionshipCollection::createEmptySeason() {
    int year = nextUniqueYear();
    Championship championship(year);
    championships.push_back(championship);
    activeIndex = (int)championships.size() - 1;
    std::cout << "Created empty season " << year << ".\n";
}

void ChampionshipCollection::createDemoSeason() {
    int year = nextUniqueYear();
    Championship championship(year);
    championships.push_back(championship);
    activeIndex = (int)championships.size() - 1;

    double initialBudget = 3000000.0;
    Team* t1 = new AttackingTeam("Lions", "M. Petrov", "Lion Arena", initialBudget);
    Team* t2 = new DefensiveTeam("Shields", "I. Ivanov", "Shield Park", initialBudget);
    Team* t3 = new BalancedTeam("Eagles", "G. Georgiev", "Eagle Ground", initialBudget);
    Team* t4 = new BalancedTeam("Rovers", "A. Dimitrov", "Rover Stadium", initialBudget);
    Team* t5 = new AttackingTeam("Wolves", "N. Nikolov", "Wolf Den", initialBudget);
    Team* t6 = new DefensiveTeam("Walls", "P. Pavlov", "Wall Ground", initialBudget);
    Team* t7 = new BalancedTeam("Falcons", "S. Stoyanov", "Falcon Field", initialBudget);
    Team* t8 = new BalancedTeam("Mariners", "V. Vasilev", "Harbor Stadium", initialBudget);

    int attacking[5] = {2, 3, 3, 4, 8};
    int defensive[5] = {2, 6, 6, 3, 3};
    int balancedA[5] = {2, 5, 5, 4, 4};
    int balancedB[5] = {2, 4, 5, 5, 4};
    addDemoPlayers(t1, "Lions", attacking);
    addDemoPlayers(t2, "Shields", defensive);
    addDemoPlayers(t3, "Eagles", balancedA);
    addDemoPlayers(t4, "Rovers", balancedB);
    addDemoPlayers(t5, "Wolves", attacking);
    addDemoPlayers(t6, "Walls", defensive);
    addDemoPlayers(t7, "Falcons", balancedA);
    addDemoPlayers(t8, "Mariners", balancedB);

    std::string error;
    active().addTeam(t1, error);
    active().addTeam(t2, error);
    active().addTeam(t3, error);
    active().addTeam(t4, error);
    active().addTeam(t5, error);
    active().addTeam(t6, error);
    active().addTeam(t7, error);
    active().addTeam(t8, error);

    if (active().generateSchedule(error)) {
        std::cout << "Created demo season " << year
                  << " with 8 valid teams and a generated schedule.\n";
    } else {
        std::cout << "Demo season created, but schedule generation failed: " << error << "\n";
    }
}

void ChampionshipCollection::listSeasons() const {
    if ((int)championships.size() == 0) {
        std::cout << "No seasons.\n";
        return;
    }
    for (int i = 0; i < (int)championships.size(); i++) {
        std::cout << (i == activeIndex ? "* " : "  ");
        std::cout << championships[i].getYear()
                  << (championships[i].isFinished() ? " finished" : " in progress")
                  << ", teams: " << championships[i].getTeamCount() << "\n";
        if (championships[i].isFinished()) {
            championships[i].printAwards();
        }
    }
}

void ChampionshipCollection::switchSeason() {
    if ((int)championships.size() == 0) {
        std::cout << "No seasons.\n";
        return;
    }
    int year = readInt("Season year: ");
    int index = findYear(year);
    if (index == -1) {
        std::cout << "Season not found.\n";
        return;
    }
    activeIndex = index;
    std::cout << "Active season is now " << year << ".\n";
}

bool ChampionshipCollection::saveToFile(const std::string& filename, std::string& error) const {
    std::ofstream out(filename.c_str());
    if (!out) {
        error = "Cannot open file for writing.";
        return false;
    }
    out << "FOOTBALL_CHAMPIONSHIPS" << FILE_SEPARATOR << "1" << FILE_SEPARATOR
        << championships.size() << FILE_SEPARATOR << activeIndex << "\n";
    for (int i = 0; i < (int)championships.size(); i++) {
        championships[i].save(out);
    }
    error = "";
    return true;
}

bool ChampionshipCollection::loadFromFile(const std::string& filename, std::string& error) {
    std::ifstream in(filename.c_str());
    if (!in) {
        error = "Cannot open file for reading.";
        return false;
    }

    std::string line;
    if (!getline(in, line)) {
        error = "Empty file.";
        return false;
    }
    std::vector<std::string> header = split(line, FILE_SEPARATOR);
    if ((int)header.size() < 4 || header[0] != "FOOTBALL_CHAMPIONSHIPS") {
        error = "Invalid file header.";
        return false;
    }

    std::vector<Championship> loaded;
    int loadedActiveIndex = atoi(header[3].c_str());

    while (getline(in, line)) {
        if (line == "") {
            continue;
        }
        std::vector<std::string> parts = split(line, FILE_SEPARATOR);
        if ((int)parts.size() == 0) {
            continue;
        }
        if (parts[0] != "CHAMPIONSHIP") {
            error = "Expected CHAMPIONSHIP block.";
            return false;
        }
        if ((int)parts.size() < 7) {
            error = "Invalid CHAMPIONSHIP line.";
            return false;
        }

        Championship championship(atoi(parts[1].c_str()));
        championship.setLoadedState(atoi(parts[2].c_str()), fileToBool(parts[3]), fileToBool(parts[4]));

        while (getline(in, line)) {
            if (line == "ENDCHAMPIONSHIP") {
                break;
            }
            std::vector<std::string> blockParts = split(line, FILE_SEPARATOR);
            if ((int)blockParts.size() == 0 || line == "") {
                continue;
            }

            if (blockParts[0] == "TEAM") {
                if ((int)blockParts.size() < 13) {
                    error = "Invalid TEAM line.";
                    return false;
                }
                Team* team = createTeamByType(blockParts[1], blockParts[2], blockParts[3],
                                              blockParts[4], atof(blockParts[5].c_str()));
                if (team == 0) {
                    error = "Unknown team type in file.";
                    return false;
                }
                team->setStats(atoi(blockParts[6].c_str()), atoi(blockParts[7].c_str()),
                               atoi(blockParts[8].c_str()), atoi(blockParts[9].c_str()),
                               atoi(blockParts[10].c_str()), atoi(blockParts[11].c_str()));

                while (getline(in, line)) {
                    if (line == "ENDTEAM") {
                        break;
                    }
                    std::vector<std::string> playerParts = split(line, FILE_SEPARATOR);
                    if ((int)playerParts.size() < 8 || playerParts[0] != "PLAYER") {
                        delete team;
                        error = "Invalid PLAYER line.";
                        return false;
                    }
                    Player player(playerParts[1], atoi(playerParts[2].c_str()),
                                  stringToPosition(playerParts[3]),
                                  atof(playerParts[4].c_str()),
                                  atof(playerParts[5].c_str()),
                                  atoi(playerParts[6].c_str()),
                                  atoi(playerParts[7].c_str()));
                    team->addPlayerUnchecked(player);
                }
                championship.addLoadedTeam(team);
            } else if (blockParts[0] == "MATCHES") {
                while (getline(in, line)) {
                    if (line == "ENDMATCHES") {
                        break;
                    }
                    std::vector<std::string> matchParts = split(line, FILE_SEPARATOR);
                    if ((int)matchParts.size() < 8 || matchParts[0] != "MATCH") {
                        error = "Invalid MATCH line.";
                        return false;
                    }
                    Match match;
                    match.setLoadedState(atoi(matchParts[1].c_str()), atoi(matchParts[2].c_str()),
                                         atoi(matchParts[3].c_str()), fileToBool(matchParts[4]),
                                         atoi(matchParts[5].c_str()), atoi(matchParts[6].c_str()));
                    while (getline(in, line)) {
                        if (line == "ENDMATCH") {
                            break;
                        }
                        std::vector<std::string> scorerParts = split(line, FILE_SEPARATOR);
                        if ((int)scorerParts.size() < 3 || scorerParts[0] != "SCORER") {
                            error = "Invalid SCORER line.";
                            return false;
                        }
                        GoalRecord record;
                        record.teamIndex = atoi(scorerParts[1].c_str());
                        record.playerName = scorerParts[2];
                        match.addLoadedScorer(record);
                    }
                    championship.addLoadedMatch(match);
                }
            } else {
                error = "Unknown block in championship file.";
                return false;
            }
        }
        loaded.push_back(championship);
    }

    championships = loaded;
    if (loadedActiveIndex >= 0 && loadedActiveIndex < (int)championships.size()) {
        activeIndex = loadedActiveIndex;
    } else if ((int)championships.size() > 0) {
        activeIndex = 0;
    } else {
        activeIndex = -1;
    }
    error = "";
    return true;
}
