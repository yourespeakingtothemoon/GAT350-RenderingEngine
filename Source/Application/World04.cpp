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

        for (int i = 0; i < 3; i++)
        {
            m_lights[i].type = light_s::lType::Point;
            m_lights[i].position = glm::vec3{ 0, 0, 0 };
            m_lights[i].direction = glm::vec3{ 0, 0, 0 };
            m_lights[i].color = glm::vec3{ 1, 1, 1 };
            m_lights[i].cutoff = glm::radians(30.0f);
            m_lights[i].innerCutoff = 15.0f;
            m_lights[i].outerCutoff = 30.0f;
            m_lights[i].intensity = 1.0f;
            m_lights[i].range = 1.0f;
        }

        return true;
    }

    void World04::Shutdown()
    {
    }

    void World04::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();




        ImGui::Begin("Light");
        ImGui::Combo ("Light", &selectedLight, "Light 1\0Light 2\0Light 3\0");
        const char* lightTypes[]{ "Point", "Directional", "Spotlight" };
        ImGui::Combo("Light Type", (int*)&m_lights[selectedLight].type, lightTypes, IM_ARRAYSIZE(lightTypes));

       ImGui::DragFloat3("Light Position", &m_lights[selectedLight].position[0], 0.1f);
      ImGui::DragFloat3("Light Direction", &m_lights[selectedLight].direction[0], 0.1f);
      //  ImGui::DragFloat("Cutoff", &m_light.cutoff, (ImGuiColorEditFlags)0.1f);
       ImGui::ColorEdit3("Diffuse Color", &m_lights[selectedLight].color[0], (ImGuiColorEditFlags)0.1f);
       if (m_lights[selectedLight].type != light_s::lType::Directional)
       {
           ImGui::DragFloat("Range", &m_lights[selectedLight].range, 0.1f, 0.1f, 100.0f);
       }
        ImGui::ColorEdit3("Ambient Color", &ambientLight[0], (ImGuiColorEditFlags)0.1f);

        if (m_lights[selectedLight].type == light_s::lType::Spotlight)
        {
			ImGui::DragFloat("Inner Cutoff", &m_lights[selectedLight].innerCutoff, 0.1f,0,m_lights[selectedLight].outerCutoff);
			ImGui::DragFloat("Outer Cutoff", &m_lights[selectedLight].outerCutoff, 0.1f,m_lights[selectedLight].innerCutoff);
		}

        ImGui::DragFloat("Intensity", &m_lights[selectedLight].intensity, 0.1f,0,10);
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
        for (int i = 0; i < 3; i++)
        {

            std::string name = "lights[" + std::to_string(i) + "]";
            //light type
            material->GetProgram()->SetUniform(name+".type", m_lights[i].type);

            //light cutoff
            material->GetProgram()->SetUniform(name+".innerAngle", m_lights[i].innerCutoff);
            material->GetProgram()->SetUniform(name+".outerAngle", m_lights[i].outerCutoff);
                                             
            //light intensity                  name+
            material->GetProgram()->SetUniform(name+".intensity", m_lights[i].intensity);
                                             
            //light range                      name+
            material->GetProgram()->SetUniform(name+".range", m_lights[i].range);
                                            
            //light position                   name+
            material->GetProgram()->SetUniform(name+".position", m_lights[i].position);
                                              
            //Light direction                  name+
            material->GetProgram()->SetUniform(name+".direction", m_lights[i].direction);
                                              
            //Diffuse light color              name+
            material->GetProgram()->SetUniform(name+".color", m_lights[i].color);



            


        }
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
