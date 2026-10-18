Football Championship Simulator

A C++11 console-based application for managing and simulating football championships, developed as a university Object-Oriented Programming project.

The system provides comprehensive team and player management, automated match simulations, championship statistics, and persistent data storage.

Overview

Championship Management: Create and manage multiple seasons, generate fixtures, and track league standings.

Teams & Players: Manage attacking, defensive, and balanced teams, player transfers, budgets, and salaries.

Match Simulation: Play matches manually or automatically, with customizable starting lineups and results.

Statistics & Awards: Track goals, appearances, match results, rankings, championship winners, and top scorers.

Data Persistence: Save and restore championship seasons using structured text files.

Validation: Enforce team composition rules, financial restrictions, and data consistency.

The application supports both manual management and fully automated championship simulations.

Object-Oriented Architecture

The project follows fundamental Object-Oriented Programming principles, with separate classes responsible for different aspects of the championship.

Principle

Implementation

Encapsulation

Classes manage their own data and behavior

Inheritance

Specialized team types derive from the abstract Team class

Polymorphism

Virtual methods implement team-specific rules

Abstraction

Common functionality is defined through abstract interfaces

Memory Management

Copy constructors, assignment operators, destructors, and clone()

Composition

Championships contain matches and teams; teams contain players

Core Components

Class

Responsibility

Player

Player information, statistics, and financial data

Team

Team management, rosters, budgets, and transfers

Match

Match results, lineups, and scorers

Championship

Season management, scheduling, standings, and awards

ChampionshipCollection

Multiple seasons and data persistence

Menu

Console interface and user interaction

Championship Simulation

Schedule Generation

The application automatically generates a double round-robin schedule, ensuring every team plays against each opponent twice — once at home and once away.

Team Management

Three specialized team types are implemented:

AttackingTeam: Focuses on offensive players.

DefensiveTeam: Prioritizes defensive and midfield positions.

BalancedTeam: Maintains an evenly distributed squad.

Each team follows specific roster requirements, enforced through polymorphic validation.

Standings & Statistics

Championship standings are calculated based on:

Total points

Goal difference

Goals scored

Goals conceded

Alphabetical order

Player performance is tracked throughout the season, including appearances, goals, and salary adjustments.

Technologies & Concepts

Category

Technologies

Language

C++11

Programming Paradigm

Object-Oriented Programming

Libraries

C++ Standard Library (STL)

Core Concepts

Inheritance, Polymorphism, Encapsulation, Abstraction

Additional Concepts

Dynamic Memory Management, File I/O, Input Validation

Build & Run

Requirements: A C++11-compatible compiler (GCC / MinGW).

Navigate to the proj directory.

Linux / macOS

Compile: g++ -std=c++11 *.cpp -o football_championship

Run: ./football_championship

Windows (MinGW)

Compile: g++ -std=c++11 *.cpp -o football_championship.exe

Run: .\football_championship.exe

No external dependencies are required.

Quick Start

Launch the application.

Select Create demo season to initialize a championship with 8 teams.

Choose Play all remaining rounds to simulate the season automatically.

View the final standings, player statistics, and championship awards.

Save the championship to a file and restore it in a future session.

Future Improvements

Advanced match simulation with realistic player performance.

Injury tracking and detailed transfer histories.

Improved transfer market functionality.

Extended statistics and performance analysis.
