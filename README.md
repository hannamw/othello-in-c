# othello-in-c
The game Othello, written in C. Play against another player or a computer of varying difficulties. Or, watch two computers play! The AI evaluates board states as far as 6 moves into the future, so it's quite good at finding the optimal move.

## Usage
Play the game by running play.exe. You can set player types via two flags, "-b" and "-w", for black and white, followed by an argument indicating the player type. Any unspecified players will be assumed to be human.

|Argument|Meaning|
|---|---|
|h|This color will be controlled via your human input to the program!|
|m|Minimax AI: The computer will pick the move that leads to the optimal board state 6 moves from now (assuming that you, too, pick the optimal moves). This is very hard to beat.
|e|Edge AI: The computer will choose a valid move that leads to the best board state, not looking ahead any further than this move. It prefers edges and corners.|
|f|First-move AI: The computer just makes the first legal move it finds.|

For example, to play a game between two bots, where the white player uses the edge strategy, and the black player uses the minimax strategy, you would run "./play.exe -w e -b m"


When playing the game, specify the location at which you want to place a piece via a two digit number "xy", where x is the row, and y is the column.
