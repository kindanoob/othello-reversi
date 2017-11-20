
#include <iostream>
#include <sstream>

#include "application.h"
#include "display.h"
#include "main_menu_state.h"
#include "play_state.h"
#include "button.h"
#include "options_state.h"



MainMenuState::MainMenuState(Application *app): GameState(app) {
    Init();
}


MainMenuState::~MainMenuState(){
}


void MainMenuState::CreateMenuFont(){
    if (!font_.loadFromFile(kFontsPath + "ubuntu.ttf")) {
        std::cout << "Error. Create_menu_font failed to load." << std::endl;
    } else {
        std::cout << "Create_menu_font loaded successfully." << std::endl;
    }
}


void MainMenuState::CreateMenuText(const sf::Font& font, const unsigned int text_size) {
    new_game_button_ = new Button(kMenuButtonSize,
                                  kMenuNewGameButtonPosition,
                                  kMenuButtonColor,
                                  kMenuButtonOutlineThickness,
                                  kMenuButtonOutlineColor,
                                  font_,
                                  "New game",
                                  kMenuCharacterSize,
                                  kMenuTextColor);


    options_button_ = new Button(kMenuButtonSize,
                                  kMenuOptionsButtonPosition,
                                  kMenuButtonColor,
                                  kMenuButtonOutlineThickness,
                                  kMenuButtonOutlineColor,
                                  font_,
                                  "Options",
                                  kMenuCharacterSize,
                                  kMenuTextColor);

    exit_button_ = new Button(kMenuButtonSize,
                                  kMenuExitButtonPosition,
                                  kMenuButtonColor,
                                  kMenuButtonOutlineThickness,
                                  kMenuButtonOutlineColor,
                                  font_,
                                  "Exit",
                                  kMenuCharacterSize,
                                  kMenuTextColor);
}


void MainMenuState::Input(Application *app) {
    sf::Event event;
    while (app->display_->window_->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            app->display_->Close();
        }
    }
    NewGameButton()->RectShape().setFillColor(kMenuButtonDefaultColor);
    ExitButton()->SetRectShapeFillColor(kMenuButtonDefaultColor);
    OptionsButton()->SetRectShapeFillColor(kMenuButtonDefaultColor);

    auto mouse_position = app->display_->window_->mapPixelToCoords(sf::Mouse::getPosition(*(app->display_->window_)));
    if (NewGameButton()->RectShape().getGlobalBounds().contains(mouse_position)) {
        NewGameButton()->SetRectShapeFillColor(kMenuButtonOnSelectColor);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            PlayState *new_play_state = new PlayState(app);
            app->PushState(new_play_state);
            new_play_state->Init(app);
        }
    }
    else if (OptionsButton()->RectShape().getGlobalBounds().contains(mouse_position)) {
        OptionsButton()->SetRectShapeFillColor(kMenuButtonOnSelectColor);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            OptionsState* new_options_state = new OptionsState(app);
            app->PushState(new_options_state);
        }
    }
    else if (ExitButton()->RectShape().getGlobalBounds().contains(mouse_position)) {
        ExitButton()->SetRectShapeFillColor(kMenuButtonOnSelectColor);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            app->display_->Close();
        }
    }
}



void MainMenuState::Input(const sf::Event& event) {}

void MainMenuState::Input() {}

void MainMenuState::Update(Application *app) {}


void MainMenuState::Draw(Application *app) {
    app->display_->Clear(kMainMenuStateBackgroundColor);
    app->display_->Draw(NewGameButton()->RectShape());
    app->display_->Draw(NewGameButton()->ButtonText());

    app->display_->Draw(OptionsButton()->RectShape());
    app->display_->Draw(OptionsButton()->ButtonText());

    app->display_->Draw(ExitButton()->RectShape());
    app->display_->Draw(ExitButton()->ButtonText());
}

void MainMenuState::Init() {
    CreateMenuFont();
    CreateMenuText(font_, kMenuTextSize);
    CreateMenuRect();
}


void MainMenuState::CreateMenuRect() {
    menu_rect_.setSize(sf::Vector2f(200, 75));
    menu_rect_.setFillColor(sf::Color::White);
    menu_rect_.setOutlineThickness(5);
    menu_rect_.setOutlineColor(sf::Color::Black);
    menu_rect_.setOrigin(menu_rect_.getSize().x / 2, menu_rect_.getSize().y / 2);
    menu_rect_.setPosition(kScreenWidth / 2, kScreenHeight / 2);
}