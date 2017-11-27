#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <cstring>//is required for memcpy

#include "application.h"
#include "display.h"
#include "play_state.h"
//#include "game_over_state.h"
#include "font_manager.h"
#include "resource_holder.h"
#include "board.h"
#include "button.h"
//#include "util.h"

//std::unordered_map<u64, std::string> bitboard_to_coordinates = gen_bitboard_to_coordinates();

//PassTurn window
const int kPassTurnWindowWidth = 200;
const int kPassTurnWindowHeight = 150;
const sf::VideoMode kPassTurnWindowVideoMode = sf::VideoMode(kPassTurnWindowWidth, kPassTurnWindowHeight);
const sf::Vector2f kPassTurnWindowSize = sf::Vector2f(kPassTurnWindowWidth, kPassTurnWindowHeight);
const int kPassTurnWindowPositionX = 0;
const int kPassTurnWindowPositionY = 0;
const sf::Vector2i kPassTurnWindowPosition = sf::Vector2i(kPassTurnWindowPositionX, kPassTurnWindowPositionY);
const int kPassTurnWindowFontSize = 20;
const std::string kPassTurnWindowTitle = "Pass turn";
const sf::Color kPassTurnWindowFillColor = sf::Color(250, 120, 0);
const std::string kPassTurnString = "No valid moves.\nYou have to pass.";

//PassTurnOk button
const int kPassTurnOkButtonWidth = 40;
const int kPassTurnOkButtonHeight = 30;
const sf::Vector2f kPassTurnOkButtonSize = sf::Vector2f(kPassTurnOkButtonWidth, kPassTurnOkButtonHeight);
//const int kPassTurnOkButtonPositionX = (kPassTurnWindowWidth - kPassTurnOkButtonWidth) / 2;
//const int kPassTurnOkButtonPositionY = (kPassTurnWindowHeight - kPassTurnOkButtonHeight) * 0.75;
const int kPassTurnOkButtonPositionX = (kPassTurnWindowWidth - kPassTurnOkButtonWidth) / 2;
const int kPassTurnOkButtonPositionY = (kPassTurnWindowHeight - kPassTurnOkButtonHeight) * 0.75;
const sf::Vector2f kPassTurnOkButtonPosition = sf::Vector2f(kPassTurnOkButtonPositionX, kPassTurnOkButtonPositionY);
const sf::Color kPassTurnOkButtonColor = sf::Color(250, 250, 250);
const int kPassTurnOkButtonOutlineThickness = 1;
const sf::Color kPassTurnOkButtonOutlineColor = sf::Color(0, 0, 0);
const std::string kPassTurnOkButtonString = "Ok";
const int kPassTurnOkButtonCharacterSize = 17;
const sf::Color kPassTurnOkButtonTextColor = sf::Color(0, 0, 0);
const sf::Color kPassTurnWindowOkButtonOnSelectColor = sf::Color(kMenuButtonOnSelectColor);
//const sf::Color kGameIsOverWindowOkButtonOnSelectColor = sf::Color(250, 120, 10);
//const sf::Color kGameIsOverWindowOkButtonOnSelectColor = sf::Color(255, 255, 255);
const sf::Color kPassTurnWindowOkButtonDefaultColor = sf::Color(255, 255, 255);

//const int kPassTurnOkButtonPositionX = (kGameIsOverWindowWidth - kGameIsOverOkButtonWidth) / 2;
//const int kPassTurnOkButtonPositionY = (kGameIsOverWindowHeight - kGameIsOverOkButtonHeight) * 0.75;
//const sf::Vector2f kPassTurnOkButtonPosition = sf::Vector2f(kGameIsOverOkButtonPositionX, kGameIsOverOkButtonPositionY);

const int kBoardEvalPrecision = 2;
const int kBoardEvalFontSize = kSquareSizeInPixels * 0.35;


const int kSearchDepthMedium = 4;
const int kSearchDepthHard = 10;
const int kShallowSearchDepth = 3;


struct MoveHistoryEntry {
    int move_number;
    int color;
    u64 move;
};

struct Line {
    int cmove;// Number of moves in the line.
    u64 argmove[kMaxMoveNumber];// array containing the moves of the PV (principal variation)
};


PlayState::PlayState(Application *app): GameState(app) {
    //std::cout << "PopcountHashTable: " << std::endl;
    //for (int i = 0; i < 20; ++i) {
        //std::cout << app->PopcountHashTable[i] << std::endl;
    //}
    board_ = new Board();
    //RunPerftTest();
};


PlayState::~PlayState(){}

void PlayState::SetSideToMove(SideToMove side_to_move) {
    side_to_move_ = side_to_move;
}

void PlayState::SwitchSideToMove() {
    if (side_to_move_ == SideToMove::Player) {
        SetSideToMove(SideToMove::Computer);
    } else {
        SetSideToMove(SideToMove::Player);
    }
}

void PlayState::GenerateBitboardToCoordinates() {
    //std::cout << "in GenerateBitboardToCoordinates: " << std::endl;
    for (int i = 0; i < 64; i++) {
        BitboardToCoordinates()[1ULL << i] = kBoardCoordinates[i];
    }
    BitboardToCoordinates()[0] = "pass";
    //for (const auto& p : BitboardToCoordinates()) {
        //std::cout << BitboardToCoordinates()[1ULL << i] = kBoardCoordinates[i];
    //}
}

std::unordered_map<u64, std::string>& PlayState::BitboardToCoordinates() {
    return bitboard_to_coordinates_;
}

void PlayState::SetAiStrengthLevel(Application *app) {
    ai_level_ = app->AiLevel();
}

