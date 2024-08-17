#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <cstring>//is required for memcpy

#include "application.h"
#include "display.h"
#include "play_state.h"
#include "font_manager.h"
#include "resource_holder.h"
#include "board.h"
#include "button.h"


const int kBoardEvalPrecision = 2;
const int kBoardEvalFontSize = kSquareSizeInPixels * 0.35;

void PlayState::DrawBoard(Application *app) {        
    app->display_->Draw(*BoardRect());
    DrawBoardGrid(app);
    DrawPieces(app);
    if (app->engine_mode_ == EngineMode::Analyze) {
        DrawBoardEvals(app);
    } else if (app->show_valid_moves_) {
        DrawValidMoves(app);
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
    black_score_text.setFillColor(sf::Color::Black);
    white_score_text.setFillColor(sf::Color::Black);

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
                board_eval_text.setFillColor(sf::Color::Yellow);
                board_eval_text.setStyle(sf::Text::Style::Bold);
                board_eval_text.setOrigin(sf::Vector2f(-17, -7));
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
