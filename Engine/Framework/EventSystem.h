#pragma once

#include "System.h"

#include <string>
#include <functional>
#include <map>
#include <list>
#include <variant>

namespace nh
{
	class Object;

	struct Event
	{
		std::string name;
		Object* receiver{ nullptr };
		std::variant<int, bool, float, std::string, void*> data;
	};

	class EventSystem : public System
	{
	public:
		typedef std::function<void(const Event&)> function_t;

	private:
		struct Observer
		{
			function_t fn;
			Object* receiver{ nullptr };
		};

	public:
		void Startup() override;
		void Shutdown() override;
		void Update(float dt) override;

		void Subscribe(const std::string& name, function_t fn, Object* receiver = nullptr);
		void Unsubscribe(const std::string& name, Object* receiver);
		void Notify(const Event& e);

	private:
		std::map<std::string, std::list<Observer>> observers;
	};
}