///is called right after PlayState is pushed onto the stack
void PlayState::Init(Application *app){
    //std::cout << "IN APP INIT" << std::endl;
    if (app->engine_mode_ == EngineMode::Play) {
        SetAiStrengthLevel(app);
        if (ai_level_ == AiStrengthLevel::Medium) {
            time_per_move_ = kTimePerMoveMedium;
        } else if (ai_level_ == AiStrengthLevel::Hard) {
            time_per_move_ = kTimePerMoveHard;
        }
        player_color_ = (app->player_color_ == PieceColor::Black) ? 1 : -1;
        computer_color_ = -player_color_;
        side_to_move_ = (player_color_ == 1) ? SideToMove::Player : SideToMove::Computer;  
        if (side_to_move_ == SideToMove::Player) {
            valid_moves_ = GetBoard()->ValidMoves(player_color_);
            color_to_move_ = player_color_;
        } else {
            valid_moves_ = GetBoard()->ValidMoves(computer_color_);
            color_to_move_ = computer_color_;
        }
    } else if (app->engine_mode_ == EngineMode::Analyze) {
        side_to_move_ = SideToMove::Player;
        color_to_move_ = 1;
    }
    
    CreatePassTurnEntities();
    board_rect_ = new sf::RectangleShape();
    board_rect_->setSize(sf::Vector2f(kBoardWidth * kSquareSizeInPixels, kBoardHeight * kSquareSizeInPixels));
    board_rect_->setFillColor(sf::Color(0, 150, 0));
    board_rect_->setOrigin(-kXMargin, -kYMargin);
    board_rect_->setPosition(0, 0);
    GenerateBitboardToCoordinates();
    std::string num_array[8] = {"1", "2", "3", "4", "5", "6", "7", "8"};
    std::string letter_array[8] = {"A", "B", "C", "D", "E", "F", "G", "H"};
    font_ = app->resource_holder_->fonts_->GetResource(FontName::kUbuntuFont);
    int text_size = kSquareSizeInPixels * 0.4;

    for (int j = 0; j < kBoardHeight; ++j) {
        num_text[j].setFont(font_);
        num_text[j].setCharacterSize(text_size);
        num_text[j].setColor(sf::Color::Black);
        num_text[j].setString(num_array[j]);
        num_text[j].setPosition(kXMargin - text_size * 4 / 5,
                             kYMargin + kSquareSizeInPixels * j + (kSquareSizeInPixels - text_size) / 2);
        letter_text[j].setFont(font_);
        letter_text[j].setCharacterSize(text_size);
        letter_text[j].setColor(sf::Color::Black);
        letter_text[j].setString(letter_array[j]);
        letter_text[j].setPosition(kXMargin + kSquareSizeInPixels * j + (kSquareSizeInPixels - text_size) * 2 / 3,
                                kScreenHeight - kYMargin * 1);        
    }
}

void PlayState::Input(Application *app) {
    if (app->engine_mode_ == EngineMode::Play) {
        if (!GameIsOver()) {
            if (PassTurnHuman()) {
                ShowPassTurnWindow(app);
            }
            //MakeMovePVS(app);
            MakeMoveNegamaxAB(app);
        } else {
            PrintMoveHistory();
            ShowFinalScore(app);        
        }
    } else if (app->engine_mode_ == EngineMode::Analyze) {
        if (!GameIsOver()) {
            if (PassTurnHuman()) {
                ShowPassTurnWindow(app);
            }
            //MakeMovePVS(app);
            MakeMoveNegamaxAB_Analyze(app);
        } else {
            PrintMoveHistory();
            ShowFinalScore(app);        
        }
    }
    
}

u64 PlayState::GetPlayerMove(Application *app) {
    if (side_to_move_ == SideToMove::Computer) {
        return 0ULL;
    }
    sf::Event event;
    while (app->display_->window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            app->display_->Close();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f cursor_pos(event.mouseButton.x, event.mouseButton.y);
                sf::FloatRect containing_rect(BoardRect()->getLocalBounds().left + kXMargin,
                                              BoardRect()->getLocalBounds().top + kYMargin,
                                              BoardRect()->getLocalBounds().width,
                                              BoardRect()->getLocalBounds().height);
                if (containing_rect.contains(cursor_pos)) {
                    int x_coord = (cursor_pos.x - kXMargin) / kSquareSizeInPixels;
                    int y_coord = (cursor_pos.y - kYMargin) / kSquareSizeInPixels;
                    return 1ULL << (63 - x_coord - (y_coord * 8));
                }
            }
        }
    }
    return 0ULL;
}


void PlayState::Input(const sf::Event& event) {}

void PlayState::Input() {}

void PlayState::Update(Application *app) {
    //update_info_text(app);
    //GetBoard()->Update(dt);
    //update_score();
    //std::cout << "play state popped" << std::endl;
}


///is called after the PlayState is popped from the stack
void PlayState::Reset(Application *app) {
    //
}

void PlayState::SetGameIsOver(bool b) {
    game_is_over_ = b;
}

bool PlayState::GameIsOver() {
    return game_is_over_;
}

Board *PlayState::GetBoard() {
    return board_;
}


void PlayState::DrawBoard(Application *app) {        
    app->display_->Draw(*BoardRect());
    DrawBoardGrid(app);
    DrawPieces(app);
    if (app->show_valid_moves_) {
        DrawValidMoves(app);
    }
    if (app->engine_mode_ == EngineMode::Analyze) {
        DrawBoardEvals(app);
    }    
    DrawScores(app);
    //app->display_->DisplayWindow();
}

void PlayState::DrawBoardGrid(Application *app) {
    for (int i = 0; i < kBoardWidth + 1; ++i) {
        sf::Vertex horiz_line[] =
        {
            sf::Vertex(sf::Vector2f(kXMargin, kYMargin + i * kSquareSizeInPixels), sf::Color::Black),
            sf::Vertex(sf::Vector2f(kScreenWidth - kXMargin, kYMargin + i * kSquareSizeInPixels), sf::Color::Black)
        };
        app->display_->window_->draw(horiz_line, 2, sf::Lines);
        sf::Vertex vert_line[] =
        {
            sf::Vertex(sf::Vector2f(kXMargin + i * kSquareSizeInPixels, kYMargin), sf::Color::Black),
            sf::Vertex(sf::Vector2f(kXMargin + i * kSquareSizeInPixels, kScreenHeight - kYMargin), sf::Color::Black)
        };
        app->display_->window_->draw(vert_line, 2, sf::Lines);
    }
    for (int j = 0; j < kBoardHeight; j++) {        
        app->display_->Draw(num_text[j]);
        app->display_->Draw(letter_text[j]);
    }
}

