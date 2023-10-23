#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"
#include <vector>



namespace nc
{
	struct light_s
	{
		enum lType
		{
			Point,
			Directional,
			Spotlight
		};

		lType type;
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 color;
		float cutoff;

	};
	class World04 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_angle = 0;
		float m_time;
		float m_speed = 5;

		light_s m_light;
		//glm::vec3 position{ 0, 8, 0 };
		//glm::vec3 color{ 1, 1, 1 };
		glm::vec3 ambientLight{ 0.05f, 0.05f, 0.05f };

		Transform m_transform;
		res_t<Model> m_model;

	};
}