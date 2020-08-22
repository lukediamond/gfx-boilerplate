cmake_minimum_required(VERSION 3.5.0)

set(CMAKE_CXX_FLAGS "--std=c++1z -fno-exceptions -m64")

add_executable(runtime
    main.cpp
    shader.cpp
    image.cpp
    prim.cpp
    stb_impl.cpp
)
target_link_libraries(runtime
    SDL2
    GLEW
    GL
)