void PlayState::DrawPieces(Application *app) {
    u64 white_copy = GetBoard()->WhiteBitboard();
    for (int row = 0; row < kBoardHeight; ++row) {
        for (int col = 0; col < kBoardWidth; ++col) {
            u64 w = white_copy & 1ULL;
            if (w == 1) {
                sf::CircleShape white_circle;
                white_circle.setRadius(kDiskRadius);
                white_circle.setFillColor(sf::Color::White);
                white_circle.setPosition(kXMargin + (7 - col) * kSquareSizeInPixels + (kSquareSizeInPixels - 2 * kDiskRadius) / 2, 
                                         kYMargin + (7 - row) * kSquareSizeInPixels + (kSquareSizeInPixels - 2 * kDiskRadius) / 2);
                white_circle.setOutlineThickness(2.35);
                white_circle.setOutlineColor(sf::Color::Black);
                app->display_->Draw(white_circle);
            }
            white_copy >>= 1ULL;
        }
    }

    u64 black_copy = GetBoard()->BlackBitboard();    
    for (int row = 0; row < kBoardHeight; row++) {
        for (int col = 0; col < kBoardWidth; col++) {
            u64 b = black_copy & 1ULL;
            if (b == 1) {
                sf::CircleShape black_circle;
                black_circle.setRadius(kDiskRadius);
                black_circle.setFillColor(sf::Color::Black);
                black_circle.setPosition(kXMargin + (7 - col) * kSquareSizeInPixels + (kSquareSizeInPixels - 2 * kDiskRadius) / 2,
                                         kYMargin + (7 - row) * kSquareSizeInPixels + (kSquareSizeInPixels - 2 * kDiskRadius) / 2);
                black_circle.setOutlineThickness(1.0);
                black_circle.setOutlineColor(sf::Color::White);
                app->display_->Draw(black_circle);

            }
            black_copy >>= 1ULL;
        }
    }
}

void PlayState::DrawValidMoves(Application *app) {
    u64 legal_moves = GetBoard()->ValidMoves(color_to_move_);
    for (int row = 0; row < kBoardHeight; row++) {
        for (int col = 0; col < kBoardWidth; col++) {
            u64 legal = legal_moves & 1ULL;
            if (legal == 1) {
                sf::CircleShape legal_move_circle;
                legal_move_circle.setRadius(kDiskRadius / 5);
                legal_move_circle.setFillColor(sf::Color::Yellow);
                legal_move_circle.setPosition(kXMargin + (7 - col) * kSquareSizeInPixels + 1 * (kSquareSizeInPixels / 2 - kDiskRadius / 5),
                                              kYMargin + (7 - row) * kSquareSizeInPixels + 1 * (kSquareSizeInPixels / 2 - kDiskRadius / 5));
                app->display_->Draw(legal_move_circle);
            }
            legal_moves >>= 1ULL;
        }
    }
}

void PlayState::DrawScores(Application *app) {
    sf::Text black_score_text("", font_, kScoreFontSize);
    sf::Text white_score_text("", font_, kScoreFontSize);
    black_score_text.setColor(sf::Color::Black);
    white_score_text.setColor(sf::Color::Black);

    int black_score = GetBlackScore();
    int white_score = GetWhiteScore();

    std::ostringstream score_white_string, score_black_string;
    score_white_string << "White score: " << white_score;
    score_black_string << "Black score: " << black_score;
    black_score_text.setString(score_black_string.str());
    white_score_text.setString(score_white_string.str());
    black_score_text.setPosition(kScreenWidth - kXMargin + kScoreFontSize / 4, kYMargin);
    white_score_text.setPosition(kScreenWidth - kXMargin + kScoreFontSize / 4, kYMargin + kScoreFontSize);
    app->display_->Draw(black_score_text);
    app->display_->Draw(white_score_text);
}

void PlayState::DrawBoardEvals(Application *app) {
    u64 valid_moves = GetBoard()->ValidMoves(color_to_move_);
    for (int j = 0; j < kBoardHeight; j++) {
        for (int i = 0; i < kBoardWidth; i++) {
            u64 legal = valid_moves & 1ULL;
            if (legal == 1) {
                sf::Text board_eval_text("", font_, kBoardEvalFontSize);
                std::ostringstream board_eval_string;
                if (board_eval_memo_.count(1ULL << (j * 8 + i)) > 0) {
                    board_eval_string << std::setprecision(kBoardEvalPrecision) <<
                    board_eval_memo_[1ULL << (j * 8 + i)];
                } else {
                    board_eval_string << "?";
                }
                board_eval_text.setString(board_eval_string.str());
                board_eval_text.setColor(sf::Color::Yellow);
                board_eval_text.setStyle(sf::Text::Style::Bold);
                board_eval_text.setPosition(kXMargin + ( 7 - i ) * kSquareSizeInPixels + (kSquareSizeInPixels - 2 * kBoardEvalFontSize) / 2, 
                    kYMargin + (7 - j) * kSquareSizeInPixels + (kSquareSizeInPixels - 2 * kBoardEvalFontSize) / 2);
                app->display_->Draw(board_eval_text);
            }
            valid_moves >>= 1ull;
        }
    }
}

void PlayState::Draw(Application *app) {
    app->display_->Clear(sf::Color(0, 200, 255));
    //app->display_->Clear(kPlayStateBackgroundColor);
    DrawBoard(app);
    app->display_->DisplayWindow();
}


void PlayState::IncreaseMoveNumber() {
    ++move_number_;
}

void PlayState::DecreaseMoveNumber() {
    --move_number_;
}

int PlayState::TimePerMove() {
    return time_per_move_;
}

void PlayState::SetPassTurnComputer(bool b) {
    pass_turn_computer_ = b;
}

void PlayState::SetPassTurnHuman(bool b) {
    pass_turn_human_ = b;
}

void PrintPrincipalVariation(const Line& principal_variation_curr, int depth) {
    std::cerr << "collected principal variation from depth = " << depth << ": ";
    std::cerr << "[";
    for(int j = 0; j < depth - 1; j++) {
        std::cerr << log(principal_variation_curr.argmove[j]) / log(2) << ", ";
    }
    std::cerr << log(principal_variation_curr.argmove[depth - 1]) / log(2) ;
    std::cerr << "]" << std::endl;
    std::cerr << std::endl;
}
            

void PlayState::AddMoveToHistory(u64 current_move) {
    move_history_.emplace_back(BitboardToCoordinates()[current_move]);
    //std::cout << "added move " << move_history_.back() << std::endl;
}


