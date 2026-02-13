cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(PROJECT_NAME Enot)
project(${PROJECT_NAME})

# Поиск FreeType
find_package(Freetype REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    rect/rect.cpp
    shader/shader.cpp
    player/player.cpp
    animSprite/anim.cpp
    spriteClass/sprite.cpp
    text/text.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# GLFW настройки
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw3)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm)

# Подключение FreeType
target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARIES})

