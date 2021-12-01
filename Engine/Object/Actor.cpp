#include "Actor.h"

#include "Engine.h"

namespace nh
{
	Actor::Actor(const Actor& other)
	{
		destroy = other.destroy;
		name = other.name;
		tag = other.tag;

		transform = other.transform;
		scene = other.scene;

		for (auto& c : other.components)
		{
			auto clone = std::unique_ptr<Component>(dynamic_cast<Component*>(c->Clone().release()));
			clone->owner = this;
			clone->Create();
			AddComponent(std::move(clone));
		}
	}

	void Actor::Update(float dt)
	{
		if (active)
		{
			for (auto& c : components)
			{
				c->Update();
			}

			transform.Update();

			for (auto& c : children)
			{
				c->transform.Update(transform.matrix);
			}
		}
	}

	void Actor::Draw(Renderer* renderer)
	{
		if (active)
		{
			for (auto& c : components)
			{
				GraphicsComponent* g;
				if (g = dynamic_cast<GraphicsComponent*>(c.get()))
				{
					g->Draw(renderer);
				}
			}
	
			for (auto& c : children)
			{
				c->Draw(renderer);
			}
		}
	}

	void Actor::BeginContact(Actor* other)
	{
		Event event;

		event.name = "collision_enter";
		event.data = other;
		event.receiver = this;

		if (!destroy)
		{
			scene->engine->Get<EventSystem>()->Notify(event);
		}
	}

	void Actor::EndContact(Actor* other)
	{
		Event event;

		event.name = "collision_exit";
		event.data = other;
		event.receiver = this;

		if (!destroy)
		{
			scene->engine->Get<EventSystem>()->Notify(event);
		}
	}

	void Actor::AddChild(std::unique_ptr<Actor> a)
	{
		a->parent = this;
		children.push_back(std::move(a));
	}

	void Actor::AddComponent(std::unique_ptr<Component> cmp)
	{
		cmp->owner = this;
		components.push_back(std::move(cmp));
	}

	bool Actor::Write(const rapidjson::Value& value) const
	{
		return false;
	}

	bool Actor::Read(const rapidjson::Value& value)
	{
		JSON_READ(value, tag);
		JSON_READ(value, name);
		if (value.HasMember("transform"))
		{
			transform.Read(value["transform"]);
		}

		if (value.HasMember("components") && value["components"].IsArray())
		{
			for (auto& c : value["components"].GetArray())
			{
				std::string type;
				JSON_READ(c, type);

				auto component = ObjectFactory::Instance().Create<Component>(type);
				if (component)
				{
					component->owner = this;
					component->Read(c);
					component->Create();
					AddComponent(std::move(component));
				}
			}
		}

		return true;
	}
}