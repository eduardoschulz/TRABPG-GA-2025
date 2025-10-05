#include "Utils.h"
#include <glad/glad.h>
#include <stb_easy_font.h>  // Assuma que está na raiz do projeto; ajuste include se necessário
#include <iostream>

// Implementação do drawRect
void drawRect(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// Implementação do renderText
void renderText(float x, float y, const std::string& text) {
    static char buffer[99999]; // Capacidade para ~500 caracteres
    int num_quads = stb_easy_font_print(x, y, (char*)text.c_str(), nullptr, buffer, sizeof(buffer));

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads * 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}