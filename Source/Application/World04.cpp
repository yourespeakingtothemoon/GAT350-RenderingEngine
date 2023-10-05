#include "World04.h"
#include "Framework/Framework.h"

#include "Input/InputSystem.h"

#define INTERLEAVE 

namespace nc
{
    bool World04::Initialize()
    {

        m_program = GET_RESOURCE(Program, "Shaders/unlit_color.prog");
        m_program->Use();

#ifdef INTERLEAVE
        //vertex time
        float vertexData[] = {
          -0.8f, -0.8f, 0.0f, 1.0f, 1.0f, 0.0f, // yellow
           0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f, // purple
           0.8f,  0.8f, 0.0f,  0.0f, 1.0f, 1.0f, // teal
           -0.8f,  0.8f, 0.0f,  1.0f, 0.0f, 0.0f // red
        };
       


        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

       
        //GLuint vao;
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glBindVertexBuffer(0, vbo, 0, sizeof(GLfloat) * 6);
        
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3);
        glVertexAttribBinding(1, 0);





#else

     
#endif

        return true;
       
        //  std::cout << "dt: " << dt << std::endl;
       // 
      //  velocityX = -1;
     //   velocityY = 1;
    }

    void World04::Shutdown()
    {
    }

    void World04::Update(float dt)
    {
        //m_angle += .1f * dt;
        //m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? .01f * dt : 0;
        //m_position.x -= ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? .01f * dt : 0;
        // m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? 1 : 0;
         //m_position.y -= ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? 1 : 0;
        //m_time += dt;

       // float speed = 0.5f * dt;
   

        
       // m_position.x += velocityX*speed;
       // m_position.y += velocityY*speed;

        // Check boundaries
       
        //if (m_position.x <= -.9f || m_position.x >= .7f) {
        //    velocityX = -velocityX;  // Invert horizontal velocity
        //}

//        if (m_position.y <= -.9f || m_position.y >= .9f) {
  //          velocityY = -velocityY;  // Invert vertical velocity
    //    }
         
        //print position
       // std::cout << "x: " << m_position.x << " y: " << m_position.y << std::endl;
        m_time += dt;

        GLint uniform = glGetUniformLocation(m_program->m_program, "time");
        glUniform1f(uniform, m_time);
    }

    void World04::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        glBindVertexArray(m_vao);
        glDrawArrays(GL_QUADS, 0, 4);

        // post-render
        renderer.EndFrame();
    }
}
