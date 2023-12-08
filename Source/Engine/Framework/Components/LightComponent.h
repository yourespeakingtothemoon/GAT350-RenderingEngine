#pragma once

#include "Component.h"

#include "Renderer/Program.h"

namespace nc {
	class LightComponent : public Component {
		public:
			CLASS_DECLARATION(LightComponent);

			bool Initialize() override;
			void Update(float dt) override;
			void SetProgram(const res_t<Program> program, const std::string& name, const glm::mat4& view);

			void ProcessGUI() override;

			glm::mat4 GetShadowMatrix();

		public:
			enum lightType {
				Point,
				Directional,
				Spot
			};

			lightType type = lightType::Point;
			glm::vec3 color{1};
			float intensity = 1.0f;
			float range = 20.0f;
			float innerAngle = 20.0f;
			float outerAngle = 30.0f;

			bool castShadows = false;
			float shadowSize = 10.0f;
			float shadowBias = 0.005f;
	};
}
