cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_CXX_FLAGS "--std=c++1z -fno-exceptions -m64")

add_executable(runtime
    main.cpp
    gl_shader.cpp
    gl_prim.cpp
    gl_texture.cpp
    gl_text.cpp
    image.cpp
    text.cpp
    stb_impl.cpp
)
target_link_libraries(runtime
    SDL2
    GLEW
    GL
    freetype
)
target_include_directories(runtime PRIVATE
    /usr/include/freetype2
)