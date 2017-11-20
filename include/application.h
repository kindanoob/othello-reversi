#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <memory>
#include <stack>


class Display;
class GameState;
class PlayState;
class GameOverState;
class MainMenuState;
class ResourceHolder;
class OptionsState;

enum class AiStrengthLevel {
    Easy,
    Medium,
    Hard,
    VeryHard
};

enum class PieceColor {
    Black,
    White
};

class Application {
public:
    Application();    
    ~Application();
    void Run();    
    void PushState(GameState *state);
    void PopState();
    void ChangeState(GameState *state);
    GameState *state() {
        return states_.top();
    }
    void Destroy();
    void ShowFps();
    void LoadResources();    
    void SetPlayerScore(int player_score) {
        player_score_ = player_score;
    }
    void SetComputerScore(int computer_score) {
        computer_score_ = computer_score;
    }
    int PlayerScore() {
        return player_score_;
    }
    void ResetScores();
    AiStrengthLevel AiLevel() {
        return ai_level_;
    }
public:
    Display *display_ = nullptr;
    int fps_count_ = 0;
    int computer_score_ = 0;    
    int player_score_ = 0;
    ResourceHolder *resource_holder_ = nullptr;
    MainMenuState *main_menu_state_ = nullptr;
    PlayState *play_state_ = nullptr;
    GameOverState *game_over_state_ = nullptr;
    OptionsState *options_state_ = nullptr;
    std::stack<GameState *> states_;
    AiStrengthLevel ai_level_ = AiStrengthLevel::Easy;
    PieceColor player_color_ = PieceColor::White;
    PieceColor computer_color_ = PieceColor::Black;

private:
    
};

#endif // APPLICATION_H_

