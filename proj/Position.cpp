#include "Position.h"

std::string positionToString(Position position) {
    if (position == Goalkeeper) {
        return "Goalkeeper";
    }
    if (position == Defender) {
        return "Defender";
    }
    if (position == Midfielder) {
        return "Midfielder";
    }
    if (position == Winger) {
        return "Winger";
    }
    if (position == Forward) {
        return "Forward";
    }
    return "Unknown";
}

Position stringToPosition(const std::string& text) {
    if (text == "Goalkeeper" || text == "GK" || text == "goalkeeper") {
        return Goalkeeper;
    }
    if (text == "Defender" || text == "DEF" || text == "defender") {
        return Defender;
    }
    if (text == "Midfielder" || text == "MID" || text == "midfielder") {
        return Midfielder;
    }
    if (text == "Winger" || text == "WING" || text == "winger") {
        return Winger;
    }
    if (text == "Forward" || text == "FWD" || text == "forward") {
        return Forward;
    }
    return UnknownPosition;
}
