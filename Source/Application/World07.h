#pragma once

#include "Framework/World.h"

#include "Core/Math/Transform.h"
#include "Renderer/Renderer.h"

namespace nc {
	class World07 : public World {
		public:
			bool Initialize() override;
			void Shutdown() override;
			void Update(float deltaTime) override;
			void Draw(Renderer& renderer) override;

		private:
			glm::vec3 ambientLightColor = glm::vec3(0.2f);
	};
}