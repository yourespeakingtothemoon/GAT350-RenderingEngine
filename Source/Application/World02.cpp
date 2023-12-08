#include "World02.h"

#include "Framework/Framework.h"
#include "Input/InputSystem.h"

//#define INTERLEAVE
#define INDEX

namespace nc {
	bool World02::Initialize() {
		// Shaders
		const char* vertexShader =
			"#version 430\n"
			"layout (location=0) in vec3 position;"
			"layout (location=1) in vec3 color;"
			"layout (location=0) out vec3 ocolor;"
			"void main() {"
			"  ocolor = color;"
			"  gl_Position = vec4(position, 1.0);"
			"}";

		const char* fragmentShader =
			"#version 430\n"
			"layout (location=0) in vec3 color;"
			"out vec4 ocolor;"
			"void main() {"
			"  ocolor = vec4(color, 1);"
			"}";

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vertexShader, NULL);
		glCompileShader(vs);

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fragmentShader, NULL);
		glCompileShader(fs);

		GLuint program = glCreateProgram();
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glUseProgram(program);

		#ifdef INTERLEAVE
		
		// Vertex data
		float vertexData[] = {
			-0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 0.0f,
			-0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.8f, -0.8f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 0.0f
		};

		GLuint vbo;
		glGenBuffers(1, &vbo);
		// Position
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		glGenVertexArrays(1, &(this->vao));
		glBindVertexArray(this->vao);

		glBindVertexBuffer(0, vbo, 0, sizeof(GLfloat) * 6);

		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexAttribBinding(0, 0);

		// Color
		glEnableVertexAttribArray(1);
		glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3);
		glVertexAttribBinding(1, 0);

		#elif defined(INDEX)

		// Vertex data
		const float vertexData[] = {
			-1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top-left
			 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
			 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
			-1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f  // bottom-left
		};

		GLuint indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		// Vertex buffer object
		GLuint vbo;
		glGenBuffers(1, &vbo);
		// Position
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		// Index buffer object
		GLuint ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Vertex array object
		glGenVertexArrays(1, &(this->vao));
		glBindVertexArray(this->vao);

		glBindVertexBuffer(0, vbo, 0, sizeof(GLfloat) * 6);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexAttribBinding(0, 0);

		// Color
		glEnableVertexAttribArray(1);
		glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3);
		glVertexAttribBinding(1, 0);

		#else

		// Vertex data
		float positionData[] = {
			-0.8f, -0.8f, 0.0f,
			 0.8f,  0.8f, 0.0f,
			-0.8f,  0.8f, 0.0f,
			-0.8f, -0.8f, 0.0f,
			 0.8f, -0.8f, 0.0f,
			 0.8f,  0.8f, 0.0f
		};

		float colorData[] =	{
			1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 0.0f
		};

		GLuint vbo[2];
		glGenBuffers(2, vbo);
		// Position
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);

		// Color
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

		glGenVertexArrays(1, &(this->vao));
		glBindVertexArray(this->vao);

		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindVertexBuffer(0, vbo[0], 0, sizeof(GLfloat) * 3);

		// Color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindVertexBuffer(1, vbo[1], 0, sizeof(GLfloat) * 3);

		#endif

		return true;
	}

	void World02::Shutdown() {
	}

	void World02::Update(float deltaTime) {
		/*
		this->position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? deltaTime : 0;
		this->position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -deltaTime : 0;

		this->position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? deltaTime : 0;
		this->position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? -deltaTime : 0;

		this->angle += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_LEFT) ? deltaTime * 90 : 0;
		this->angle += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_RIGHT) ? -deltaTime * 90 : 0;

		this->time += deltaTime;
		*/
	}

	void World02::Draw(Renderer& renderer) {
		// pre-render
		renderer.BeginFrame();

		// render
		glBindVertexArray(this->vao);

		#ifdef INDEX
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		#else
		glDrawArrays(GL_TRIANGLES, 0, 6);
		#endif

		// post-render
		renderer.EndFrame();
	}
}
