#include <iostream>
#include <new>

#include "display.h"
#include "application.h"



Display::Display() {
    std::cout << "Display()" << std::endl;
    sf::ContextSettings settings;
    settings.antialiasingLevel = kAntialiasingLevel;
    window_ = new sf::RenderWindow(sf::VideoMode(kScreenWidth, kScreenHeight), kScreenTitle, sf::Style::Default, settings);
    window_->setPosition(sf::Vector2i(300, 100));
    is_fullscreen_ = false;
    SetVerticalSync(true);
}

Display::~Display() {
    std::cout << "~Display()" << std::endl;
}

void Display::Clear(const sf::Color& color) {
    window_->clear(color);
}

void Display::Draw(const sf::Drawable& drawable) {
    window_->draw(drawable);
}

void Display::Close () {
    window_->close();
}

void Display::DisplayWindow() {
    window_->display();
}

void Display::CheckWindowEvents(Application *app) {
    sf::Event event;
    while(window_->pollEvent(event)) {
        if(event.type == sf::Event::Closed) {
            window_->close();
        }
    }
}

bool Display::IsOpen() {
    return window_->isOpen();
}

void Display::Destroy() {
    delete this;
}

void Display::SetVerticalSync(bool b) {
    window_->setVerticalSyncEnabled(b);
}
