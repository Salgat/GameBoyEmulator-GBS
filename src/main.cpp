#include <iostream>
#include <chrono>
#include <thread>

#include "GameBoy.hpp"

sf::RenderWindow window;

void PollEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void DrawFrame(sf::Image const& frame) {
    window.clear(sf::Color::Black);

    static sf::Texture texture;
    texture.update(frame);
    static sf::Sprite sprite;
    sprite.setTexture(texture);

    window.draw(sprite);

    window.display();
}

int main() {
    window.create(sf::VideoMode(160, 144), "GameBoy Emulator");

    GameBoy gameboy;
    gameboy.LoadGame("TESTGAME.GB");

    while(true) {
        auto start = std::chrono::high_resolution_clock::now();
        PollEvents();
        sf::Image frame = gameboy.RenderFrame();
        DrawFrame(frame);

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> elapsed = end-start;
        std::chrono::duration<double, std::milli> sleep_time = std::chrono::duration<double, std::milli>(1.0/59.7 * 1000.0) - elapsed;
        if (sleep_time > std::chrono::duration<double, std::milli>(0)) {
            std::this_thread::sleep_for(sleep_time);
        }
    }

	std::cout << "Exit Emulator" << std::endl;
}