#ifndef CHAMPIONSHIP_COLLECTION_H
#define CHAMPIONSHIP_COLLECTION_H

#include "Championship.h"

#include <string>
#include <vector>

class ChampionshipCollection {
public:
    ChampionshipCollection();

    bool hasActive() const;
    Championship& active();
    const Championship& active() const;

    void createEmptySeason();
    void createDemoSeason();
    void listSeasons() const;
    void switchSeason();
    bool saveToFile(const std::string& filename, std::string& error) const;
    bool loadFromFile(const std::string& filename, std::string& error);
private:
    std::vector<Championship> championships;
    int activeIndex;

    int findYear(int year) const;
    int nextUniqueYear() const;
    void addDemoPlayers(Team* team, const std::string& prefix, const int counts[5]);


};

#endif
