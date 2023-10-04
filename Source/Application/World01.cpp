#include "World01.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

namespace nc
{
    bool World01::Initialize()
    {
        return true;
    }

    void World01::Shutdown()
    {
    }

    void World01::Update(float dt)
    {
        m_angle += .1f*dt;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? .01f* dt : 0;
        m_position.x -= ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? .01f* dt : 0;
       // m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? 1 : 0;
        //m_position.y -= ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? 1 : 0;
        m_time += dt;

    }

    void World01::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render

        glPushMatrix();
        glTranslatef(m_position.x, m_position.y,0);
        glRotatef(sin(m_angle), 1, 1, 0);
       // glScalef(sin(m_time),0,0);

        glBegin(GL_TRIANGLES);
        //"hey, heres our list of commnads"

        //dancing tri
        
        //the dancing triangle
        glColor3f(1, 0, 0);
        glVertex2f(-0.5f, -0.5f);
        glColor3f(0,1,0);
        glVertex2f(.5f, -0.5f);
        glColor3f(0, 0, 1);
        glVertex2f(-0.5f, 0.5f);
        //end of dancing triangle


        //"End of Line"
        glEnd();

        // post-render
        renderer.EndFrame();
    }
}
