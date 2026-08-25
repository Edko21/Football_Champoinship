#ifndef MENU_H
#define MENU_H

#include "ChampionshipCollection.h"
#include "Player.h"

Position readPosition();
Player readPlayerFromConsole();
void addTeamMenu(ChampionshipCollection& collection);
void addOrBuyPlayerMenu(ChampionshipCollection& collection);
void removePlayerMenu(ChampionshipCollection& collection);
void swapPlayersMenu(ChampionshipCollection& collection);
void teamInfoMenu(ChampionshipCollection& collection);
void printMainMenu();

#endif
