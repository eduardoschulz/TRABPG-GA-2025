#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


// Desenha um retângulo simples usando OpenGL imediato
void drawRect(float x, float y, float width, float height);

// Renderiza texto usando stb_easy_font
void renderText(float x, float y, const std::string& text);

#endif