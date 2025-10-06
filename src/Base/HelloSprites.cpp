#include <iostream>
#include <random>
#include <string>
#include <assert.h>
#include <vector>

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

// STB_IMAGE
// STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Sprite.h"
#include "Background.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
GLuint loadTexture(string filePath);
void processInput(Sprite &spr);
void spriteCreation(Sprite &spr, int tipo, GLuint shaderID);
bool CheckCollision(Sprite &one, Sprite &two);


// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar *vertexShaderSource = "#version 400\n"
								   "layout (location = 0) in vec3 position;\n"
								   "layout (location = 1) in vec2 texc;\n"
								   "uniform mat4 projection;\n"
								   "uniform mat4 model;\n"
								   "out vec2 texCoord;\n"
								   "void main()\n"
								   "{\n"
								   //...pode ter mais linhas de código aqui!
								   "gl_Position = projection * model * vec4(position.x, position.y, position.z, 1.0);\n"
								   "texCoord = vec2(texc.s, 1.0 - texc.t);\n"
								   "}\0";

// Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar *fragmentShaderSource = "#version 400\n"
									 "in vec2 texCoord;\n"
									 "uniform sampler2D texBuffer;\n"
									 "uniform vec2 offsetTex;\n"
									 "out vec4 color;\n"
									 "void main()\n"
									 "{\n"
									 "color = texture(texBuffer, texCoord + offsetTex);\n"
									 "}\n\0";


bool keys[1024];
int score = 0;


// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! -- Rossana", nullptr, nullptr);
	if (!window)
	{
		cerr << "Falha ao criar a janela GLFW" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cerr << "Falha ao inicializar GLAD" << endl;
		return -1;
	}

	// Obtendo as informações de versão
	const GLubyte *renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte *version = glGetString(GL_VERSION);	/* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	//Inicializar o array de controle de teclas
	for (int i =0; i < 1024; i++) { keys[i] = false; }

	// Compilando e buildando o programa de shader

	/*Definindo 2 sprites*/

    
	Sprite mulher;
	Sprite background;


	GLuint shaderID = setupShader();
	spriteCreation(mulher, 1, shaderID);

	spriteCreation(background, 3, shaderID);

	vector<Sprite> fruits;
	//Habilitação do teste de profundidade]
	glEnable(GL_DEPTH_TEST);

    //Habilitação da transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


	glUseProgram(shaderID); // Reseta o estado do shader para evitar problemas futuros

	double prev_s = glfwGetTime();	// Define o "tempo anterior" inicial.
	double title_countdown_s = 0.1; // Intervalo para atualizar o título da janela com o FPS.

	// Criação da matriz de projeção
	mat4 projection = ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);

	// Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
	// que não está nos buffers
	// Mandar a matriz de projeção para o shader
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"),1,GL_FALSE,value_ptr(projection));

	mat4 model = mat4(1); //matriz identidade

	// Ativar o primeiro buffer de textura do OpenGL
	glActiveTexture(GL_TEXTURE0);

	
	// Registrando o nome que o buffer da textura terá no fragment shader
	glUniform1i(glGetUniformLocation(shaderID, "tex_buffer"), 0);
	
	random_device rd;

	double lastSpawnTime = glfwGetTime();
	double spawnInterval = 1.0 + (rand() % 10)/10.0;
	// Loop da aplicação - "game loop"
	
	while (!glfwWindowShouldClose(window))
	{
		// Este trecho de código é totalmente opcional: calcula e mostra a contagem do FPS na barra de título
		{
			double curr_s = glfwGetTime();		// Obtém o tempo atual.
			double elapsed_s = curr_s - prev_s; // Calcula o tempo decorrido desde o último frame.
			prev_s = curr_s;					// Atualiza o "tempo anterior" para o próximo frame.

			// Exibe o FPS, mas não a cada frame, para evitar oscilações excessivas.
			title_countdown_s -= elapsed_s;
			if (title_countdown_s <= 0.0 && elapsed_s > 0.0)
			{
				double fps = 1.0 / elapsed_s; // Calcula o FPS com base no tempo decorrido.

				// Cria uma string e define o FPS como título da janela.
				char tmp[256];
				sprintf(tmp, "Ola Triangulo! -- Rossana\tFPS %.2lf", fps);
				glfwSetWindowTitle(window, tmp);

				title_countdown_s = 0.1; // Reinicia o temporizador para atualizar o título periodicamente.
			}
		}

		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();
		processInput(mulher);

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		background.draw();

		glLineWidth(10);
		glPointSize(20);

		// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		double currentTime = glfwGetTime();
		if (currentTime - lastSpawnTime >= spawnInterval) {
    		Sprite newFruit;
    		spriteCreation(newFruit, 2, shaderID);  // Cria uma nova fruta com pos aleatória
    		fruits.push_back(newFruit);
    		lastSpawnTime = currentTime;
		}

        //Atualizar e desenhar o sprite
		
        mulher.update();
        mulher.draw();
        vector<bool> toRemove(fruits.size(), false);

		// Atualize, mova, desenhe e cheque cada fruta
		bool gameOver = false;
		for (size_t i = 0; i < fruits.size(); ++i) {
			fruits[i].update();
			fruits[i].moveDown();
			fruits[i].draw();

			// Checa colisão (remove se colidir)
			if (CheckCollision(fruits[i], mulher)) {
				toRemove[i] = true;
				score++;
				cout << "\nContato! Fruta coletada. Score: " << endl;
				cout << score;
				// Aqui você pode adicionar pontuação, som, etc.
			} 
			// Checa se atingiu o fundo (game over)
			else if (fruits[i].getPosY() <= 0) {
				gameOver = true;
			}
		}

		// Remova as frutas marcadas (de trás para frente para evitar shifts errados)
		for (int i = static_cast<int>(fruits.size()) - 1; i >= 0; --i) {
			if (toRemove[i]) {
				fruits.erase(fruits.begin() + i);
			}
		}

		// Game Over se qualquer fruta passou
		if (gameOver) {
			cout << "\nGame Over" << endl;
			glfwSetWindowShouldClose(window, true);
		}
		glBindVertexArray(0); // Desnecessário aqui, pois não há múltiplos VAOs

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	//glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (action == GLFW_PRESS)
	{
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}
}

