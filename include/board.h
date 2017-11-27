#ifndef BOARD_H_
#define BOARD_H_

#include <iostream>
#include <SFML/Graphics.hpp>
#include <bitset>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include<unordered_map>

#include "config.h"

struct ScoreMove {
    ScoreMove(){};
    ScoreMove(u64 l_move, double l_score) : move(l_move), score(l_score){};    
    u64 move = 0ULL;
    double score = 0.0;
};

/*
//structures of this kind are used as values in the memo dictionary
struct PositionInfo {
    double eval = 0.0;
    u64 move = 0ULL;
    int depth = 0;
    int colorto_move_ = 0;
    int flag = 0;//-1 means LOWER_BOUND, 0 means EXACT, 1 means UPPER_BOUND
    //enum FLAG {LOWER_BOUND = -1, EXACT, UPPER_BOUND};
};
*/


//indicator bitboards representing particular squares on the board
const u64 kIndicatorBitboards[] = {1ULL,    
                                    1ULL << 1,
                                    1ULL << 2,
                                    1ULL << 3,
                                    1ULL << 4,
                                    1ULL << 5,
                                    1ULL << 6,
                                    1ULL << 7,
                                    1ULL << 8,
                                    1ULL << 9,
                                    1ULL << 10,
                                    1ULL << 11,
                                    1ULL << 12,
                                    1ULL << 13,
                                    1ULL << 14,
                                    1ULL << 15,
                                    1ULL << 16,
                                    1ULL << 17,
                                    1ULL << 18,
                                    1ULL << 19,
                                    1ULL << 20,
                                    1ULL << 21,
                                    1ULL << 22,
                                    1ULL << 23,
                                    1ULL << 24,
                                    1ULL << 25,
                                    1ULL << 26,
                                    1ULL << 27,
                                    1ULL << 28,
                                    1ULL << 29,
                                    1ULL << 30,
                                    1ULL << 31,
                                    1ULL << 32,
                                    1ULL << 33,
                                    1ULL << 34,
                                    1ULL << 35,
                                    1ULL << 36,
                                    1ULL << 37,
                                    1ULL << 38,
                                    1ULL << 39,
                                    1ULL << 40,
                                    1ULL << 41,
                                    1ULL << 42,
                                    1ULL << 43,
                                    1ULL << 44,
                                    1ULL << 45,
                                    1ULL << 46,
                                    1ULL << 47,
                                    1ULL << 48,
                                    1ULL << 49,
                                    1ULL << 50,
                                    1ULL << 51,
                                    1ULL << 52,
                                    1ULL << 53,
                                    1ULL << 54,
                                    1ULL << 55,
                                    1ULL << 56,
                                    1ULL << 57,
                                    1ULL << 58,
                                    1ULL << 59,
                                    1ULL << 60,
                                    1ULL << 61,
                                    1ULL << 62,
                                    1ULL << 63,
                                    };








/*
  Square numbering:

  63 62 61 60 59 58 57 56
  55 54 53 52 51 50 49 48
  47 46 45 44 43 42 41 40
  39 38 37 36 35 34 33 32
  31 30 29 28 27 26 25 24
  23 22 21 20 19 18 17 16
  15 14 13 12 11 10  9  8
   7  6  5  4  3  2  1  0
*/




class Board {
public:
    Board();
    Board(const Board& another_board);
    u64 WhiteBitboard();
    u64 BlackBitboard();
    void SetBlackBitboard();
    int WhiteScore();
    int BlackScore();
    u64 Occupied();
    u64 Empty();    
    void DrawBoard(const sf::RectangleShape& board_rect, const sf::Text& score_text, sf::RenderWindow& window);
    u64 GenerateMoves(u64 my_bitboard, u64 opp_bitboard);
    void MakeMove(int color, u64 my_move);
    u64 MakeMoveImproved(int color, u64 my_move);
    void UnmakeMove(int color, u64 m_move, u64 tiles_to_flip);
    void UndoMove(int color, u64 my_move);

    u64 North(u64 bitboard);
    u64 North2(u64 bitboard);
    u64 North4(u64 bitboard);
    u64 South(u64 bitboard);
    u64 South2(u64 bitboard);
    u64 South4(u64 bitboard);
    u64 East(u64 bitboard);
    u64 East2(u64 bitboard);
    u64 East4(u64 bitboard);
    u64 West(u64 bitboard);
    u64 West2(u64 bitboard);
    u64 West4(u64 bitboard);
    u64 NorthEast(u64 bitboard);
    u64 NorthEast2(u64 bitboard);
    u64 NorthEast4(u64 bitboard);
    u64 NorthWest(u64 bitboard);
    u64 NorthWest2(u64 bitboard);
    u64 NorthWest4(u64 bitboard);
    u64 SouthEast(u64 bitboard);
    u64 SouthEast2(u64 bitboard);
    u64 SouthEast4(u64 bitboard);
    u64 SouthWest(u64 bitboard);
    u64 SouthWest2(u64 bitboard);
    u64 SouthWest4(u64 bitboard);

    u64 SaturateNorth(u64 x, u64 obstacles);
    u64 SaturateSouth(u64 x, u64 obstacles);
    u64 SaturateEast(u64 x, u64 obstacles);
    u64 SaturateWest(u64 x, u64 obstacles);
    u64 SaturateNorthEast(u64 x, u64 obstacles);
    u64 SaturateNorthWest(u64 x, u64 obstacles);
    u64 SaturateSouthEast(u64 x, u64 obstacles);
    u64 SaturateSouthWest(u64 x, u64 obstacles);

    bool IsOnBoard(u64 move);
    bool IsOnCorner(u64 move);
    std::vector<int> GetScore();
    int BitboardPopcount(u64 bitboard);
    int BitboardPopcount2(u64 b);
    void Reset();
    u64 ValidMoves(int color);
    u64 ValidMovesTwo(int color);
    u64 ValidMovesImproved(int color);
    u64 RandomComputerMove(int color);
    int BitScanForward(u64 bb);

    double EvalBoardSort(int color, const std::array<int, 65536>& popcount_hash_table);

    void PrintBitboardToConsole(u64 b);
    void PrintBoardToConsole();

    double EvalBoard(int move_number, int color, const std::array<int, 65536>& popcount_hash_table);
    void ConvertToBinary(u64 bitboard);
    std::vector<u64> GenPossibleMovesIndicators(u64 possible_moves);
    std::vector<u64> GenPossibleMovesIndicatorsImproved(u64 data);
    std::vector<ScoreMove> GenPossibleMoveScorePairsOne(const std::vector<u64>& possible_moves_indicators, 
        int color, int move_number, const std::array<int, 65536>& popcount_hash_table);
    std::vector<ScoreMove> GenPossibleMoveScorePairsTwo(const std::vector<u64>& possible_moves_indicators, 
        int color, int move_number);
    void OrderMoveScorePairsByEvalDescending(std::vector<ScoreMove>& possible_move_score_pairs);
    void OrderMovesByEvalDescending(std::vector<u64>& valid_moves);

    bool IsTerminalNode();
    
private:
    u64 white_bitboard_ = 0ULL;
    u64 black_bitboard_ = 0ULL;
};

#endif // BOARD_H_