#include "World04.h"

#include <glm/glm/gtx/color_space.hpp>

#include "Framework/Framework.h"
#include "Input/InputSystem.h"

namespace nc {
	bool World04::Initialize() {
		this->material = GET_RESOURCE(Material, "Materials/squirrel.mtrl");
		this->model = std::make_shared<Model>();
		//this->model->Load("Models/plane.obj", glm::vec3(0, -1, 0));
		//this->model->Load("Models/plane.obj", glm::vec3(0), glm::vec3(90, 0, 0));
		//this->model->Load("Models/buddha.obj", glm::vec3(0), glm::vec3(-90, 0, 0));
		this->model->Load("Models/squirrel.glb", glm::vec3(0, -0.7f, 0), glm::vec3(0), glm::vec3(0.4f));

		for(int i = 0; i < 3; i++) {
			this->lights[i].type = light_t::lightType::Point;
			this->lights[i].position = glm::vec3(randomf(-5, 5), randomf(1, 8), randomf(-5, 5));
			this->lights[i].direction = glm::vec3(0, -1, 0);
			this->lights[i].color = glm::rgbColor(glm::vec3(randomf() * 360, 1, 1)); //glm::vec3(1);
			this->lights[i].intensity = 0.4f;
			this->lights[i].range = 15.0f;
			this->lights[i].innerAngle = 10.0f;
			this->lights[i].outerAngle = 30.0f;
		}

		return true;
	}

	void World04::Shutdown() {
	}

	void World04::Update(float deltaTime) {
		ENGINE.GetSystem<Gui>()->BeginFrame();

		this->transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_UP) ? 90 * -deltaTime : 0;
		this->transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_DOWN) ? 90 * deltaTime : 0;
		this->transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_PAGEUP) ? 90 * -deltaTime : 0;
		this->transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_PAGEDOWN) ? 90 * deltaTime : 0;
		this->transform.rotation.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_LEFT) ? 90 * deltaTime : 0;
		this->transform.rotation.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_RIGHT) ? 90 * -deltaTime : 0;

		this->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? this->speed * +deltaTime : 0;
		this->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? this->speed * -deltaTime : 0;
		this->transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? this->speed * -deltaTime : 0;
		this->transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? this->speed * +deltaTime : 0;
		this->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? this->speed * +deltaTime : 0;
		this->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? this->speed * -deltaTime : 0;

		//this->time += deltaTime;

		this->material->ProcessGUI();
		this->material->Bind();

		ImGui::Begin("Scene");
		ImGui::ColorEdit3("Ambient Color", glm::value_ptr(this->ambientLightColor));
		ImGui::Separator();

		for(int i = 0; i < 3; i++) {
			std::string name = "Light " + std::to_string(i + 1);
			if(ImGui::Selectable(name.c_str(), this->lightSelected == i)) {
				this->lightSelected = i;
			}
		}

		ImGui::End();

		ImGui::Begin("Light");
		const char* types[] = {"Point", "Directional", "Spot"};
		ImGui::Combo("Type", (int*) &(this->lights[this->lightSelected].type), types, 3);

		//ImGui::ColorEdit3("Ambient Light Color", glm::value_ptr(this->ambientLightColor));

		ImGui::ColorEdit3("Diffuse Light Color", glm::value_ptr(this->lights[this->lightSelected].color));

		ImGui::DragFloat("Diffuse Light Intensity", &(this->lights[this->lightSelected].intensity), 0.1f, 0, 10);


		if(this->lights[this->lightSelected].type != light_t::lightType::Point) {
			ImGui::DragFloat3("Diffuse Light Direction", glm::value_ptr(this->lights[this->lightSelected].direction), 0.25f);
		}

		if(this->lights[this->lightSelected].type != light_t::lightType::Directional) {
			ImGui::DragFloat3("Diffuse Light Position", glm::value_ptr(this->lights[this->lightSelected].position), 0.25f);
			ImGui::DragFloat("Diffuse Light Range", &(this->lights[this->lightSelected].range), 0.1f, 0.1f, 50);
		}

		if(this->lights[this->lightSelected].type == light_t::lightType::Spot) {
			ImGui::DragFloat("Diffuse Light Inner Angle", &(this->lights[this->lightSelected].innerAngle), 1, 0, this->lights[this->lightSelected].outerAngle);
			ImGui::DragFloat("Diffuse Light Outer Angle", &(this->lights[this->lightSelected].outerAngle), 1, this->lights[this->lightSelected].innerAngle, 90);
		}

		ImGui::End();

		// Lights
		material->GetProgram()->SetUniform("ambientLight", this->ambientLightColor);

		for(int i = 0; i < 3; i++) {
			std::string name = "lights[" + std::to_string(i) + "]";

			material->GetProgram()->SetUniform(name + ".type", this->lights[i].type);
			material->GetProgram()->SetUniform(name + ".position", this->lights[i].position);
			material->GetProgram()->SetUniform(name + ".direction", glm::normalize(this->lights[i].direction));
			material->GetProgram()->SetUniform(name + ".color", this->lights[i].color);
			material->GetProgram()->SetUniform(name + ".intensity", this->lights[i].intensity);
			material->GetProgram()->SetUniform(name + ".range", this->lights[i].range);
			material->GetProgram()->SetUniform(name + ".innerAngle", glm::radians(this->lights[i].innerAngle));
			material->GetProgram()->SetUniform(name + ".outerAngle", glm::radians(this->lights[i].outerAngle));
		}

		// Model
		material->GetProgram()->SetUniform("model", this->transform.GetMatrix());

		// View
		glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		material->GetProgram()->SetUniform("view", view);

		// Projection
		glm::mat4 projection = glm::perspective(glm::radians(70.0f), static_cast<float>(ENGINE.GetSystem<Renderer>()->GetWidth()) / ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 100.0f);
		material->GetProgram()->SetUniform("projection", projection);

		ENGINE.GetSystem<Gui>()->EndFrame();
	}

	void World04::Draw(Renderer& renderer) {
		// pre-render
		renderer.BeginFrame();

		// render
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		this->material->Bind();
		this->model->Draw(GL_TRIANGLES);

		ENGINE.GetSystem<Gui>()->Draw();

		// post-render
		renderer.EndFrame();
	}
}
