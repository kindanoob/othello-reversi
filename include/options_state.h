#ifndef OPTIONS_STATE_H_
#define OPTIONS_STATE_H_

#include <SFML/Graphics.hpp>

#include "game_state.h"



class Application;
class Button;


class OptionsState: public GameState {
public:
    OptionsState(Application *app);
    ~OptionsState();
    virtual void Input(Application *app);
    virtual void Input(const sf::Event& event);
    virtual void Input();
    virtual void Update(Application *app);
    virtual void Draw(Application *app);
    virtual void Init(Application *app);

    void CreateEntities(Application *app);
    void CreateBackButton(Application *app);
    void DrawEntities();
    sf::RenderWindow *Window(){
        return window_;
    }
    Button *BackButton(){
        return back_button_;
    }
    void CreateChooseAiLevelText(Application *app);
    void CreateAiLevelButtons(Application *app);
    void CreateFont(Application *app);
    void CreateAiColorText(Application *app);
    void CreateAiColorButtons(Application *app);
    void CreateShowValidMovesColorText(Application *app);
    void CreateShowValidMovesButtons(Application *app);
private:
    sf::RenderWindow *window_ = nullptr;
    Button *back_button_ = nullptr;
    Button *easy_ai_button_ = nullptr;
    Button *medium_ai_button_ = nullptr;
    Button *hard_ai_button_ = nullptr;
    Button *ai_color_white_button_ = nullptr;
    Button *ai_color_black_button_ = nullptr;
    Button *analyze_button_ = nullptr;
    Button *show_valid_moves_button_yes_ = nullptr;
    Button *show_valid_moves_button_no_ = nullptr;
    sf::Text choose_ai_color_text_;
    sf::Text choose_ai_level_text_;
    sf::Text show_valid_moves_text_;
    sf::Font font_;
};

#endif // OPTIONS_STATE_H_

