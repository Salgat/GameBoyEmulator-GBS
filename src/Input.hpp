//
// Created by Austin on 6/26/2015.
//

#ifndef GAMEBOYEMULATOR_INPUT_HPP
#define GAMEBOYEMULATOR_INPUT_HPP

// SFML
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

#include <array>

enum class KeyType {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    A,
    B,
    START,
    SELECT,
    NONE
};

class MemoryManagementUnit;

/**
 * Handles input for game (joypad).
 */
class Input {
public:
    Input();

    void Initialize(MemoryManagementUnit* mmu_, sf::RenderWindow* window_);
    void Reset();
    uint8_t ReadByte();
    void WriteByte(uint8_t value);

    bool PollEvents();
    void UpdateInput();
    void KeyUp(KeyType key);
    void KeyDown(KeyType key);

private:
    MemoryManagementUnit* mmu;
    sf::RenderWindow* window;

    std::array<uint8_t, 2> rows;
    uint8_t column;
};

#endif //GAMEBOYEMULATOR_INPUT_HPP
