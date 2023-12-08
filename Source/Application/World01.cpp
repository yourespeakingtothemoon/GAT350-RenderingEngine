#include "World01.h"

#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

namespace nc {
	bool World01::Initialize() {
		return true;
	}

	void World01::Shutdown() {
	}

	void World01::Update(float deltaTime) {
		this->position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? deltaTime : 0;
		this->position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -deltaTime : 0;

		this->position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? deltaTime : 0;
		this->position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? -deltaTime : 0;

		this->angle += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_LEFT) ? deltaTime * 90 : 0;
		this->angle += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_RIGHT) ? -deltaTime * 90 : 0;

		this->time += deltaTime;
	}

	void World01::Draw(Renderer& renderer) {
		// pre-render
		renderer.BeginFrame();

		// render
		glPushMatrix();

		glTranslatef(this->position.x, this->position.y, 0.0f);
		glRotatef(this->angle, 0.0f, 0.0f, 1.0f);
		glScalef(Clamp(((std::sinf(this->time * 5) + 1) * 0.5f), 0.01f, 1.0f), 1.0f, 1.0f);

		glBegin(GL_QUADS);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(-0.5f, -0.5f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(-0.5f, 0.5f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(0.5f, 0.5f);

		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex2f(0.5f, -0.5f);

		glEnd();

		glPopMatrix();

		glPushMatrix();

		glTranslatef(0, 0.64f, 0);

		glBegin(GL_POLYGON);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(-0.8f, -0.25f);

		glColor3f(0.5f, 0.0f, 1.0f);
		glVertex2f(-0.6f, -0.25f);

		glColor3f(0.5f, 0.0f, 1.0f);
		glVertex2f(-0.45f, 0.0f);

		glColor3f(0.5f, 0.0f, 1.0f);
		glVertex2f(-0.6f, 0.25f);

		glColor3f(0.5f, 0.0f, 1.0f);
		glVertex2f(-0.8f, 0.25f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(-0.95f, 0.0f);

		glEnd();

		glPopMatrix();

		// post-render
		renderer.EndFrame();
	}
}
