set(PROJECT_NAME FloatEngine)

################################################################################
# Source groups
################################################################################
set(
    "FloatEngine.h"
)
source_group("头文件" FILES ${})

set(__Anim
    "F_Anim.h"
    "F_Easings.h"
)
source_group("头文件\\Anim" FILES ${__Anim})

set(__Audio
    "F_Audio.h"
)
source_group("头文件\\Audio" FILES ${__Audio})

set(__Console
    "F_Console.h"
)
source_group("头文件\\Console" FILES ${__Console})

set(__Control
    "Enum.h"
    "F_Resource.h"
    "FloatApi.h"
    "Game.h"
    "Object.h"
    "Room.h"
    "Sprite.h"
    "winfuns.h"
)
source_group("头文件\\Control" FILES ${__Control})

set(__Control__Class
    "Actor.h"
    "Block.h"
)
source_group("头文件\\Control\\Class" FILES ${__Control__Class})

set(__Font
    "F_Font.h"
)
source_group("头文件\\Font" FILES ${__Font})

set(__Math
    "FMath.h"
)
source_group("头文件\\Math" FILES ${__Math})

set(__Shader
    "F_Shader.h"
)
source_group("头文件\\Shader" FILES ${__Shader})

set(__Text
    "F_Text.h"
)
source_group("头文件\\Text" FILES ${__Text})

set(__Anim
    "F_Anim.cpp"
)
source_group("源文件\\Anim" FILES ${__Anim})

set(__Audio
    "F_Audio.cpp"
)
source_group("源文件\\Audio" FILES ${__Audio})

set(__Console
    "F_Console.cpp"
)
source_group("源文件\\Console" FILES ${__Console})

set(__Control
    "F_Resource.cpp"
    "FloatAPI.cpp"
    "Game.cpp"
    "Object.cpp"
    "Room.cpp"
    "Sprite.cpp"
)
source_group("源文件\\Control" FILES ${__Control})

set(__Control__Class
    "Actor.cpp"
    "Block.cpp"
)
source_group("源文件\\Control\\Class" FILES ${__Control__Class})

set(__Font
    "F_Font.cpp"
)
source_group("源文件\\Font" FILES ${__Font})

set(__Math
    "FMath.cpp"
)
source_group("源文件\\Math" FILES ${__Math})

set(__Shader
    "F_Shader.cpp"
)
source_group("源文件\\Shader" FILES ${__Shader})

set(__Text
    "F_Text.cpp"
)
source_group("源文件\\Text" FILES ${__Text})

set(ALL_FILES
    ${}
    ${__Anim}
    ${__Audio}
    ${__Console}
    ${__Control}
    ${__Control__Class}
    ${__Font}
    ${__Math}
    ${__Shader}
    ${__Text}
    ${__Anim}
    ${__Audio}
    ${__Console}
    ${__Control}
    ${__Control__Class}
    ${__Font}
    ${__Math}
    ${__Shader}
    ${__Text}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE FloatEngine)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG;"
            "UNICODE;"
            "_UNICODE"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG;"
            "_MBCS"
        ">"
        "_CONSOLE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /W3
            >
            $<$<CONFIG:Release>:
                /Oi;
                /Gy;
                /w;
                /execution-charset:utf-8
            >
            /permissive-;
            /std:c++17;
            /sdl;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /SUBSYSTEM:CONSOLE;
                minizip.lib;
                zlib.lib
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

