#include <iostream>
#include <chrono>
#include <thread>

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

int main() {
    window.create(sf::VideoMode(160, 144), "GameBoy Emulator");

    GameBoy gameboy(window);
	//gameboy.LoadGame("adjtris.gb");
    //gameboy.LoadGame("opus5.gb");
    //gameboy.LoadGame("ttt.gb");
    //gameboy.LoadGame("Tetris.gb");
    //gameboy.LoadGame("PokemonRed.gb");
    //gameboy.LoadGame("SML.gb");
    //gameboy.LoadGame("SML2.gb");
    //gameboy.LoadGame("cpu_instrs.gb");
    //gameboy.LoadGame("Madden '96.gb");
    gameboy.LoadGame("LoZ.gb");
	//gameboy.LoadGame("Tamagotchi.gb");
	
    //gameboy.LoadGame("01-special.gb");
    //gameboy.LoadGame("02-interrupts.gb");
    //gameboy.LoadGame("03-op sp,hl.gb");
    //gameboy.LoadGame("04-op r,imm.gb");
    //gameboy.LoadGame("05-op rp.gb");
    //gameboy.LoadGame("06-ld r,r.gb");
    //gameboy.LoadGame("07-jr,jp,call,ret,rst.gb");
    //gameboy.LoadGame("08-misc instrs.gb");
    //gameboy.LoadGame("09-op r,r.gb");
    //gameboy.LoadGame("10-bit ops.gb");
    //gameboy.LoadGame("11-op a,(hl).gb");

    //gameboy.LoadGame("instr_timing.gb");
    //gameboy.LoadGame("01-read_timing.gb");
    //gameboy.LoadGame("02-write_timing.gb");
    //gameboy.LoadGame("03-modify_timing.gb");

    //gameboy.LoadGame("mem_timing.gb");

    unsigned int count = 0;
	bool running = true;
    while(running) {
        auto start = std::chrono::high_resolution_clock::now();
        auto result = gameboy.RenderFrame();
        DrawFrame(result.first);
        running = result.second;
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> elapsed = end-start;
        std::chrono::duration<double, std::milli> sleep_time = std::chrono::duration<double, std::milli>(1.0/59.7 * 1000.0) - elapsed;
        if (sleep_time > std::chrono::duration<double, std::milli>(0)) {
            std::this_thread::sleep_for(sleep_time);
        }
    }

	std::cout << "Exit Emulator" << std::endl;
}