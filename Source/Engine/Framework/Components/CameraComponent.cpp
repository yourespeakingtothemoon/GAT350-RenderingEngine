#include "CameraComponent.h"

#include "Framework/Engine.h"
#include "Framework/Actor.h"

namespace nc {
	CLASS_DEFINITION(CameraComponent);

	bool CameraComponent::Initialize() {
		// get aspect from window if not set
		if(aspect == NULL) {
			// set aspect with renderer width / renderer height (make sure it is a floating point division)
			this->aspect = static_cast<float>(ENGINE.GetSystem<Renderer>()->GetWidth()) / ENGINE.GetSystem<Renderer>()->GetHeight();
		}

		return true;
	}

	void CameraComponent::Update(float deltaTime) {
		this->view = glm::lookAt(m_owner->transform.position, m_owner->transform.position + m_owner->transform.Forward(), m_owner->transform.Up());
		if(this->projectionType == projectionType::Perspective) {
			this->projection = glm::perspective(glm::radians(this->fov), this->aspect, this->near, this->far);
		} else {
			this->projection = glm::ortho(-(this->size * this->aspect) * 0.5f, this->size * this->aspect * 0.5f, -(this->size) * 0.5f, this->size * 0.5f, this->near, this->far);
		}
	}

	void CameraComponent::SetPerspective(float fov, float aspect, float near, float far) {
		// set this class fov, aspect, near and far values with parameters (this->fov = fov, ...)
		this->fov = fov;
		this->aspect = aspect;
		this->near = near;
		this->far = far;

		// set projection matrix with glm::perspective function (fov is in degrees, convert to radians)
		this->projection = glm::perspective(glm::radians(this->fov), this->aspect, this->near, this->far);
	}

	void CameraComponent::SetLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
		// set view matrix with glm::lookAt function
		this->view = glm::lookAt(eye, center, up);
	}

	void CameraComponent::SetProgram(res_t<Program> program) {
		// set program uniform for "view" with view matrix
		program->SetUniform("view", this->view);
		// set program uniform for "projection" with projection matrix
		program->SetUniform("projection", this->projection);
	}

	void CameraComponent::ProcessGUI() {
		const char* types[] = {"Perspective", "Orthographic"};
		ImGui::Combo("Projection", (int*) &(this->projectionType), types, 2);

		ImGui::DragFloat("FOV", &(this->fov), 0.1f);
		ImGui::DragFloat("Aspect Ratio", &(this->aspect), 0.1f);
		ImGui::DragFloat("Near Clip", &(this->near), 0.1f);
		ImGui::DragFloat("Far Clip", &(this->far), 0.1f);
		ImGui::DragFloat("Size", &(this->size), 0.1f);
	}

	void CameraComponent::Read(const json_t& value) {
		READ_DATA(value, fov);
		READ_NAME_DATA(value, "aspect_ratio", aspect);
		READ_NAME_DATA(value, "near_clip", near);
		READ_NAME_DATA(value, "far_clip", far);

		std::string projType = "";
		READ_NAME_DATA(value, "projectionType", projType);
		if(StringUtils::IsEqualIgnoreCase(projType, "Perspective")) {
			this->projectionType = projectionType::Perspective;
		} else if(StringUtils::IsEqualIgnoreCase(projType, "Orthographic")) {
			this->projectionType = projectionType::Orthographic;
		}

		READ_DATA(value, size);
	}
}