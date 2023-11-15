
#include "World06Old.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

#define INTERLEAVE 


    namespace nc
    {
        bool World06Old::Initialize()
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

        void World06Old::Shutdown()
        {
        }

        void World06Old::Update(float dt)
        {
            //GUI!!!!!
            ENGINE.GetSystem<Gui>()->BeginFrame();

            m_scene->Update(dt);
            m_scene->ProcessGui();

            ENGINE.GetSystem<Gui>()->EndFrame();
        }

        void World06Old::Draw(Renderer& renderer)
        {
            // *** pass 1 ***
            m_scene->GetActorByName("cube")->active = false;

            auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
            renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
            framebuffer->Bind();

            renderer.BeginFrame({ 1,0,0 });
            m_scene->Draw(renderer);

            framebuffer->Unbind();

            // *** pass 2 ***
            m_scene->GetActorByName("cube")->active = true;

            renderer.ResetViewport();
            renderer.BeginFrame();
            m_scene->Draw(renderer);

            // post-render
            ENGINE.GetSystem<Gui>()->Draw();
            renderer.EndFrame();
        }
    }

