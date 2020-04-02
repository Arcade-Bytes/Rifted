#Especificar donde se encuentran los ficheros cpp a compilar
target_sources(libModulos PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/Engine.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/Game.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/State.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/GameState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/MainMenuState.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/Level.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/Entity.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/Player.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/Enemy.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/Lever.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/Door.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/BreakableDoor.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/LevelExit.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/Map.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/Animation.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/Weapon.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/Shield.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/MovementComponent.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/AnimationComponent.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/Hitbox.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Source/FileManager.cpp"
)

#Y donde se encuentran los .h
target_include_directories(libModulos PUBLIC 
"${CMAKE_CURRENT_SOURCE_DIR}"
)

