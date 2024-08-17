#include <iostream>



#include "application.h"
#include "display.h"
#include "options_state.h"
#include "play_state.h"
#include "button.h"
#include "resource_holder.h"
#include "font_manager.h"



OptionsState::OptionsState(Application *app): GameState(app) {
    window_ = new sf::RenderWindow(sf::VideoMode(kOptionsWindowWidth, kOptionsWindowHeight), "Options");
    window_->setPosition(app->display_->window_->getPosition() + sf::Vector2i(250, 250));
    Init(app);
}

OptionsState::~OptionsState() {}

void OptionsState::Input(Application *app) {
    while (window_->isOpen()) {
        sf::Event e;
        while (window_->pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window_->close();
                app->PopState();
            }
        }
        BackButton()->SetRectShapeFillColor(sf::Color(255, 255, 255));
        easy_ai_button_->SetRectShapeFillColor(sf::Color(255, 255, 255));
        medium_ai_button_->SetRectShapeFillColor(sf::Color(255, 255, 255));
        hard_ai_button_->SetRectShapeFillColor(sf::Color(255, 255, 255));
        analyze_button_->SetRectShapeFillColor(sf::Color(255, 255, 255));
        ai_color_black_button_->SetRectShapeFillColor(sf::Color(255, 255, 255));
        ai_color_white_button_->SetRectShapeFillColor(sf::Color(255, 255, 255));
        analyze_button_->SetRectShapeFillColor(sf::Color(255, 255, 255));
        show_valid_moves_button_yes_->SetRectShapeFillColor(sf::Color(255, 255, 255));
        show_valid_moves_button_no_->SetRectShapeFillColor(sf::Color(255, 255, 255));
        if (app->ai_level_ == AiStrengthLevel::Easy) {
            easy_ai_button_->SetRectShapeFillColor(kMenuButtonOnSelectColor);
        } else if (app->ai_level_ == AiStrengthLevel::Medium) {
            medium_ai_button_->SetRectShapeFillColor(kMenuButtonOnSelectColor);
        } else if (app->ai_level_ == AiStrengthLevel::Hard) {
            hard_ai_button_->SetRectShapeFillColor(kMenuButtonOnSelectColor);
        } 
        if (app->engine_mode_ == EngineMode::Play) {
            if (app->computer_color_ == PieceColor::Black) {
                ai_color_black_button_->SetRectShapeFillColor(kMenuButtonOnSelectColor);
            } else if (app->computer_color_ == PieceColor::White) {
                ai_color_white_button_->SetRectShapeFillColor(kMenuButtonOnSelectColor);
            }
        } else if (app->engine_mode_ == EngineMode::Analyze) {
            analyze_button_->SetRectShapeFillColor(kMenuButtonOnSelectColor);
        }
        if (app->show_valid_moves_) {
            show_valid_moves_button_yes_->SetRectShapeFillColor(kMenuButtonOnSelectColor);
        } else {
            show_valid_moves_button_no_->SetRectShapeFillColor(kMenuButtonOnSelectColor);
        }
        auto mouse_position = window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));
        if(BackButton()->RectShape().getGlobalBounds().contains(mouse_position)){
            BackButton()->SetRectShapeFillColor(kMenuButtonOnSelectColor);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                window_->close();
                app->PopState();
            }
        } else if(easy_ai_button_->RectShape().getGlobalBounds().contains(mouse_position)){            
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                easy_ai_button_->SetRectShapeFillColor(kMenuButtonOnSelectColor);
                app->ai_level_ = AiStrengthLevel::Easy;
            }
        } else if(medium_ai_button_->RectShape().getGlobalBounds().contains(mouse_position)){            
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                medium_ai_button_->SetRectShapeFillColor(kMenuButtonOnSelectColor);
                app->ai_level_ = AiStrengthLevel::Medium;
            }
        } else if(hard_ai_button_->RectShape().getGlobalBounds().contains(mouse_position)){            
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                hard_ai_button_->SetRectShapeFillColor(kMenuButtonOnSelectColor);
                app->ai_level_ = AiStrengthLevel::Hard;
            }
        } else if(ai_color_black_button_->RectShape().getGlobalBounds().contains(mouse_position)){            
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                ai_color_black_button_->SetRectShapeFillColor(kMenuButtonOnSelectColor);
                app->engine_mode_ = EngineMode::Play;
                app->computer_color_ = PieceColor::Black;
                app->player_color_ = PieceColor::White;
            }
        } else if(ai_color_white_button_->RectShape().getGlobalBounds().contains(mouse_position)){            
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                ai_color_white_button_->SetRectShapeFillColor(kMenuButtonOnSelectColor);
                app->engine_mode_ = EngineMode::Play;
                app->computer_color_ = PieceColor::White;
                app->player_color_ = PieceColor::Black;
            }
        } else if(analyze_button_->RectShape().getGlobalBounds().contains(mouse_position)){            
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                analyze_button_->SetRectShapeFillColor(kMenuButtonOnSelectColor);
                //app->computer_color_ = PieceColor::White;
                //app->player_color_ = PieceColor::Black;
                app->engine_mode_ = EngineMode::Analyze;
            }
        } else if(show_valid_moves_button_yes_->RectShape().getGlobalBounds().contains(mouse_position)){            
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                show_valid_moves_button_yes_->SetRectShapeFillColor(kMenuButtonOnSelectColor);
                app->show_valid_moves_ = true;
            }
        } else if(show_valid_moves_button_no_->RectShape().getGlobalBounds().contains(mouse_position)){            
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                show_valid_moves_button_no_->SetRectShapeFillColor(kMenuButtonOnSelectColor);
                app->show_valid_moves_ = false;
            }
        }
        window_->clear(sf::Color(255, 255, 255));        
        window_->draw(back_button_->RectShape());
        window_->draw(back_button_->ButtonText());
        window_->draw(choose_ai_level_text_);
        window_->draw(easy_ai_button_->RectShape());
        window_->draw(easy_ai_button_->ButtonText());
        window_->draw(medium_ai_button_->RectShape());
        window_->draw(medium_ai_button_->ButtonText());
        window_->draw(hard_ai_button_->RectShape());
        window_->draw(hard_ai_button_->ButtonText());
        window_->draw(analyze_button_->RectShape());
        window_->draw(analyze_button_->ButtonText());
        window_->draw(choose_ai_color_text_);
        window_->draw(ai_color_black_button_->RectShape());
        window_->draw(ai_color_black_button_->ButtonText());
        window_->draw(ai_color_white_button_->RectShape());
        window_->draw(ai_color_white_button_->ButtonText());
        window_->draw(show_valid_moves_text_);
        window_->draw(show_valid_moves_button_yes_->RectShape());
        window_->draw(show_valid_moves_button_yes_->ButtonText());
        window_->draw(show_valid_moves_button_no_->RectShape());
        window_->draw(show_valid_moves_button_no_->ButtonText());
        window_->display();
    }
}

