#ifndef BUTTON_H
#define BUTTON_H


#include <string>
#include <SFML/Graphics.hpp>



class Button{
public:
    Button(const sf::Vector2f& rect_shape_size,
               const sf::Vector2f& rect_shape_position,
               const sf::Color& rect_shape_color,
               const double rect_shape_outline_thickness,
               const sf::Color& rect_shape_outline_color,
               const sf::Font& font,
               const std::string& button_text_string,
               const int text_character_size,
               const sf::Color& text_color);
    sf::RectangleShape& RectShape(){
        return rect_shape_;
    }
    void SetRectShape(sf::RectangleShape& rect_shape){
        rect_shape_ = rect_shape;
    }
    sf::Vector2f RectShapeSize(){
        return rect_shape_size_;
    }
    void SetRectShapeSize(sf::Vector2f& rect_shape_size){
        rect_shape_size_ = rect_shape_size;
    }
    sf::Text ButtonText(){
        return button_text_;
    }
    void SetButtonText(sf::Text& button_text, const sf::Font& button_font,
                         const int button_text_size){
        button_text_ = button_text;
        button_text_.setFont(button_font);
        button_text_.setCharacterSize(button_text_size);
    }
    void SetRectShapeFillColor(const sf::Color& color){
        RectShape().setFillColor(color);
    }
private:
    sf::RectangleShape rect_shape_;
    sf::Vector2f rect_shape_size_;
    sf::Vector2f rect_shape_position_;

    double rect_shape_outline_thickness_ = 0.0;
    sf::Color rect_shape_outline_color_;

    sf::Color rect_shape_color_;
    sf::Text button_text_;
};
#endif // BUTTON_H
