#ifndef FALLINGOBJECT_H
#define FALLINGOBJECT_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdlib>

class FallingObject {
public:
    FallingObject(float size, float speed, int windowWidth);
    void update(float deltaTime);
    void reset();
    void draw();
    bool isOffScreen(int windowHeight) const;
    float getX() const { return x_; }
    float getY() const { return y_; }
    float getSize() const { return size_; }

private:
    float x_, y_;
    float size_;
    float speed_;
    int windowWidth_;
};

#endif