void PlayState::PrintMoveHistory() {
    std::cout << "Move history: ";
    for (size_t i = 0; i < move_history_.size(); ++i) {
        if (i % 2 == 0) {
            std::cout << std::endl;
        }
        std::cout << (i + 1) << "." << move_history_[i] << " ";
    }
    std::cout << std::endl;
}

void PlayState::MakeMoveNegamaxAB(Application *app) {
    if (GetBoard()->ValidMoves(color_to_move_) == 0ULL) {
        std::cout << "No valid moves. Game is over!" << std::endl; 
        SetGameIsOver(true);
    } else if (side_to_move_ == SideToMove::Player) {
        u64 input_move = GetPlayerMove(app);
        if ((GetBoard()->ValidMoves(color_to_move_) & input_move) != 0ULL) {
            GetBoard()->MakeMoveImproved(color_to_move_, input_move);
            AddMoveToHistory(input_move);
            IncreaseMoveNumber();
            if (GetBoard()->ValidMoves(-color_to_move_) != 0ULL) {
                color_to_move_ *= -1;
                SetSideToMove(SideToMove::Computer);
                SetPassTurnComputer(false);
            } else {
                if (GetBoard()->ValidMoves(color_to_move_) != 0ULL) {
                    SetPassTurnComputer(true);
                    AddMoveToHistory(0);
                } else {
                    SetGameIsOver(true);
                }
            }
        }
    } else if (side_to_move_ == SideToMove::Computer) {
        begin_ = std::chrono::steady_clock::now();
        u64 computer_move = 0ULL;
        if (ai_level_ == AiStrengthLevel::Easy) {
            computer_move = GetBoard()->RandomComputerMove(computer_color_);
        } else if (ai_level_ == AiStrengthLevel::Medium) {
            computer_move = RootNegamaxAB(computer_color_, kSearchDepthMedium, kNegativeInfinity, kInfinity, move_number_, app);
        } else if (ai_level_ == AiStrengthLevel::Hard) {
            computer_move = RootNegamaxAB(computer_color_, kSearchDepthHard, kNegativeInfinity, kInfinity, move_number_, app);
        }

        
        std::cerr << "Computer made move: " << 
            BitboardToCoordinates()[computer_move] << std::endl;
        GetBoard()->MakeMoveImproved(computer_color_, computer_move);

        std::chrono::steady_clock::time_point end_thinking_negamax = std::chrono::steady_clock::now();
        std::cout << "computer thought for " << 
                     std::chrono::duration_cast<std::chrono::milliseconds>(end_thinking_negamax - begin_).count() << 
                     " milliseconds" <<std::endl;
        std::cout << std::endl;

        AddMoveToHistory(computer_move);
        IncreaseMoveNumber();   

        if ((GetBoard()->ValidMoves(-color_to_move_)) != 0ULL) {
            color_to_move_ *= -1;
            SetSideToMove(SideToMove::Player);
            SetPassTurnHuman(false);
        } else {
            if ((GetBoard()->ValidMoves(color_to_move_)) != 0ULL) {
                SetPassTurnHuman(true);
                AddMoveToHistory(0);
            } else {
                SetGameIsOver(true);
            }
        }
    }
}

void PlayState::MakeMoveNegamaxAB_Analyze(Application *app) {
    if (GetBoard()->ValidMoves(color_to_move_) == 0ULL) {
        std::cout << "No valid moves. Game is over!" << std::endl; 
        SetGameIsOver(true);
    } else {
        if (!finished_analysis_) {
            begin_ = std::chrono::steady_clock::now();
            RootNegamaxAB(color_to_move_, kSearchDepthHard, kNegativeInfinity, kInfinity, move_number_, app);
            finished_analysis_ = true;
            std::chrono::steady_clock::time_point end_thinking_negamax = std::chrono::steady_clock::now();
            std::cout << "Evaluation complete, computer thought for " << 
                         std::chrono::duration_cast<std::chrono::milliseconds>(end_thinking_negamax - begin_).count() << 
                         " milliseconds" <<std::endl;
            std::cout << std::endl;
        }
        
        u64 input_move = GetPlayerMove(app);
        if ((GetBoard()->ValidMoves(color_to_move_) & input_move) != 0ULL) {
            GetBoard()->MakeMoveImproved(color_to_move_, input_move);
            AddMoveToHistory(input_move);
            IncreaseMoveNumber();
            finished_analysis_ = false;
            if (GetBoard()->ValidMoves(-color_to_move_) != 0ULL) {
                color_to_move_ *= -1;
                //SwitchSideToMove();
                SetPassTurnHuman(false);
            } else {
                if (GetBoard()->ValidMoves(color_to_move_) != 0ULL) {
                    SetPassTurnHuman(true);
                    AddMoveToHistory(0);
                } else {
                    SetGameIsOver(true);
                }
            }
        }
    }
}

