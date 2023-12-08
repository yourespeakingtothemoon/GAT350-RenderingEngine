#include "FrameBuffer.h"

#include "Core/Logger.h"
#include "Renderer.h"

namespace nc {
	Framebuffer::~Framebuffer() {
		if(fbo != NULL) {
			glDeleteFramebuffers(1, &(this->fbo));
		}
	}

	bool Framebuffer::Create(std::string filename, ...) {
		va_list args;

		// get texture name
		va_start(args, filename);
		const char* textureName = va_arg(args, const char*);
		va_end(args);

		// get texture resource
		auto texture = GET_RESOURCE(Texture, textureName);

		return CreateFramebuffer(texture);
	}

	bool Framebuffer::CreateFramebuffer(res_t<Texture> texture) {
		this->texture = texture;
		this->size = this->texture->GetSize();

		this->texture->Bind();

		glGenFramebuffers(1, &(this->fbo));
		glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

		// create the depth buffer (bind)
		CreateDepthbuffer(this->size.x, this->size.y);

		// bind the texture to the FBO
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture->texture, 0);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			ERROR_LOG("Error creating frame buffer.");
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return true;
	}

	bool Framebuffer::CreateDepthbuffer(int width, int height) {
		GLuint depthBuffer;
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			ERROR_LOG("Error creating depth buffer.");
			return false;
		}

		return true;
	}

	bool Framebuffer::CreateDepthbuffer(res_t<Texture> texture) {
		this->texture = texture;
		this->size = this->texture->GetSize();
		this->texture->Bind();

		glGenFramebuffers(1, &(this->fbo));
		glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->texture->texture, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			ERROR_LOG("Error creating frame buffer.");
			return false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return true;
	}

	void Framebuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	}

	void Framebuffer::Unbind() {
		glFlush();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}
