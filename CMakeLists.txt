cmake_minimum_required(VERSION 3.20)

set(VCPKG_TARGET_TRIPLET  "x64-windows-static" CACHE STRING "" FORCE)

project(FloatEngine VERSION 1.5.0 LANGUAGES C CXX)

# ==============================================================================
# 1. 基础配置
# ==============================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
if(MSVC)
    add_compile_options(
        /execution-charset:utf-8
        $<$<OR:$<CXX_COMPILER_ID:MSVC>,$<C_COMPILER_ID:MSVC>>:/wd4005>

    )
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    # MultiThreaded      -> /MT (Release 静态)
    # MultiThreadedDebug -> /MTd (Debug 静态)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# ==============================================================================
# 2. 查找依赖库
# ==============================================================================
find_package(minizip-ng CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(SLikeNet CONFIG REQUIRED)
set(CUSTOM_RAYLIB_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/FloatEngine/raylib")

find_path(SOL2_INCLUDE_DIRS "sol/abort.hpp")
if(NOT SOL2_INCLUDE_DIRS)
    message(FATAL_ERROR "Sol2 not found!")
endif()

# ==============================================================================
# 3. 源文件扫描
# ==============================================================================
file(GLOB_RECURSE SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/FloatEngine/*.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/FloatEngine/*.c"
)

file(GLOB_RECURSE HEADERS 
    "${CMAKE_CURRENT_SOURCE_DIR}/FloatEngine/*.h" 
    "${CMAKE_CURRENT_SOURCE_DIR}/FloatEngine/*.hpp"
)


if(NOT SOURCES)
    message(FATAL_ERROR "No sources found in FloatEngine folder!")
endif()

# ==============================================================================
# 4. 构建静态库
# ==============================================================================
add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})

# ==============================================================================
# 5. 包含路径与链接
# ==============================================================================
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/FloatEngine
    ${LUA_INCLUDE_DIR}
    ${SOL2_INCLUDE_DIRS}
    "${CUSTOM_RAYLIB_ROOT}"
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>    
    ASIO_STANDALONE
    _CONSOLE
    WIN32
    WIN32_LEAN_AND_MEAN
    NOMINMAX
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    MINIZIP::minizip-ng
    slikenet
    ${LUA_LIBRARIES}
    "${CUSTOM_RAYLIB_ROOT}/lib/raylib.lib"
)

# ==============================================================================
# 6. 安装配置：FloatEngine 自身内容
# ==============================================================================
install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/FloatEngine/
    DESTINATION include/FloatEngine
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# ==============================================================================
# 7. 安装配置：Vcpkg 依赖内容 (DLL 自动拷贝)
# ==============================================================================
file(GLOB VCPKG_DIRS "${CMAKE_BINARY_DIR}/vcpkg_installed/*")
set(MY_VCPKG_INSTALLED_DIR "")

foreach(DIR ${VCPKG_DIRS})
    if(IS_DIRECTORY "${DIR}/include")
        set(MY_VCPKG_INSTALLED_DIR "${DIR}")
        break()
    endif()
endforeach()

if(MY_VCPKG_INSTALLED_DIR)
    # 复制头文件
    install(DIRECTORY "${MY_VCPKG_INSTALLED_DIR}/include/" DESTINATION include)
    # 复制 Lib
    install(DIRECTORY "${MY_VCPKG_INSTALLED_DIR}/lib/" DESTINATION lib)
    
    if(IS_DIRECTORY "${MY_VCPKG_INSTALLED_DIR}/bin")
        install(DIRECTORY "${MY_VCPKG_INSTALLED_DIR}/bin/" DESTINATION bin)
    endif()

    if(IS_DIRECTORY "${MY_VCPKG_INSTALLED_DIR}/debug/lib")
        install(DIRECTORY "${MY_VCPKG_INSTALLED_DIR}/debug/lib/" DESTINATION lib/debug)
    endif()
    
    if(IS_DIRECTORY "${MY_VCPKG_INSTALLED_DIR}/debug/bin")
        install(DIRECTORY "${MY_VCPKG_INSTALLED_DIR}/debug/bin/" DESTINATION bin/debug)
    endif()

else()
    message(WARNING "Could not locate vcpkg_installed folder.")
endif()