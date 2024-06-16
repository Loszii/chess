# Chess

This **C/C++** application includes a playable chess engine with a built in algorithm to play against. I made this engine from scratch to display my love for chess and computer science.

# How It Works

- The board is represented using a **Board** class that contains information about the game state. The board's data is represented using an **array** of size 64, as a one dimensional array is more efficient than my previous 2D implementation. 
- Pieces are represented using different numbers Ex: 4 is a white rook and -4 is a black rook.
- Firstly, each piece has a function which gets its trajectories, this would be considered it's **psuedo-legal moves**.
- Secondly, using the above functions my program makes each move to detect checks on the current players king; if the king is safe then the move is considered **legal**.
- Once legal move generation was performing properly, I added things like en passant, castling and pawn promotion to finalize all of the movement rules.
- The only things left to implement were things like checkmate detection, draw by repition, and draw by insufficient material.

# Artifical Intelligence

The AI in my program uses a common Artificial Intelligence algorithm called minimax, or more specifically in this case **negamax**. The AI plays at around **1000** elo and seems capable of beating beginner-medium level chess players.

-  Minimax works by first utilizing a **evaluation** function to evaluate a board for the current player. A positive number indicates a winning position for white and negative number a winning for black **(Note: negamax has positive for current player)**.
- The function **recursively** makes all legal moves to a certain depth, once that depth is reached it returns the static board evaluation. If it was whites turn previously, the function will iterate through these results and **maximize**  the evaluation, returning the max. Since it will now be black's turn and a lower eval is in black's favor, we will now **minimize** all of the evaluations. Practically, our algorithm is simulating best play from both sides, to minimize risk and play the best move that is guranteed to be in the engine's favor.
- My program uses a type of minimax called **negamax**, this version is just a way of simplifying the code into a more readable function using the fact: **max(a, b) == -min(-a, -b)**. In other words, if a positive evaluation means better for the current player to move and a specific board is a +5 for black, this is the same as a -5 for white. This is because one players benefit is another player loss. By using a evaluation function that returns positive for whose turn it is, we can have both players maximizing by negating the previous evaluation. This effectively cuts the code in half while doing the same thing!
- Many optimizations are needed, since a faster program can search more moves and as a result will perform better. One major optimization is **alpha-beta pruning**, which prunes boards we know will not be optimal for the player based off of previously evaluated game states.

- More info here: https://www.chessprogramming.org/Minimax, https://www.chessprogramming.org/Negamax

# Libraries

The entire chess engine was made using basic C++ code. The GUI was made possible using **Raylib** and **GoogleTests** was used to test move generation via a method called **"Perft"**. Perft simply iterates through all possible different boards that can come from a single board at a certain depth. By achieving the correct numbers this ensures moves like en passant and castling are properly accounted for, as well as legality. Finally, I used **CMake** to build my program.
- More info here: https://www.chessprogramming.org/Perft_Results

# How to Play

To play my chess engine you can either compile the program yourself using CMake, or navigate to the **installer** directory and run the **.exe** to get the latest version.

<div>
  <img src="/images/start_board.jpg" width="45%">
  <img src="/images/check_mate.jpg" width="45%">
</div>