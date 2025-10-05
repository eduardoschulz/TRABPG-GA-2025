#include "FallingObject.h"
#include "../Utils/Utils.h"  // Caminho relativo para Utils
#include <cstdlib>

FallingObject::FallingObject(float size, float speed, int windowWidth)
    : size_(size), speed_(speed), windowWidth_(windowWidth) {
    reset();  // Inicializa posição aleatória
}

void FallingObject::update(float deltaTime) {
    y_ += speed_ * deltaTime;
}

void FallingObject::reset() {
    x_ = rand() % (windowWidth_ - static_cast<int>(size_));
    y_ = 0;
}

void FallingObject::draw() {
    glColor3f(0.0f, 1.0f, 0.0f);  // Cor verde
    drawRect(x_, y_, size_, size_);  // Usa a função do Utils
}

bool FallingObject::isOffScreen(int windowHeight) const {
    return y_ > windowHeight;
}