void PlayState::MakeMovePVS(Application *app) {    
    
    if (GetBoard()->ValidMoves(color_to_move_) == 0ULL) {
        std::cout << "No valid moves. Game is over!" << std::endl; 
        SetGameIsOver(true);
    } else if (side_to_move_ == SideToMove::Player) {
        u64 input_move = GetPlayerMove(app);
        if ((GetBoard()->ValidMoves(color_to_move_) & input_move) != 0ULL) {
            GetBoard()->MakeMoveImproved(color_to_move_, input_move);
            AddMoveToHistory(input_move);
            IncreaseMoveNumber();
            if (GetBoard()->ValidMoves(-color_to_move_) != 0ULL) {
                color_to_move_ *= -1;
                SetSideToMove(SideToMove::Computer);
                SetPassTurnComputer(false);
            } else {
                if (GetBoard()->ValidMoves(color_to_move_) != 0ULL) {
                    SetPassTurnComputer(true);
                    AddMoveToHistory(0);
                } else {
                    SetGameIsOver(true);
                }
            }
        }
    } else if (side_to_move_ == SideToMove::Computer) {
        u64 computer_move = 0ULL;
        u64 computer_move_prev = 0ULL;
        begin_ = std::chrono::steady_clock::now();
        timeout_triggered_flag_ = false;

        if (ai_level_ == AiStrengthLevel::Easy) {
            computer_move = GetBoard()->RandomComputerMove(computer_color_);
        } else {
            computer_move = 0ULL;
            Line principal_variation_prev;
            follow_pv_flag_ = false;
            int depth_lower = 1;
            int depth_upper = 0;
            if (ai_level_ == AiStrengthLevel::Medium) {
                depth_upper = kSearchDepthMedium;
            } else {
                depth_upper = kSearchDepthHard;
            }
            for(int depth = depth_lower; depth <= depth_upper; ++depth) {
            //for(int depth = 8; depth <= 8; ++depth) {
                std::cerr << "============================== " << depth << " ==================================" << std::endl;
                if(depth > depth_lower) {
                    //follow_pv_flag_ = true;
                }
                Line principal_variation_curr;
                computer_move_prev = computer_move;
                computer_move = RootSearchPvsPv(computer_color_, depth, kNegativeInfinity, kInfinity, move_number_,
                                                &principal_variation_prev, &principal_variation_curr, app);

                std::chrono::steady_clock::time_point end_thinking_iteration = std::chrono::steady_clock::now();
                think_time_ = std::chrono::duration_cast<std::chrono::milliseconds>(end_thinking_iteration - begin_).count();
                std::cerr << "think time: " << think_time_ << ", depth: " << depth << std::endl;
                if(timeout_triggered_flag_) {
                    std::cerr << "iterative deepening stopped on depth = " << depth << std::endl;
                    std::cerr << "computer_move: " << computer_move << std::endl;
                    std::cerr << "computer_move_prev: " << computer_move << std::endl;
                    break;
                }
                if((think_time_ > TimePerMove()) || (depth > 81 - move_number_)) {
                    timeout_triggered_flag_ = true;
                    std::cerr << "iterative deepening stopped on depth = " << depth << std::endl;
                    std::cerr << "computer_move: " << computer_move << std::endl;
                    std::cerr << "computer_move_prev: " << computer_move << std::endl;
                    break;
                }
                //PrintPrincipalVariation(principal_variation_curr);                
                principal_variation_prev = principal_variation_curr;
            }
        }        
        if(timeout_triggered_flag_) {
            computer_move = computer_move_prev;
        }


        std::cerr << "computer_move: " << BitboardToCoordinates()[computer_move] << std::endl;
        GetBoard()->MakeMoveImproved(computer_color_, computer_move);

        std::chrono::steady_clock::time_point end_thinking_pvs = std::chrono::steady_clock::now();
        std::cerr << "computer thought for " << 
                     std::chrono::duration_cast<std::chrono::milliseconds>(end_thinking_pvs - begin_).count() << 
                     " milliseconds" <<std::endl;
        std::cerr << std::endl;

        AddMoveToHistory(computer_move);

        IncreaseMoveNumber();   

        if ((GetBoard()->ValidMoves(-color_to_move_)) != 0ULL) {
            color_to_move_ *= -1;
            SetSideToMove(SideToMove::Player);
            SetPassTurnHuman(false);
        } else {
            if ((GetBoard()->ValidMoves(color_to_move_)) != 0ULL) {
                SetPassTurnHuman(true);
                AddMoveToHistory(0);
            } else {
                SetGameIsOver(true);
            }
        }
    }
}


double PlayState::PvsPv(int color, int depth, int root_depth, double alpha, double beta, int move_number,
                 Line *pline_prev, Line *pline_curr, Application *app) {    
    std::cerr << "entered PvsPv with color = " << color << 
        ", depth = " << depth << ", root_depth: " << root_depth << 
        ", alpha = " << alpha << ", beta = " << beta << std::endl;
    Line line;
    if (depth == 0) {
        std::cout << "depth 0, return " << GetBoard()->EvalBoard(move_number, color, app->PopcountHashTable) << std::endl;
        pline_curr->cmove = 0;
        return GetBoard()->EvalBoard(move_number, color, app->PopcountHashTable);
    }
    if(depth == 1) {
        follow_pv_flag_ = false;
    }
    u64 possible_moves = GetBoard()->ValidMoves(color);
    if ((possible_moves == 0ULL) && (GetBoard()->ValidMoves(-color) == 0ULL)) {
        pline_curr->cmove = 0;
        return GetBoard()->EvalBoard(move_number, color, app->PopcountHashTable);
    } else {
        if (possible_moves == 0ULL) {
            return -PvsPv(-color, depth, root_depth, -beta, -alpha, move_number, pline_prev, pline_curr, app);//pline_curr instead of line??????
        }
        std::vector<u64> possible_moves_indicators = GetBoard()->GenPossibleMovesIndicators(possible_moves);
        int possible_moves_indicators_size = possible_moves_indicators.size();
        std::vector<ScoreMove> possible_move_score_pairs = GetBoard()->GenPossibleMoveScorePairsOne(possible_moves_indicators, color, move_number, app->PopcountHashTable);
        std::sort(possible_move_score_pairs.begin(), possible_move_score_pairs.end(), 
            [](const ScoreMove& a, const ScoreMove& b) {
                return a.score > b.score;
            });

        u64 current_move = 0ULL;
        u64 tiles_to_flip = 0ULL;
        bool found_pv = false;
        double score = 0.0;
        int pv_index = root_depth - depth;
        if (follow_pv_flag_) {            
            if (possible_move_score_pairs[0].move != (pline_prev->argmove[pv_index])) {
                int pos = 0;//index of the PV move in the valid moves array
                for (int i = 0; i < possible_moves_indicators_size; i++) {
                    if (possible_move_score_pairs[i].move == (pline_prev->argmove[pv_index])) {
                        pos = i;
                        break;
                    }
                }
                ScoreMove temp_score_move = possible_move_score_pairs[0];
                possible_move_score_pairs[0].move = pline_prev->argmove[pv_index];
                possible_move_score_pairs[0].score = 0;//doesn't matter, just for completeness, this value won't be used
                possible_move_score_pairs[pos] = temp_score_move;
            }            
        }
        for (int i = 0; i < possible_moves_indicators_size; i++) {
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            think_time_ = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin_).count();
            if ((think_time_ > TimePerMove())) {
                timeout_triggered_flag_ = true;
                break;
            }
            if (i > 0) {
                follow_pv_flag_ = false;
            }

            current_move = possible_move_score_pairs[i].move;
            //std::cout << "made move " << log(current_move) / log(2) << std::endl;
            std::cout << "made move " << BitboardToCoordinates()[current_move] << std::endl;
            
            tiles_to_flip = GetBoard()->MakeMoveImproved(color, current_move);
            if (found_pv) {
                score = -PvsPv(-color, depth - 1, root_depth, -alpha - 1, -alpha, move_number + 1, pline_prev, &line, app);
                if (score > alpha && score < beta) {
                    score = -PvsPv(-color, depth - 1, root_depth, -beta, -alpha, move_number + 1, pline_prev, &line, app);
                }
            } else {
                score = -PvsPv(-color, depth - 1, root_depth, -beta, -alpha, move_number + 1, pline_prev, &line, app);
            }
            //std::cout << "score: " << score << std::endl;
            std::cout << "i: " << i << ", return with score " << score <<
            " for move: " << BitboardToCoordinates()[current_move] << std::endl;
            GetBoard()->UnmakeMove(color, current_move, tiles_to_flip);
            if (score >= beta) {
                return beta;
            }
            if (score > alpha) {
                alpha = score;
                found_pv = true;
                pline_curr->argmove[0] = current_move;
                std::memcpy(pline_curr->argmove + 1, line.argmove, line.cmove * sizeof(u64));
                pline_curr->cmove = line.cmove + 1;                
            }
        }
        std::cout << "returned " << alpha << std::endl;
        return alpha;
    }
}


