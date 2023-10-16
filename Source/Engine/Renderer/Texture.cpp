#include "Texture.h"
#include "Renderer.h"
#include "Core/Logger.h"
#include <SDL2-2.28.4/include/SDL_image.h>

namespace nc
{
	Texture::~Texture()
	{
		if (m_texture) SDL_DestroyTexture(m_texture);
	}

	bool Texture::Create(std::string filename, ...)
	{
		va_list args;

		va_start(args, filename);

		Renderer& renderer = va_arg(args, Renderer);

		va_end(args);

		return Load(filename, renderer);
	}

	bool Texture::Load(const std::string& filename, Renderer& renderer)
	{
		SDL_Surface* surface = IMG_Load(filename.c_str());
		if (!surface)
		{
			WARNING_LOG("Could not create surface: " << filename);
			return false;
		}

		m_texture = SDL_CreateTextureFromSurface(renderer.m_renderer, surface);
		SDL_FreeSurface(surface);
		if (!m_texture)
		{
			WARNING_LOG("Could not create texture: " << filename);
			return false;
		}

		glGenTextures(1, &m_texture);
		glBindTexture(m_target, m_texture);
		
		GLenum internalFormat = (channels == 4) ? GL_RGBA : GL_RGB;
GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

glTexStorage2D(m_target,1,internalFormat,m_size.x,m_size.y);
glTexSubImage2D(m_target,0,0,0,m_size.x,m_size.y,format,GL_UNSIGNED_BYTE,m_data.data());

glTexParameteri(m_target,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
glTexParameteri(m_target,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

//wrap
glTexParameteri(m_target,GL_TEXTURE_WRAP_S,GL_REPEAT);
glTexParameteri(m_target,GL_TEXTURE_WRAP_T,GL_REPEAT);



	stbi_image_free(data);

		return true;
	}

	vec2 Texture::GetSize()
	{
		SDL_Point point;
		SDL_QueryTexture(m_texture, nullptr, nullptr, &point.x, &point.y);

		return vec2{ point.x, point.y };
	}


}