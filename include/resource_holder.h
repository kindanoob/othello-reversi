#ifndef RESOURCE_HOLDER_H_
#define RESOURCE_HOLDER_H_

//#include "texture_manager.h"
//#include "font_manager.h"
//#include "sound_manager.h"

class FontManager;
class TextureManager;
class SoundManager;
class MusicManager;


class ResourceHolder {
public:
    //static const Resource_holder& get();
    ResourceHolder();
    //TextureManager *textures_ = nullptr;
    FontManager *fonts_ = nullptr;
    //SoundManager *sounds = nullptr;
    //MusicManager *music = nullptr;
    //const sf::Texture& get_texture(Texture_name name) const;
    //const sf::Font& get_font(Font_name name) const;
    //const sf::SoundBuffer& get_sound_buffer(Sound_name name) const;
private:

    //Sound_manager sounds;
};

#endif // RESOURCE_HOLDER_H_
