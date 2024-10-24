#pragma once

#ifndef FLRAYLIB_H
#define FLRAYLIB_H
#include <stdarg.h>     // Required for: va_list - Only used by TraceLogCallback
#ifndef FRLAPI
#define FRLAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif
#ifndef PI
#define PI 3.14159265358979323846f
#endif
#ifndef DEG2RAD
#define DEG2RAD (PI/180.0f)
#endif
#ifndef RAD2DEG
#define RAD2DEG (180.0f/PI)
#endif
#ifndef RL_MALLOC
#define RL_MALLOC(sz)       malloc(sz)
#endif
#ifndef RL_CALLOC
#define RL_CALLOC(n,sz)     calloc(n,sz)
#endif
#ifndef RL_REALLOC
#define RL_REALLOC(ptr,sz)  realloc(ptr,sz)
#endif
#ifndef RL_FREE
#define RL_FREE(ptr)        free(ptr)
#endif
#if defined(__cplusplus)
#define CLITERAL(type)      type
#else
#define CLITERAL(type)      (type)
#endif
#if !defined(_MSC_VER) && (defined(__cplusplus) && __cplusplus < 201103L)
#error "C++11 or later is required. Add -std=c++11"
#endif
#define RL_COLOR_TYPE
#define RL_RECTANGLE_TYPE
#define RL_VECTOR2_TYPE
#define RL_VECTOR3_TYPE
#define RL_VECTOR4_TYPE
#define RL_QUATERNION_TYPE
#define RL_MATRIX_TYPE
#define LIGHTGRAY  CLITERAL(Color){ 200, 200, 200, 255 }   // Light Gray
#define GRAY       CLITERAL(Color){ 130, 130, 130, 255 }   // Gray
#define DARKGRAY   CLITERAL(Color){ 80, 80, 80, 255 }      // Dark Gray
#define YELLOW     CLITERAL(Color){ 253, 249, 0, 255 }     // Yellow
#define GOLD       CLITERAL(Color){ 255, 203, 0, 255 }     // Gold
#define ORANGE     CLITERAL(Color){ 255, 161, 0, 255 }     // Orange
#define PINK       CLITERAL(Color){ 255, 109, 194, 255 }   // Pink
#define RED        CLITERAL(Color){ 230, 41, 55, 255 }     // Red
#define MAROON     CLITERAL(Color){ 190, 33, 55, 255 }     // Maroon
#define GREEN      CLITERAL(Color){ 0, 228, 48, 255 }      // Green
#define LIME       CLITERAL(Color){ 0, 158, 47, 255 }      // Lime
#define DARKGREEN  CLITERAL(Color){ 0, 117, 44, 255 }      // Dark Green
#define SKYBLUE    CLITERAL(Color){ 102, 191, 255, 255 }   // Sky Blue
#define BLUE       CLITERAL(Color){ 0, 121, 241, 255 }     // Blue
#define DARKBLUE   CLITERAL(Color){ 0, 82, 172, 255 }      // Dark Blue
#define PURPLE     CLITERAL(Color){ 200, 122, 255, 255 }   // Purple
#define VIOLET     CLITERAL(Color){ 135, 60, 190, 255 }    // Violet
#define DARKPURPLE CLITERAL(Color){ 112, 31, 126, 255 }    // Dark Purple
#define BEIGE      CLITERAL(Color){ 211, 176, 131, 255 }   // Beige
#define BROWN      CLITERAL(Color){ 127, 106, 79, 255 }    // Brown
#define DARKBROWN  CLITERAL(Color){ 76, 63, 47, 255 }      // Dark Brown

#define WHITE      CLITERAL(Color){ 255, 255, 255, 255 }   // White
#define BLACK      CLITERAL(Color){ 0, 0, 0, 255 }         // Black
#define BLANK      CLITERAL(Color){ 0, 0, 0, 0 }           // Blank (Transparent)
#define MAGENTA    CLITERAL(Color){ 255, 0, 255, 255 }     // Magenta
#define RAYWHITE   CLITERAL(Color){ 245, 245, 245, 255 }   // My own White (raylib logo)
#if (defined(__STDC__) && __STDC_VERSION__ >= 199901L) || (defined(_MSC_VER) && _MSC_VER >= 1800)
#include <stdbool.h>
#elif !defined(__cplusplus) && !defined(bool)
typedef enum bool{ false = 0, true = !false } bool;
#define RL_BOOL_TYPE
#endif

typedef struct Vector2 {
    float x;                
    float y;                
} Vector2;

// Vector3, 3 components
typedef struct Vector3 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
} Vector3;

// Vector4, 4 components
typedef struct Vector4 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
    float w;                // Vector w component
} Vector4;

// Quaternion, 4 components (Vector4 alias)
typedef Vector4 Quaternion;

// Matrix, 4x4 components, column major, OpenGL style, right-handed
typedef struct Matrix {
    float m0, m4, m8, m12;  // Matrix first row (4 components)
    float m1, m5, m9, m13;  // Matrix second row (4 components)
    float m2, m6, m10, m14; // Matrix third row (4 components)
    float m3, m7, m11, m15; // Matrix fourth row (4 components)
} Matrix;

// Color, 4 components, R8G8B8A8 (32bit)
typedef struct Color {
    unsigned char r;        // Color red value
    unsigned char g;        // Color green value
    unsigned char b;        // Color blue value
    unsigned char a;        // Color alpha value
} Color;

// Rectangle, 4 components
typedef struct Rectangle {
    float x;                // Rectangle top-left corner position x
    float y;                // Rectangle top-left corner position y
    float width;            // Rectangle width
    float height;           // Rectangle height
} Rectangle;

// Image, pixel data stored in CPU memory (RAM)
typedef struct Image {
    void* data;             // Image raw data
    int width;              // Image base width
    int height;             // Image base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (PixelFormat type)
} Image;

// Texture, tex data stored in GPU memory (VRAM)
typedef struct Texture {
    unsigned int id;        // OpenGL texture id
    int width;              // Texture base width
    int height;             // Texture base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (PixelFormat type)
} Texture;

// Texture2D, same as Texture
typedef Texture Texture2D;

// TextureCubemap, same as Texture
typedef Texture TextureCubemap;

// RenderTexture, fbo for texture rendering
typedef struct RenderTexture {
    unsigned int id;        // OpenGL framebuffer object id
    Texture texture;        // Color buffer attachment texture
    Texture depth;          // Depth buffer attachment texture
} RenderTexture;

// RenderTexture2D, same as RenderTexture
typedef RenderTexture RenderTexture2D;

// NPatchInfo, n-patch layout info
typedef struct NPatchInfo {
    Rectangle source;       // Texture source rectangle
    int left;               // Left border offset
    int top;                // Top border offset
    int right;              // Right border offset
    int bottom;             // Bottom border offset
    int layout;             // Layout of the n-patch: 3x3, 1x3 or 3x1
} NPatchInfo;

// GlyphInfo, font characters glyphs info
typedef struct GlyphInfo {
    int value;              // Character value (Unicode)
    int offsetX;            // Character offset X when drawing
    int offsetY;            // Character offset Y when drawing
    int advanceX;           // Character advance position X
    Image image;            // Character image data
} GlyphInfo;

// Font, font texture and GlyphInfo array data
typedef struct Font {
    int baseSize;           // Base size (default chars height)
    int glyphCount;         // Number of glyph characters
    int glyphPadding;       // Padding around the glyph characters
    Texture2D texture;      // Texture atlas containing the glyphs
    Rectangle* recs;        // Rectangles in texture for the glyphs
    GlyphInfo* glyphs;      // Glyphs info data
} Font;

// Camera, defines position/orientation in 3d space
typedef struct Camera3D {
    Vector3 position;       // Camera position
    Vector3 target;         // Camera target it looks-at
    Vector3 up;             // Camera up vector (rotation over its axis)
    float fovy;             // Camera field-of-view aperture in Y (degrees) in perspective, used as near plane width in orthographic
    int projection;         // Camera projection: CAMERA_PERSPECTIVE or CAMERA_ORTHOGRAPHIC
} Camera3D;

typedef Camera3D Camera;    // Camera type fallback, defaults to Camera3D

// Camera2D, defines position/orientation in 2d space
typedef struct Camera2D {
    Vector2 offset;         // Camera offset (displacement from target)
    Vector2 target;         // Camera target (rotation and zoom origin)
    float rotation;         // Camera rotation in degrees
    float zoom;             // Camera zoom (scaling), should be 1.0f by default
} Camera2D;

// Mesh, vertex data and vao/vbo
typedef struct Mesh {
    int vertexCount;        // Number of vertices stored in arrays
    int triangleCount;      // Number of triangles stored (indexed or not)

    // Vertex attributes data
    float* vertices;        // Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
    float* texcoords;       // Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
    float* texcoords2;      // Vertex texture second coordinates (UV - 2 components per vertex) (shader-location = 5)
    float* normals;         // Vertex normals (XYZ - 3 components per vertex) (shader-location = 2)
    float* tangents;        // Vertex tangents (XYZW - 4 components per vertex) (shader-location = 4)
    unsigned char* colors;      // Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
    unsigned short* indices;    // Vertex indices (in case vertex data comes indexed)

    // Animation vertex data
    float* animVertices;    // Animated vertex positions (after bones transformations)
    float* animNormals;     // Animated normals (after bones transformations)
    unsigned char* boneIds; // Vertex bone ids, max 255 bone ids, up to 4 bones influence by vertex (skinning)
    float* boneWeights;     // Vertex bone weight, up to 4 bones influence by vertex (skinning)

    // OpenGL identifiers
    unsigned int vaoId;     // OpenGL Vertex Array Object id
    unsigned int* vboId;    // OpenGL Vertex Buffer Objects id (default vertex data)
} Mesh;

// Shader
typedef struct Shader {
    unsigned int id;        // Shader program id
    int* locs;              // Shader locations array (RL_MAX_SHADER_LOCATIONS)
} Shader;

// MaterialMap
typedef struct MaterialMap {
    Texture2D texture;      // Material map texture
    Color color;            // Material map color
    float value;            // Material map value
} MaterialMap;

// Material, includes shader and maps
typedef struct Material {
    Shader shader;          // Material shader
    MaterialMap* maps;      // Material maps array (MAX_MATERIAL_MAPS)
    float params[4];        // Material generic parameters (if required)
} Material;

// Transform, vertex transformation data
typedef struct Transform {
    Vector3 translation;    // Translation
    Quaternion rotation;    // Rotation
    Vector3 scale;          // Scale
} Transform;

// Bone, skeletal animation bone
typedef struct BoneInfo {
    char name[32];          // Bone name
    int parent;             // Bone parent
} BoneInfo;

// Model, meshes, materials and animation data
typedef struct Model {
    Matrix transform;       // Local transform matrix

    int meshCount;          // Number of meshes
    int materialCount;      // Number of materials
    Mesh* meshes;           // Meshes array
    Material* materials;    // Materials array
    int* meshMaterial;      // Mesh material number

    // Animation data
    int boneCount;          // Number of bones
    BoneInfo* bones;        // Bones information (skeleton)
    Transform* bindPose;    // Bones base transformation (pose)
} Model;

// ModelAnimation
typedef struct ModelAnimation {
    int boneCount;          // Number of bones
    int frameCount;         // Number of animation frames
    BoneInfo* bones;        // Bones information (skeleton)
    Transform** framePoses; // Poses array by frame
    char name[32];          // Animation name
} ModelAnimation;

// Ray, ray for raycasting
typedef struct Ray {
    Vector3 position;       // Ray position (origin)
    Vector3 direction;      // Ray direction
} Ray;

// RayCollision, ray hit information
typedef struct RayCollision {
    bool hit;               // Did the ray hit something?
    float distance;         // Distance to the nearest hit
    Vector3 point;          // Point of the nearest hit
    Vector3 normal;         // Surface normal of hit
} RayCollision;

// BoundingBox
typedef struct BoundingBox {
    Vector3 min;            // Minimum vertex box-corner
    Vector3 max;            // Maximum vertex box-corner
} BoundingBox;

// Wave, audio wave data
typedef struct Wave {
    unsigned int frameCount;    // Total number of frames (considering channels)
    unsigned int sampleRate;    // Frequency (samples per second)
    unsigned int sampleSize;    // Bit depth (bits per sample): 8, 16, 32 (24 not supported)
    unsigned int channels;      // Number of channels (1-mono, 2-stereo, ...)
    void* data;                 // Buffer data pointer
} Wave;

// Opaque structs declaration
// NOTE: Actual structs are defined internally in raudio module
typedef struct rAudioBuffer rAudioBuffer;
typedef struct rAudioProcessor rAudioProcessor;

// AudioStream, custom audio stream
typedef struct AudioStream {
    rAudioBuffer* buffer;       // Pointer to internal data used by the audio system
    rAudioProcessor* processor; // Pointer to internal data processor, useful for audio effects

    unsigned int sampleRate;    // Frequency (samples per second)
    unsigned int sampleSize;    // Bit depth (bits per sample): 8, 16, 32 (24 not supported)
    unsigned int channels;      // Number of channels (1-mono, 2-stereo, ...)
} AudioStream;

// Sound
typedef struct Sound {
    AudioStream stream;         // Audio stream
    unsigned int frameCount;    // Total number of frames (considering channels)
} Sound;

// Music, audio stream, anything longer than ~10 seconds should be streamed
typedef struct Music {
    AudioStream stream;         // Audio stream
    unsigned int frameCount;    // Total number of frames (considering channels)
    bool looping;               // Music looping enable

    int ctxType;                // Type of music context (audio filetype)
    void* ctxData;              // Audio context data, depends on type
} Music;

// VrDeviceInfo, Head-Mounted-Display device parameters
typedef struct VrDeviceInfo {
    int hResolution;                // Horizontal resolution in pixels
    int vResolution;                // Vertical resolution in pixels
    float hScreenSize;              // Horizontal size in meters
    float vScreenSize;              // Vertical size in meters
    float vScreenCenter;            // Screen center in meters
    float eyeToScreenDistance;      // Distance between eye and display in meters
    float lensSeparationDistance;   // Lens separation distance in meters
    float interpupillaryDistance;   // IPD (distance between pupils) in meters
    float lensDistortionValues[4];  // Lens distortion constant parameters
    float chromaAbCorrection[4];    // Chromatic aberration correction parameters
} VrDeviceInfo;

