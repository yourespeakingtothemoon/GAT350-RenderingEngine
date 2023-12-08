#include "Editor.h"

#include "Components/CameraComponent.h"
#include "Scene.h"

namespace nc {
	void Editor::Update() {
		if(ImGui::IsKeyPressed(ImGuiKey_GraveAccent)) {
			this->active = !(this->active);
		}
	}
	void Editor::ProcessGUI(Scene* scene) {
		if(!(this->active)) {
			return;
		}
		ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(1, 1, 0, 1));

//Resource Filter
		ImGui::Begin("Resource Filter");

		// Textures
		if(ImGui::CollapsingHeader("Textures")) {
			auto resources = GET_RESOURCES(Texture);
			for(auto& resource : resources) {
				if(ImGui::Selectable(resource->name.c_str(), resource.get() == this->selectedObject)) {
					this->selectedObject = resource.get();
				}
			}
		}

		// Models
		if(ImGui::CollapsingHeader("Models")) {
			auto resources = GET_RESOURCES(Model);
			for(auto& resource : resources) {
				if(ImGui::Selectable(resource->name.c_str(), resource.get() == this->selectedObject)) {
					this->selectedObject = resource.get();
				}
			}
		}

		// Materials
		if(ImGui::CollapsingHeader("Materials")) {
			auto resources = GET_RESOURCES(Material);
			for(auto& resource : resources) {
				if(ImGui::Selectable(resource->name.c_str(), resource.get() == this->selectedObject)) {
					this->selectedObject = resource.get();
				}
			}
		}

		// Shaders
		if(ImGui::CollapsingHeader("Shaders")) {
			auto resources = GET_RESOURCES(Shader);
			for(auto& resource : resources) {
				if(ImGui::Selectable(resource->name.c_str(), resource.get() == this->selectedObject)) {
					this->selectedObject = resource.get();
				}
			}
		}

		ImGui::End();

		// Scene GUI
		ImGui::Begin("Scene");
		scene->ProcessGUI();
		ImGui::Separator();

		// Actor List
		int index = 0;
		for(auto& actor : scene->m_actors) {
			bool done = false;
			ImGui::PushID(index++);

	//Rename Actor
			if(actor.get() == this->renameObject) {
				if(ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)) {
					ImGui::SetKeyboardFocusHere(0);
				}
				char name[32];
				strcpy_s(name, 32, actor->name.c_str());
				if(ImGui::InputText("", name, 32, ImGuiInputTextFlags_EnterReturnsTrue)) {
					actor->name = name;
					this->renameObject = nullptr;
				}
			} else if(ImGui::Selectable(actor->name.c_str(), actor.get() == this->selectedObject)) {
				this->selectedObject = actor.get();
			}
			if(actor.get() == this->selectedObject && ImGui::IsMouseDoubleClicked(0)) {
				this->renameObject = actor.get();
			}
			if(ImGui::BeginPopupContextItem(actor->name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
				if(ImGui::MenuItem("Duplicate")) {
					auto newActor = dynamic_cast<Actor*>(actor->Clone().release());
					int number = StringUtils::GetTrailingNumber(actor->name);
					newActor->name = (number != -1) ? StringUtils::RemoveTrailingNumber(actor->name) + std::to_string(++number) : actor->name + "1";
					this->selectedObject = newActor;
					scene->Add(std::unique_ptr<Actor>(newActor), actor.get());

					done = true;
				}
				if(ImGui::MenuItem("Delete")) {
					scene->Remove(actor.get());
					this->selectedObject = nullptr;
					done = true;
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
			if(done) {
				break;
			}
		}

		// Actor Creator
		ImGui::BeginChild("ActorCreate");
		// Actor Controller
		if(ImGui::BeginPopupContextWindow()) {
			auto cameras = scene->GetComponents<CameraComponent>();
			auto camera = (!cameras.empty()) ? cameras[0] : nullptr;

			if(ImGui::MenuItem("Create Empty")) {
				auto actor = CREATE_CLASS(Actor);
				actor->name = StringUtils::CreateUnique(actor->GetClassName());
				actor->Initialize();

				this->selectedObject = actor.get();
				scene->Add(std::move(actor));
			}
			if(ImGui::MenuItem("Create Sphere")) {
				auto actor = CREATE_CLASS_BASE(Actor, "Sphere");
				actor->name = StringUtils::CreateUnique(actor->name);
				if(camera != nullptr) {
					actor->transform.position = camera->m_owner->transform.position + (camera->m_owner->transform.Forward() * 3.0f);
				}
				actor->Initialize();

				this->selectedObject = actor.get();
				scene->Add(std::move(actor));
			}
			if(ImGui::MenuItem("Create Cube")) {
				auto actor = CREATE_CLASS_BASE(Actor, "Cube");
				actor->name = StringUtils::CreateUnique(actor->name);
				if(camera != nullptr) {
					actor->transform.position = camera->m_owner->transform.position + (camera->m_owner->transform.Forward() * 3.0f);
				}
				actor->Initialize();

				this->selectedObject = actor.get();
				scene->Add(std::move(actor));
			}
			if(ImGui::MenuItem("Create Camera")) {
				auto actor = CREATE_CLASS_BASE(Actor, "Camera");
				actor->name = StringUtils::CreateUnique(actor->name);
				if(camera != nullptr) {
					actor->transform.position = camera->m_owner->transform.position + (camera->m_owner->transform.Forward() * 3.0f);
				}
				actor->Initialize();

				this->selectedObject = actor.get();
				scene->Add(std::move(actor));
			}
			if(ImGui::MenuItem("Create Light")) {
				auto actor = CREATE_CLASS_BASE(Actor, "Light");
				actor->name = StringUtils::CreateUnique(actor->name);
				if(camera != nullptr) {
					actor->transform.position = camera->m_owner->transform.position + (camera->m_owner->transform.Forward() * 3.0f);
				}
				actor->Initialize();

				this->selectedObject = actor.get();
				scene->Add(std::move(actor));
			}

			ImGui::EndPopup();
		}
		ImGui::EndChild();
		ImGui::End();

		// Inspector
		ImGui::Begin("Inspector");
		if(this->selectedObject != nullptr) {
			this->selectedObject->ProcessGUI();
			// Delete Actor
			if(ImGui::IsKeyPressed(ImGuiKey_Delete)) {
				auto actor = dynamic_cast<Actor*>(this->selectedObject);
				if(actor != nullptr) {
					scene->Remove(actor);
					this->selectedObject = nullptr;
				}
			}
		}

		ImGui::PopStyleColor();

		ImGui::End();
	}
}