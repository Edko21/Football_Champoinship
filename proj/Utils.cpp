#include "Utils.h"
#include "Constants.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

void clearInputLine() {
    char ch;
    while (std::cin.get(ch)) {
        if (ch == '\n') {
            break;
        }
    }
}

int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInputLine();
            return value;
        }
        std::cout << "Invalid integer. Try again.\n";
        std::cin.clear();
        clearInputLine();
    }
}

double readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInputLine();
            return value;
        }
        std::cout << "Invalid number. Try again.\n";
        std::cin.clear();
        clearInputLine();
    }
}

std::string readText(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    getline(std::cin, value);
    return value;
}

bool askYesNo(const std::string& prompt) {
    while (true) {
        std::string value = readText(prompt + " (y/n): ");
        if (value == "y" || value == "Y") {
            return true;
        }
        if (value == "n" || value == "N") {
            return false;
        }
        std::cout << "Please answer with y or n.\n";
    }
}

bool containsFileSeparator(const std::string& text) {
    for (int i = 0; i < (int)text.size(); i++) {
        if (text[i] == FILE_SEPARATOR) {
            return true;
        }
    }
    return false;
}

std::string safeInputText(const std::string& prompt) {
    while (true) {
        std::string value = readText(prompt);
        if (!containsFileSeparator(value)) {
            return value;
        }
        std::cout << "The character '" << FILE_SEPARATOR
                  << "' is reserved for files. Try again.\n";
    }
}

void printSeparator() {
    std::cout << "------------------------------------------------------------\n";
}

void printPadded(const std::string& text, int width) {
    std::cout << text;
    int remaining = width - (int)text.size();
    for (int i = 0; i < remaining; i++) {
        std::cout << ' ';
    }
}

std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> parts;
    std::string current;
    for (int i = 0; i < (int)line.size(); i++) {
        if (line[i] == delimiter) {
            parts.push_back(current);
            current = "";
        } else {
            current += line[i];
        }
    }
    parts.push_back(current);
    return parts;
}

std::string boolToFile(bool value) {
    return value ? "1" : "0";
}

bool fileToBool(const std::string& value) {
    return value == "1";
}

int currentCalendarYear() {
    time_t rawTime = time(0);
    tm* local = localtime(&rawTime);
    if (local == 0) {
        return 2026;
    }
    return local->tm_year + 1900;
}
