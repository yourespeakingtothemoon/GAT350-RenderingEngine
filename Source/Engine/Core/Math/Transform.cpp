#include "Transform.h"

#include "Renderer/Gui.h"

namespace nc {
	void Transform::ProcessGUI() {
		ImGui::DragFloat3("Position", glm::value_ptr(this->position), 0.1f);
		if(ImGui::DragFloat3("Rotation", glm::value_ptr(this->euler))) {
			rotation = EulerToQuaternion(euler);
		}
		ImGui::DragFloat3("Scale", glm::value_ptr(this->scale), 0.1f);
	}

	void Transform::Read(const json_t& value) {
		READ_DATA(value, position);
		READ_DATA(value, scale);

		READ_NAME_DATA(value, "rotation", euler);
		rotation = EulerToQuaternion(euler);
	}
}
