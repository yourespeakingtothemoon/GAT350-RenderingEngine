#include "ModelComponent.h"
#include "Framework/Actor.h"
#include "Framework/Resource/ResourceManager.h"

namespace nc
{
	CLASS_DEFINITION(ModelComponent)

	bool ModelComponent::Initialize()
	{
		//if (!modelName.empty()) m_model = GET_RESOURCE(Model, modelName);

		if (!modelName.empty())
		{
			model = std::make_shared<Model>();
			model->Load(modelName);
		//	ADD_RESOURCE(modelName, model);
		}
		if (model && !materialName.empty())
		{
			model->SetMaterial(GET_RESOURCE(Material, materialName));
		}

		return true;
	}

	void ModelComponent::Update(float dt)
	{
	}

	void ModelComponent::Draw(Renderer& renderer)
	{
		auto material = model->GetMaterial();
		material->Bind();
		material->GetProgram()->SetUniform("model", m_owner->transform.GetMatrix());

		glDepthMask(enableDepth);
		glCullFace(cullface);
		model->Draw();
		
	}

	void ModelComponent::Read(const json_t& value)
	{
		READ_DATA(value, modelName);
		READ_DATA(value, materialName);
	//	bool enableDepth{ true };
		READ_DATA(value, enableDepth);
		std::string cullfaceName;
		READ_NAME_DATA(value, "cullface", cullfaceName);
		if (IsEqualIgnoreCase(cullfaceName, "Front")) cullface = GL_FRONT;
		if (IsEqualIgnoreCase(cullfaceName, "None")) cullface = GL_NONE;

	//	depthTest = enableDepth;

	}
}
