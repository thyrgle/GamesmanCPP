#include <iostream>
#include <string>
#include <algorithm>
#include <tuple>

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
				P2	  B     C
	   Say that we don't know A's status, but B is a WIN for P2
	   and C is a LOSS for P2. At position A, P1 can go down and
	   choose position C forcing P2 to lose (and P1 to win).
	   In general if there is one losing position, the current 
	   player can go down it. If there is no losing position then
	   the current player must lose.
    */
	auto tie = find(children.begin(), children.end(), TIE);
	if (tie != children.end()) return TIE;
	//Check if there is a loss in the children.
	auto loss = find(children.begin(), children.end(), LOSS);
	if (loss != children.end()) return WIN;
	return LOSS;
}

template <typename Remotes>
int calculate_remoteness(Status parent_state, Remotes children) {
	/**
	  Given the status of the parent node, calculate the remoteness given
	  an iterable of the statuses of the children nodes.
	*/
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
		InitialFn initial_position __attribute__((const));
		GenerateFn generate_moves __attribute__((const));
		DoMoveFn do_move __attribute__((const));
		PrimitiveFn primitive __attribute__((const));
};

template <typename Game, typename Position>
std::tuple<Status, int> solve_pos(Game & game, Position position) {
	/**
	  Solve a game at a particular position.
	  (Assume it is from P1's perspective)
	*/
	// If the position can be trivial solved, we are done.
	if (game.primitive(position) != UNDECIDED) {
		return game.primitive(position);
	}
	// Otherwise we must recurse downward!
	std::vector<Status> children_statuses;
	auto const moves = game.generate_moves(position);
	std::transform(std::begin(moves), std::end(moves),
				   std::back_inserter(children_statuses),
				   [=](auto const & move) {
				       return std::get<0>(solve_pos(game,
							   			game.do_move(position, move)));
				   });
	return std::make_pair(reduce_statuses(children_statuses), 0);
}

template <typename Game>
std::tuple<Status, int> solve(Game& game) {
	/**
	  Determine whether it is a WIN, LOSS, or TIE for P1.
	*/
	return solve_pos(game, game.initial_position());
}