// VrStereoConfig, VR stereo rendering configuration for simulator
typedef struct VrStereoConfig {
    Matrix projection[2];           // VR projection matrices (per eye)
    Matrix viewOffset[2];           // VR view offset matrices (per eye)
    float leftLensCenter[2];        // VR left lens center
    float rightLensCenter[2];       // VR right lens center
    float leftScreenCenter[2];      // VR left screen center
    float rightScreenCenter[2];     // VR right screen center
    float scale[2];                 // VR distortion scale
    float scaleIn[2];               // VR distortion scale in
} VrStereoConfig;

// File path list
typedef struct FilePathList {
    unsigned int capacity;          // Filepaths max entries
    unsigned int count;             // Filepaths entries count
    char** paths;                   // Filepaths entries
} FilePathList;

// Automation event
typedef struct AutomationEvent {
    unsigned int frame;             // Event frame
    unsigned int type;              // Event type (AutomationEventType)
    int params[4];                  // Event parameters (if required)
} AutomationEvent;

// Automation event list
typedef struct AutomationEventList {
    unsigned int capacity;          // Events max entries (MAX_AUTOMATION_EVENTS)
    unsigned int count;             // Events entries count
    AutomationEvent* events;        // Events entries
} AutomationEventList;

//----------------------------------------------------------------------------------
// Enumerators Definition
//----------------------------------------------------------------------------------
// System/Window config flags
// NOTE: Every bit registers one state (use it with bit masks)
// By default all flags are set to 0
typedef enum {
    FLAG_VSYNC_HINT = 0x00000040,   // Set to try enabling V-Sync on GPU
    FLAG_FULLSCREEN_MODE = 0x00000002,   // Set to run program in fullscreen
    FLAG_WINDOW_RESIZABLE = 0x00000004,   // Set to allow resizable window
    FLAG_WINDOW_UNDECORATED = 0x00000008,   // Set to disable window decoration (frame and buttons)
    FLAG_WINDOW_HIDDEN = 0x00000080,   // Set to hide window
    FLAG_WINDOW_MINIMIZED = 0x00000200,   // Set to minimize window (iconify)
    FLAG_WINDOW_MAXIMIZED = 0x00000400,   // Set to maximize window (expanded to monitor)
    FLAG_WINDOW_UNFOCUSED = 0x00000800,   // Set to window non focused
    FLAG_WINDOW_TOPMOST = 0x00001000,   // Set to window always on top
    FLAG_WINDOW_ALWAYS_RUN = 0x00000100,   // Set to allow windows running while minimized
    FLAG_WINDOW_TRANSPARENT = 0x00000010,   // Set to allow transparent framebuffer
    FLAG_WINDOW_HIGHDPI = 0x00002000,   // Set to support HighDPI
    FLAG_WINDOW_MOUSE_PASSTHROUGH = 0x00004000, // Set to support mouse passthrough, only supported when FLAG_WINDOW_UNDECORATED
    FLAG_BORDERLESS_WINDOWED_MODE = 0x00008000, // Set to run program in borderless windowed mode
    FLAG_MSAA_4X_HINT = 0x00000020,   // Set to try enabling MSAA 4X
    FLAG_INTERLACED_HINT = 0x00010000    // Set to try enabling interlaced video format (for V3D)
} ConfigFlags;

// Trace log level
// NOTE: Organized by priority level
typedef enum {
    LOG_ALL = 0,        // Display all logs
    LOG_TRACE,          // Trace logging, intended for internal use only
    LOG_DEBUG,          // Debug logging, used for internal debugging, it should be disabled on release builds
    LOG_INFO,           // Info logging, used for program execution info
    LOG_WARNING,        // Warning logging, used on recoverable failures
    LOG_ERROR,          // Error logging, used on unrecoverable failures
    LOG_FATAL,          // Fatal logging, used to abort program: exit(EXIT_FAILURE)
    LOG_NONE            // Disable logging
} TraceLogLevel;

// Keyboard keys (US keyboard layout)
// NOTE: Use GetKeyPressed() to allow redefining
// required keys for alternative layouts
typedef enum {
    KEY_NULL = 0,        // Key: NULL, used for no key pressed
    // Alphanumeric keys
    KEY_APOSTROPHE = 39,       // Key: '
    KEY_COMMA = 44,       // Key: ,
    KEY_MINUS = 45,       // Key: -
    KEY_PERIOD = 46,       // Key: .
    KEY_SLASH = 47,       // Key: /
    KEY_ZERO = 48,       // Key: 0
    KEY_ONE = 49,       // Key: 1
    KEY_TWO = 50,       // Key: 2
    KEY_THREE = 51,       // Key: 3
    KEY_FOUR = 52,       // Key: 4
    KEY_FIVE = 53,       // Key: 5
    KEY_SIX = 54,       // Key: 6
    KEY_SEVEN = 55,       // Key: 7
    KEY_EIGHT = 56,       // Key: 8
    KEY_NINE = 57,       // Key: 9
    KEY_SEMICOLON = 59,       // Key: ;
    KEY_EQUAL = 61,       // Key: =
    KEY_A = 65,       // Key: A | a
    KEY_B = 66,       // Key: B | b
    KEY_C = 67,       // Key: C | c
    KEY_D = 68,       // Key: D | d
    KEY_E = 69,       // Key: E | e
    KEY_F = 70,       // Key: F | f
    KEY_G = 71,       // Key: G | g
    KEY_H = 72,       // Key: H | h
    KEY_I = 73,       // Key: I | i
    KEY_J = 74,       // Key: J | j
    KEY_K = 75,       // Key: K | k
    KEY_L = 76,       // Key: L | l
    KEY_M = 77,       // Key: M | m
    KEY_N = 78,       // Key: N | n
    KEY_O = 79,       // Key: O | o
    KEY_P = 80,       // Key: P | p
    KEY_Q = 81,       // Key: Q | q
    KEY_R = 82,       // Key: R | r
    KEY_S = 83,       // Key: S | s
    KEY_T = 84,       // Key: T | t
    KEY_U = 85,       // Key: U | u
    KEY_V = 86,       // Key: V | v
    KEY_W = 87,       // Key: W | w
    KEY_X = 88,       // Key: X | x
    KEY_Y = 89,       // Key: Y | y
    KEY_Z = 90,       // Key: Z | z
    KEY_LEFT_BRACKET = 91,       // Key: [
    KEY_BACKSLASH = 92,       // Key: '\'
    KEY_RIGHT_BRACKET = 93,       // Key: ]
    KEY_GRAVE = 96,       // Key: `
    // Function keys
    KEY_SPACE = 32,       // Key: Space
    KEY_ESCAPE = 256,      // Key: Esc
    KEY_ENTER = 257,      // Key: Enter
    KEY_TAB = 258,      // Key: Tab
    KEY_BACKSPACE = 259,      // Key: Backspace
    KEY_INSERT = 260,      // Key: Ins
    KEY_DELETE = 261,      // Key: Del
    KEY_RIGHT = 262,      // Key: Cursor right
    KEY_LEFT = 263,      // Key: Cursor left
    KEY_DOWN = 264,      // Key: Cursor down
    KEY_UP = 265,      // Key: Cursor up
    KEY_PAGE_UP = 266,      // Key: Page up
    KEY_PAGE_DOWN = 267,      // Key: Page down
    KEY_HOME = 268,      // Key: Home
    KEY_END = 269,      // Key: End
    KEY_CAPS_LOCK = 280,      // Key: Caps lock
    KEY_SCROLL_LOCK = 281,      // Key: Scroll down
    KEY_NUM_LOCK = 282,      // Key: Num lock
    KEY_PRINT_SCREEN = 283,      // Key: Print screen
    KEY_PAUSE = 284,      // Key: Pause
    KEY_F1 = 290,      // Key: F1
    KEY_F2 = 291,      // Key: F2
    KEY_F3 = 292,      // Key: F3
    KEY_F4 = 293,      // Key: F4
    KEY_F5 = 294,      // Key: F5
    KEY_F6 = 295,      // Key: F6
    KEY_F7 = 296,      // Key: F7
    KEY_F8 = 297,      // Key: F8
    KEY_F9 = 298,      // Key: F9
    KEY_F10 = 299,      // Key: F10
    KEY_F11 = 300,      // Key: F11
    KEY_F12 = 301,      // Key: F12
    KEY_LEFT_SHIFT = 340,      // Key: Shift left
    KEY_LEFT_CONTROL = 341,      // Key: Control left
    KEY_LEFT_ALT = 342,      // Key: Alt left
    KEY_LEFT_SUPER = 343,      // Key: Super left
    KEY_RIGHT_SHIFT = 344,      // Key: Shift right
    KEY_RIGHT_CONTROL = 345,      // Key: Control right
    KEY_RIGHT_ALT = 346,      // Key: Alt right
    KEY_RIGHT_SUPER = 347,      // Key: Super right
    KEY_KB_MENU = 348,      // Key: KB menu
    // Keypad keys
    KEY_KP_0 = 320,      // Key: Keypad 0
    KEY_KP_1 = 321,      // Key: Keypad 1
    KEY_KP_2 = 322,      // Key: Keypad 2
    KEY_KP_3 = 323,      // Key: Keypad 3
    KEY_KP_4 = 324,      // Key: Keypad 4
    KEY_KP_5 = 325,      // Key: Keypad 5
    KEY_KP_6 = 326,      // Key: Keypad 6
    KEY_KP_7 = 327,      // Key: Keypad 7
    KEY_KP_8 = 328,      // Key: Keypad 8
    KEY_KP_9 = 329,      // Key: Keypad 9
    KEY_KP_DECIMAL = 330,      // Key: Keypad .
    KEY_KP_DIVIDE = 331,      // Key: Keypad /
    KEY_KP_MULTIPLY = 332,      // Key: Keypad *
    KEY_KP_SUBTRACT = 333,      // Key: Keypad -
    KEY_KP_ADD = 334,      // Key: Keypad +
    KEY_KP_ENTER = 335,      // Key: Keypad Enter
    KEY_KP_EQUAL = 336,      // Key: Keypad =
    // Android key buttons
    KEY_BACK = 4,        // Key: Android back button
    KEY_MENU = 82,       // Key: Android menu button
    KEY_VOLUME_UP = 24,       // Key: Android volume up button
    KEY_VOLUME_DOWN = 25        // Key: Android volume down button
} KeyboardKey;

// Add backwards compatibility support for deprecated names
#define MOUSE_LEFT_BUTTON   MOUSE_BUTTON_LEFT
#define MOUSE_RIGHT_BUTTON  MOUSE_BUTTON_RIGHT
#define MOUSE_MIDDLE_BUTTON MOUSE_BUTTON_MIDDLE

// Mouse buttons
typedef enum {
    MOUSE_BUTTON_LEFT = 0,       // Mouse button left
    MOUSE_BUTTON_RIGHT = 1,       // Mouse button right
    MOUSE_BUTTON_MIDDLE = 2,       // Mouse button middle (pressed wheel)
    MOUSE_BUTTON_SIDE = 3,       // Mouse button side (advanced mouse device)
    MOUSE_BUTTON_EXTRA = 4,       // Mouse button extra (advanced mouse device)
    MOUSE_BUTTON_FORWARD = 5,       // Mouse button forward (advanced mouse device)
    MOUSE_BUTTON_BACK = 6,       // Mouse button back (advanced mouse device)
} MouseButton;

// Mouse cursor
typedef enum {
    MOUSE_CURSOR_DEFAULT = 0,     // Default pointer shape
    MOUSE_CURSOR_ARROW = 1,     // Arrow shape
    MOUSE_CURSOR_IBEAM = 2,     // Text writing cursor shape
    MOUSE_CURSOR_CROSSHAIR = 3,     // Cross shape
    MOUSE_CURSOR_POINTING_HAND = 4,     // Pointing hand cursor
    MOUSE_CURSOR_RESIZE_EW = 5,     // Horizontal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NS = 6,     // Vertical resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NWSE = 7,     // Top-left to bottom-right diagonal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_NESW = 8,     // The top-right to bottom-left diagonal resize/move arrow shape
    MOUSE_CURSOR_RESIZE_ALL = 9,     // The omnidirectional resize/move cursor shape
    MOUSE_CURSOR_NOT_ALLOWED = 10     // The operation-not-allowed shape
} MouseCursor;

// Gamepad buttons
typedef enum {
    GAMEPAD_BUTTON_UNKNOWN = 0,         // Unknown button, just for error checking
    GAMEPAD_BUTTON_LEFT_FACE_UP,        // Gamepad left DPAD up button
    GAMEPAD_BUTTON_LEFT_FACE_RIGHT,     // Gamepad left DPAD right button
    GAMEPAD_BUTTON_LEFT_FACE_DOWN,      // Gamepad left DPAD down button
    GAMEPAD_BUTTON_LEFT_FACE_LEFT,      // Gamepad left DPAD left button
    GAMEPAD_BUTTON_RIGHT_FACE_UP,       // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
    GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,    // Gamepad right button right (i.e. PS3: Square, Xbox: X)
    GAMEPAD_BUTTON_RIGHT_FACE_DOWN,     // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
    GAMEPAD_BUTTON_RIGHT_FACE_LEFT,     // Gamepad right button left (i.e. PS3: Circle, Xbox: B)
    GAMEPAD_BUTTON_LEFT_TRIGGER_1,      // Gamepad top/back trigger left (first), it could be a trailing button
    GAMEPAD_BUTTON_LEFT_TRIGGER_2,      // Gamepad top/back trigger left (second), it could be a trailing button
    GAMEPAD_BUTTON_RIGHT_TRIGGER_1,     // Gamepad top/back trigger right (one), it could be a trailing button
    GAMEPAD_BUTTON_RIGHT_TRIGGER_2,     // Gamepad top/back trigger right (second), it could be a trailing button
    GAMEPAD_BUTTON_MIDDLE_LEFT,         // Gamepad center buttons, left one (i.e. PS3: Select)
    GAMEPAD_BUTTON_MIDDLE,              // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
    GAMEPAD_BUTTON_MIDDLE_RIGHT,        // Gamepad center buttons, right one (i.e. PS3: Start)
    GAMEPAD_BUTTON_LEFT_THUMB,          // Gamepad joystick pressed button left
    GAMEPAD_BUTTON_RIGHT_THUMB          // Gamepad joystick pressed button right
} GamepadButton;

