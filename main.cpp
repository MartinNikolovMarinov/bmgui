#include <core.h>
#include <fmt/core.h>
#include <SFML/Window.hpp>

using namespace coretypes;

i32 main(i32, char const**) {
    constexpr i32 N = 100;
    sf::Window windows[N];
    for (size_t i = 0; i < N; i++) {
        windows[i].create(sf::VideoMode(800, 600), fmt::format("Window {}", i));
    }
    return 0;
}
