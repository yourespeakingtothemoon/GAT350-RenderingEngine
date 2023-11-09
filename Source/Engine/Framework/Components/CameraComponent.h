#pragma once
#include "Component.h"
#include "Renderer/Program.h"

namespace nc
{
	class CameraComponent : public Component
	{
	public:
		CLASS_DECLARATION(CameraComponent)

		bool Initialize() override;
		void Update(float deltaTime) override;
		void ProcessGui() override;

		void SetPerspective(float fov, float ar, float n, float f);
		void SetLookAt(const glm::vec3& lens, const glm::vec3& center, const glm::vec3& up = { 0, 1, 0 });

		void SetProgram(const res_t<Program> Program);

	public:
		glm::mat4 projection{ 1 };
		glm::mat4 view{ 1 };

		float FOV = 70.0f;
		float AspectRatio = 0.0f;
		float Near = 0.1f;
		float Far = 100.0f;
	};
}
