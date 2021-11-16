#pragma once

#include "Object/Object.h"
#include "Core/Serializable.h"

namespace nh
{
	class Actor;

	class Component : public Object, public ISerializable
	{
	public:
		virtual void Update() = 0;

		virtual bool Write(const rapidjson::Value& value) const override = 0;
		virtual bool Read(const rapidjson::Value& value) override = 0;

	public:
		Actor* owner{ nullptr };
	};
}