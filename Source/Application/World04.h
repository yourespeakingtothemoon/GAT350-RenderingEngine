#pragma once

#include "Framework/World.h"

#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"

namespace nc {
	struct light_t {
		enum lightType {
			Point,
			Directional,
			Spot
		};

		lightType type;
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 color;
		float intensity;
		float range;
		float innerAngle;
		float outerAngle;
	};

	class World04 : public World {
		public:
			bool Initialize() override;
			void Shutdown() override;
			void Update(float deltaTime) override;
			void Draw(Renderer& renderer) override;

		private:
			float speed = 1.0f;

			light_t lights[3];
			int lightSelected = 0;
			glm::vec3 ambientLightColor = glm::vec3(0.2f);

			Transform transform;

			res_t<Model> model;
			res_t<Material> material;
	};
}