// Gamepad axis
typedef enum {
    GAMEPAD_AXIS_LEFT_X = 0,     // Gamepad left stick X axis
    GAMEPAD_AXIS_LEFT_Y = 1,     // Gamepad left stick Y axis
    GAMEPAD_AXIS_RIGHT_X = 2,     // Gamepad right stick X axis
    GAMEPAD_AXIS_RIGHT_Y = 3,     // Gamepad right stick Y axis
    GAMEPAD_AXIS_LEFT_TRIGGER = 4,     // Gamepad back trigger left, pressure level: [1..-1]
    GAMEPAD_AXIS_RIGHT_TRIGGER = 5      // Gamepad back trigger right, pressure level: [1..-1]
} GamepadAxis;

// Material map index
typedef enum {
    MATERIAL_MAP_ALBEDO = 0,        // Albedo material (same as: MATERIAL_MAP_DIFFUSE)
    MATERIAL_MAP_METALNESS,         // Metalness material (same as: MATERIAL_MAP_SPECULAR)
    MATERIAL_MAP_NORMAL,            // Normal material
    MATERIAL_MAP_ROUGHNESS,         // Roughness material
    MATERIAL_MAP_OCCLUSION,         // Ambient occlusion material
    MATERIAL_MAP_EMISSION,          // Emission material
    MATERIAL_MAP_HEIGHT,            // Heightmap material
    MATERIAL_MAP_CUBEMAP,           // Cubemap material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    MATERIAL_MAP_IRRADIANCE,        // Irradiance material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    MATERIAL_MAP_PREFILTER,         // Prefilter material (NOTE: Uses GL_TEXTURE_CUBE_MAP)
    MATERIAL_MAP_BRDF               // Brdf material
} MaterialMapIndex;

#define MATERIAL_MAP_DIFFUSE      MATERIAL_MAP_ALBEDO
#define MATERIAL_MAP_SPECULAR     MATERIAL_MAP_METALNESS

// Shader location index
typedef enum {
    SHADER_LOC_VERTEX_POSITION = 0, // Shader location: vertex attribute: position
    SHADER_LOC_VERTEX_TEXCOORD01,   // Shader location: vertex attribute: texcoord01
    SHADER_LOC_VERTEX_TEXCOORD02,   // Shader location: vertex attribute: texcoord02
    SHADER_LOC_VERTEX_NORMAL,       // Shader location: vertex attribute: normal
    SHADER_LOC_VERTEX_TANGENT,      // Shader location: vertex attribute: tangent
    SHADER_LOC_VERTEX_COLOR,        // Shader location: vertex attribute: color
    SHADER_LOC_MATRIX_MVP,          // Shader location: matrix uniform: model-view-projection
    SHADER_LOC_MATRIX_VIEW,         // Shader location: matrix uniform: view (camera transform)
    SHADER_LOC_MATRIX_PROJECTION,   // Shader location: matrix uniform: projection
    SHADER_LOC_MATRIX_MODEL,        // Shader location: matrix uniform: model (transform)
    SHADER_LOC_MATRIX_NORMAL,       // Shader location: matrix uniform: normal
    SHADER_LOC_VECTOR_VIEW,         // Shader location: vector uniform: view
    SHADER_LOC_COLOR_DIFFUSE,       // Shader location: vector uniform: diffuse color
    SHADER_LOC_COLOR_SPECULAR,      // Shader location: vector uniform: specular color
    SHADER_LOC_COLOR_AMBIENT,       // Shader location: vector uniform: ambient color
    SHADER_LOC_MAP_ALBEDO,          // Shader location: sampler2d texture: albedo (same as: SHADER_LOC_MAP_DIFFUSE)
    SHADER_LOC_MAP_METALNESS,       // Shader location: sampler2d texture: metalness (same as: SHADER_LOC_MAP_SPECULAR)
    SHADER_LOC_MAP_NORMAL,          // Shader location: sampler2d texture: normal
    SHADER_LOC_MAP_ROUGHNESS,       // Shader location: sampler2d texture: roughness
    SHADER_LOC_MAP_OCCLUSION,       // Shader location: sampler2d texture: occlusion
    SHADER_LOC_MAP_EMISSION,        // Shader location: sampler2d texture: emission
    SHADER_LOC_MAP_HEIGHT,          // Shader location: sampler2d texture: height
    SHADER_LOC_MAP_CUBEMAP,         // Shader location: samplerCube texture: cubemap
    SHADER_LOC_MAP_IRRADIANCE,      // Shader location: samplerCube texture: irradiance
    SHADER_LOC_MAP_PREFILTER,       // Shader location: samplerCube texture: prefilter
    SHADER_LOC_MAP_BRDF             // Shader location: sampler2d texture: brdf
} ShaderLocationIndex;

#define SHADER_LOC_MAP_DIFFUSE      SHADER_LOC_MAP_ALBEDO
#define SHADER_LOC_MAP_SPECULAR     SHADER_LOC_MAP_METALNESS

// Shader uniform data type
typedef enum {
    SHADER_UNIFORM_FLOAT = 0,       // Shader uniform type: float
    SHADER_UNIFORM_VEC2,            // Shader uniform type: vec2 (2 float)
    SHADER_UNIFORM_VEC3,            // Shader uniform type: vec3 (3 float)
    SHADER_UNIFORM_VEC4,            // Shader uniform type: vec4 (4 float)
    SHADER_UNIFORM_INT,             // Shader uniform type: int
    SHADER_UNIFORM_IVEC2,           // Shader uniform type: ivec2 (2 int)
    SHADER_UNIFORM_IVEC3,           // Shader uniform type: ivec3 (3 int)
    SHADER_UNIFORM_IVEC4,           // Shader uniform type: ivec4 (4 int)
    SHADER_UNIFORM_SAMPLER2D        // Shader uniform type: sampler2d
} ShaderUniformDataType;

// Shader attribute data types
typedef enum {
    SHADER_ATTRIB_FLOAT = 0,        // Shader attribute type: float
    SHADER_ATTRIB_VEC2,             // Shader attribute type: vec2 (2 float)
    SHADER_ATTRIB_VEC3,             // Shader attribute type: vec3 (3 float)
    SHADER_ATTRIB_VEC4              // Shader attribute type: vec4 (4 float)
} ShaderAttributeDataType;

// Pixel formats
// NOTE: Support depends on OpenGL version and platform
typedef enum {
    PIXELFORMAT_UNCOMPRESSED_GRAYSCALE = 1, // 8 bit per pixel (no alpha)
    PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA,    // 8*2 bpp (2 channels)
    PIXELFORMAT_UNCOMPRESSED_R5G6B5,        // 16 bpp
    PIXELFORMAT_UNCOMPRESSED_R8G8B8,        // 24 bpp
    PIXELFORMAT_UNCOMPRESSED_R5G5B5A1,      // 16 bpp (1 bit alpha)
    PIXELFORMAT_UNCOMPRESSED_R4G4B4A4,      // 16 bpp (4 bit alpha)
    PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,      // 32 bpp
    PIXELFORMAT_UNCOMPRESSED_R32,           // 32 bpp (1 channel - float)
    PIXELFORMAT_UNCOMPRESSED_R32G32B32,     // 32*3 bpp (3 channels - float)
    PIXELFORMAT_UNCOMPRESSED_R32G32B32A32,  // 32*4 bpp (4 channels - float)
    PIXELFORMAT_UNCOMPRESSED_R16,           // 16 bpp (1 channel - half float)
    PIXELFORMAT_UNCOMPRESSED_R16G16B16,     // 16*3 bpp (3 channels - half float)
    PIXELFORMAT_UNCOMPRESSED_R16G16B16A16,  // 16*4 bpp (4 channels - half float)
    PIXELFORMAT_COMPRESSED_DXT1_RGB,        // 4 bpp (no alpha)
    PIXELFORMAT_COMPRESSED_DXT1_RGBA,       // 4 bpp (1 bit alpha)
    PIXELFORMAT_COMPRESSED_DXT3_RGBA,       // 8 bpp
    PIXELFORMAT_COMPRESSED_DXT5_RGBA,       // 8 bpp
    PIXELFORMAT_COMPRESSED_ETC1_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_ETC2_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA,   // 8 bpp
    PIXELFORMAT_COMPRESSED_PVRT_RGB,        // 4 bpp
    PIXELFORMAT_COMPRESSED_PVRT_RGBA,       // 4 bpp
    PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA,   // 8 bpp
    PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA    // 2 bpp
} PixelFormat;

// Texture parameters: filter mode
// NOTE 1: Filtering considers mipmaps if available in the texture
// NOTE 2: Filter is accordingly set for minification and magnification
typedef enum {
    TEXTURE_FILTER_POINT = 0,               // No filter, just pixel approximation
    TEXTURE_FILTER_BILINEAR,                // Linear filtering
    TEXTURE_FILTER_TRILINEAR,               // Trilinear filtering (linear with mipmaps)
    TEXTURE_FILTER_ANISOTROPIC_4X,          // Anisotropic filtering 4x
    TEXTURE_FILTER_ANISOTROPIC_8X,          // Anisotropic filtering 8x
    TEXTURE_FILTER_ANISOTROPIC_16X,         // Anisotropic filtering 16x
} TextureFilter;

// Texture parameters: wrap mode
typedef enum {
    TEXTURE_WRAP_REPEAT = 0,                // Repeats texture in tiled mode
    TEXTURE_WRAP_CLAMP,                     // Clamps texture to edge pixel in tiled mode
    TEXTURE_WRAP_MIRROR_REPEAT,             // Mirrors and repeats the texture in tiled mode
    TEXTURE_WRAP_MIRROR_CLAMP               // Mirrors and clamps to border the texture in tiled mode
} TextureWrap;

// Cubemap layouts
typedef enum {
    CUBEMAP_LAYOUT_AUTO_DETECT = 0,         // Automatically detect layout type
    CUBEMAP_LAYOUT_LINE_VERTICAL,           // Layout is defined by a vertical line with faces
    CUBEMAP_LAYOUT_LINE_HORIZONTAL,         // Layout is defined by a horizontal line with faces
    CUBEMAP_LAYOUT_CROSS_THREE_BY_FOUR,     // Layout is defined by a 3x4 cross with cubemap faces
    CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE,     // Layout is defined by a 4x3 cross with cubemap faces
    CUBEMAP_LAYOUT_PANORAMA                 // Layout is defined by a panorama image (equirrectangular map)
} CubemapLayout;

// Font type, defines generation method
typedef enum {
    FONT_DEFAULT = 0,               // Default font generation, anti-aliased
    FONT_BITMAP,                    // Bitmap font generation, no anti-aliasing
    FONT_SDF                        // SDF font generation, requires external shader
} FontType;

// Color blending modes (pre-defined)
typedef enum {
    BLEND_ALPHA = 0,                // Blend textures considering alpha (default)
    BLEND_ADDITIVE,                 // Blend textures adding colors
    BLEND_MULTIPLIED,               // Blend textures multiplying colors
    BLEND_ADD_COLORS,               // Blend textures adding colors (alternative)
    BLEND_SUBTRACT_COLORS,          // Blend textures subtracting colors (alternative)
    BLEND_ALPHA_PREMULTIPLY,        // Blend premultiplied textures considering alpha
    BLEND_CUSTOM,                   // Blend textures using custom src/dst factors (use rlSetBlendFactors())
    BLEND_CUSTOM_SEPARATE           // Blend textures using custom rgb/alpha separate src/dst factors (use rlSetBlendFactorsSeparate())
} BlendMode;

// Gesture
// NOTE: Provided as bit-wise flags to enable only desired gestures
typedef enum {
    GESTURE_NONE = 0,        // No gesture
    GESTURE_TAP = 1,        // Tap gesture
    GESTURE_DOUBLETAP = 2,        // Double tap gesture
    GESTURE_HOLD = 4,        // Hold gesture
    GESTURE_DRAG = 8,        // Drag gesture
    GESTURE_SWIPE_RIGHT = 16,       // Swipe right gesture
    GESTURE_SWIPE_LEFT = 32,       // Swipe left gesture
    GESTURE_SWIPE_UP = 64,       // Swipe up gesture
    GESTURE_SWIPE_DOWN = 128,      // Swipe down gesture
    GESTURE_PINCH_IN = 256,      // Pinch in gesture
    GESTURE_PINCH_OUT = 512       // Pinch out gesture
} Gesture;

// Camera system modes
typedef enum {
    CAMERA_CUSTOM = 0,              // Custom camera
    CAMERA_FREE,                    // Free camera
    CAMERA_ORBITAL,                 // Orbital camera
    CAMERA_FIRST_PERSON,            // First person camera
    CAMERA_THIRD_PERSON             // Third person camera
} CameraMode;

// Camera projection
typedef enum {
    CAMERA_PERSPECTIVE = 0,         // Perspective projection
    CAMERA_ORTHOGRAPHIC             // Orthographic projection
} CameraProjection;

// N-patch layout
typedef enum {
    NPATCH_NINE_PATCH = 0,          // Npatch layout: 3x3 tiles
    NPATCH_THREE_PATCH_VERTICAL,    // Npatch layout: 1x3 tiles
    NPATCH_THREE_PATCH_HORIZONTAL   // Npatch layout: 3x1 tiles
} NPatchLayout;

