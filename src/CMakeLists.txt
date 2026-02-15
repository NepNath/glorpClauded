include(GenerateExportHeader)

add_library(ecs SHARED
    "ecs/internal/component_manager.cpp"
    "ecs/internal/entity_manager.cpp"
    "ecs/internal/system_manager.cpp" )

add_library(lib::ecs ALIAS ecs)

target_include_directories(ecs
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
)

generate_export_header(ecs
    BASE_NAME        ECS
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/ecs/export.h
)

add_executable(ecs_tests
    ecs/tests/core_suitetests.cpp
)

target_link_libraries(ecs_tests
    PRIVATE
        lib::ecs
        GTest::gtest
        GTest::gtest_main
)



add_library(
    menu_ui SHARED 
    menu/ui/window.cpp
    menu/ui/window.hpp
 )

add_library(
    game SHARED
    game/game.cpp
    game/game.hpp
 )



target_link_libraries(menu_ui 
    PUBLIC sfml::sfml
    game
)
target_link_libraries(game 
    PUBLIC sfml::sfml
)

target_include_directories(menu_ui PUBLIC 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR} # Pour inclure le fichier généré
)
target_include_directories(game PUBLIC 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR} # Pour inclure le fichier généré menu_export.h
)

set_target_properties(menu_ui PROPERTIES DEFINE_SYMBOL MENU_WINDOW_EXPORTS)
generate_export_header(menu_ui
    BASE_NAME        MENU
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/menu_export.h
)

set_target_properties(game PROPERTIES DEFINE_SYMBOL GAME_WINDOW_EXPORTS)
generate_export_header(game
    BASE_NAME        GAME
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/game_export.h
)

add_executable(BaseProject main.cpp ) 

target_link_libraries(BaseProject 
    PUBLIC sfml::sfml
    menu_ui
    game
# ou selon les besoins : sfml-system sfml-window sfml-graphics ...
)

add_executable(nep_test
    ecs/tests/nep_test.cpp
)

target_link_libraries(nep_test
    PRIVATE
        lib::ecs
        GTest::gtest
        GTest::gtest_main
        game
)