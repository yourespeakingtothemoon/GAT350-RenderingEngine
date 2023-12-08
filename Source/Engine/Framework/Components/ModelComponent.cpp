#include "ModelComponent.h"

#include "Framework/Resource/ResourceManager.h"

#include "Core/StringUtils.h"
#include "Framework/Actor.h"

namespace nc {
	CLASS_DEFINITION(ModelComponent);

	bool ModelComponent::Initialize() {
		if(!modelName.empty()) {
			model = GET_RESOURCE(Model, modelName);
		}

		if(model != nullptr && !materialName.empty()) {
			material = GET_RESOURCE(Material, materialName);
		}

		return true;
	}

	void ModelComponent::Update(float dt) {
	}

	void ModelComponent::Draw(Renderer& renderer) {
		material->Bind();
		material->GetProgram()->SetUniform("model", m_owner->transform.GetMatrix());

		glDepthMask(this->enableDepth);
		glCullFace(this->cullface);

		model->Draw();
	}

	void ModelComponent::ProcessGUI() {
		(model != nullptr) ? ImGui::Text("Model: %s", model->name.c_str()) : ImGui::Text("None");
		Gui::GetDialogResource<Model>(model, "ModelTextureDialog", "Open Model", "Model file (*.obj;*.fbx){.obj,.fbx},.*");

		(material != nullptr) ? ImGui::Text("Material: %s", material->name.c_str()) : ImGui::Text("None");
		Gui::GetDialogResource<Material>(material, "MaterialTextureDialog", "Open Material", "Material file (*.mtrl){.mtrl},.*");

		ImGui::Checkbox("Cast Shadows", &(this->castShadows));
		ImGui::Checkbox("Enable Depth", &(this->enableDepth));
	}

	void ModelComponent::Read(const json_t& value) {
		READ_DATA(value, modelName);
		READ_DATA(value, materialName);

		READ_DATA(value, castShadows);
		READ_DATA(value, enableDepth);

		std::string cullfaceValue;
		if(READ_NAME_DATA(value, "cullface", cullfaceValue)) {
			if(StringUtils::IsEqualIgnoreCase(cullfaceValue, "front")) {
				cullface = GL_FRONT;
			} else if(StringUtils::IsEqualIgnoreCase(cullfaceValue, "back")) {
				cullface = GL_BACK;
			} else if(StringUtils::IsEqualIgnoreCase(cullfaceValue, "front_and_back")) {
				cullface = GL_FRONT_AND_BACK;
			}
		}
	}
}
