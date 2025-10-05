// CatchGame.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream> 
#include "stb_easy_font.h"

// Função para renderizar texto do Score usando stb_easy_font (que é puxada pelo cmake)
void renderText(float x, float y, const std::string& text) {
    static char buffer[99999]; // Capacidade para ~500 caracteres
    int num_quads = stb_easy_font_print(x, y, (char*)text.c_str(), nullptr, buffer, sizeof(buffer));

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads * 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}



// Window dimensions
const int WIDTH = 800;
const int HEIGHT = 600;

// Paddle properties
float paddleWidth = 100.0f;
float paddleHeight = 20.0f;
float paddleX = WIDTH / 2 - paddleWidth / 2;
float paddleY = HEIGHT - 50.0f;
float paddleSpeed = 1000.0f;

// Falling object
float objectSize = 20.0f;
float objectX = rand() % (WIDTH - (int)objectSize);
float objectY = 0;
float objectSpeed = 700.0f;

int score = 0;


// Function to draw rectangle
void drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void resetObject() {
    objectX = rand() % (WIDTH - (int)objectSize);
    objectY = 0;
}

void processInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        paddleX -= paddleSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        paddleX += paddleSpeed * deltaTime;
    }

    // Clamp within window
    if (paddleX < 0) paddleX = 0;
    if (paddleX + paddleWidth > WIDTH) paddleX = WIDTH - paddleWidth;
}

bool checkCollision() {
    return (objectY <= paddleY + paddleHeight &&
            objectY + objectSize >= paddleY &&
            objectX + objectSize >= paddleX &&
            objectX <= paddleX + paddleWidth);
}

int main() {
    srand((unsigned)time(0));

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

    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1); // 2D projection

    float lastTime = glfwGetTime();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);  // Corrige coordenadas
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();   

    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Input
        processInput(window, deltaTime);

        // Update
        objectY += objectSpeed * deltaTime;

        // Collision check
        if (checkCollision()) {
            ++score;
            std::cout << "Score: " << score << std::endl;
            resetObject();
        } else if (objectY > HEIGHT) {
            std::cout << "Missed! Game Over. Final Score: " << score << std::endl;
            score = 0;
            resetObject();
        }

        // Rendering
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 1.0f, 1.0f);

        // Cor do texto
        glColor3f(1.0f, 1.0f, 1.0f);

// Renderiza texto 3x maior, no canto superior esquerdo
glPushMatrix();
glTranslatef(0.0f, 0.0f, 0.0f);          // Nenhuma translação (opcional)
glScalef(3.0f, 3.0f, 1.0f);             // Escala normal (sem inverter o eixo X)
renderText(10.0f / 3.0f, (HEIGHT - 30.0f) / 3.0f, "Score: " + std::to_string(score));
glPopMatrix();

        // Draw paddle
        glColor3f(0.2f, 0.6f, 0.9f);
        drawRect(paddleX, paddleY, paddleWidth, paddleHeight);

        // Draw falling object
        glColor3f(0.0f, 1.0f, 0.0f); // Verde
        drawRect(objectX, objectY, objectSize, objectSize);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}