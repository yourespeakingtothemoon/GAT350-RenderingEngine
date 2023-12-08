#include "World06.h"

#include <glm/glm/gtx/color_space.hpp>

#include "Framework/Framework.h"
#include "Input/InputSystem.h"

namespace nc {
	bool World06::Initialize() {
		m_scene = std::make_unique<Scene>();
		m_scene->Load("Scenes/scene_framebuffer.json");
		m_scene->Initialize();

		auto texture = std::make_shared<Texture>();
		texture->CreateTexture(1024, 1024);
		ADD_RESOURCE(Texture, "fb_texture", texture);

		auto framebuffer = std::make_shared<Framebuffer>();
		framebuffer->CreateFramebuffer(texture);
		ADD_RESOURCE(Framebuffer, "fb", framebuffer);

		auto material = GET_RESOURCE(Material, "Materials/post_process.mtrl");
		if(material != nullptr) {
			material->albedoTexture = texture;
		}

		return true;
	}

	void World06::Shutdown() {
	}

	void World06::Update(float deltaTime) {
		ENGINE.GetSystem<Gui>()->BeginFrame();

		m_scene->Update(deltaTime);
		m_scene->ProcessGUI();

		auto program = GET_RESOURCE(Program, "Shaders/post_process.prog");
		if(program != nullptr) {
			program->Use();

			ImGui::Begin("Post Processing");
			ImGui::SliderFloat("Blend", &(this->blend), 0, 1);
			bool effect = this->params & INVERT_MASK;
			if(ImGui::Checkbox("Invert", &effect)) {
				if(effect) {
					this->params |= INVERT_MASK;
				} else {
					this->params &= ~INVERT_MASK;
				}
			}
			effect = this->params & GRAYSCALE_MASK;
			if(ImGui::Checkbox("Grayscale", &effect)) {
				this->params ^= GRAYSCALE_MASK;
			}
			effect = this->params & COLORTINT_MASK;
			if(ImGui::Checkbox("Color Tint", &effect)) {
				this->params ^= COLORTINT_MASK;
			}
			ImGui::ColorEdit3("Tint", glm::value_ptr(this->colorTint));
			effect = this->params & SCANLINE_MASK;
			if(ImGui::Checkbox("Scanlines", &effect)) {
				this->params ^= SCANLINE_MASK;
			}
			effect = this->params & GRAIN_MASK;
			if(ImGui::Checkbox("Grain", &effect)) {
				this->params ^= GRAIN_MASK;
			}
			effect = this->params & DEUTERANOPIA_MASK;
			if(ImGui::Checkbox("Deuteranopia (Red-Green Colorblindness)", &effect)) {
				this->params ^= DEUTERANOPIA_MASK;
			}
			ImGui::End();

			program->SetUniform("blend", this->blend);
			program->SetUniform("params", this->params);
			program->SetUniform("tint", this->colorTint);
		}

		ENGINE.GetSystem<Gui>()->EndFrame();
	}

	void World06::Draw(Renderer& renderer) {
		// ** PASS 1 **
		m_scene->GetActorByName("post_process")->active = false;

		auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
		renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
		framebuffer->Bind();

		renderer.BeginFrame(glm::vec3(0, 0, 0));
		m_scene->Draw(renderer);

		framebuffer->Unbind();

		// ** PASS 2 **
		m_scene->GetActorByName("post_process")->active = true;

		renderer.ResetViewport();
		renderer.BeginFrame();
		m_scene->GetActorByName("post_process")->Draw(renderer);

		//

		// post-render
		ENGINE.GetSystem<Gui>()->Draw();
		renderer.EndFrame();
	}
}
