#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_


#include <SFML/Graphics.hpp>

#include "game_state.h"

class Application;
class Button;


class MainMenuState: public GameState{
public:
    MainMenuState(Application *app);
    ~MainMenuState();    
    void CreateMenuText(const sf::Font& font, const unsigned int text_size);
    void CreateMenuFont();
    void CreateMenuRect();
    void CreateRect();
    void Input(const sf::Event& event);
    void Input();
    void Input(Application *app);
    void Update(Application *app);
    void Draw(Application *app);
    void InitEntities();
    void Init();
    Button *NewGameButton(){
        return new_game_button_;
    }
    Button *OptionsButton(){
        return options_button_;
    }
    Button *ExitButton(){
        return exit_button_;
    }
    sf::Font font_;
    sf::Text menu_text_;
    sf::Text new_game_text_;
    sf::RectangleShape menu_rect_;
    sf::RectangleShape new_game_button_rect_;
private:
    Button *new_game_button_ = nullptr;
    Button *options_button_ = nullptr;
    Button *exit_button_ = nullptr;
};

#endif // MAIN_MENU_H_