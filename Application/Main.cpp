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

	float time = 0;

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
		time += engine->time.deltaTime;

		// update actor
		auto actor = scene->FindActor("model");
		if (actor != nullptr)
		{
			actor->transform.rotation = actor->transform.rotation + glm::vec3{ 0, engine->time.deltaTime, 0 };
			auto component = actor->GetComponent<nh::ModelComponent>();
		}

		//update shader
		auto shader = engine->Get<nh::ResourceSystem>()->Get<nh::Program>("Shaders/effect.shdr");
		if (shader)
		{
			shader->Use();
			shader->SetUniform("time", time);
			shader->SetUniform("uv.tiling", glm::vec2{ 1 });
			shader->SetUniform("uv.offset", glm::vec2{ 0 });
			shader->SetUniform("strength", (std::sin(time * 4) + 1.0f) * 0.5f);
			shader->SetUniform("radius", 0.5f);
		}

		engine->Get<nh::Renderer>()->BeginFrame();

		scene->Draw(nullptr);

		engine->Get<nh::Renderer>()->EndFrame();
	}

	return 0;
}