#include "CameraComponent.h"
#include "Framework/Actor.h"
#include "Framework/Engine.h"

namespace nc
{
	CLASS_DEFINITION(CameraComponent)

	bool CameraComponent::Initialize()
	{
		
		if (AspectRatio == 0)
		{
		//get camera AspectRatio
			AspectRatio = static_cast<float>(Renderer().GetWidth()) / static_cast<float>((Renderer().GetHeight()));
		}

		return true;
	}

	void CameraComponent::Update(float deltaTime)
	{
		//get camera view
		view = glm::lookAt(m_owner->transform.position, m_owner->transform.position + m_owner->transform.Forward(), m_owner->transform.Up());
		
		//set camera projection
		projection = glm::perspective(glm::radians(FOV), AspectRatio, Near, Far);

	}

	void CameraComponent::SetPerspective(float fov, float ar, float n, float f)
	{
		//set camera perspective
		this->FOV = fov;
		this->AspectRatio = ar;
		this->Near = n;
		this->Far = f;
		projection = glm::perspective(glm::radians(FOV), AspectRatio, Near, Far);
	}

	void CameraComponent::SetLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
	{
		//set camera view
		view = glm::lookAt(eye, center, up);
	}

	void CameraComponent::SetProgram(res_t<Program> program)
	{
		//set camera shader
		program->SetUniform("view", view);
		program->SetUniform("projection", projection);
	}

	void CameraComponent::ProcessGui()
	{
		//gui time
		ImGui::DragFloat("Field of View", &FOV, 0.1f);
		
		ImGui::DragFloat("Near", &Near, 0.1f);
		ImGui::DragFloat("Far", &Far, 0.1f);
	}

	void CameraComponent::Read(const json_t& value)
	{
		// READ_DATA of fov, aspect, near and far values
		READ_DATA(value, FOV);
		READ_DATA(value, AspectRatio);
		READ_DATA(value, Near);
		READ_DATA(value, Far);
	}
}