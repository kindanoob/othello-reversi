#include <iostream>

#include "config.h"
#include "font_manager.h"


FontManager::FontManager() {
    std::cout << "FontManager()" << std::endl;
    AddResource(FontName::kArialFont, kFontsPath + "arial.ttf");
    AddResource(FontName::kUbuntuFont, kFontsPath + "ubuntu.ttf");
    AddResource(FontName::kCyrillicFont, kFontsPath + "CyrillicOld.ttf");
}
