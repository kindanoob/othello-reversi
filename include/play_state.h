#ifndef PLAY_STATE_H_
#define PLAY_STATE_H_

#include <list>
#include <string>
#include <unordered_map>
#include <chrono>

#include <SFML/Graphics.hpp>


#include "game_state.h"
#include "util.h"
//#include "board.h"



class Application;
class Board;
struct ScoreMove;
struct Line;
class Button;

enum class SideToMove {
    Player,
    Computer
};

class PlayState: public GameState {
public:    
    PlayState(Application *app);
    ~PlayState();    
    void Init();    
    void CreateBoard(Application *app);
    void Input();
    void Input(const sf::Event& event);
    void Input(Application *app);
    u64 GetPlayerMove(Application *app);
    void Update(Application *app);
    void Draw(Application *app);
    void DrawBoard(Application *app);
    void Reset(Application *app);    
    void Init(Application *app);
    Board *GetBoard();
    bool GameIsOver();
    void SetGameIsOver(bool b);
    sf::RectangleShape *BoardRect() {
        return board_rect_;
    }
    int GetBlackScore();
    int GetWhiteScore();
    void MakeMoveNegamaxAB(Application *app);
    void MakeMoveNegamaxAB_Analyze(Application *app);
    void MakeMovePVS(Application *app);
    void GenerateBitboardToCoordinates();
    std::unordered_map<u64, std::string>& BitboardToCoordinates();
    u64 RootNegamaxAB(int color, int depth, double alpha, double beta, int move_number, Application *app);
    double NegamaxAB(int color, int depth, int max_depth, double alpha, double beta, int move_number, Application *app);    
    std::vector<ScoreMove> OrderMovesByShallowSearch(const std::vector<u64>& valid_moves_indicators, 
        int color, int move_number, Application *app);
    void ShowFinalScore(Application *app);
    void InitGameIsOverOkButton();
    Button *GameIsOverOkButton();
    u64 RootSearchPvsPv(int color, int depth, double alpha, double beta, int move_number,
                             Line *line_prev, Line *pline_curr, Application *app);
    double PvsPv(int color, int depth, int root_depth, double alpha, double beta, int move_number,
                 Line *pline_prev, Line *pline_curr, Application *app);
    void SetSideToMove(SideToMove side_to_move);
    void SwitchSideToMove();
    void IncreaseMoveNumber();
    void DecreaseMoveNumber();
    int TimePerMove();
    void SetAiStrengthLevel(Application *app);
    void SetPassTurnHuman(bool b);
    void SetPassTurnComputer(bool b);
    void ShowPassTurnWindow(Application *app);
    Button *PassTurnOkButton();
    void CreatePassTurnText();
    void CreatePassTurnOkButton();
    void CreatePassTurnEntities();
    bool PassTurnHuman();
    void PrintPrincipalVariation(const Line& principal_variation_curr, int depth);
    void AddMoveToHistory(u64 current_move);
    void PrintMoveHistory();
    void RunPerftTest(Application *app);
    u64 Perft(Board board, int depth, int color, Application *app);
    void DrawPieces(Application *app);
    void DrawValidMoves(Application *app);
    void DrawBoardGrid(Application *app);
    void DrawScores(Application *app);
    void DrawBoardEvals(Application *app);

    
private:
    Board *board_ = nullptr;
    sf::Clock play_state_clock_;
    bool game_is_over_ = false;
    int color_to_move_ = 1;
    SideToMove side_to_move_ = SideToMove::Player;
    int player_color_ = 1;
    int computer_color_ = -1;
    u64 valid_moves_ = 0ull;
    sf::RectangleShape *board_rect_ = nullptr;
    u64 player_move_bitboard_ = 0ull;
    std::unordered_map<u64, std::string> bitboard_to_coordinates_;
    int move_number_ = 1;
    sf::Font font_;
    sf::Text num_text[kBoardHeight];
    sf::Text letter_text[kBoardWidth];
    sf::RenderWindow *final_score_window_ = nullptr;
    Button *game_is_over_ok_button_ = nullptr;
    bool timeout_triggered_flag_ = false;
    bool follow_pv_flag_ = true;
    int think_time_ = 0;;
    std::chrono::steady_clock::time_point begin_;
    AiStrengthLevel ai_level_ = AiStrengthLevel::Easy;
    int time_per_move_ = 0;//in milliseconds
    sf::RenderWindow *pass_turn_window_ = nullptr;
    bool pass_turn_human_ = false;
    bool pass_turn_computer_ = false;
    Button *pass_turn_ok_button_ = nullptr;    
    sf::Text pass_turn_text_;
    std::vector<std::string> move_history_;
    std::unordered_map<u64, double> board_eval_memo_; //is used for board evaluation scores
    bool finished_analysis_ = false; // is used in analyze mode
    
};

#endif // PLAY_STATE_H_
