#include "Cubemap.h"

#include <cstdarg>
#include <array>

#include <stb/stb_image.h>
#include <SDL_image.h>
#include <SDL.h>

#include "Core/Logger.h"
#include "Renderer.h"

namespace nc {
	bool Cubemap::Create(std::string filename, ...) {
		// va_list - type to hold information about variable arguments
		va_list args;

		// va_start - enables access to variadic function arguments
		va_start(args, filename);

		// va_arg - accesses the next variadic function arguments
		std::vector<std::string> filenames = va_arg(args, std::vector<std::string>);

		// va_end - ends traversal of the variadic function arguments
		va_end(args);

		// create textures (returns true/false if successful)
		return Load(filenames);
	}


	bool Cubemap::Load(const std::vector<std::string>& filenames) {
		this->target = GL_TEXTURE_CUBE_MAP;

		glGenTextures(1, &(this->texture));
		glBindTexture(this->target, this->texture);

		GLuint targets[] = {
			GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
			GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		};

		for(size_t i = 0; i < filenames.size(); i++) {
			int channels = 0;
			unsigned char* data = stbi_load(filenames[i].c_str(), &(this->size.x), &(this->size.y), &channels, 0);
			if(!data) {
				WARNING_LOG("Could not create surface: " << filenames[i]);
				return false;
			}

			GLenum internalFormat = (channels == 4) ? GL_RGBA8 : GL_RGB8;
			GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

			glTexImage2D(targets[i], 0, GL_RGB, this->size.x, this->size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}

		glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(this->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(this->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(this->target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return true;
	}
}
