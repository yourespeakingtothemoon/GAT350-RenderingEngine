#pragma once

#include "Framework/Resource/Resource.h"

#include <glad/include/glad/glad.h>
#include <glm/glm/glm.hpp>

namespace nc {
	class Texture;

	class Framebuffer : public Resource {
		public:
			Framebuffer() = default;
			~Framebuffer();

			bool Create(std::string filename, ...) override;
			bool CreateFramebuffer(res_t<Texture> texture);
			bool CreateDepthbuffer(int width, int height);
			bool CreateDepthbuffer(res_t<Texture> texture);

			void Bind();
			void Unbind();

			glm::ivec2 GetSize() const {return size;}

		protected:
			GLuint fbo = 0;
			res_t<Texture> texture;
			glm::ivec2 size = glm::ivec2(0);
	};
}
