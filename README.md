# GamesmanCPP

A game solver similar to [GamesmanClassic](https://github.com/GamesCrafters/GamesmanClassic) but in C++. 

## How it works

Board games (roughly speaking) can be represented with the four following functions:

    initial_position    -> Starting board position
    generate_moves(pos) -> Generate all valid moves given a position.
    do_move(pos, move)  -> Given a position perform a valid move on it.
    primitive           -> Check if the game is over.

Using these methods a game tree can be generated and completely traversed. One can go back up the tree to determine if Player 1 wins a game.

As an example, the game four of to zero is implemented. The game four to zero works by starting off with four balls. The first player may choose to take away 2 or 1 balls, then the second player may do the same. This is repeated until there are no balls left. Whoever cannot make a move loses.
