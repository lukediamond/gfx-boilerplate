cmake_minimum_required(VERSION 3.5.0)

add_executable(runtime
    main.cpp
)
target_link_libraries(runtime
    SDL2
    GLEW
    GL
)
