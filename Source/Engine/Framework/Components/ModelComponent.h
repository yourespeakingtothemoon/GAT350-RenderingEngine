#pragma once
#include "RenderComponent.h"
#include "Renderer/Model.h"

namespace nc
{
	class ModelComponent : public RenderComponent
	{
	public:
		CLASS_DECLARATION(ModelComponent)

		bool Initialize() override;
		void Update(float dt) override;
		void Draw(class Renderer& renderer) override;
		void ProcessGui() override;


	public:
		std::string modelName;
		std::string materialName;
		res_t<Model> m_model;
		res_t<Material> m_material;

		bool castShadow{ true };
		bool enableDepth{ true };
		GLint cullface{ GL_BACK };
	};
}