#include "World08.h"

#include <glm/glm/gtx/color_space.hpp>

#include "Framework/Framework.h"
#include "Input/InputSystem.h"

namespace nc {
	bool World08::Initialize() {
		m_scene = std::make_unique<Scene>();
		m_scene->Load("Scenes/scene_editor.json");
		m_scene->Load("imaginationsarena/imaginations_arena.json");
		m_scene->Initialize();

		m_editor = std::make_unique<Editor>();

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

	void World08::Shutdown() {
	}

	void World08::Update(float deltaTime) {
		ENGINE.GetSystem<Gui>()->BeginFrame();

		m_scene->Update(deltaTime);
		m_editor->Update();
		m_editor->ProcessGUI(m_scene.get());

		auto program = GET_RESOURCE(Program, "Shaders/lit_phong_cel.prog"); // Find if any objects are using the cel shader
		if(program != nullptr) { // If yes, create an ImGui window to handle their parameters
			program->Use();

			// Create an ImGui window to allow the user to modify the cel shader's parameters easily
			// To see what effect these values have on the rendering, go to the lit_phong_cel.frag uniforms for some explanation
			ImGui::Begin("Cel");
			ImGui::SliderInt("Levels", &(this->celLevels), 1, 10);
			ImGui::SliderFloat("Specular Cutoff", &(this->celSpecularCutoff), 0.1f, 1.0f);
			ImGui::SliderFloat("Outline Cutoff", &(this->celOutline), 0.1f, 1.0f);
			ImGui::ColorEdit4("Outline Color", glm::value_ptr(this->celOutlineColor));
			ImGui::End();

			// Pass the uniforms to the shader
			program->SetUniform("celLevels", this->celLevels);
			program->SetUniform("celSpecularCutoff", this->celSpecularCutoff);
			program->SetUniform("celOutline", this->celOutline);
			program->SetUniform("celOutlineColor", this->celOutlineColor);
		}

		ENGINE.GetSystem<Gui>()->EndFrame();
	}

	void World08::Draw(Renderer& renderer) {
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
