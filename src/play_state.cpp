#include <iostream>
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



struct MoveHistoryEntry {
    int move_number;
    int color;
    u64 move;
};

struct Line {
    int cmove;// Number of moves in the line.
    u64 argmove[kMaxMoveNumber];// The line.
};


PlayState::PlayState(Application *app): GameState(app) {
    board_ = new Board();
};


PlayState::~PlayState(){}

void PlayState::SetSideToMove(SideToMove side_to_move) {
    side_to_move_ = side_to_move;
}

void PlayState::GenerateBitboardToCoordinates() {
    //std::unordered_map<u64, std::string> bitboard_to_coordinates;
    for (int i = 0; i < 64; i++) {
        BitboardToCoordinates()[pow(2, i)] = kBoardCoordinates[i];
    }
    //return bitboard_to_coordinates;
}

std::unordered_map<u64, std::string> PlayState::BitboardToCoordinates() {
    return bitboard_to_coordinates_;
}

void PlayState::SetAiStrengthLevel(Application *app) {
    ai_level_ = app->AiLevel();
}

///is called right after PlayState is pushed onto the stack
void PlayState::Init(Application *app){
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
    if (!GameIsOver()) {
        //MakeMove(app);
        if (PassTurnHuman()) {
            ShowPassTurnWindow(app);
        } else {
            //MakeMoveImproved(app);
        }
        MakeMoveImproved(app);
    } else {
        ShowFinalScore(app);
    }
}

u64 PlayState::GetPlayerMove(Application *app) {
    //std::cout << "in PlayState::GetPlayerMove" << std::endl;
    if (side_to_move_ == SideToMove::Computer) {
        return 0ull;
    }
    sf::Event event;
    while (app->display_->window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            app->display_->Close();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            //std::cout << "MouseButtonPressed" << std::endl;
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f cursor_pos(event.mouseButton.x, event.mouseButton.y);
                sf::FloatRect containing_rect(BoardRect()->getLocalBounds().left + kXMargin,
                                              BoardRect()->getLocalBounds().top + kYMargin,
                                              BoardRect()->getLocalBounds().width,
                                              BoardRect()->getLocalBounds().height);
                if (containing_rect.contains(cursor_pos)) {
                    int x_coord = (cursor_pos.x - kXMargin) / kSquareSizeInPixels;
                    int y_coord = (cursor_pos.y - kYMargin) / kSquareSizeInPixels;
                    return 0b0000000000000000000000000000000000000000000000000000000000000001ull << (63 - x_coord - (y_coord * 8));
                }
            }
        }
    }
    //std::cout << "This should never happen." << std::endl;
    return 0ull;
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

Board* PlayState::GetBoard() {
    return board_;
}


void PlayState::DrawBoard(Application *app) {
    int black_score = GetBlackScore();
    int white_score = GetWhiteScore();
    app->display_->Clear(sf::Color(0, 200, 255));
    app->display_->Draw(*BoardRect());


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


    u64 white_copy = GetBoard()->WhiteBitboard();
    for (int row = 0; row < kBoardHeight; ++row) {
        for (int col = 0; col < kBoardWidth; ++col) {
            u64 w = white_copy & 1ull;
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
            white_copy >>= 1ull;
        }
    }

    u64 black_copy = GetBoard()->BlackBitboard();    
    for (int row = 0; row < kBoardHeight; row++) {
        for (int col = 0; col < kBoardWidth; col++) {
            u64 b = black_copy & 1ull;
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
            black_copy >>= 1ull;
        }
    }

    u64 legal_moves = GetBoard()->ValidMoves(color_to_move_);
    for (int row = 0; row < kBoardHeight; row++) {
        for (int col = 0; col < kBoardWidth; col++) {
            u64 legal = legal_moves & 1ull;
            if (legal == 1) {
                sf::CircleShape legal_move_circle;
                legal_move_circle.setRadius(kDiskRadius / 5);
                legal_move_circle.setFillColor(sf::Color::Yellow);
                //legal_move_circle.setOrigin(-kSquareSizeInPixels / 4 - kDiskRadius / 5, -kSquareSizeInPixels / 4 - kDiskRadius / 5);
                legal_move_circle.setPosition(kXMargin + (7 - col) * kSquareSizeInPixels + 1 * (kSquareSizeInPixels / 2 - kDiskRadius / 5),
                                              kYMargin + (7 - row) * kSquareSizeInPixels + 1 * (kSquareSizeInPixels / 2 - kDiskRadius / 5));
                app->display_->Draw(legal_move_circle);
            }
            legal_moves >>= 1ull;
        }
    }

    
    for (int j = 0; j < kBoardHeight; j++) {        
        app->display_->Draw(num_text[j]);
        app->display_->Draw(letter_text[j]);
    }

    sf::Text black_score_text("", font_, kScoreFontSize);
    sf::Text white_score_text("", font_, kScoreFontSize);
    black_score_text.setColor(sf::Color::Black);
    white_score_text.setColor(sf::Color::Black);

    std::ostringstream score_white_string, score_black_string;
    //score_white_string << this->get_score()[0];
    //score_black_string << this->get_score()[1];
    score_white_string << "White score: " << white_score;
    score_black_string << "Black score: " << black_score;
    black_score_text.setString(score_black_string.str());
    white_score_text.setString(score_white_string.str());
    //score_text.setPosition(350, 446);//setposition for 640x480 video mode
    black_score_text.setPosition(kScreenWidth - kXMargin + kScoreFontSize / 4, kYMargin);
    white_score_text.setPosition(kScreenWidth - kXMargin + kScoreFontSize / 4, kYMargin + kScoreFontSize);
    app->display_->Draw(black_score_text);
    app->display_->Draw(white_score_text);
    app->display_->DisplayWindow();
}

