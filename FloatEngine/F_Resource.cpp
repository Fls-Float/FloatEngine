#include "F_Resource.h"

void F_Resource::LoadResource(ResourceType type, std::string name, std::string path)
{
    switch (type) {
    case ResourceType::ResTexture: {
        // Load texture
        std::pair<std::string, Texture> t = std::make_pair(name, LoadTexture(path.c_str()));
        textures.insert(t);
        break;
    }
    case ResourceType::ResSound: {
        // Load texture
        std::pair<std::string, Sound> t = std::make_pair(name, LoadSound(path.c_str()));
        sounds.insert(t);
        break;
    }
    case ResourceType::ResMusic: {
        // Load texture
        std::pair<std::string, Music> t = std::make_pair(name, LoadMusicStream(path.c_str()));
        musics.insert(t);
        break;
    }
    case ResourceType::ResText: {
        // Load texture
        std::pair<std::string, F_Text> t = std::make_pair(name, F_Text(path.c_str()));
        texts.insert(t);
        break;
    }
    case ResourceType::ResImage: {
        // Load texture
        std::pair<std::string, Image> t = std::make_pair(name, LoadImage(path.c_str()));
        images.insert(t);
        break;
    }
    case ResourceType::ResFont: {
        // Load texture
        std::pair<std::string, Font> t = std::make_pair(name, LoadFont(path.c_str()));
        fonts.insert(t);
        break;
    }
    }
}

Texture F_Resource::GetTexture(std::string name)
{
    if (textures.find(name) != textures.end())
        return textures[name];
    else {
        std::cout << "错误:在F_Resource_GetTexture中 Texture Name:\" " << name << "\" 未被找到" << std::endl;
        return Texture();
    }
}
Sound F_Resource::GetSound(std::string name) {
    if (sounds.find(name) != sounds.end())
        return sounds[name];
    else {
        std::cout << "错误:在F_Resource_GetSound中 Sound Name:\" " << name << "\" 未被找到" << std::endl;
        return Sound();
    }
}

Music F_Resource::GetMusic(std::string name)
{
    if (musics.find(name) != musics.end())
        return musics[name];
    else {
        std::cout << "错误:在F_Resource_GetMusic中 Music Name:\" " << name << "\" 未被找到" << std::endl;
        return Music();
    }
}

F_Text F_Resource::GetText(std::string name)
{
    if (texts.find(name) != texts.end())
        return texts[name];
    else {
        std::cout << "错误:在F_Resource_GetText中 Text Name:\" " << name << "\" 未被找到" << std::endl;
        return F_Text("");
    }
}

Font F_Resource::GetFont(std::string name)
{
    if (fonts.find(name) != fonts.end())
        return fonts[name];
    else {
        std::cout << "错误:在F_Resource_GetFont中 Font Name:\" " << name << "\" 未被找到" << std::endl;
        return Font();
    }
}

Image F_Resource::GetImage(std::string name)
{
    if (images.find(name) != images.end())
        return images[name];
    else {
        std::cout << "错误:在F_Resource_GetImage中 Image Name:\" " << name << "\" 未被找到" << std::endl;
        return Image();
    }
}

