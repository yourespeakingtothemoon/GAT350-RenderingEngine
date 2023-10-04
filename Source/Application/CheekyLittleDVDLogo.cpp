#include "CheekyLittleDVDLogo.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
namespace nc
{
    bool CheekyLittleDVDLogo::Initialize()
    {
        return true;
       
        //  std::cout << "dt: " << dt << std::endl;
       // 
      //  velocityX = -1;
     //   velocityY = 1;
    }

    void CheekyLittleDVDLogo::Shutdown()
    {
    }

    void CheekyLittleDVDLogo::Update(float dt)
    {
        //m_angle += .1f * dt;
        //m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? .01f * dt : 0;
        //m_position.x -= ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? .01f * dt : 0;
        // m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? 1 : 0;
         //m_position.y -= ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? 1 : 0;
        //m_time += dt;

        float speed = 0.5f * dt;
   

        
        m_position.x += velocityX*speed;
        m_position.y += velocityY*speed;

        // Check boundaries
       
        if (m_position.x <= -.9f || m_position.x >= .7f) {
            velocityX = -velocityX;  // Invert horizontal velocity
        }

        if (m_position.y <= -.9f || m_position.y >= .9f) {
            velocityY = -velocityY;  // Invert vertical velocity
        }
         
        //print position
        std::cout << "x: " << m_position.x << " y: " << m_position.y << std::endl;
    }

    void CheekyLittleDVDLogo::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render

        glPushMatrix();
        glLoadIdentity();
        glTranslatef(m_position.x, m_position.y, 0);
        
        //glRotatef(sin(m_angle), 1, 1, 0);
        // glScalef(sin(m_time),0,0);

      //D
        glBegin(GL_TRIANGLES);
        //"hey, heres our list of commnads"

        glColor3f(1, 0, 0);
        glVertex2f(-0.075f, .075f);
        glColor3f(0, 1, 0);
        glVertex2f(.05f, 0);
        glColor3f(0, 0, 1);
        glVertex2f(-0.075f, -0.075f);
      


        //"End of Line"
        glEnd();
        //D hole
        glBegin(GL_TRIANGLES);
        glColor3f(0, 0, 0);
        glVertex2f(-0.05f, .05f);
        glColor3f(0, 0, 0);
        glVertex2f(.01f, 0);
        glColor3f(0, 0, 0);
        glVertex2f(-0.05f, -0.05f);

        glEnd();
       //end of D

    //V
        glBegin(GL_QUADS);
        glColor3f(1, 0, 0);
        glVertex2f(0, .075f);
        
        glColor3f(0, 1, 1);
        glVertex2f(.075f, 0);
    
        glColor3f(0, 0, 1);
        glVertex2f(.15f, 0.075f);

        glColor3f(0, 1, 0);
        glVertex2f(.075f, -0.075f);

        glEnd();

        glBegin(GL_TRIANGLES);
        //"hey, heres our list of commnads"

        glColor3f(0, 0, 1);
        glVertex2f(.15f, .075f);
        glColor3f(0, 1, 0);
        glVertex2f(.265f, 0);
        glColor3f(1, 0, 0);
        glVertex2f(.15f, -0.075f);



        //"End of Line"
        glEnd();
        //D hole
        glBegin(GL_TRIANGLES);
        glColor3f(0, 0, 0);
        glVertex2f(.175f, .05f);
        glColor3f(0, 0, 0);
        glVertex2f(.225f, 0);
        glColor3f(0, 0, 0);
        glVertex2f(0.175f, -0.05f);

        glEnd();
        //end of D

        // post-render
        renderer.EndFrame();
    }
}
