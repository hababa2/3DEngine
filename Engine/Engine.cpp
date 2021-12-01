#include "Engine.h"

namespace nh 
{
	void Engine::Startup()
	{
		systems.push_back(std::make_unique<InputSystem>());
		systems.push_back(std::make_unique<Renderer>());
		systems.push_back(std::make_unique<AudioSystem>());
		systems.push_back(std::make_unique<ResourceSystem>());
		systems.push_back(std::make_unique<EventSystem>());

		for (auto& system : systems)
		{
			system->Startup();
		}

		REGISTER_CLASS(Actor);
		REGISTER_CLASS(PhysicsComponent);
		REGISTER_CLASS(AudioComponent);
		REGISTER_CLASS(MeshComponent);
		REGISTER_CLASS(CameraComponent);
		REGISTER_CLASS(ModelComponent);
		REGISTER_CLASS(FreeCameraController);
	}
	
	void Engine::Shutdown()
	{
		for (auto& system : systems)
		{
			system->Shutdown();
		}
	}
	
	void Engine::Update()
	{
		time.Tick();

		for (auto& system : systems)
		{
			system->Update(time.deltaTime);
		}
	}

	void Engine::Draw(Renderer* renderer)
	{
		for (auto& system : systems)
		{
			GraphicsSystem* gs = dynamic_cast<GraphicsSystem*>(system.get());
			if (gs) { gs->Draw(renderer); }
		}
	}
}