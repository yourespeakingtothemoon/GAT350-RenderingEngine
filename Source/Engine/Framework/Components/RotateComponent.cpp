#include "RotateComponent.h"

#include "Core/Math/MathUtils.h"
#include "Framework/Actor.h"

namespace nc {
	CLASS_DEFINITION(RotateComponent);

	bool RotateComponent::Initialize() {
		return true;
	}

	void RotateComponent::Update(float deltaTime) {
		glm::quat rotation = EulerToQuaternion(this->euler * deltaTime);
		m_owner->transform.rotation *= rotation;
		m_owner->transform.euler += this->euler * deltaTime;
	}

	void RotateComponent::ProcessGUI() {
		ImGui::DragFloat3("Euler Rotation", glm::value_ptr(this->euler), 0.01f);
	}

	void RotateComponent::Read(const json_t& value) {
		READ_NAME_DATA(value, "rotation", euler);
	}
}