# foompello
Foompello is a C++ implementation of game of othello (often mistakenly referred to as "reversi") using SFML


Requirements: 1) C++11, 2) SFML 2.3.2

The program was tested on Ubuntu 16.04 LTS Xenial, gcc 5.4.0.


Instructions:
1) For an out-of-place build: create a build directory (mkdir build)
2) Change directory to the created build directory (cd build)
3) Create build files with CMake in the build directory (cmake ..)
4) Build the project (e.g. for Unix makefiles it is achieved by using "make" command in the terminal)
5) Launch the executable file "foompello"
6) Choose color of your pieces and AI playing level in the Options menu.
7) You can also choose analysis mode. In this mode human player makes moves for both colors. After the move is made foompello shows it's evaluations (on depth 10) for all valid moves. The higher the evaluation the better is the move according to foompello.
8) Mouse controls. Click menu buttons with left mouse click. Left mouse click on the board square to make a move.

Foompello uses bitboards for board representation and move generation. Computer move search algorithm is based on negamax with alpha-beta pruning and iterative deepening. Heuristic evaluation fuction is handcrafted and reflects major strategical principles used by strong human players (number of corner discs, numbers of x-square discs, mobility, potential mobility etc.).

Currently playing strength of Foompello is above human beginner level and below human expert level. I hope it give a tough fight to experienced human players who don't consider themselves experts. Also note that currently Foompello is much weaker than strongest othello programs like Edax, WZebra and Ntest.

The project is still in the development stage and there are many things yet to be implemented.

The name of the program is a tribute to Ben Seeley (aka foompy_katt) - person who is an inspiration for me (and many other othello players around the World).

To learn more about game of othello please refer to the following wikipedia article: https://en.wikipedia.org/wiki/Reversi
