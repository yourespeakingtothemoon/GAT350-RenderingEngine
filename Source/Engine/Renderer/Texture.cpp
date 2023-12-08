#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Core/Logger.h"
#include "Renderer.h"

namespace nc {
	Texture::~Texture() {
		if(this->texture != NULL) {
			glDeleteTextures(1, &texture);
		}
	}

	bool Texture::Create(std::string filename, ...) {
		va_list args;

		va_start(args, filename);

		Renderer& renderer = va_arg(args, Renderer);

		va_end(args);

		return Load(filename, renderer);
	}

	bool Texture::CreateTexture(int width, int height, GLuint filteringMode) {
		this->target = GL_TEXTURE_2D;
		this->size = glm::vec2(width, height);

		glGenTextures(1, &(this->texture));
		glBindTexture(this->target, this->texture);

		// create texture (width, height)
		glTexImage2D(this->target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		// set texture parameters
		glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, filteringMode);
		glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, filteringMode);

		return true;
	}

	bool Texture::CreateDepthTexture(int width, int height, GLuint filteringMode) {
		this->target = GL_TEXTURE_2D;
		this->size = glm::vec2(width, height);

		glGenTextures(1, &(this->texture));
		glBindTexture(this->target, this->texture);

		// create texture (width, height)
		glTexImage2D(this->target, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		// set texture parameters
		glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, filteringMode);
		glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, filteringMode);

		glTexParameteri(this->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(this->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		GLfloat border[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glTexParameterfv(this->target, GL_TEXTURE_BORDER_COLOR, border);

		return true;
	}

	bool Texture::Load(const std::string& filename, Renderer& renderer) {
		int channels = 0;

		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(filename.c_str(), &(this->size.x), &(this->size.y), &channels, 0);

		if(data == nullptr) {
			WARNING_LOG("Could not create surface: " << filename);
			return false;
		}

		glGenTextures(1, &(this->texture));
		glBindTexture(this->target, this->texture);

		GLenum internalFormat = (channels == 4) ? GL_RGBA8 : GL_RGB8;
		GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

		glTexStorage2D(this->target, 1, internalFormat, this->size.x, this->size.y);
		glTexSubImage2D(this->target, NULL, 0, 0, this->size.x, this->size.y, format, GL_UNSIGNED_BYTE, data);

		glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(this->target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(this->target, GL_TEXTURE_WRAP_T, GL_REPEAT);

		stbi_image_free(data);

		return true;
	}

	void Texture::ProcessGUI() {
		ImGui::TextColored(ImVec4{0, 1, 0, 1}, "Name: %s", name.c_str());
		ImGui::Text("Size: %d x %d", this->size.x, this->size.y);
		ImGui::Separator();
		ImVec2 size = ImVec2(256, 256);
		ImGui::Image((void*) (intptr_t) this->texture, size);
	}
}