#pragma once

// Direction constants
struct {
    static constexpr float LEFT = 270.0f;
    static constexpr float RIGHT = 90.0f;
    static constexpr float UP = 0.0f;
    static constexpr float DOWN = 180.0f;
}DIR;

// Collision states
enum Collision {
    Collision_None = 0,
    Collision_With = 1,
    Collision_WithBlock = 2,
    Collision_WithActor = 3,
};

// Object properties constants
struct {
    static constexpr int Normal = 0;
    static constexpr int Actor = 1;
    static constexpr int Block = 2;
}OBJPRO;

// Audio modes
enum AudioMode {
    Audio_Unknown = 0,
    Audio_Sound = 1,
    Audio_Music = 2,
};
