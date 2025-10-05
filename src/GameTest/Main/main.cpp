// CatchGame.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include "../Utils/Utils.h"      // Caminho relativo para Utils
#include "../Paddle/Paddle.h"   // Caminho relativo para Paddle
#include "../FallingObject/FallingObject.h"  // Caminho relativo para FallingObject

// Window dimensions
const int WIDTH = 800;
const int HEIGHT = 600;

int main() {
    srand(static_cast<unsigned>(time(0)));

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "GLFW init failed!" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Catch Game", NULL, NULL);
    if (!window) {
        std::cerr << "Window creation failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Configuração de projeção 2D
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Instâncias das classes
    Paddle paddle(100.0f, 20.0f, WIDTH / 2 - 50.0f, HEIGHT - 50.0f, 1000.0f);
    FallingObject fallingObject(20.0f, 700.0f, WIDTH);
    int score = 0;

    float lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Input e Update da Paddle
        paddle.update(window, deltaTime);

        // Update do FallingObject
        fallingObject.update(deltaTime);

        // Checagem de colisão
        bool collision = (fallingObject.getY() <= paddle.getY() + paddle.getHeight() &&
                          fallingObject.getY() + fallingObject.getSize() >= paddle.getY() &&
                          fallingObject.getX() + fallingObject.getSize() >= paddle.getX() &&
                          fallingObject.getX() <= paddle.getX() + paddle.getWidth());

        if (collision) {
            ++score;
            std::cout << "Score: " << score << std::endl;
            fallingObject.reset();
        } else if (fallingObject.isOffScreen(HEIGHT)) {
            std::cout << "Missed! Game Over. Final Score: " << score << std::endl;
            score = 0;
            fallingObject.reset();
        }

        // Rendering
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 1.0f, 1.0f);

        // Renderiza texto do score (escalado 3x, no topo esquerdo)
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);
        glScalef(3.0f, 3.0f, 1.0f);
        renderText(10.0f / 3.0f, (HEIGHT - 30.0f) / 3.0f, "Score: " + std::to_string(score));
        glPopMatrix();

        // Desenha Paddle e FallingObject (usam Utils internamente)
        paddle.draw();
        fallingObject.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}