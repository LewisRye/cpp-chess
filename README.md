# Chess Game
This is the final project for the C++ Workshops. The idea is to create a chess game (excluding the moves empassant, castling, and pawn promotion). 

### Starter code
We've provided some starter code for dealing with the GUI and taking in user input, so when the game first runs 
- The board initializes with the pieces of each side in their correct locations
- These pieces are displayed on the UI
- White player starts

<img width="504" alt="Screenshot 2023-11-30 at 4 10 02 PM" src="https://github.com/Wenfei134/chess/assets/60166421/975f892b-0e97-416b-a4ef-57c1f9437b05">

Additionally we handle user input
- When a user clicks on a piece of their colour the legal moves for that piece are displayed 
    - Currently only the Pawn piece will return a list of moves, every other peice returns an empty list so it looks like nothing happens, but the input is still being handled correctly
- If a legal move is clicked, tell the game to move the piece. 
    - Currently the move function does nothing, so it is called but the UI doesn't change. The input is handled correctly but the game logic has not yet been implemented. 
- If a different piece is clicked, the legal moves of that piece is shown 
- If any other click happens, then the piece is deselected
- When a move happens, the turn switches to the opposite player.
 
<img width="503" alt="Screenshot 2023-11-30 at 4 12 57 PM" src="https://github.com/Wenfei134/chess/assets/60166421/4df5dae5-5646-4223-8053-8a4315e0c255">

### Not implemented
- Get legal moves of each piece* (except Pawn which has been implemented)
- Moving a piece when a move happens 
- Undo a move (important for other functions)
- Determining if King is in check for each colour 
- Determining if the game is in stalemate 
- Determining if the game is in checkmate

*Legal moves is actually a bit difficult to determine in chess and is done in two phases, with pseudolegal moves that are then filtered to get the actual legal moves. We will explain this in the workshop.

## System Pre-requisites
***
### Supported
- Windows 64bit with MingW
- MacOS

For Windows if you followed the set up from the first workshop, you *should* have MingW.

If your system is supported, we have the SDL2 libraries already included wtihin the GitHub repository. This means cloning the repository should be sufficient and no additional setup is required. Otherwise, continue reading.

### Not supported
- Linux
- Windows not MingW (Cygwin, MSYS, etc)
- Windows 32bit with MingW

If your system is not supported then you need to install SDL2 on your own. 

1. Follow this set up guide: [https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php)

2. Update the makefile for your OS so that the `INCLUDE_PATHS` and `LIBRARY_PATHS` point to where you downloaded SDL2. (If you're on Linux good luck, we do not have a Linux Makefile).

## Running the Game
***
In the `chess` directory run the command 

Unix: 

```make```

Followed by 

```./Chess```

Windows: 

```mingw32-make.exe```

Followed by

```.\Chess.exe```
