#include "Engine.h"

#include <glad/glad.h>
#include <sdl.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <iostream>

int main(int argc, char** argv)
{
	// create engine
	std::unique_ptr<nh::Engine> engine = std::make_unique<nh::Engine>();
	engine->Startup();
	engine->Get<nh::Renderer>()->Create("OpenGL", 800, 600);

	// create scene
	std::unique_ptr<nh::Scene> scene = std::make_unique<nh::Scene>();
	scene->engine = engine.get();

	nh::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nh::SetFilePath("../Resources");

	// create camera
	{
		auto actor = CREATE_ENGINE_OBJECT(Actor);
		actor->name = "camera";
		actor->transform.position = glm::vec3{ 0, 0, 5 };

		{
			auto component = CREATE_ENGINE_OBJECT(CameraComponent);
			component->SetPerspective(45.0f, 800.0f / 600.0f, 0.01f, 100.0f);
			actor->AddComponent(std::move(component));
		}
		{
			auto component = CREATE_ENGINE_OBJECT(FreeCameraController);
			component->speed = 8;
			component->sensitivity = 0.1f;
			actor->AddComponent(std::move(component));
		}

		scene->AddActor(std::move(actor));
	}

	// create model
	{
		auto actor = CREATE_ENGINE_OBJECT(Actor);
		actor->name = "model";
		actor->transform.position = glm::vec3{ 0 };
		actor->transform.scale = glm::vec3{ 1 };

		auto component = CREATE_ENGINE_OBJECT(ModelComponent);
		component->model = engine->Get<nh::ResourceSystem>()->Get<nh::Model>("Models/spot.obj");
		component->material = engine->Get<nh::ResourceSystem>()->Get<nh::Material>("Materials/spot.mtl", engine.get());

		actor->AddComponent(std::move(component));
		scene->AddActor(std::move(actor));
	}

	// create light
	{
		auto actor = CREATE_ENGINE_OBJECT(Actor);
		actor->name = "light";
		actor->transform.position = glm::vec3{ 4 };

		auto component = CREATE_ENGINE_OBJECT(LightComponent);
		component->ambient = glm::vec3{ 0.2f };
		component->diffuse = glm::vec3{ 1 };
		component->specular = glm::vec3{ 1 };

		actor->AddComponent(std::move(component));
		scene->AddActor(std::move(actor));
	}

	glm::vec3 translate{ 0 };
	float angle = 0;

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
		engine->Update();
		scene->Update(engine->time.deltaTime);

		// update actor
		auto actor = scene->FindActor("model");
		if (actor != nullptr)
		{
			actor->transform.rotation.y += engine->time.deltaTime;
		}

		engine->Get<nh::Renderer>()->BeginFrame();

		scene->Draw(nullptr);

		engine->Get<nh::Renderer>()->EndFrame();
	}

	return 0;
}