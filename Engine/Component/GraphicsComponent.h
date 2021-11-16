#pragma once

#include "Component/Component.h"

namespace nh
{
	class GraphicsComponent : public Component
	{
	public:
		virtual void Draw(class Renderer* renderer) = 0;
	};
}