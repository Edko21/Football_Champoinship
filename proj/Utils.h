#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

void clearInputLine();
int readInt(const std::string& prompt);
double readDouble(const std::string& prompt);
std::string readText(const std::string& prompt);
bool askYesNo(const std::string& prompt);
bool containsFileSeparator(const std::string& text);
std::string safeInputText(const std::string& prompt);
void printSeparator();
void printPadded(const std::string& text, int width);
std::vector<std::string> split(const std::string& line, char delimiter);
std::string boolToFile(bool value);
bool fileToBool(const std::string& value);
int currentCalendarYear();

#endif
