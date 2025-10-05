#include "Paddle.h"
#include "../Utils/Utils.h"  // Caminho relativo para Utils
#include <iostream>

Paddle::Paddle(float width, float height, float initialX, float initialY, float speed)
    : width_(width), height_(height), x_(initialX), y_(initialY), speed_(speed) {}

void Paddle::update(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        x_ -= speed_ * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        x_ += speed_ * deltaTime;
    }

    // Clamp dentro da janela
    if (x_ < 0) x_ = 0;
    if (x_ + width_ > WINDOW_WIDTH) x_ = WINDOW_WIDTH - width_;
}

void Paddle::draw() {
    glColor3f(0.2f, 0.6f, 0.9f);  // Cor azulada
    drawRect(x_, y_, width_, height_);  // Usa a função do Utils
}