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

	nh::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nh::SetFilePath("../Resources");

	// create scene
	std::unique_ptr<nh::Scene> scene = std::make_unique<nh::Scene>();
	scene->engine = engine.get();

	// load scene
	rapidjson::Document document;
	bool success = nh::json::Load("Scenes/main.scn", document);
	scene->Read(document);

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
		//auto actor = scene->FindActor("model");
		//if (actor != nullptr)
		//{
		//	actor->transform.rotation.y += engine->time.deltaTime;
		//}
		auto actor = scene->FindActor("light");
		if (actor)
		{
			glm::mat3 rotation = glm::rotate(engine->time.deltaTime, glm::vec3{ 0, 0, 1 });
			actor->transform.position = actor->transform.position * rotation;

			
		}

		engine->Get<nh::Renderer>()->BeginFrame();

		scene->Draw(nullptr);

		engine->Get<nh::Renderer>()->EndFrame();
	}

	return 0;
}