// Callbacks to hook some internal functions
// WARNING: These callbacks are intended for advance users
typedef void (*TraceLogCallback)(int logLevel, const char* text, va_list args);  // Logging: Redirect trace log messages
typedef unsigned char* (*LoadFileDataCallback)(const char* fileName, int* dataSize);    // FileIO: Load binary data
typedef bool (*SaveFileDataCallback)(const char* fileName, void* data, int dataSize);   // FileIO: Save binary data
typedef char* (*LoadFileTextCallback)(const char* fileName);            // FileIO: Load text data
typedef bool (*SaveFileTextCallback)(const char* fileName, char* text); // FileIO: Save text data

//------------------------------------------------------------------------------------
// Global Variables Definition
//------------------------------------------------------------------------------------
// It's lonely here...

//------------------------------------------------------------------------------------
// Window and Graphics Device Functions (Module: core)
//------------------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

    // Window-related functions
    FRLAPI void InitWindow(int width, int height, const char* title);  // Initialize window and OpenGL context
    FRLAPI void CloseWindow(void);                                     // Close window and unload OpenGL context
    FRLAPI bool WindowShouldClose(void);                               // Check if application should close (KEY_ESCAPE pressed or windows close icon clicked)
    FRLAPI bool IsWindowReady(void);                                   // Check if window has been initialized successfully
    FRLAPI bool IsWindowFullscreen(void);                              // Check if window is currently fullscreen
    FRLAPI bool IsWindowHidden(void);                                  // Check if window is currently hidden (only PLATFORM_DESKTOP)
    FRLAPI bool IsWindowMinimized(void);                               // Check if window is currently minimized (only PLATFORM_DESKTOP)
    FRLAPI bool IsWindowMaximized(void);                               // Check if window is currently maximized (only PLATFORM_DESKTOP)
    FRLAPI bool IsWindowFocused(void);                                 // Check if window is currently focused (only PLATFORM_DESKTOP)
    FRLAPI bool IsWindowResized(void);                                 // Check if window has been resized last frame
    FRLAPI bool IsWindowState(unsigned int flag);                      // Check if one specific window flag is enabled
    FRLAPI void SetWindowState(unsigned int flags);                    // Set window configuration state using flags (only PLATFORM_DESKTOP)
    FRLAPI void ClearWindowState(unsigned int flags);                  // Clear window configuration state flags
    FRLAPI void ToggleFullscreen(void);                                // Toggle window state: fullscreen/windowed (only PLATFORM_DESKTOP)
    FRLAPI void ToggleBorderlessWindowed(void);                        // Toggle window state: borderless windowed (only PLATFORM_DESKTOP)
    FRLAPI void MaximizeWindow(void);                                  // Set window state: maximized, if resizable (only PLATFORM_DESKTOP)
    FRLAPI void MinimizeWindow(void);                                  // Set window state: minimized, if resizable (only PLATFORM_DESKTOP)
    FRLAPI void RestoreWindow(void);                                   // Set window state: not minimized/maximized (only PLATFORM_DESKTOP)
    FRLAPI void SetWindowIcon(Image image);                            // Set icon for window (single image, RGBA 32bit, only PLATFORM_DESKTOP)
    FRLAPI void SetWindowIcons(Image* images, int count);              // Set icon for window (multiple images, RGBA 32bit, only PLATFORM_DESKTOP)
    FRLAPI void SetWindowTitle(const char* title);                     // Set title for window (only PLATFORM_DESKTOP and PLATFORM_WEB)
    FRLAPI void SetWindowPosition(int x, int y);                       // Set window position on screen (only PLATFORM_DESKTOP)
    FRLAPI void SetWindowMonitor(int monitor);                         // Set monitor for the current window
    FRLAPI void SetWindowMinSize(int width, int height);               // Set window minimum dimensions (for FLAG_WINDOW_RESIZABLE)
    FRLAPI void SetWindowMaxSize(int width, int height);               // Set window maximum dimensions (for FLAG_WINDOW_RESIZABLE)
    FRLAPI void SetWindowSize(int width, int height);                  // Set window dimensions
    FRLAPI void SetWindowOpacity(float opacity);                       // Set window opacity [0.0f..1.0f] (only PLATFORM_DESKTOP)
    FRLAPI void SetWindowFocused(void);                                // Set window focused (only PLATFORM_DESKTOP)
    FRLAPI void* GetWindowHandle(void);                                // Get native window handle
    FRLAPI int GetScreenWidth(void);                                   // Get current screen width
    FRLAPI int GetScreenHeight(void);                                  // Get current screen height
    FRLAPI int GetRenderWidth(void);                                   // Get current render width (it considers HiDPI)
    FRLAPI int GetRenderHeight(void);                                  // Get current render height (it considers HiDPI)
    FRLAPI int GetMonitorCount(void);                                  // Get number of connected monitors
    FRLAPI int GetCurrentMonitor(void);                                // Get current connected monitor
    FRLAPI Vector2 GetMonitorPosition(int monitor);                    // Get specified monitor position
    FRLAPI int GetMonitorWidth(int monitor);                           // Get specified monitor width (current video mode used by monitor)
    FRLAPI int GetMonitorHeight(int monitor);                          // Get specified monitor height (current video mode used by monitor)
    FRLAPI int GetMonitorPhysicalWidth(int monitor);                   // Get specified monitor physical width in millimetres
    FRLAPI int GetMonitorPhysicalHeight(int monitor);                  // Get specified monitor physical height in millimetres
    FRLAPI int GetMonitorRefreshRate(int monitor);                     // Get specified monitor refresh rate
    FRLAPI Vector2 GetWindowPosition(void);                            // Get window position XY on monitor
    FRLAPI Vector2 GetWindowScaleDPI(void);                            // Get window scale DPI factor
    FRLAPI const char* GetMonitorName(int monitor);                    // Get the human-readable, UTF-8 encoded name of the specified monitor
    FRLAPI void SetClipboardText(const char* text);                    // Set clipboard text content
    FRLAPI const char* GetClipboardText(void);                         // Get clipboard text content
    FRLAPI void EnableEventWaiting(void);                              // Enable waiting for events on EndDrawing(), no automatic event polling
    FRLAPI void DisableEventWaiting(void);                             // Disable waiting for events on EndDrawing(), automatic events polling

    // Cursor-related functions
    FRLAPI void ShowCursor(void);                                      // Shows cursor
    FRLAPI void HideCursor(void);                                      // Hides cursor
    FRLAPI bool IsCursorHidden(void);                                  // Check if cursor is not visible
    FRLAPI void EnableCursor(void);                                    // Enables cursor (unlock cursor)
    FRLAPI void DisableCursor(void);                                   // Disables cursor (lock cursor)
    FRLAPI bool IsCursorOnScreen(void);                                // Check if cursor is on the screen

    // Drawing-related functions
    FRLAPI void ClearBackground(Color color);                          // Set background color (framebuffer clear color)
    FRLAPI void BeginDrawing(void);                                    // Setup canvas (framebuffer) to start drawing
    FRLAPI void EndDrawing(void);                                      // End canvas drawing and swap buffers (double buffering)
    FRLAPI void BeginMode2D(Camera2D camera);                          // Begin 2D mode with custom camera (2D)
    FRLAPI void EndMode2D(void);                                       // Ends 2D mode with custom camera
    FRLAPI void BeginMode3D(Camera3D camera);                          // Begin 3D mode with custom camera (3D)
    FRLAPI void EndMode3D(void);                                       // Ends 3D mode and returns to default 2D orthographic mode
    FRLAPI void BeginTextureMode(RenderTexture2D target);              // Begin drawing to render texture
    FRLAPI void EndTextureMode(void);                                  // Ends drawing to render texture
    FRLAPI void BeginShaderMode(Shader shader);                        // Begin custom shader drawing
    FRLAPI void EndShaderMode(void);                                   // End custom shader drawing (use default shader)
    FRLAPI void BeginBlendMode(int mode);                              // Begin blending mode (alpha, additive, multiplied, subtract, custom)
    FRLAPI void EndBlendMode(void);                                    // End blending mode (reset to default: alpha blending)
    FRLAPI void BeginScissorMode(int x, int y, int width, int height); // Begin scissor mode (define screen area for following drawing)
    FRLAPI void EndScissorMode(void);                                  // End scissor mode
    FRLAPI void BeginVrStereoMode(VrStereoConfig config);              // Begin stereo rendering (requires VR simulator)
    FRLAPI void EndVrStereoMode(void);                                 // End stereo rendering (requires VR simulator)

    // VR stereo config functions for VR simulator
    FRLAPI VrStereoConfig LoadVrStereoConfig(VrDeviceInfo device);     // Load VR stereo config for VR simulator device parameters
    FRLAPI void UnloadVrStereoConfig(VrStereoConfig config);           // Unload VR stereo config

    // Shader management functions
    // NOTE: Shader functionality is not available on OpenGL 1.1
    FRLAPI Shader LoadShader(const char* vsFileName, const char* fsFileName);   // Load shader from files and bind default locations
    FRLAPI Shader LoadShaderFromMemory(const char* vsCode, const char* fsCode); // Load shader from code strings and bind default locations
    FRLAPI bool IsShaderReady(Shader shader);                                   // Check if a shader is ready
    FRLAPI int GetShaderLocation(Shader shader, const char* uniformName);       // Get shader uniform location
    FRLAPI int GetShaderLocationAttrib(Shader shader, const char* attribName);  // Get shader attribute location
    FRLAPI void SetShaderValue(Shader shader, int locIndex, const void* value, int uniformType);               // Set shader uniform value
    FRLAPI void SetShaderValueV(Shader shader, int locIndex, const void* value, int uniformType, int count);   // Set shader uniform value vector
    FRLAPI void SetShaderValueMatrix(Shader shader, int locIndex, Matrix mat);         // Set shader uniform value (matrix 4x4)
    FRLAPI void SetShaderValueTexture(Shader shader, int locIndex, Texture2D texture); // Set shader uniform value for texture (sampler2d)
    FRLAPI void UnloadShader(Shader shader);                                    // Unload shader from GPU memory (VRAM)

    // Screen-space-related functions
    FRLAPI Ray GetMouseRay(Vector2 mousePosition, Camera camera);      // Get a ray trace from mouse position
    FRLAPI Matrix GetCameraMatrix(Camera camera);                      // Get camera transform matrix (view matrix)
    FRLAPI Matrix GetCameraMatrix2D(Camera2D camera);                  // Get camera 2d transform matrix
    FRLAPI Vector2 GetWorldToScreen(Vector3 position, Camera camera);  // Get the screen space position for a 3d world space position
    FRLAPI Vector2 GetScreenToWorld2D(Vector2 position, Camera2D camera); // Get the world space position for a 2d camera screen space position
    FRLAPI Vector2 GetWorldToScreenEx(Vector3 position, Camera camera, int width, int height); // Get size position for a 3d world space position
    FRLAPI Vector2 GetWorldToScreen2D(Vector2 position, Camera2D camera); // Get the screen space position for a 2d camera world space position

    // Timing-related functions
    FRLAPI void SetTargetFPS(int fps);                                 // Set target FPS (maximum)
    FRLAPI float GetFrameTime(void);                                   // Get time in seconds for last frame drawn (delta time)
    FRLAPI double GetTime(void);                                       // Get elapsed time in seconds since InitWindow()
    FRLAPI int GetFPS(void);                                           // Get current FPS

    // Custom frame control functions
    // NOTE: Those functions are intended for advance users that want full control over the frame processing
    // By default EndDrawing() does this job: draws everything + SwapScreenBuffer() + manage frame timing + PollInputEvents()
    // To avoid that behaviour and control frame processes manually, enable in config.h: SUPPORT_CUSTOM_FRAME_CONTROL
    FRLAPI void SwapScreenBuffer(void);                                // Swap back buffer with front buffer (screen drawing)
    FRLAPI void PollInputEvents(void);                                 // Register all input events
    FRLAPI void WaitTime(double seconds);                              // Wait for some time (halt program execution)

    // Random values generation functions
    FRLAPI void SetRandomSeed(unsigned int seed);                      // Set the seed for the random number generator
    FRLAPI int GetRandomValue(int min, int max);                       // Get a random value between min and max (both included)
    FRLAPI int* LoadRandomSequence(unsigned int count, int min, int max); // Load random values sequence, no values repeated
    FRLAPI void UnloadRandomSequence(int* sequence);                   // Unload random values sequence

    // Misc. functions
    FRLAPI void TakeScreenshot(const char* fileName);                  // Takes a screenshot of current screen (filename extension defines format)
    FRLAPI void SetConfigFlags(unsigned int flags);                    // Setup init configuration flags (view FLAGS)
    FRLAPI void OpenURL(const char* url);                              // Open URL with default system browser (if available)

    // NOTE: Following functions implemented in module [utils]
    //------------------------------------------------------------------
    FRLAPI void TraceLog(int logLevel, const char* text, ...);         // Show trace log messages (LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR...)
    FRLAPI void SetTraceLogLevel(int logLevel);                        // Set the current threshold (minimum) log level
    FRLAPI void* MemAlloc(unsigned int size);                          // Internal memory allocator
    FRLAPI void* MemRealloc(void* ptr, unsigned int size);             // Internal memory reallocator
    FRLAPI void MemFree(void* ptr);                                    // Internal memory free

    // Set custom callbacks
    // WARNING: Callbacks setup is intended for advance users
    FRLAPI void SetTraceLogCallback(TraceLogCallback callback);         // Set custom trace log
    FRLAPI void SetLoadFileDataCallback(LoadFileDataCallback callback); // Set custom file binary data loader
    FRLAPI void SetSaveFileDataCallback(SaveFileDataCallback callback); // Set custom file binary data saver
    FRLAPI void SetLoadFileTextCallback(LoadFileTextCallback callback); // Set custom file text data loader
    FRLAPI void SetSaveFileTextCallback(SaveFileTextCallback callback); // Set custom file text data saver

    // Files management functions
    FRLAPI unsigned char* LoadFileData(const char* fileName, int* dataSize); // Load file data as byte array (read)
    FRLAPI void UnloadFileData(unsigned char* data);                   // Unload file data allocated by LoadFileData()
    FRLAPI bool SaveFileData(const char* fileName, void* data, int dataSize); // Save data to file from byte array (write), returns true on success
    FRLAPI bool ExportDataAsCode(const unsigned char* data, int dataSize, const char* fileName); // Export data to code (.h), returns true on success
    FRLAPI char* LoadFileText(const char* fileName);                   // Load text data from file (read), returns a '\0' terminated string
    FRLAPI void UnloadFileText(char* text);                            // Unload file text data allocated by LoadFileText()
    FRLAPI bool SaveFileText(const char* fileName, char* text);        // Save text data to file (write), string must be '\0' terminated, returns true on success
    //------------------------------------------------------------------

    // File system functions
    FRLAPI bool FileExists(const char* fileName);                      // Check if file exists
    FRLAPI bool DirectoryExists(const char* dirPath);                  // Check if a directory path exists
    FRLAPI bool IsFileExtension(const char* fileName, const char* ext); // Check file extension (including point: .png, .wav)
    FRLAPI int GetFileLength(const char* fileName);                    // Get file length in bytes (NOTE: GetFileSize() conflicts with windows.h)
    FRLAPI const char* GetFileExtension(const char* fileName);         // Get pointer to extension for a filename string (includes dot: '.png')
    FRLAPI const char* GetFileName(const char* filePath);              // Get pointer to filename for a path string
    FRLAPI const char* GetFileNameWithoutExt(const char* filePath);    // Get filename string without extension (uses static string)
    FRLAPI const char* GetDirectoryPath(const char* filePath);         // Get full path for a given fileName with path (uses static string)
    FRLAPI const char* GetPrevDirectoryPath(const char* dirPath);      // Get previous directory path for a given path (uses static string)
    FRLAPI const char* GetWorkingDirectory(void);                      // Get current working directory (uses static string)
    FRLAPI const char* GetApplicationDirectory(void);                  // Get the directory of the running application (uses static string)
    FRLAPI bool ChangeDirectory(const char* dir);                      // Change working directory, return true on success
    FRLAPI bool IsPathFile(const char* path);                          // Check if a given path is a file or a directory
    FRLAPI FilePathList LoadDirectoryFiles(const char* dirPath);       // Load directory filepaths
    FRLAPI FilePathList LoadDirectoryFilesEx(const char* basePath, const char* filter, bool scanSubdirs); // Load directory filepaths with extension filtering and recursive directory scan
    FRLAPI void UnloadDirectoryFiles(FilePathList files);              // Unload filepaths
    FRLAPI bool IsFileDropped(void);                                   // Check if a file has been dropped into window
    FRLAPI FilePathList LoadDroppedFiles(void);                        // Load dropped filepaths
    FRLAPI void UnloadDroppedFiles(FilePathList files);                // Unload dropped filepaths
    FRLAPI long GetFileModTime(const char* fileName);                  // Get file modification time (last write time)

    // Compression/Encoding functionality
    FRLAPI unsigned char* CompressData(const unsigned char* data, int dataSize, int* compDataSize);        // Compress data (DEFLATE algorithm), memory must be MemFree()
    FRLAPI unsigned char* DecompressData(const unsigned char* compData, int compDataSize, int* dataSize);  // Decompress data (DEFLATE algorithm), memory must be MemFree()
    FRLAPI char* EncodeDataBase64(const unsigned char* data, int dataSize, int* outputSize);               // Encode data to Base64 string, memory must be MemFree()
    FRLAPI unsigned char* DecodeDataBase64(const unsigned char* data, int* outputSize);                    // Decode Base64 string data, memory must be MemFree()

    // Automation events functionality
    FRLAPI AutomationEventList LoadAutomationEventList(const char* fileName);                // Load automation events list from file, NULL for empty list, capacity = MAX_AUTOMATION_EVENTS
    FRLAPI void UnloadAutomationEventList(AutomationEventList* list);                        // Unload automation events list from file
    FRLAPI bool ExportAutomationEventList(AutomationEventList list, const char* fileName);   // Export automation events list as text file
    FRLAPI void SetAutomationEventList(AutomationEventList* list);                           // Set automation event list to record to
    FRLAPI void SetAutomationEventBaseFrame(int frame);                                      // Set automation event internal base frame to start recording
    FRLAPI void StartAutomationEventRecording(void);                                         // Start recording automation events (AutomationEventList must be set)
    FRLAPI void StopAutomationEventRecording(void);                                          // Stop recording automation events
    FRLAPI void PlayAutomationEvent(AutomationEvent event);                                  // Play a recorded automation event

    //------------------------------------------------------------------------------------
    // Input Handling Functions (Module: core)
    //------------------------------------------------------------------------------------

    // Input-related functions: keyboard
    FRLAPI bool IsKeyPressed(int key);                             // Check if a key has been pressed once
    FRLAPI bool IsKeyPressedRepeat(int key);                       // Check if a key has been pressed again (Only PLATFORM_DESKTOP)
    FRLAPI bool IsKeyDown(int key);                                // Check if a key is being pressed
    FRLAPI bool IsKeyReleased(int key);                            // Check if a key has been released once
    FRLAPI bool IsKeyUp(int key);                                  // Check if a key is NOT being pressed
    FRLAPI int GetKeyPressed(void);                                // Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
    FRLAPI int GetCharPressed(void);                               // Get char pressed (unicode), call it multiple times for chars queued, returns 0 when the queue is empty
    FRLAPI void SetExitKey(int key);                               // Set a custom key to exit program (default is ESC)

    // Input-related functions: gamepads
    FRLAPI bool IsGamepadAvailable(int gamepad);                   // Check if a gamepad is available
    FRLAPI const char* GetGamepadName(int gamepad);                // Get gamepad internal name id
    FRLAPI bool IsGamepadButtonPressed(int gamepad, int button);   // Check if a gamepad button has been pressed once
    FRLAPI bool IsGamepadButtonDown(int gamepad, int button);      // Check if a gamepad button is being pressed
    FRLAPI bool IsGamepadButtonReleased(int gamepad, int button);  // Check if a gamepad button has been released once
    FRLAPI bool IsGamepadButtonUp(int gamepad, int button);        // Check if a gamepad button is NOT being pressed
    FRLAPI int GetGamepadButtonPressed(void);                      // Get the last gamepad button pressed
    FRLAPI int GetGamepadAxisCount(int gamepad);                   // Get gamepad axis count for a gamepad
    FRLAPI float GetGamepadAxisMovement(int gamepad, int axis);    // Get axis movement value for a gamepad axis
    FRLAPI int SetGamepadMappings(const char* mappings);           // Set internal gamepad mappings (SDL_GameControllerDB)

    // Input-related functions: mouse
    FRLAPI bool IsMouseButtonPressed(int button);                  // Check if a mouse button has been pressed once
    FRLAPI bool IsMouseButtonDown(int button);                     // Check if a mouse button is being pressed
    FRLAPI bool IsMouseButtonReleased(int button);                 // Check if a mouse button has been released once
    FRLAPI bool IsMouseButtonUp(int button);                       // Check if a mouse button is NOT being pressed
    FRLAPI int GetMouseX(void);                                    // Get mouse position X
    FRLAPI int GetMouseY(void);                                    // Get mouse position Y
    FRLAPI Vector2 GetMousePosition(void);                         // Get mouse position XY
    FRLAPI Vector2 GetMouseDelta(void);                            // Get mouse delta between frames
    FRLAPI void SetMousePosition(int x, int y);                    // Set mouse position XY
    FRLAPI void SetMouseOffset(int offsetX, int offsetY);          // Set mouse offset
    FRLAPI void SetMouseScale(float scaleX, float scaleY);         // Set mouse scaling
    FRLAPI float GetMouseWheelMove(void);                          // Get mouse wheel movement for X or Y, whichever is larger
    FRLAPI Vector2 GetMouseWheelMoveV(void);                       // Get mouse wheel movement for both X and Y
    FRLAPI void SetMouseCursor(int cursor);                        // Set mouse cursor

    // Input-related functions: touch
    FRLAPI int GetTouchX(void);                                    // Get touch position X for touch point 0 (relative to screen size)
    FRLAPI int GetTouchY(void);                                    // Get touch position Y for touch point 0 (relative to screen size)
    FRLAPI Vector2 GetTouchPosition(int index);                    // Get touch position XY for a touch point index (relative to screen size)
    FRLAPI int GetTouchPointId(int index);                         // Get touch point identifier for given index
    FRLAPI int GetTouchPointCount(void);                           // Get number of touch points

    //------------------------------------------------------------------------------------
    // Gestures and Touch Handling Functions (Module: rgestures)
    //------------------------------------------------------------------------------------
    FRLAPI void SetGesturesEnabled(unsigned int flags);      // Enable a set of gestures using flags
    FRLAPI bool IsGestureDetected(unsigned int gesture);     // Check if a gesture have been detected
    FRLAPI int GetGestureDetected(void);                     // Get latest detected gesture
    FRLAPI float GetGestureHoldDuration(void);               // Get gesture hold time in milliseconds
    FRLAPI Vector2 GetGestureDragVector(void);               // Get gesture drag vector
    FRLAPI float GetGestureDragAngle(void);                  // Get gesture drag angle
    FRLAPI Vector2 GetGesturePinchVector(void);              // Get gesture pinch delta
    FRLAPI float GetGesturePinchAngle(void);                 // Get gesture pinch angle

    //------------------------------------------------------------------------------------
    // Camera System Functions (Module: rcamera)
    //------------------------------------------------------------------------------------
    FRLAPI void UpdateCamera(Camera* camera, int mode);      // Update camera position for selected mode
    FRLAPI void UpdateCameraPro(Camera* camera, Vector3 movement, Vector3 rotation, float zoom); // Update camera movement/rotation

    //------------------------------------------------------------------------------------
    // Basic Shapes Drawing Functions (Module: shapes)
    //------------------------------------------------------------------------------------
    // Set texture and rectangle to be used on shapes drawing
    // NOTE: It can be useful when using basic shapes and one single font,
    // defining a font char white rectangle would allow drawing everything in a single draw call
    FRLAPI void SetShapesTexture(Texture2D texture, Rectangle source);       // Set texture and rectangle to be used on shapes drawing

    // Basic shapes drawing functions
    FRLAPI void DrawPixel(int posX, int posY, Color color);                                                   // Draw a pixel
    FRLAPI void DrawPixelV(Vector2 position, Color color);                                                    // Draw a pixel (Vector version)
    FRLAPI void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color);                // Draw a line
    FRLAPI void DrawLineV(Vector2 startPos, Vector2 endPos, Color color);                                     // Draw a line (using gl lines)
    FRLAPI void DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color);                       // Draw a line (using triangles/quads)
    FRLAPI void DrawLineStrip(Vector2* points, int pointCount, Color color);                                  // Draw lines sequence (using gl lines)
    FRLAPI void DrawLineBezier(Vector2 startPos, Vector2 endPos, float thick, Color color);                   // Draw line segment cubic-bezier in-out interpolation
    FRLAPI void DrawCircle(int centerX, int centerY, float radius, Color color);                              // Draw a color-filled circle
    FRLAPI void DrawCircleSector(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color);      // Draw a piece of a circle
    FRLAPI void DrawCircleSectorLines(Vector2 center, float radius, float startAngle, float endAngle, int segments, Color color); // Draw circle sector outline
    FRLAPI void DrawCircleGradient(int centerX, int centerY, float radius, Color color1, Color color2);       // Draw a gradient-filled circle
    FRLAPI void DrawCircleV(Vector2 center, float radius, Color color);                                       // Draw a color-filled circle (Vector version)
    FRLAPI void DrawCircleLines(int centerX, int centerY, float radius, Color color);                         // Draw circle outline
    FRLAPI void DrawCircleLinesV(Vector2 center, float radius, Color color);                                  // Draw circle outline (Vector version)
    FRLAPI void DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color);             // Draw ellipse
    FRLAPI void DrawEllipseLines(int centerX, int centerY, float radiusH, float radiusV, Color color);        // Draw ellipse outline
    FRLAPI void DrawRing(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color); // Draw ring
    FRLAPI void DrawRingLines(Vector2 center, float innerRadius, float outerRadius, float startAngle, float endAngle, int segments, Color color);    // Draw ring outline
    FRLAPI void DrawRectangle(int posX, int posY, int width, int height, Color color);                        // Draw a color-filled rectangle
    FRLAPI void DrawRectangleV(Vector2 position, Vector2 size, Color color);                                  // Draw a color-filled rectangle (Vector version)
    FRLAPI void DrawRectangleRec(Rectangle rec, Color color);                                                 // Draw a color-filled rectangle
    FRLAPI void DrawRectanglePro(Rectangle rec, Vector2 origin, float rotation, Color color);                 // Draw a color-filled rectangle with pro parameters
    FRLAPI void DrawRectangleGradientV(int posX, int posY, int width, int height, Color color1, Color color2);// Draw a vertical-gradient-filled rectangle
    FRLAPI void DrawRectangleGradientH(int posX, int posY, int width, int height, Color color1, Color color2);// Draw a horizontal-gradient-filled rectangle
    FRLAPI void DrawRectangleGradientEx(Rectangle rec, Color col1, Color col2, Color col3, Color col4);       // Draw a gradient-filled rectangle with custom vertex colors
    FRLAPI void DrawRectangleLines(int posX, int posY, int width, int height, Color color);                   // Draw rectangle outline
    FRLAPI void DrawRectangleLinesEx(Rectangle rec, float lineThick, Color color);                            // Draw rectangle outline with extended parameters
    FRLAPI void DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color);              // Draw rectangle with rounded edges
    FRLAPI void DrawRectangleRoundedLines(Rectangle rec, float roundness, int segments, float lineThick, Color color); // Draw rectangle with rounded edges outline
    FRLAPI void DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color);                                // Draw a color-filled triangle (vertex in counter-clockwise order!)
    FRLAPI void DrawTriangleLines(Vector2 v1, Vector2 v2, Vector2 v3, Color color);                           // Draw triangle outline (vertex in counter-clockwise order!)
    FRLAPI void DrawTriangleFan(Vector2* points, int pointCount, Color color);                                // Draw a triangle fan defined by points (first vertex is the center)
    FRLAPI void DrawTriangleStrip(Vector2* points, int pointCount, Color color);                              // Draw a triangle strip defined by points
    FRLAPI void DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color);               // Draw a regular polygon (Vector version)
    FRLAPI void DrawPolyLines(Vector2 center, int sides, float radius, float rotation, Color color);          // Draw a polygon outline of n sides
    FRLAPI void DrawPolyLinesEx(Vector2 center, int sides, float radius, float rotation, float lineThick, Color color); // Draw a polygon outline of n sides with extended parameters

    // Splines drawing functions
    FRLAPI void DrawSplineLinear(Vector2* points, int pointCount, float thick, Color color);                  // Draw spline: Linear, minimum 2 points
    FRLAPI void DrawSplineBasis(Vector2* points, int pointCount, float thick, Color color);                   // Draw spline: B-Spline, minimum 4 points
    FRLAPI void DrawSplineCatmullRom(Vector2* points, int pointCount, float thick, Color color);              // Draw spline: Catmull-Rom, minimum 4 points
    FRLAPI void DrawSplineBezierQuadratic(Vector2* points, int pointCount, float thick, Color color);         // Draw spline: Quadratic Bezier, minimum 3 points (1 control point): [p1, c2, p3, c4...]
    FRLAPI void DrawSplineBezierCubic(Vector2* points, int pointCount, float thick, Color color);             // Draw spline: Cubic Bezier, minimum 4 points (2 control points): [p1, c2, c3, p4, c5, c6...]
    FRLAPI void DrawSplineSegmentLinear(Vector2 p1, Vector2 p2, float thick, Color color);                    // Draw spline segment: Linear, 2 points
    FRLAPI void DrawSplineSegmentBasis(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float thick, Color color); // Draw spline segment: B-Spline, 4 points
    FRLAPI void DrawSplineSegmentCatmullRom(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float thick, Color color); // Draw spline segment: Catmull-Rom, 4 points
    FRLAPI void DrawSplineSegmentBezierQuadratic(Vector2 p1, Vector2 c2, Vector2 p3, float thick, Color color); // Draw spline segment: Quadratic Bezier, 2 points, 1 control point
    FRLAPI void DrawSplineSegmentBezierCubic(Vector2 p1, Vector2 c2, Vector2 c3, Vector2 p4, float thick, Color color); // Draw spline segment: Cubic Bezier, 2 points, 2 control points

    // Spline segment point evaluation functions, for a given t [0.0f .. 1.0f]
    FRLAPI Vector2 GetSplinePointLinear(Vector2 startPos, Vector2 endPos, float t);                           // Get (evaluate) spline point: Linear
    FRLAPI Vector2 GetSplinePointBasis(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float t);              // Get (evaluate) spline point: B-Spline
    FRLAPI Vector2 GetSplinePointCatmullRom(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float t);         // Get (evaluate) spline point: Catmull-Rom
    FRLAPI Vector2 GetSplinePointBezierQuad(Vector2 p1, Vector2 c2, Vector2 p3, float t);                     // Get (evaluate) spline point: Quadratic Bezier
    FRLAPI Vector2 GetSplinePointBezierCubic(Vector2 p1, Vector2 c2, Vector2 c3, Vector2 p4, float t);        // Get (evaluate) spline point: Cubic Bezier

    // Basic shapes collision detection functions
    FRLAPI bool CheckCollisionRecs(Rectangle rec1, Rectangle rec2);                                           // Check collision between two rectangles
    FRLAPI bool CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2);        // Check collision between two circles
    FRLAPI bool CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec);                         // Check collision between circle and rectangle
    FRLAPI bool CheckCollisionPointRec(Vector2 point, Rectangle rec);                                         // Check if point is inside rectangle
    FRLAPI bool CheckCollisionPointCircle(Vector2 point, Vector2 center, float radius);                       // Check if point is inside circle
    FRLAPI bool CheckCollisionPointTriangle(Vector2 point, Vector2 p1, Vector2 p2, Vector2 p3);               // Check if point is inside a triangle
    FRLAPI bool CheckCollisionPointPoly(Vector2 point, Vector2* points, int pointCount);                      // Check if point is within a polygon described by array of vertices
    FRLAPI bool CheckCollisionLines(Vector2 startPos1, Vector2 endPos1, Vector2 startPos2, Vector2 endPos2, Vector2* collisionPoint); // Check the collision between two lines defined by two points each, returns collision point by reference
    FRLAPI bool CheckCollisionPointLine(Vector2 point, Vector2 p1, Vector2 p2, int threshold);                // Check if point belongs to line created between two points [p1] and [p2] with defined margin in pixels [threshold]
    FRLAPI Rectangle GetCollisionRec(Rectangle rec1, Rectangle rec2);                                         // Get collision rectangle for two rectangles collision

    //------------------------------------------------------------------------------------
    // Texture Loading and Drawing Functions (Module: textures)
    //------------------------------------------------------------------------------------

    // Image loading functions
    // NOTE: These functions do not require GPU access
    FRLAPI Image LoadImage(const char* fileName);                                                             // Load image from file into CPU memory (RAM)
    FRLAPI Image LoadImageRaw(const char* fileName, int width, int height, int format, int headerSize);       // Load image from RAW file data
    FRLAPI Image LoadImageSvg(const char* fileNameOrString, int width, int height);                           // Load image from SVG file data or string with specified size
    FRLAPI Image LoadImageAnim(const char* fileName, int* frames);                                            // Load image sequence from file (frames appended to image.data)
    FRLAPI Image LoadImageFromMemory(const char* fileType, const unsigned char* fileData, int dataSize);      // Load image from memory buffer, fileType refers to extension: i.e. '.png'
    FRLAPI Image LoadImageFromTexture(Texture2D texture);                                                     // Load image from GPU texture data
    FRLAPI Image LoadImageFromScreen(void);                                                                   // Load image from screen buffer and (screenshot)
    FRLAPI bool IsImageReady(Image image);                                                                    // Check if an image is ready
    FRLAPI void UnloadImage(Image image);                                                                     // Unload image from CPU memory (RAM)
    FRLAPI bool ExportImage(Image image, const char* fileName);                                               // Export image data to file, returns true on success
    FRLAPI unsigned char* ExportImageToMemory(Image image, const char* fileType, int* fileSize);              // Export image to memory buffer
    FRLAPI bool ExportImageAsCode(Image image, const char* fileName);                                         // Export image as code file defining an array of bytes, returns true on success

    // Image generation functions
    FRLAPI Image GenImageColor(int width, int height, Color color);                                           // Generate image: plain color
    FRLAPI Image GenImageGradientLinear(int width, int height, int direction, Color start, Color end);        // Generate image: linear gradient, direction in degrees [0..360], 0=Vertical gradient
    FRLAPI Image GenImageGradientRadial(int width, int height, float density, Color inner, Color outer);      // Generate image: radial gradient
    FRLAPI Image GenImageGradientSquare(int width, int height, float density, Color inner, Color outer);      // Generate image: square gradient
    FRLAPI Image GenImageChecked(int width, int height, int checksX, int checksY, Color col1, Color col2);    // Generate image: checked
    FRLAPI Image GenImageWhiteNoise(int width, int height, float factor);                                     // Generate image: white noise
    FRLAPI Image GenImagePerlinNoise(int width, int height, int offsetX, int offsetY, float scale);           // Generate image: perlin noise
    FRLAPI Image GenImageCellular(int width, int height, int tileSize);                                       // Generate image: cellular algorithm, bigger tileSize means bigger cells
    FRLAPI Image GenImageText(int width, int height, const char* text);                                       // Generate image: grayscale image from text data

    // Image manipulation functions
    FRLAPI Image ImageCopy(Image image);                                                                      // Create an image duplicate (useful for transformations)
    FRLAPI Image ImageFromImage(Image image, Rectangle rec);                                                  // Create an image from another image piece
    FRLAPI Image ImageText(const char* text, int fontSize, Color color);                                      // Create an image from text (default font)
    FRLAPI Image ImageTextEx(Font font, const char* text, float fontSize, float spacing, Color tint);         // Create an image from text (custom sprite font)
    FRLAPI void ImageFormat(Image* image, int newFormat);                                                     // Convert image data to desired format
    FRLAPI void ImageToPOT(Image* image, Color fill);                                                         // Convert image to POT (power-of-two)
    FRLAPI void ImageCrop(Image* image, Rectangle crop);                                                      // Crop an image to a defined rectangle
    FRLAPI void ImageAlphaCrop(Image* image, float threshold);                                                // Crop image depending on alpha value
    FRLAPI void ImageAlphaClear(Image* image, Color color, float threshold);                                  // Clear alpha channel to desired color
    FRLAPI void ImageAlphaMask(Image* image, Image alphaMask);                                                // Apply alpha mask to image
    FRLAPI void ImageAlphaPremultiply(Image* image);                                                          // Premultiply alpha channel
    FRLAPI void ImageBlurGaussian(Image* image, int blurSize);                                                // Apply Gaussian blur using a box blur approximation
    FRLAPI void ImageResize(Image* image, int newWidth, int newHeight);                                       // Resize image (Bicubic scaling algorithm)
    FRLAPI void ImageResizeNN(Image* image, int newWidth, int newHeight);                                      // Resize image (Nearest-Neighbor scaling algorithm)
    FRLAPI void ImageResizeCanvas(Image* image, int newWidth, int newHeight, int offsetX, int offsetY, Color fill);  // Resize canvas and fill with color
    FRLAPI void ImageMipmaps(Image* image);                                                                   // Compute all mipmap levels for a provided image
    FRLAPI void ImageDither(Image* image, int rBpp, int gBpp, int bBpp, int aBpp);                            // Dither image data to 16bpp or lower (Floyd-Steinberg dithering)
    FRLAPI void ImageFlipVertical(Image* image);                                                              // Flip image vertically
    FRLAPI void ImageFlipHorizontal(Image* image);                                                            // Flip image horizontally
    FRLAPI void ImageRotate(Image* image, int degrees);                                                       // Rotate image by input angle in degrees (-359 to 359)
    FRLAPI void ImageRotateCW(Image* image);                                                                  // Rotate image clockwise 90deg
    FRLAPI void ImageRotateCCW(Image* image);                                                                 // Rotate image counter-clockwise 90deg
    FRLAPI void ImageColorTint(Image* image, Color color);                                                    // Modify image color: tint
    FRLAPI void ImageColorInvert(Image* image);                                                               // Modify image color: invert
    FRLAPI void ImageColorGrayscale(Image* image);                                                            // Modify image color: grayscale
    FRLAPI void ImageColorContrast(Image* image, float contrast);                                             // Modify image color: contrast (-100 to 100)
    FRLAPI void ImageColorBrightness(Image* image, int brightness);                                           // Modify image color: brightness (-255 to 255)
    FRLAPI void ImageColorReplace(Image* image, Color color, Color replace);                                  // Modify image color: replace color
    FRLAPI Color* LoadImageColors(Image image);                                                               // Load color data from image as a Color array (RGBA - 32bit)
    FRLAPI Color* LoadImagePalette(Image image, int maxPaletteSize, int* colorCount);                         // Load colors palette from image as a Color array (RGBA - 32bit)
    FRLAPI void UnloadImageColors(Color* colors);                                                             // Unload color data loaded with LoadImageColors()
    FRLAPI void UnloadImagePalette(Color* colors);                                                            // Unload colors palette loaded with LoadImagePalette()
    FRLAPI Rectangle GetImageAlphaBorder(Image image, float threshold);                                       // Get image alpha border rectangle
    FRLAPI Color GetImageColor(Image image, int x, int y);                                                    // Get image pixel color at (x, y) position

    // Image drawing functions
    // NOTE: Image software-rendering functions (CPU)
    FRLAPI void ImageClearBackground(Image* dst, Color color);                                                // Clear image background with given color
    FRLAPI void ImageDrawPixel(Image* dst, int posX, int posY, Color color);                                  // Draw pixel within an image
    FRLAPI void ImageDrawPixelV(Image* dst, Vector2 position, Color color);                                   // Draw pixel within an image (Vector version)
    FRLAPI void ImageDrawLine(Image* dst, int startPosX, int startPosY, int endPosX, int endPosY, Color color); // Draw line within an image
    FRLAPI void ImageDrawLineV(Image* dst, Vector2 start, Vector2 end, Color color);                          // Draw line within an image (Vector version)
    FRLAPI void ImageDrawCircle(Image* dst, int centerX, int centerY, int radius, Color color);               // Draw a filled circle within an image
    FRLAPI void ImageDrawCircleV(Image* dst, Vector2 center, int radius, Color color);                        // Draw a filled circle within an image (Vector version)
    FRLAPI void ImageDrawCircleLines(Image* dst, int centerX, int centerY, int radius, Color color);          // Draw circle outline within an image
    FRLAPI void ImageDrawCircleLinesV(Image* dst, Vector2 center, int radius, Color color);                   // Draw circle outline within an image (Vector version)
    FRLAPI void ImageDrawRectangle(Image* dst, int posX, int posY, int width, int height, Color color);       // Draw rectangle within an image
    FRLAPI void ImageDrawRectangleV(Image* dst, Vector2 position, Vector2 size, Color color);                 // Draw rectangle within an image (Vector version)
    FRLAPI void ImageDrawRectangleRec(Image* dst, Rectangle rec, Color color);                                // Draw rectangle within an image
    FRLAPI void ImageDrawRectangleLines(Image* dst, Rectangle rec, int thick, Color color);                   // Draw rectangle lines within an image
    FRLAPI void ImageDraw(Image* dst, Image src, Rectangle srcRec, Rectangle dstRec, Color tint);             // Draw a source image within a destination image (tint applied to source)
    FRLAPI void ImageDrawText(Image* dst, const char* text, int posX, int posY, int fontSize, Color color);   // Draw text (using default font) within an image (destination)
    FRLAPI void ImageDrawTextEx(Image* dst, Font font, const char* text, Vector2 position, float fontSize, float spacing, Color tint); // Draw text (custom sprite font) within an image (destination)

    // Texture loading functions
    // NOTE: These functions require GPU access
    FRLAPI Texture2D LoadTexture(const char* fileName);                                                       // Load texture from file into GPU memory (VRAM)
    FRLAPI Texture2D LoadTextureFromImage(Image image);                                                       // Load texture from image data
    FRLAPI TextureCubemap LoadTextureCubemap(Image image, int layout);                                        // Load cubemap from image, multiple image cubemap layouts supported
    FRLAPI RenderTexture2D LoadRenderTexture(int width, int height);                                          // Load texture for rendering (framebuffer)
    FRLAPI bool IsTextureReady(Texture2D texture);                                                            // Check if a texture is ready
    FRLAPI void UnloadTexture(Texture2D texture);                                                             // Unload texture from GPU memory (VRAM)
    FRLAPI bool IsRenderTextureReady(RenderTexture2D target);                                                 // Check if a render texture is ready
    FRLAPI void UnloadRenderTexture(RenderTexture2D target);                                                  // Unload render texture from GPU memory (VRAM)
    FRLAPI void UpdateTexture(Texture2D texture, const void* pixels);                                         // Update GPU texture with new data
    FRLAPI void UpdateTextureRec(Texture2D texture, Rectangle rec, const void* pixels);                       // Update GPU texture rectangle with new data

    // Texture configuration functions
    FRLAPI void GenTextureMipmaps(Texture2D* texture);                                                        // Generate GPU mipmaps for a texture
    FRLAPI void SetTextureFilter(Texture2D texture, int filter);                                              // Set texture scaling filter mode
    FRLAPI void SetTextureWrap(Texture2D texture, int wrap);                                                  // Set texture wrapping mode

    // Texture drawing functions
    FRLAPI void DrawTexture(Texture2D texture, int posX, int posY, Color tint);                               // Draw a Texture2D
    FRLAPI void DrawTextureV(Texture2D texture, Vector2 position, Color tint);                                // Draw a Texture2D with position defined as Vector2
    FRLAPI void DrawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);  // Draw a Texture2D with extended parameters
    FRLAPI void DrawTextureRec(Texture2D texture, Rectangle source, Vector2 position, Color tint);            // Draw a part of a texture defined by a rectangle
    FRLAPI void DrawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint); // Draw a part of a texture defined by a rectangle with 'pro' parameters
    FRLAPI void DrawTextureNPatch(Texture2D texture, NPatchInfo nPatchInfo, Rectangle dest, Vector2 origin, float rotation, Color tint); // Draws a texture (or part of it) that stretches or shrinks nicely

    // Color/pixel related functions
    FRLAPI Color Fade(Color color, float alpha);                                 // Get color with alpha applied, alpha goes from 0.0f to 1.0f
    FRLAPI int ColorToInt(Color color);                                          // Get hexadecimal value for a Color
    FRLAPI Vector4 ColorNormalize(Color color);                                  // Get Color normalized as float [0..1]
    FRLAPI Color ColorFromNormalized(Vector4 normalized);                        // Get Color from normalized values [0..1]
    FRLAPI Vector3 ColorToHSV(Color color);                                      // Get HSV values for a Color, hue [0..360], saturation/value [0..1]
    FRLAPI Color ColorFromHSV(float hue, float saturation, float value);         // Get a Color from HSV values, hue [0..360], saturation/value [0..1]
    FRLAPI Color ColorTint(Color color, Color tint);                             // Get color multiplied with another color
    FRLAPI Color ColorBrightness(Color color, float factor);                     // Get color with brightness correction, brightness factor goes from -1.0f to 1.0f
    FRLAPI Color ColorContrast(Color color, float contrast);                     // Get color with contrast correction, contrast values between -1.0f and 1.0f
    FRLAPI Color ColorAlpha(Color color, float alpha);                           // Get color with alpha applied, alpha goes from 0.0f to 1.0f
    FRLAPI Color ColorAlphaBlend(Color dst, Color src, Color tint);              // Get src alpha-blended into dst color with tint
    FRLAPI Color GetColor(unsigned int hexValue);                                // Get Color structure from hexadecimal value
    FRLAPI Color GetPixelColor(void* srcPtr, int format);                        // Get Color from a source pixel pointer of certain format
    FRLAPI void SetPixelColor(void* dstPtr, Color color, int format);            // Set color formatted into destination pixel pointer
    FRLAPI int GetPixelDataSize(int width, int height, int format);              // Get pixel data size in bytes for certain format

    //------------------------------------------------------------------------------------
    // Font Loading and Text Drawing Functions (Module: text)
    //------------------------------------------------------------------------------------

    // Font loading/unloading functions
    FRLAPI Font GetFontDefault(void);                                                            // Get the default Font
    FRLAPI Font LoadFont(const char* fileName);                                                  // Load font from file into GPU memory (VRAM)
    FRLAPI Font LoadFontEx(const char* fileName, int fontSize, int* codepoints, int codepointCount);  // Load font from file with extended parameters, use NULL for codepoints and 0 for codepointCount to load the default character set
    FRLAPI Font LoadFontFromImage(Image image, Color key, int firstChar);                        // Load font from Image (XNA style)
    FRLAPI Font LoadFontFromMemory(const char* fileType, const unsigned char* fileData, int dataSize, int fontSize, int* codepoints, int codepointCount); // Load font from memory buffer, fileType refers to extension: i.e. '.ttf'
    FRLAPI bool IsFontReady(Font font);                                                          // Check if a font is ready
    FRLAPI GlyphInfo* LoadFontData(const unsigned char* fileData, int dataSize, int fontSize, int* codepoints, int codepointCount, int type); // Load font data for further use
    FRLAPI Image GenImageFontAtlas(const GlyphInfo* glyphs, Rectangle** glyphRecs, int glyphCount, int fontSize, int padding, int packMethod); // Generate image font atlas using chars info
    FRLAPI void UnloadFontData(GlyphInfo* glyphs, int glyphCount);                               // Unload font chars info data (RAM)
    FRLAPI void UnloadFont(Font font);                                                           // Unload font from GPU memory (VRAM)
    FRLAPI bool ExportFontAsCode(Font font, const char* fileName);                               // Export font as code file, returns true on success

    // Text drawing functions
    FRLAPI void DrawFPS(int posX, int posY);                                                     // Draw current FPS
    FRLAPI void DrawText(const char* text, int posX, int posY, int fontSize, Color color);       // Draw text (using default font)
    FRLAPI void DrawTextEx(Font font, const char* text, Vector2 position, float fontSize, float spacing, Color tint); // Draw text using font and additional parameters
    FRLAPI void DrawTextPro(Font font, const char* text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint); // Draw text using Font and pro parameters (rotation)
    FRLAPI void DrawTextCodepoint(Font font, int codepoint, Vector2 position, float fontSize, Color tint); // Draw one character (codepoint)
    FRLAPI void DrawTextCodepoints(Font font, const int* codepoints, int codepointCount, Vector2 position, float fontSize, float spacing, Color tint); // Draw multiple character (codepoint)

    // Text font info functions
    FRLAPI void SetTextLineSpacing(int spacing);                                                 // Set vertical line spacing when drawing with line-breaks
    FRLAPI int MeasureText(const char* text, int fontSize);                                      // Measure string width for default font
    FRLAPI Vector2 MeasureTextEx(Font font, const char* text, float fontSize, float spacing);    // Measure string size for Font
    FRLAPI int GetGlyphIndex(Font font, int codepoint);                                          // Get glyph index position in font for a codepoint (unicode character), fallback to '?' if not found
    FRLAPI GlyphInfo GetGlyphInfo(Font font, int codepoint);                                     // Get glyph font info data for a codepoint (unicode character), fallback to '?' if not found
    FRLAPI Rectangle GetGlyphAtlasRec(Font font, int codepoint);                                 // Get glyph rectangle in font atlas for a codepoint (unicode character), fallback to '?' if not found

    // Text codepoints management functions (unicode characters)
    FRLAPI char* LoadUTF8(const int* codepoints, int length);                // Load UTF-8 text encoded from codepoints array
    FRLAPI void UnloadUTF8(char* text);                                      // Unload UTF-8 text encoded from codepoints array
    FRLAPI int* LoadCodepoints(const char* text, int* count);                // Load all codepoints from a UTF-8 text string, codepoints count returned by parameter
    FRLAPI void UnloadCodepoints(int* codepoints);                           // Unload codepoints data from memory
    FRLAPI int GetCodepointCount(const char* text);                          // Get total number of codepoints in a UTF-8 encoded string
    FRLAPI int GetCodepoint(const char* text, int* codepointSize);           // Get next codepoint in a UTF-8 encoded string, 0x3f('?') is returned on failure
    FRLAPI int GetCodepointNext(const char* text, int* codepointSize);       // Get next codepoint in a UTF-8 encoded string, 0x3f('?') is returned on failure
    FRLAPI int GetCodepointPrevious(const char* text, int* codepointSize);   // Get previous codepoint in a UTF-8 encoded string, 0x3f('?') is returned on failure
    FRLAPI const char* CodepointToUTF8(int codepoint, int* utf8Size);        // Encode one codepoint into UTF-8 byte array (array length returned as parameter)

    // Text strings management functions (no UTF-8 strings, only byte chars)
    // NOTE: Some strings allocate memory internally for returned strings, just be careful!
    FRLAPI int TextCopy(char* dst, const char* src);                                             // Copy one string to another, returns bytes copied
    FRLAPI bool TextIsEqual(const char* text1, const char* text2);                               // Check if two text string are equal
    FRLAPI unsigned int TextLength(const char* text);                                            // Get text length, checks for '\0' ending
    FRLAPI const char* TextFormat(const char* text, ...);                                        // Text formatting with variables (sprintf() style)
    FRLAPI const char* TextSubtext(const char* text, int position, int length);                  // Get a piece of a text string
    FRLAPI char* TextReplace(char* text, const char* replace, const char* by);                   // Replace text string (WARNING: memory must be freed!)
    FRLAPI char* TextInsert(const char* text, const char* insert, int position);                 // Insert text in a position (WARNING: memory must be freed!)
    FRLAPI const char* TextJoin(const char** textList, int count, const char* delimiter);        // Join text strings with delimiter
    FRLAPI const char** TextSplit(const char* text, char delimiter, int* count);                 // Split text into multiple strings
    FRLAPI void TextAppend(char* text, const char* append, int* position);                       // Append text at specific position and move cursor!
    FRLAPI int TextFindIndex(const char* text, const char* find);                                // Find first text occurrence within a string
    FRLAPI const char* TextToUpper(const char* text);                      // Get upper case version of provided string
    FRLAPI const char* TextToLower(const char* text);                      // Get lower case version of provided string
    FRLAPI const char* TextToPascal(const char* text);                     // Get Pascal case notation version of provided string
    FRLAPI int TextToInteger(const char* text);                            // Get integer value from text (negative values not supported)

    //------------------------------------------------------------------------------------
    // Basic 3d Shapes Drawing Functions (Module: models)
    //------------------------------------------------------------------------------------

    // Basic geometric 3D shapes drawing functions
    FRLAPI void DrawLine3D(Vector3 startPos, Vector3 endPos, Color color);                                    // Draw a line in 3D world space
    FRLAPI void DrawPoint3D(Vector3 position, Color color);                                                   // Draw a point in 3D space, actually a small line
    FRLAPI void DrawCircle3D(Vector3 center, float radius, Vector3 rotationAxis, float rotationAngle, Color color); // Draw a circle in 3D world space
    FRLAPI void DrawTriangle3D(Vector3 v1, Vector3 v2, Vector3 v3, Color color);                              // Draw a color-filled triangle (vertex in counter-clockwise order!)
    FRLAPI void DrawTriangleStrip3D(Vector3* points, int pointCount, Color color);                            // Draw a triangle strip defined by points
    FRLAPI void DrawCube(Vector3 position, float width, float height, float length, Color color);             // Draw cube
    FRLAPI void DrawCubeV(Vector3 position, Vector3 size, Color color);                                       // Draw cube (Vector version)
    FRLAPI void DrawCubeWires(Vector3 position, float width, float height, float length, Color color);        // Draw cube wires
    FRLAPI void DrawCubeWiresV(Vector3 position, Vector3 size, Color color);                                  // Draw cube wires (Vector version)
    FRLAPI void DrawSphere(Vector3 centerPos, float radius, Color color);                                     // Draw sphere
    FRLAPI void DrawSphereEx(Vector3 centerPos, float radius, int rings, int slices, Color color);            // Draw sphere with extended parameters
    FRLAPI void DrawSphereWires(Vector3 centerPos, float radius, int rings, int slices, Color color);         // Draw sphere wires
    FRLAPI void DrawCylinder(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color); // Draw a cylinder/cone
    FRLAPI void DrawCylinderEx(Vector3 startPos, Vector3 endPos, float startRadius, float endRadius, int sides, Color color); // Draw a cylinder with base at startPos and top at endPos
    FRLAPI void DrawCylinderWires(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color); // Draw a cylinder/cone wires
    FRLAPI void DrawCylinderWiresEx(Vector3 startPos, Vector3 endPos, float startRadius, float endRadius, int sides, Color color); // Draw a cylinder wires with base at startPos and top at endPos
    FRLAPI void DrawCapsule(Vector3 startPos, Vector3 endPos, float radius, int slices, int rings, Color color); // Draw a capsule with the center of its sphere caps at startPos and endPos
    FRLAPI void DrawCapsuleWires(Vector3 startPos, Vector3 endPos, float radius, int slices, int rings, Color color); // Draw capsule wireframe with the center of its sphere caps at startPos and endPos
    FRLAPI void DrawPlane(Vector3 centerPos, Vector2 size, Color color);                                      // Draw a plane XZ
    FRLAPI void DrawRay(Ray ray, Color color);                                                                // Draw a ray line
    FRLAPI void DrawGrid(int slices, float spacing);                                                          // Draw a grid (centered at (0, 0, 0))

    //------------------------------------------------------------------------------------
    // Model 3d Loading and Drawing Functions (Module: models)
    //------------------------------------------------------------------------------------

    // Model management functions
    FRLAPI Model LoadModel(const char* fileName);                                                // Load model from files (meshes and materials)
    FRLAPI Model LoadModelFromMesh(Mesh mesh);                                                   // Load model from generated mesh (default material)
    FRLAPI bool IsModelReady(Model model);                                                       // Check if a model is ready
    FRLAPI void UnloadModel(Model model);                                                        // Unload model (including meshes) from memory (RAM and/or VRAM)
    FRLAPI BoundingBox GetModelBoundingBox(Model model);                                         // Compute model bounding box limits (considers all meshes)

    // Model drawing functions
    FRLAPI void DrawModel(Model model, Vector3 position, float scale, Color tint);               // Draw a model (with texture if set)
    FRLAPI void DrawModelEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint); // Draw a model with extended parameters
    FRLAPI void DrawModelWires(Model model, Vector3 position, float scale, Color tint);          // Draw a model wires (with texture if set)
    FRLAPI void DrawModelWiresEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint); // Draw a model wires (with texture if set) with extended parameters
    FRLAPI void DrawBoundingBox(BoundingBox box, Color color);                                   // Draw bounding box (wires)
    FRLAPI void DrawBillboard(Camera camera, Texture2D texture, Vector3 position, float size, Color tint);   // Draw a billboard texture
    FRLAPI void DrawBillboardRec(Camera camera, Texture2D texture, Rectangle source, Vector3 position, Vector2 size, Color tint); // Draw a billboard texture defined by source
    FRLAPI void DrawBillboardPro(Camera camera, Texture2D texture, Rectangle source, Vector3 position, Vector3 up, Vector2 size, Vector2 origin, float rotation, Color tint); // Draw a billboard texture defined by source and rotation

    // Mesh management functions
    FRLAPI void UploadMesh(Mesh* mesh, bool dynamic);                                            // Upload mesh vertex data in GPU and provide VAO/VBO ids
    FRLAPI void UpdateMeshBuffer(Mesh mesh, int index, const void* data, int dataSize, int offset); // Update mesh vertex data in GPU for a specific buffer index
    FRLAPI void UnloadMesh(Mesh mesh);                                                           // Unload mesh data from CPU and GPU
    FRLAPI void DrawMesh(Mesh mesh, Material material, Matrix transform);                        // Draw a 3d mesh with material and transform
    FRLAPI void DrawMeshInstanced(Mesh mesh, Material material, const Matrix* transforms, int instances); // Draw multiple mesh instances with material and different transforms
    FRLAPI bool ExportMesh(Mesh mesh, const char* fileName);                                     // Export mesh data to file, returns true on success
    FRLAPI BoundingBox GetMeshBoundingBox(Mesh mesh);                                            // Compute mesh bounding box limits
    FRLAPI void GenMeshTangents(Mesh* mesh);                                                     // Compute mesh tangents

    // Mesh generation functions
    FRLAPI Mesh GenMeshPoly(int sides, float radius);                                            // Generate polygonal mesh
    FRLAPI Mesh GenMeshPlane(float width, float length, int resX, int resZ);                     // Generate plane mesh (with subdivisions)
    FRLAPI Mesh GenMeshCube(float width, float height, float length);                            // Generate cuboid mesh
    FRLAPI Mesh GenMeshSphere(float radius, int rings, int slices);                              // Generate sphere mesh (standard sphere)
    FRLAPI Mesh GenMeshHemiSphere(float radius, int rings, int slices);                          // Generate half-sphere mesh (no bottom cap)
    FRLAPI Mesh GenMeshCylinder(float radius, float height, int slices);                         // Generate cylinder mesh
    FRLAPI Mesh GenMeshCone(float radius, float height, int slices);                             // Generate cone/pyramid mesh
    FRLAPI Mesh GenMeshTorus(float radius, float size, int radSeg, int sides);                   // Generate torus mesh
    FRLAPI Mesh GenMeshKnot(float radius, float size, int radSeg, int sides);                    // Generate trefoil knot mesh
    FRLAPI Mesh GenMeshHeightmap(Image heightmap, Vector3 size);                                 // Generate heightmap mesh from image data
    FRLAPI Mesh GenMeshCubicmap(Image cubicmap, Vector3 cubeSize);                               // Generate cubes-based map mesh from image data

    // Material loading/unloading functions
    FRLAPI Material* LoadMaterials(const char* fileName, int* materialCount);                    // Load materials from model file
    FRLAPI Material LoadMaterialDefault(void);                                                   // Load default material (Supports: DIFFUSE, SPECULAR, NORMAL maps)
    FRLAPI bool IsMaterialReady(Material material);                                              // Check if a material is ready
    FRLAPI void UnloadMaterial(Material material);                                               // Unload material from GPU memory (VRAM)
    FRLAPI void SetMaterialTexture(Material* material, int mapType, Texture2D texture);          // Set texture for a material map type (MATERIAL_MAP_DIFFUSE, MATERIAL_MAP_SPECULAR...)
    FRLAPI void SetModelMeshMaterial(Model* model, int meshId, int materialId);                  // Set material for a mesh

    // Model animations loading/unloading functions
    FRLAPI ModelAnimation* LoadModelAnimations(const char* fileName, int* animCount);            // Load model animations from file
    FRLAPI void UpdateModelAnimation(Model model, ModelAnimation anim, int frame);               // Update model animation pose
    FRLAPI void UnloadModelAnimation(ModelAnimation anim);                                       // Unload animation data
    FRLAPI void UnloadModelAnimations(ModelAnimation* animations, int animCount);                // Unload animation array data
    FRLAPI bool IsModelAnimationValid(Model model, ModelAnimation anim);                         // Check model animation skeleton match

    // Collision detection functions
    FRLAPI bool CheckCollisionSpheres(Vector3 center1, float radius1, Vector3 center2, float radius2);   // Check collision between two spheres
    FRLAPI bool CheckCollisionBoxes(BoundingBox box1, BoundingBox box2);                                 // Check collision between two bounding boxes
    FRLAPI bool CheckCollisionBoxSphere(BoundingBox box, Vector3 center, float radius);                  // Check collision between box and sphere
    FRLAPI RayCollision GetRayCollisionSphere(Ray ray, Vector3 center, float radius);                    // Get collision info between ray and sphere
    FRLAPI RayCollision GetRayCollisionBox(Ray ray, BoundingBox box);                                    // Get collision info between ray and box
    FRLAPI RayCollision GetRayCollisionMesh(Ray ray, Mesh mesh, Matrix transform);                       // Get collision info between ray and mesh
    FRLAPI RayCollision GetRayCollisionTriangle(Ray ray, Vector3 p1, Vector3 p2, Vector3 p3);            // Get collision info between ray and triangle
    FRLAPI RayCollision GetRayCollisionQuad(Ray ray, Vector3 p1, Vector3 p2, Vector3 p3, Vector3 p4);    // Get collision info between ray and quad

    //------------------------------------------------------------------------------------
    // Audio Loading and Playing Functions (Module: audio)
    //------------------------------------------------------------------------------------
    typedef void (*AudioCallback)(void* bufferData, unsigned int frames);

    // Audio device management functions
    FRLAPI void InitAudioDevice(void);                                     // Initialize audio device and context
    FRLAPI void CloseAudioDevice(void);                                    // Close the audio device and context
    FRLAPI bool IsAudioDeviceReady(void);                                  // Check if audio device has been initialized successfully
    FRLAPI void SetMasterVolume(float volume);                             // Set master volume (listener)
    FRLAPI float GetMasterVolume(void);                                    // Get master volume (listener)

    // Wave/Sound loading/unloading functions
    FRLAPI Wave LoadWave(const char* fileName);                            // Load wave data from file
    FRLAPI Wave LoadWaveFromMemory(const char* fileType, const unsigned char* fileData, int dataSize); // Load wave from memory buffer, fileType refers to extension: i.e. '.wav'
    FRLAPI bool IsWaveReady(Wave wave);                                    // Checks if wave data is ready
    FRLAPI Sound LoadSound(const char* fileName);                          // Load sound from file
    FRLAPI Sound LoadSoundFromWave(Wave wave);                             // Load sound from wave data
    FRLAPI Sound LoadSoundAlias(Sound source);                             // Create a new sound that shares the same sample data as the source sound, does not own the sound data
    FRLAPI bool IsSoundReady(Sound sound);                                 // Checks if a sound is ready
    FRLAPI void UpdateSound(Sound sound, const void* data, int sampleCount); // Update sound buffer with new data
    FRLAPI void UnloadWave(Wave wave);                                     // Unload wave data
    FRLAPI void UnloadSound(Sound sound);                                  // Unload sound
    FRLAPI void UnloadSoundAlias(Sound alias);                             // Unload a sound alias (does not deallocate sample data)
    FRLAPI bool ExportWave(Wave wave, const char* fileName);               // Export wave data to file, returns true on success
    FRLAPI bool ExportWaveAsCode(Wave wave, const char* fileName);         // Export wave sample data to code (.h), returns true on success

    // Wave/Sound management functions
    FRLAPI void PlaySound(Sound sound);                                    // Play a sound
    FRLAPI void StopSound(Sound sound);                                    // Stop playing a sound
    FRLAPI void PauseSound(Sound sound);                                   // Pause a sound
    FRLAPI void ResumeSound(Sound sound);                                  // Resume a paused sound
    FRLAPI bool IsSoundPlaying(Sound sound);                               // Check if a sound is currently playing
    FRLAPI void SetSoundVolume(Sound sound, float volume);                 // Set volume for a sound (1.0 is max level)
    FRLAPI void SetSoundPitch(Sound sound, float pitch);                   // Set pitch for a sound (1.0 is base level)
    FRLAPI void SetSoundPan(Sound sound, float pan);                       // Set pan for a sound (0.5 is center)
    FRLAPI Wave WaveCopy(Wave wave);                                       // Copy a wave to a new wave
    FRLAPI void WaveCrop(Wave* wave, int initSample, int finalSample);     // Crop a wave to defined samples range
    FRLAPI void WaveFormat(Wave* wave, int sampleRate, int sampleSize, int channels); // Convert wave data to desired format
    FRLAPI float* LoadWaveSamples(Wave wave);                              // Load samples data from wave as a 32bit float data array
    FRLAPI void UnloadWaveSamples(float* samples);                         // Unload samples data loaded with LoadWaveSamples()

    // Music management functions
    FRLAPI Music LoadMusicStream(const char* fileName);                    // Load music stream from file
    FRLAPI Music LoadMusicStreamFromMemory(const char* fileType, const unsigned char* data, int dataSize); // Load music stream from data
    FRLAPI bool IsMusicReady(Music music);                                 // Checks if a music stream is ready
    FRLAPI void UnloadMusicStream(Music music);                            // Unload music stream
    FRLAPI void PlayMusicStream(Music music);                              // Start music playing
    FRLAPI bool IsMusicStreamPlaying(Music music);                         // Check if music is playing
    FRLAPI void UpdateMusicStream(Music music);                            // Updates buffers for music streaming
    FRLAPI void StopMusicStream(Music music);                              // Stop music playing
    FRLAPI void PauseMusicStream(Music music);                             // Pause music playing
    FRLAPI void ResumeMusicStream(Music music);                            // Resume playing paused music
    FRLAPI void SeekMusicStream(Music music, float position);              // Seek music to a position (in seconds)
    FRLAPI void SetMusicVolume(Music music, float volume);                 // Set volume for music (1.0 is max level)
    FRLAPI void SetMusicPitch(Music music, float pitch);                   // Set pitch for a music (1.0 is base level)
    FRLAPI void SetMusicPan(Music music, float pan);                       // Set pan for a music (0.5 is center)
    FRLAPI float GetMusicTimeLength(Music music);                          // Get music time length (in seconds)
    FRLAPI float GetMusicTimePlayed(Music music);                          // Get current music time played (in seconds)

    // AudioStream management functions
    FRLAPI AudioStream LoadAudioStream(unsigned int sampleRate, unsigned int sampleSize, unsigned int channels); // Load audio stream (to stream raw audio pcm data)
    FRLAPI bool IsAudioStreamReady(AudioStream stream);                    // Checks if an audio stream is ready
    FRLAPI void UnloadAudioStream(AudioStream stream);                     // Unload audio stream and free memory
    FRLAPI void UpdateAudioStream(AudioStream stream, const void* data, int frameCount); // Update audio stream buffers with data
    FRLAPI bool IsAudioStreamProcessed(AudioStream stream);                // Check if any audio stream buffers requires refill
    FRLAPI void PlayAudioStream(AudioStream stream);                       // Play audio stream
    FRLAPI void PauseAudioStream(AudioStream stream);                      // Pause audio stream
    FRLAPI void ResumeAudioStream(AudioStream stream);                     // Resume audio stream
    FRLAPI bool IsAudioStreamPlaying(AudioStream stream);                  // Check if audio stream is playing
    FRLAPI void StopAudioStream(AudioStream stream);                       // Stop audio stream
    FRLAPI void SetAudioStreamVolume(AudioStream stream, float volume);    // Set volume for audio stream (1.0 is max level)
    FRLAPI void SetAudioStreamPitch(AudioStream stream, float pitch);      // Set pitch for audio stream (1.0 is base level)
    FRLAPI void SetAudioStreamPan(AudioStream stream, float pan);          // Set pan for audio stream (0.5 is centered)
    FRLAPI void SetAudioStreamBufferSizeDefault(int size);                 // Default size for new audio streams
    FRLAPI void SetAudioStreamCallback(AudioStream stream, AudioCallback callback); // Audio thread callback to request new data

    FRLAPI void AttachAudioStreamProcessor(AudioStream stream, AudioCallback processor); // Attach audio stream processor to stream, receives the samples as <float>s
    FRLAPI void DetachAudioStreamProcessor(AudioStream stream, AudioCallback processor); // Detach audio stream processor from stream

    FRLAPI void AttachAudioMixedProcessor(AudioCallback processor); // Attach audio stream processor to the entire audio pipeline, receives the samples as <float>s
    FRLAPI void DetachAudioMixedProcessor(AudioCallback processor); // Detach audio stream processor from the entire audio pipeline
}
#endif

