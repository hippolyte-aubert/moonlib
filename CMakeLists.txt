cmake_minimum_required(VERSION 3.16)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

enable_language(CXX)

include(GNUInstallDirs)

project(moonlib VERSION 0.1.0 LANGUAGES CXX)

find_package(raylib CONFIG REQUIRED)

add_library(moonlib SHARED
    ./src/moonlib.h
    ./src/Core/Engine.cpp
    ./src/Core/Scene.cpp
    ./src/Core/Asset.cpp
    ./src/Core/Camera.cpp
    ./src/Systems/PhysicsSystem.cpp
    ./src/Systems/RenderingSystem.cpp
    ./src/Systems/InputSystem.cpp
    ./src/Systems/CollisionSystem.cpp
    ./src/Systems/UserInterfaceSystem.cpp
    ./src/Systems/AudioSystem.cpp
    ./src/Systems/EntityDataSystem.cpp
    ./src/Components/Physics.hpp
    ./src/Components/Rendering.hpp
    ./src/Core/Entity.hpp
    ./src/Core/Engine.hpp
    ./src/Core/Component.hpp
    ./src/Core/Coordinator.hpp
    ./src/Core/Scene.hpp
    ./src/Core/Asset.hpp
    ./src/Core/Camera.hpp
    ./src/Core/System.hpp
    ./src/Systems/PhysicsSystem.hpp
    ./src/Systems/RenderingSystem.hpp
    ./src/Systems/InputSystem.hpp
    ./src/Systems/CollisionSystem.hpp
    ./src/Systems/UserInterfaceSystem.hpp
    ./src/Systems/AudioSystem.hpp
    ./src/Systems/EntityDataSystem.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ./src)

if (WIN32)
    install(TARGETS moonlib LIBRARY DESTINATION "moonlib/lib"
                        ARCHIVE DESTINATION "moonlib/lib"
                        RUNTIME DESTINATION "moonlib/bin"
                        COMPONENT library)

    install(FILES ./src/moonlib.h DESTINATION "moonlib/include")
endif()

if (UNIX)
    install(TARGETS moonlib LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
                        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
                        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
                        COMPONENT library)

    install(FILES ./src/moonlib.h DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)