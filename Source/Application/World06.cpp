#include "World06.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

#define INTERLEAVE 


namespace nc
{
    bool World06::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/ComponentsScene.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateTexture(512, 512);
        ADD_RESOURCE("fb_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/framebuffer.mtrl");
        if (material)
        {
            material->albedoTexture = texture;
        }

        return true;
    }

    void World06::Shutdown()
    {
    }

    void World06::Update(float dt)
    {
        //GUI!!!!!
        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();

        //set ppgui
        ImGui::Begin("Post Process Effect");
        ImGui::SliderFloat("Blend", &m_blend, 0, 1);
        bool effect = params & INVERT_MASK;
        if (ImGui::Checkbox("Invert", &effect))
        {
            effect ? params |= INVERT_MASK : params ^= INVERT_MASK;
			
        }
        effect = params & GREYSCALE_MASK;
        if (ImGui::Checkbox("Greyscale", &effect))
        {
			effect ? params |= GREYSCALE_MASK : params ^= GREYSCALE_MASK;
		}
        effect = params & COLORTINT_MASK;
        if (ImGui::Checkbox("Color Tint", &effect))
        {

            effect ? params |= COLORTINT_MASK : params ^= COLORTINT_MASK;
        }
        
        ImGui::End();

        //set pp shader
        auto program = GET_RESOURCE(Program, "shaders/postprocess.prog");
            if (program)
            {
                program->Use();
                program->SetUniform("blend", m_blend);
                program->SetUniform("params",params);
            }

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World06::Draw(Renderer& renderer)
    {
        // *** pass 1 ***
        m_scene->GetActorByName("postprocess")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.BeginFrame({1,0,0});
        m_scene->Draw(renderer);

        framebuffer->Unbind();

        // *** pass 2 ***
        m_scene->GetActorByName("postprocess")->active = true;

        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->Draw(renderer);

        // post-render
        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}