// Esta função está bastante hardcoded - objetivo é compilar e "buildar" um programa de
//  shader simples e único neste exemplo de código
//  O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
//  fragmentShader source no iniçio deste arquivo
//  A função retorna o identificador do programa de shader
int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				  << infoLog << endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

GLuint loadTexture(string filePath)
{
	GLuint texID;

	// Gera o identificador da textura na memória
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// Ajusta os parâmetros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Carregamento dos pixels da imagem
	int width, height, nrChannels;
	unsigned char *data = stbi_load(filePath.c_str(), &width, &height,
	&nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
 		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
			data);
 		}
 		else //png
 		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			data);
 		}
 		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}

	// Liberar o data e desconectar a textura
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

    return texID;
}

void processInput(Sprite &spr)
{
	if (keys[GLFW_KEY_D])
	{
		spr.moveRight();
	}
	if (keys[GLFW_KEY_A])
	{
		spr.moveLeft();
	}
}

/*Criacao de Sprite*/

void spriteCreation(Sprite &spr, int tipo, GLuint shaderID){
	//	GLuint shaderID = setupShader();
    random_device rd;   // fonte de entropia real (se o sistema suportar)
    mt19937 gen(rd());  // Mersenne Twister (gerador de alta qualidade)
    uniform_int_distribution<> dist(0, 800);

    int r = dist(gen);


	if (tipo == 1){
		GLuint texID = loadTexture("../assets/sprites/pessoa_spr.png");
    	spr.initialize(shaderID,texID,4,3,vec3(400.0,100.0,1.0),vec3(24.0 * 3, 32.0 * 3, 1.0));
	}
	else if (tipo == 2){
		GLuint texID = loadTexture("../assets/sprites/fruta_spr.png");
    	spr.initialize(shaderID,texID,1,1,vec3(r,600.0,1.0),vec3(18.0 * 3, 18.0 * 3, 1.0));
	}
	else{
		GLuint texID = loadTexture("../assets/sprites/fundo_2.png");
		spr.initialize(shaderID,texID,1,1,vec3(400.0,300.0,0.0),vec3(800.0, 600.0, 1.0));
	}
}


bool CheckCollision(Sprite &one, Sprite &two) // AABB - Axis Aligned Bounding Box
{
	// Collision x-axis?
	bool collisionX = one.getPosX() + 50 >= two.getPosX() &&
					  two.getPosX() + 50 >= one.getPosX();
	// Collision y-axis?
	bool collisionY = one.getPosY() + 50 >= two.getPosY() &&
					  two.getPosY() + 50 >= one.getPosY();
	// Collision only if on both axes
	return collisionX && collisionY;
}

