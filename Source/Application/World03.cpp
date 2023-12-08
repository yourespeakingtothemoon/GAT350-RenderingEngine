#include "World03.h"

#include "Framework/Framework.h"
#include "Input/InputSystem.h"

namespace nc {
	bool World03::Initialize() {
		this->material = GET_RESOURCE(Material, "Materials/quad.mtrl");
		
		// Vertex data
		float vertexData[] = {
			//    Position            Color           UV
			-0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			 0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			-0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
			-0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			 0.8f, -0.8f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			 0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f
		};

		this->vertexBuffer = std::make_shared<VertexBuffer>();
		this->vertexBuffer->CreateVertexBuffer(sizeof(vertexData), 6, vertexData);
		this->vertexBuffer->SetAttribute(0, 3, sizeof(GLfloat) * 8, 0);                   // position
		this->vertexBuffer->SetAttribute(1, 3, sizeof(GLfloat) * 8, sizeof(GLfloat) * 3); // color
		this->vertexBuffer->SetAttribute(2, 2, sizeof(GLfloat) * 8, sizeof(GLfloat) * 6); // uv

		return true;
	}

	void World03::Shutdown() {
	}

	void World03::Update(float deltaTime) {
		ENGINE.GetSystem<Gui>()->BeginFrame();

		ImGui::Begin("Transform");
		ImGui::DragFloat3("Position", &this->transform.position[0]);
		ImGui::DragFloat3("Rotation", &this->transform.rotation[0]);
		ImGui::DragFloat3("Scale", &this->transform.scale[0]);
		ImGui::End();

		//ImGui::Begin("Texture");
		//ImGui::DragFloat("Offset X", &this->texOffset[0]);
		//ImGui::DragFloat("Offset Y", &this->texOffset[1]);
		//ImGui::DragFloat("Tiling", &this->texTiling);
		//ImGui::End();

		this->transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_UP) ? 90 * -deltaTime : 0;
		this->transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_DOWN) ? 90 * deltaTime : 0;
		this->transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_PAGEUP) ? 90 * deltaTime : 0;
		this->transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_PAGEDOWN) ? 90 * -deltaTime : 0;
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
		
		// UV Offset
		//this->program->SetUniform("offset", this->texOffset);

		// Tiling
		//this->program->SetUniform("tiling", glm::vec2(this->texTiling));

		// Model
		this->material->GetProgram()->SetUniform("model", this->transform.GetMatrix());

		// View
		glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		this->material->GetProgram()->SetUniform("view", view);

		// Projection
		glm::mat4 projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.01f, 100.0f);
		this->material->GetProgram()->SetUniform("projection", projection);

		ENGINE.GetSystem<Gui>()->EndFrame();
	}

	void World03::Draw(Renderer& renderer) {
		// pre-render
		renderer.BeginFrame();

		// render
		this->vertexBuffer->Draw(GL_TRIANGLES);

		ENGINE.GetSystem<Gui>()->Draw();

		// post-render
		renderer.EndFrame();
	}
}
