#pragma once

#include "Component.h"

namespace nc {
	class CameraController : public Component {
		public:
			CLASS_DECLARATION(CameraController);

			bool Initialize() override;
			void Update(float deltaTime) override;

			void ProcessGUI() override;

		public:
			float sensitivity = 0.1f;
			float speed = 3.0f;
			float yaw = 0.0f;
			float pitch = 0.0f;
	};
}