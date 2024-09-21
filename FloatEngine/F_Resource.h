#pragma once
#include <raylib.hpp>
#include <map>
#include <iostream>
#include "F_Text.h"
enum ResourceType{
	ResText,
	ResSound,
   	ResMusic,
	ResImage,
	ResTexture,
	ResFont,
};
class F_Resource
{
	std::map<std::string, Texture> textures;
    std::map<std::string, Image> images;
    std::map<std::string, Sound> sounds;
    std::map<std::string, Music> musics;
	std::map<std::string, F_Text> texts;
	std::map<std::string, Font> fonts;
public:
	void LoadResource(ResourceType type, std::string name, std::string path);
	//施工中
	void LoadTextPro(std::string name, std::string path);

    //获取
    Texture GetTexture(std::string name);
    Sound GetSound(std::string name);
    Music GetMusic(std::string name);
    F_Text GetText(std::string name);
    Font GetFont(std::string name);
    Image GetImage(std::string name);

    template<class ResType>
    ResType GetRes(ResourceType type, std::string name);
	template<class ResType>
	auto GetRes(ResourceType type, std::string name,ResType returnType) -> decltype(returnType);
};

template<class ResType>
inline ResType F_Resource::GetRes(ResourceType type, std::string name)
{
    switch (type) {
        case ResourceType::ResTexture: {
            return textures[name];
            break;
        }
        case ResourceType::ResSound: {
            return sounds[name];
            break;
        }
        case ResourceType::ResMusic: {
            return musics[name];
            break;
        }
        case ResourceType::ResText: {
            return texts[name];
            break;
        }
        case ResourceType::ResImage: {
            return images[name];
            break;
        }
        case ResourceType::ResFont: {
            return fonts[name];
            break;
        }
	}
}
template<class ResType>
inline auto F_Resource::GetRes(ResourceType type, std::string name, ResType returnType) -> decltype(returnType)
{
    switch (type) {
    case ResourceType::ResTexture: {
        return textures[name];
        break;
    }
    case ResourceType::ResSound: {
        return sounds[name];
        break;
    }
    case ResourceType::ResMusic: {
        return musics[name];
        break;
    }
    case ResourceType::ResText: {
        return texts[name];
        break;
    }
    case ResourceType::ResImage: {
        return images[name];
        break;
    }
    case ResourceType::ResFont: {
        return fonts[name];
        break;
    }
    default:
        return returnType;
        break;
    }
}