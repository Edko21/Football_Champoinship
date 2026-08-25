Football Championship Simulator

A console-based C++ application that simulates and manages football championship seasons. The project was developed as an Object-Oriented Programming course project and models players, teams, line-ups, matches, standings, awards, and multiple seasons.

The application supports both manual interaction and automatic simulation, as well as saving and restoring the complete championship history from a text file.

Features

Create an empty season with an automatically assigned unique year.

Create a ready-to-use demo season with 8 valid teams and 20 players per team.

Maintain multiple seasons and switch between them.

Add attacking, defensive, and balanced teams.

Add, remove, buy, and exchange players between teams.

Validate team rosters according to team type and player positions.

Generate a double round-robin schedule in which every team plays every other team twice.

Select starting line-ups manually or automatically.

Enter match results and scorers manually or generate them automatically.

Update team and player statistics after every match.

Display the schedule, team information, standings, medalists, and top scorer.

Regulate player salaries according to appearances, goals, and position.

Save all seasons to a text file and load them again later.

Validate user input and loaded file structure.

Object-Oriented Design

The project separates the football domain into classes with clearly defined responsibilities.

Encapsulation: each class owns and manages its data and behaviour.

Inheritance: AttackingTeam, DefensiveTeam, and BalancedTeam derive from the abstract Team class.

Polymorphism: virtual functions define team-specific roster and transfer rules.

Polymorphic copying: clone() preserves the concrete team type when a championship is copied.

Correct object lifecycle: Championship implements a copy constructor, assignment operator, and destructor for its dynamically allocated teams.

Composition: championships contain teams and matches; teams contain players; matches use line-ups.

Separation of concerns: domain logic, menu interaction, validation, and file persistence are kept in separate files.

Core Classes

Class

Responsibility

Player

Stores a player's name, number, position, salary, transfer fee, appearances, and goals.

Team

Abstract base class that manages team information, budget, roster operations, statistics, and validation.

AttackingTeam

Requires an attack-focused roster and applies an additional rule when purchasing forwards.

DefensiveTeam

Requires a strong defensive and midfield roster.

BalancedTeam

Requires an evenly distributed roster across all outfield positions.

Lineup

Stores and validates the 11 selected players for a match.

Match

Stores the home and away teams, round, result, and scorers, and updates statistics after being played.

Championship

Represents one season, generates its schedule, plays rounds, calculates standings, and determines awards.

ChampionshipCollection

Stores all seasons and manages creation, selection, saving, and loading.

Team and Line-up Rules

General team rules

A team can contain no more than 20 players.

Player names and shirt numbers must be unique within a team.

A valid team must contain exactly 2 goalkeepers.

The | character cannot be used in team or player names because it is reserved for the save-file format.

Team types

Attacking team: at least 8 forwards and at least 2 players in every other outfield position. A forward can be purchased only if the player has scored at least 6 goals.

Defensive team: at least 6 defenders, 6 midfielders, 2 wingers, and 2 forwards.

Balanced team: at least 4 players in each outfield position.

Each team type also requires exactly 2 goalkeepers.

Starting line-up

A valid starting line-up contains exactly 11 players:

1 goalkeeper;

at least 2 defenders;

at least 2 midfielders;

at least 2 wingers;

at least 2 forwards.

The remaining two places may be filled by any available outfield players.

Championship Rules

Schedule generation

The championship requires at least 4 teams and an even number of teams. A round-robin algorithm generates the first half of the season, after which the second half is created with the home and away teams reversed. As a result, every pair of teams plays twice.

Standings

Teams are ordered using the following criteria:

Points;

Goal difference;

Goals scored;

Fewer goals conceded;

Team name in alphabetical order.

The final awards include the champion, runner-up, bronze team, and top scorer.

Top scorer

Only players with at least 7 appearances are eligible. The player with the most goals wins. If multiple players have the same number of goals, the player with fewer appearances is preferred. If they are still tied, all matching players are displayed as top scorers.

Salary regulation

At the end of a season, player salaries are adjusted according to performance:

More than 50% participation in the team's matches: +20%.

Fewer than 7 appearances: -10%.

More than 5 goals: an additional +10%.

More than 10 goals: an additional +20%.

A forward with fewer than 5 goals: -15%.

Project Structure

File

Purpose

main.cpp

Starts the application and runs the main menu loop.

Menu.h / Menu.cpp

Implements the interactive menu and user input flow.

Player.h / Player.cpp

Defines players and their statistics.

Team.h / Team.cpp

Defines the abstract team base class and common team behaviour.

TeamTypes.h / TeamTypes.cpp

Defines attacking, defensive, and balanced team types.

Lineup.h / Lineup.cpp

Selects and validates an 11-player starting line-up.

Match.h / Match.cpp

Models a match, its result, and its scorers.

Championship.h / Championship.cpp

Manages one season, its schedule, standings, and awards.

ChampionshipCollection.h / ChampionshipCollection.cpp

Manages the collection of seasons and file persistence.

Position.h / Position.cpp

Defines player positions and text conversion functions.

Utils.h / Utils.cpp

Provides input, output, validation, and file-format helpers.

Constants.h

Contains shared limits and the file separator.

Build and Run

Requirements

A C++11-compatible compiler or newer;

No external libraries are required.

Linux and macOS

From the source directory, run:

g++ -std=c++11 -Wall -Wextra -pedantic *.cpp -o football_championship
./football_championship

Windows with MinGW

g++ -std=c++11 -Wall -Wextra -pedantic *.cpp -o football_championship.exe
.\football_championship.exe

Main Menu

1. Create empty season
2. Create demo season
3. Add team to active season
4. Add or buy player
5. Remove player
6. Swap players between teams
7. Generate schedule
8. Play next round
9. Play all remaining rounds
10. Show standings and awards
11. Show team info
12. Show schedule
13. List seasons
14. Switch active season
15. Save all seasons
16. Load seasons
0. Exit

Quick Test

The fastest way to test the complete application flow is:

Select Create demo season.

Select Play all remaining rounds.

Enable automatic line-ups and automatic results.

Display the standings and awards.

Save all seasons to a file.

Restart the application, load the file, and list the available seasons.

Save-File Format

Data is stored in a custom text format. Each line begins with a record type, and fields are separated by |.

The main record types are:

FOOTBALL_CHAMPIONSHIPS - file version, number of seasons, and active season;

CHAMPIONSHIP - season year, current round, completion state, and record counts;

TEAM - team type, name, coach, stadium, budget, and statistics;

PLAYER - player identity, position, financial information, and statistics;

MATCH - round, teams, result, and number of scorers;

SCORER - team index and scorer name.

Validation and Error Handling

The application checks that:

team rosters do not exceed 20 players;

player names and numbers are not duplicated within a team;

valid teams contain exactly 2 goalkeepers and satisfy their team-type rules;

a championship contains at least 4 teams and an even number of teams before schedule generation;

a team has enough budget and roster space before buying a player;

both teams remain valid after a player exchange;

loaded files contain the expected records and structure;

invalid numeric and text input is rejected without terminating the application.

Possible Future Improvements

Detailed transfer and injury history;

A separate transfer market with players outside the existing teams;

A more realistic automatic match-result model;

Additional tie-breaking criteria for the standings.
