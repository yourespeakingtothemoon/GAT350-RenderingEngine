#pragma once

#include "Framework/World.h"
#include "Core/Math/Vector2.h"
#include "Renderer/Renderer.h"

namespace nc {
	class World02 : public World {
		public:
			bool Initialize() override;
			void Shutdown() override;
			void Update(float deltaTime) override;
			void Draw(Renderer& renderer) override;

		private:
			float angle = 0.0f;
			float time;
			Vector2 position;
			GLuint vao;
	};
}