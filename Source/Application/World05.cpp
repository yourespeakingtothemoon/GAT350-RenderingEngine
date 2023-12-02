#include "World05.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>


namespace nc {
    bool World05::Initialize() {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("scenes/scene.json");
        m_scene->Initialize();
        /* {
            auto actor = CREATE_CLASS(Actor);
            actor->name = "actor1";
            actor->transform.position = glm::vec3{ 0, 0, 0 };
            auto modelComponent = CREATE_CLASS(ModelComponent);
            modelComponent->m_model = std::make_shared<Model>();
            modelComponent->m_model->SetMaterial(GET_RESOURCE(Material, "materials/squirrel.mtrl"));
            modelComponent->m_model->Load("models/squirrel.glb", glm::vec3{ 0, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });
            actor->AddComponent(std::move(modelComponent));
            m_scene->Add(std::move(actor));
        } */

        {
            auto actor = CREATE_CLASS(Actor);
            actor->name = "light1";
            actor->transform.position = glm::vec3{ 3, 3, 3 };
            auto lightComponent = CREATE_CLASS(LightComponent);
            lightComponent->type = LightComponent::eType::Point;
            //lightComponent->color = glm::rgbColor(glm::vec3{ randomf() * 360, 1, 1 }); // random color
            lightComponent->color = glm::vec3{ 1, 1, 1 }; // set white
            lightComponent->intensity = 1;
            lightComponent->range = 20;
            lightComponent->innerAngle = 10.0f;
            lightComponent->outerAngle = 30.0f;
            actor->AddComponent(std::move(lightComponent));
            m_scene->Add(std::move(actor));

            actor = CREATE_CLASS(Actor);
            actor->name = "camera1";
            actor->transform.position = glm::vec3{ 0, 0, 2 };
            actor->transform.rotation = glm::radians(glm::vec3{ 0, 180, 0 });

            auto cameraComponent = CREATE_CLASS(CameraComponent);
            cameraComponent->SetPerspective(70.0f, (float)ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.1f, 100.0f);
            actor->AddComponent(std::move(cameraComponent));

            auto cameraController = CREATE_CLASS(CameraController);
            cameraController->speed = 5;
            cameraController->sensitivity = 0.5f;
            cameraController->m_owner = actor.get();
            cameraController->Initialize();
            actor->AddComponent(std::move(cameraController));

            m_scene->Add(std::move(actor));
        }

        for (int i = 0; i < 2; i++) {
            auto actor = CREATE_CLASS_BASE(Actor, "tree");
            actor->name = CreateUnique("tree");
            actor->transform.position = glm::vec3{ randomf(-10, 10), 0, randomf(-10, 10) };
            actor->transform.scale = glm::vec3{ randomf(0.5f, 3.0f), randomf(0.5f, 3.0f), 0 };
            actor->Initialize();
            m_scene->Add(std::move(actor));
        }
        return true;
    }

    void World05::Shutdown() {
    }

    void World05::Update(float dt) {
        ENGINE.GetSystem<Gui>()->BeginFrame();
        m_scene->Update(dt);
        m_scene->ProcessGui();
        
        //m_angle += dt * 3;
        //m_transform.rotation.z += 30 * dt;
        // Model Movement
        auto actor = m_scene->GetActorByName<Actor>("actor1");
        /*actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : 0;
        actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed *  dt : 0;
        actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * -dt : 0;
        actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed *  dt : 0;
        // Model Rotation
        actor->transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_UP) ? m_rotateSpeed * -dt : 0;
        actor->transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_DOWN) ? m_rotateSpeed *  dt : 0;
        actor->transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_LEFT) ? m_rotateSpeed * -dt : 0;
        actor->transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_RIGHT) ? m_rotateSpeed *  dt : 0;
        actor->transform.rotation.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? m_rotateSpeed * -dt : 0;
        actor->transform.rotation.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? m_rotateSpeed * dt : 0;
        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_R)) {
            actor->transform.position.x = 0.0f;
            actor->transform.position.y = 0.0f;
            actor->transform.position.z = 0.0f;
            actor->transform.rotation.x = 0.0f;
            actor->transform.rotation.y = 0.0f;
            actor->transform.rotation.z = 0.0f;
            actor->transform.scale.x = 1.0f;
            actor->transform.scale.y = 1.0f;
            actor->transform.scale.z = 1.0f;
        } */
        m_time += dt;

        auto material = actor->GetComponent<ModelComponent>()->m_material;
        material->ProcessGui();
        material->Bind();

        material = GET_RESOURCE(Material, "Materials/refraction.mtrl");
        if (material) {
            ImGui::Begin("Refraction");
            ImGui::DragFloat("IOR", &m_refraction, 0.01f, 1, 3);
            auto program = material->GetProgram();
            program->Use();
            program->SetUniform("ior", m_refraction);
            ImGui::End();
        }

        
        //material->GetProgram()->SetUniform("light.ambientLight", m_ambientLight); 
        

        // model matrix
        //material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        //// view matrix
        //glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });
        //material->GetProgram()->SetUniform("view", view);

        //// projection matrix
        //glm::mat4 projection = glm::perspective(glm::radians(70.0f), ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 100.0f);
        //material->GetProgram()->SetUniform("projection", projection);
        //ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World05::Draw(Renderer& renderer) {
        // pre-render
        renderer.BeginFrame();

        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_scene->Draw(renderer);

        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();
    }
}
