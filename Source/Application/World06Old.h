#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"
#include <vector>

namespace nc
{

	class World06Old : public World
	{
	public:
		const uint32_t INVERT_MASK = (1<<0);
		const uint32_t GREYSCALE_MASK = (1<<1);
		const uint32_t COLORTINT_MASK = (1<<2);



	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time;
		float m_speed = 5;
		float m_blend = 0.5f;

		uint32_t params{ 0 };

		float m_refraction{ 1 };
		glm::vec3 m_lightAmbient = { 1.0f,1.0f,1.0f };
	};
}