#include <iostream>
#include <vector>
#include "solver.h"

class FourToOne : public Game<std::function<int()>,
    std::function<std::vector<int>(int)>,
    std::function<int(int, int)>,
    std::function<Status(int)> > {
        /**
          Implementation of the game FourToOne.

          Description of Game:
          Imagine you and a friend are playing with 4 balls.
          You go first and may either take 1 or 2 away.
          Then your friend goes and can take 1 or 2 away.
          This is reapeated until there are no balls left.
          Who ever cannot make a move looses.
          */
        public:
            FourToOne() : Game() {}
            int initial_position() const;
            std::vector<int> generate_moves(int position) const;
            int do_move(const int position, const int move) const;
            Status primitive(int position) const;
    };

int FourToOne::initial_position() const {
    /**
      Start the game at 4.
      */
    return 4;
}

std::vector<int> FourToOne::generate_moves(int position) const {
    /**
      Generate the moves for FourToOne.
      You can either decided to take 2 or 1 away from the board
      unless you are at 1 (in which case you can only take 1
      away), or when you are at 0 (you lost).
Note: Moves have been represented as negative integers i.e.
-1 -> take one away
-2 -> take two away
*/
    std::vector<int> moves(0);
    if (position == 1) {
        moves.push_back(-1);
        return moves;
    } else if (position == 0) {
        return moves; // No moves!
    } else {
        moves.push_back(-1);
        moves.push_back(-2);
        return moves;
    }
    // Should really never get here...
    return std::vector<int>();
}

int FourToOne::do_move(const int position, const int move) const {
    /**
      Perform a (valid) move on a given position.
      Moves are either -1 or -2, so adding it to the current
      board position gives a new position.
      */
    return position + move;
}

Status FourToOne::primitive(const int position) const {
    /**
      It is obvious a game is over when you are at 0, return 
      a LOSS.
      Otherwise, we can't tell immediately (in which case
      return UNDECIDED).
      */
    if (position == 0) return LOSS;
    return UNDECIDED;
}

int main() {
    /**
      Launches the game FourToOne and then sovles it.
      */
    FourToOne game = FourToOne();
    std::cout << to_string(std::get<0>(solve(game)));
    return 0;
}
