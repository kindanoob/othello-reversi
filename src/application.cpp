#include <iostream>
#include <memory>
#include  <utility>

#include "application.h"
#include "display.h"
#include "game_state.h"
#include "play_state.h"
#include "game_over_state.h"
#include "main_menu_state.h"
#include "resource_holder.h"
#include "options_state.h"
#include "util.h"
#include "config.h"



Application::Application(): player_score_(0) {
    GeneratePopcountHashTable();
    resource_holder_ = new ResourceHolder();
    display_ = new Display();
    main_menu_state_ = new MainMenuState(this);
    PushState(main_menu_state_);
}

Application::~Application() {}

void Application::Run() {
    sf::Clock clock;
    while (display_->IsOpen()) {
        u64 time = clock.getElapsedTime().asMicroseconds();
        if (time % 100 == 0) {
            fps_count_ = 1000000.0 / time;
        }
        clock.restart();
        states_.top()->Input(this);
        states_.top()->Update(this);
        states_.top()->Draw(this);
        display_->DisplayWindow();
    }
    display_->Destroy();
}


void Application::PushState(GameState *state) {
    states_.push(state);
};


void Application::PopState() {
    states_.pop();
};


void Application::ChangeState(GameState *state) {
    states_.pop();
    states_.push(state);
};


void Application::Destroy() {}


void Application::ShowFps() {
    std::cout << "FPS: " << fps_count_ << std::endl;
}


void Application::ResetScores() {
    SetPlayerScore(0);
    SetComputerScore(0);
}

void Application::GeneratePopcountHashTable() {
    for (u64 i = 0; i < 65536; ++i) {
        PopcountHashTable[i] = Util::BitboardPopcount(i);
    }
}

