#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Largura e altura da janela
const GLuint WIDTH = 800, HEIGHT = 600;

// Vertex Shader (projeção ortográfica simulada via NDC)
const char* vertexShaderSource = R"(
#version 400 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

out vec3 fragColor;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0, 1.0);
    fragColor = color;
}
)";

// Fragment Shader
const char* fragmentShaderSource = R"(
#version 400 core
in vec3 fragColor;
out vec4 finalColor;

void main()
{
    finalColor = vec4(fragColor, 1.0);
}
)";

// Global OpenGL IDs
GLuint shaderProgram, VBO, VAO;

// Armazenamento de dados dinâmicos
std::vector<GLfloat> vertexData;

// Prototipações
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void setupShader();
void setupBuffers();
void updateBuffer();

// Função utilitária para gerar cores aleatórias
std::vector<float> generateRandomColor() {
    return {
        static_cast<float>(rand()) / RAND_MAX,
        static_cast<float>(rand()) / RAND_MAX,
        static_cast<float>(rand()) / RAND_MAX
    };
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Para gerar cores aleatórias

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Triângulos com clique do mouse", NULL, NULL);
    if (!window) {
        std::cerr << "Erro ao criar janela GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Erro ao inicializar GLAD" << std::endl;
        return -1;
    }

    // Setup
    setupShader();
    setupBuffers();

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // Desenha os triângulos completos
        glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 5);

        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

// Resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Mouse click callback
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Converte de coordenadas da tela para coordenadas NDC (-1 a 1)
        float x_ndc = static_cast<float>((xpos / WIDTH) * 2.0 - 1.0);
        float y_ndc = static_cast<float>(((HEIGHT - ypos) / HEIGHT) * 2.0 - 1.0);

        static std::vector<float> currentColor;
        static int vertexCount = 0;

        // Se for o primeiro vértice do triângulo, gera nova cor
        if (vertexCount % 3 == 0) {
            currentColor = generateRandomColor();
        }

        // Adiciona vértice com a cor atual
        vertexData.push_back(x_ndc);
        vertexData.push_back(y_ndc);
        vertexData.push_back(currentColor[0]);
        vertexData.push_back(currentColor[1]);
        vertexData.push_back(currentColor[2]);

        vertexCount++;

        // Atualiza o VBO com os novos dados
        updateBuffer();
    }
}

// Compilação e linkagem dos shaders
void setupShader() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Erro ao compilar vertex shader:\n" << infoLog << std::endl;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Erro ao compilar fragment shader:\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Erro ao linkar shader program:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Inicializa VBO e VAO
void setupBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    // posição (2 floats)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // cor (3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Atualiza o conteúdo do VBO com os dados mais recentes
void updateBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
