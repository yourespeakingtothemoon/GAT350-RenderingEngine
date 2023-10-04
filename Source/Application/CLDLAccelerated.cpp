#include "CLDLAccelerated.h"
#include "Framework/Framework.h"

#include "Input/InputSystem.h"
namespace nc
{
    bool CLDLAccelerated::Initialize()
    {
        //shaderz
        const char* vertexShader =
            "#version 430\n"
            "layout (location=0) in vec3 position;"
            "layout (location=1) in vec3 color;"
            "layout (location=0) out vec3 ocolor;"
            "void main() {"
            "  ocolor = color;"
            "  gl_Position = vec4(position, 1.0);"
            "}";



        const char* fragmentShader =
            "#version 430\n"
            "layout (location=0) in vec3 color;"
            "out vec4 ocolor;"
            "void main() {"
            "  ocolor = vec4(color, 1);"
            "}";

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertexShader, NULL);
        glCompileShader(vs);

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragmentShader, NULL);
        glCompileShader(fs);


        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vs);
        glAttachShader(shaderProgram, fs);
        glLinkProgram(shaderProgram);
        glUseProgram(shaderProgram);


        //vertex time
        float positionData[] = {
          -0.8f, -0.8f, 0.0f,
           0.8f, -0.8f, 0.0f,
           0.0f,  0.8f, 0.0f
        };
        //color time
        float colorData[] = {
            1.0f, 1.0f, 0.0f, // orange
            1.0f, 0.0f, 1.0f, // purple
            0.0f, 1.0f, 1.0f  // teal
        };
       

        GLuint vbo[2];
        glGenBuffers(2, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

        //GLuint vao;
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexBuffer(0, vbo[0], 0, sizeof(GLfloat) * 3);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexBuffer(1, vbo[1], 0, sizeof(GLfloat) * 3);

        
       

        


        return true;
       
        //  std::cout << "dt: " << dt << std::endl;
       // 
      //  velocityX = -1;
     //   velocityY = 1;
    }

    void CLDLAccelerated::Shutdown()
    {
    }

    void CLDLAccelerated::Update(float dt)
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
       // std::cout << "x: " << m_position.x << " y: " << m_position.y << std::endl;
    }

    void CLDLAccelerated::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // post-render
        renderer.EndFrame();
    }
}
