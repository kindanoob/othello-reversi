# foompello
C++ implementation of game of othello (often mistakenly referred to as "reversi") using SFML

Requirements: 1) C++11, 2) SFML 2.3.2

The program was tested on Ubuntu 16.04 LTS Xenial, gcc 5.4.0.


Instructions:
1) For an out-of-place build: create a build directory (mkdir build)
2) Change directory to the created build directory (cd build)
3) Create build files with CMake in the build directory (cmake ..)
4) Build the project (e.g. for Unix makefiles it is achieved by typing "make" without quotes in the terminal)
5) Launch the executable file "foompello"
6) Choose color of your pieces and AI playing level in the Options menu.
7) You can also choose analysis mode. In this mode human player makes moves for both colors. After the move is made foompello shows it's evaluations (on depth 10) for all valid moves. The higher the evaluation the better is the move according to foompello.
8) Mouse controls. Click menu buttons with left mouse click. Left mouse click on the board square to make a move.

Some concepts used in the project: bitboards, minimax, negamax, alpha beta pruning, principle variation search (PVS), iterative deepening, heuristic evaluation fuction.

The project is still in the development stage and there are many things yet to be implemented.

To learn more about game of othello please refer to the following wikipedia article: https://en.wikipedia.org/wiki/Reversi
