// Sprite.cpp
#include "Sprite.h"
#include <algorithm>

extern void DEBUG_LOG(int lv, const char* text, bool english = 1, bool auto_enter = 1,bool =1);
Sprite::Sprite() {
    frames = std::vector<Texture>();
    currentFrame = 0;
    isOwner = 1;
}

Sprite::Sprite(Texture2D texture, bool takeOwnership) {
    if (takeOwnership) {
        frames.push_back(texture);
    }
    else {
        frames.push_back(Texture2D{
            texture.id, texture.width, texture.height,
            texture.mipmaps, texture.format
            });
    }
}

Sprite::Sprite(const std::vector<const char*>& filePaths) {
    for (auto path : filePaths) {
        LoadFrame(path);
    }
}

Sprite::~Sprite() {
    if (isOwner) {
        for (auto& tex : frames) {
            if (tex.id != 0) {
                UnloadTexture(tex);
            }
        }
    }
    frames.clear();
}

Sprite::Sprite(const Sprite&other):
    frames(other.frames),
    currentFrame(other.currentFrame),
    isOwner(other.isOwner)
{
    
}

Sprite& Sprite::operator=(const Sprite& other) {
    if (this != &other) {
        if (isOwner) {
            for (auto& tex : frames) {
                if (tex.id != 0) {
                    UnloadTexture(tex);
                }
            }
        }
        frames = other.frames;
        currentFrame = other.currentFrame;
        isOwner = other.isOwner;
    }
    return *this;
}

Sprite::Sprite(Sprite&& other) noexcept
    : frames(std::move(other.frames)),
    currentFrame(other.currentFrame),
    isOwner(other.isOwner) {
    other.isOwner = false;
}

Sprite& Sprite::operator=(Sprite&& other) noexcept {
    if (this != &other) {
        if (isOwner) {
            for (auto& tex : frames) {
                if (tex.id != 0) {
                    UnloadTexture(tex);
                }
            }
        }
        frames = std::move(other.frames);
        currentFrame = other.currentFrame;
        isOwner = other.isOwner;
        other.isOwner = false;
    }
    return *this;
}

void Sprite::AddFrame(Texture2D&& texture) {
    frames.push_back(texture);
}

bool Sprite::LoadFrame(const char* filePath) {
    Texture tex = LoadTexture(filePath);

    if (tex.id == 0) return false;

    frames.push_back(tex);
    return true;
}

bool Sprite::LoadFrameFromResource(F_Resource* resource, const char* name)
{
    if (resource->HasResource(name, ResTexture)) {
        Texture t = resource->GetTexture(name);
		frames.push_back(t);
		return true;
    }
    else {
		DEBUG_LOG(LOG_WARNING, "Sprite.LoadFrameFromResource:资源未找到",0);
        return false;
    }
}


void Sprite::SetFrame(size_t index) {
    if (index < frames.size()) {
        currentFrame = index;
    }
    else {
		DEBUG_LOG(LOG_WARNING, "Sprite.SetFrame:索引超出范围", 0);
    }
}

void Sprite::UpdateFrameIndex()
{
	currentFrame = (currentFrame + 1) % frames.size();
}

void Sprite::SetFrameTexture(size_t index, const Texture& tex)
{
	if (index < frames.size()) {
		frames[index] = tex;
	}
    else {
		DEBUG_LOG(LOG_WARNING, "Sprite.SetFrameTexture:索引超出范围", 0);
    }
}

Texture2D Sprite::GetCurrentFrame() const {
    return !frames.empty() ? frames[currentFrame] : Texture2D{ 0 };
}

Texture2D Sprite::GetFrameIndex(int index) const
{
	if (index>0 && index < frames.size())
	{
		return frames[index];
	}
    return Texture{};
}

Texture2D* Sprite::GetAllFrame() const
{
    return const_cast<Texture*>(frames.data());
}

size_t Sprite::FrameCount() const {
    return frames.size();
}