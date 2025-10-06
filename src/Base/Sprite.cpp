#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

int Sprite::getPosY(){
	return pos.y;
}

int Sprite::getPosX(){
	return pos.x;
}

void Sprite::setPosY(int y){
	this->pos.y = y;
}

void Sprite::setPosX(int x){
	this->pos.x = x;
}

void Sprite::setD(float s, float t){
	this->d.s = s;
	this->d.t = t;
}

void Sprite::initialize(GLuint shaderID, GLuint texID, int nAnimations, int nFrames, vec3 pos, vec3 dimensions, float angle)
{
    this->shaderID = shaderID;
    this->texID = texID;
    this->pos = pos;
    this->dimensions = dimensions;
    this->angle = angle;
	this->nAnimations = nAnimations;
	this->nFrames = nFrames;
	this->iAnimations = 1; //animação inicial
	this->iFrames = 0; //frame inicial
	this->d.s = 1.0 / (float) nFrames;
	this->d.t = 1.0 / (float) nAnimations;
	this->FPS = 12.0;
	this->lastTime = 0.0;
	this->vel = 3;
    this->VAO = setupGeometry();
}

// Atualiza o estado do sprite, incluindo matriz de transformação e animação.
void Sprite::update()
{
	// Cria matriz de modelo com transformação de translação e escala.
    mat4 model = mat4(1); 
    model = translate(model,pos);
	model = scale(model,dimensions);
	// Mandar a matriz de modelo para o shader
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"),1,GL_FALSE,value_ptr(model));
	// Calcula deslocamento de textura com base no quadro e animação atuais.
	vec2 offsetTex = vec2(iFrames*d.s,iAnimations * d.t);
	glUniform2f(glGetUniformLocation(shaderID, "offsetTex"),offsetTex.s,offsetTex.t);
	// Atualiza quadro de animação com base no tempo decorrido.
	float now = glfwGetTime();
	float dt = now - lastTime;

	if (dt >= 1.0/FPS)
	{
		iFrames = (iFrames + 1) % nFrames;
		lastTime = now;
	}

}

// Renderiza o sprite na tela, aplicando textura e transformações.
void Sprite::draw()
{
    glBindVertexArray(VAO); // Conectando ao buffer de geometria

	glBindTexture(GL_TEXTURE_2D, texID); // conectando o buffer de textura

	// Matriz de modelo - Tranformações na geometria, nos objetos
	mat4 model = mat4(1); // matriz identidade
	// Translação
	model = translate(model, pos);
	// Rotação
	model = rotate(model, radians(angle), vec3(0.0, 0.0, 1.0));
	// Escala
	model = scale(model, dimensions);
	// Enviar para o shader
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, value_ptr(model));

	// Chamada de desenho - drawcall
	// Poligono Preenchido - GL_TRIANGLES
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0); // Desconectando o buffer de geometria
}

void Sprite::moveRight()
{
	if (pos.x < 799){// Limita movimento à borda direita da janela.
	pos.x += vel;
	iAnimations = 3;}// Define animação de movimento para a direita.

}

void Sprite::moveLeft()
{
	if (pos.x > 1)// Limita movimento à borda esquerda da janela.
	{
		pos.x -= vel;
		iAnimations = 2;// Define animação de movimento para a esquerda.
	}
}

// Move o sprite para baixo com velocidade fixa. Usado para as frutas
void Sprite::moveDown()
{
	pos.y -= 3.4;
}

GLuint Sprite::setupGeometry()
{
    // Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat vertices[] = {
		// x   y     z    s     		t
		// T0
		-0.5, -0.5, 0.0, 0.0, 0.0, // V0 inferior esquerdo
		-0.5, 0.5, 0.0, 0.0, d.t,  // V1 superior esquerdo
		0.5, -0.5, 0.0, d.s, 0.0,  // V2 inferior direito
		0.5, 0.5, 0.0, d.s, d.t	   // V3 inferior esquerdo

	};

	GLuint VBO;
	// Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	// Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos
	glBindVertexArray(VAO);
	// Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando:
	//  Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	//  Numero de valores que o atributo tem (por ex, 3 coordenadas xyz)
	//  Tipo do dado
	//  Se está normalizado (entre zero e um)
	//  Tamanho em bytes
	//  Deslocamento a partir do byte zero

	// Atributo posição - x, y, z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Atributo coordenada de textura - s, t
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}