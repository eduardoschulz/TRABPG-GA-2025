#ifndef PADDLE_H
#define PADDLE_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Paddle {
public:
    Paddle(float width, float height, float initialX, float initialY, float speed);
    void update(GLFWwindow* window, float deltaTime);
    void draw();
    float getX() const { return x_; }
    float getY() const { return y_; }
    float getWidth() const { return width_; }
    float getHeight() const { return height_; }

private:
    float x_, y_;
    float width_, height_;
    float speed_;
    const int WINDOW_WIDTH = 800;  // Pode ser parametrizado se quiser
};

#endif