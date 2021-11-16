#include "InputSystem.h"

namespace nh
{
	void InputSystem::Startup()
	{
		const Uint8* keyboardStateSDL = SDL_GetKeyboardState(&numKeys);
		keyboardState.resize(numKeys);
		std::copy(keyboardStateSDL + 0, keyboardStateSDL + numKeys, keyboardState.begin());
		prevKeyboardState = keyboardState;
	}

	void InputSystem::Shutdown()
	{

	}

	void InputSystem::Update(float dt)
	{
		prevKeyboardState = keyboardState;
		const Uint8* keyboardStateSDL = SDL_GetKeyboardState(nullptr);
		std::copy(keyboardStateSDL + 0, keyboardStateSDL + numKeys, keyboardState.begin());

		prevMouseButtonState = mouseButtonState;
		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);
		mousePosition = {x, y};
		mouseButtonState[0] = buttons & SDL_BUTTON_LMASK;
		mouseButtonState[1] = buttons & SDL_BUTTON_MMASK;
		mouseButtonState[2] = buttons & SDL_BUTTON_RMASK;
	}

	InputSystem::eKeyState InputSystem::GetKeyState(int id) const
	{
		eKeyState state = eKeyState::Idle;
		bool keyDown = IsKeyDown(id);
		bool prevKeyDown = IsPreviousKeyDown(id);

		if (keyDown) { state = (prevKeyDown) ? eKeyState::Held : eKeyState::Pressed; }
		else { state = (prevKeyDown) ? eKeyState::Release : eKeyState::Idle; }

		return state;
	}

	InputSystem::eKeyState InputSystem::GetButtonState(int id) const
	{
		eKeyState state = eKeyState::Idle;
		bool buttonDown = IsButtonDown(id);
		bool prevButtonDown = IsPreviousButtonDown(id);

		if (buttonDown) { state = (prevButtonDown) ? eKeyState::Held : eKeyState::Pressed; }
		else { state = (prevButtonDown) ? eKeyState::Release : eKeyState::Idle; }

		return state;
	}
}