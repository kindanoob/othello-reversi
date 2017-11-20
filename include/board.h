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
    double score = 0.0;
    u64 move = 0ull;
};

/*
//structures of this kind are used as values in the memo dictionary
struct PositionInfo {
    double eval = 0.0;
    u64 move = 0ull;
    int depth = 0;
    int colorto_move_ = 0;
    int flag = 0;//-1 means LOWER_BOUND, 0 means EXACT, 1 means UPPER_BOUND
    //enum FLAG {LOWER_BOUND = -1, EXACT, UPPER_BOUND};
};
*/


//indicator bitboards representing particular squares on the board
const u64 kIndicatorBitboards[] = {1ull,    
                                    1ull << 1,
                                    1ull << 2,
                                    1ull << 3,
                                    1ull << 4,
                                    1ull << 5,
                                    1ull << 6,
                                    1ull << 7,
                                    1ull << 8,
                                    1ull << 9,
                                    1ull << 10,
                                    1ull << 11,
                                    1ull << 12,
                                    1ull << 13,
                                    1ull << 14,
                                    1ull << 15,
                                    1ull << 16,
                                    1ull << 17,
                                    1ull << 18,
                                    1ull << 19,
                                    1ull << 20,
                                    1ull << 21,
                                    1ull << 22,
                                    1ull << 23,
                                    1ull << 24,
                                    1ull << 25,
                                    1ull << 26,
                                    1ull << 27,
                                    1ull << 28,
                                    1ull << 29,
                                    1ull << 30,
                                    1ull << 31,
                                    1ull << 32,
                                    1ull << 33,
                                    1ull << 34,
                                    1ull << 35,
                                    1ull << 36,
                                    1ull << 37,
                                    1ull << 38,
                                    1ull << 39,
                                    1ull << 40,
                                    1ull << 41,
                                    1ull << 42,
                                    1ull << 43,
                                    1ull << 44,
                                    1ull << 45,
                                    1ull << 46,
                                    1ull << 47,
                                    1ull << 48,
                                    1ull << 49,
                                    1ull << 50,
                                    1ull << 51,
                                    1ull << 52,
                                    1ull << 53,
                                    1ull << 54,
                                    1ull << 55,
                                    1ull << 56,
                                    1ull << 57,
                                    1ull << 58,
                                    1ull << 59,
                                    1ull << 60,
                                    1ull << 61,
                                    1ull << 62,
                                    1ull << 63,
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
    //void UnmakeMove(int color, u64 my_move);
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
    std::vector<u64> GetScore();
    u64 BitboardPopcount(u64 bitboard);
    u64 BitboardPopcount2(u64 b);
    void Reset();
    u64 ValidMoves(int color);
    u64 ValidMovesTwo(int color);
    u64 ValidMovesImproved(int color);
    u64 RandomComputerMove(int color);
    int BitScanForward(u64 bb);

    double EvalBoardSort(int color);

    double EvalBoard(int move_number, int color);
    void ConvertToBinary(u64 bitboard);
    std::vector<u64> GenPossibleMovesIndicators(u64 possible_moves);
    std::vector<u64> GenPossibleMovesIndicatorsImproved(u64 data);
    std::vector<ScoreMove> GenPossibleMoveScorePairsOne(const std::vector<u64>& possible_moves_indicators, int color, int move_number);
    std::vector<ScoreMove> GenPossibleMoveScorePairsTwo(const std::vector<u64>& possible_moves_indicators, int color, int move_number);
    void OrderMovesByEvalDescending(std::vector<ScoreMove>& possible_move_score_pairs);
    
private:
    u64 white_bitboard_ = 0ull;
    u64 black_bitboard_ = 0ull;
};

#endif // BOARD_H_