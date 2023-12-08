#include "Scene.h"

#include "Framework/Components/LightComponent.h"
#include "Framework/Components/CameraComponent.h"

namespace nc
{
	bool Scene::Initialize()
	{
		for (auto& actor : m_actors) actor->Initialize();

		return true;
	}

	void Scene::Update(float dt)
	{
		this->deltaTime = dt;
		// update and remove destroyed actors
		auto iter = m_actors.begin();
		while (iter != m_actors.end())
		{
			if ((*iter)->active) (*iter)->Update(dt);
			((*iter)->destroyed) ? iter = m_actors.erase(iter) : iter++;
		}
	}

	void Scene::Draw(Renderer& renderer) {
		// get light components
		std::vector<LightComponent*> lights = GetComponents<LightComponent>();

		// get camera components
		std::vector<CameraComponent*> cameras = GetComponents<CameraComponent>();

		// get first active camera component
		CameraComponent* camera = (!cameras.empty()) ? cameras[0] : nullptr;

		// get all shader programs in the resource system
		auto programs = GET_RESOURCES(Program);
		// set all shader programs camera and lights uniforms
		for(auto& program : programs) {
			program->Use();

			// set camera in shader program
			if(camera) {
				camera->SetProgram(program);
			}

			// set lights in shader program
			int index = 0;
			for(auto light : lights) {
				std::string name = "lights[" + std::to_string(index++) + "]";

				glm::mat4 view = (camera != nullptr) ? camera->view : glm::mat4(1);
				light->SetProgram(program, name, view);
			}

			program->SetUniform("numLights", index);
			program->SetUniform("ambientLight", ambientColor);
		}

		for(auto& actor : m_actors) {
			if(actor->active) actor->Draw(renderer);
		}
	}

	void Scene::Add(std::unique_ptr<Actor> actor, Actor* prevActor)
	{
		actor->m_scene = this;
		actor->m_game = m_game;

		// Check if previous actor pointer provided
		if(prevActor != nullptr) {
			// Find previous actor iterator
			auto iter = std::find_if(m_actors.begin(), m_actors.end(), [prevActor](auto& actor) {return actor.get() == prevActor;});

			// If previous actor found, set iterator to next element
			iter = (iter != m_actors.begin()) ? std::next(iter) : iter;
			// Insert new actor (before iterator)
			m_actors.insert(iter, std::move(actor));
		} else {
			// Previous actor pointer not provided, add to back of the container
			m_actors.push_back(std::move(actor));
		}
	}

	void Scene::Remove(Actor* actor) {
		auto iter = m_actors.begin();
		while(iter != m_actors.end()) {
			if((*iter).get() == actor) {
				m_actors.erase(iter);
				break;
			}
			iter++;
		}
	}

	void Scene::RemoveAll(bool force)
	{
		auto iter = m_actors.begin();
		while (iter != m_actors.end())
		{
			(force || !(*iter)->persistent) ? iter = m_actors.erase(iter) : iter++;
		}
	}

	bool Scene::Load(const std::string& filename)
	{
		rapidjson::Document document;
		if (!Json::Load(filename, document))
		{
			ERROR_LOG("Could not load scene file: " << filename);
			return false;
		}

		Read(document);

		return true;
	}

	void Scene::Read(const json_t& value)
	{
		if (HAS_DATA(value, actors) && GET_DATA(value, actors).IsArray())
		{
			for (auto& actorValue : GET_DATA(value, actors).GetArray())
			{
				std::string type;
				READ_DATA(actorValue, type);

				auto actor = CREATE_CLASS_BASE(Actor, type);
				actor->Read(actorValue);

				if (actor->prototype)
				{
					std::string name = actor->name;
					Factory::Instance().RegisterPrototype(name, std::move(actor));
				}
				else
				{
					Add(std::move(actor));
				}
			}
		}

	}

	void Scene::ProcessGUI() {
		float fps = 1.0f / this->deltaTime;
		float ms = 1000.0f * this->deltaTime;

		ImVec4 color = (fps < 30) ? ImVec4(1, 0, 0, 1) : ImVec4(1, 1, 1 ,1);
		ImGui::TextColored(color, "%.2f FPS (%.2f)", fps, ms);
		ImGui::ColorEdit3("Ambient", glm::value_ptr(ambientColor));
	}
}