void PlayState::Draw(Application *app) {
    app->display_->Clear(kPlayStateBackgroundColor);
    DrawBoard(app);
}

void PlayState::MakeMove(Application *app) {
    //std::cout << "in MakeMove, board: " << GetBoard() << ", game_is_over_: " << game_is_over_ << ", color_to_move_: " <<
    //color_to_move_ << ", side_to_move_: " << side_to_move_ << ", player_color_: " << player_color_ << 
    //", computer_color_: " << computer_color_ << std::endl;

    if (GetBoard()->ValidMoves(color_to_move_) == 0ull) {
        std::cout << "No valid moves. Game is over!" << std::endl; 
        SetGameIsOver(true); 
    } else if (side_to_move_ == SideToMove::Player) {
        u64 input_move = GetPlayerMove(app);
        if ((GetBoard()->ValidMoves(color_to_move_) & input_move) != 0ull) {
            GetBoard()->MakeMove(color_to_move_, input_move);
            ++move_number_;
            if (GetBoard()->ValidMoves(-color_to_move_) != 0ull) {
                color_to_move_ *= -1;
                SetSideToMove(SideToMove::Computer);
                //side_to_move_ *= -1;
            }
        }
    } else if (side_to_move_ == SideToMove::Computer) {
        sf::Clock clock;
        sf::Time time;
        if (GetBoard()->ValidMoves(computer_color_) == 0ull) {
            std::cout << "No valid moves. Game is over!" << std::endl; 
            SetGameIsOver(true);
        }
        u64 computer_move = NegamaxAB(GetBoard(), computer_color_, kMinimaxDepthMedium, kNegativeInfinity, kInfinity, move_number_).move;

        GetBoard()->MakeMove(computer_color_, computer_move);
        //AddMoveToHistory(move_number_, computer_color_, computer_move);
        ++move_number_;
        if ((GetBoard()->ValidMoves(player_color_)) != 0ull) {
            color_to_move_ *= -1;
            SetSideToMove(SideToMove::Player);
        }
    time = clock.getElapsedTime();
    //std::cout << "computer thought for " << m_time.asSeconds() << " seconds on move " << move_number_ << std::endl;
    std::cout << time.asSeconds() << " seconds on move " << move_number_ << std::endl;
    }
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

void PlayState::MakeMoveImproved(Application *app) {    
    if (GetBoard()->ValidMoves(color_to_move_) == 0ull) {
        std::cout << "No valid moves. Game is over!" << std::endl; 
        SetGameIsOver(true);
    } else if (side_to_move_ == SideToMove::Player) {
        u64 input_move = GetPlayerMove(app);
        if ((GetBoard()->ValidMoves(color_to_move_) & input_move) != 0ull) {
            //u64 tiles_to_flip = GetBoard()->MakeMoveImproved(color_to_move_, input_move);
            GetBoard()->MakeMoveImproved(color_to_move_, input_move);
            IncreaseMoveNumber();
            if (GetBoard()->ValidMoves(-color_to_move_) != 0ull) {
                color_to_move_ *= -1;
                SetSideToMove(SideToMove::Computer);
                SetPassTurnComputer(false);
            } else {
                SetPassTurnComputer(true);
            }
        }
    } else if (side_to_move_ == SideToMove::Computer) {
        u64 computer_move;
        u64 computer_move_prev;
        //std::chrono::steady_clock::time_point 
        begin_ = std::chrono::steady_clock::now();
        timeout_triggered_flag_ = false;
        if (GetBoard()->ValidMoves(computer_color_) == 0ull) {
            std::cout << "No valid moves. Game is over!" << std::endl;
            SetGameIsOver(true);
        }

        if (ai_level_ == AiStrengthLevel::Easy) {
            computer_move = GetBoard()->RandomComputerMove(computer_color_);
        } else {
            computer_move = 0;
            Line principal_variation_prev;
            follow_pv_flag_ = false;
            int depth_lower = 1;
            int depth_upper = 100;
            for(int depth = depth_lower; depth < depth_upper; ++depth) {
                std::cerr << "============================== " << depth << " ==================================" << std::endl;
                if(depth > depth_lower) {
                    follow_pv_flag_ = true;
                }
                Line principal_variation_curr;
                computer_move_prev = computer_move;
                computer_move = RootSearchPvsPv(computer_color_, depth, kNegativeInfinity, kInfinity, move_number_,
                                                &principal_variation_prev, &principal_variation_curr);

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
                //std::cerr << "collected principal variation from depth = " << i << ": ";
                //std::cerr << "[";
                //for(int j = 0; j < i - 1; j++)
                //{
                    //std::cerr << log(principal_variation_curr.argmove[j]) / log(2) << ", ";
                //}
                //std::cerr << log(principal_variation_curr.argmove[i - 1]) / log(2) ;
                //std::cerr << "]" << std::endl;
                //std::cerr << std::endl;
                principal_variation_prev = principal_variation_curr;
            }
        }
        if(timeout_triggered_flag_) {
            computer_move = computer_move_prev;
        }


        std::cerr << "computer_move: " << log(computer_move) / log(2) << std::endl;
        //u64 tiles_to_flip = GetBoard()->MakeMoveImproved(computer_color_, computer_move);
        GetBoard()->MakeMoveImproved(computer_color_, computer_move);

        std::chrono::steady_clock::time_point end_thinking_pvs = std::chrono::steady_clock::now();
        std::cerr << "computer thought for " << 
                     std::chrono::duration_cast<std::chrono::milliseconds>(end_thinking_pvs - begin_).count() << 
                     " milliseconds" <<std::endl;
        std::cerr << std::endl;

        IncreaseMoveNumber();        
        if ((GetBoard()->ValidMoves(player_color_)) != 0ull) {
            color_to_move_ *= -1;
            SetSideToMove(SideToMove::Player);
            SetPassTurnHuman(false);
        } else {
            SetPassTurnHuman(true);
        }
    }
}

double PlayState::PvsPv(int color, int depth, int root_depth, double alpha, double beta, int move_number,
                 Line *pline_prev, Line *pline_curr) {    
    Line line;
    if (depth == 0) {
        pline_curr->cmove = 0;
        return GetBoard()->EvalBoard(move_number, color);
    }
    if(depth == 1) {
        follow_pv_flag_ = false;
    }
    u64 possible_moves = GetBoard()->ValidMoves(color);
    if ((possible_moves == 0ull) && (GetBoard()->ValidMoves(-color) == 0ull)) {
        pline_curr->cmove = 0;
        return GetBoard()->EvalBoard(move_number, color);
    } else {
        if (possible_moves == 0ull) {
            return -PvsPv(-color, depth, root_depth, -beta, -alpha, move_number, pline_prev, pline_curr);//pline_curr instead of line??????
        }
        std::vector<u64> possible_moves_indicators = GetBoard()->GenPossibleMovesIndicators(possible_moves);
        int possible_moves_indicators_size = possible_moves_indicators.size();
        std::vector<ScoreMove> possible_move_score_pairs = GetBoard()->GenPossibleMoveScorePairsOne(possible_moves_indicators, color, move_number);
        std::sort(possible_move_score_pairs.begin(), possible_move_score_pairs.end(), 
            [](const ScoreMove& a, const ScoreMove& b) {
                return a.score > b.score;
            });

        u64 current_move = 0ull;
        u64 tiles_to_flip = 0ull;
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
            tiles_to_flip = GetBoard()->MakeMoveImproved(color, current_move);
            if (found_pv) {
                score = -PvsPv(-color, depth - 1, root_depth, -alpha - 1, -alpha, move_number + 1, pline_prev, &line);
                if (score > alpha && score < beta) {
                    score = -PvsPv(-color, depth - 1, root_depth, -beta, -alpha, move_number + 1, pline_prev, &line);
                }
            } else {
                score = -PvsPv(-color, depth - 1, root_depth, -beta, -alpha, move_number + 1, pline_prev, &line);
            }
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
        return alpha;
    }
}


u64 PlayState::RootSearchPvsPv(int color, int depth, double alpha, double beta, int move_number,
                             Line *pline_prev, Line *pline_curr) {
    //std::cerr << "AT ROOT entered negamax with color = " << color << ", depth = " << depth << ", alpha = " << alpha << ", beta = " << beta << std::endl;
    Line line;
    u64 possible_moves = GetBoard()->ValidMoves(color);

    std::vector<u64> possible_moves_indicators = GetBoard()->GenPossibleMovesIndicators(possible_moves);
    int possible_moves_indicators_size = possible_moves_indicators.size();
    std::vector<ScoreMove> possible_move_score_pairs = GetBoard()->GenPossibleMoveScorePairsOne(possible_moves_indicators, color, move_number);
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
    
    u64 current_move = 0ull;
    u64 tiles_to_flip = 0ull;
    bool found_pv = false;
    double score = 0.0;
    u64 best_move = 0ull;

    for (int i = 0; i < possible_moves_indicators_size; i++) {
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
        current_move = possible_move_score_pairs[i].move;
        tiles_to_flip = GetBoard()->MakeMoveImproved(color, current_move);
        if (found_pv) {
            score = -PvsPv(-color, depth - 1, depth, -alpha - 1, -alpha, move_number + 1, pline_prev, &line);
            if (score > alpha && score < beta) {
                score = -PvsPv(-color, depth - 1, depth, -beta, -alpha, move_number + 1, pline_prev, &line);
            }
        } else {
            score = -PvsPv(-color, depth - 1, depth, -beta, -alpha, move_number + 1, pline_prev, &line);
        }
        GetBoard()->UnmakeMove(color, current_move, tiles_to_flip);
        if(score >= beta) {
            break;
        }
        if (score > alpha) {
            alpha = score;
            best_move = current_move;
            found_pv = true;
            pline_curr->argmove[0] = current_move;
            std::memcpy(pline_curr->argmove + 1, line.argmove, line.cmove * sizeof(u64));
            pline_curr->cmove = line.cmove + 1;
        }
    }
    return best_move;
}


ScoreMove PlayState::NegamaxAB(Board *board, int color, int depth, double alpha, double beta, int move_number) {
    ScoreMove best_score_move;
    ScoreMove new_score_move;
    if (depth == 0) {
        best_score_move.score = GetBoard()->EvalBoard(move_number, color);
        return best_score_move;
    }
    u64 possible_moves = GetBoard()->ValidMoves(color);
    if ((possible_moves == 0ull) && (GetBoard()->ValidMoves(-color) == 0ull)) {
        best_score_move.score = GetBoard()->EvalBoard(move_number, color);
        return best_score_move;
    } else {
        if (possible_moves == 0ull) {
            double best_score = kNegativeInfinity;
            u64 best_move;
            //u64 current_move;
            //u64 new_move;
            double new_score;
            new_score_move = NegamaxAB(board, -color, depth, -beta, -alpha, move_number);
            new_score = -new_score_move.score;
            u64 new_move = new_score_move.move;
            //undo_move();
            //if (depth != kMinimaxDepth) {delete_last_move_from_history();}
            if (new_score > best_score) {
                best_score = new_score;
                best_move = new_move;
            }
            if (new_score > alpha) {
                alpha = new_score;
            }
            if (alpha >= beta) {
                best_score = alpha;
                best_move = new_move;
                best_score_move.score = best_score;
                best_score_move.move = best_move;
            }
            best_score_move.score = best_score;
            best_score_move.move = best_move;
        }
        std::vector<u64> possible_moves_indicators = GetBoard()->GenPossibleMovesIndicators(possible_moves);
        std::vector<ScoreMove> possible_move_score_pairs = 
            GetBoard()->GenPossibleMoveScorePairsOne(possible_moves_indicators, color, move_number);
        GetBoard()->OrderMovesByEvalDescending(possible_move_score_pairs);
        double best_score = -1000000.0;
        u64 best_move;
        u64 current_move;
        //u64 new_move;
        double new_score;
        int possible_moves_indicators_size = possible_moves_indicators.size();
        for (int i = 0; i < possible_moves_indicators_size; i++) {
            current_move = possible_move_score_pairs[i].move;
            Board *dupe_board = new Board(*GetBoard());
            dupe_board->MakeMove(color, current_move);
            //AddMoveToHistory(color, current_move);
            new_score_move = NegamaxAB(dupe_board, -color, depth - 1, -beta, -alpha, move_number + 1);
            new_score = -new_score_move.score;//*****************************************************************************changed sign********************************************
            //u64 new_move = new_score_move.move;
            //undo_move();
            //if (depth != kMinimaxDepth) {delete_last_move_from_history();}
            if (new_score > best_score) {
                best_score = new_score;
                best_move = current_move;
            }
            if (new_score > alpha) {
                alpha = new_score;
            }
            if (alpha >= beta) {
                best_score = alpha;
                best_move = current_move;
                best_score_move.score = best_score;
                best_score_move.move = best_move;
                break;
            }
            best_score_move.score = best_score;
            best_score_move.move = best_move;
        }
        return best_score_move;
    }
}

int PlayState::GetBlackScore() {
    return GetBoard()->BlackScore();
}

int PlayState::GetWhiteScore() {
    return GetBoard()->WhiteScore();
}

void PlayState::ShowFinalScore(Application *app) {            
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
}

//void PlayState::InitGameIsOverWindow() {
    //
//}

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

Button * PlayState::GameIsOverOkButton() {
    return game_is_over_ok_button_;
}

//void PlayState::CreatePassTurnWindow() {
    //pass_turn_window_ = new sf::RenderWindow(kGameIsOverWindowVideoMode, "");
    //auto main_window_pos = app->display_->window_->getPosition();
    //pass_turn_window_->setPosition(main_window_pos + 
        //sf::Vector2i((kScreenWidth - kGameIsOverOkButtonWidth) / 2, (kScreenHeight - kGameIsOverOkButtonHeight) / 2));
//}

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
    //pass_turn_window_->setPosition(kPassTurnWindowPosition);
    //pass_turn_window_->setPosition(main_window_pos + 
        //sf::Vector2i((kScreenWidth - kGameIsOverOkButtonWidth) / 2, (kScreenHeight - kGameIsOverOkButtonHeight) / 2));
    pass_turn_window_->setPosition(main_window_pos + sf::Vector2i(kPassTurnWindowWidth * 2, kPassTurnWindowHeight * 2));

    //std::string pass_turn_string = ((SideToPass() == SideToMove::Player) ? "Black" : "White") + " has to pass.";
    //pass_turn_text_.setPosition(pass_turn_window_->getPosition() + sf::Vector2f(20, 20));
    CreatePassTurnOkButton();
    pass_turn_text_.setPosition(kGameIsOverWindowFinalScoreTextPosition);
    while (pass_turn_window_->isOpen()) {
        sf::Event event;
        while (pass_turn_window_->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                pass_turn_window_->close();                
                //app->PopState();
            }
        }
        auto mouse_position = pass_turn_window_->mapPixelToCoords(sf::Mouse::getPosition(*pass_turn_window_));
        if (PassTurnOkButton()->RectShape().getGlobalBounds().contains(mouse_position)) {
            PassTurnOkButton()->SetRectShapeFillColor(kGameIsOverWindowOkButtonOnSelectColor);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                pass_turn_window_->close();
                //app->PopState();
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