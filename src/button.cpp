#include <iostream>


#include "button.h"


Button::Button(const sf::Vector2f& rect_shape_size,
               const sf::Vector2f& rect_shape_position,
               const sf::Color& rect_shape_color,
               const double rect_shape_outline_thickness,
               const sf::Color& rect_shape_outline_color,
               const sf::Font& font,
               const std::string& button_text_string,
               const int text_character_size,
               const sf::Color& text_color){

    rect_shape_.setSize(rect_shape_size);
    auto button_rect = RectShape().getLocalBounds();
    rect_shape_.setPosition(rect_shape_position);
    rect_shape_.setFillColor(rect_shape_color);

    rect_shape_.setOutlineThickness(rect_shape_outline_thickness);
    rect_shape_.setOutlineColor(rect_shape_outline_color);

    button_text_.setFont(font);
    button_text_.setCharacterSize(text_character_size);
    button_text_.setString(button_text_string);
    auto text_rect = ButtonText().getLocalBounds();


    button_text_.setOrigin(sf::Vector2f(text_rect.width / 2, text_rect.height / 2));

    button_text_.setPosition(rect_shape_position + sf::Vector2f(button_rect.width / 2, button_rect.height / 2));
    button_text_.setFillColor(text_color);

}

//void Button::HandleInput(const sf::RenderWindow& w) {
	//
//}

//void Button::Draw(sf::RenderWindow& w) {
//	w.draw(RectShape());
//	w.draw(ButtonText());
//}
