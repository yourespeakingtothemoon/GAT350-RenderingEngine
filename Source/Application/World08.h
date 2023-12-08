#pragma once

#include "Framework/World.h"

#include "Core/Math/Transform.h"
#include "Renderer/Renderer.h"

namespace nc {
	class World08 : public World {
		public:
			bool Initialize() override;
			void Shutdown() override;
			void Update(float deltaTime) override;
			void Draw(Renderer& renderer) override;

		private:
			// Values to be passed to the cel shader as uniforms
			int celLevels = 5;
			float celSpecularCutoff = 0.3f;
			float celOutline = 0.3f;
			glm::vec4 celOutlineColor = glm::vec4(1);

			glm::vec3 ambientLightColor = glm::vec3(0.2f);
	};
}