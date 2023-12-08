#pragma once

#include "Framework/World.h"

#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"

namespace nc {
	class World03 : public World {
		public:
			bool Initialize() override;
			void Shutdown() override;
			void Update(float deltaTime) override;
			void Draw(Renderer& renderer) override;

		private:
			//float time = 0.0f;
			float speed = 5.0f;

			//glm::vec2 texOffset = glm::vec2(0);
			//float texTiling = 1.0f;

			Transform transform;

			res_t<VertexBuffer> vertexBuffer;
			res_t<Material> material;
			res_t<Program> program;
			res_t<Texture> texture;
	};
}