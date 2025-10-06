#include "SpriteFruit.h"
#include <iostream>

SpriteFruit::SpriteFruit(float x, float y, float speed)
    : Sprite(x, y, speed) {
    // Optionally set texture, color, etc. specific to the fruit
}

void SpriteFruit::moveDown() {
    // Assuming Sprite has members like posY and speed
    posY -= speed; // Moves down the screen
}

void SpriteFruit::update() {
    // Example of overridden update logic
    moveDown();

    // Optionally, you could add gravity or off-screen check
    if (posY < -10.0f) {
        std::cout << "Fruit fell off screen\n";
    }
}
