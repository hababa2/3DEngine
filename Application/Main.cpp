#include <glad\glad.h>
#include <sdl.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <iostream>

// vertices
const float vertices[] =
{
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

// vertex shader
const char* vertexSource = R"(
    #version 430 core 

    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 color;

	out vec3 fragColor;

	uniform float scale;

    void main()
    {
		fragColor = color;
        gl_Position = vec4(position * scale, 1.0);
    }
)";

// fragment
const char* fragmentSource = R"(
    #version 430 core

	in vec3 fragColor;

    out vec4 outColor;

	uniform vec3 tint;

    void main()
    {
        outColor = vec4(fragColor * tint, 1.0);
    }
)";

int main(int argc, char** argv)
{
	int result = SDL_Init(SDL_INIT_VIDEO);
	if (result != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
	}

	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
	if (window == nullptr)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetSwapInterval(1);

	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (!gladLoadGL())
	{
		SDL_Log("Failed to create OpenGL context");
		exit(-1);
	}

	//Shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		std::cout << buffer;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		std::cout << buffer;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);

	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		char buffer[512];
		glGetShaderInfoLog(shaderProgram, 512, NULL, buffer);
		std::cout << buffer;
	}

	glUseProgram(shaderProgram);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLubyte*)NULL);
	glEnableVertexAttribArray(0);

	//color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLubyte*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	//uniform
	GLuint scaleLoc = glGetUniformLocation(shaderProgram, "scale");
	GLuint tintLoc = glGetUniformLocation(shaderProgram, "tint");

	float time = 0;
	glm::vec3 tint{ 1.0f, 0.33f, 0.33f };

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}

		SDL_PumpEvents();

		time += 0.1f;
		glUniform1f(scaleLoc, (std::sin(time) / 2) + 1);
		glUniform3fv(tintLoc, 1, &tint[0]);

		glClearColor(0.85f, 0.85f, 0.85f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(window);
	}

	return 0;
}