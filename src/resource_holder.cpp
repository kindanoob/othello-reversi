#include <iostream>

#include "resource_holder.h"
#include "font_manager.h"
//#include "sound_manager.h"
//#include "music_manager.h"

/*
const Resource_holder& Resource_holder::get(){
    static Resource_holder holder;
    return holder;
};
*/


ResourceHolder::ResourceHolder(){
    std::cout << "ResourceHolder()" << std::endl;
    //textures = new TextureManager();
    fonts_ = new FontManager();
    //sounds_ = new SoundManager();
    //music_ = new MusicManager();
}



//const sf::SoundBuffer& Resource_holder::get_sound_buffer(Sound_name name) const{
    //sounds.get_resource(name);
//};
