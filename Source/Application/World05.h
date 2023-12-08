#pragma once

#include "Framework/World.h"

#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"

namespace nc {
	class World05 : public World {
		public:
			bool Initialize() override;
			void Shutdown() override;
			void Update(float deltaTime) override;
			void Draw(Renderer& renderer) override;

		private:
			float speed = 1.0f;

			float refraction = 1.5f;

			glm::vec3 ambientLightColor = glm::vec3(0.2f);
	};
}