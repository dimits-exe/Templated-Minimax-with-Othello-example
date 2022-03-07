#pragma once

#include "Minimax.h"

#include <limits>
#include <list>
#include <exception>
#include <math.h>


//some compilers like GCC don't have these included apparently
#ifndef INT_MIN
	#define INT_MIN std::numeric_limits<int>::min()
#endif

#ifndef INT_MAX
	#define INT_MAX std::numeric_limits<int>::max()
#endif

template <class T>
Minimax<T>::Context::Context(int searchDepth) : bestValue(INT_MIN), searchDepth(searchDepth) {}

template <class T>
void Minimax<T>::Context::store_move_if_best(Position& AI_move, int move_value, int remaining_depth) {
	if (searchDepth == remaining_depth + 1 && move_value > bestValue) {
		moveMade = AI_move;
		bestValue = move_value;
	}
}

template <class T>
Minimax<T>::Minimax(std::function<int(int)> depthCalculator, int maxDepth): turnsPlayed(0), depthCalculator(depthCalculator) {
	setMaxDepth(maxDepth);
}

template <class T>
int Minimax<T>::getMaxDepth() {
	return maxDepth;
}

template <class T>
void Minimax<T>::setMaxDepth(int newDepth) {
	if (newDepth <= 2) {
		throw std::logic_error("Depth must be more than 2.");
	}

	maxDepth = newDepth;
}

template <class T>
//I'm so proud of my boy, Im naming him Stockfish Jr
Position Minimax<T>::getBestMove(const T startingState) {
	turnsPlayed++;

	int searchDepth = std::min(maxDepth, depthCalculator(turnsPlayed));
	Context context(searchDepth);

	min_value(context, startingState, INT_MIN, INT_MAX, searchDepth);

	return context.moveMade;
}

template <class T>
int Minimax<T>::min_value(Context& context, const T& state, int a, int b, int remaining_depth) {

	//if the game is predicted to end or if the state was already examined or the recursion has to end
	if (state.isFinal() || remaining_depth < 0 || context.closedSet.count(state) == 1)
		return state.getValue();

	context.closedSet.insert(state);
	int value = INT_MAX;
	const T* min_value_state = nullptr;  //will point to the state with the min value

	const std::list<T>& children = state.getChildren();

	if (children.size() == 0) {						//if no move is possible, defer to max_value
		return max_value(context, state, a, b, remaining_depth - 1);
	}

	//examine all children and find the least fit one
	for (const T& child : children) {
		int max_v = max_value(context, child, a, b, remaining_depth - 1);
		if (value > max_v) {
			value = max_v;
			min_value_state = &child;
		}

		if (value <= a) {
			return child.getValue();
		}

		b = std::min(b, value);
	}

	if (min_value_state == nullptr) {
		return state.getValue();
	}
	else {
		return min_value_state->getValue();
	}

}

template <class T>
int Minimax<T>::max_value(Context& context, const T& state, int a, int b, int remaining_depth) {

	Position AI_move;
	if (context.searchDepth == remaining_depth + 1) {				//if directly below the root of the search tree
		AI_move = state.getLastMove();								//save the move made
	}

	//if the game is predicted to end or if the state was already examined or the recursion has to end
	if (state.isFinal() || remaining_depth < 0 || context.closedSet.count(state) == 1) {
		int value = state.getValue();
		context.store_move_if_best(AI_move, value, remaining_depth);
		return value;
	}

	context.closedSet.insert(state);
	int value = INT_MIN;
	const T* max_value_state = nullptr;	//will point to the state with the max value
	const std::list<T>& children = state.getChildren();

	if (children.size() == 0) {						//if no move is possible, defer to max_value
		return min_value(context, state, a, b, remaining_depth - 1);
	}

	//examine all children and find the fittest one
	for (const T& child : children) {
		int min_v = min_value(context, child, a, b, remaining_depth - 1);
		if (value < min_v) {
			value = min_v;
			max_value_state = &child;
		}

		if (value >= b) {
			context.store_move_if_best(AI_move, value, remaining_depth);
			return child.getValue();
		}

		a = std::max(a, value);
	}

	if (max_value_state == nullptr) {
		return state.getValue();
	}
	else {
		context.store_move_if_best(AI_move, value, remaining_depth);
		return max_value_state->getValue();
	}

}

template <class	T>
int Minimax<T>::exponential_calculator(int turnsPlayed) {
	return static_cast <int>(std::floor(std::pow(turnsPlayed, 1.5))) + 1;
}

template <class	T>
int Minimax<T>::linear_calculator(int turnsPlayed) {
	return turnsPlayed + 2;
}

template <class	T>
int Minimax<T>::always_max_calculator(int turnsPlayed) {
	return INT_MAX;
}
