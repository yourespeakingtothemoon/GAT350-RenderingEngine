#include "CameraController.h"
#include "Framework/Actor.h"
#include "Framework/Engine.h"
#include "Input/InputSystem.h"

namespace nc
{
	CLASS_DEFINITION(CameraController)

	bool CameraController::Initialize()
	{
		glm::vec3 euler = QuaternionToEuler(m_owner->transform.rotation);

		Pitch = m_owner->transform.rotation.x;
		Yaw = m_owner->transform.rotation.y;

		return true;
	}

	void CameraController::Update(float dt)
	{
		// get camera rotation
		if (ENGINE.GetSystem<InputSystem>()->GetMouseButtonDown(2))
		{
			glm::vec2 axis = ENGINE.GetSystem<InputSystem>()->GetMouseRelative() * Sensitivity;

			Yaw += axis.x;
			Pitch -= axis.y;
			Pitch = glm::clamp(Pitch, -89.0f, 89.0f);
		}

		glm::vec3 forward;
		forward.x = cos(glm::radians(Yaw - 90.0f)) * cos(glm::radians(Pitch));
		forward.y = sin(glm::radians(Pitch));
		forward.z = sin(glm::radians(Yaw - 90.0f)) * cos(glm::radians(Pitch));
		forward = glm::normalize(forward);

		glm::mat4 view = glm::lookAt(glm::vec3{ 0.0f }, -forward, glm::vec3{ 0, 1, 0 });
		m_owner->transform.rotation = glm::conjugate(glm::quat_cast(view));

		glm::vec3 direction{ 0 };

		if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A)) direction.x += 1;
		if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D)) direction.x -= 1;
		if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E)) direction.y += 1;
		if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q)) direction.y -= 1;
		if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W)) direction.z += 1;
		if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S)) direction.z -= 1;

		// convert world direction space to camera space
		direction = m_owner->transform.rotation * direction;
		m_owner->transform.position += direction * (Speed * dt);
	}

	void CameraController::ProcessGui()
	{
		ImGui::DragFloat("Speed", &Speed, 0.1f);
		ImGui::DragFloat("Sensitivity", &Sensitivity, 0.1f);
	}

	void CameraController::Read(const json_t& value)
	{
		READ_DATA(value, Speed);
		READ_DATA(value, Sensitivity);
	}

}