u64 PlayState::RootSearchPvsPv(int color, int depth, double alpha_original, double beta_original, int move_number,
                             Line *pline_prev, Line *pline_curr, Application *app) {
    std::cerr << "AT ROOT entered RootSearchPvsPv with color = " << color << 
        ", depth = " << depth << ", alpha = " << alpha_original << ", beta = " << beta_original << std::endl;    
    //GetBoard()->PrintBoardToConsole();
    Line line;
    u64 possible_moves = GetBoard()->ValidMoves(color);

    std::vector<u64> possible_moves_indicators = GetBoard()->GenPossibleMovesIndicators(possible_moves);
    int possible_moves_indicators_size = possible_moves_indicators.size();
    std::vector<ScoreMove> possible_move_score_pairs = 
        GetBoard()->GenPossibleMoveScorePairsOne(possible_moves_indicators, color, move_number, app->PopcountHashTable);
    std::sort(possible_move_score_pairs.begin(), possible_move_score_pairs.end(), 
        [](const ScoreMove& a, const ScoreMove& b) {
            return a.score > b.score;
        });


    if (follow_pv_flag_) {
        if (possible_move_score_pairs[0].move != (pline_prev->argmove[0])) {
            int pos = 0;//index of the PV move in the valid moves array
            for (int i = 0; i < possible_moves_indicators_size; i++) {
                if (possible_move_score_pairs[i].move == (pline_prev->argmove[0])) {
                    pos = i;
                    break;
                }
            }
            ScoreMove temp_score_move = possible_move_score_pairs[0];
            possible_move_score_pairs[0].move = pline_prev->argmove[0];
            possible_move_score_pairs[0].score = 0;//doesn't matter, just for completeness, this value won't be used
            possible_move_score_pairs[pos] = temp_score_move;
        }
    }
    
    bool found_pv = false;    
    u64 best_move = possible_move_score_pairs[0].move;
    double best_score = -kInfinity;

    for (int i = 0; i < possible_moves_indicators_size; i++) {
        double alpha = alpha_original;
        double beta = beta_original;
        double score = 0.0;
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        //std::cerr << "spent " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " milliseconds thinking on depth " << i << std::endl;
        think_time_ = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin_).count();
        //std::cerr << "think time: " << think_time_<< ", depth: " << i << ", 42 - move_number =  " << (42 - m_move_number) << std::endl;
        if((think_time_> TimePerMove())) {
            timeout_triggered_flag_ = true;
            //std::cerr << "iterative deepening stopped on depth = " << i << std::endl;
            break;
        }
        if (i > 0) {
            follow_pv_flag_= false;
        }
        u64 current_move = possible_move_score_pairs[i].move;        
        u64 tiles_to_flip = GetBoard()->MakeMoveImproved(color, current_move);
        std::cout << "AT ROOT made move: " << BitboardToCoordinates()[current_move] << std::endl;
        if (found_pv) {
            std::cout << "PV" << std::endl;
            score = -PvsPv(-color, depth - 1, depth, -alpha - 1, -alpha, move_number + 1, pline_prev, &line, app);
            std::cout << "PV score: " << score << std::endl;
            if (score > alpha && score < beta) {
                std::cout << "recalculate" << std::endl;
                score = -PvsPv(-color, depth - 1, depth, -beta, -alpha, move_number + 1, pline_prev, &line, app);
            }
        } else {
            std::cout << "not PV" << std::endl;
            score = -PvsPv(-color, depth - 1, depth, -beta, -alpha, move_number + 1, pline_prev, &line, app);
        }
        std::cout << "AT ROOT i: " << i << ", returned with score " << score <<
        "for move: " << BitboardToCoordinates()[current_move] << std::endl;
        //std::cout << "score: " << score << std::endl;
        GetBoard()->UnmakeMove(color, current_move, tiles_to_flip);
        //if(score >= beta) {
            //break;
        //}
        if (score > best_score) {
            //alpha = score;
            best_score = score;
            best_move = current_move;
            found_pv = true;
            pline_curr->argmove[0] = current_move;
            std::memcpy(pline_curr->argmove + 1, line.argmove, line.cmove * sizeof(u64));
            pline_curr->cmove = line.cmove + 1;
        }
    }
    std::cout << "AT ROOT best move: " << BitboardToCoordinates()[best_move] <<
    ", best_score: " << best_score << std::endl;
    return best_move;
}


