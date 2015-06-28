//
// Special credit goes to Imran Nazar's JavaScript implementation of the input (written in JavaScript), which was
// used as a reference. imrannazar.com/GameBoy-Emulation-in-Javascript:-Input
//

#include "Input.hpp"
#include "MemoryManagementUnit.hpp"

Input::Input() {
    Reset();
}

void Input::Initialize(MemoryManagementUnit* mmu_) {
    mmu = mmu_;
}

void Input::Reset() {
    rows[0] = 0x0F;
    rows[1] = 0x0F;
    column = 0x00;
}

uint8_t Input::ReadByte() {
    switch (column) {
        case 0x00:
            return 0x00;
        case 0x10:
            return rows[0];
        case 0x20:
            return rows[1];
        default:
            return 0x00;
    }
}

void Input::WriteByte(uint8_t value) {
    column = value & 0x30;
}

void Input::KeyUp(KeyType key) {
    mmu->interrupt_flag |= 0x04;

    switch(key) {
        case KeyType::RIGHT:  rows[1] &= 0xE; break;
        case KeyType::LEFT:   rows[1] &= 0xD; break;
        case KeyType::UP:     rows[1] &= 0xB; break;
        case KeyType::DOWN:   rows[1] &= 0x7; break;
        case KeyType::A:      rows[0] &= 0xE; break;
        case KeyType::B:      rows[0] &= 0xD; break;
        case KeyType::SELECT: rows[0] &= 0xB; break;
        case KeyType::START:  rows[0] &= 0x7; break;
    }
}

void Input::KeyDown(KeyType key) {
    switch(key) {
        case KeyType::RIGHT:  rows[1] |= 0x01; break;
        case KeyType::LEFT:   rows[1] |= 0x02; break;
        case KeyType::UP:     rows[1] |= 0x04; break;
        case KeyType::DOWN:   rows[1] |= 0x08; break;
        case KeyType::A:      rows[0] |= 0x01; break;
        case KeyType::B:      rows[0] |= 0x02; break;
        case KeyType::SELECT: rows[0] |= 0x04; break;
        case KeyType::START:  rows[0] |= 0x08; break;
    }
}


bool Input::PollEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            return false;
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left) {
                KeyDown(KeyType::LEFT);
            } else if (event.key.code == sf::Keyboard::Right) {
                KeyDown(KeyType::RIGHT);
            } else if (event.key.code == sf::Keyboard::Up) {
                KeyDown(KeyType::UP);
            } else if (event.key.code == sf::Keyboard::Down) {
                KeyDown(KeyType::DOWN);
            } else if (event.key.code == sf::Keyboard::X) {
                KeyDown(KeyType::A);
            } else if (event.key.code == sf::Keyboard::Z) {
                KeyDown(KeyType::B);
            } else if (event.key.code == sf::Keyboard::Return) {
                KeyDown(KeyType::START);
            } else if (event.key.code == sf::Keyboard::RShift) {
                KeyDown(KeyType::SELECT);
            }
        }

        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Left) {
                KeyUp(KeyType::LEFT);
            } else if (event.key.code == sf::Keyboard::Right) {
                KeyUp(KeyType::RIGHT);
            } else if (event.key.code == sf::Keyboard::Up) {
                KeyUp(KeyType::UP);
            } else if (event.key.code == sf::Keyboard::Down) {
                KeyUp(KeyType::DOWN);
            } else if (event.key.code == sf::Keyboard::X) {
                KeyUp(KeyType::A);
            } else if (event.key.code == sf::Keyboard::Z) {
                KeyUp(KeyType::B);
            } else if (event.key.code == sf::Keyboard::Return) {
                KeyUp(KeyType::START);
            } else if (event.key.code == sf::Keyboard::RShift) {
                KeyUp(KeyType::SELECT);
            }
        }
    }

    return true;
}