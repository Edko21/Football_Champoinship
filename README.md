# Football Championship Simulator

A **C++11 console-based application** for managing and simulating football championships, developed as a university Object-Oriented Programming project.

The system provides comprehensive team and player management, automated match simulations, championship statistics, and persistent data storage.

## Overview

- **Championship Management:** Create and manage multiple seasons, generate fixtures, and track league standings.
- **Teams & Players:** Manage attacking, defensive, and balanced teams, player transfers, budgets, and salaries.
- **Match Simulation:** Play matches manually or automatically, with customizable starting lineups and results.
- **Statistics & Awards:** Track goals, appearances, match results, rankings, championship winners, and top scorers.
- **Data Persistence:** Save and restore championship seasons using structured text files.
- **Validation:** Enforce team composition rules, financial restrictions, and data consistency.

> The application supports both manual management and fully automated championship simulations.

---

## Object-Oriented Architecture

The project follows fundamental **Object-Oriented Programming principles**, with separate classes responsible for different aspects of the championship.

| Principle | Implementation |
|---|---|
| **Encapsulation** | Classes manage their own data and behavior |
| **Inheritance** | Specialized team types derive from the abstract `Team` class |
| **Polymorphism** | Virtual methods implement team-specific rules |
| **Abstraction** | Common functionality is defined through abstract interfaces |
| **Memory Management** | Copy constructors, assignment operators, destructors, and `clone()` |
| **Composition** | Championships contain matches and teams; teams contain players |

### Core Components

| Class | Responsibility |
|---|---|
| `Player` | Player information, statistics, and financial data |
| `Team` | Team management, rosters, budgets, and transfers |
| `Match` | Match results, lineups, and scorers |
| `Championship` | Season management, scheduling, standings, and awards |
| `ChampionshipCollection` | Multiple seasons and data persistence |
| `Menu` | Console interface and user interaction |

---

## Championship Simulation

### Schedule Generation

The application automatically generates a **double round-robin schedule**, ensuring every team plays against each opponent twice — once at home and once away.

### Team Management

Three specialized team types are implemented:

- **AttackingTeam:** Focus
