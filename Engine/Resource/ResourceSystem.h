#pragma once

#include "Framework/System.h"
#include "Resource/Resource.h"

#include <string>
#include <map>
#include <memory>
#include <algorithm>

namespace nh
{
	class ResourceSystem : public System
	{
	public:
		void Startup() override {}
		void Shutdown() override {}
		void Update(float dt) override {}

		template<typename T>
		std::shared_ptr<T> Get(const std::string& name, void* data = nullptr);

		void Add(const std::string& name, std::shared_ptr<Resource> resource);

	private:
		std::map<std::string, std::shared_ptr<Resource>> resources;
	};

	template<typename T>
	inline std::shared_ptr<T> ResourceSystem::Get(const std::string& name, void* data)
	{
		std::string n = nh::ToLower(name);
		if (resources.find(n) != resources.end())
		{
			return std::dynamic_pointer_cast<T>(resources[n]);
		}
		else
		{
			std::shared_ptr resource = std::make_shared<T>();
			resource->Load(name, data);
			resources[n] = resource;

			return resource;
		}
	}

	inline void ResourceSystem::Add(const std::string& name, std::shared_ptr<Resource> resource)
	{
		resources[nh::ToLower(name)] = resource;
	}
}