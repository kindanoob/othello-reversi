# othello1
![Othello1_screenshot](resources/othello1.png?raw=true "Othello1")
This is a C++ implementation of game of othello (often called "reversi", although these are different games) using SFML


Requirements: 1) C++14, 2) SFML 2.6.1, 3) CMake 3.22.1
    
The program was tested on Ubuntu 22.04 LTS Jammy, gcc 11.4.0.

Instructions for an out-or-place build:
1) Change the SFML_ROOT variable in the CMakeLists.txt file according to your SFML location.
2) Go to the directory containing the CMakeLists.txt file.
3) Run `cmake -S . -B build`. This will create a `build` directory and put CMake files there.
4) Go to the `build` directory.
5) Build the project (e.g. for Unix makefiles it is achieved by running `make` command in the terminal).
6) Launch the executable file "othello1".
7) Choose color of your pieces and AI playing level in the Options menu.
8) You can also choose analysis mode. In this mode human player makes moves for both colors. After the move is made othello1 shows it's evaluations (on depth 10) for all valid moves. The higher the evaluation the better is the move according to othello1.
9) Mouse controls. Click menu buttons with left mouse click. Left mouse click on the board square to make a move.

This Othello program uses bitboards for board representation and move generation. Computer move search algorithm is based on Principal variation search with iterative deepening. Heuristic evaluation function is handcrafted and reflects major strategical principles used by strong human players (number of corner discs, numbers of x-square discs, mobility, potential mobility etc.).

Currently playing strength of Othello1 is above human beginner level and below human expert level. I hope it can give a tough fight to experienced human players who don't consider themselves experts. Also note that currently Othello1 is much weaker than strongest othello programs like Edax, WZebra and Ntest which can defeat human expert players easily.

The project is still in the development stage and there are many things yet to be implemented.

To learn more about game of othello please refer to the following wikipedia article: https://en.wikipedia.org/wiki/Reversi
