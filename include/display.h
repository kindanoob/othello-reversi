#ifndef DISPLAY_H_
#define DISPLAY_H_


#include <SFML/Graphics.hpp>
#include "config.h"

class Application;
class Entity;


class Display {
public:    
    Display();
    ~Display();
    void Clear(const sf::Color& clear_color);
    void Draw(const sf::Drawable& drawable);
    void CheckWindowEvents(Application *app);
    bool IsOpen();
    void Close();
    void DisplayWindow();
    void Destroy();    
    void SetVerticalSync(bool b);    
    void UpdateView(double offset_x, double offset_y);
    void ViewResetToInitialState();
    sf::RenderWindow *window_ = nullptr;
    sf::RenderWindow *initial_window = nullptr;
    bool is_fullscreen_ = false;
    sf::View view_;
};

#endif // DISPLAY_H_
