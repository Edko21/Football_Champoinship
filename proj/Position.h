#ifndef POSITION_H
#define POSITION_H

#include <string>

enum Position {
    Goalkeeper = 0,
    Defender = 1,
    Midfielder = 2,
    Winger = 3,
    Forward = 4,
    UnknownPosition = 5
};

std::string positionToString(Position position);
Position stringToPosition(const std::string& text);

#endif
