#include "TeamTypes.h"

AttackingTeam::AttackingTeam() : Team() {
}

AttackingTeam::AttackingTeam(const std::string& teamName, const std::string& coach,
                             const std::string& stadium, double initialBudget)
    : Team(teamName, coach, stadium, initialBudget) {
}

Team* AttackingTeam::clone() const {
    return new AttackingTeam(*this);
}

std::string AttackingTeam::typeName() const {
    return "Attacking";
}

int AttackingTeam::minFor(Position position) const {
    if (position == Goalkeeper) return 2;
    if (position == Forward) return 8;
    if (position == Defender || position == Midfielder || position == Winger) return 2;
    return 0;
}

bool AttackingTeam::styleAllowsPurchase(const Player& player, std::string& error) const {
    if (player.getPosition() == Forward && player.getGoals() < 6) {
        error = "Attacking teams can buy a forward only if he has at least 6 goals.";
        return false;
    }
    error = "";
    return true;
}

DefensiveTeam::DefensiveTeam() : Team() {
}

DefensiveTeam::DefensiveTeam(const std::string& teamName, const std::string& coach,
                             const std::string& stadium, double initialBudget)
    : Team(teamName, coach, stadium, initialBudget) {
}

Team* DefensiveTeam::clone() const {
    return new DefensiveTeam(*this);
}

std::string DefensiveTeam::typeName() const {
    return "Defensive";
}

int DefensiveTeam::minFor(Position position) const {
    if (position == Goalkeeper) return 2;
    if (position == Defender) return 6;
    if (position == Midfielder) return 6;
    if (position == Winger || position == Forward) return 2;
    return 0;
}

BalancedTeam::BalancedTeam() : Team() {
}

BalancedTeam::BalancedTeam(const std::string& teamName, const std::string& coach,
                           const std::string& stadium, double initialBudget)
    : Team(teamName, coach, stadium, initialBudget) {
}

Team* BalancedTeam::clone() const {
    return new BalancedTeam(*this);
}

std::string BalancedTeam::typeName() const {
    return "Balanced";
}

int BalancedTeam::minFor(Position position) const {
    if (position == Goalkeeper) return 2;
    if (position == Defender || position == Midfielder ||
        position == Winger || position == Forward) return 4;
    return 0;
}

Team* createTeamByType(const std::string& type, const std::string& name,
                       const std::string& coach, const std::string& stadium, double budget) {
    if (type == "Attacking") {
        return new AttackingTeam(name, coach, stadium, budget);
    }
    if (type == "Defensive") {
        return new DefensiveTeam(name, coach, stadium, budget);
    }
    if (type == "Balanced") {
        return new BalancedTeam(name, coach, stadium, budget);
    }
    return 0;
}
