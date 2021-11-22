#include "Engine.h"

#include <glad/glad.h>
#include <sdl.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <iostream>

const float vertices[] =
{
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	 
	 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
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
	nh::SetFilePath("../Resources");

	std::shared_ptr<nh::Program> program = engine.Get<nh::ResourceSystem>()->Get<nh::Program>("basic_program");
	std::shared_ptr<nh::Shader> vshader = engine.Get<nh::ResourceSystem>()->Get<nh::Shader>("shaders/basic.vert", (void*)GL_VERTEX_SHADER);
	std::shared_ptr<nh::Shader> fshader = engine.Get<nh::ResourceSystem>()->Get<nh::Shader>("shaders/basic.frag", (void*)GL_FRAGMENT_SHADER);

	program->AddShader(vshader);
	program->AddShader(fshader);
	program->Link();
	program->Use();

	std::shared_ptr<nh::IndexBuffer> vertexBuffer = engine.Get<nh::ResourceSystem>()->Get<nh::IndexBuffer>("vertex_index_buffer");
	vertexBuffer->CreateVertexBuffer(sizeof(vertices), 4, (void*)vertices);
	vertexBuffer->CreateIndexBuffer(GL_UNSIGNED_INT, 6, (void*)indices);
	vertexBuffer->SetAttribute(0, 3, 8 * sizeof(GL_FLOAT), 0);
	vertexBuffer->SetAttribute(1, 3, 8 * sizeof(GL_FLOAT), (size_t)(3 * sizeof(GL_FLOAT)));
	vertexBuffer->SetAttribute(2, 2, 8 * sizeof(GL_FLOAT), (size_t)(6 * sizeof(GL_FLOAT)));

	//texture
	nh::Texture texture;
	texture.CreateTexture("Textures/llama.jpg");
	texture.Bind();

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

		vertexBuffer->Draw(GL_TRIANGLES);

		engine.Get<nh::Renderer>()->EndFrame();
	}

	return 0;
}