void OptionsState::Input(const sf::Event& event) {}

void OptionsState::Input() {}
   

void OptionsState::Update(Application *app) {}

void OptionsState::Draw(Application *app) {}

void OptionsState::CreateFont(Application *app) {
    //font_ = app->resource_holder_->fonts_->GetResource(FontName::kUbuntuFont);
    font_ = app->resource_holder_->fonts_->GetResource(FontName::kArialFont);
    //font_ = app->resource_holder_->fonts_->GetResource(FontName::kCyrillicFont);
}

void OptionsState::Init(Application *app) {
    CreateFont(app);
    CreateBackButton(app);
    CreateChooseAiLevelText(app);
    CreateAiLevelButtons(app);
    CreateAiColorText(app);
    CreateAiColorButtons(app);
    CreateShowValidMovesColorText(app);
    CreateShowValidMovesButtons(app);
}

void OptionsState::CreateBackButton(Application *app) {
    back_button_ = new Button(sf::Vector2f(80, 40),
                              sf::Vector2f(400, 325),
                              sf::Color(255, 255, 255),
                              3,
                              sf::Color(0, 0, 0),
                              font_,
                              "ok",
                              20,
                              sf::Color(0, 0, 0)
                              );
}

void OptionsState::CreateChooseAiLevelText(Application *app) {
    choose_ai_level_text_.setFont(font_);
    choose_ai_level_text_.setCharacterSize(20);
    choose_ai_level_text_.setString("Choose AI level: ");
    choose_ai_level_text_.setPosition(sf::Vector2f(60, 60));
    choose_ai_level_text_.setFillColor(sf::Color(0, 0, 0));
}

