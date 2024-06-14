# About

This **C/C++** application includes a playable chess engine with a mid-range algorithm to play against. I made this engine from scratch to display my love for chess and computer science.

# Algorithm

The AI in my program uses a common Artificial Intelligence algorithm called minimax, or more specifically in this case **negamax**. I've implemented multiple optimizations like alpha beta pruning. The AI plays like a **800-1000** elo player and seems capable of beating beginner-medium level chess players.

# Libraries

The entire chess engine was made using basic C++ code. The GUI was made possible using **Raylib** and GoogleTests was used to test move generation via a method called **"Perft"**.

![Board](/res/readme_board.jpg)

# How to Play

To try and beat my mid-range AI, you can either compile the program yourself using cmake, or navigate to the **installer** directory and run the **.exe** to get the latest version.

![CheckMate](/res/readme_mate.jpg)