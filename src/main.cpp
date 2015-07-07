#include <iostream>
#include <chrono>
#include <thread>

using namespace std::literals;

#include "GameBoy.hpp"

sf::RenderWindow window;

void DrawFrame(sf::Image const& frame) {
    window.clear(sf::Color::Green);
	
    sf::Texture texture;
    texture.loadFromImage(frame, sf::IntRect(0, 0, 160, 144));
    sf::Sprite sprite;
    sprite.setTexture(texture);
	
    window.draw(sprite);
    window.display();
}

int main(int argc, char* argv[]) {
    std::string game_name = "";
    for (int argument = 1; argument < argc; ++argument) {
        auto arg = std::string(argv[argument]);
        if (arg.find("-game=") == 0) {
            // Game's File Name to load
            game_name = arg.substr(6);
        }
    }

    if (game_name == "") {
        std::cout << "No game loaded." << std::endl;
        return 0;
    }

    window.create(sf::VideoMode(160, 144), "GBS");
    GameBoy gameboy(window);
    gameboy.LoadGame(game_name);

	bool running = true;
    auto start_time = std::chrono::steady_clock::now();
	auto next_frame = start_time + 16.75041876ms;
    while(running) {
		auto result = gameboy.RenderFrame();
        DrawFrame(result.first);
        running = result.second;
        
		std::this_thread::sleep_until(next_frame);
		next_frame += 16.75041876ms;
    }
}