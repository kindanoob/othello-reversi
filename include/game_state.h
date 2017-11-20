#ifndef GAME_STATE_H_
#define GAME_STATE_H_


#include <SFML/Graphics.hpp>


class Application;


class GameState {
public:    
    GameState(Application *app);
    virtual void Input(Application *app) = 0;
    virtual void Input(const sf::Event& event) = 0;
    virtual void Input() = 0;
    virtual void Update(Application *app) = 0;
    virtual void Draw(Application *app) = 0;
    Application *application_ = nullptr;
};

#endif // GAME_STATE_H_
