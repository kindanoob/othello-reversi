#ifndef FONT_MANAGER_H_
#define FONT_MANAGER_H_

#include <SFML/Graphics.hpp>
#include <map>

#include "resource_manager.h"

enum class FontName {
    kUbuntuFont,
    kArialFont,
    kCyrillicFont
};

class FontManager: public ResourceManager<FontName, sf::Font> {
public:
    FontManager();
};


#endif // FONT_MANAGER_H_
