#pragma once
#include "Components/Component.h"

namespace nc {
	class RotateComponent : public Component 
	{
	public:
		CLASS_DECLARATION(RotateComponent)

		void ProcessGui();
		void Update(float dt);

	public:
		glm::vec3 EulerAngle{ 0 };
	};
}