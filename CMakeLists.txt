cmake_minimum_required(VERSION 3.26)


set(MAIN_PROJECT_NAME "oop")
set(MAIN_EXECUTABLE_NAME "${MAIN_PROJECT_NAME}")


project(${MAIN_PROJECT_NAME})
# set(CMAKE_PROJECT_VERSION_MAJOR 0)
# set(CMAKE_PROJECT_VERSION_MINOR 0)
# set(CMAKE_PROJECT_VERSION_PATCH 1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(SFML_DIR "C:/Users/Madalina/CLionProjects/untitled1/SFML-2.6.1/lib/cmake/SFML")

include(cmake/Options.cmake)
include(cmake/CompilerFlags.cmake)
include(cmake/CopyHelper.cmake)

find_package(SFML 2.6 CONFIG REQUIRED COMPONENTS system window graphics)

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing name here
add_executable(${MAIN_EXECUTABLE_NAME}
        main.cpp
        Game.cpp
        GameException.h
        InputState.h
        Entity.h
        Entity.cpp
        Player.h
        Player.cpp
        Enemy.h
        Enemy.cpp
        Game.h
        Game.cpp
        EnemyFactory.h
        EnemyFactory.cpp
)


target_link_libraries(${MAIN_EXECUTABLE_NAME} PRIVATE
        sfml-graphics sfml-window sfml-system
)

set_compiler_flags(RUN_SANITIZERS TRUE TARGET_NAMES ${MAIN_EXECUTABLE_NAME})

install(TARGETS ${MAIN_EXECUTABLE_NAME} DESTINATION ${DESTINATION_DIR})
if(APPLE)
    install(FILES launcher.command DESTINATION ${DESTINATION_DIR})
endif()

copy_files(FILES tastatura.txt COPY_TO_DESTINATION TARGET_NAME ${MAIN_EXECUTABLE_NAME})
if (WIN32)
    add_custom_command(TARGET ${MAIN_EXECUTABLE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${MAIN_EXECUTABLE_NAME}>
            $<TARGET_FILE_DIR:${MAIN_EXECUTABLE_NAME}>
            COMMAND_EXPAND_LISTS
    )
endif()
