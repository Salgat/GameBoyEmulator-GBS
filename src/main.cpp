#include <iostream>
#include <chrono>
#include <thread>

#include "GameBoy.hpp"

sf::RenderWindow window;

bool PollEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
			return false;
		}	
    }
	
	return true;
}

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

    GameBoy gameboy;
    //gameboy.LoadGame("opus5.gb");
    //gameboy.LoadGame("cpu_instrs.gb");

    //gameboy.LoadGame("01-special.gb");
    gameboy.LoadGame("02-interrupts.gb");
    //gameboy.LoadGame("03-op sp,hl.gb"); // Causes errors running the wrong opcode
    //gameboy.LoadGame("04-op r,imm.gb");
    //gameboy.LoadGame("05-op rp.gb");
    //gameboy.LoadGame("06-ld r,r.gb");
    //gameboy.LoadGame("07-jr,jp,call,ret,rst.gb");
    //gameboy.LoadGame("08-misc instrs.gb");
    //gameboy.LoadGame("09-op r,r.gb");
    //gameboy.LoadGame("10-bit ops.gb");
    //gameboy.LoadGame("11-op a,(hl).gb");

    unsigned int count = 0;
	bool running = true;
    while(running) {
		//std::cout << "Processing Frame" << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
		//std::cout << "Polling events" << std::endl;
        running = PollEvents();
		//std::cout << "Rendering frame" << std::endl;
        sf::Image frame = gameboy.RenderFrame();
		//std::cout << "Drawing frame" << std::endl;
        DrawFrame(frame);

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> elapsed = end-start;
        std::chrono::duration<double, std::milli> sleep_time = std::chrono::duration<double, std::milli>(1.0/59.7 * 1000.0) - elapsed;
        if (sleep_time > std::chrono::duration<double, std::milli>(0)) {
            std::this_thread::sleep_for(sleep_time);
        }

        // Debugging
        /*if (++count > 120) {
            count = 0;

            std::cout << "VRAM 0x8700: " << std::endl;
            for (unsigned int index = 0; index < 16; ++index) {
                std::cout << std::hex << static_cast<unsigned int>(gameboy.mmu.ReadByte(0x8700+index)) << std::endl;
            }

            std::cout << "0xFF42 and 0xFF43: " << std::hex << static_cast<unsigned int>(gameboy.mmu.ReadByte(0xFF42)) << ", " << static_cast<unsigned int>(gameboy.mmu.ReadByte(0xFF43)) << std::endl;

            std::cout << "VRAM 0x9800: " << std::endl;
            for (unsigned int index = 0; index < 9; ++index) {
                std::cout << std::hex << static_cast<unsigned int>(gameboy.mmu.ReadByte(0x9800+index)) << std::endl;
            }
        }*/
    }

	std::cout << "Exit Emulator" << std::endl;
}