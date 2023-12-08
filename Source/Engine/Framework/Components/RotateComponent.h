#pragma once

#include "Component.h"

namespace nc {
	class RotateComponent : public Component {
		public:
			CLASS_DECLARATION(RotateComponent);

			bool Initialize() override;
			void Update(float deltaTime) override;

			void ProcessGUI() override;

		public:
			glm::vec3 euler = glm::vec3(0);
	};
}