#pragma once
#include "Framework/World.h"
#include "Core/Math/Vector2.h"

namespace nc
{
	class CheekyLittleDVDLogo : public World
	{
	public:
		bool Initialize() override;
		CheekyLittleDVDLogo() {
			velocityX = (rand() % 2 == 0 ? 1 : -1);  // Randomly choose left or right
			 velocityY = (rand() % 2 == 0 ? 1 : -1);  // Randomly choose up or down
		};
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_angle = 0;
		float m_time;
		vec2 m_position;
		int m_direction = 1;

		float velocityX = 0;
		float velocityY = 0;
	};
}