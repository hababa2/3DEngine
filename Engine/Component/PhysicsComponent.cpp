#include "PhysicsComponent.h"

#include "Object/Actor.h"
#include "Engine.h"

namespace nh
{
	void PhysicsComponent::Update()
	{
		velocity += acceleration * owner->scene->engine->time.deltaTime;
		owner->transform.position += velocity * owner->scene->engine->time.deltaTime;
		owner->transform.position.x = nh::Wrap(owner->transform.position.x, -10.0f, 810.0f);
		owner->transform.position.y = nh::Wrap(owner->transform.position.y, -10.0f, 810.0f);
		velocity *= damping;

		acceleration = glm::vec3{ 0.0f, 0.0f, 0.0f };
	}

	bool PhysicsComponent::Write(const rapidjson::Value& value) const
	{
		return false;
	}

	bool PhysicsComponent::Read(const rapidjson::Value& value)
	{
		JSON_READ(value, damping);
		return true;
	}
}
