#pragma once
#include "Component.h"

namespace nc
{
	class CameraController : public Component
	{
	public:
		CLASS_DECLARATION(CameraController)

		bool Initialize() override;
		void Update(float dt) override;
		void ProcessGui();

	public:
		float Sensitivity = 0.1f;
		float Speed = 0;
		float Yaw = 0;
		float Pitch = 0;
	};
}