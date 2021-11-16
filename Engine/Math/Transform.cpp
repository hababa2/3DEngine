#include "Transform.h"

namespace nh
{
	void Transform::Update()
	{
		glm::mat4 mxs = glm::scale(scale);
		glm::mat4 mxt = glm::translate(position);

		matrix = mxs * mxt;
	}

	void Transform::Update(const glm::mat4& mx)
	{
		glm::mat4 mxs = glm::scale(localScale);
		glm::mat4 mxt = glm::translate(localPosition);

		matrix = mxs * mxt * mx;
	}


	bool Transform::Write(const rapidjson::Value& value) const
	{
		return false;
	}

	bool Transform::Read(const rapidjson::Value& value)
	{
		JSON_READ(value, position);
		JSON_READ(value, rotation);
		JSON_READ(value, scale);

		return true;
	}
}