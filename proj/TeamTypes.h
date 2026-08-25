#ifndef TEAM_TYPES_H
#define TEAM_TYPES_H

#include "Team.h"

class AttackingTeam : public Team {
public:
    AttackingTeam();
    AttackingTeam(const std::string& teamName, const std::string& coach,
                  const std::string& stadium, double initialBudget);
    Team* clone() const;
    std::string typeName() const;
    int minFor(Position position) const;
    bool styleAllowsPurchase(const Player& player, std::string& error) const;
};

class DefensiveTeam : public Team {
public:
    DefensiveTeam();
    DefensiveTeam(const std::string& teamName, const std::string& coach,
                  const std::string& stadium, double initialBudget);
    Team* clone() const;
    std::string typeName() const;
    int minFor(Position position) const;
};

class BalancedTeam : public Team {
public:
    BalancedTeam();
    BalancedTeam(const std::string& teamName, const std::string& coach,
                 const std::string& stadium, double initialBudget);
    Team* clone() const;
    std::string typeName() const;
    int minFor(Position position) const;
};

Team* createTeamByType(const std::string& type, const std::string& name,
                       const std::string& coach, const std::string& stadium, double budget);

#endif
