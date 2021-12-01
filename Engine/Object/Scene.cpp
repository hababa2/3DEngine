#include "Scene.h"

#include "Engine.h"

namespace nh
{
	void Scene::Update(float dt)
	{
		for (auto it = actors.begin(); it != actors.end();)
		{
			(*it)->Update(dt);

			if ((*it)->destroy) { it = actors.erase(it); }
			else { ++it; }
		}
	}

	void Scene::Draw(Renderer* renderer)
	{
		for (auto& a : actors)
		{
			a->Draw(renderer);
		}
	}

	void Scene::AddActor(std::unique_ptr<Actor> actor)
	{
		actor->scene = this;
		actors.push_back(std::move(actor));
	}

	void Scene::RemoveActor(Actor* actor)
	{
		for (auto it = actors.begin(); it != actors.end(); ++it)
		{
			if ((*it).get() == actor) { actors.erase(it); return; }
		}
	}

	void Scene::RemoveAll()
	{
		actors.clear();
	}

	bool Scene::Write(const rapidjson::Value& value) const
	{
		return false;
	}

	bool Scene::Read(const rapidjson::Value& value)
	{
		if (value.HasMember("actors") && value["actors"].IsArray())
		{
			for (auto& a : value["actors"].GetArray())
			{
				std::string type;
				JSON_READ(a, type);

				bool prototype = false;
				JSON_READ(a, prototype);

				auto actor = ObjectFactory::Instance().Create<Actor>(type);
				if (actor)
				{
					actor->scene = this;
					actor->Read(a);

					if (prototype)
					{
						std::string name = actor->name;
						ObjectFactory::Instance().RegisterPrototype<Actor>(name, std::move(actor));
					}
					else
					{
						AddActor(std::move(actor));
					}
				}
			}
		}

		return true;
	}

	Actor* Scene::FindActor(const std::string& name)
	{
		for (auto& a : actors)
		{
			if (a->name == name)
			{
				return a.get();
			}
		}

		return nullptr;
	}
}