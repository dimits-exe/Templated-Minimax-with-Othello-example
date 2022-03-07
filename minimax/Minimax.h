#pragma once
#include "Position.h"

#include <unordered_set>
#include <functional>

/// <summary>
/// An AI player using the Minimax algorithm with closed set and a-b pruning to determine the 
/// best move according to a given state.
/// 
/// The state is given as a template for optimization purposes and must include;
/// * A std::hash<T> function
/// * A list<T> getChildren() method
/// * A int getValue() method
/// * A Position getLastMove() method
/// * A bool isFinal() method
/// </summary>
/// <typeparam name="T">The state used to evaluate the best move.</typeparam>
template <class T>
class Minimax {

public:

	/// <summary>
	/// A function scaling depth exponentially to a factor of 1.5.
	/// </summary>
	static int exponential_calculator(int turnsPlayed);

	/// <summary>
	/// A function scaling depth linearly, adding one to the depth for each turn.
	/// </summary>
	static int linear_calculator(int turnsPlayed);

	/// <summary>
	/// A function which always uses the max depth, effectively ignoring how many turns have been played.
	/// </summary>
	static int always_max_calculator(int turnsPlayed);

	/// <summary>
	/// Initializes the minimax algorithm. The object should be constructed once per game.
	/// </summary>	
	/// 
	/// <param name="max_depth">
	/// The number of turns the algorithm will look ahead before making a decision.
	/// A number between 5 and 9 is recommended. Default is 5.
	/// </param>
	/// 
	/// <param name="depthCalculator">
	/// A function taking as input the turns played so far and returning the ideal
	/// search depth. Standard functions are provided in this header file. 
	/// Default is always_max_calculator.
	/// </param>
	/// 
	Minimax(std::function<int(int)> depthCalculator = always_max_calculator, int max_depth = 5);

	/// <summary>
	/// Consult the algorithm about the best move to make according to the current state.
	/// The algorithm will run to a depth specified by the depthCalculator function, but will
	/// never exceed the provided max depth.
	/// </summary>
	/// <param name="turn">The player representing the computer.</param>
	/// <param name="start"></param>
	/// <returns>The best move for the computer.</returns>
	Position getBestMove(const T start);

	/// <summary>
	/// Returns the max depth of the algorithm
	/// </summary>
	int getMaxDepth();

	/// <summary>
	/// Sets a new max depth for the algorithm.
	/// </summary>
	/// <param name="newDepth"> The new max depth of the algorithm.</param>
	/// <throws> logic_error if less than 2 </throws>
	void setMaxDepth(int newDepth);


private:

	/// <summary>
	/// Runtime state for the getBestMove method.
	/// </summary>
	struct Context {
		std::unordered_set <T> closedSet;
		const int searchDepth;
		int bestValue;
		Position moveMade;

		Context(int searchDepth);

		/// <summary>
		/// Stores the move if it's better than the currently best one in the Info object.
		/// </summary>
		/// <param name="AI_move">The attempted move by the AI.</param>
		/// <param name="move_value">The value of the board after the supplied move.</param>
		/// <param name="curr_depth">The current depth of the DFS search.</param>
		void store_move_if_best(Position& AI_move, int move_value, int remaining_depth);
	};

	int maxDepth;
	int turnsPlayed;
	std::function<int(int)> depthCalculator;

	/// <summary>
	/// The 'min' half of the alpha-beta pruning algorithm.
	/// </summary>
	/// <param name="state">The state to be expanded and examined.</param>
	/// <param name="a">The parameter of the node's max value</param>
	/// <param name="b">The parameter of the node's min value</param>
	/// <param name="TTL">How many more calls will be made before the algorithm terminates.</param>
	/// <returns>The state with the current best outcome for the computer.</returns>
	int min_value(Context& context, const T& state, int a, int b, int TTL);

	/// <summary>
	/// The 'max' half of the alpha-beta pruning algorithm.
	/// </summary>
	/// <param name="state">The state to be expanded and examined.</param>
	/// <param name="a">The parameter of the node's max value</param>
	/// <param name="b">The parameter of the node's min value</param>
	/// <param name="TTL">How many more calls will be made before the algorithm terminates.</param>
	/// <returns>The state with the current best outcome for the computer.</returns>
	int max_value(Context& context, const T& state, int a, int b, int TTL);

};

#include "Minimax.hpp"