u64 PlayState::RootNegamaxAB(int color, int depth, double alpha, double beta, int move_number,
    Application *app) {
    //std::cout << "AT ROOT entered RootNegamaxAB with color " << color << ", depth: " << depth << 
    //", move_number: " << move_number << ", alpha: " << alpha << ", beta: " << beta << std::endl;
    u64 possible_moves = GetBoard()->ValidMoves(color);
    std::vector<u64> possible_moves_indicators = GetBoard()->GenPossibleMovesIndicators(possible_moves);
    //std::vector<ScoreMove> possible_move_score_pairs = 
        //GetBoard()->GenPossibleMoveScorePairsOne(possible_moves_indicators, color, move_number);
    //GetBoard()->OrderMoveScorePairsByEvalDescending(possible_move_score_pairs);
    //OrderMovesByShallowSearch(possible_moves_indicators, color, move_number);
    std::vector<ScoreMove> possible_move_score_pairs = 
        OrderMovesByShallowSearch(possible_moves_indicators, color, move_number, app);
    std::cout << "AT ROOT, moves after sort: " << std::endl;
    for (const auto& p : possible_move_score_pairs) {
        std::cout << BitboardToCoordinates()[p.move] << " : " << p.score << std::endl;
    }
    double best_score = -kInfinity;
    u64 best_move = possible_move_score_pairs[0].move;
    for (size_t i = 0; i < possible_moves_indicators.size(); i++) {
        u64 current_move = possible_move_score_pairs[i].move;
        u64 tiles_to_flip = GetBoard()->MakeMoveImproved(color, current_move);
        //std::cout << "AT ROOT made move " << BitboardToCoordinates()[current_move] << std::endl;
        double current_score = -NegamaxAB(-color, depth - 1, depth - 1, -beta, -alpha, move_number + 1, app);
        //std::cout << "AT ROOT returned with score: " << current_score << " for move: " << 
            //BitboardToCoordinates()[current_move] << std::endl;
        board_eval_memo_[current_move] = current_score;
        GetBoard()->UnmakeMove(color, current_move, tiles_to_flip);
        if (current_score > best_score) {
            best_score = current_score;
            best_move = current_move;
        }
    }
    return best_move;
}


//**************************************************
//this function creates a vector of [score, move] pairs and sorts this vector in ascending order
//according to the scores of NegamaxAB performed at depth kShallowSearchDepth
//**************************************************
std::vector<ScoreMove> PlayState::OrderMovesByShallowSearch(const std::vector<u64>& valid_moves_indicators, 
    int color, int move_number, Application *app) {
    //std::cout << "in OrderMovesByShallowSearch " << std::endl;
    std::vector<ScoreMove> move_score_pairs;
    for (u64 current_move : valid_moves_indicators) {        
        u64 tiles_to_flip = GetBoard()->MakeMoveImproved(color, current_move);
        double current_score = -NegamaxAB(-color, kShallowSearchDepth, 
            kShallowSearchDepth, -kInfinity, kInfinity, move_number, app);
        GetBoard()->UnmakeMove(color, current_move, tiles_to_flip);
        //std::cout << "current_move: " << BitboardToCoordinates()[current_move] << 
        //", current_score: " << current_score << std::endl;
        move_score_pairs.push_back(ScoreMove(current_move, current_score));
    }
    //std::cout << "Q" << std::endl;
    //for (const auto& p : move_score_pairs) {
        //std::cout << p.move << " : " << p.score << std::endl;
    //}
    std::sort(begin(move_score_pairs), end(move_score_pairs),
        [](const ScoreMove& a, const ScoreMove& b) {
            return a.score > b.score;
        });
    return move_score_pairs;
}

double PlayState::NegamaxAB(int color, int depth, int max_depth, double alpha, double beta, int move_number,
    Application *app) {
    if (depth == 0) {        
        double score = GetBoard()->EvalBoard(move_number, color, app->PopcountHashTable);
        return score;
    }
    u64 possible_moves = GetBoard()->ValidMoves(color);
    if (possible_moves == 0ULL) {
        if (GetBoard()->ValidMoves(-color) == 0ULL) {
            return GetBoard()->EvalBoard(move_number, color, app->PopcountHashTable);
        } else {
            return -NegamaxAB(-color, depth, max_depth, -beta, -alpha, move_number, app);
        }        
    } else {
        double best_score = -kInfinity;
        std::vector<u64> possible_moves_indicators = GetBoard()->GenPossibleMovesIndicators(possible_moves);
        std::vector<ScoreMove> possible_move_score_pairs = 
            GetBoard()->GenPossibleMoveScorePairsOne(possible_moves_indicators, color, move_number, app->PopcountHashTable);
        GetBoard()->OrderMoveScorePairsByEvalDescending(possible_move_score_pairs);
        
        
        for (size_t i = 0; i < possible_moves_indicators.size(); i++) {
            u64 current_move = possible_move_score_pairs[i].move;
            u64 tiles_to_flip = GetBoard()->MakeMoveImproved(color, current_move);
            double score = -NegamaxAB(-color, depth - 1, max_depth, -beta, -alpha, move_number + 1, app);
            GetBoard()->UnmakeMove(color, current_move, tiles_to_flip);
            best_score = std::max(score, best_score);
            alpha = std::max(alpha, score);
            if (alpha >= beta) {
                break;
            }            
        }
        return best_score;        
    }
}

int PlayState::GetBlackScore() {
    return GetBoard()->BlackScore();
}

int PlayState::GetWhiteScore() {
    return GetBoard()->WhiteScore();
}