void OptionsState::CreateAiLevelButtons(Application *app) {
    std::cout << "Ai_buttons created" << std::endl;
    easy_ai_button_ = new Button(sf::Vector2f(80, 40),
                              sf::Vector2f(225, 50),
                              sf::Color(255, 255, 255),
                              3,
                              sf::Color(0, 0, 0),
                              font_,
                              "Easy",
                              20,
                              sf::Color(0, 0, 0)
                              );
    medium_ai_button_ = new Button(sf::Vector2f(80, 40),
                              sf::Vector2f(325, 50),
                              sf::Color(255, 255, 255),
                              3,
                              sf::Color(0, 0, 0),
                              font_,
                              "Medium",
                              20,
                              sf::Color(0, 0, 0)
                              );
    hard_ai_button_ = new Button(sf::Vector2f(80, 40),
                              sf::Vector2f(425, 50),
                              sf::Color(255, 255, 255),
                              3,
                              sf::Color(0, 0, 0),
                              font_,
                              "Hard",
                              20,
                              sf::Color(0, 0, 0)
                              );
}

void OptionsState::CreateAiColorText(Application *app) {
    choose_ai_color_text_.setFont(font_);
    choose_ai_color_text_.setCharacterSize(20);
    choose_ai_color_text_.setString("Foompello plays: ");
    choose_ai_color_text_.setPosition(sf::Vector2f(60, 160));
    choose_ai_color_text_.setFillColor(sf::Color(0, 0, 0));
}

void OptionsState::CreateAiColorButtons(Application *app) {
    ai_color_black_button_ = new Button(sf::Vector2f(80, 40),
                              sf::Vector2f(225, 150),
                              sf::Color(255, 255, 255),
                              3,
                              sf::Color(0, 0, 0),
                              font_,
                              "Black",
                              20,
                              sf::Color(0, 0, 0)
                              );
    ai_color_white_button_ = new Button(sf::Vector2f(80, 40),
                              sf::Vector2f(325, 150),
                              sf::Color(255, 255, 255),
                              3,
                              sf::Color(0, 0, 0),
                              font_,
                              "White",
                              20,
                              sf::Color(0, 0, 0)
                              );    
    analyze_button_ = new Button(sf::Vector2f(80, 40),
                              sf::Vector2f(425, 150),
                              sf::Color(255, 255, 255),
                              3,
                              sf::Color(0, 0, 0),
                              font_,
                              "Analyzes",
                              20,
                              sf::Color(0, 0, 0)
                              );    
}

void OptionsState::CreateShowValidMovesColorText(Application *app) {
    show_valid_moves_text_.setFont(font_);
    show_valid_moves_text_.setCharacterSize(20);
    show_valid_moves_text_.setString("Show valid moves: ");
    show_valid_moves_text_.setPosition(sf::Vector2f(60, 260));
    show_valid_moves_text_.setFillColor(sf::Color(0, 0, 0));
}

void OptionsState::CreateShowValidMovesButtons(Application *app) {
    show_valid_moves_button_yes_ = new Button(sf::Vector2f(80, 40),
                              sf::Vector2f(225, 250),
                              sf::Color(255, 255, 255),
                              3,
                              sf::Color(0, 0, 0),
                              font_,
                              "Yes",
                              20,
                              sf::Color(0, 0, 0)
                              );
    show_valid_moves_button_no_ = new Button(sf::Vector2f(80, 40),
                              sf::Vector2f(325, 250),
                              sf::Color(255, 255, 255),
                              3,
                              sf::Color(0, 0, 0),
                              font_,
                              "No",
                              20,
                              sf::Color(0, 0, 0)
                              );    
}
