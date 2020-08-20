cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_CXX_FLAGS "--std=c++1z")

add_executable(runtime
    main.cpp
    shader.cpp
    stb_impl.cpp
)
target_link_libraries(runtime
    SDL2
    GLEW
    GL
)
