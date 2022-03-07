# Templated-Minimax-with-Othello-example
A templated, optimized implementation of the Minimax algorithm using closed set and a-b pruning. Includes a Othello/Reversi game using it as an opponent.


## How to use include Minimax in your project
The Minimax algorithm is included in the 3 header files in the 'minimax' folders. Simply copy these files into your project and include "Minimax.h".

To use the Minimax&lt;T&gt; class you will need to write a class representing your game's state and which implements the interface described in the class documentation of Minimax&lt;T&gt;. The T class must include:
  
 <ul>
    <li> A std::hash&lt;T&gt; function
    <li> A list&lt;T&gt; getChildren() method
    <li> A int getValue() method
    <li> A Position getLastMove() method
    <li> A bool isFinal() method
 </ul>

The Minimax&lt;T&gt; class also supports depth ramp-up; that is, it can start the first round with a shallower depth and gradually go up to the maximum depth as the game progresses.
This is useful for games like Othello where the first rounds have very limited options, so a costly, in-depth calculation doesn't make sesnse. Both standard and custom options are supported for this feature.

## How to play the game
The project includes a demo where a player can face off against the computer in a game of Othello. To play the demo, copy the files from both folders in the <b>same</b> directory and execute Game.cpp
