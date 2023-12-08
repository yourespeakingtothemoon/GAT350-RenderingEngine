#include "LightComponent.h"

#include "Framework/Actor.h"

namespace nc {
	CLASS_DEFINITION(LightComponent);

	bool LightComponent::Initialize() {
		return true;
	}

	void LightComponent::Update(float dt) {
	}

	void LightComponent::SetProgram(const res_t<Program> program, const std::string& name, const glm::mat4& view) {
		// Transform light position and direction to camera space
		glm::vec3 position = glm::vec3(view * glm::vec4(m_owner->transform.position, 1));
		glm::vec3 direction = glm::vec3(view * glm::vec4(m_owner->transform.Forward(), 0));

		program->SetUniform(name + ".type", type);
		program->SetUniform(name + ".position", position);
		program->SetUniform(name + ".direction", direction);
		program->SetUniform(name + ".color", color);
		program->SetUniform(name + ".intensity", intensity);
		program->SetUniform(name + ".range", range);
		program->SetUniform(name + ".innerAngle", glm::radians(innerAngle));
		program->SetUniform(name + ".outerAngle", glm::radians(outerAngle));

		if(castShadows) {
			glm::mat4 bias = glm::mat4(
				glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
				glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
				glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
				glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

			program->SetUniform("shadowVP", bias * GetShadowMatrix());
			program->SetUniform("shadowBias", this->shadowBias);
		}
	}

	void LightComponent::ProcessGUI() {
		const char* types[] = {"Point", "Directional", "Spot"};
		ImGui::Combo("Type", (int*) (&type), types, 3);

		if(type == Spot) {
			ImGui::DragFloat("Inner Angle", &innerAngle, 1, 0, outerAngle);
			ImGui::DragFloat("Outer Angle", &outerAngle, 1, innerAngle, 90);
		}

		ImGui::ColorEdit3("Color", glm::value_ptr(color));
		ImGui::DragFloat("Intensity", &intensity, 0.1f, 0, 10);
		if(type != Directional) {
			ImGui::DragFloat("Range", &range, 0.1f, 0.1f, 50);
		}

		ImGui::Checkbox("Casts Shadows", &(this->castShadows));
		if(this->castShadows) {
			ImGui::DragFloat("Shadow Size", &(this->shadowSize), 0.1f, 1, 60);
			ImGui::DragFloat("Shadow Bias", &(this->shadowBias), 0.001f, 0, 0.5f);
		}
	}

	glm::mat4 LightComponent::GetShadowMatrix() {
		glm::mat4 projection = glm::ortho(-(this->shadowSize) * 0.5f, this->shadowSize * 0.5f, -(this->shadowSize) * 0.5f, this->shadowSize * 0.5f, 0.1f, 50.0f);
		glm::mat4 view = glm::lookAt(m_owner->transform.position, m_owner->transform.position + m_owner->transform.Forward(), m_owner->transform.Up());

		return projection * view;
	}

	void LightComponent::Read(const nc::json_t& value) {
		std::string lightType = "";
		READ_DATA(value, lightType);
		// I wanted to use a dang switch here but apparently c++ doesn't allow strings in switch statements
		if(StringUtils::IsEqualIgnoreCase(lightType, "Point")) {
			this->type = lightType::Point;
		} else if(StringUtils::IsEqualIgnoreCase(lightType, "Directional")) {
			this->type = lightType::Directional;
		} else if(StringUtils::IsEqualIgnoreCase(lightType, "Spot")) {
			this->type = lightType::Spot;
		}

		READ_DATA(value, color);
		READ_DATA(value, intensity);
		READ_DATA(value, range);
		READ_DATA(value, innerAngle);
		READ_DATA(value, outerAngle);

		READ_DATA(value, castShadows);
	}
}