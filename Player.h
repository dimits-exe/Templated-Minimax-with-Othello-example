#pragma once

/// <summary>
/// An enum describing who occupies a certain square.
/// </summary>
enum class PLAYER : char { EMPTY = '-', PLAYER1 = '@', PLAYER2 = 'X' };

/// <summary>
/// Get the next player's turn.
/// </summary>
/// <param name="currentTurn">The current turn.</param>
inline PLAYER nextTurn(PLAYER currentTurn) {
	if (currentTurn == PLAYER::PLAYER1)
		return PLAYER::PLAYER2;
	else
		return PLAYER::PLAYER1;
}