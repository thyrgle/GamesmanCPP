#include <iostream>
#include <string>
#include <algorithm>
#include <tuple>
#include <queue>
#include <map>
#include <functional>

enum Status {
    /**
      Is a particular position a WIN, LOSS, TIE, or is it not
      immediately obvious (UNDECIDED).
      */
    WIN,
    LOSS,
    TIE,
    UNDECIDED
};

template<typename Statuses>
Status reduce_statuses(Statuses const & children) {
    /**
      Takes statuses and reduces them to (ultimately) determine
      a parent node's status.
      To illustrate say we have a portion of a game tree:
          P1       A
                 /   \
          P2    B     C
      Say that we don't know A's status, but B is a WIN for P2
      and C is a LOSS for P2. At position A, P1 can go down and
      choose position C forcing P2 to lose (and P1 to win).
      In general if there is one losing position, the current 
      player can go down it. If there is no losing position then
      the current player must lose.
      */
    auto tie = find(std::begin(children), std::end(children), TIE);
    if (tie != children.end()) return TIE;
    //Check if there is a loss in the children.
    auto loss = find(std::begin(children), std::end(children), LOSS);
    if (loss != std::end(children)) return WIN;
    return LOSS;
}

template <typename Remotes>
int calculate_remoteness(Status parent_state, Remotes children) {
    /**
      Given the status of the parent node, calculate the remoteness given
      an iterable of the statuses of the children nodes.
      */
    switch (parent_state) {
        case WIN:
        case LOSS:
        case TIE:
        case UNDECIDED:
        default:
            break;
    }
    return 0;
}

std::string to_string(Status s) {
    switch (s) {
        case WIN:       return "WIN";
        case LOSS:      return "LOSS";
        case TIE:       return "TIE";
        case UNDECIDED: return "UNDECIDED";
    }
}

template <typename InitialFn,
         typename GenerateFn,
         typename DoMoveFn,
         typename PrimitiveFn> // Movement, or action on a board
         struct Game {
         /**
          Abstract class that describes an Abstract Two Player
          Strategy Game.
          initial_position: The start state of the board game.
          generate_moves: Generate all valid moves given a 
          particular board state.
          do_move: Given a position, perform a move on that 
          position.
          primitive: Test if the game is over.
          */
             InitialFn initial_position;
             GenerateFn generate_moves;
             DoMoveFn do_move;
             PrimitiveFn primitive;
         };

template <typename Game, typename Position>
void solve_pos(Game &game, Position position) {
    /**
      Solve a game at a particular position.
      (Assume it is from P1's perspective)
      */
    auto const moves = game.generate_moves(position);
    std::queue<std::tuple<Position, Position>> children;
    for (auto const move: moves) {
        children.push(std::make_tuple(game.do_move(position, move), position));
    }
    std::map<Position, Status> results;
    std::map<Position, int> remotes;
    // Perform breadth first search on children
    while (!children.empty()) {
        // Really a child, parent tuple because you may need to update the parent.
        auto const child = children.front();
        children.pop();
        if (game.primitive(std::get<0>(child)) != UNDECIDED) {
            // Check if the child can be trivially solved.
            results[std::get<0>(child)] = game.primitive(std::get<0>(child));
            results[std::get<1>(child)] = results[std::get<0>(child)];
        } else if (results.find(std::get<0>(child)) == std::end(results)) {
            // Found in results, no more computation needed.
            results[std::get<1>(child)] = results[std::get<0>(child)];
        } else {
            // Otherwie continue the search.
            auto const moves = game.generate_moves(std::get<0>(child));
            for (auto const move: moves) {
                children.push(std::make_tuple(game.do_move(std::get<0>(child), move), std::get<0>(child)));
            }
        }
    }
}

template <typename Game>
void solve(Game& game) {
    /**
      Determine whether it is a WIN, LOSS, or TIE for P1.
      */
    return solve_pos(game, game.initial_position());
}
