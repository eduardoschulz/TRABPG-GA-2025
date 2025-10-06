#ifndef SPRITE_H
#define SPRITE_H

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

class Sprite
{
public:
    Sprite();
    ~Sprite();

    // Aqui deixamos o parâmetro angle com valor default
    void initialize(GLuint shaderID, GLuint texID,
                    int nAnimations, int nFrames,
                    vec3 pos, vec3 dimensions,
                    float angle = 0.0f);

    void update();
    void draw();
    void moveRight();
    void moveLeft();
    void moveDown();
    int getPosY();  // Getter for Y position
    int getPosX();  // Getter for Y position
    void setPosY(int y);
    void setPosX(int x);
    void setD(float s, float t);

private:
    GLuint VAO;        // id do VAO da geometria
    GLuint texID;      // id da textura
    vec3 pos;          // posição
    vec3 dimensions;   // escala
    float angle;       // rotação no eixo Z
    GLuint shaderID;   // shader usado
    int nAnimations, nFrames, iAnimations, iFrames;
    vec2 d;
    float lastTime, FPS;
    float vel;

    GLuint setupGeometry();
};

#endif
