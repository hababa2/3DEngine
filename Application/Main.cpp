#include "Engine.h"

#include <glad/glad.h>
#include <sdl.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <iostream>

const float vertices[] =
{
	// front
	-1.0f, -1.0f,  1.0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f,  1.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	 1.0f,  1.0f,  1.0, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-1.0f,  1.0f,  1.0, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	// back
	-1.0f, -1.0f, -1.0, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 1.0f, -1.0f, -1.0, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, -1.0, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, -1.0, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f
};

const GLuint indices[] =
{
	// front
	0, 1, 2,
	2, 3, 0,
	// right
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// left
	4, 0, 3,
	3, 7, 4,
	// bottom
	4, 5, 1,
	1, 0, 4,
	// top
	3, 2, 6,
	6, 7, 3
};

int main(int argc, char** argv)
{
	nh::Engine engine;
	engine.Startup();
	engine.Get<nh::Renderer>()->Create("OpenGL", 800, 600);

	// create scene
	std::unique_ptr<nh::Scene> scene = std::make_unique<nh::Scene>();
	scene->engine = &engine;

	nh::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nh::SetFilePath("../Resources");

	//shaders
	std::shared_ptr<nh::Program> program = engine.Get<nh::ResourceSystem>()->Get<nh::Program>("light_shader");
	std::shared_ptr<nh::Shader> vshader = engine.Get<nh::ResourceSystem>()->Get<nh::Shader>("shaders/light.vert", (void*)GL_VERTEX_SHADER);
	std::shared_ptr<nh::Shader> fshader = engine.Get<nh::ResourceSystem>()->Get<nh::Shader>("shaders/light.frag", (void*)GL_FRAGMENT_SHADER);

	program->AddShader(vshader);
	program->AddShader(fshader);
	program->Link();
	program->Use();

	//buffers
	std::shared_ptr<nh::VertexBuffer> vertexBuffer = engine.Get<nh::ResourceSystem>()->Get<nh::VertexBuffer>("cube_mesh");
	vertexBuffer->CreateVertexBuffer(sizeof(vertices), 8, (void*)vertices);
	vertexBuffer->CreateIndexBuffer(GL_UNSIGNED_INT, 36, (void*)indices);
	vertexBuffer->SetAttribute(0, 3, 8 * sizeof(GL_FLOAT), 0);
	vertexBuffer->SetAttribute(1, 3, 8 * sizeof(GL_FLOAT), (size_t)(3 * sizeof(GL_FLOAT)));
	vertexBuffer->SetAttribute(2, 2, 8 * sizeof(GL_FLOAT), (size_t)(6 * sizeof(GL_FLOAT)));

	//texture
	{
		auto texture = engine.Get<nh::ResourceSystem>()->Get<nh::Texture>("textures/llama.jpg");
		texture = engine.Get<nh::ResourceSystem>()->Get<nh::Texture>("textures/rocks.bmp");
		texture = engine.Get<nh::ResourceSystem>()->Get<nh::Texture>("textures/ogre.bmp");
		texture = engine.Get<nh::ResourceSystem>()->Get<nh::Texture>("textures/spot.png");
		texture->Bind();
	}

	//camera
	{
		auto actor = nh::ObjectFactory::Instance().Create<nh::Actor>("Actor");
		actor->name = "camera";
		actor->transform.position = glm::vec3{ 0, 0, 5 };

		{
			auto component = nh::ObjectFactory::Instance().Create<nh::CameraComponent>("CameraComponent");
			component->SetPerspective(45.0f, 800.0f / 600.0f, 0.01f, 100.0f);
			actor->AddComponent(std::move(component));
		}

		{
			auto component = nh::ObjectFactory::Instance().Create<nh::FreeCameraController>("FreeCameraController");
			component->speed = 3;
			component->sensitivity = 0.1f;
			actor->AddComponent(std::move(component));
		}

		scene->AddActor(std::move(actor));
	}

	//cube
	{
		auto actor = nh::ObjectFactory::Instance().Create<nh::Actor>("Actor");
		actor->name = "cube";
		actor->transform.position = glm::vec3{ 0 };
		actor->transform.scale = glm::vec3{ 1 };

		auto component = nh::ObjectFactory::Instance().Create<nh::ModelComponent>("ModelComponent");
		component->program = engine.Get<nh::ResourceSystem>()->Get<nh::Program>("light_shader");
		component->model = engine.Get<nh::ResourceSystem>()->Get<nh::Model>("models/spot.obj");

		actor->AddComponent(std::move(component));
		scene->AddActor(std::move(actor));
	}

	auto shader = engine.Get<nh::ResourceSystem>()->Get<nh::Program>("light_shader");
	shader->SetUniform("light.ambient", glm::vec3{ 1});
	shader->SetUniform("light.material", glm::vec3{ 1 });
	shader->SetUniform("light.spectular", glm::vec3{ 1 });
	shader->SetUniform("material.ambient", glm::vec3{ 1 });
	shader->SetUniform("material.material", glm::vec3{ 1 });
	shader->SetUniform("material.spectular", glm::vec3{ 1 });

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

		engine.Update();
		scene->Update(engine.time.deltaTime);

		auto actor = scene->FindActor("cube");
		if (actor != nullptr)
		{
			actor->transform.rotation.y += engine.time.deltaTime;
		}

		engine.Get<nh::Renderer>()->BeginFrame();

		scene->Draw(engine.Get<nh::Renderer>());

		engine.Get<nh::Renderer>()->EndFrame();
	}

	return 0;
}