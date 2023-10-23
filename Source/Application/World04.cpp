#include "World04.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include <glm/glm/gtc/type_ptr.hpp>


namespace nc
{
   


    bool World04::Initialize()
    {
        auto material = GET_RESOURCE(Material, "Materials/grid.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
    
        m_model->Load("Models/drachen.fbx");


        m_light.type = light_s::lType::Point;
        m_light.position = glm::vec3{ 0, 8, 0 };
        m_light.direction = glm::vec3{ 0, 1, 0 };
        m_light.color = glm::vec3{ 1, 1, 1 };
        m_light.cutoff = glm::radians(30.0f);


        return true;
    }

    void World04::Shutdown()
    {
    }

    void World04::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0], 0.1f);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0], 0.1f);
        ImGui::DragFloat3("Scale", &m_transform.scale[0], 0.1f);
        ImGui::End();

        ImGui::Begin("Light");
        const char* lightTypes[]{ "Point", "Directional", "Spotlight" };
        ImGui::Combo("Light Type", (int*)&m_light.type, lightTypes, IM_ARRAYSIZE(lightTypes));

       ImGui::DragFloat3("Light Position", &m_light.position[0], 0.1f);
      ImGui::DragFloat3("Light Direction", &m_light.direction[0], 0.1f);
        ImGui::DragFloat("Cutoff", &m_light.cutoff, (ImGuiColorEditFlags)0.1f);
       ImGui::ColorEdit3("Diffuse Color", &m_light.color[0], (ImGuiColorEditFlags)0.1f);
        ImGui::ColorEdit3("Ambient Color", &ambientLight[0], (ImGuiColorEditFlags)0.1f);
        ImGui::End();

        //m_angle += 180 * dt;
        //m_transform.rotation.z += 100 * dt;


        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * dt : 0;

        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * dt : 0;
        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * -dt : 0;

        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_O) ? m_speed * -dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_I) ? m_speed * dt : 0;

        m_time += dt;

        auto material = m_model->GetMaterial();
        material->ProcessGui();
        material->Bind();        

        //Model matrix
        material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        //View matrix
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3 }, glm::vec3{ 0,0,0 }, glm::vec3{ 0,1,0 });
        material->GetProgram()->SetUniform("view", view);

        //Projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 100.0f);
        material->GetProgram()->SetUniform("projection", projection);

        //light type
        material->GetProgram()->SetUniform("light.type", m_light.type);

        //light cutoff
        material->GetProgram()->SetUniform("light.cutoff", m_light.cutoff);

        //light position
        material->GetProgram()->SetUniform("light.position", m_light.position);

        //Light direction
        material->GetProgram()->SetUniform("light.direction", m_light.direction);
        
        //Diffuse light color
        material->GetProgram()->SetUniform("light.color", m_light.color);



        //Ambient light color
        material->GetProgram()->SetUniform("ambientLight", ambientLight);


        

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_model->Draw(); 
        ENGINE.GetSystem<Gui>()->Draw();

        // post-render
        renderer.EndFrame();
    }
}
