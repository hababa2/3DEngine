#include "FreeCameraController.h"
#include "Engine.h"

namespace nh
{
	void FreeCameraController::Update()
	{
		glm::vec3 rotate{ 0 };

		if (owner->scene->engine->Get<InputSystem>()->GetButtonState((int)InputSystem::eMouseButton::Left) == InputSystem::eKeyState::Held)
		{
			glm::vec2 axis = owner->scene->engine->Get<InputSystem>()->GetMouseRelative() * sensitivity;
			rotate.x -= glm::radians(axis.y);
			rotate.y -= glm::radians(axis.x);
		}

		owner->transform.rotation += rotate;

		glm::vec3 direction{ 0 };
		direction.x -= (owner->scene->engine->Get<nh::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == nh::InputSystem::eKeyState::Held);
		direction.x += (owner->scene->engine->Get<nh::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == nh::InputSystem::eKeyState::Held);
		direction.z -= (owner->scene->engine->Get<nh::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == nh::InputSystem::eKeyState::Held);
		direction.z += (owner->scene->engine->Get<nh::InputSystem>()->GetKeyState(SDL_SCANCODE_S) == nh::InputSystem::eKeyState::Held);
		direction.y -= (owner->scene->engine->Get<nh::InputSystem>()->GetKeyState(SDL_SCANCODE_Q) == nh::InputSystem::eKeyState::Held);
		direction.y += (owner->scene->engine->Get<nh::InputSystem>()->GetKeyState(SDL_SCANCODE_E) == nh::InputSystem::eKeyState::Held);

		glm::quat rotation = { owner->transform.rotation };

		owner->transform.position += (rotation * direction) * speed * owner->scene->engine->time.deltaTime;
	}

	bool FreeCameraController::Write(const rapidjson::Value& value) const
	{
		return false;
	}

	bool FreeCameraController::Read(const rapidjson::Value& value)
	{
		JSON_READ(value, speed);
		JSON_READ(value, sensitivity);

		return true;
	}
}