void PlayState::ShowFinalScore(Application *app) {
    //std::cout << "in ShowFinalScore " << std::endl;
    sf::Text final_score_text;
    final_score_text.setFont(font_);
    final_score_text.setCharacterSize(kGameIsOverWindowFinalScoreTextCharacterSize);
    final_score_text.setColor(kGameIsOverWindowFinalScoreTextColor);
    final_score_text.setPosition(kGameIsOverWindowFinalScoreTextPosition);
    std::string final_score_string = "Score " + std::to_string(GetBlackScore()) + " : " + std::to_string(GetWhiteScore());

    if (GetBlackScore() > GetWhiteScore()) {
        final_score_text.setString("Black wins.\n" + final_score_string);
    } else if (GetBlackScore() < GetWhiteScore()) {
        final_score_text.setString("White wins.\n" + final_score_string);
    } else {
        final_score_text.setString("Draw.\n" + final_score_string);
    }        

    final_score_window_ = new sf::RenderWindow(kGameIsOverWindowVideoMode, kGameIsOverWindowTitle);
    auto main_window_pos = app->display_->window_->getPosition();
    final_score_window_->setPosition(main_window_pos + 
        sf::Vector2i((kScreenWidth - kGameIsOverOkButtonWidth) / 2, (kScreenHeight - kGameIsOverOkButtonHeight) / 2));
    InitGameIsOverOkButton();

    while (final_score_window_->isOpen()) {
        sf::Event event;
        while (final_score_window_->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                final_score_window_->close();                
                app->PopState();
            }
        }
        auto mouse_position = final_score_window_->mapPixelToCoords(sf::Mouse::getPosition(*final_score_window_));
        if (GameIsOverOkButton()->RectShape().getGlobalBounds().contains(mouse_position)) {
            GameIsOverOkButton()->SetRectShapeFillColor(kGameIsOverWindowOkButtonOnSelectColor);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                final_score_window_->close();
                app->PopState();
            }
        } else {
            GameIsOverOkButton()->SetRectShapeFillColor(kGameIsOverWindowOkButtonDefaultColor);
        }
        final_score_window_->clear(kGameIsOverWindowFillColor);
        final_score_window_->draw(final_score_text);
        final_score_window_->draw(game_is_over_ok_button_->RectShape());
        final_score_window_->draw(game_is_over_ok_button_->ButtonText());
        final_score_window_->display();
    }
    //std::cout << "out ShowFinalScore " << std::endl;
}

void PlayState::InitGameIsOverOkButton() {
    game_is_over_ok_button_ = new Button(
                                         kGameIsOverOkButtonSize,
                                         kGameIsOverOkButtonPosition,
                                         kGameIsOverOkButtonColor,
                                         kGameIsOverOkButtonOutlineThickness,
                                         kGameIsOverOkButtonOutlineColor,
                                         font_,
                                         kGameIsOverOkButtonString,
                                         kGameIsOverOkButtonCharacterSize,
                                         kGameIsOverOkButtonTextColor
                                        );
}

Button *PlayState::GameIsOverOkButton() {
    return game_is_over_ok_button_;
}


void PlayState::CreatePassTurnOkButton() {
    pass_turn_ok_button_ = new Button(
                                     kPassTurnOkButtonSize,
                                     kPassTurnOkButtonPosition,
                                     kPassTurnOkButtonColor,
                                     kPassTurnOkButtonOutlineThickness,
                                     kPassTurnOkButtonOutlineColor,
                                     font_,
                                     kPassTurnOkButtonString,
                                     kPassTurnOkButtonCharacterSize,
                                     kPassTurnOkButtonTextColor
                                    );
}

void PlayState::CreatePassTurnText() {
    pass_turn_text_.setFont(font_);
    pass_turn_text_.setCharacterSize(20);
    pass_turn_text_.setColor(sf::Color(0, 0, 0));
    pass_turn_text_.setString(kPassTurnString);        
}

Button * PlayState::PassTurnOkButton() {
    return pass_turn_ok_button_;
}

void PlayState::ShowPassTurnWindow(Application *app) {
    pass_turn_window_ = new sf::RenderWindow(kPassTurnWindowVideoMode, kPassTurnWindowTitle);
    auto main_window_pos = app->display_->window_->getPosition();
    pass_turn_window_->setPosition(main_window_pos + sf::Vector2i(kPassTurnWindowWidth * 2, kPassTurnWindowHeight * 2));

    CreatePassTurnOkButton();
    pass_turn_text_.setPosition(kGameIsOverWindowFinalScoreTextPosition);
    while (pass_turn_window_->isOpen()) {
        sf::Event event;
        while (pass_turn_window_->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                pass_turn_window_->close();                
            }
        }
        auto mouse_position = pass_turn_window_->mapPixelToCoords(sf::Mouse::getPosition(*pass_turn_window_));
        if (PassTurnOkButton()->RectShape().getGlobalBounds().contains(mouse_position)) {
            PassTurnOkButton()->SetRectShapeFillColor(kGameIsOverWindowOkButtonOnSelectColor);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                pass_turn_window_->close();
            }
        } else {
            PassTurnOkButton()->SetRectShapeFillColor(kGameIsOverWindowOkButtonDefaultColor);
        }
        pass_turn_window_->clear(kGameIsOverWindowFillColor);
        pass_turn_window_->draw(pass_turn_text_);
        pass_turn_window_->draw(PassTurnOkButton()->RectShape());
        pass_turn_window_->draw(PassTurnOkButton()->ButtonText());
        pass_turn_window_->display();
    }
}

void PlayState::CreatePassTurnEntities() {
    //CreatePassTurnOkButton();
    CreatePassTurnText();
}

bool PlayState::PassTurnHuman() {
    return pass_turn_human_;
}

u64 PlayState::Perft(Board board, int depth, int color, Application *app) {
    //std::cout << "entered perft with depth " << depth << std::endl;
    if (depth == 0) {
        return 1;
    }
    u64 nodes = 0;
    u64 valid_moves = board.ValidMoves(color);
    int valid_size = Util::BitboardPopcountHashTable(valid_moves, app->PopcountHashTable);
    if (valid_size == 0) {
        if (board.ValidMoves(-color) == 0ull) {
            return 1;
        }
        nodes += Perft(board, depth - 1, -color, app);
    } else {
        std::vector<u64> move_indicators = board.GenPossibleMovesIndicators(valid_moves);
        for (size_t i = 0; i < move_indicators.size(); i++) {
            Board dupe_board(board);
            dupe_board.MakeMoveImproved(color, move_indicators[i]);
            nodes += Perft(dupe_board, depth - 1, -color, app);
        }
    }
    return nodes;
}

void PlayState::RunPerftTest(Application *app) {
    sf::Clock m_clock;
    sf::Time m_time = m_clock.restart();
    int perft_depth;
    std::cout << "enter max perft depth: ";
    std::cin >> perft_depth;
    std::cout << std::endl;
    for (int i = 0; i < perft_depth; i++) {
        u64 perft_number = Perft(*(GetBoard()), i, 1, app);
        m_time = m_clock.getElapsedTime();
        m_clock.restart();
        std::cout << "perft number for depth " << i << " is equal to " << perft_number << " and it took " << m_time.asSeconds() << " seconds to compute" <<  std::endl;
    }
}

