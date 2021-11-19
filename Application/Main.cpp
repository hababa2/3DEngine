#include "Engine.h"

#include <glad/glad.h>
#include <sdl.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <iostream>

const float vertices[] =
{
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f
};

const GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};

int main(int argc, char** argv)
{
	nh::Engine engine;
	engine.Startup();
	engine.Get<nh::Renderer>()->Create("OpenGL", 800, 600);

	nh::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nh::SetFilePath("../resources");

	std::shared_ptr<nh::Program> program = engine.Get<nh::ResourceSystem>()->Get<nh::Program>("basic_program");
	std::shared_ptr<nh::Shader> vshader = engine.Get<nh::ResourceSystem>()->Get<nh::Shader>("shaders/basic.vert", (void*)GL_VERTEX_SHADER);
	std::shared_ptr<nh::Shader> fshader = engine.Get<nh::ResourceSystem>()->Get<nh::Shader>("shaders/basic.frag", (void*)GL_FRAGMENT_SHADER);

	program->AddShader(vshader);
	program->AddShader(fshader);
	program->Link();
	program->Use();

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLubyte*)NULL);
	glEnableVertexAttribArray(0);

	//color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLubyte*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	//uniform
	float time = 0;
	glm::vec3 tint{ 1.0f, 1.0f, 1.0f };
	program->SetUniform("scale", time);
	program->SetUniform("tint", tint);

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
		program->SetUniform("scale", (std::sin(time) / 2) + 1);

		engine.Get<nh::Renderer>()->BeginFrame();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		engine.Get<nh::Renderer>()->EndFrame();
	}

	return 0;
}