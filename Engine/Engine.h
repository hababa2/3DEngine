#pragma once

//Core
#include "Core/Utilities.h"
#include "Core/FileSystem.h"
#include "Core/Timer.h"
#include "Core/Serializable.h"

//Math
#include "Math/MathTypes.h"
#include "Math/Random.h"
#include "Math/MathUtils.h"
#include "Math/Transform.h"

//Framework
#include "Framework/EventSystem.h"
#include "Framework/Singleton.h"
#include "Framework/Factory.h"

//Graphics
#include "Graphics/Renderer.h"
#include "Graphics/Texture.h"
#include "Graphics/Program.h"
#include "Graphics/Shader.h"

//Audio
#include "Audio/AudioSystem.h"

//Input
#include "Input/InputSystem.h"

//Resource
#include "Resource/ResourceSystem.h"

//Objects
#include "Object/Actor.h"
#include "Object/Scene.h"

//Components
#include "Component/AudioComponent.h"
#include "Component/PhysicsComponent.h"

#include <vector>
#include <memory>
#include <algorithm>

#define REGISTER_CLASS(T) nh::ObjectFactory::Instance().Register<T>(#T)

namespace nh
{
	typedef Singleton<Factory<std::string, Object>> ObjectFactory;

	class Engine
	{
	public:
		void Startup();
		void Shutdown();

		void Update();

		void Draw(Renderer* renderer);

		template<typename T>
		T* Get();

	public:
		FrameTimer time;

	private:
		std::vector<std::unique_ptr<System>> systems;
	};

	template<typename T>
	inline T* Engine::Get()
	{
		for (const auto& system : systems)
		{
			T* t = dynamic_cast<T*>(system.get());
			if (t) { return t; }
		}

		return nullptr;
	}
}