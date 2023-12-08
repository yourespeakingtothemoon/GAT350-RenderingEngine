#include "World07.h"

#include <glm/glm/gtx/color_space.hpp>

#include "Framework/Framework.h"
#include "Input/InputSystem.h"

namespace nc {
	bool World07::Initialize() {
		m_scene = std::make_unique<Scene>();
		m_scene->Load("Scenes/scene_shadow.json");
		m_scene->Initialize();

		int shadowRes = 8192;
		auto texture = std::make_shared<Texture>();
		texture->CreateDepthTexture(shadowRes, shadowRes);
		ADD_RESOURCE(Texture, "depth_texture", texture);

		auto framebuffer = std::make_shared<Framebuffer>();
		framebuffer->CreateDepthbuffer(texture);
		ADD_RESOURCE(Framebuffer, "depth_buffer", framebuffer);

		// Set depth texture to debug sprite
		auto material = GET_RESOURCE(Material, "Materials/sprite.mtrl");
		if(material != nullptr) {
			material->albedoTexture = texture;
		}

		//material = GET_RESOURCE(Material, "Fantasy/fantasy.mtrl");
		auto materials = GET_RESOURCES(Material);
		for(auto material : materials) {
			material->depthTexture = texture;
		}

		return true;
	}

	void World07::Shutdown() {
	}

	void World07::Update(float deltaTime) {
		ENGINE.GetSystem<Gui>()->BeginFrame();

		m_scene->Update(deltaTime);
		m_scene->ProcessGUI();

		ENGINE.GetSystem<Gui>()->EndFrame();
	}

	void World07::Draw(Renderer& renderer) {
		// ** PASS 1 **
		auto framebuffer = GET_RESOURCE(Framebuffer, "depth_buffer");
		renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
		framebuffer->Bind();

		renderer.ClearDepth();
		auto program = GET_RESOURCE(Program, "shaders/shadow_depth.prog");
		program->Use();
		
		auto lights = m_scene->GetComponents<LightComponent>();
		for(auto light : lights) {
			if(light->castShadows) {
				glm::mat4 shadowMatrix = light->GetShadowMatrix();
				program->SetUniform("shadowVP", shadowMatrix);
			}
		}

		auto models = m_scene->GetComponents<ModelComponent>();
		for(auto model : models) {
			if(model->castShadows) {
				//glCullFace(GL_FRONT);
				program->SetUniform("model", model->m_owner->transform.GetMatrix());
				model->model->Draw();
			}
		}

		framebuffer->Unbind();
		

		// ** PASS 2 **
		renderer.ResetViewport();
		renderer.BeginFrame();
		m_scene->Draw(renderer);

		// post-render
		ENGINE.GetSystem<Gui>()->Draw();
		renderer.EndFrame();
	}
}
